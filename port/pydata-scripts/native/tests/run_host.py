#!/usr/bin/env python3
"""Compile the C ABI decoder without C++ runtime support and run host corpus tests."""

import argparse
import os
from pathlib import Path
import shutil
import subprocess
import tempfile


HERE = Path(__file__).resolve().parent
NATIVE = HERE.parent


def compiler(env_name: str, default: str) -> str:
    requested = os.environ.get(env_name, default)
    resolved = shutil.which(requested)
    if resolved:
        return resolved
    if Path(requested).is_file():
        return str(Path(requested).resolve())
    raise SystemExit(f"compiler not found: {requested} (set {env_name} to override)")


def run(command: list[str]) -> None:
    print("+", subprocess.list2cmdline(command), flush=True)
    subprocess.run(command, check=True)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--cache",
        type=Path,
        default=HERE.parents[4] / "cache" / "files",
        help="cache/files directory containing the recovered PyData tables",
    )
    args = parser.parse_args()
    cache = args.cache.resolve()
    common_names = cache / "data" / "pydata" / "scripts_pyscriptnames.bin"
    common_programs = cache / "data" / "pydata" / "scripts_pyscripts.bin"
    swamp_names = cache / "data" / "pydata" / "scripts" / "001_swamp_pyscriptnames.bin"
    swamp_programs = cache / "data" / "pydata" / "scripts" / "001_swamp_pyscripts.bin"
    for path in (common_names, common_programs, swamp_names, swamp_programs):
        if not path.is_file():
            raise SystemExit(f"missing corpus input: {path}")

    cxx = compiler("CXX", "c++")
    cc = compiler("CC", "gcc")
    with tempfile.TemporaryDirectory(prefix="dh2-native-pydata-") as temp:
        temp_dir = Path(temp)
        object_file = temp_dir / "pydata_scripts.o"
        executable = temp_dir / "corpus_runner"
        run([
            cxx, "-std=c++11", "-Wall", "-Wextra", "-Werror",
            "-fno-exceptions", "-fno-rtti", "-c",
            str(NATIVE / "pydata_scripts.cpp"), "-o", str(object_file),
        ])
        run([
            cc, "-std=c99", "-Wall", "-Wextra", "-Werror",
            str(HERE / "corpus_runner.c"), str(object_file), "-o", str(executable),
        ])
        run([
            str(executable), str(common_names), str(common_programs),
            str(swamp_names), str(swamp_programs),
        ])
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
