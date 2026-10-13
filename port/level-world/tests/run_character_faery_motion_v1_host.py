"""Build and run the Faery root-motion owner audit against the level-world library.

Links the already built dh2_level_world shared library (it needs the real celeste
model/clips from the bundled assets):

    python run_character_faery_motion_v1_host.py --build-dir %TEMP%/dh2-faery-build
"""
from __future__ import annotations

import argparse
import os
from pathlib import Path
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[3]
LEVEL = ROOT / "port/level-world"
SOURCE = LEVEL / "tests/character_faery_motion_v1_host.cpp"
ASSETS = ROOT / "port/android-native/app/src/main/assets/actors/"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", help="C++ compiler (defaults to CXX, g++, or clang++)")
    parser.add_argument("--build-dir", type=Path, required=True,
                        help="desktop build directory with level-world/libdh2_level_world.dll.a")
    parser.add_argument("--output", type=Path,
                        default=ROOT / "port/level-world/build/faery-motion/character_faery_motion_v1_host")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        parser.error("pass --compiler or set CXX")
    build = args.build_dir.resolve()
    libdirs = [build / "level-world", build / "level-world/game-data",
               build / "level-world/adam-script-runtime", build / "scene-materials",
               build / "engine-animation", build / "engine-skinning"]
    output = args.output.resolve()
    if os.name == "nt" and not output.suffix:
        output = output.with_suffix(".exe")
    output.parent.mkdir(parents=True, exist_ok=True)
    command = [compiler, "-std=gnu++17", "-O1", "-Wall", "-Wextra",
               "-fno-fast-math", "-ffp-contract=off", "-I", str(LEVEL),
               "-I", str(ROOT / "port/game-data"), "-I", str(ROOT / "port/engine-animation"),
               "-I", str(ROOT / "port/scene-materials"), "-I", str(ROOT / "port/engine-skinning"),
               "-isystem", str(ROOT / "port/physics-backend/box2d-2.0.1/Include"), "-isystem", str(ROOT / "port/physics-backend/box2d-2.0.1/Source"),
               "-I", str(ROOT / "port"), str(SOURCE), "-o", str(output)]
    for libdir in libdirs:
        command += ["-L", str(libdir)]
    command += ["-ldh2_level_world", "-ldh2_game_data", "-ldh2_engine_animation",
                "-ldh2_engine_skinning", "-ldh2_scene_materials"]
    built = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    if built.returncode:
        raise RuntimeError(built.stdout + built.stderr)
    env = dict(os.environ)
    env["PATH"] = os.pathsep.join([str(build / "bin"), env.get("PATH", "")])
    ran = subprocess.run([str(output), str(ASSETS) + os.sep], cwd=ROOT, capture_output=True,
                         text=True, env=env)
    if ran.returncode:
        raise RuntimeError(ran.stdout + ran.stderr)
    print(ran.stdout.strip())
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
