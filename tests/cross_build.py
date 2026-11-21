"""Cross-link desktop cores using Zig's bundled target libc libraries."""
import argparse
from concurrent.futures import ThreadPoolExecutor
import os
from pathlib import Path
import subprocess

TARGETS={
    "x86_64-windows-gnu":"dll","x86-windows-gnu":"dll","aarch64-windows-gnu":"dll",
    "x86_64-linux-gnu":"so","x86-linux-gnu":"so","aarch64-linux-gnu":"so",
    "arm-linux-gnueabihf":"so","riscv64-linux-gnu":"so","powerpc64-linux-musl":"so",
    "x86_64-macos":"dylib","aarch64-macos":"dylib",
}

def main():
    p=argparse.ArgumentParser()
    p.add_argument("zig",type=Path)
    p.add_argument("output",type=Path)
    args=p.parse_args()
    zig=args.zig.resolve()
    root=args.output.resolve()
    root.mkdir(parents=True,exist_ok=True)
    def build(item):
        target,ext=item
        dest=root/target
        dest.mkdir(exist_ok=True)
        cmd=[zig,"cc","-std=c11","-O2","-DNDEBUG","-fvisibility=hidden","-target",target,
             "-shared","src/core.c","src/machine.c","src/psg.c","-o",dest/("gamate_libretro."+ext)]
        result=subprocess.run(cmd,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        (dest/"build.log").write_bytes(result.stdout)
        print(target,result.returncode,flush=True)
        return result.returncode
    with ThreadPoolExecutor(max_workers=2) as pool:
        codes=list(pool.map(build,TARGETS.items()))
    raise SystemExit(any(codes))
if __name__=="__main__":
    main()
