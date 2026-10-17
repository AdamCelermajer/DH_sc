"""Quick selected-library proof for requested=-1 powered enemy loot."""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess

TESTS = Path(__file__).resolve().parent
MODULE = TESTS.parent
ROOT = MODULE.parents[1]
WRAPPER = TESTS / "item-loot-backbone-v7"
FIXTURE = MODULE / "reference/player-creation-v2/powered-addloot-v7.bin"


def run(command, *, cwd, env):
    result = subprocess.run([str(value) for value in command], cwd=cwd, env=env,
                            capture_output=True, text=True)
    if result.returncode:
        raise RuntimeError(result.stderr or result.stdout)
    return result.stdout


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path, required=True)
    parser.add_argument("--character-data", type=Path,
                        default=ROOT / "port/android-native/app/src/main/assets/data")
    parser.add_argument("--build", type=Path,
                        default=ROOT / "outputs/enemy-drop-powered-quantity-host")
    parser.add_argument("--compiler", default=shutil.which("g++") or "g++")
    parser.add_argument("--crypt-ghost", action="store_true",
                        help="run one bounded Crypt_Ghost Loot=22 selected-library parity case")
    args = parser.parse_args()

    compiler = Path(shutil.which(args.compiler) or args.compiler).resolve()
    c_compiler = compiler.with_name("gcc.exe" if os.name == "nt" else "gcc")
    if not FIXTURE.is_file() or not args.cache.is_dir() or not args.character_data.is_dir():
        raise FileNotFoundError("fixture, canonical cache, or Character data directory missing")
    build = args.build.resolve()
    build.mkdir(parents=True, exist_ok=True)
    env = os.environ.copy()
    env["PATH"] = str(compiler.parent) + os.pathsep + env.get("PATH", "")
    cmake = shutil.which("cmake") or "cmake"
    run([cmake, "-S", WRAPPER, "-B", build, "-G", "Ninja",
         "-DCMAKE_BUILD_TYPE=RelWithDebInfo",
         "-DCMAKE_CXX_COMPILER=" + str(compiler),
         "-DCMAKE_C_COMPILER=" + str(c_compiler)], cwd=ROOT, env=env)
    run([cmake, "--build", build, "--parallel", "1", "--target",
         "item_loot_random_table_v1_selected"], cwd=ROOT, env=env)

    suffix = ".exe" if os.name == "nt" else ""
    executable = build / ("enemy_drop_powered_quantity_host" + suffix)
    lib = build / "game-data"
    random_object = build / "enemy_drop_random.o"
    run([c_compiler, "-c", ROOT / "port/random/random.c", "-o", random_object],
        cwd=ROOT, env=env)
    run([compiler, "-std=c++17", "-O1", "-Wall", "-Wextra",
         "-fno-fast-math", "-ffp-contract=off", "-I" + str(MODULE),
         TESTS / "enemy_drop_powered_quantity_host.cpp",
         MODULE / "player_add_loot_v1.cpp", random_object,
         "-L" + str(lib), "-ldh2_game_data", "-o", executable],
        cwd=ROOT, env=env)
    env["PATH"] = str(lib) + os.pathsep + env["PATH"]
    command = [executable, FIXTURE, args.cache.resolve(),
               args.character_data.resolve()]
    if args.crypt_ghost:
        command.append("crypt_ghost")
    output = run(command, cwd=ROOT, env=env)
    result = json.loads(output)
    if result.get("validation") != "PASS":
        raise AssertionError("enemy DropLoot quantity fixture did not pass")
    print(json.dumps(result))


if __name__ == "__main__":
    main()
