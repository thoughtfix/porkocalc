#include "ticker.h"

#include <Arduino.h>
#include <SD.h>

#include "display.h"                 // COLOR_FG/BG, sceneBottomOffset
#include "../core/network_recon.h"   // WiFi AP list
#include "../modes/oink.h"           // DetectedNetwork definition
#include "../core/config.h"          // GPS enable, SD availability
#include "../gps/gps.h"              // GPS fix
#include "../piglet/avatar.h"        // pig gets excited on a GPS fix

namespace Ticker {

namespace {
constexpr int MAX_ENTRIES = 16;           // cap so the string and the read stay cheap
// Detection scrolls ~30% faster than status, so the two lines are deliberately asymmetric instead
// of looking like they're racing to keep pace with each other.
constexpr float SCROLL_DETECTION_PX_PER_S = 34.0f;
constexpr float SCROLL_STATUS_PX_PER_S = 26.0f;
constexpr int GAP_PX = 48;                // gap between marquee repeats
constexpr int ROW_H = 18;                 // line pitch (Font2 is ~16px)
const lgfx::IFont* TICKER_FONT = &fonts::Font2;  // single line, double-tall

// One independent scroll position + cached text per marquee.
struct Marquee {
    String text;
    float scrollX = 0.0f;
    uint32_t lastBuildMs = 0;
    uint32_t lastDrawMs = 0;
};
Marquee detectionM;
Marquee statusM;
Marquee gpsM;

// GPS trivia shown while the module is still searching for a fix. No "~": on a small screen the
// tilde reads as a garbage character, so spell it "about".
const char* const GPS_TRIVIA[] = {
    "GPS sats orbit about 20,200 km up",
    "a 3D fix needs 4 or more satellites",
    "GPS time ignores leap seconds",
    "about 31 GPS satellites circle Earth",
    "a cold start can take a few minutes",
    "clear sky means a faster lock",
    "the signal travels about 20,000 km to you",
    "GPS is free and worldwide",
};
constexpr int GPS_TRIVIA_COUNT = sizeof(GPS_TRIVIA) / sizeof(GPS_TRIVIA[0]);

bool gpsUserHidden = false;  // toggled by the G key; independent of auto-detect

// Build the GPS line for a given trivia index. The trivia only changes between full marquee cycles
// (the caller advances triviaIdx), and every trivia is padded to a fixed width, so the text a
// viewer sees never changes or shifts while it's on screen.
String buildGps(int triviaIdx) {
    GPSData d = GPS::getData();
    if (GPS::hasFix() && d.fix) {
        char buf[128];
        snprintf(buf, sizeof(buf),
                 "GPS FIX   %.5f, %.5f   sats:%u   alt:%.0fm   %.1f km/h",
                 d.latitude, d.longitude, (unsigned)d.satellites, d.altitude, d.speed);
        return String(buf);
    }
    static int maxTrivia = 0;
    if (maxTrivia == 0) {
        for (int i = 0; i < GPS_TRIVIA_COUNT; i++) {
            int len = (int)strlen(GPS_TRIVIA[i]);
            if (len > maxTrivia) maxTrivia = len;
        }
    }
    char padded[64];
    snprintf(padded, sizeof(padded), "%-*s", maxTrivia, GPS_TRIVIA[triviaIdx % GPS_TRIVIA_COUNT]);
    char buf[128];
    snprintf(buf, sizeof(buf), "GPS SEARCHING   sats used:   %u   *   %s", (unsigned)d.satellites, padded);
    return String(buf);
}

const char* modeName(PorkchopMode mode) {
    switch (mode) {
        case PorkchopMode::IDLE: return "IDLE";
        case PorkchopMode::OINK_MODE: return "OINK";
        case PorkchopMode::DNH_MODE: return "DO-NO-HAM";
        case PorkchopMode::WARHOG_MODE: return "WARHOG";
        case PorkchopMode::PIGGYBLUES_MODE: return "PIGGYBLUES";
        case PorkchopMode::SPECTRUM_MODE: return "SPECTRUM";
        case PorkchopMode::BACON_MODE: return "BACON";
        default: return "PORKOCALC";
    }
}

String buildWifi() {
    uint16_t n = NetworkRecon::getNetworkCount();
    if (n == 0) return "SCANNING FOR NETWORKS...";
    auto& nets = NetworkRecon::getNetworks();
    if (nets.empty()) return "SCANNING FOR NETWORKS...";
    String s;
    int shown = 0;
    for (size_t i = 0; i < nets.size() && shown < MAX_ENTRIES; i++) {
        const DetectedNetwork& net = nets[i];
        const char* ssid = (net.ssid[0] != '\0') ? net.ssid : "<hidden>";
        char buf[64];
        snprintf(buf, sizeof(buf), "%s  ch:%u  %ddBm", ssid, (unsigned)net.channel, (int)net.rssi);
        if (shown) s += "    *    ";
        s += buf;
        shown++;
    }
    return s;
}

String buildStatus(PorkchopMode mode) {
    uint32_t up = millis() / 1000;
    char buf[224];

    // Battery, via the shared helper so the ticker and top bar always agree (N/A on the old
    // hardcoded-100% STM32 firmware).
    char battField[16];
    snprintf(battField, sizeof(battField), "BATT:%s", batteryFieldStr(M5.Power.getBatteryLevel()));

    // SD free space, only if a card is mounted.
    char sdField[28] = "SD CARD:--";
    if (Config::isSDAvailable()) {
        uint64_t freeMB = (SD.totalBytes() - SD.usedBytes()) / (1024ULL * 1024ULL);
        snprintf(sdField, sizeof(sdField), "SD CARD:%lluMB", (unsigned long long)freeMB);
    }
    snprintf(buf, sizeof(buf),
             "MODE:%s      UPTIME:%luh%02lum%02lus      %s      HEAP:%luK      PSRAM:%luK      %s",
             modeName(mode),
             (unsigned long)(up / 3600), (unsigned long)((up % 3600) / 60), (unsigned long)(up % 60),
             battField,
             (unsigned long)(ESP.getFreeHeap() / 1024),
             (unsigned long)(ESP.getFreePsram() / 1024),
             sdField);
    return String(buf);
}

// Draw + advance one marquee at the given text row. Returns true on the frame the marquee finishes
// a full cycle (wraps), so callers can swap content between cycles instead of mid-scroll.
bool drawMarquee(M5Canvas& canvas, Marquee& m, int row, float speedPxPerS) {
    if (m.text.isEmpty()) return false;
    uint32_t now = millis();
    float dt = (m.lastDrawMs == 0) ? 0.0f : (now - m.lastDrawMs) / 1000.0f;
    m.lastDrawMs = now;

    canvas.setFont(TICKER_FONT);
    canvas.setTextSize(1);
    canvas.setTextDatum(TL_DATUM);

    int span = canvas.textWidth(m.text) + GAP_PX;
    if (span <= 0) return false;
    bool wrapped = false;
    m.scrollX += speedPxPerS * dt;
    while (m.scrollX >= span) { m.scrollX -= span; wrapped = true; }

    int y = 3 + row * ROW_H;
    canvas.setTextColor(COLOR_FG);
    int x = -(int)m.scrollX;
    while (x < (int)canvas.width()) {
        canvas.drawString(m.text, x, y);
        x += span;
    }
    canvas.setFont(&fonts::Font0);
    canvas.setTextSize(1);
    return wrapped;
}
}  // namespace

void toggleGps() { gpsUserHidden = !gpsUserHidden; }

void drawDetection(M5Canvas& canvas, PorkchopMode mode, int row) {
    if (sceneBottomOffset(canvas) <= 0) return;  // fullscreen layouts only
    uint32_t now = millis();
    if (now - detectionM.lastBuildMs >= 1000 || detectionM.text.isEmpty()) {
        detectionM.lastBuildMs = now;
        detectionM.text = buildWifi();
    }
    drawMarquee(canvas, detectionM, row, SCROLL_DETECTION_PX_PER_S);
}

void drawStatus(M5Canvas& canvas, PorkchopMode mode, int row) {
    if (sceneBottomOffset(canvas) <= 0) return;
    uint32_t now = millis();
    if (now - statusM.lastBuildMs >= 1000 || statusM.text.isEmpty()) {
        statusM.lastBuildMs = now;
        statusM.text = buildStatus(mode);
    }
    drawMarquee(canvas, statusM, row, SCROLL_STATUS_PX_PER_S);
}

bool drawGps(M5Canvas& canvas, PorkchopMode mode, int row) {
    (void)mode;
    if (sceneBottomOffset(canvas) <= 0) return false;
    if (gpsUserHidden) return false;          // manual G toggle
    if (!Config::gps().enabled) return false; // disabled in settings
    // Auto-detect: hide the GPS line entirely if no receiver is streaming. charsProcessed() stays 0
    // until real NMEA arrives, so this appears a beat after boot when a module is present and never
    // appears when it isn't.
    if (GPS::charsProcessed() == 0) return false;

    // Nudge the pig to EXCITED the moment a fix first lands.
    static bool prevFix = false;
    bool fixNow = GPS::hasFix();
    if (fixNow && !prevFix) {
        Avatar::setState(AvatarState::EXCITED);
        Avatar::cuteJump();
    }
    prevFix = fixNow;

    // Single-pass scroll: the line enters from the right, crosses, and exits left; only once it is
    // fully off the left edge do we pick the next trivia and restart from the right. So the text a
    // viewer sees never changes on screen -- the swap always happens before the first pixel is drawn.
    // (A fix-status flip rebuilds immediately so a lock shows promptly.)
    static int triviaIdx = 0;
    static bool lastFix = false;
    if (gpsM.text.isEmpty() || fixNow != lastFix) {
        gpsM.text = buildGps(triviaIdx);
        gpsM.scrollX = 0.0f;
        lastFix = fixNow;
    }

    uint32_t now = millis();
    float dt = (gpsM.lastDrawMs == 0) ? 0.0f : (now - gpsM.lastDrawMs) / 1000.0f;
    gpsM.lastDrawMs = now;

    canvas.setFont(TICKER_FONT);
    canvas.setTextSize(1);
    canvas.setTextDatum(TL_DATUM);
    int textW = canvas.textWidth(gpsM.text);
    int W = (int)canvas.width();
    gpsM.scrollX += SCROLL_DETECTION_PX_PER_S * dt;
    if (gpsM.scrollX > (float)(W + textW)) {  // fully off the left -> advance trivia off-screen
        if (!fixNow) triviaIdx = (triviaIdx + 1) % GPS_TRIVIA_COUNT;
        gpsM.text = buildGps(triviaIdx);
        gpsM.scrollX = 0.0f;
    }
    canvas.setTextColor(COLOR_FG);
    canvas.drawString(gpsM.text, W - (int)gpsM.scrollX, 3 + row * ROW_H);
    canvas.setFont(&fonts::Font0);
    canvas.setTextSize(1);
    return true;
}

}  // namespace Ticker
