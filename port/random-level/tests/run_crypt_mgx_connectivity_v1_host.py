#!/usr/bin/env python3
"""Compile and run the bounded Crypt MGX adjacency audit on a host compiler."""

from __future__ import annotations

import argparse
import pathlib
import shutil
import subprocess
import tempfile


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--mgx-dir", required=True, type=pathlib.Path)
    parser.add_argument("--cxx", type=pathlib.Path, default=pathlib.Path(shutil.which("g++") or "g++"))
    args = parser.parse_args()

    source_root = pathlib.Path(__file__).resolve().parents[1]
    test_source = source_root / "tests" / "crypt_mgx_connectivity_v1.cpp"
    implementation = source_root / "crypt_mgx_connectivity_v1.cpp"
    placement_implementation = source_root / "crypt_mgx_placement_v1.cpp"
    with tempfile.TemporaryDirectory(prefix="dh2-crypt-mgx-") as temp_dir:
        executable = pathlib.Path(temp_dir) / "crypt_mgx_connectivity_v1"
        compile_command = [
            str(args.cxx),
            "-std=c++17",
            "-Wall",
            "-Wextra",
            "-Werror",
            "-pedantic",
            str(implementation),
            str(placement_implementation),
            str(test_source),
            "-o",
            str(executable),
        ]
        print("compile:", subprocess.list2cmdline(compile_command), flush=True)
        subprocess.run(compile_command, check=True)
        result = subprocess.run(
            [str(executable), str(args.mgx_dir.resolve())],
            check=False,
            capture_output=True,
            text=True,
        )
        if result.stdout:
            print(result.stdout, end="")
        if result.stderr:
            print(result.stderr, end="")
        return result.returncode


if __name__ == "__main__":
    raise SystemExit(main())
