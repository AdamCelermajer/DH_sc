#!/usr/bin/env python3
"""Compile and run the authored class-click to profile-class regression."""
from __future__ import annotations

import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

TESTS = Path(__file__).resolve().parent
MODULE = TESTS.parent


def main() -> None:
    compiler = os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        raise SystemExit("A C++17 compiler is required")
    output = Path(tempfile.gettempdir()) / "dh2-menu-class-selection-v1"
    if os.name == "nt":
        output = output.with_suffix(".exe")
    subprocess.run([compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror", "-pedantic",
                    "-I", str(MODULE), str(TESTS / "menu_class_selection_v1.cpp"),
                    "-o", str(output)], check=True)
    result = subprocess.run([str(output)], check=True, capture_output=True, text=True)
    print(json.dumps(json.loads(result.stdout), separators=(",", ":")))


if __name__ == "__main__":
    main()
