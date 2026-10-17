"""Test the saved-slot adapter against the selected game-data shared library."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
MODULE = ROOT / "port/game-data"
TESTS = MODULE / "tests"
ORACLE = TESTS / "player_saved_skill_slots_v1_original.py"
REFERENCE = MODULE / "reference/player-saved-skill-slots-v1"


def sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(command, **kwargs) -> subprocess.CompletedProcess:
    result = subprocess.run(command, capture_output=True, text=True, **kwargs)
    if result.returncode:
        raise RuntimeError("Command failed: " + " ".join(map(str, command)) +
                           "\n" + result.stdout + result.stderr)
    return result


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", default=shutil.which("g++") or "g++")
    parser.add_argument("--c-compiler", default=shutil.which("gcc") or "gcc")
    parser.add_argument("--cmake", default=shutil.which("cmake") or "cmake")
    parser.add_argument("--ninja", default=shutil.which("ninja") or "ninja")
    parser.add_argument("--original-elf", type=Path,
                        default=ROOT / ".local-inputs/libDungeonHunter2.so")
    parser.add_argument("--loot-cache", type=Path,
                        default=ROOT / ".local-inputs/items-discovery")
    parser.add_argument("--output-dir", type=Path,
                        default=MODULE / "build/player-saved-skill-slots-v1-host")
    parser.add_argument("--report", type=Path)
    args = parser.parse_args()

    compiler = shutil.which(args.compiler) or args.compiler
    c_compiler = shutil.which(args.c_compiler) or args.c_compiler
    loot_cache = args.loot_cache.resolve()
    for name in ("loot_table_pyarray.bin", "loot_table_pyarraynames.bin",
                 "loot_table_pystructnames.bin"):
        if not (loot_cache / name).is_file():
            raise FileNotFoundError(loot_cache / name)

    output_dir = args.output_dir.resolve()
    output_dir.mkdir(parents=True, exist_ok=True)
    project_dir = output_dir / "generated-project"
    build_dir = output_dir / "generated-build"
    project_dir.mkdir(parents=True, exist_ok=True)
    test = TESTS / "player_saved_skill_slots_v1.cpp"
    adapter = MODULE / "player_saved_skill_slots_v1.cpp"
    generated_test = project_dir / "player_saved_skill_slots_v1_host.cpp"
    generated_adapter = project_dir / "player_saved_skill_slots_v1_adapter.cpp"
    shutil.copy2(test, generated_test)
    shutil.copy2(adapter, generated_adapter)
    wrapper = project_dir / "CMakeLists.txt"
    wrapper.write_text(
        "cmake_minimum_required(VERSION 3.22)\n"
        "project(player_saved_skill_slots_v1_host LANGUAGES C CXX)\n"
        f'add_subdirectory("{MODULE.as_posix()}" game-data)\n'
        "add_executable(player_saved_skill_slots_v1_host\n"
        '  "${CMAKE_CURRENT_LIST_DIR}/player_saved_skill_slots_v1_host.cpp"\n'
        '  "${CMAKE_CURRENT_LIST_DIR}/player_saved_skill_slots_v1_adapter.cpp")\n'
        "target_compile_features(player_saved_skill_slots_v1_host PRIVATE cxx_std_17)\n"
        "target_compile_options(player_saved_skill_slots_v1_host PRIVATE\n"
        "  -Wall -Wextra -Werror -pedantic -fno-fast-math -ffp-contract=off)\n"
        "target_link_libraries(player_saved_skill_slots_v1_host PRIVATE dh2_game_data)\n",
        encoding="utf-8")
    configure = [args.cmake, "-S", str(project_dir), "-B", str(build_dir),
                 "-G", "Ninja", f"-DCMAKE_C_COMPILER={c_compiler}",
                 f"-DCMAKE_CXX_COMPILER={compiler}", "-DCMAKE_BUILD_TYPE=Release",
                 "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"]
    run(configure, cwd=ROOT)
    build = [args.cmake, "--build", str(build_dir), "--target",
             "player_saved_skill_slots_v1_host", "--parallel", "2"]
    run(build, cwd=ROOT)
    executable = build_dir / "player_saved_skill_slots_v1_host.exe"
    compile_db_path = build_dir / "compile_commands.json"
    library_dir = build_dir / "game-data"
    library = library_dir / "libdh2_game_data.dll"
    if not executable.is_file() or not library.is_file():
        raise FileNotFoundError("Generated selected-library build outputs are missing")
    env = os.environ.copy()
    env["PATH"] = str(library_dir) + os.pathsep + env.get("PATH", "")
    host = json.loads(run([str(executable), str(loot_cache)], cwd=ROOT,
                          env=env).stdout)
    assert host["validation"] == "PASS"

    original_report = output_dir / "original-selectors.json"
    original = json.loads(run([sys.executable, str(ORACLE), "--elf",
                               str(args.original_elf.resolve()), "--report",
                               str(original_report)], cwd=ROOT).stdout)
    assert original["validation"] == "PASS" and original["comparisons"] == 24

    try:
        import pefile
    except ImportError as error:
        raise RuntimeError("pefile is required to verify selected DSO linkage") from error
    executable_image = pefile.PE(str(executable))
    imported = sorted(entry.dll.decode("ascii", errors="replace")
                      for entry in executable_image.DIRECTORY_ENTRY_IMPORT)
    assert any(name.casefold() == "libdh2_game_data.dll" for name in imported), imported

    ninja = shutil.which(args.ninja) or args.ninja
    selected_commands = run([ninja, "-C", str(build_dir), "-t", "commands",
                             "player_saved_skill_slots_v1_host"], cwd=ROOT).stdout
    compile_db = json.loads(compile_db_path.read_text(encoding="utf-8"))
    selected_source_paths = sorted({
        str(Path(row["file"]).resolve()) for row in compile_db
        if str(row["file"]).replace("\\", "/").casefold() in
        selected_commands.replace("\\", "/").casefold()
    })
    expected_tus = [generated_test.resolve(), generated_adapter.resolve(),
                    (MODULE / "player_savegame_v1.cpp").resolve(),
                    (MODULE / "fresh_inventory_owned_v4.cpp").resolve()]
    selected_paths = {Path(path).resolve() for path in selected_source_paths}
    assert all(path in selected_paths for path in expected_tus), expected_tus
    selected_target_input_sha256 = {
        path.relative_to(ROOT).as_posix() if ROOT in path.parents else str(path): sha(path)
        for path in sorted(selected_paths) if path.is_file()
    }
    adjacent_headers = set()
    for path in selected_paths:
        if path.is_file():
            adjacent_headers.update(path.parent.glob("*.h"))
            adjacent_headers.update(path.parent.glob("*.hpp"))

    report_path = (args.report.resolve() if args.report else
                   output_dir / "validation.json")
    report_path.parent.mkdir(parents=True, exist_ok=True)
    inputs = [test, adapter, generated_test, generated_adapter,
              MODULE / "player_saved_skill_slots_v1.hpp",
              MODULE / "player_savegame_v1.hpp", MODULE / "fresh_inventory_owned_v4.hpp",
              MODULE / "CMakeLists.txt", wrapper, compile_db_path,
              ORACLE, Path(__file__).resolve(), REFERENCE / "original-functions.json",
              REFERENCE / "NOTES.md", *[loot_cache / n for n in
              ("loot_table_pyarray.bin", "loot_table_pyarraynames.bin",
               "loot_table_pystructnames.bin")], args.original_elf.resolve()]
    report = {
        "validation": "PASS",
        "host_report": host,
        "original_selector_report": original,
        "original_selector_report_sha256": sha(original_report),
        "configure_command": configure,
        "build_command": build,
        "selected_target_commands": selected_commands.splitlines(),
        "selected_target_translation_units": selected_source_paths,
        "selected_target_input_sha256": selected_target_input_sha256,
        "guarded_adjacent_headers": len(adjacent_headers),
        "guarded_adjacent_header_paths": sorted(
            p.relative_to(ROOT).as_posix() for p in adjacent_headers if ROOT in p.parents),
        "header_guard_scope": "Adjacent h/hpp files are conservatively listed, not claimed as the exact compiler dependency closure.",
        "generated_wrapper_sha256": sha(wrapper),
        "compile_database_sha256": sha(compile_db_path),
        "compiler_version": run([compiler, "--version"], cwd=ROOT).stdout.splitlines()[0],
        "selected_library_path": str(library),
        "selected_library_sha256": sha(library),
        "host_executable_sha256": sha(executable),
        "host_imports": imported,
        "input_sha256": {str(path): sha(path) for path in inputs},
        "source_scope": "The runner generates and records a CMake wrapper that builds the current dh2_game_data target and links the focused test executable to that selected shared library. Borrowed SavegameV1 and same-character FreshInventoryOwnedV4 remain the only saved/inventory owners. Original GetCurrentSkillSet resolves saved map zero independently from equipment selection.",
        "native_wired": False,
    }
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"validation": "PASS", "host_checks": host["checks"],
                      "original_selector_comparisons": original["comparisons"],
                      "selected_library_sha256": report["selected_library_sha256"],
                      "report": report_path.resolve().as_posix()}))


if __name__ == "__main__":
    main()
