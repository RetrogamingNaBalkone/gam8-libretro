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
INFO_FILE = Path("info/gam8_libretro.info")
LICENSE_FILES = ("LICENSE", "NOTICE", "third_party/MAME-BSD.txt")
LICENSE_BUNDLE = Path("licenses/gam8/COPYING.txt")


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


def existing_builds():
    targets = [(Path(name), f"gam8_libretro.{ext}") for name, _, ext in DESKTOP_TARGETS]
    targets.extend((Path("android") / abi, "gam8_libretro_android.so") for abi in ANDROID_ABIS)
    for name, filename in targets:
        destination = ROOT / "dist" / name
        library = destination / "cores" / filename
        yield name, library if library.is_file() else destination / filename


def publish(builds, staging):
    licenses = "\n\n".join(
        filename + "\n\n" + (ROOT / filename).read_text(encoding="utf-8").rstrip()
        for filename in LICENSE_FILES
    ) + "\n"
    for name, library in builds:
        if not library.is_file() or not library.stat().st_size:
            raise OSError(f"Missing build output for {name}: {library}")
        package = staging / name
        for folder in ("cores", INFO_FILE.parent, LICENSE_BUNDLE.parent):
            (package / folder).mkdir(parents=True)
        shutil.copy2(library, package / "cores" / library.name)
        shutil.copy2(ROOT / INFO_FILE, package / INFO_FILE)
        (package / LICENSE_BUNDLE).write_text(licenses, encoding="utf-8")

    for name, library in builds:
        destination = ROOT / "dist" / name
        core = Path("cores") / library.name
        for relative_path in (core, INFO_FILE, LICENSE_BUNDLE):
            output = destination / relative_path
            output.parent.mkdir(parents=True, exist_ok=True)
            (staging / name / relative_path).replace(output)
        # Retire only known outputs of the flat package layout.
        obsolete = [library.name, INFO_FILE.name, *LICENSE_FILES]
        if library.suffix == ".dll":
            obsolete.append("libgam8_libretro.dll.a")
        for filename in obsolete:
            (destination / filename).unlink(missing_ok=True)
        third_party = destination / "third_party"
        if third_party.is_dir() and not any(third_party.iterdir()):
            third_party.rmdir()
        print(f"Published dist/{name.as_posix()}/{core.as_posix()}", flush=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repack", action="store_true", help="Package the existing dist binaries without recompiling")
    parser.add_argument("--zig", default="zig", help="Zig executable (default: zig on PATH)")
    ndk_command = "ndk-build.cmd" if os.name == "nt" else "ndk-build"
    parser.add_argument(
        "--ndk-build", default=ndk_command, help="NDK build executable (default: use PATH)",
    )
    args = parser.parse_args()
    zig = shutil.which(args.zig)
    ndk_build = shutil.which(args.ndk_build)
    if not args.repack and (not zig or not ndk_build):
        parser.error("Zig and Android ndk-build are required. Set PATH or pass --zig and --ndk-build.")

    build_dir = ROOT / "build"
    build_dir.mkdir(exist_ok=True)
    scratch = build_dir / f"release-{uuid4().hex}"
    scratch.mkdir()
    try:
        if args.repack:
            builds = list(existing_builds())
        else:
            packages = scratch / "packages"
            builds = list(build_desktop(Path(zig).resolve(), scratch, packages))
            builds.extend(build_android(Path(ndk_build).resolve(), scratch, packages))
        publish(builds, scratch / "release")
    except (OSError, subprocess.CalledProcessError, KeyboardInterrupt) as error:
        print(f"Release build failed: {error or type(error).__name__}", file=sys.stderr)
        return 1
    finally:
        shutil.rmtree(scratch)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
