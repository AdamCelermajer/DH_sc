"""Build the Faery placement binding regression in a temporary host directory."""
from __future__ import annotations

import argparse
import os
from pathlib import Path
import shutil
import subprocess
import sys

LEVEL_WORLD = Path(__file__).resolve().parents[1]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", help="C++17 host compiler")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX")
    repo = LEVEL_WORLD.parents[1]
    build_dir = repo / ".checks" / "level-world-host"
    if not (build_dir / "CMakeCache.txt").exists():
        command = ["cmake", "-S", str(LEVEL_WORLD), "-B", str(build_dir),
                   "-G", "Ninja"]
        if compiler:
            command.append(f"-DCMAKE_CXX_COMPILER={compiler}")
        configured = subprocess.run(command, text=True, capture_output=True)
        if configured.stdout:
            sys.stdout.write(configured.stdout)
        if configured.stderr:
            sys.stderr.write(configured.stderr)
        if configured.returncode:
            return configured.returncode
    built = subprocess.run(["cmake", "--build", str(build_dir),
        "--target", "character_faery_live_binding_v1_audit", "-j", "2"],
        text=True, capture_output=True)
    if built.stdout:
        sys.stdout.write(built.stdout)
    if built.stderr:
        sys.stderr.write(built.stderr)
    if built.returncode:
        return built.returncode
    executable = build_dir / "character_faery_live_binding_v1_audit.exe"
    env = os.environ.copy()
    compiler_path = shutil.which(compiler or "g++")
    runtime_dirs = [build_dir, build_dir / "game-data",
        build_dir / "engine-skinning",
        build_dir / "engine-skinning" / "engine-animation",
        build_dir / "engine-skinning" / "engine-animation" / "scene-materials",
        build_dir / "adam-script-runtime"]
    if compiler_path:
        runtime_dirs.insert(0, Path(compiler_path).resolve().parent)
    env["PATH"] = os.pathsep.join(str(path) for path in runtime_dirs) + os.pathsep + env.get("PATH", "")
    run = subprocess.run([str(executable)], text=True, capture_output=True, env=env)
    if run.stdout:
        sys.stdout.write(run.stdout)
    if run.stderr:
        sys.stderr.write(run.stderr)
    if run.returncode:
        return run.returncode
    print("PASS fail-closed Faery live binding regression")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
