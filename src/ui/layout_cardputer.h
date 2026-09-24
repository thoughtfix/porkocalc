#pragma once
// Cardputer layout defaults (the original PorkChop geometry).
// This is the per-board profile the Cardputer build uses. Edit here to change the Cardputer
// layout only. See layout.h for the struct and how the active profile is chosen.

#include "layout_profile_def.h"

static constexpr LayoutProfile LAYOUT_CARDPUTER = {
    /* panelW      */ 240,
    /* panelH      */ 135,
    /* uiW         */ 240,
    /* uiH         */ 135,
    /* topBarH     */ 14,
    /* bottomBarH  */ 14,
    /* rotation    */ 1,   // Cardputer panel is landscape at rotation 1
};
