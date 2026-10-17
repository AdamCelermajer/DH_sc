#!/usr/bin/env python3
"""Bundle original paired Crypt scripts and the bounded GhostAmbush01 placement."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import zipfile
import xml.etree.ElementTree as ET

EXPECTED = "3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679"
FILES = ["scripts_pyscriptnames.bin", "scripts_pyscripts.bin",
         "007_crypt_01_pyscriptnames.bin", "007_crypt_01_pyscripts.bin"]


def vec(text):
    values = [float(value) for value in text.split(",")]
    assert len(values) == 3
    return values


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("cache", type=Path)
    parser.add_argument("--project", type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()
    with args.cache.open("rb") as stream:
        assert hashlib.file_digest(stream, "sha256").hexdigest() == EXPECTED
    assets = args.project / "app/src/main/assets"
    layout = json.loads((assets / "worlds/crypt01-provenance.json").read_text(encoding="utf-8"))
    assert layout["cache_sha256"] == EXPECTED
    trigger_name = "_prim_TriggerZone_GhostAmbush01"
    candidates = [obj for obj in layout["objects"] if obj.get("name") == trigger_name]
    assert len(candidates) == 1
    trigger = candidates[0]
    assert (trigger["gametype"], trigger["script"], trigger["triggercount"], trigger["triggerdelay"]) == (
        "TriggerZone", "GhostAmbush01", "1", "0")
    room = layout["rooms"][trigger["room"]]
    assert room["gameplay"] == "crypt_straight_c_ns_01.mgp"
    assert vec(trigger["rotation"]) == [0, 0, 0]
    position = [a + b for a, b in zip(vec(trigger["position"]), room["position"])]
    scale = vec(trigger["scale"])
    inputs = []
    out = assets / "scripts"
    out.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(args.cache) as archive:
        for name in FILES + [room["gameplay"]]:
            entries = [entry for entry in archive.infolist() if Path(entry.filename).name == name]
            assert len(entries) == 1, (name, len(entries))
            entry = entries[0]
            raw = archive.read(entry)  # ZIP CRC verified by the reader.
            inputs.append({"entry": entry.filename, "name": name, "bytes": len(raw),
                           "sha256": hashlib.sha256(raw).hexdigest()})
            if name in FILES:
                (out / name).write_bytes(raw)
            else:
                cached = [obj.attrib for obj in ET.fromstring(raw).findall("GameObject")
                          if obj.get("name") == trigger_name]
                assert len(cached) == 1
                assert all(trigger[key] == value for key, value in cached[0].items())
    # Adapter format, not a historical game format: magic, version, room,
    # resolved world position XYZ, source owner scale XYZ, activation count/delay.
    descriptor = struct.pack("<4sII6fii", b"DCTR", 1, trigger["room"], *position, *scale, 1, 0)
    (out / "crypt-ghost01.dctr").write_bytes(descriptor)
    report = {"cache_sha256": EXPECTED, "inputs": inputs, "trigger_record": trigger,
              "module_record": room, "world_position": position,
              "descriptor_sha256": hashlib.sha256(descriptor).hexdigest(),
              "descriptor_bytes": len(descriptor), "scope": "one original Crypt GhostAmbush01 trigger; full paired tables included; all other script services pending"}
    (out / "crypt-script-provenance.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"paired_tables": 4, "trigger": trigger_name, "position": position,
                      "descriptor_bytes": len(descriptor)}))


if __name__ == "__main__":
    main()
