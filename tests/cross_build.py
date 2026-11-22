"""Cross-link desktop cores using Zig's bundled target libc libraries."""
import argparse
from concurrent.futures import ThreadPoolExecutor
import os
from pathlib import Path
import subprocess
import shutil
import tempfile

TARGETS={
    "x86_64-windows-gnu":"dll","x86-windows-gnu":"dll","aarch64-windows-gnu":"dll",
    "x86_64-linux-gnu":"so","x86-linux-gnu":"so","aarch64-linux-gnu":"so",
    "arm-linux-gnueabihf":"so","riscv64-linux-gnu":"so","powerpc64-linux-musl":"so",
    "x86_64-macos":"dylib","aarch64-macos":"dylib",
}

def main():
    p=argparse.ArgumentParser()
    p.add_argument("zig",type=Path)
    p.add_argument("output",type=Path,nargs="?",default=Path("dist"))
    args=p.parse_args()
    zig=args.zig.resolve()
    root=args.output.resolve()
    root.mkdir(parents=True,exist_ok=True)
    work = Path("build").resolve()
    work.mkdir(exist_ok=True)
    with tempfile.TemporaryDirectory(dir=work, prefix="cross-") as scratch:
        return build_targets(zig, root, Path(scratch))

def build_targets(zig, root, scratch):
    def build(item):
        target,ext=item
        stage=scratch/target
        stage.mkdir()
        library=stage/("gam8_libretro."+ext)
        cmd=[zig,"cc","-std=c11","-O2","-DNDEBUG","-fvisibility=hidden","-target",target,
             "-shared","src/core.c","src/machine.c","src/psg.c","-o",library]
        result=subprocess.run(cmd,env=dict(os.environ,ZIG_GLOBAL_CACHE_DIR=str(scratch/"zig-cache"),
                                          ZIG_LOCAL_CACHE_DIR=str(stage/"cache")))
        if result.returncode == 0:
            dest=root/target
            dest.mkdir(exist_ok=True)
            shutil.copy2(library, dest/library.name)
        print(target,result.returncode,flush=True)
        return result.returncode
    with ThreadPoolExecutor(max_workers=2) as pool:
        codes=list(pool.map(build,TARGETS.items()))
    raise SystemExit(any(codes))
if __name__=="__main__":
    main()
