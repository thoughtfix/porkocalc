# Known issues & TODO

Porkocalc is a young port. It boots, runs, and does the fun stuff, but a few things are unfinished
or depend on your specific hardware. Nothing here is a crash. It's the honest punch-list. (Nothing
launches bug-free.)

## Open

- **GPS: field-test a real fix.** GPS is wired to the PicoCalc's "Core GPIOs" side header (GPS TX >
  header GP28, VCC > 3V3 OUT, GND > GND, 9600 baud) and the raw NMEA feed is confirmed streaming.
  It hasn't yet been verified with a full satellite *lock* outdoors, or through wardriving mode
  writing location-tagged WiGLE rows end-to-end. The plumbing is all there; it needs a clear sky.
- **Battery: works after firmware + gauge calibration.** Verified reading a real, dropping
  percentage on the test unit. Requirements for any unit: the PicoCalc's STM32 keyboard firmware
  must be recent enough to report battery (version register non-zero). Porkocalc shows `N/A` on
  older firmware and the fix is to reflash the STM32 keyboard firmware. Even then, a fresh AXP2101
  fuel gauge may sit at 100% until one full charge/discharge cycle calibrates it.
- **Battery: drains faster than expected.** On the test unit the charge falls quicker than the
  runtime we'd hoped for. Root cause is under investigation. It needs to run down to auto-shutdown
  and re-inserted to standalone 18650 charger to get that device's read of the battery. NOTE on
  first test: 18650 charger reported 94% when PicoCalc said 100%. Full drain test planned.
- **LCD brightness keys not implemented.** The STM32 keyboard can set the LCD backlight level, but
  Porkocalc doesn't yet bind any keys to raise/lower it. Brightness sits at the boot default.
- **Cardputer regression testing.** All changes are guarded behind `#ifdef PORKOCALC`, and the
  `env:m5cardputer` build still compiles, but the changes haven't been re-run on real Cardputer
  hardware.

## Board-specific notes

- **Menus now use the full panel height.** The root menu and group popups scale their visible row
  count to the 320-tall screen instead of the Cardputer's 4 rows, so nothing is cut off below the
  fold. Still top-left aligned rather than horizontally centered, and the boot splash is unchanged.
- **Audio** is PWM tones only (the PicoCalc has no audio DAC). There's no PCM/music playback. If
  0N3P0rK ports are attempted in the future, this will be a major disadvantage.
- **Attack modes are intentionally gated**: Porkocalc never auto-boots into an attack mode. This is
  a recon device.

## Ideas / nice-to-have

- BLE-mode detection ticker (WiFi ticker works; BLE needs the mode to publish a device list).
- GPS as a user-selectable ticker source.
- Reintroduce weather clouds as proper wide clouds lower in the sky.
- Native 320x320 re-layout of the menus and text screens.
- Just generally do more with all that wide open screen space.
- Implement more color.
