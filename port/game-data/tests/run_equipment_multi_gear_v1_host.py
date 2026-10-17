"""Build selected game-data and verify multi-item live gear recomputation."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[3]

def run(command, env):
    r = subprocess.run([str(x) for x in command], cwd=ROOT, env=env,
                       capture_output=True, text=True)
    if r.returncode:
        raise RuntimeError(r.stderr or r.stdout)
    return r.stdout

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--cache", type=Path, required=True)
    ap.add_argument("--build", type=Path, required=True)
    ap.add_argument("--compiler", type=Path, required=True)
    ap.add_argument("--report", type=Path, required=True)
    a = ap.parse_args()
    a.build.mkdir(parents=True, exist_ok=True)
    env = os.environ.copy()
    env["PATH"] = str(a.compiler.parent) + os.pathsep + env.get("PATH", "")
    run(["cmake", "-S", ROOT / "port/game-data/tests/equipment-multi-gear-v1",
         "-B", a.build, "-G", "Ninja", "-DCMAKE_BUILD_TYPE=RelWithDebInfo",
         "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
         "-DCMAKE_CXX_COMPILER=" + str(a.compiler),
         "-DCMAKE_C_COMPILER=" + str(a.compiler.with_name("gcc.exe"))], env)
    commands = json.loads((a.build / "compile_commands.json").read_text())
    selected = [x for x in commands if "CMakeFiles/dh2_game_data.dir" in x["command"].replace("\\", "/")]
    selected_files = {Path(x["file"]).name for x in selected}
    for source in ("fresh_inventory_owned_v4.cpp", "player_gear_effects_v5.cpp",
                   "player_equipment_live_services_v1.cpp", "player_equipment_queries_live_v1.cpp",
                   "item_gear_properties_v5.cpp"):
        if source not in selected_files:
            raise AssertionError("source missing from selected DSO: " + source)
    run(["cmake", "--build", a.build, "--parallel", "1", "--target", "equipment_multi_gear_v1"], env)
    library = a.build / "game-data/libdh2_game_data.dll"
    env["PATH"] = str(library.parent) + os.pathsep + env["PATH"]
    result = json.loads(run([a.build / "equipment_multi_gear_v1", a.cache], env))
    if result.get("validation") != "PASS":
        raise AssertionError("multi-gear host fixture did not pass")
    report = {"validation": "PASS", "scope": "selected host DSO; source-authored Item/ItemPower rows; V4 inventory; one Character PropertyView; power generation separately covered by AddLoot fixture", "result": result,
              "selected_library": {"path": str(library), "sha256": sha(library)},
              "selected_sources": sorted(x for x in selected_files if x in {
                  "fresh_inventory_owned_v4.cpp", "player_gear_effects_v5.cpp",
                  "player_equipment_live_services_v1.cpp", "player_equipment_queries_live_v1.cpp",
                  "item_gear_properties_v5.cpp"})}
    a.report.parent.mkdir(parents=True, exist_ok=True)
    a.report.write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report))

if __name__ == "__main__":
    main()
