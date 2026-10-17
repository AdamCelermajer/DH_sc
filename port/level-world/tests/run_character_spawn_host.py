"""Compile and run the source-backed Character Limbus/Spawn host test."""
from __future__ import annotations

import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys


LEVEL_WORLD = Path(__file__).resolve().parents[1]
REPOSITORY = LEVEL_WORLD.parents[1]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", help="C++ compiler executable (defaults to CXX, g++, or clang++)")
    parser.add_argument(
        "--output", type=Path,
        default=LEVEL_WORLD / "build" / ("character_spawn_host.exe" if os.name == "nt" else "character_spawn_host"),
        help="host test executable path (default: ignored level-world/build/)",
    )
    args = parser.parse_args()

    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        parser.error("no C++ compiler found; pass --compiler or set CXX")

    output = args.output.resolve()
    if os.name == "nt" and not output.suffix:
        output = output.with_suffix(".exe")
    output.parent.mkdir(parents=True, exist_ok=True)

    sources = [
        LEVEL_WORLD / "tests" / "character_spawn.cpp",
        LEVEL_WORLD / "character_state.cpp",
        LEVEL_WORLD / "character_skill_state_dispatch_v1.cpp",
        LEVEL_WORLD / "character_skill_fsm_callbacks_v1.cpp",
        LEVEL_WORLD / "character_cast_lifecycle_v1.cpp",
        LEVEL_WORLD / "character_factory.cpp",
        LEVEL_WORLD / "object_manager_runtime_owner_v1.cpp",
    ]
    command = [
        compiler,
        "-std=c++17",
        "-O1",
        "-Wall",
        "-Wextra",
        "-Werror",
        "-fno-fast-math",
        "-ffp-contract=off",
        f"-I{LEVEL_WORLD}",
        *(str(source) for source in sources),
        "-o",
        str(output),
    ]
    built = subprocess.run(command, cwd=REPOSITORY, text=True, capture_output=True)
    if built.stdout:
        sys.stdout.write(built.stdout)
    if built.stderr:
        sys.stderr.write(built.stderr)
    if built.returncode:
        return built.returncode

    result = subprocess.run([str(output)], cwd=REPOSITORY, text=True, capture_output=True)
    if result.stdout:
        sys.stdout.write(result.stdout)
    if result.stderr:
        sys.stderr.write(result.stderr)
    if result.returncode:
        return result.returncode

    report = json.loads(result.stdout)
    expected = {
        "limbus_spawn_idle_path": True,
        "interactive_event_body": True,
        "object_manager_backed_spawn_verified": True,
        "fade_boundary_is_stub": True,
        "on_update_invents_no_transition": True,
        "mismatches": 0,
    }
    if any(report.get(key) != value for key, value in expected.items()):
        raise SystemExit(f"host report did not satisfy expectations: {report}")
    print(json.dumps({"validation": "PASS", "executable": str(output), "host_report": report}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
