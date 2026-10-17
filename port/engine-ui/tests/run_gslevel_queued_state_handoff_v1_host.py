#!/usr/bin/env python3
"""Compile and run the deferred GSLevel state-switch handoff regression."""
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
test = root / "tests" / "gslevel_queued_state_handoff_v1.cpp"
sources = [root / "gslevel_queued_state_handoff_v1.cpp",
          root.parent / "level-world" / "level_quick_save_v1.cpp"]
with tempfile.TemporaryDirectory(prefix="dh2-gslevel-handoff-") as tmp:
    exe = Path(tmp) / "gslevel_queued_state_handoff_v1.exe"
    subprocess.run(["g++", "-std=c++17", "-Wall", "-Wextra", "-Werror",
                    str(test), *(str(p) for p in sources), "-o", str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
