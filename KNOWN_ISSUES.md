# Known issues & TODO

Porkocalc is a young port. It boots, runs, and does the fun stuff, but a few things are unfinished
or depend on your specific hardware. Nothing here is a crash - it's the honest punch-list. (Nothing
launches bug-free.)

## Open

- **GPS: field-test a real fix.** GPS is wired to the PicoCalc's "Core GPIOs" side header (GPS TX →
  header GP28, VCC → 3V3 OUT, GND → GND, 9600 baud) and the raw NMEA feed is confirmed streaming.
  It hasn't yet been verified with a full satellite *lock* outdoors, or through wardriving mode
  writing location-tagged WiGLE rows end-to-end. The plumbing is all there; it needs a clear sky.
- **Battery: works after firmware + gauge calibration.** Verified reading a real, dropping
  percentage on the test unit. Requirements for any unit: the PicoCalc's STM32 keyboard firmware
  must be recent enough to report battery (version register non-zero) - Porkocalc shows `N/A` on
  older firmware and the fix is to reflash the STM32 keyboard firmware. Even then, a fresh AXP2101
  fuel gauge may sit at 100% until one full charge→discharge cycle calibrates it.
- **Battery: drains faster than expected.** On the test unit the charge falls quicker than the
  runtime we'd hoped for. Not yet root-caused - needs a full run down to auto-poweroff and then a
  multimeter check of the 18650s to see the real cell voltage vs. the reported percentage. Still
  longer-lived than a Cardputer.
- **LCD brightness keys not implemented.** The STM32 keyboard can set the LCD backlight level, but
  Porkocalc doesn't yet bind any keys to raise/lower it. Brightness sits at the boot default.
- **Cardputer regression testing.** All changes are guarded behind `#ifdef PORKOCALC`, and the
  `env:m5cardputer` build still compiles, but the changes haven't been re-run on real Cardputer
  hardware.

## Board-specific notes

- **Menus now use the full panel height** - the root menu and group popups scale their visible row
  count to the 320-tall screen instead of the Cardputer's 4 rows, so nothing is cut off below the
  fold. Still top-left aligned rather than horizontally centered, and the boot splash is unchanged.
- **Audio** is PWM tones only (the PicoCalc has no audio DAC); there's no PCM/music playback.
- **Attack modes are intentionally gated**: Porkocalc never auto-boots into an attack mode. This is
  a recon device.

## Ideas / nice-to-have

- BLE-mode detection ticker (WiFi ticker works; BLE needs the mode to publish a device list).
- GPS as a user-selectable ticker source.
- Reintroduce weather clouds as proper wide clouds lower in the sky.
- Native 320×320 re-layout of the menus and text screens.
