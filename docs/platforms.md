# Build and verification matrix

The local build targets the libretro ABI and uses software video and audio. A dynamic frontend loads the library directly. On static platforms the build produces an archive to link with the matching RetroArch frontend. The jobs in [the GitLab configuration](../.gitlab-ci.yml) use the [pinned libretro build templates](revisions.txt). The pipeline runs the device and presentation tests before building distribution artifacts. Both tests use synthetic inputs. The results below come from local builds. Upstream buildbot execution and installation via Core Downloader remain unverified.

Use [`tools/build_release.py`](../tools/build_release.py) for every local release rebuild. It packages Windows x86 and x64, Linux x86, x64, ARMv7 hard-float and ARM64, plus all four Android ABIs. Linux builds target glibc 2.17. The ARMv7 package uses an ARMv6 + VFPv2 instruction baseline for compatibility with ARMv7 handhelds. Android builds use API 21 and enable flexible page sizes with NDK r27d. Local builds use `build/` for working files and `dist/<target>/` for deliverables, with Android packages in `dist/android/<abi>/`. Each package contains the library in `cores/`, its metadata in `info/` and the combined license texts and credits in `licenses/gam8/COPYING.txt`.

In the table, a Make platform name means `make platform=NAME` in the corresponding SDK environment. The table also records earlier verification for other targets.

| Target | Build entry point / SDK | Verified result |
|---|---|---|
| Windows x86-64 | CMake, MinGW (`win`) or Zig | DLL loaded in RetroArch 1.22.2 with a zipped cartridge, both BIOS revisions, core options, save and restore. |
| Windows x86, ARM64 | MinGW or Zig | Both DLLs linked. |
| Linux x86-64 | `unix`, Zig or CMake | Shared library linked and exercised with the libretro test frontend on Linux. |
| Linux x86, ARMv7 hard float, ARM64, RISC-V 64, big-endian PPC64 | `unix` with target toolchain or Zig | Shared libraries linked. PPC64 verification used musl. |
| macOS x86-64, ARM64 | `osx` with Xcode or Zig | Mach-O libraries linked for both architectures. |
| Android ARMv7, ARM64, x86, x86-64 | NDK r27d, `ndk-build NDK_OUT=build/android NDK_LIBS_OUT=dist/android` | Four ABI-specific shared libraries linked. |
| WebAssembly | `emscripten`, emsdk | Archive and WebAssembly frontend exercised in Node. Full RetroArch JS/WASM linked using the SDK matching its precompiled objects (3.1.46). |
| PSP | `psp1`, PSPDEV | Archive and RetroArch EBOOT.PBP linked. |
| PlayStation 2 | `ps2`, PS2DEV | Archive and RetroArch ELF linked. |
| PlayStation Vita | `vita`, VitaSDK | Archive and software-only RetroArch VPK linked. The local frontend link omitted the optional vitaGL and vitashark libraries. |
| Nintendo 3DS | `ctr`, devkitARM/libctru | Archive and RetroArch 3DSX linked. The separate legacy libctru job is configured. |
| GameCube | `ngc`, devkitPPC/libogc | Archive and RetroArch DOL linked. The frontend's audio, graphics and timer objects were recompiled for the installed libogc. |
| Wii | `wii`, devkitPPC/libogc | Archive built. The precompiled frontend and local libogc differ in their network and delay functions. Final DOL unverified. |
| Wii U | `wiiu`, devkitPPC/wut | Archive, RetroArch ELF and uncompressed RPX linked. The local SDK lacks the final RPX compression utility. |
| Nintendo Switch | `libnx`, devkitA64/libnx | Archive and RetroArch NRO linked. |
| Nintendo Switch 2 | `libounce`, devkitA64/libounce | CI job configured. Local devkitPro image lacks libounce. |
| iOS ARMv7 / ARM64, tvOS ARM64 | `ios9`, `ios-arm64`, `tvos-arm64` | CI jobs configured. Target Apple SDKs unavailable locally. |
| macOS PowerPC | `osx arch=ppc` | The pinned libretro template disables this job with `when: never`. The PowerPC SDK build remains unverified. |
| Windows MSVC | Windows CI jobs for MSVC 2005 x86, 2010 and 2019 x86/x64 | C++98 compilation checked with Clang. Local Visual Studio installation lacks the necessary SDK and compiler headers. |
| webOS ARMv7 / ARM64 | `unix` after sourcing webOS SDK environment | Both CI jobs configured. Target sysroots unverified locally. |
| GCW Zero, OpenDingux beta, RS90, RetroFW, Miyoo | Dingux CI templates and device toolchains | CI jobs configured. Device-specific sysroots unverified locally. |
| DOS | `dos`, DJGPP 12.2 | Archive and RetroArch DOS executable linked. |
| PlayStation 3, PlayStation 4 | `psl1ght` with PSL1GHT, `orbis` with OrbisDev | Static RetroArch jobs configured. Target SDK and frontend combinations unverified locally. |

No console executables have been exercised on physical devices. Additional platform branches inherited from Potator, including Raspberry Pi, QNX and Xbox SDKs, are available in [Makefile](../Makefile) and have not been independently qualified.

## Binary distribution

The configured CI archives include `LICENSE`, `NOTICE` and `third_party/MAME-BSD.txt`. Keep these files with the binaries or in their accompanying distribution materials when repackaging a core. GNU Make installs them under `$(prefix)/share/licenses/gam8`.
