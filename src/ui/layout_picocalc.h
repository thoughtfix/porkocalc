#pragma once
// PicoCalc (Porkocalc) layout defaults.
// This is the per-board profile the PicoCalc build uses. It starts as the Cardputer's 240x135 UI
// centered on the 320x320 panel; change the values here as the interface is redesigned for the
// bigger screen (stretch elements, taller bars, native 320-wide widgets, etc.). See layout.h.

#include "layout_profile_def.h"

static constexpr LayoutProfile LAYOUT_PICOCALC = {
    /* panelW      */ 320,
    /* panelH      */ 320,
    /* uiW         */ 320,   // fullscreen: top/bottom bars stretch the full width
    /* uiH         */ 320,   // bars sit at the true top and bottom of the panel
    /* topBarH     */ 14,    // same bar height as the Cardputer; content is left/center/right aligned
    /* bottomBarH  */ 14,
    /* rotation    */ 0,     // PicoCalc panel is upright at rotation 0 (HWTEST 01 verified)
};
