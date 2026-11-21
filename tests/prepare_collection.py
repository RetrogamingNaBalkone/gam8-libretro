"""Extract user-supplied archives into a private test workspace."""
import hashlib
import json
from pathlib import Path
import sys
import xml.etree.ElementTree as ET
import zipfile

BIOS = {"ea449dc607601f9a68d855ad6ab53800d2e99297": "umc",
        "4e9dfbfe916ca485530ef4221593ab68738e2217": "bit"}

def prepare(collection, xml, root):
    catalog = {}
    for software in ET.parse(xml).findall("software"):
        rom = software.find(".//rom")
        if rom is not None:
            catalog[rom.get("sha1")] = (software.get("name"), rom.get("name"))
    system = root / "system"
    system.mkdir(parents=True, exist_ok=True)
    rows = []
    for archive in sorted(collection.glob("*.zip")):
        with zipfile.ZipFile(archive) as z:
            member, = [n for n in z.namelist() if n.lower().endswith(".bin")]
            data = z.read(member)
        digest = hashlib.sha1(data).hexdigest()
        if digest in BIOS:
            (system / ("gamate_bios_" + BIOS[digest] + ".bin")).write_bytes(data)
            continue
        name, chip = catalog.get(digest, (digest, "game.bin"))
        raw = root / "roms" / (archive.stem + ".bin")
        raw.parent.mkdir(exist_ok=True)
        raw.write_bytes(data)
        game = root / "software" / "gamate" / name
        game.mkdir(parents=True, exist_ok=True)
        (game / chip).write_bytes(data)
        rows.append(dict(title=archive.stem, sha1=digest, shortname=name, path=str(raw.resolve())))
    firmware = root / "software" / "gamate"
    for bios in system.glob("gamate_bios_*.bin"):
        (firmware / bios.name).write_bytes(bios.read_bytes())
    (root / "collection.json").write_text(json.dumps(rows, indent=2), encoding="utf8")

if __name__ == "__main__":
    prepare(*(Path(arg) for arg in sys.argv[1:]))
