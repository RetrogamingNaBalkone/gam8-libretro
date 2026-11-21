"""Capture equivalent MAME software-list runs; all outputs stay private."""
import argparse
from concurrent.futures import ThreadPoolExecutor
import json
import os
from pathlib import Path
import subprocess

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("workspace", type=Path)
    parser.add_argument("mame", type=Path)
    parser.add_argument("--jobs", type=int, default=4)
    parser.add_argument("--game")
    parser.add_argument("--force",action="store_true")
    args = parser.parse_args()
    root = args.workspace.resolve()
    exe = args.mame.resolve()
    lua = Path(__file__).with_suffix(".lua").resolve()
    games = json.loads((root/"collection.json").read_text())
    def run(item):
        game, bios = item
        name = game["shortname"]
        frames = 1800 if name in ("cubeup","4in1","1milwhys") else 1200
        target = root/("capture-"+exe.parent.name)/bios/name
        target.mkdir(parents=True, exist_ok=True)
        raw = target/"frames.raw"
        count = 8 if frames > 1200 else 7
        if not args.force and raw.exists() and raw.stat().st_size == count*96000 and (target/"mame.wav").is_file() and (target/"mame-psg.txt").is_file():
            return
        cart = name if len(name) != 40 else game["path"]
        command = [str(exe),"gamate","-cart",cart,"-bios","default" if bios=="umc" else "newer",
                   "-rompath",str(root/"software"),"-hashpath",str(exe.parent/"hash"),
                   "-noreadconfig","-skip_gameinfo","-nothrottle","-video","none","-sound","none",
                   "-wavwrite",str(target/"mame.wav"),"-samplerate","48000","-joystickprovider","none","-window","-autoboot_delay","0","-autoboot_script",str(lua),
                   "-seconds_to_run",str(frames//60+4)]
        result = subprocess.run(command, cwd=target, capture_output=True, timeout=100,
                                env=dict(os.environ,GAMATE_CAPTURE=str(raw),GAMATE_PSG=str(target/"mame-psg.txt"),GAMATE_FRAMES=str(frames)))
        (target/"run.log").write_bytes(result.stdout+result.stderr)
        if result.returncode or not raw.exists() or raw.stat().st_size != count*96000:
            raise RuntimeError(f"Capture failed for {bios} {name}, see {target}/run.log")
    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        list(pool.map(run, ((g,b) for g in games if args.game in (None,g["shortname"])
                             for b in ("umc","bit"))))

if __name__ == "__main__":
    main()
