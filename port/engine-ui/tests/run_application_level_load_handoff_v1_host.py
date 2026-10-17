#!/usr/bin/env python3
"""Compile and run the Application::LoadLevel existing-level boundary check."""
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
test = root / "tests" / "application_level_load_handoff_v1.cpp"
source = root / "application_level_load_handoff_v1.cpp"
with tempfile.TemporaryDirectory(prefix="dh2-app-level-load-") as tmp:
    exe = Path(tmp) / "application_level_load_handoff_v1.exe"
    subprocess.run(["g++", "-std=c++17", "-Wall", "-Wextra", "-Werror",
                    str(test), str(source), "-o", str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
