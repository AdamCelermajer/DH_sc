"""Build and run the gameplay multi-touch camera finger map audit."""
from __future__ import annotations

import argparse
import os
from pathlib import Path
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[3]
SOURCE = ROOT / "port/android-native/tests/native_gameplay_touch_v1_host.cpp"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", help="C++ compiler (defaults to CXX, g++, or clang++)")
    parser.add_argument("--output", type=Path,
                        default=ROOT / "port/android-native/build/gameplay-touch/native_gameplay_touch_v1_host")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        parser.error("pass --compiler or set CXX")
    output = args.output.resolve()
    if os.name == "nt" and not output.suffix:
        output = output.with_suffix(".exe")
    output.parent.mkdir(parents=True, exist_ok=True)
    command = [compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror",
               "-pedantic", "-fno-fast-math", "-ffp-contract=off",
               str(SOURCE), "-o", str(output)]
    built = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    if built.returncode:
        raise RuntimeError(built.stdout + built.stderr)
    ran = subprocess.run([str(output)], cwd=ROOT, capture_output=True, text=True)
    if ran.returncode:
        raise RuntimeError(ran.stdout + ran.stderr)
    print(ran.stdout.strip() or "PASS: gameplay multi-touch camera finger map")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
