# Gam8

A standalone software-rendered Bit Corporation Gamate emulator for RetroArch. The machine and CPU follow the pinned MAME driver. The LCD runs at 160 × 150 and 60.8093 Hz. The core supplies 48 kHz stereo audio, one RetroPad, save states, rewind and run-ahead. [Validation results](docs/compatibility.md) · [Build matrix](docs/platforms.md) · [Upstream revisions](docs/revisions.txt)

## Start a game

1. Copy the library for your platform into RetroArch's **Cores** directory and `gam8_libretro.info` into its **Core Info** directory. Alternatively, use **Load Core → Install or Restore a Core**.
2. Extract at least one of the following 4096-byte BIOS files into RetroArch's **System/BIOS** directory. Rename the extracted file as shown.
3. Select **Load Core → Bit Corporation - Gamate (Gam8)**, then **Load Content** and select a cartridge `.bin` or its single-game ZIP. RetroArch handles archive extraction. Supported ROM sizes are 16, 32, 64, 128, 256 and 512 KiB.

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
| Cartridge board | Auto detects the original 4-in-1 by checksum. For a patched 128 KiB 4-in-1, select **4-in-1**, save the option for that game and reload the content. Known dumps always use their original board. |

Use the D-pad, A, B, Start/Pause and Select on RetroPad port 1. Save states require the same cartridge image, BIOS and board.

### Pico palette files

Copy the game's Pico preset to `<System/BIOS>/gamate/palettes/<cartridge basename>.ini`, then select **Game (INI)**. For example, `Cube Up (World).bin` uses `Cube Up (World).ini`. The file contains four six-digit RGB values, accepts LF or CRLF and uses the same shade order as pico-gamate.

```ini
[palette]
rgb0=CCFFFF
rgb1=FFB266
rgb2=CC0066
rgb3=663300
```

## Upgrading from the Gamate core

Install the `gam8_libretro` library and its matching info file, then select Gam8 for playlists that point to the old core. To reuse per-core settings, copy `config/Gamate/` to `config/Gam8/` and rename `Gamate.opt` and `Gamate.cfg` there to `Gam8.opt` and `Gam8.cfg` if present. If RetroArch sorts saves or states by core name, copy the files from each `Gamate/` save or state folder into its sibling `Gam8/` folder. Existing BIOS files and palette presets use the same paths.

## Build

Use `build/` for build trees and test workspaces. Publish finished files under `dist/<target>/`. See the [project layout and cleanup rules](AGENTS.md).

From a clean checkout with a C compiler and CMake:

```sh
cmake -S . -B build/native -DCMAKE_BUILD_TYPE=Release
cmake --build build/native --config Release
cmake --install build/native --config Release --prefix dist/native
```

GNU Make is also supported. Run `make platform=unix` for Linux or BSD, `make platform=osx` for macOS, or `make platform=win` under MinGW. A static frontend can use `-DGAM8_STATIC=ON` and its SDK toolchain file with CMake. Android uses the NDK's `ndk-build NDK_OUT=build/android NDK_LIBS_OUT=dist/android` from the project root. Platform names and SDK-specific commands are listed in the [build matrix](docs/platforms.md).

## Test

```sh
cmake -S . -B build/tests -DGAM8_TESTS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build/tests --config Release
ctest --test-dir build/tests -C Release --output-on-failure
```

The two device and presentation tests generate their own programs and video data. For the optional private comparison, supply No-Intro ZIPs and a MAME binary built from the revision in `docs/revisions.txt`. Place the matching MAME `hash/gamate.xml` beside that binary in its `hash` directory.

```sh
python tests/run_comparison.py "/path/to/No-Intro" /path/to/mame/mame build/tests/gam8_frontend build/comparison.json
```

On Windows, pass the built `gam8_frontend.exe` including its configuration subdirectory if applicable. The runner removes extracted games, raw frames, audio and traces when it exits. The JSON report goes to the path supplied as the last argument.

## Source and license

The core is licensed under GPL-2.0-or-later. Attribution and component licenses are listed in [NOTICE](NOTICE). Normal builds use the checked-in CPU decoder. To regenerate it, run `python tools/generate_cpu.py <MAME source>/src/devices/cpu/m6502` with the pinned source revision. No BIOS or cartridge image is included.
