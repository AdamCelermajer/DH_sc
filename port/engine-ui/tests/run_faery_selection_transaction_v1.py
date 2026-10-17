#!/usr/bin/env python3
"""Compile and run the narrow NativeHUDSetActiveFaery transaction test."""
from pathlib import Path
import subprocess
import tempfile

test = Path(__file__).with_name("faery_selection_transaction_v1.cpp")
with tempfile.TemporaryDirectory(prefix="dh2-faery-selection-") as tmp:
    binary = Path(tmp) / "faery_selection_transaction_v1"
    subprocess.run(["g++", "-std=c++17", "-Wall", "-Wextra", "-Werror",
                    str(test), "-o", str(binary)], check=True)
    subprocess.run([str(binary)], check=True)
