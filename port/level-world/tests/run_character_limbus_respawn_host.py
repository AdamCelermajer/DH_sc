#!/usr/bin/env python3
"""Compile and run the source-bound CSLimbus::OnFocus respawn host fixture."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys


HERE = Path(__file__).resolve().parent
MODULE = HERE.parent
REPOSITORY = MODULE.parents[1]


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", help="compiler (defaults to CXX, g++, or clang++)")
    parser.add_argument("--output", type=Path,
                        default=MODULE / "build" / "character-limbus-respawn" /
                        ("character_limbus_respawn_host.exe" if os.name == "nt" else
                         "character_limbus_respawn_host"))
    parser.add_argument("--report", type=Path,
                        default=MODULE / "build" / "character-limbus-respawn" /
                        "validation.json")
    args = parser.parse_args()

    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        parser.error("no C++ compiler found; pass --compiler or set CXX")

    output = args.output.resolve()
    if os.name == "nt" and not output.suffix:
        output = output.with_suffix(".exe")
    output.parent.mkdir(parents=True, exist_ok=True)
    sources = [
        HERE / "character_limbus_respawn.cpp",
        MODULE / "character_limbus_respawn.cpp",
        MODULE / "character_respawn_outer.cpp",
        MODULE / "character_group_respawn.cpp",
        MODULE / "character_coordinator.cpp",
        MODULE / "character_state.cpp",
        MODULE / "character_timers.cpp",
    ]
    command = [
        compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror", "-pedantic",
        *(str(source) for source in sources), "-o", str(output),
    ]
    built = subprocess.run(command, cwd=REPOSITORY, text=True, capture_output=True)
    if built.stdout:
        sys.stdout.write(built.stdout)
    if built.stderr:
        sys.stderr.write(built.stderr)
    if built.returncode:
        return built.returncode

    tested = subprocess.run([str(output)], cwd=REPOSITORY, text=True, capture_output=True)
    if tested.stdout:
        sys.stdout.write(tested.stdout)
    if tested.stderr:
        sys.stderr.write(tested.stderr)
    if tested.returncode:
        return tested.returncode

    host_report = json.loads(tested.stdout)
    required = {
        "limbus_respawn_focus_cases": 14,
        "byte530_distinct_from_1481": True,
        "delay_getter_called_twice": True,
        "coordinator_timer_store_reused": True,
        "timer_repeat": 0,
        "timer_event": 0x2F,
        "online_host_gate": True,
        "normal_path_clear_aggro_last": True,
        "adapter_failure_does_not_invent_clear_aggro": True,
        "mismatches": 0,
    }
    if any(host_report.get(key) != expected for key, expected in required.items()):
        raise SystemExit(f"host report did not satisfy expectations: {host_report}")

    report_path = args.report.resolve()
    report_path.parent.mkdir(parents=True, exist_ok=True)
    evidence = {
        "validation": "PASS",
        "scope": "host-only CSLimbus::OnFocus producer decision; timer storage is the existing Character Coordinator",
        "executable": str(output),
        "host_report": host_report,
        "compiler_command": command,
        "compiled_source_sha256": {
            str(path.relative_to(REPOSITORY)).replace("\\", "/"): digest(path)
            for path in sources
        },
        "source_limbus_gate_and_manager_policy_wired_to_android": False,
    }
    report_path.write_text(json.dumps(evidence, indent=2) + "\n", encoding="utf-8")
    print(f"report: {report_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
