#!/usr/bin/env python3
"""Build and test the host trigger-contact adapter against the original cache."""

import argparse
import math
import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import xml.etree.ElementTree as ET


HERE = Path(__file__).resolve().parent
PORT = HERE.parent.parent
REPO = HERE.parents[2]
NATIVE = PORT / "pydata-scripts" / "native"


def compiler() -> str:
    requested = os.environ.get("CXX", "c++")
    found = shutil.which(requested)
    if found:
        return found
    if Path(requested).is_file():
        return str(Path(requested).resolve())
    raise SystemExit(f"C++ compiler not found: {requested}; set CXX to override")


def close(actual: float, expected: float) -> bool:
    return math.isclose(actual, expected, rel_tol=0.0, abs_tol=1e-4)


def validate_source(cache: Path) -> None:
    level_path = cache / "data" / "scene" / "005_infectedvillage.mlx"
    level = ET.parse(level_path).getroot()
    modules = [obj for obj in level.findall("GameObject")
               if obj.attrib.get("name") == "_module_infectedvillage_01_001"]
    if len(modules) != 1:
        raise SystemExit(f"expected one module 01 placement; found {len(modules)}")
    module = modules[0]
    module_pos = tuple(float(value) for value in module.attrib["position"].split(","))
    module_scale = tuple(float(value) for value in module.attrib["scale"].split(","))
    module_rotation = tuple(float(value) for value in module.attrib["rotation"].split(","))
    if not close(module_pos[0], -3448.5) or not close(module_pos[1], 3000.0) or not close(module_pos[2], 0.0):
        raise SystemExit(f"unexpected module placement: {module_pos}")
    if not all(close(value, 1.0) for value in module_scale):
        raise SystemExit(f"unexpected module scale: {module_scale}")
    if not all(close(value, 0.0) for value in module_rotation):
        raise SystemExit(f"unexpected module rotation: {module_rotation}")
    relative = module.attrib["mgp"].replace("data/iphone/", "data/").lower()
    mgp = ET.parse(cache / relative).getroot()
    matches = [obj.attrib for obj in mgp.findall("GameObject")
               if obj.attrib.get("name") == "_prim_TriggerZone_ambush"]
    if len(matches) != 1:
        raise SystemExit(f"expected one Infected Village Ambush trigger; found {len(matches)}")
    trigger = matches[0]
    expected = {
        "gametype": "TriggerZone",
        "script": "Ambush",
        "triggercount": "1",
        "triggerdelay": "0",
        "script_move_out": "",
        "script_all_player": "",
        "script_all_player_move_out": "",
        "effect_one_player": "",
    }
    for key, value in expected.items():
        if trigger.get(key) != value:
            raise SystemExit(f"unexpected source field {key}: {trigger.get(key)!r}")
    if "dimensions" in trigger:
        raise SystemExit("trigger unexpectedly overrides inherited Zone dimensions")
    local_pos = tuple(float(value) for value in trigger["position"].split(","))
    scale = tuple(float(value) for value in trigger["scale"].split(","))
    rotation = tuple(float(value) for value in trigger["rotation"].split(","))
    if not all(close(a, b) for a, b in zip(local_pos, (5993.8, -1889.17, 1313.39))):
        raise SystemExit(f"unexpected trigger position: {local_pos}")
    if not all(close(a, b) for a, b in zip(scale, (1.92682, 1.92682, 1.0))):
        raise SystemExit(f"unexpected trigger scale: {scale}")
    if not all(close(value, 0.0) for value in rotation):
        raise SystemExit(f"unexpected trigger rotation: {rotation}")
    world_pos = tuple(a + b for a, b in zip(module_pos, local_pos))
    if not all(close(a, b) for a, b in zip(world_pos, (2545.3, 1110.83, 1313.39))):
        raise SystemExit(f"unexpected translated trigger position: {world_pos}")


def run(command: list[str]) -> None:
    print("+", subprocess.list2cmdline(command), flush=True)
    subprocess.run(command, check=True)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--cache", type=Path,
        default=REPO.parent / "cache" / "files",
        help="cache/files directory containing the recovered game data",
    )
    args = parser.parse_args()
    cache = args.cache.resolve()
    validate_source(cache)

    data = cache / "data" / "pydata"
    common_names = data / "scripts_pyscriptnames.bin"
    common_programs = data / "scripts_pyscripts.bin"
    level_names = data / "scripts" / "005_infectedvillage_pyscriptnames.bin"
    level_programs = data / "scripts" / "005_infectedvillage_pyscripts.bin"
    required = (common_names, common_programs, level_names, level_programs)
    for path in required:
        if not path.is_file():
            raise SystemExit(f"missing original cache file: {path}")

    with tempfile.TemporaryDirectory(prefix="dh2-trigger-contact-") as temp:
        executable = Path(temp) / "trigger_contact_test"
        run([
            compiler(), "-std=c++11", "-Wall", "-Wextra", "-Werror",
            "-fno-exceptions", "-fno-rtti",
            str(HERE / "trigger_contact_test.cpp"),
            str(HERE.parent / "trigger_contact.cpp"),
            str(PORT / "zone-contact-runtime" / "zone_geometry.cpp"),
            str(PORT / "script-runtime" / "script_runtime.cpp"),
            str(NATIVE / "pydata_scripts.cpp"),
            "-o", str(executable),
        ])
        run([str(executable), str(common_names), str(common_programs),
             str(level_names), str(level_programs)])

        vertical_slice = Path(temp) / "ambush_vertical_slice_test"
        vertical_sources = [
            HERE / "ambush_vertical_slice_test.cpp",
            HERE.parent / "trigger_contact.cpp",
            PORT / "zone-contact-runtime" / "zone_geometry.cpp",
            HERE.parent / "ambush_vertical_slice.cpp",
            PORT / "script-runtime" / "script_runtime.cpp",
            NATIVE / "pydata_scripts.cpp",
            PORT / "actor-runtime" / "actor_registry.cpp",
            PORT / "world-data" / "world.cpp",
            PORT / "world-data" / "world_scene.cpp",
            PORT / "scene-payloads" / "scene.cpp",
            PORT / "engine-resources" / "resources.cpp",
            PORT / "engine-math" / "math.cpp",
        ]
        run([
            compiler(), "-std=c++17", "-O2", "-fno-exceptions", "-fno-rtti",
            "-fno-fast-math", "-ffp-contract=off", "-fno-builtin",
            "-Wall", "-Wextra", "-Werror",
            *(str(path) for path in vertical_sources), "-lm", "-o", str(vertical_slice),
        ])
        run([str(vertical_slice), str(cache), str(common_names), str(common_programs),
             str(level_names), str(level_programs)])
    print("verified source-backed contact adapter and Ambush-to-actor host slice")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
