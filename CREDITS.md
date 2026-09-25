# Credits

Porkocalc is a port of someone else's excellent work to different hardware, standing on a lot of
shoulders. If any attribution here is wrong or missing, please open an issue. Credit matters.

## The project this is a port of

- **0ct0 / 0ct0sec: [M5PORKCHOP](https://github.com/0ct0sec/M5PORKCHOP)** (MIT). Porkocalc is a
  fork of M5PorkChop: the Tamagotchi pig, the WiFi/BLE recon, the wardriving (WiGLE) logging, the
  spectrum analyzer, the whole personality, all 0ct0's. Porkocalc's job was to make it run on a
  ClockworkPi PicoCalc with an ESP32-S3; the app itself is M5PorkChop. Please star the original.

## Hardware and platform

- **ClockworkPi: [PicoCalc](https://github.com/clockworkpi/PicoCalc)**. The handheld, its
  schematics, the STM32 keyboard/BIOS firmware, and the reference Pico drivers. The display init
  values, the STM32 keyboard/battery register protocol, and the pin assignments all come from
  their published hardware and code.
- **Waveshare: [ESP32-S3-Pico](https://www.waveshare.com/wiki/ESP32-S3-Pico)**. The Pi-Pico-shaped
  ESP32-S3 board that drops into the PicoCalc, and its schematic (the header > GPIO map).
- **lovyan03: [LovyanGFX](https://github.com/lovyan03/LovyanGFX)** (FreeBSD) and **M5Stack:
  [M5GFX](https://github.com/m5stack/M5GFX) / [M5Unified](https://github.com/m5stack/M5Unified) /
  [M5Cardputer](https://github.com/m5stack/M5Cardputer)** (MIT). The graphics engine and the M5
  API surface Porkocalc's compatibility layer mirrors.

## The PicoCalc driver layer

- **[picocalc-esp32](https://github.com/thoughtfix/picocalc-esp32)**: the reusable driver +
  Cardputer-compatibility library Porkocalc is built on. Its own CREDITS list the hardware sources
  in detail.

## Inspiration and the community

- **skizzophrenic: [M5PORKCHOP_DualScreen](https://github.com/skizzophrenic/M5PORKCHOP_DualScreen)**
  (MIT). YouTube: **[TalkingSasquach](https://www.youtube.com/@TalkingSasquach)**. The scrolling
  detected-networks ticker was inspired by DualScreen's `fxTicker` (concept only, no code copied).
- **jblanked: [Picoware](https://github.com/jblanked/Picoware)** (GPL-3.0). YouTube:
  **[jblanked](https://www.youtube.com/@jblanked)**. Picoware showed that one codebase can run on
  both the PicoCalc and the Cardputer; it shaped the approach. Used as reference only. No GPL code
  is included.
- **lexilexiko: [0N3P0rK](https://github.com/lexilexiko/0N3P0rK)** (MIT). A feature-rich cousin of
  M5PorkChop; reviewed as a reference for what's possible on this class of hardware.
- **[Valleytechsolutions](https://www.youtube.com/@Valleytechsolutions)**: for tirelessly
  showcasing what this whole community is building, and helping people find these projects.
- **kaust149: [esp32S3Pico_Zephyr](https://github.com/kaust149/esp32S3Pico_Zephyr)**: an early
  ESP32-S3-on-PicoCalc experiment, useful for cross-checking the pin map.

## Bundled libraries (compiled into the firmware)

- TinyGPSPlus (mikalhart), ArduinoJson (bblanchon), NimBLE-Arduino (h2zero),
  arduino-esp32 core + ESP-IDF (Espressif), plus M5GFX / LovyanGFX above. See each library's own
  license; a THIRD_PARTY_NOTICES file ships with binary releases.
