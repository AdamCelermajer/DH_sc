#!/usr/bin/env python3
"""Compile the source C++ slice and compose its events with Spawn-state logic."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
MODULE = HERE.parent
PORT = MODULE.parent
REPO = PORT.parent
sys.path.insert(0, str(PORT / "actor-spawn-runtime"))
sys.path.insert(0, str(MODULE))

from actor_spawn_runtime import (  # noqa: E402
    parse_character_templates,
    parse_integer_constants,
    parse_name_tables,
)
from ambush_spawn_composition import compose_projection  # noqa: E402


def compiler() -> str:
    requested = os.environ.get("CXX", "c++")
    found = shutil.which(requested)
    if found:
        return found
    if Path(requested).is_file():
        return str(Path(requested).resolve())
    raise SystemExit(f"C++ compiler not found: {requested}; set CXX to override")


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path, default=REPO.parent / "cache" / "files")
    parser.add_argument(
        "--report", type=Path,
        default=MODULE / "build" / "ambush-spawn-runtime-validation.json",
    )
    args = parser.parse_args()
    cache = args.cache.resolve()
    pydata = cache / "data" / "pydata"
    script_data = pydata / "scripts"
    paths = {
        "common_names": pydata / "scripts_pyscriptnames.bin",
        "common_programs": pydata / "scripts_pyscripts.bin",
        "level_names": script_data / "005_infectedvillage_pyscriptnames.bin",
        "level_programs": script_data / "005_infectedvillage_pyscripts.bin",
        "templates": pydata / "character_templates_pyarray.bin",
        "template_names": pydata / "character_templates_pyarraynames.bin",
        "property_names": pydata / "character_properties_pyarraynames.bin",
        "ai_constants": pydata / "ai_pycst.bin",
    }
    for path in paths.values():
        if not path.is_file():
            raise SystemExit(f"missing cache input: {path}")

    sources = [
        MODULE / "ambush_spawn_composition.cpp",
        PORT / "trigger-contact" / "trigger_contact.cpp",
        PORT / "trigger-contact" / "ambush_vertical_slice.cpp",
        PORT / "zone-contact-runtime" / "zone_geometry.cpp",
        PORT / "script-runtime" / "script_runtime.cpp",
        PORT / "pydata-scripts" / "native" / "pydata_scripts.cpp",
        PORT / "actor-runtime" / "actor_registry.cpp",
        PORT / "world-data" / "world.cpp",
        PORT / "world-data" / "world_scene.cpp",
        PORT / "scene-payloads" / "scene.cpp",
        PORT / "engine-resources" / "resources.cpp",
        PORT / "engine-math" / "math.cpp",
    ]
    flags = [
        "-std=c++17", "-O2", "-fno-exceptions", "-fno-rtti",
        "-fno-fast-math", "-ffp-contract=off", "-fno-builtin",
        "-Wall", "-Wextra", "-Werror",
    ]
    arguments = [
        str(cache), str(paths["common_names"]), str(paths["common_programs"]),
        str(paths["level_names"]), str(paths["level_programs"]),
    ]
    with tempfile.TemporaryDirectory(prefix="dh2-ambush-spawn-") as temp:
        executable = Path(temp) / ("ambush_spawn_composition.exe" if os.name == "nt"
                                   else "ambush_spawn_composition")
        build = [compiler(), *flags, *(str(source) for source in sources),
                 "-lm", "-o", str(executable)]
        print("+", subprocess.list2cmdline(build), flush=True)
        subprocess.run(build, check=True)
        command = [str(executable), *arguments]
        print("+", subprocess.list2cmdline(command), flush=True)
        completed = subprocess.run(command, check=False, capture_output=True, text=True)
        if completed.returncode != 0:
            print(f"source composition executable exited {completed.returncode}",
                  file=sys.stderr)
            if completed.stderr:
                print(completed.stderr, file=sys.stderr, end="")
            raise SystemExit(completed.returncode)

    try:
        projection = json.loads(completed.stdout)
    except json.JSONDecodeError as exc:
        raise SystemExit(f"source composition executable returned invalid JSON: {exc}") from exc

    raw = {key: path.read_bytes() for key, path in paths.items()}
    property_tables = parse_name_tables(raw["property_names"])
    if len(property_tables) != 3:
        raise SystemExit("expected three concatenated Character property name tables")
    templates = parse_character_templates(
        raw["templates"], raw["template_names"], raw["property_names"]
    )
    constants = parse_integer_constants(raw["ai_constants"])
    assembly = REPO / "recovered/native/assembly/libDungeonHunter2.so"
    character_asm = (assembly / "Character-1405a63e8a78-001.asm").read_text(
        encoding="utf-8"
    )
    state_machine_asm = (
        assembly / "CharStateMachine-9e67f9b0cab6-001.asm"
    ).read_text(encoding="utf-8")
    if ("FUNCTION 0x003a9340" not in character_asm or
            "bl #0x3c7318" not in character_asm or
            "cmp r6, #0x14" not in character_asm):
        raise SystemExit("Character constructor state-registration loop evidence changed")
    if ("FUNCTION 0x003c7318" not in state_machine_asm or
            "cmp r3, #0x14" not in state_machine_asm):
        raise SystemExit("per-machine RegisterState range evidence changed")
    registered_state_ids = tuple(range(20))
    result = compose_projection(
        projection, templates, property_tables[0], constants.get("AIStates", {}),
        registered_state_ids=registered_state_ids,
    )
    result["native_state_registration"] = {
        "constructor_elf": "0x3a9340",
        "register_state_elf": "0x3c7318",
        "per_character_state_ids": list(registered_state_ids),
        "state_implementations_shared_singletons": True,
    }
    result["projection"] = projection
    result["cache_inputs"] = {
        key: {"bytes": len(data), "sha256": sha256(data)}
        for key, data in raw.items()
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(result, indent=2))
    print(f"wrote {args.report}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
