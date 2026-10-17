#!/usr/bin/env python3
"""Compile and run the generated Crypt layout serializer host audit."""

from __future__ import annotations

import argparse
import pathlib
import shutil
import subprocess
import tempfile


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--cxx", type=pathlib.Path, default=pathlib.Path(shutil.which("g++") or "g++")
    )
    args = parser.parse_args()

    repo = pathlib.Path(__file__).resolve().parents[3]
    source_root = repo / "port" / "random-level"
    command = [
        str(args.cxx),
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-Werror",
        "-pedantic",
        str(source_root / "crypt_generated_layout_v1.cpp"),
        str(repo / "port" / "world-data" / "world.cpp"),
        str(source_root / "tests" / "crypt_generated_layout_v1.cpp"),
        "-o",
    ]
    with tempfile.TemporaryDirectory(prefix="dh2-generated-layout-") as temp_dir:
        executable = pathlib.Path(temp_dir) / "crypt_generated_layout_v1"
        compile_command = command + [str(executable)]
        print("compile:", subprocess.list2cmdline(compile_command), flush=True)
        subprocess.run(compile_command, check=True)
        result = subprocess.run(
            [str(executable)], check=False, capture_output=True, text=True
        )
        if result.stdout:
            print(result.stdout, end="")
        if result.stderr:
            print(result.stderr, end="")
        return result.returncode


if __name__ == "__main__":
    raise SystemExit(main())
