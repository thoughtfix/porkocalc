#pragma once
// The shape of a board layout profile. Kept separate so both profile files and layout.h can
// include just the struct without a circular include.
//
// A profile is pure geometry: the physical panel size, the logical UI rectangle, the bar heights,
// and the panel rotation. Colors are NOT here - PorkChop already has a runtime theme system
// (THEMES[] in display.cpp). Fonts/text sizes can be added here later if the redesign needs them.

#include <cstdint>

struct LayoutProfile {
    int16_t panelW;      // physical panel width in pixels (at the chosen rotation)
    int16_t panelH;      // physical panel height
    int16_t uiW;         // logical UI width the mode canvases are drawn at
    int16_t uiH;         // logical UI height (topBar + main + bottomBar)
    int16_t topBarH;     // status bar height
    int16_t bottomBarH;  // bottom bar height
    uint8_t rotation;    // M5GFX rotation for this panel

    // The UI rectangle is centered on the panel. These are derived, so a redesign only sets the
    // fields above.
    constexpr int16_t originX() const { return (panelW - uiW) / 2; }
    constexpr int16_t originY() const { return (panelH - uiH) / 2; }
    constexpr int16_t mainH() const { return uiH - topBarH - bottomBarH; }
};
