"""Cold-process round-trip of generated powered AddLoot through canonical GEAR."""
from __future__ import annotations

import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess

TESTS = Path(__file__).resolve().parent
MODULE = TESTS.parent
ROOT = MODULE.parents[1]
DEFAULT_FIXTURE = ROOT / "port/game-data/reference/player-creation-v2/powered-addloot-v7.bin"


def run(command, *, cwd=ROOT, env=None):
    result = subprocess.run(list(map(str, command)), cwd=cwd, env=env,
                            capture_output=True, text=True)
    if result.returncode:
        raise RuntimeError(f"{command}\n{result.stdout}\n{result.stderr}")
    return result.stdout


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path,
        default=ROOT / ".local-inputs/ui-inventory-cache")
    parser.add_argument("--character-data", type=Path,
        default=ROOT / "port/android-native/app/src/main/assets/data")
    parser.add_argument("--fixture", type=Path, default=DEFAULT_FIXTURE)
    parser.add_argument("--output-dir", type=Path,
        default=ROOT / "tmp/player-add-loot-gear-roundtrip-v1")
    args = parser.parse_args()
    cache, character_data = args.cache.resolve(), args.character_data.resolve()
    fixture, output = args.fixture.resolve(), args.output_dir.resolve()
    for path in (cache, character_data):
        if not path.is_dir():
            raise NotADirectoryError(path)
    if not fixture.is_file():
        raise FileNotFoundError(fixture)
    compiler = Path(shutil.which("g++") or "g++").resolve()
    cc = compiler.with_name(compiler.name.replace("g++", "gcc"))
    cmake, ninja = shutil.which("cmake"), shutil.which("ninja")
    if not cmake or not ninja or not cc.is_file():
        raise RuntimeError("matching GCC, CMake and Ninja are required")

    wrapper, build = output / "wrapper", output / "selected"
    wrapper.mkdir(parents=True, exist_ok=True)
    (wrapper / "CMakeLists.txt").write_text(f'''cmake_minimum_required(VERSION 3.22)
project(player_add_loot_gear_roundtrip_v1_host LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_OBJECT_PATH_MAX 128)
add_subdirectory("{MODULE.as_posix()}" selected-level-world)
add_executable(player_add_loot_gear_roundtrip_v1_host
  "{(TESTS / 'player_add_loot_gear_roundtrip_v1.cpp').as_posix()}"
  "{(ROOT / 'port/random/random.c').as_posix()}")
target_compile_features(player_add_loot_gear_roundtrip_v1_host PRIVATE cxx_std_17)
target_compile_options(player_add_loot_gear_roundtrip_v1_host PRIVATE
  $<$<COMPILE_LANGUAGE:CXX>:-Wall;-Wextra;-Werror;-fno-fast-math;-ffp-contract=off>)
target_link_libraries(player_add_loot_gear_roundtrip_v1_host PRIVATE dh2_level_world)
''', encoding="utf-8")
    run([cmake, "-S", wrapper, "-B", build, "-G", "Ninja",
         f"-DCMAKE_MAKE_PROGRAM={Path(ninja).as_posix()}",
         f"-DCMAKE_CXX_COMPILER={compiler.as_posix()}",
         f"-DCMAKE_C_COMPILER={cc.as_posix()}",
         "-DCMAKE_BUILD_TYPE=Release", "-DCMAKE_CXX_FLAGS_RELEASE=-O1",
         "-DCMAKE_C_FLAGS_RELEASE=-O1"])
    run([cmake, "--build", build, "--target",
         "player_add_loot_gear_roundtrip_v1_host", "--parallel", "2"])
    exe = build / "player_add_loot_gear_roundtrip_v1_host.exe"
    if not exe.is_file():
        matches = list(build.rglob("player_add_loot_gear_roundtrip_v1_host.exe"))
        if len(matches) != 1:
            raise RuntimeError("selected round-trip host executable missing")
        exe = matches[0]
    env = os.environ.copy()
    env["PATH"] = os.pathsep.join([str(compiler.parent),
        *(str(p.parent) for p in build.rglob("*.dll")), env.get("PATH", "")])
    payload = output / "generated-powered-gear.bin"
    generated = json.loads(run([exe, "generate", cache, character_data,
                                fixture, payload], env=env))
    cold = json.loads(run([exe, "cold-open", cache, character_data,
                           fixture, payload], env=env))
    if generated.get("validation") != "PASS" or generated.get("phase") != "generate":
        raise RuntimeError(f"generation phase failed: {generated}")
    if cold.get("validation") != "PASS" or cold.get("phase") != "cold_open":
        raise RuntimeError(f"cold-open phase failed: {cold}")
    if generated.get("item_id") != 841 or generated.get("power_id") != 193 or \
       cold.get("item_id") != 841 or cold.get("power_id") != 193 or \
       cold.get("equipped_slot") != 3 or cold.get("save_owner_count") != 1:
        raise RuntimeError(f"unexpected round-trip result: {generated}, {cold}")
    print(json.dumps({"validation": "PASS", "selected_library": "dh2_level_world",
                      "generated": generated, "cold_open": cold,
                      "payload": str(payload)}))


if __name__ == "__main__":
    main()
