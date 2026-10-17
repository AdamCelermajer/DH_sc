#!/usr/bin/env python3
"""Compile and run the recursive Crypt rule execution slice on the host."""

from __future__ import annotations

import argparse
import pathlib
import shutil
import subprocess
import tempfile


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--cxx",
        type=pathlib.Path,
        default=pathlib.Path(shutil.which("g++") or "g++"),
    )
    args = parser.parse_args()

    source_root = pathlib.Path(__file__).resolve().parents[1]
    sources = [
        source_root / "crypt_rule_generation_runtime_v1.cpp",
        source_root / "crypt_candidate_selection_v1.cpp",
        source_root / "crypt_path_runtime_v1.cpp",
        source_root / "crypt_root_rule_runtime_v1.cpp",
        source_root / "crypt_rule_distribution_v1.cpp",
        source_root / "crypt_tile_map_v1.cpp",
        source_root / "crypt_mgx_placement_v1.cpp",
        source_root / "crypt_mgx_connectivity_v1.cpp",
        source_root / "native_rule_plan_v1.cpp",
        source_root / "tests" / "crypt_rule_generation_runtime_v1.cpp",
    ]
    with tempfile.TemporaryDirectory(prefix="dh2-rule-generation-") as temp_dir:
        executable = pathlib.Path(temp_dir) / "crypt_rule_generation_runtime_v1"
        compile_command = [
            str(args.cxx),
            "-std=c++17",
            "-Wall",
            "-Wextra",
            "-Werror",
            "-pedantic",
            *[str(source) for source in sources],
            "-o",
            str(executable),
        ]
        print("compile:", subprocess.list2cmdline(compile_command), flush=True)
        subprocess.run(compile_command, check=True)
        result = subprocess.run(
            [str(executable)],
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
