#!/usr/bin/env python3
"""Build the header-only Level/QEST frame-order and provenance regression."""
import argparse
from pathlib import Path
import shutil
import subprocess

HERE = Path(__file__).resolve().parent


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", default="g++")
    parser.add_argument("--output", type=Path,
                        default=HERE / "build/native-level-sg-update-v1")
    args = parser.parse_args()
    compiler = shutil.which(args.compiler)
    if not compiler:
        raise RuntimeError(f"C++ compiler missing: {args.compiler}")
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    executable = output / "native-level-sg-update-v1-host.exe"
    source = HERE / "native_level_sg_update_v1_host.cpp"
    command = [compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror",
               "-pedantic", str(source), "-o", str(executable)]
    subprocess.run(command, check=True)
    result = subprocess.run([str(executable)], check=True, capture_output=True,
                            text=True)
    print(result.stdout.strip())


if __name__ == "__main__":
    main()
