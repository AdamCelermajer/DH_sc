"""Extract only the text/query dependency closure from pinned Adam blobs.

Does not change the audit checkout. Original APK/cache/ELF remain local inputs.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess

COMMIT = "791e961b12233100b303038c961666834f4beb9d"
ROOT = Path(__file__).resolve().parents[4]
REF = Path(__file__).resolve().parent

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--audit", type=Path, required=True)
    args = ap.parse_args()
    paths = {}
    for base in ("localization", "hud_text_format_v1", "hud_text_v1", "item_text_varargs_v5", "item_text_owner_v5"):
        for suffix in ("hpp", "cpp"):
            path = f"port/engine-ui/{base}.{suffix}"
            paths[path] = path
    for name in ("hud_text_format_v1.cpp", "item_text_varargs_v5.cpp", "item_text_varargs_v5_fixture.cpp"):
        path = "port/engine-ui/tests/" + name
        paths[path] = path
    refs = {
        "port/engine-ui/reference/hud-formatting-v1/format-gold.bin": "format-gold.bin",
        "port/engine-ui/reference/item-text-varargs-v5/fixtures.bin": "varargs-fixtures.bin",
        "port/level-world/reference/player-equipment-render-owner-v1/query-fixtures.bin": "query-fixtures.bin",
        "port/engine-ui/reports/hud-text-format-v1-arm64-differential.json": "upstream-format-original.json",
        "port/engine-ui/reports/item-text-varargs-v5-arm64-differential.json": "upstream-varargs-original.json",
        "port/level-world/reports/player-equipment-queries-v1-arm64-differential.json": "upstream-query-original.json",
        "port/level-world/reference/player-equipment-render-owner-v1/original-capture/requirements/original-functions.json": "query-original-functions.json",
        "port/level-world/reference/player-equipment-render-owner-v1/original-capture/requirements/reference/original-functions.asm": "query-original.asm",
        "port/engine-ui/reference/localization/original-functions.json": "localization-original-functions.json",
        "port/engine-ui/reference/hud-formatting-v1/original-functions.json": "format-original-functions.json",
    }
    paths.update({k: str((REF / v).relative_to(ROOT)).replace("\\", "/") for k, v in refs.items()})
    rows = []
    for source, destination in paths.items():
        blob = subprocess.run(["git", "-c", "core.longpaths=true", "show", COMMIT + ":" + source],
                              cwd=args.audit, check=True, capture_output=True).stdout
        target = ROOT / destination
        if target.exists() and target.read_bytes() != blob:
            raise RuntimeError("Refusing to overwrite different existing file: " + destination)
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(blob)
        rows.append({"source": source, "destination": destination, "sha256": hashlib.sha256(blob).hexdigest()})
    capture_groups = (
        ("port/game-data/reference/player-inventory-v1/captures/core", ("_ZN13ItemInventoryC1Ev",)),
        ("port/game-data/reference/player-creation-v2/original/initial-grants", ("_ZN9Character8InitPostEv", "_ZN9Character14_InitEquipmentEv")),
    )
    original_functions = []
    original_asm = []
    for base, symbols in capture_groups:
        suffix = "reference/original-functions.asm"
        metadata = json.loads(subprocess.run(["git", "-c", "core.longpaths=true", "show", COMMIT + ":" + base + "/original-functions.json"], cwd=args.audit, check=True, capture_output=True).stdout)
        assembly = subprocess.run(["git", "-c", "core.longpaths=true", "show", COMMIT + ":" + base + "/" + suffix], cwd=args.audit, check=True, capture_output=True).stdout.decode()
        blocks = re.split(r"(?=^# )", assembly, flags=re.MULTILINE)
        for symbol in symbols:
            original_functions.extend(row for row in metadata["functions"] if row["original_symbol"] == symbol)
            found = [block for block in blocks if block.startswith("# " + symbol + "\n")]
            if len(found) != 1:
                raise RuntimeError("Missing scoped original function " + symbol)
            original_asm.extend(found)
    (REF / "inventory-construction-original.json").write_text(json.dumps({"original_sha256": metadata["original_sha256"], "functions": original_functions}, indent=2) + "\n")
    (REF / "inventory-construction-original.asm").write_text("\n".join(original_asm))
    query_inputs = {}
    for suffix in ("hpp", "cpp"):
        path = "port/level-world/player_equipment_queries_v1." + suffix
        blob = subprocess.run(["git", "show", COMMIT + ":" + path], cwd=args.audit, check=True, capture_output=True).stdout
        query_inputs[path] = hashlib.sha256(blob).hexdigest()
    (REF / "import-manifest.json").write_text(json.dumps({"commit": COMMIT, "files": rows, "query_upstream_input_sha256": query_inputs,
         "adaptation": "Only weapon facts kernel reused; existing requirement kernel remains frozen. Live facade borrows existing V4/PropertyView and maps raw two-hander to existing CombatantView."}, indent=2) + "\n")
    print(json.dumps({"commit": COMMIT, "files": len(rows)}))

if __name__ == "__main__":
    main()
