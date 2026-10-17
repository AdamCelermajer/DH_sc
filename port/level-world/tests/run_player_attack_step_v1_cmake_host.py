"""Build and run the Player attack-step audit against the selected CMake library."""
from __future__ import annotations

import argparse
import os
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[3]
MODULE = ROOT / "port/level-world"
DEFAULT_BUILD = ROOT / ".checks/level-world-host"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build-dir", type=Path, default=DEFAULT_BUILD)
    args = parser.parse_args()
    build = args.build_dir.resolve()
    if not (build / "CMakeCache.txt").is_file():
        subprocess.run(
            ["cmake", "-S", str(MODULE), "-B", str(build), "-DCMAKE_BUILD_TYPE=Debug"],
            cwd=ROOT,
            check=True,
        )
    subprocess.run(
        ["cmake", "--build", str(build), "--target", "player_attack_step_v1_audit"],
        cwd=ROOT,
        check=True,
    )
    executable = build / "player_attack_step_v1_audit"
    if os.name == "nt":
        executable = executable.with_suffix(".exe")
        dll_dirs = {path.parent for path in build.rglob("*.dll")}
        env = os.environ.copy()
        env["PATH"] = os.pathsep.join([*(str(path) for path in dll_dirs), env.get("PATH", "")])
    else:
        env = os.environ.copy()
    subprocess.run([str(executable)], cwd=ROOT, env=env, check=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
