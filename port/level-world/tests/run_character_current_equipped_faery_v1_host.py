"""Selected-world and original ARM checks for current equipped-faery callbacks."""
from __future__ import annotations

import argparse
import hashlib
import itertools
import json
import os
import random
import shutil
import subprocess
import sys
from pathlib import Path

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
MANIFEST = MODULE / "reference/character-current-equipped-faery-v1/original-functions.json"
SOURCE = MODULE / "character_current_equipped_faery_v1.cpp"
TEST = MODULE / "tests/character_current_equipped_faery_v1.cpp"


def sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(command, env=None) -> str:
    result = subprocess.run(list(map(str, command)), cwd=ROOT, env=env,
                            capture_output=True, text=True)
    if result.returncode:
        raise RuntimeError(f"{command}\nexit={result.returncode}\n{result.stdout}\n{result.stderr}")
    return result.stdout


def arm(original: Path, executable: Path, compiler: Path, env, manifest: dict) -> dict:
    sys.path.insert(0, str(ROOT / "port/engine-resources/tests"))
    from cpu import Cpu, i32, u32
    from unicorn import UC_HOOK_CODE

    cpu = Cpu(original, False, manifest)
    for row in manifest["functions"]:
        start = int(row["elf_address"], 16)
        actual = bytes(cpu.uc.mem_read(cpu.base + start, row["size"]))
        assert hashlib.sha256(actual).hexdigest() == row["sha256"], row["original_symbol"]

    character = cpu.data + 0x1000
    returns = cpu.data + 0x3000
    seen = {"id": set(), "level": set()}
    comparisons = []
    base_cases = list(itertools.product(
        [0, 1, 4, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF, 16777217],
        [0, 1, 4, 65535, 0x80000000, 0xFFFFFFFF, 16777217]))
    rng = random.Random(0x3B6DA0)
    base_cases.extend((rng.getrandbits(32), rng.getrandbits(32)) for _ in range(16))

    for mode in ("id", "level"):
        entry = 0x3B6DA0 if mode == "id" else 0x3B6DF8
        end = entry + (36 if mode == "id" else 56)
        for selected, level in base_cases:
            trace, pushed = [], []

            def returned(value: int) -> None:
                cpu.put(0, value)
                cpu.uc.reg_write(cpu.pc, cpu.uc.reg_read(cpu.lr))

            def hook(_, address, __, ___):
                if entry <= address < end:
                    seen[mode].add(address)
                if address == 0x3BB98C:
                    assert cpu.reg(0) == character and cpu.reg(1) == 0xFFFFFFFF
                    trace.append([0, 0, -1])
                    returned(selected)
                elif address == 0x3BBC18:
                    assert [cpu.reg(i) for i in range(3)] == [
                        character, u32(selected), 0xFFFFFFFF]
                    trace.append([2, u32(selected), -1])
                    returned(level)
                elif address == 0x37CB24:
                    expected = i32(selected if mode == "id" else level)
                    assert cpu.reg(0) == returns and cpu.reg(1) == u32(expected)
                    pushed.append(i32(cpu.reg(1)))
                    returned(0)

            callback = cpu.uc.hook_add(UC_HOOK_CODE, hook)
            try:
                # Arguments pointer is intentionally nonsensical: both source
                # callbacks ignore it and use only Returns + captured Character.
                cpu.invoke(entry, [1, returns, character])
            finally:
                cpu.uc.hook_del(callback)

            result = json.loads(run([executable, "--oracle", mode, selected, level], env))
            expected_trace = [[0, 0, -1]]
            if mode == "level":
                expected_trace.append([2, u32(selected), -1])
            expected_value = i32(selected if mode == "id" else level)
            expected = {"status": 1, "faery_id": i32(selected),
                        "level": i32(level) if mode == "level" else 0,
                        "calls": len(expected_trace), "complete": 1,
                        "trace": expected_trace}
            assert result == expected, (mode, selected, level, expected, result)
            assert pushed == [expected_value]
            comparisons.append({"callback": mode, "input": [selected, level],
                                "expected": expected, "compiled": result})

        assert seen[mode] == set(range(entry, end, 4)), (mode, sorted(seen[mode]))

    return {
        "validation": "PASS",
        "comparisons": len(comparisons),
        "cases": comparisons,
        "instruction_addresses": {
            mode: [hex(address) for address in sorted(addresses)]
            for mode, addresses in seen.items()},
        "mismatches": 0,
        "scope": "Complete 36B ID and 56B level callback bodies execute from the pinned ARM ELF. SG_GetCurrentFaerieId, SG_GetFaerieLevel, and ReturnValues::pushInteger are explicit hooked providers; no dependency-body credit.",
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", required=True, type=Path)
    parser.add_argument("--original-elf", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--report", type=Path,
                        default=MODULE / "build/character-current-equipped-faery-v1/validation.json")
    args = parser.parse_args()

    output = args.output.resolve()
    wrapper = output / "wrapper"
    build = output / "selected"
    wrapper.mkdir(parents=True, exist_ok=True)
    build.mkdir(parents=True, exist_ok=True)
    compiler = args.compiler.resolve()
    c_compiler = compiler.with_name(compiler.name.replace("g++", "gcc"))
    cmake = shutil.which("cmake")
    ninja = shutil.which("ninja")
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    test_name = "current_equipped_faery_audit"
    wrapper_text = f'''cmake_minimum_required(VERSION 3.22)
project(current_equipped_faery_audit LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-world)
add_executable({test_name} "{TEST.as_posix()}")
target_compile_features({test_name} PRIVATE cxx_std_17)
target_compile_options({test_name} PRIVATE -Wall -Wextra -Werror -pedantic -fno-fast-math -ffp-contract=off)
target_link_libraries({test_name} PRIVATE dh2_level_world)
'''
    (wrapper / "CMakeLists.txt").write_text(wrapper_text, encoding="utf-8")
    logs = [run([cmake, "-S", wrapper, "-B", build, "-G", "Ninja",
                 f"-DCMAKE_MAKE_PROGRAM={ninja}",
                 f"-DCMAKE_CXX_COMPILER={compiler.as_posix()}",
                 f"-DCMAKE_C_COMPILER={c_compiler.as_posix()}",
                 "-DCMAKE_BUILD_TYPE=Release", "-DCMAKE_CXX_FLAGS_RELEASE=-O1",
                 "-DCMAKE_C_FLAGS_RELEASE=-O1"])]
    command_text = run([ninja, "-C", build, "-t", "commands", test_name])
    compile_db = json.loads((build / "compile_commands.json").read_text(encoding="utf-8"))
    normalized_commands = command_text.replace("\\", "/")
    sources = {Path(row["file"]).resolve() for row in compile_db
               if str(row["file"]).replace("\\", "/") in normalized_commands}
    current_spell = MODULE / "character_current_spell_v1.cpp"
    save_source = ROOT / "port/game-data/player_savegame_v1.cpp"
    assert SOURCE in sources and current_spell in sources and save_source in sources
    new_source_commands = [row["command"].replace("\\", "/") for row in compile_db
                          if Path(row["file"]).resolve() == SOURCE]
    assert len(new_source_commands) == 1 and "dh2_level_world.dir" in new_source_commands[0]

    protected = sources | {Path(__file__).resolve(), MANIFEST,
                           MANIFEST.with_name("NOTES.md"), TEST,
                           ROOT / "port/engine-resources/tests/cpu.py"}
    repository_names = run(["git", "-C", ROOT, "ls-files", "-z", "--cached", "--others", "--exclude-standard"]).split("\0")
    protected.update(ROOT / name for name in repository_names
                     if name.startswith(("port/", "vendor/", "third_party/"))
                     and Path(name).suffix in (".h", ".hpp", ".c", ".cpp", ".inc", ".inl")
                     and (ROOT / name).is_file())
    for directory in {path.parent for path in sources}:
        protected.update(directory.glob("*.h"))
        protected.update(directory.glob("*.hpp"))
        if (directory / "CMakeLists.txt").exists():
            protected.add(directory / "CMakeLists.txt")
    before = {path.relative_to(ROOT).as_posix(): sha(path)
              for path in protected if path.is_relative_to(ROOT)}
    logs.append(run([cmake, "--build", build, "--target", test_name, "--parallel", "1"]))
    # Adjacent headers were captured conservatively before compilation. Only
    # headers actually read by Ninja and explicit proof inputs guard the final
    # test; other agents may change unselected drafts in these directories.
    dependency_text = run([ninja, "-C", build, "-t", "deps"])
    dependency_paths = set()
    for line in dependency_text.splitlines():
        if not line.startswith("    "):
            continue
        path = Path(line.strip())
        path = (path if path.is_absolute() else build / path).resolve()
        if path.is_relative_to(ROOT):
            dependency_paths.add(path)
    required = sources | dependency_paths | {Path(__file__).resolve(), MANIFEST,
        MANIFEST.with_name("NOTES.md"), TEST, ROOT / "port/engine-resources/tests/cpu.py"}
    required.update(path for path in protected if path.name == "CMakeLists.txt")
    required_names = {path.relative_to(ROOT).as_posix() for path in required if path.is_relative_to(ROOT)}
    assert required_names <= before.keys(), ('Actual compiler input missing from prebuild snapshot', sorted(required_names - before.keys()))
    before = {name: before[name] for name in required_names}
    assert all(sha(ROOT / name) == value for name, value in before.items()), \
        "selected sources changed during build"

    executable = build / f"{test_name}.exe"
    dsos = sorted(build.rglob("*.dll"))
    env = os.environ.copy()
    env["PATH"] = os.pathsep.join([str(compiler.parent), *(str(path.parent) for path in dsos),
                                   env.get("PATH", "")])
    host = json.loads(run([executable], env))
    assert host["validation"] == "PASS"
    original = arm(args.original_elf.resolve(), executable, compiler, env, manifest)
    assert all(sha(ROOT / name) == value for name, value in before.items())

    objdump = compiler.with_name("objdump.exe")
    binaries = [executable, *dsos]
    dump = {path.relative_to(output).as_posix(): run([objdump, "-p", path])
            for path in binaries}
    imports = {name: __import__("re").findall(r"DLL Name: (\S+)", text)
               for name, text in dump.items()}
    executable_imports = imports[executable.relative_to(output).as_posix()]
    assert "libdh2_level_world.dll" in executable_imports
    assert "current_equipped_faery_id_v1" in dump[executable.relative_to(output).as_posix()]
    assert "current_equipped_faery_level_v1" in dump[executable.relative_to(output).as_posix()]
    world = next(path for path in dsos if path.name == "libdh2_level_world.dll")
    world_dump = dump[world.relative_to(output).as_posix()]
    assert "current_equipped_faery_v1" in world_dump and "saved_services" in world_dump

    source_tus = {path.relative_to(ROOT).as_posix(): sha(path)
                  for path in sources if path.is_relative_to(ROOT)}
    report = {
        "validation": "PASS",
        "host_report": host,
        "original_arm_comparison": original,
        "source_sha256": before,
        "compile_translation_units_sha256": source_tus,
        "source_pin_scope": "Actual host Ninja compiler dependencies and selected translation units plus explicit proof/CMake inputs, verified against prebuild hashes. Unselected adjacent drafts are excluded; no Android provenance claim.",
        "selected_dso_imports": imports,
        "selected_commands": command_text,
        "wrapper_cmake": wrapper_text,
        "binary_sha256": {path.relative_to(output).as_posix(): sha(path) for path in binaries},
        "original_sha256": sha(args.original_elf.resolve()),
        "attribution": manifest["attribution"],
        "new_complete_original_callers": 2,
        "new_complete_dependency_bodies": 0,
        "live_native_validation": False,
        "scope": "Thin exact one-call ID and two-call level wrappers imported from the actual project-selected world DLL reuse CurrentSpell saved-service bindings and one PlayerSavegame. No GetCharFaery validation, second owner or invented selection default. Native wiring requires a separate Android/live receipt.",
        "build_stdout": logs,
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"host": host, "original_comparisons": original["comparisons"],
                      "mismatches": 0}))


if __name__ == "__main__":
    main()
