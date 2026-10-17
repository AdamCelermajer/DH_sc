"""Build and run the bounded equipment slot/category source-table fixture."""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[3]

def run(command, env):
    result = subprocess.run([str(x) for x in command], cwd=ROOT, env=env,
                            capture_output=True, text=True)
    if result.returncode:
        raise RuntimeError(result.stderr or result.stdout)
    return result.stdout

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--loot-cache", type=Path, required=True)
    ap.add_argument("--data-assets", type=Path, required=True)
    ap.add_argument("--power-cache", type=Path, required=True)
    ap.add_argument("--build", type=Path, required=True)
    ap.add_argument("--compiler", type=Path, required=True)
    ap.add_argument("--report", type=Path, required=True)
    a = ap.parse_args()
    a.build.mkdir(parents=True, exist_ok=True)
    source = a.build / "source-cache"
    source.mkdir(parents=True, exist_ok=True)
    for stem, directory in (("loot_table", a.loot_cache), ("item_powers", a.power_cache),
                            ("character_properties", a.data_assets), ("character_classes", a.data_assets)):
        for suffix in ("_pyarray.bin", "_pyarraynames.bin", "_pystructnames.bin"):
            shutil.copyfile(directory / (stem + suffix), source / (stem + suffix))
    env = os.environ.copy()
    env["PATH"] = str(a.compiler.parent) + os.pathsep + env.get("PATH", "")
    run(["cmake", "-S", ROOT / "port/game-data/tests/equipment-slot-resolution-v1",
         "-B", a.build, "-G", "Ninja", "-DCMAKE_BUILD_TYPE=RelWithDebInfo",
         "-DCMAKE_CXX_COMPILER=" + str(a.compiler),
         "-DCMAKE_C_COMPILER=" + str(a.compiler.with_name("gcc.exe"))], env)
    run(["cmake", "--build", a.build, "--parallel", "1", "--target",
         "equipment_slot_resolution_v1"], env)
    library = a.build / "game-data/libdh2_game_data.dll"
    env["PATH"] = str(library.parent) + os.pathsep + env["PATH"]
    result = json.loads(run([a.build / "equipment_slot_resolution_v1", source], env))
    if result.get("validation") != "PASS":
        raise RuntimeError("equipment-slot test did not pass")
    report = {"fixture": "source item_table rows with V4/PlayerEquipmentLiveV1 and one Character PropertyView",
              "result": result, "selected_library": str(library)}
    a.report.parent.mkdir(parents=True, exist_ok=True)
    a.report.write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report))

if __name__ == "__main__":
    main()
