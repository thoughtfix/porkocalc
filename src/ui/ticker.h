#pragma once
// Tickers: single-line, double-height marquees in the space above the bottom-anchored pig on the
// fullscreen (PicoCalc) layout. Two kinds:
//   - Detection ticker: what the device is actively detecting (WiFi APs). Shown ONLY while
//     scanning, so an idle pig isn't streaming data it isn't collecting.
//   - Status ticker: a "news chyron" of system status (uptime, mode, battery, heap, PSRAM, SD).
//     Always available, so the center has motion even when idle.
// Both draw nothing on the Cardputer's short canvas.
//
// Detection-ticker concept adapted from the scrolling AP marquee in M5PORKCHOP_DualScreen by
// skizzophrenic (MIT): https://github.com/skizzophrenic/M5PORKCHOP_DualScreen (fxTicker). No code
// was copied.

#include <M5Unified.h>
#include "../core/porkchop.h"  // PorkchopMode

namespace Ticker {

// Row 0 is the top marquee line, row 1 the one below it, and so on.
void drawDetection(M5Canvas& canvas, PorkchopMode mode, int row);
void drawStatus(M5Canvas& canvas, PorkchopMode mode, int row);

// GPS status marquee: live fix / satellites / coordinates when locked, or "searching" with GPS
// trivia (stable per scroll cycle) while it hunts. Nudges the pig to EXCITED the moment a fix lands.
// Auto-hides when no receiver is streaming; also honors a manual show/hide toggle. Returns true if
// it drew (so callers can lay out rows).
bool drawGps(M5Canvas& canvas, PorkchopMode mode, int row);

// Manually show/hide the GPS marquee (the G key). Independent of the auto-detect / settings gates.
void toggleGps();

}  // namespace Ticker
