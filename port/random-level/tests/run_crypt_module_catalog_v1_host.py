#!/usr/bin/env python3
"""Build and audit the original Crypt rule candidates against a cache/files tree."""

from __future__ import annotations

import argparse
import pathlib
import shutil
import subprocess
import tempfile


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--cache-files",
        required=True,
        type=pathlib.Path,
        help="recovered cache/files directory (contains data/scene and data/3d)",
    )
    parser.add_argument(
        "--cxx",
        type=pathlib.Path,
        default=pathlib.Path(shutil.which("g++") or "g++"),
    )
    args = parser.parse_args()
    if not args.cache_files.is_dir():
        parser.error(f"cache/files directory does not exist: {args.cache_files}")

    source_root = pathlib.Path(__file__).resolve().parents[1]
    test_source = source_root / "tests" / "crypt_module_catalog_v1.cpp"
    implementation = source_root / "crypt_module_catalog_v1.cpp"
    mgx_implementation = source_root / "crypt_mgx_connectivity_v1.cpp"
    placement_implementation = source_root / "crypt_mgx_placement_v1.cpp"
    rule_implementation = source_root / "native_rule_plan_v1.cpp"

    with tempfile.TemporaryDirectory(prefix="dh2-crypt-module-catalog-") as temp_dir:
        executable = pathlib.Path(temp_dir) / "crypt_module_catalog_v1"
        compile_command = [
            str(args.cxx),
            "-std=c++17",
            "-Wall",
            "-Wextra",
            "-Werror",
            "-pedantic",
            str(implementation),
            str(mgx_implementation),
            str(placement_implementation),
            str(rule_implementation),
            str(test_source),
            "-o",
            str(executable),
        ]
        print("compile:", subprocess.list2cmdline(compile_command), flush=True)
        subprocess.run(compile_command, check=True)
        command = [str(executable), str(args.cache_files.resolve())]
        print("audit:", subprocess.list2cmdline(command), flush=True)
        result = subprocess.run(command, check=False, capture_output=True, text=True)
        if result.stdout:
            print(result.stdout, end="")
        if result.stderr:
            print(result.stderr, end="")
        return result.returncode


if __name__ == "__main__":
    raise SystemExit(main())
