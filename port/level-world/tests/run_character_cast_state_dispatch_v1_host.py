#!/usr/bin/env python3
"""Compile and run the isolated cast-state event dispatcher regression."""

import argparse
import subprocess
from pathlib import Path


MODULE = Path(__file__).resolve().parents[1]
REPO = MODULE.parents[1]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", required=True, type=Path)
    parser.add_argument(
        "--output-dir",
        type=Path,
        default=MODULE / "build/character-cast-state-dispatch-v1",
    )
    args = parser.parse_args()

    output_dir = args.output_dir.resolve()
    output_dir.mkdir(parents=True, exist_ok=True)
    executable = output_dir / "character_cast_state_dispatch_v1_test"
    command = [
        str(args.compiler.resolve()),
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-Werror",
        "-fsanitize=address,undefined",
        "-fno-omit-frame-pointer",
        str(MODULE / "character_cast_state_dispatch_v1.cpp"),
        str(MODULE / "tests/character_cast_state_dispatch_v1.cpp"),
        "-o",
        str(executable),
    ]
    subprocess.run(command, cwd=REPO, check=True)
    subprocess.run([str(executable)], cwd=REPO, check=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
