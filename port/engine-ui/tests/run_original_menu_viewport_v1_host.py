"""Build and run the source-backed full-driver front-menu viewport audit."""
from __future__ import annotations

import argparse
import os
from pathlib import Path
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[3]
ENGINE_UI = ROOT / "port/engine-ui"
SOURCE = ENGINE_UI / "tests/original_menu_viewport_v1_host.cpp"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", help="C++ compiler (defaults to CXX, g++, or clang++)")
    parser.add_argument(
        "--output", type=Path,
        default=ROOT / "port/android-native/build/original-menu-viewport-v1/original_menu_viewport_v1_host")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        parser.error("pass --compiler or set CXX")
    output = args.output.resolve()
    if os.name == "nt" and not output.suffix:
        output = output.with_suffix(".exe")
    output.parent.mkdir(parents=True, exist_ok=True)
    command = [compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror", "-pedantic",
               "-I", str(ENGINE_UI), str(ENGINE_UI / "viewport.cpp"), str(SOURCE), "-o", str(output)]
    built = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    if built.returncode:
        raise RuntimeError(built.stdout + built.stderr)
    ran = subprocess.run([str(output)], cwd=ROOT, capture_output=True, text=True)
    if ran.returncode:
        raise RuntimeError(ran.stdout + ran.stderr)
    print("PASS: source-faithful full-driver front/menu bounds and hit mapping (26 assertions)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
