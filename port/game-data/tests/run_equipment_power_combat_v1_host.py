"""Verify a real table power flows from V4 equipment into resolved combat damage."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[3]

def run(args, env):
    result = subprocess.run([str(x) for x in args], cwd=ROOT, env=env,
                            capture_output=True, text=True,
                            encoding="utf-8", errors="replace")
    if result.returncode:
        raise RuntimeError(result.stderr or result.stdout)
    return result.stdout

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path, required=True, help="loot table pyarray cache")
    parser.add_argument("--power-cache", type=Path, required=True, help="item power pyarray cache")
    parser.add_argument("--source-data", type=Path, required=True, help="character properties/classes pyarrays")
    parser.add_argument("--build", type=Path, required=True)
    parser.add_argument("--compiler", type=Path, required=True)
    parser.add_argument("--report", type=Path, required=True)
    a = parser.parse_args()
    a.build.mkdir(parents=True, exist_ok=True)
    env = os.environ.copy()
    env["PATH"] = str(a.compiler.parent) + os.pathsep + env.get("PATH", "")
    run(["cmake", "-S", ROOT / "port/game-data/tests/equipment-power-combat-v1",
         "-B", a.build, "-G", "Ninja", "-DCMAKE_BUILD_TYPE=RelWithDebInfo",
         "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON", "-DCMAKE_CXX_COMPILER=" + str(a.compiler),
         "-DCMAKE_C_COMPILER=" + str(a.compiler.with_name("gcc.exe"))], env)
    commands = json.loads((a.build / "compile_commands.json").read_text())
    selected = [c for c in commands if "CMakeFiles/dh2_game_data.dir" in c["command"].replace("\\", "/")]
    files = {Path(c["file"]).name for c in selected}
    required = {"fresh_inventory_owned_v4.cpp", "player_gear_effects_v5.cpp",
                "player_equipment_live_services_v1.cpp", "player_equipment_queries_live_v1.cpp",
                "item_gear_properties_v5.cpp", "combat.cpp"}
    if not required <= files:
        raise AssertionError("selected DSO missing: " + ", ".join(sorted(required-files)))
    run(["cmake", "--build", a.build, "--parallel", "1", "--target", "equipment_power_combat_v1"], env)
    library = a.build / "game-data/libdh2_game_data.dll"
    env["PATH"] = str(library.parent) + os.pathsep + env["PATH"]
    output = run([a.build / "equipment_power_combat_v1", a.cache, a.source_data, a.power_cache], env)
    result = json.loads(output)
    if result.get("validation") != "PASS" or result.get("powered_damage", 0) <= result.get("bare_damage", 0):
        raise AssertionError("powered damage did not increase")
    report = {"validation": "PASS", "scope": "source ItemPower table row type28 -> V4 equipped item -> canonical resolved PropertyView -> selected DSO combat kernel; host only",
              "result": result, "selected_library": {"path": str(library), "sha256": hashlib.sha256(library.read_bytes()).hexdigest()},
              "selected_sources": sorted(required)}
    a.report.parent.mkdir(parents=True, exist_ok=True)
    a.report.write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report))

if __name__ == "__main__":
    main()
