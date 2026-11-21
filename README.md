# Gamate for libretro

A standalone software-rendered Bit Corporation Gamate emulator for RetroArch. The machine and CPU follow the pinned MAME driver. The LCD runs at 160 × 150 and 60.8093 Hz. The core supplies 48 kHz stereo audio, one RetroPad, save states, rewind and run-ahead. [Validation results](docs/compatibility.md) · [Build matrix](docs/platforms.md) · [Upstream revisions](docs/revisions.txt)

## Start a game

1. Copy the library for your platform into RetroArch's **Cores** directory and `gamate_libretro.info` into its **Core Info** directory. Alternatively, use **Load Core → Install or Restore a Core**.
2. Extract at least one of the following 4096-byte BIOS files into RetroArch's **System/BIOS** directory. Rename the extracted file as shown.
3. Select **Load Core → Gamate**, then **Load Content** and select a cartridge `.bin` or its single-game ZIP. RetroArch handles archive extraction.

| BIOS | No-Intro name | Filename in System/BIOS | SHA-1 |
|---|---|---|---|
| UMC | [BIOS] Gamate (World) | `gamate_bios_umc.bin` | `ea449dc607601f9a68d855ad6ab53800d2e99297` |
| BIT | [BIOS] Gamate (World) (Rev 1) | `gamate_bios_bit.bin` | `4e9dfbfe916ca485530ef4221593ab68738e2217` |

**Quick Menu → Core Options** provides the following settings:

| Setting | Behaviour |
|---|---|
| Color palette | Original (MAME), Pico, grayscale or an individual game's INI. Changes apply while playing. |
| LCD ghosting | Off by default. Light, medium and strong retain 25%, 50% and 75% of the previous displayed pixel respectively. |
| BIOS | Auto tries UMC, then BIT. An explicit choice requires that revision. Reload the content after changing BIOS. |

Use the D-pad, A, B, Start/Pause and Select on RetroPad port 1. Saves are associated with the game and BIOS. A state from a different image is rejected.

### Pico palette files

Copy the game's Pico preset to `<System/BIOS>/gamate/palettes/<cartridge basename>.ini`, then select **Game (INI)**. For example, `Cube Up (World).bin` uses `Cube Up (World).ini`. The file contains four six-digit RGB values, accepts LF or CRLF and uses the same shade order as pico-gamate.

```ini
[palette]
rgb0=CCFFFF
rgb1=FFB266
rgb2=CC0066
rgb3=663300
```

## Build

From a clean checkout with a C compiler and CMake:

```sh
cmake -S . -B build/native -DCMAKE_BUILD_TYPE=Release
cmake --build build/native --config Release
```

GNU Make is also supported. Run `make platform=unix` for Linux or BSD, `make platform=osx` for macOS, or `make platform=win` under MinGW. A static frontend can use `-DGAMATE_STATIC=ON` and its SDK toolchain file with CMake. Android uses the NDK's `ndk-build` from the project root. Platform names and SDK-specific commands are listed in the [build matrix](docs/platforms.md).

## Test

```sh
cmake -S . -B build/tests -DGAMATE_TESTS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build/tests --config Release
ctest --test-dir build/tests -C Release --output-on-failure
```

The two device and presentation tests generate their own programs and video data. For the optional private comparison, supply No-Intro ZIPs and a MAME binary built from the revision in `docs/revisions.txt`. Place the matching MAME `hash/gamate.xml` beside that binary in its `hash` directory.

```sh
python tests/prepare_collection.py "/path/to/No-Intro" /path/to/mame/hash/gamate.xml .local
python tests/reference.py .local /path/to/mame/mame
python tests/compare.py .local build/tests/gamate_frontend --reference capture-mame
```

The reference directory is named `capture-<MAME executable's parent directory>`. On Windows, pass the built `gamate_frontend.exe` including its configuration subdirectory if applicable. The harness stores game data, raw framebuffers, sound recordings and comparison JSON under the ignored `.local/` directory.

## Source and license

The core is licensed under GPL-2.0-or-later. Attribution and component licenses are listed in [NOTICE](NOTICE). Normal builds use the checked-in CPU decoder. To regenerate it, run `python tools/generate_cpu.py <MAME source>/src/devices/cpu/m6502` with the pinned source revision. No BIOS or cartridge image is included.
