"""Build and run the Faery CharAI::Update owner audit against the level-world library.

The audit uses a real Coordinator and the recovered CharAI kernels, so it links
the already built dh2_level_world shared library instead of recompiling the
Character stack. Pass the desktop build directory that contains
level-world/libdh2_level_world.dll.a (and its sibling libraries):

    python run_native_faery_ai_frame_v1_host.py --build-dir %TEMP%/dh2-faery-build
"""
from __future__ import annotations

import argparse
import os
from pathlib import Path
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[3]
LEVEL = ROOT / "port/level-world"
CPP = ROOT / "port/android-native/app/src/main/cpp"
SOURCES = [CPP / "native_faery_ai_frame_v1.cpp",
           ROOT / "port/android-native/tests/native_faery_ai_frame_v1_host.cpp"]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", help="C++ compiler (defaults to CXX, g++, or clang++)")
    parser.add_argument("--build-dir", type=Path, required=True,
                        help="desktop build directory with level-world/libdh2_level_world.dll.a")
    parser.add_argument("--output", type=Path,
                        default=ROOT / "port/android-native/build/faery-ai-frame/native_faery_ai_frame_v1_host")
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
    command = [compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror", "-pedantic",
               "-fno-fast-math", "-ffp-contract=off", "-I", str(LEVEL),
               "-I", str(ROOT / "port/game-data"), "-I", str(ROOT / "port/random"),
               "-isystem", str(ROOT / "port/physics-backend/box2d-2.0.1/Include"), "-isystem", str(ROOT / "port/physics-backend/box2d-2.0.1/Source"),
               *map(str, SOURCES), "-o", str(output)]
    for libdir in libdirs:
        command += ["-L", str(libdir)]
    command += ["-ldh2_level_world", "-ldh2_game_data"]
    built = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    if built.returncode:
        raise RuntimeError(built.stdout + built.stderr)
    env = dict(os.environ)
    env["PATH"] = os.pathsep.join([str(build / "bin"), env.get("PATH", "")])
    ran = subprocess.run([str(output)], cwd=ROOT, capture_output=True, text=True, env=env)
    if ran.returncode:
        raise RuntimeError(ran.stdout + ran.stderr)
    print(ran.stdout.strip())
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
