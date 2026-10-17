"""Gate borrowed equipment services through the selected game-data DSO.

Uses canonical local pydata without importing it. Attachment/world/text/Skin
services are explicit host fixtures, not native rendering/gameplay evidence.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess

ROOT = Path(__file__).resolve().parents[3]
REF = ROOT / "port/game-data/reference/adam-791e961-equipment-live"
TARGET = "player_equipment_live_services_v1_selected"

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("cache", "build", "compiler", "report"):
        parser.add_argument("--" + name, type=Path, required=True)
    args = parser.parse_args()
    args.build.mkdir(parents=True, exist_ok=True)
    env = os.environ.copy()
    env["PATH"] = str(args.compiler.parent) + os.pathsep + env.get("PATH", "")
    logs = []
    def run(command):
        result = subprocess.run([str(x) for x in command], cwd=ROOT, env=env, capture_output=True, text=True)
        logs.append({"command": [str(x) for x in command], "returncode": result.returncode,
                     "stdout": result.stdout, "stderr": result.stderr})
        if result.returncode:
            (args.build / "gate-command-outputs.json").write_text(json.dumps(logs, indent=2) + "\n")
            raise RuntimeError(result.stderr or result.stdout)
        return result.stdout
    run(["cmake", "-S", ROOT / "port/game-data/tests/equipment-live-services-v1", "-B", args.build,
         "-G", "Ninja", "-DCMAKE_BUILD_TYPE=RelWithDebInfo", "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
         "-DCMAKE_CXX_COMPILER=" + str(args.compiler),
         "-DCMAKE_C_COMPILER=" + str(args.compiler.with_name("gcc.exe"))])
    database = json.loads((args.build / "compile_commands.json").read_text())
    selected = [entry for entry in database if "CMakeFiles/dh2_game_data.dir" in entry["command"].replace("\\", "/")]
    assert sum(Path(entry["file"]).name == "player_equipment_live_services_v1.cpp" for entry in selected) == 1
    reached = selected + [entry for entry in database if "CMakeFiles/" + TARGET + ".dir" in entry["command"].replace("\\", "/")]
    sources = set()
    pending = [Path(entry["file"]).resolve() for entry in reached]
    while pending:
        path = pending.pop()
        if path in sources:
            continue
        path.relative_to(ROOT)
        sources.add(path)
        for include in re.findall(r'^\s*#\s*include\s*"([^"]+)"', path.read_text(), re.MULTILINE):
            dependency = (path.parent / include).resolve()
            if dependency.is_file():
                pending.append(dependency)
    sources.update([Path(__file__).resolve(), ROOT / "port/game-data/CMakeLists.txt",
                    ROOT / "port/game-data/tests/equipment-live-services-v1/CMakeLists.txt"])
    before = {p.relative_to(ROOT).as_posix(): digest(p) for p in sorted(sources)}
    run(["cmake", "--build", args.build, "--parallel", "1", "--target", TARGET])
    library = args.build / "game-data/libdh2_game_data.dll"
    env["PATH"] = str(library.parent) + os.pathsep + env["PATH"]
    gold = [REF / "requirements-fixtures.bin", ROOT / "port/game-data/reference/player-item-effects-v5/starter-effects-fixtures.bin"]
    command = [args.build / (TARGET + ".exe"), *gold, args.cache]
    result = json.loads(run(command))
    assert result["validation"] == "PASS" and result["mismatches"] == 0
    after = {p.relative_to(ROOT).as_posix(): digest(p) for p in sources}
    assert before == after, "Source changed during gate; rerun frozen selection"
    report = {"validation": "PASS", "upstream_commit": "791e961b12233100b303038c961666834f4beb9d",
              "scope": "Borrowed V4 inventory + exact live PropertyView/buffs with existing V5 gear/class/vitals calculations. Selected game-data DSO, original requirement/starter gold and explicit attachment/world/text/Skin fixtures. Native rendering/lifetime integration and failed temporary/whole-inventory retirement remain mandatory; no native equipment/gameplay claim.",
              "source_before_after_equal": True, "source_sha256": before,
              "selected_library": {"path": str(library), "sha256": digest(library)},
              "selected_commands": selected,
              "test": {"command": [str(x) for x in command], "result": result},
              "reference_sha256": {p.relative_to(ROOT).as_posix(): digest(p) for p in gold + [REF / "import-manifest.json", REF / "original-functions.json", REF / "requirements-original.asm"]},
              "cache_sha256": {prefix + suffix: digest(args.cache / (prefix + suffix))
                               for prefix in ["loot_table", "item_powers", "character_properties", "character_classes"]
                               for suffix in ["_pyarray.bin", "_pyarraynames.bin", "_pystructnames.bin"]}}
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + "\n")
    (args.build / "gate-command-outputs.json").write_text(json.dumps(logs, indent=2) + "\n")
    print(json.dumps({"validation": "PASS", "result": result, "report": str(args.report)}))

if __name__ == "__main__":
    main()
