#pragma once
// Active display layout for the current build.
//
// LAYOUT is the single source of truth for UI geometry. The per-board defaults live in
// layout_cardputer.h and layout_picocalc.h; this file selects which one is live (the equivalent
// of copying a layout-<board>-defaults file to the live layout). To change the layout, edit the
// matching profile file - not the many draw sites.
//
// Why this is a compile-time struct and not a runtime layout.xml/json:
//  - The firmware is reflashed to change behavior anyway, so edit-without-recompile buys little.
//  - Loading a layout file at boot adds an SD/SPIFFS dependency and a failure mode (missing or
//    malformed file => no UI). A constexpr profile can't fail and costs nothing at runtime.
//  - The draw code is imperative C++ (animations, spectrum, the pig); only parameters can be
//    externalized, which is exactly what this profile holds.
// A runtime JSON override (read a few values from SD to tweak the active profile) can be layered
// on top later if on-device theming is ever wanted; it would fill this same struct.

#include "layout_profile_def.h"

#ifdef PORKOCALC
#include "layout_picocalc.h"
static constexpr LayoutProfile LAYOUT = LAYOUT_PICOCALC;
#else
#include "layout_cardputer.h"
static constexpr LayoutProfile LAYOUT = LAYOUT_CARDPUTER;
#endif
