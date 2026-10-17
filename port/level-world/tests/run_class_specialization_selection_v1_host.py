"""Build and run focused NativeSelectClassSpec owner-order checks."""
from __future__ import annotations

import argparse
import os
from pathlib import Path
import shutil
import subprocess
import sys

TESTS = Path(__file__).resolve().parent
ROOT = TESTS.parents[2]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        raise SystemExit("no host C++ compiler found; pass --compiler")
    output = TESTS / "build" / "class_specialization_selection_v1_host"
    if os.name == "nt":
        output = output.with_suffix(".exe")
    output.parent.mkdir(parents=True, exist_ok=True)
    command = [compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror",
               str(TESTS / "class_specialization_selection_v1.cpp"), "-o", str(output)]
    compiler_path = shutil.which(compiler) or compiler
    child_env = os.environ.copy()
    child_env["PATH"] = str(Path(compiler_path).resolve().parent) + os.pathsep + child_env.get("PATH", "")
    built = subprocess.run(command, cwd=ROOT, env=child_env, text=True, capture_output=True)
    if built.stdout:
        sys.stdout.write(built.stdout)
    if built.stderr:
        sys.stderr.write(built.stderr)
    if built.returncode:
        return built.returncode
    result = subprocess.run([str(output)], cwd=ROOT, env=child_env, text=True, capture_output=True)
    if result.stdout:
        sys.stdout.write(result.stdout)
    if result.stderr:
        sys.stderr.write(result.stderr)
    return result.returncode


if __name__ == "__main__":
    raise SystemExit(main())
