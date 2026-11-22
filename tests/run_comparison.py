"""Compare a collection in disposable storage and keep only the JSON report."""
import argparse
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile


def main():
    parser = argparse.ArgumentParser()
    for name in ("collection", "mame", "frontend", "report"):
        parser.add_argument(name, type=Path)
    args = parser.parse_args()
    scripts = Path(__file__).resolve().parent
    build = scripts.parent / "build"
    build.mkdir(exist_ok=True)
    mame = args.mame.resolve()
    with tempfile.TemporaryDirectory(dir=build, prefix="comparison-") as scratch:
        work = Path(scratch)
        subprocess.run([sys.executable, scripts / "prepare_collection.py", args.collection,
                        mame.parent / "hash" / "gamate.xml", work], check=True)
        subprocess.run([sys.executable, scripts / "reference.py", work, mame], check=True)
        result = subprocess.run([sys.executable, scripts / "compare.py", work,
                                 args.frontend.resolve(), "--reference", "capture-" + mame.parent.name])
        report = work / "comparison.json"
        if report.is_file():
            args.report.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(report, args.report)
        result.check_returncode()


if __name__ == "__main__":
    main()
