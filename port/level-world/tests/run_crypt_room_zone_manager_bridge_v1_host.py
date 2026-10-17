#!/usr/bin/env python3
"""Compile and run the bounded RoomZone/ObjectManager bridge host test."""
from __future__ import annotations

import argparse
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[3]
MODULE = ROOT / "port/level-world"


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", required=True)
    parser.add_argument(
        "--output",
        type=Path,
        default=MODULE / "build/crypt-room-zone-manager-bridge-v1-host",
    )
    args = parser.parse_args()
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    executable = output / "crypt-room-zone-manager-bridge-v1-host.exe"
    subprocess.run(
        [
            args.compiler,
            "-std=c++17",
            "-O1",
            "-fno-fast-math",
            "-ffp-contract=off",
            "-Wall",
            "-Wextra",
            "-Werror",
            "-pedantic",
            "-fno-rtti",
            str(MODULE / "room_zone_enrollment.cpp"),
            str(MODULE / "module_room_zone_bounds.cpp"),
            str(MODULE / "crypt_room_zone_owner_v1.cpp"),
            str(MODULE / "object_manager_runtime_owner_v1.cpp"),
            str(MODULE / "crypt_room_zone_manager_bridge_v1.cpp"),
            str(MODULE / "tests/crypt_room_zone_manager_bridge_v1.cpp"),
            "-o",
            str(executable),
        ],
        check=True,
    )
    subprocess.run([str(executable)], check=True)


if __name__ == "__main__":
    main()
