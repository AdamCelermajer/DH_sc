#!/usr/bin/env python3
"""Compile and run focused source ProjectileManager pool-owner checks."""
from __future__ import annotations

import argparse
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[3]
MODULE = ROOT / "port/level-world"
ASSETS = ROOT / "port/android-native/app/src/main/assets/data"
GAME_DATA = ROOT / "port/game-data"


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    executable = output / "projectile-manager-runtime-owner-v1-host.exe"
    subprocess.run(
        [
            args.compiler,
            "-std=c++17",
            "-O1",
            "-Wall",
            "-Wextra",
            "-Werror",
            "-pedantic",
            "-fno-rtti",
            str(MODULE / "object_manager_runtime_owner_v1.cpp"),
            str(MODULE / "object_creation_map_v1.cpp"),
            str(MODULE / "projectile_lifecycle_runtime_v1.cpp"),
            str(MODULE / "projectile_factory_owner_v1.cpp"),
            str(MODULE / "projectile_manager_runtime_owner_v1.cpp"),
            str(GAME_DATA / "data.cpp"),
            str(GAME_DATA / "projectile_table_v1.cpp"),
            str(MODULE / "tests/projectile_manager_runtime_owner_v1.cpp"),
            "-o",
            str(executable),
        ],
        check=True,
    )
    subprocess.run([str(executable), str(ASSETS)], check=True)


if __name__ == "__main__":
    main()
