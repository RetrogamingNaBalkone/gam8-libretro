"""Build and package all supported Gam8 release targets."""

import argparse
import os
from pathlib import Path
import shutil
import subprocess
import sys
from uuid import uuid4


ROOT = Path(__file__).resolve().parents[1]
DESKTOP_TARGETS = (
    ("windows-x86", "x86-windows-gnu", "dll"),
    ("windows-x64", "x86_64-windows-gnu", "dll"),
    ("linux-x86", "x86-linux-gnu.2.17", "so"),
    ("linux-x64", "x86_64-linux-gnu.2.17", "so"),
    ("linux-armv7", "arm-linux-gnueabihf.2.17", "so"),
    ("linux-arm64", "aarch64-linux-gnu.2.17", "so"),
)
ANDROID_ABIS = ("armeabi-v7a", "arm64-v8a", "x86", "x86_64")
PACKAGE_FILES = (
    ("info/gam8_libretro.info", "gam8_libretro.info"),
    ("LICENSE", "LICENSE"),
    ("NOTICE", "NOTICE"),
    ("third_party/MAME-BSD.txt", "third_party/MAME-BSD.txt"),
)


def build_desktop(zig, scratch, packages):
    for name, target, extension in DESKTOP_TARGETS:
        print(f"Building {name}", flush=True)
        package = packages / name
        package.mkdir(parents=True)
        library = package / f"gam8_libretro.{extension}"
        flags = []
        if extension == "so":
            flags.extend(("-fPIC", "-Wl,-z,defs", "-Wl,--version-script=link.T"))
        if name == "linux-armv7":
            flags.append("-mcpu=arm1176jzf_s")
        subprocess.run(
            [zig, "cc", "-std=c11", "-O2", "-DNDEBUG", "-fvisibility=hidden",
             "-target", target, "-shared", *flags, "src/core.c", "src/machine.c",
             "src/psg.c", "-o", library],
            cwd=ROOT, check=True,
            env=dict(os.environ, ZIG_GLOBAL_CACHE_DIR=str(scratch / "zig-global"),
                     ZIG_LOCAL_CACHE_DIR=str(scratch / "zig-local")),
        )
        yield Path(name), library


def build_android(ndk_build, scratch, packages):
    print("Building Android", flush=True)
    libraries = scratch / "android-libs"
    subprocess.run(
        [ndk_build, f"NDK_OUT={(scratch / 'android-obj').relative_to(ROOT).as_posix()}",
         f"NDK_LIBS_OUT={libraries.relative_to(ROOT).as_posix()}",
         "APP_SUPPORT_FLEXIBLE_PAGE_SIZES=true"],
        cwd=ROOT, check=True,
    )
    for abi in ANDROID_ABIS:
        name = Path("android") / abi
        package = packages / name
        package.mkdir(parents=True)
        library = package / "gam8_libretro_android.so"
        shutil.copy2(libraries / abi / "libretro.so", library)
        yield name, library


def publish(builds):
    # Stage metadata and verify all libraries before modifying any distribution files.
    for name, library in builds:
        if not library.is_file() or not library.stat().st_size:
            raise OSError(f"Missing build output for {name}: {library}")
        for source, relative_path in PACKAGE_FILES:
            staged = library.parent / relative_path
            staged.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(ROOT / source, staged)

    for name, library in builds:
        destination = ROOT / "dist" / name
        for relative_path in [*(path for _, path in PACKAGE_FILES), library.name]:
            output = destination / relative_path
            output.parent.mkdir(parents=True, exist_ok=True)
            (library.parent / relative_path).replace(output)
        print(f"Published dist/{name.as_posix()}/{library.name}", flush=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--zig", default="zig", help="Zig executable (default: zig on PATH)")
    ndk_command = "ndk-build.cmd" if os.name == "nt" else "ndk-build"
    parser.add_argument(
        "--ndk-build", default=ndk_command, help="NDK build executable (default: use PATH)",
    )
    args = parser.parse_args()
    zig = shutil.which(args.zig)
    ndk_build = shutil.which(args.ndk_build)
    if not zig or not ndk_build:
        parser.error("Zig and Android ndk-build are required. Set PATH or pass --zig and --ndk-build.")

    build_dir = ROOT / "build"
    build_dir.mkdir(exist_ok=True)
    scratch = build_dir / f"release-{uuid4().hex}"
    scratch.mkdir()
    try:
        packages = scratch / "packages"
        builds = list(build_desktop(Path(zig).resolve(), scratch, packages))
        builds.extend(build_android(Path(ndk_build).resolve(), scratch, packages))
        publish(builds)
    except (OSError, subprocess.CalledProcessError, KeyboardInterrupt) as error:
        print(f"Release build failed: {error or type(error).__name__}", file=sys.stderr)
        return 1
    finally:
        shutil.rmtree(scratch)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
