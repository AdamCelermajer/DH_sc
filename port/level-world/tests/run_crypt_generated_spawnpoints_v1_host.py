#!/usr/bin/env python3
"""Compile and audit the generated Crypt SPWN v1 source bridge on the host."""
import argparse
import os
from pathlib import Path
import subprocess
import tempfile


TESTS = Path(__file__).resolve().parent
LEVEL_WORLD = TESTS.parent
PORT = LEVEL_WORLD.parent
REPO = PORT.parent
DEFAULT_FILES = REPO / "port/android-native/app/src/main/assets/worlds"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--files-root", type=Path, default=DEFAULT_FILES,
                        help="directory containing x07_crypt_backup.mlx and its Crypt MGP files")
    parser.add_argument("--cxx", default=os.environ.get("CXX", "g++"),
                        help="C++17 compiler executable")
    args = parser.parse_args()
    files_root = args.files_root.resolve()
    if not (files_root / "x07_crypt_backup.mlx").is_file():
        raise SystemExit(f"missing Crypt layout under {files_root}")

    with tempfile.TemporaryDirectory(prefix="dh2-crypt-spawnpoints-") as temporary:
        executable = Path(temporary) / ("crypt_spawnpoints.exe" if os.name == "nt" else "crypt_spawnpoints")
        sources = [
            PORT / "world-data/world.cpp",
            LEVEL_WORLD / "crypt_generated_spawnpoints_v1.cpp",
            TESTS / "crypt_generated_spawnpoints_v1.cpp",
        ]
        command = [args.cxx, "-std=c++17", "-O2", "-Wall", "-Wextra", "-Werror",
                   *(str(path) for path in sources), "-o", str(executable)]
        subprocess.run(command, check=True)
        subprocess.run([str(executable), str(files_root)], check=True)


if __name__ == "__main__":
    main()
