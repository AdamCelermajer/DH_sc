#!/usr/bin/env python3
"""Compile and run source-order cases for the authored inventory list."""
from __future__ import annotations

import json
from pathlib import Path
import shutil
import subprocess
import tempfile

TESTS = Path(__file__).resolve().parent
MODULE = TESTS.parent


def main() -> None:
    compiler = shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        raise SystemExit("A C++17 compiler is required")
    output = Path(tempfile.gettempdir()) / "dh2-character-menu-inventory-order-v1"
    subprocess.run([compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror",
                    "-pedantic", str(TESTS / "character_menu_inventory_order_v1.cpp"),
                    str(MODULE / "character_menu_inventory_order_v1.cpp"),
                    "-o", str(output)], check=True)
    result = subprocess.run([str(output)], check=True, capture_output=True, text=True)
    print(json.dumps(json.loads(result.stdout), separators=(",", ":")))


if __name__ == "__main__":
    main()
