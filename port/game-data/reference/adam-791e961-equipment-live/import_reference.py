"""Extract scoped immutable evidence; never change the pinned audit checkout."""
import hashlib
import json
from pathlib import Path
import struct
import subprocess
import sys

COMMIT = "791e961b12233100b303038c961666834f4beb9d"
DEST = Path(__file__).resolve().parent
ROOT = DEST.parents[3]
audit = Path(sys.argv[1])

def blob(path):
    return subprocess.check_output(["git", "-c", "core.longpaths=true", "show", COMMIT + ":" + path], cwd=audit)

def sha(data):
    return hashlib.sha256(data).hexdigest()

fixture_path = "port/level-world/reference/player-equipment-render-owner-v1/query-fixtures.bin"
raw = blob(fixture_path)
magic, requirements, weapons = struct.unpack_from("<III", raw)
assert magic == 0x31515245
subset = struct.pack("<III", magic, requirements, 0) + raw[12:12 + requirements * 200]
(DEST / "requirements-fixtures.bin").write_bytes(subset)
base = "port/level-world/reference/player-equipment-render-owner-v1/original-capture/requirements/"
manifest_raw = blob(base + "original-functions.json")
functions = json.loads(manifest_raw)
functions["functions"] = [f for f in functions["functions"] if "INV_DoesMeetRequirements" in f["original_symbol"]]
assert len(functions["functions"]) == 1
(DEST / "original-functions.json").write_text(json.dumps(functions, indent=2) + "\n")
assembly_raw = blob(base + "reference/original-functions.asm")
sections = assembly_raw.decode().split("\n# ")
assembly = next(s for s in sections if s.startswith("_ZN9Character24INV_DoesMeetRequirements"))
(DEST / "requirements-original.asm").write_text("# " + assembly)
paths = ["port/level-world/player_equipment_queries_v1.cpp",
         "port/level-world/player_equipment_queries_v1.hpp",
         "port/level-world/player_equipment_render_owner_v1.cpp",
         "port/level-world/player_equipment_render_owner_v1.hpp",
         fixture_path, base + "original-functions.json", base + "reference/original-functions.asm"]
report = {"upstream": "https://github.com/AdamCelermajer/DH_sc", "commit": COMMIT,
          "upstream_inputs": [{"path": p, "sha256": sha(blob(p))} for p in paths],
          "extraction": {"requirements_cases": requirements, "omitted_weapon_cases": weapons,
                         "requirements_fixture_sha256": sha(subset),
                         "layout": "ERQ1 header with weapon count zero; unchanged 200-byte requirement records"},
          "adaptation": "Borrow caller V4 inventory, exact live PropertyView and hooks. Retain V5 gear/class/power authority. No owned renderer/inventory/RNG/property/text/VM/timer graph.",
          "local_inputs": [{"path": p, "sha256": sha((ROOT / p).read_bytes())} for p in [
              "port/game-data/player_equipment_live_services_v1.cpp",
              "port/game-data/player_equipment_live_services_v1.hpp"]]}
(DEST / "import-manifest.json").write_text(json.dumps(report, indent=2) + "\n")
print(json.dumps(report["extraction"]))
