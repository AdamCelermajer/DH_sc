#!/usr/bin/env python3
"""Focused host regression for generated Crypt template source retention."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile


HERE = Path(__file__).resolve().parent
MODULE = HERE.parent
PORT = MODULE.parent
REPO = PORT.parent
DEFAULT_ASSETS = REPO / "port/android-native/app/src/main/assets"


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--assets-root", type=Path, default=DEFAULT_ASSETS)
    parser.add_argument("--cxx", default=os.environ.get("CXX", "g++"))
    args = parser.parse_args()
    assets = args.assets_root.resolve()
    provenance = json.loads((MODULE / "reference/character-template-catalog-v1.json")
                            .read_text(encoding="utf-8"))
    for name, expected in provenance["inputs"].items():
        raw = (assets / "data" / name).read_bytes()
        if len(raw) != expected["bytes"] or hashlib.sha256(raw).hexdigest() != expected["sha256"]:
            raise SystemExit(f"cache-derived catalog input differs from provenance: {name}")
    compiler = shutil.which(args.cxx) or (
        args.cxx if Path(args.cxx).is_file() else None)
    if compiler is None:
        raise SystemExit(f"C++ compiler not found: {args.cxx}")

    with tempfile.TemporaryDirectory(prefix="dh2-crypt-template-source-") as tmp:
        output = Path(tmp) / (
            "crypt_template_source.exe" if os.name == "nt"
            else "crypt_template_source")
        sources = [
            PORT / "world-data/world.cpp",
            PORT / "game-data/data.cpp",
            PORT / "game-data/condition_data_v1.cpp",
            PORT / "game-data/quest_condition_eval_v1.cpp",
            PORT / "game-data/quest_condition_factory_v1.cpp",
            PORT / "game-data/quest_table_bindings_v1.cpp",
            PORT / "quest-data/quests.c",
            PORT / "random-level/crypt_generated_dact_v1.cpp",
            MODULE / "character_constructor_owner_v1.cpp",
            MODULE / "character_runtime_factory_v1.cpp",
            MODULE / "crypt_conditional_character_runtime_v1.cpp",
            MODULE / "source_level_owner_v1.cpp",
            MODULE / "level_quick_save_v1.cpp",
            MODULE / "level_savegame_save_v1.cpp",
            MODULE / "object_manager_runtime_owner_v1.cpp",
            MODULE / "game_object_zoning_visibility.cpp",
            MODULE / "room_zone_enrollment.cpp",
            MODULE / "character_template_factory.cpp",
            MODULE / "character_template_catalog_v1.cpp",
            MODULE / "crypt_template_character_projection_v1.cpp",
            MODULE / "crypt_generated_source_dact_v1.cpp",
            MODULE / "crypt_spawn_trigger.cpp",
            PORT / "trigger-contact/trigger_contact.cpp",
            PORT / "zone-contact-runtime/zone_geometry.cpp",
            PORT / "script-runtime/script_runtime.cpp",
            PORT / "pydata-scripts/native/pydata_scripts.cpp",
            HERE / "crypt_generated_template_character_source_v1.cpp",
        ]
        command = [compiler, "-std=c++17", "-O2", "-Wall", "-Wextra",
                   "-Werror", "-pedantic", *(str(path) for path in sources),
                   "-o", str(output)]
        subprocess.run(command, check=True, cwd=REPO)
        result = subprocess.run([str(output), str(assets)],
                                cwd=REPO, text=True, capture_output=True)
        if result.returncode:
            raise SystemExit(result.stdout + result.stderr)
        print(result.stdout.strip())


if __name__ == "__main__":
    main()
