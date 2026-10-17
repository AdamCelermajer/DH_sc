"""Compile and run the ObjectManager-to-Factory Character walk regression."""
from __future__ import annotations

import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

LEVEL_WORLD = Path(__file__).resolve().parents[1]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", help="C++17 host compiler")
    parser.add_argument("--output", type=Path,
        default=LEVEL_WORLD / "build" / "object_manager_character_factory_walk_v1.exe")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        parser.error("no C++ host compiler found; pass --compiler or set CXX")
    output = args.output.resolve()
    output.parent.mkdir(parents=True, exist_ok=True)
    sources = [
        LEVEL_WORLD / "tests" / "object_manager_character_factory_walk_v1.cpp",
        LEVEL_WORLD / "object_manager_character_factory_walk_v1.cpp",
        LEVEL_WORLD / "character_factory_role_query_v1.cpp",
        LEVEL_WORLD / "character_ai_classification.cpp",
        LEVEL_WORLD / "character_runtime_factory_v1.cpp",
        LEVEL_WORLD / "character_gameplay_save_v1.cpp",
        LEVEL_WORLD / ".." / "game-data" / "player_save_load_owner_v1.cpp",
        LEVEL_WORLD / ".." / "game-data" / "player_profile_index_v1.cpp",
        LEVEL_WORLD / ".." / "game-data" / "player_savegame_v1.cpp",
        LEVEL_WORLD / "character_constructor_owner_v1.cpp",
        LEVEL_WORLD / "character_net_state_owner_v1.cpp",
        LEVEL_WORLD / "object_manager_runtime_owner_v1.cpp",
        LEVEL_WORLD / "game_object_zoning_visibility.cpp",
        LEVEL_WORLD / "room_zone_enrollment.cpp",
    ]
    command = [compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror",
        "-ffunction-sections", "-fdata-sections", "-Wl,--gc-sections",
        *(str(source) for source in sources), "-o", str(output)]
    built = subprocess.run(command, text=True, capture_output=True)
    if built.stdout:
        sys.stdout.write(built.stdout)
    if built.stderr:
        sys.stderr.write(built.stderr)
    if built.returncode:
        return built.returncode
    run = subprocess.run([str(output)], text=True, capture_output=True)
    if run.stdout:
        sys.stdout.write(run.stdout)
    if run.stderr:
        sys.stderr.write(run.stderr)
    if run.returncode:
        return run.returncode
    report = json.loads(run.stdout)
    expected = {"manager_order": True, "factory_identity": True,
        "noncharacter_skipped": True, "visitor_failure_propagated": True,
        "player_prefix_predicate": True, "merchant_type_predicate": True,
        "role_identity_gate": True}
    if report != expected:
        raise SystemExit(f"unexpected regression result: {report}")
    print(json.dumps({"validation": "PASS", "host_report": report}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
