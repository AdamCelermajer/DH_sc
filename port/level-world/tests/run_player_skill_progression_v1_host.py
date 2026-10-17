"""Selected game-data host gate plus bounded original ARM predicate replay."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import random
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
MANIFEST = MODULE / "reference/player-skill-progression-v1/original-functions.json"
CAN_INCREMENT = 0x3BC9EC
IS_AVAILABLE = 0x3BCA50
GET_LEVEL = 0x3BD120
GET_CHAR_SKILL = 0x3BC784


def sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(command, *, cwd=ROOT, env=None):
    process = subprocess.run(list(map(str, command)), cwd=cwd, env=env,
                             capture_output=True, text=True)
    if process.returncode:
        raise RuntimeError(f"{command}\n{process.stdout}\n{process.stderr}")
    return process.stdout


def compile_selected(a):
    output = a.output.resolve()
    wrapper = output / "wrapper"
    build = output / "selected"
    wrapper.mkdir(parents=True, exist_ok=True)
    build.mkdir(parents=True, exist_ok=True)
    cxx = Path(a.compiler).resolve()
    cc = cxx.with_name(cxx.name.replace("g++", "gcc"))
    cmake = shutil.which("cmake")
    ninja = shutil.which("ninja")
    if not cmake or not ninja:
        raise RuntimeError("CMake and Ninja are required")
    production = MODULE / "player_skill_progression_v1.cpp"
    test = MODULE / "tests/player_skill_progression_v1_host.cpp"
    game_data = ROOT / "port/game-data"
    cmake_text = f'''cmake_minimum_required(VERSION 3.22)
project(player_skill_progression_v1_audit LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{game_data.as_posix()}" selected-game-data)
add_executable(player_skill_progression_v1_audit
  "{production.as_posix()}" "{test.as_posix()}")
target_compile_features(player_skill_progression_v1_audit PRIVATE cxx_std_17)
target_compile_options(player_skill_progression_v1_audit PRIVATE
  -Wall -Wextra -Werror -pedantic -fno-fast-math -ffp-contract=off)
target_link_libraries(player_skill_progression_v1_audit PRIVATE dh2_game_data)
'''
    (wrapper / "CMakeLists.txt").write_text(cmake_text, encoding="utf-8")
    commands = []
    logs = []
    configure = [cmake, "-S", wrapper, "-B", build, "-G", "Ninja",
                 f"-DCMAKE_MAKE_PROGRAM={ninja}", f"-DCMAKE_CXX_COMPILER={cxx}",
                 f"-DCMAKE_C_COMPILER={cc}", "-DCMAKE_BUILD_TYPE=Release",
                 "-DCMAKE_CXX_FLAGS_RELEASE=-O1", "-DCMAKE_C_FLAGS_RELEASE=-O1"]
    commands.append(list(map(str, configure)))
    logs.append(run(configure))
    compilation = json.loads((build / "compile_commands.json").read_text())
    selected_commands = run([ninja, "-C", build, "-t", "commands",
                             "player_skill_progression_v1_audit"])
    sources = {Path(item["file"]).resolve() for item in compilation
               if str(item["file"]).replace("\\", "/") in
               selected_commands.replace("\\", "/")}
    required = {
        production.resolve(), test.resolve(),
        (game_data / "player_savegame_v1.cpp").resolve(),
        (game_data / "skill_tables.cpp").resolve(),
        (game_data / "properties.cpp").resolve(),
    }
    if not required <= sources:
        raise RuntimeError(f"selected CMake closure missing: {required - sources}")
    build_command = [cmake, "--build", build, "--target",
                     "player_skill_progression_v1_audit", "--parallel", "1"]
    commands.append(list(map(str, build_command)))
    logs.append(run(build_command))
    binary = build / "player_skill_progression_v1_audit.exe"
    if not binary.exists():
        raise RuntimeError("selected host executable missing")
    dso = next(build.rglob("libdh2_game_data.dll"), None)
    if not dso:
        raise RuntimeError("selected dh2_game_data library missing")
    env = os.environ.copy()
    env["PATH"] = os.pathsep.join([str(cxx.parent), str(dso.parent),
                                    env.get("PATH", "")])
    return {
        "wrapper": wrapper, "build": build, "binary": binary, "dso": dso,
        "env": env, "sources": sources,
        "commands": commands, "logs": logs,
        "selected_commands": selected_commands, "wrapper_cmake": cmake_text,
    }


def verify_manifest(original: Path):
    from elftools.elf.elffile import ELFFile
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    if sha(original) != manifest["original_sha256"]:
        raise RuntimeError("pinned original ELF hash mismatch")
    raw = original.read_bytes()
    with original.open("rb") as stream:
        elf = ELFFile(stream)
        symbols = {symbol.name: symbol for symbol in
                   elf.get_section_by_name(".symtab").iter_symbols()}
        segments = [segment for segment in elf.iter_segments()
                    if segment["p_type"] == "PT_LOAD"]
        for function in manifest["functions"]:
            address = int(function["elf_address"], 16)
            size = function["size"]
            symbol = symbols[function["original_symbol"]]
            if (int(symbol["st_value"]), int(symbol["st_size"])) != (address, size):
                raise RuntimeError(f"symbol mismatch: {function['original_symbol']}")
            segment = next(item for item in segments
                           if item["p_vaddr"] <= address and
                           address + size <= item["p_vaddr"] + item["p_filesz"])
            offset = int(segment["p_offset"]) + address - int(segment["p_vaddr"])
            if hashlib.sha256(raw[offset:offset + size]).hexdigest() != function["sha256"]:
                raise RuntimeError(f"range hash mismatch: {function['original_symbol']}")
    return manifest


def scenarios():
    cases = []
    levels = [-8388608, -257, -1, 0, 1, 2, 3, 7, 24, 100, 8388607]
    required = [-(1 << 31), -10, -3, -1, 0, 1, 4, 25, (1 << 31) - 1]
    saved_levels = [0, 1, 3, 7, 255, 32768, 65535]
    for level in levels:
        for need in required:
            for saved in saved_levels:
                cases.append((level, need, saved, 1))
    cases.extend([
        (-2, -3, 0, 0), (8388607, -2147483648, 65535, 1),
        (-8388608, 2147483647, 0, 1), (0, 0, 0, 2),
    ])
    rng = random.Random(0x3BC9EC)
    for _ in range(80):
        cases.append((rng.randint(-10000, 10000),
                      rng.choice(required + [rng.randint(-1000, 1000)]),
                      rng.choice(saved_levels), rng.choice([0, 1, 2])))
    return cases


def original_compare(original: Path, executable: Path, env, manifest):
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0, str(ROOT / "port/engine-resources/tests"))
    from cpu import Cpu

    cpu = Cpu(original, False, manifest)
    character = cpu.data + 0x1000
    savegame = cpu.data + 0x3000
    saved_rows = cpu.data + 0x5000
    skill_row = cpu.data + 0x7000
    source_instructions = set()

    def store(address, value):
        cpu.pointer(address, value)

    def intercept(uc, address, size, unused):
        if address == GET_LEVEL:
            if cpu.reg(0) != character:
                raise AssertionError("GetLevel received a different Character")
            cpu.put(0, active[0][0])
            uc.reg_write(cpu.pc, uc.reg_read(cpu.lr))
        elif address == GET_CHAR_SKILL:
            if cpu.reg(0) != character or cpu.reg(1) != 0:
                raise AssertionError("GetCharSkill source arguments changed")
            cpu.put(0, skill_row)
            uc.reg_write(cpu.pc, uc.reg_read(cpu.lr))

    active = [(0, 0, 0, 0)]
    hook = cpu.uc.hook_add(UC_HOOK_CODE, intercept)
    records = []
    try:
        for level, requirement, saved, save_state in scenarios():
            active[0] = (level, requirement, saved, save_state)
            cpu.uc.mem_write(character, bytes(0x1600))
            cpu.uc.mem_write(savegame, bytes(0x100))
            cpu.uc.mem_write(saved_rows, bytes(0x20))
            cpu.uc.mem_write(skill_row, bytes(0x40))
            if save_state:
                store(character + 0x14e8, savegame)
                store(savegame + 0x80, saved_rows if save_state == 1 else 0)
                cpu.uc.mem_write(saved_rows + 4, struct.pack("<H", saved))
            else:
                store(character + 0x14e8, 0)
            store(skill_row + 0x20, requirement & 0xffffffff)

            can_original = cpu.invoke(CAN_INCREMENT, [character, 0])
            can_host = int(run([executable, "predicate", "can", level,
                                requirement, saved, save_state], env=env).strip())
            if can_original != can_host:
                raise AssertionError(("CanIncrementSkill", level, requirement,
                                      saved, save_state, can_original, can_host))
            available_original = cpu.invoke(IS_AVAILABLE, [character, 0])
            available_host = int(run([executable, "predicate", "available",
                                      level, requirement, saved, save_state],
                                     env=env).strip())
            if available_original != available_host:
                raise AssertionError(("IsSkillAvailable", level, requirement,
                                      available_original, available_host))
            records.append({"input": [level, requirement, saved, save_state],
                            "can_increment": can_original,
                            "is_skill_available": available_original})
    finally:
        cpu.uc.hook_del(hook)

    expected = set(range(CAN_INCREMENT, CAN_INCREMENT + 100, 4))
    expected |= set(range(IS_AVAILABLE, IS_AVAILABLE + 52, 4))
    seen = cpu.seen & expected
    if seen != expected:
        missing = sorted(expected - seen)
        raise AssertionError(f"original predicate code not reached: {[hex(x) for x in missing]}")
    return {
        "validation": "PASS", "comparisons": len(records), "mismatches": 0,
        "original_instructions_observed": len(seen),
        "original_instructions_expected": len(expected),
        "dependency_bodies": {
            "Character::GetLevel": "fixture at the pinned source call boundary",
            "Character::GetCharSkill": "fixture returns a mapped Skill row",
        },
        "cases": records,
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", required=True)
    parser.add_argument("--cache", type=Path, required=True)
    parser.add_argument("--original-elf", type=Path, required=True)
    parser.add_argument(
        "--output", type=Path,
        default=Path(tempfile.gettempdir()) / "dh2-player-skill-progression-v1")
    parser.add_argument(
        "--report", type=Path,
        default=MODULE / "build/player-skill-progression-v1/validation.json")
    args = parser.parse_args()
    original = args.original_elf.resolve()
    manifest = verify_manifest(original)
    compiled = compile_selected(args)
    before_paths = set(compiled["sources"])
    before_paths.update({MODULE / "player_skill_progression_v1.hpp",
                         MODULE / "player_skill_progression_v1.cpp",
                         MODULE / "tests/player_skill_progression_v1_host.cpp",
                         Path(__file__).resolve(), MANIFEST,
                         compiled["wrapper"] / "CMakeLists.txt",
                         ROOT / "port/game-data/CMakeLists.txt",
                         ROOT / "port/engine-resources/tests/cpu.py"})
    before = {path.resolve().relative_to(ROOT).as_posix(): sha(path.resolve())
              for path in before_paths if path.resolve().is_relative_to(ROOT)}
    cache = args.cache.resolve()
    cache_inputs = {}
    for filename in ("skills_pyarray.bin", "skills_pyarraynames.bin",
                     "skills_pystructnames.bin"):
        path = cache / "data/pydata" / filename
        cache_inputs[path.relative_to(cache).as_posix()] = sha(path)
    host = json.loads(run([compiled["binary"], cache], env=compiled["env"]))
    if host.get("validation") != "PASS":
        raise RuntimeError("host regression did not pass")
    arm = original_compare(original, compiled["binary"], compiled["env"], manifest)
    if any(sha(Path(ROOT / path)) != digest for path, digest in before.items()):
        raise RuntimeError("source input changed during build/replay")
    if any(sha(cache / path) != digest for path, digest in cache_inputs.items()):
        raise RuntimeError("cache input changed during replay")
    report_path = args.report
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report = {
        "validation": "PASS", "host_report": host, "original_arm": arm,
        "original_sha256": sha(original), "cache_inputs_sha256": cache_inputs,
        "source_sha256": before,
        "binary_sha256": {
            "host_executable": sha(compiled["binary"]),
            "selected_dh2_game_data": sha(compiled["dso"]),
        },
        "selected_sources": sorted(path.relative_to(ROOT).as_posix()
                                    for path in compiled["sources"]
                                    if path.is_relative_to(ROOT)),
        "selected_commands": compiled["selected_commands"],
        "compiler_commands": compiled["commands"],
        "build_stdout": compiled["logs"],
        "scope": "Original CanIncrementSkill/IsSkillAvailable bodies are instruction-compared with GetLevel/GetCharSkill as pinned typed fixtures. IncSkill and _InitSkillsSlots are host-tested service-orchestration projections; Character design/debug/property/CharAI operations remain borrowed boundaries. No starter skill or complete player InitPost claim.",
    }
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"validation": "PASS", "host_cases": host["host_cases"],
                      "arm_comparisons": arm["comparisons"],
                      "arm_instructions": arm["original_instructions_observed"],
                      "report": str(report_path)}))


if __name__ == "__main__":
    main()
