"""Build the selected inventory/powered-loot library and run the AddLoot adapter case."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess


TESTS = Path(__file__).resolve().parent
MODULE = TESTS.parent
ROOT = MODULE.parents[1]
WRAPPER = TESTS / "item-loot-backbone-v7"
DEFAULT_FIXTURE = MODULE / "reference/player-creation-v2/powered-addloot-v7.bin"


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path, required=True,
                        help="canonical cache/files/data/pydata directory")
    parser.add_argument("--fixture", type=Path, default=DEFAULT_FIXTURE,
                        help="original powered AddLoot fixture")
    parser.add_argument("--character-data", type=Path,
                        default=ROOT / "port/android-native/app/src/main/assets/data",
                        help="canonical Character and class cache directory")
    parser.add_argument("--build", type=Path,
                        default=MODULE / "build/player-add-loot-v1-host")
    parser.add_argument("--compiler", default=shutil.which("g++") or "g++",
                        help="host C++ compiler; matching gcc is used for random.c")
    parser.add_argument("--cmake", default=shutil.which("cmake") or "cmake")
    parser.add_argument("--report", type=Path)
    args = parser.parse_args()

    cache = args.cache.resolve()
    character_data = args.character_data.resolve()
    fixture = args.fixture.resolve()
    build = args.build.resolve()
    compiler = Path(shutil.which(args.compiler) or args.compiler).resolve()
    c_compiler = compiler.with_name("gcc.exe" if os.name == "nt" else "gcc")
    if not cache.is_dir():
        raise NotADirectoryError(cache)
    if not fixture.is_file():
        raise FileNotFoundError(fixture)
    if not character_data.is_dir():
        raise NotADirectoryError(character_data)
    if not c_compiler.is_file():
        raise FileNotFoundError(f"matching C compiler not found: {c_compiler}")
    cache_names = (
        "item_powers_pyarray.bin", "item_powers_pyarraynames.bin",
        "item_powers_pystructnames.bin", "item_powers_monopoly_pyarray.bin",
        "item_powers_monopoly_pyarraynames.bin", "item_powers_monopoly_pystructnames.bin",
        "loot_table_pyarray.bin", "loot_table_pyarraynames.bin",
        "loot_table_pystructnames.bin",
    )
    cache_files = [cache / name for name in cache_names]
    for path in cache_files:
        if not path.is_file():
            raise FileNotFoundError(path)

    build.mkdir(parents=True, exist_ok=True)
    env = os.environ.copy()
    env["PATH"] = str(compiler.parent) + os.pathsep + env.get("PATH", "")
    commands = []

    def run(command, **kwargs):
        command = [str(value) for value in command]
        result = subprocess.run(command, cwd=ROOT, env=env, capture_output=True,
                                text=True, **kwargs)
        commands.append({"command": command, "returncode": result.returncode,
                         "stdout": result.stdout, "stderr": result.stderr})
        if result.returncode:
            raise RuntimeError(result.stderr or result.stdout)
        return result

    cmake = shutil.which(args.cmake) or args.cmake
    run([cmake, "-S", WRAPPER, "-B", build, "-G", "Ninja",
         "-DCMAKE_BUILD_TYPE=RelWithDebInfo",
         "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
         "-DCMAKE_CXX_COMPILER=" + str(compiler),
         "-DCMAKE_C_COMPILER=" + str(c_compiler)])
    run([cmake, "--build", build, "--parallel", "1", "--target",
         "item_loot_random_table_v1_selected"])

    database = json.loads((build / "compile_commands.json").read_text(encoding="utf-8"))
    dso_sources = [row for row in database
                   if "CMakeFiles/dh2_game_data.dir" in
                   row["command"].replace("\\", "/")]
    required_sources = {
        "fresh_inventory_owned_v4.cpp", "loot_tables_v2.cpp",
        "loot_entry_selection_v1.cpp", "item_presentation_v5.cpp",
        "loot_power_resources_v7.cpp", "loot_power_creation_v7.cpp",
    }
    selected_names = {Path(row["file"]).name for row in dso_sources}
    missing = sorted(required_sources - selected_names)
    if missing:
        raise AssertionError(f"selected game-data library is missing modules: {missing}")

    lib_dir = build / "game-data"
    if os.name == "nt":
        library = lib_dir / "libdh2_game_data.dll"
        link_name = "dh2_game_data"
        suffix = ".exe"
    else:
        library = lib_dir / "libdh2_game_data.so"
        link_name = "dh2_game_data"
        suffix = ""
    if not library.is_file():
        raise FileNotFoundError(library)

    random_object = build / ("player_add_loot_random.o")
    executable = build / ("player_add_loot_v1_host" + suffix)
    run([c_compiler, "-c", ROOT / "port/random/random.c", "-o", random_object])
    compile_command = [
        compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror",
        "-fno-fast-math", "-ffp-contract=off", "-I" + str(MODULE),
        TESTS / "player_add_loot_v1_host.cpp",
        MODULE / "player_add_loot_v1.cpp", random_object,
        "-L" + str(lib_dir), "-l" + link_name, "-o", executable,
    ]
    run(compile_command)

    env["PATH"] = str(lib_dir) + os.pathsep + env.get("PATH", "")
    result = run([executable, fixture, cache, character_data])
    host = json.loads(result.stdout)
    if host.get("validation") != "PASS":
        raise AssertionError("player AddLoot host test did not pass")

    character_files = [character_data / name for name in (
        "character_properties_pyarray.bin", "character_properties_pyarraynames.bin",
        "character_properties_pystructnames.bin", "character_classes_pyarray.bin",
        "character_classes_pyarraynames.bin", "character_classes_pystructnames.bin")]
    missing_character = [path for path in character_files if not path.is_file()]
    if missing_character:
        raise FileNotFoundError(missing_character[0])
    inputs = [TESTS / "player_add_loot_v1_host.cpp",
              MODULE / "player_add_loot_v1.cpp",
              MODULE / "player_add_loot_v1.hpp", fixture, *cache_files,
              *character_files]
    report_path = (args.report.resolve() if args.report else
                   build / "player_add_loot_v1_host.json")
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report = {
        "validation": "PASS",
        "host_test": host,
        "selected_library": {"path": str(library), "sha256": sha(library)},
        "selected_library_sources": sorted(required_sources),
        "input_sha256": {str(path): sha(path) for path in inputs},
        "commands": commands,
    }
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"validation": "PASS", "host_test": host,
                      "selected_library": str(library), "report": str(report_path)}))


if __name__ == "__main__":
    main()
