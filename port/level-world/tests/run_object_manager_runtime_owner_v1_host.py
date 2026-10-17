#!/usr/bin/env python3
"""Compile and run the isolated ObjectManager runtime owner host checks."""
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
        default=MODULE / "build/object-manager-runtime-owner-v1-host",
    )
    args = parser.parse_args()
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    executable = output / "object-manager-runtime-owner-v1-host.exe"
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
            str(MODULE / "tests/object_manager_runtime_owner_v1.cpp"),
            "-o",
            str(executable),
        ],
        check=True,
    )
    subprocess.run([str(executable)], check=True)


if __name__ == "__main__":
    main()
