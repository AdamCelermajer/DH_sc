#!/usr/bin/env python3
"""Build and run the exact Crypt GhostAmbush01 source-trigger host slice."""

import argparse
import hashlib
import json
import math
import os
from pathlib import Path
import shutil
import subprocess
import xml.etree.ElementTree as ET


HERE = Path(__file__).resolve().parent
PORT = HERE.parent.parent
REPO = HERE.parents[2]
NATIVE = PORT / "pydata-scripts" / "native"
SOURCE_NAME = "_prim_TriggerZone_GhostAmbush01"
SOURCE_SCRIPT = "GhostAmbush01"


def compiler(requested: str) -> str:
    found = shutil.which(requested)
    if found:
        return found
    if Path(requested).is_file():
        return str(Path(requested).resolve())
    raise SystemExit(f"C++ compiler not found: {requested}")


def close(actual: float, expected: float, epsilon: float = 1e-4) -> bool:
    return math.isfinite(actual) and abs(actual - expected) <= epsilon


def vector(value: str) -> tuple[float, float, float]:
    parts = tuple(float(part) for part in value.split(","))
    if len(parts) != 3:
        raise SystemExit(f"expected a three-component vector, got {value!r}")
    return parts


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for block in iter(lambda: source.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def validate_source(cache: Path) -> dict:
    mgp_path = cache / "data" / "3d" / "modules" / "crypt" / "mgp" / "crypt_straight_c_ns_01.mgp"
    layout_path = cache / "data" / "scene" / "x07_crypt_backup.mlx"
    for path in (mgp_path, layout_path):
        if not path.is_file():
            raise SystemExit(f"missing original cache input: {path}")

    mgp = ET.parse(mgp_path).getroot()
    matches = [node.attrib for node in mgp.findall("GameObject")
               if node.attrib.get("name") == SOURCE_NAME]
    if len(matches) != 1:
        raise SystemExit(f"expected one {SOURCE_NAME}; found {len(matches)}")
    trigger = matches[0]
    expected = {
        "gametype": "TriggerZone",
        "_templateName": "TriggerZone",
        "script": SOURCE_SCRIPT,
        "triggercount": "1",
        "triggerdelay": "0",
        "script_move_out": "",
        "script_all_player": "",
        "script_all_player_move_out": "",
        "effect_one_player": "",
    }
    for key, value in expected.items():
        if trigger.get(key) != value:
            raise SystemExit(f"unexpected MGP field {key}: {trigger.get(key)!r}")
    if "dimensions" in trigger:
        raise SystemExit("Crypt trigger unexpectedly overrides Zone dimensions")
    if any(key in trigger for key in ("door", "door_name", "associated_door")):
        raise SystemExit("Crypt trigger unexpectedly has a door binding")

    local_position = vector(trigger["position"])
    scale = vector(trigger["scale"])
    rotation = vector(trigger["rotation"])
    if not all(close(a, b) for a, b in zip(local_position, (-1405.23, 300.262, 608.062))):
        raise SystemExit(f"unexpected Crypt trigger local position: {local_position}")
    if not all(close(a, b) for a, b in zip(scale, (5.06089, 1.43277, 1.0))):
        raise SystemExit(f"unexpected Crypt trigger object scale: {scale}")
    if not all(close(value, 0.0) for value in rotation):
        raise SystemExit(f"unexpected Crypt trigger local rotation: {rotation}")

    layout = ET.parse(layout_path).getroot()
    module_file = "crypt_straight_c_ns_01.mgp"
    modules = [node.attrib for node in layout.findall("GameObject")
               if node.attrib.get("gametype") == "Module" and
               node.attrib.get("mgp", "").replace("data/iphone/", "data/").lower().endswith(module_file)]
    if len(modules) != 1:
        raise SystemExit(f"expected one canonical x07 module placement; found {len(modules)}")
    module = modules[0]
    module_position = vector(module["position"])
    module_scale = vector(module["scale"])
    module_rotation = vector(module["rotation"])
    if not all(close(a, b) for a, b in zip(module_position, (0.0, 19200.0, 0.0))):
        raise SystemExit(f"unexpected x07 backup placement: {module_position}")
    if not all(close(value, 1.0) for value in module_scale):
        raise SystemExit(f"unexpected x07 module scale: {module_scale}")
    if not all(close(value, 0.0) for value in module_rotation):
        raise SystemExit(f"unexpected x07 module rotation: {module_rotation}")
    world_position = tuple(a + b for a, b in zip(module_position, local_position))

    return {
        "mgp_cache_path": str(mgp_path),
        "mgp_sha256": sha256(mgp_path),
        "layout_cache_path": str(layout_path),
        "layout_sha256": sha256(layout_path),
        "trigger_name": SOURCE_NAME,
        "script_name": SOURCE_SCRIPT,
        "local_position": local_position,
        "scale": scale,
        "rotation": rotation,
        "canonical_backup_module_position": module_position,
        "resolved_backup_world_position": world_position,
        "transform_note": "This is the authored x07 backup layout fixture. Runtime-generated layouts must pass the loader-resolved owner transform to the adapter.",
    }


def run(command: list[str]) -> None:
    print("+", subprocess.list2cmdline(command), flush=True)
    subprocess.run(command, check=True)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--cache", type=Path,
        default=REPO.parent / "cache" / "files",
        help="cache/files containing the original Crypt MGP and PyData tables",
    )
    parser.add_argument(
        "--output", type=Path,
        default=PORT / "level-world" / "build" / "crypt-spawn-trigger",
        help="directory for the host executable and validation report",
    )
    parser.add_argument("--compiler", default=os.environ.get("CXX", "c++"))
    args = parser.parse_args()
    cache = args.cache.resolve()
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    source = validate_source(cache)

    data = cache / "data" / "pydata"
    common_names = data / "scripts_pyscriptnames.bin"
    common_programs = data / "scripts_pyscripts.bin"
    crypt_names = data / "scripts" / "007_crypt_01_pyscriptnames.bin"
    crypt_programs = data / "scripts" / "007_crypt_01_pyscripts.bin"
    tables = (common_names, common_programs, crypt_names, crypt_programs)
    for path in tables:
        if not path.is_file():
            raise SystemExit(f"missing original PyData input: {path}")

    executable = output / "crypt_spawn_trigger_host.exe"
    test_sources = [
        HERE / "crypt_spawn_trigger.cpp",
        PORT / "level-world" / "crypt_spawn_trigger.cpp",
        PORT / "trigger-contact" / "trigger_contact.cpp",
        PORT / "zone-contact-runtime" / "zone_geometry.cpp",
        PORT / "script-runtime" / "script_runtime.cpp",
        NATIVE / "pydata_scripts.cpp",
    ]
    command = [
        compiler(args.compiler), "-std=c++11", "-O2", "-Wall", "-Wextra",
        "-Werror", "-fno-exceptions", "-fno-rtti",
        *(str(path) for path in test_sources), "-o", str(executable),
    ]
    run(command)
    run([str(executable), *(str(path) for path in tables)])

    report = {
        "status": "passed",
        "compiler": command[0],
        "compile_command": command,
        "test_command": [str(executable), *(str(path) for path in tables)],
        "source": source,
        "pydata_tables": {str(path): sha256(path) for path in tables},
        "scope": "host-only exact trigger geometry/contact and original GhostAmbush01 scheduler activation; no emulator or Android runtime integration",
    }
    report_path = output / "validation.json"
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"validation report: {report_path}")
    print("verified source-backed Crypt GhostAmbush01 trigger contact and edge")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
