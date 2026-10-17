"""Borrowed initial skill binding: selected game-data DSO and original ARM callers."""
from __future__ import annotations

import argparse
import hashlib
import itertools
import json
import os
from pathlib import Path
import random
import re
import shutil
import struct
import subprocess
import sys

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
MANIFEST = MODULE / "reference/player-skill-progression-v1/original-functions.json"
ORIGINAL_SHA256 = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(command, env=None):
    completed = subprocess.run(list(map(str, command)), cwd=ROOT, env=env,
                               capture_output=True, text=True)
    if completed.returncode:
        raise RuntimeError(f"{command}\nexit={completed.returncode}\n{completed.stdout}\n{completed.stderr}")
    return completed.stdout


def verify(original):
    from elftools.elf.elffile import ELFFile
    assert sha(original) == ORIGINAL_SHA256
    manifest = json.loads(MANIFEST.read_text())
    raw = original.read_bytes()
    with original.open("rb") as stream:
        elf = ELFFile(stream)
        symbols = {symbol.name: symbol for symbol in elf.get_section_by_name(".symtab").iter_symbols()}
        loads = [segment for segment in elf.iter_segments() if segment["p_type"] == "PT_LOAD"]

        def data(at, size):
            segment = next(segment for segment in loads if segment["p_vaddr"] <= at and
                           at + size <= segment["p_vaddr"] + segment["p_filesz"])
            offset = int(segment["p_offset"]) + at - int(segment["p_vaddr"])
            return raw[offset:offset + size]

        for row in manifest["functions"]:
            symbol = symbols[row["original_symbol"]]
            at, size = int(row["elf_address"], 0), row["size"]
            assert (symbol["st_value"], symbol["st_size"]) == (at, size)
            assert hashlib.sha256(data(at, size)).hexdigest() == row["sha256"]
        helpers = []
        for at, size in [(0x3bbea0, 28), (0x3bbe54, 20), (0x3bbed0, 28),
                         (0x4668dc, 300), (0x3fc6c8, 32), (0x3fc6a0, 8)]:
            matches = [symbol for symbol in symbols.values() if symbol["st_value"] == at and symbol["st_size"] == size]
            assert matches, hex(at)
            helpers.append({"original_symbol": matches[0].name, "elf_address": hex(at), "size": size,
                            "sha256": hashlib.sha256(data(at, size)).hexdigest()})
    manifest["functions"] += helpers
    return manifest


def oracle(original, exe, cache, env, manifest):
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0, str(ROOT / "port/engine-resources/tests"))
    from cpu import Cpu, i32
    cpu = Cpu(original, False, manifest)
    character, saved, rows, skill = [cpu.data + offset for offset in (0x1000, 0x4000, 0x5000, 0x6000)]
    instructions = set()
    facts = {}
    effects = []
    increment_returns = set()
    source_ranges = [(0x3b3a90, 0x3b3b00), (0x3bcc58, 0x3bcf4c),
                     (0x3bc9ec, 0x3bca50), (0x3bca50, 0x3bca84),
                     (0x3fc6c8, 0x3fc6e8), (0x3bbed0, 0x3bbeec), (0x4668dc, 0x466a08)]

    def word(at):
        return struct.unpack("<I", cpu.uc.mem_read(at, 4))[0]

    def half(at):
        return struct.unpack("<H", cpu.uc.mem_read(at, 2))[0]

    def returned(value=0):
        cpu.put(0, value)
        cpu.uc.reg_write(cpu.pc, cpu.uc.reg_read(cpu.lr))

    def label(at):
        return bytes(cpu.uc.mem_read(at, 128)).split(b"\0")[0]

    def hook(_, at, __, ___):
        if any(low <= at < high for low, high in source_ranges):
            instructions.add(at)
        if at in increment_returns:
            facts["source_return"] = cpu.reg(0)
        if at == 0x3bbea0:
            assert cpu.reg(0) == character
            returned(facts["has_slot"])
        elif at == 0x3bbe54:
            assert [cpu.reg(i) for i in range(3)] == [character, 0, 0]
            facts["has_slot"] = 1
            facts["slot0"] = 0
            facts["slot_updates"] += 1
            returned()
        elif at == 0x3bcc58:
            assert [cpu.reg(i) for i in range(3)] == [character, 0, 0]
            facts["increments"] += 1
            increment_returns.add(cpu.uc.reg_read(cpu.lr))
        elif at == 0x3df6e0:
            assert cpu.reg(0) == character + 0x560 and cpu.reg(2) == 0
            property_id = cpu.reg(1)
            assert property_id in (157, 194)
            returned(facts["points"] if property_id == 157 else 12)
        elif at == 0x3bd120:
            assert cpu.reg(0) == character
            returned(facts["level"])
        elif at == 0x3bc784:
            assert [cpu.reg(i) for i in range(2)] == [character, 0]
            returned(skill)
        elif at == 0x4c4bdc:
            lr = cpu.uc.reg_read(cpu.lr)
            cap_index = {0x3bcd58: 0, 0x3bce84: 1, 0x3bcf40: 2}[lr]
            effects.append(4)
            returned(facts["caps"][cap_index])
        elif at == 0x3bb918:
            assert cpu.reg(0) == character
            returned(facts["difficulty"])
        elif at == 0x3e0798:
            assert cpu.reg(0) == character + 0x560 and cpu.reg(1) == 157 and i32(cpu.reg(2)) == -1
            facts["points"] -= 1
            returned()
        elif at == 0x3d8894:
            assert cpu.reg(0) == character + 0x3c8
            effects.append(7)
            returned()
        elif at == 0x3e0810:
            assert [cpu.reg(i) for i in range(2)] == [character + 0x560, 1]
            effects.append(8)
            returned()
        elif at == 0x337888:
            assert cpu.uc.reg_read(cpu.lr) in (0x3bcce4, 0x3bcdbc, 0x3bcef4)
            effects.append(10)
            returned()
        elif at == 0x337a88:
            assert cpu.uc.reg_read(cpu.lr) in (0x3bcd0c, 0x3bcde4, 0x3bcf1c)
            effects.append(11)
            returned()
        elif at == 0x3bcc08:
            # Specialized string constructor takes the end pointer. Its
            # source-fixed start is the same 19-byte debug key in all branches.
            assert label(cpu.reg(1) - 19) == b"isTracingChar_Stats", label(cpu.reg(1) - 19)
            returned()
        elif at == 0x3139ac:
            returned()

    hook_id = cpu.uc.hook_add(UC_HOOK_CODE, hook)
    cases = list(itertools.product((0, 1), (0, 1), (1,), (0, 1), (0, 1, 2), (0, 1), (0, 1), (20,), (25,)))
    # Source-declined level requirement and difficulty caps; these never create points.
    cases += [(2, 10, 1, 0, difficulty, 0, equipment, cap, cap)
              for difficulty, equipment, cap in itertools.product((0, 1, 2), (0, 1), (-1, 0, 1))]
    rng = random.Random(0x3b3a90)
    cases += [(rng.choice((0, 1, 2, 6)), rng.choice((0, 1, 3, 15)), rng.choice((-2, 0, 1, 10)),
               rng.choice((0, 1, 2, 65535)), rng.choice((-1, 0, 1, 2, 3)), rng.randrange(2),
               rng.randrange(2), rng.choice((-1, 0, 1, 2, 20)), rng.choice((-1, 0, 1, 2, 25)))
              for _ in range(36)]
    records = []
    try:
        for points, level, required, saved_level, difficulty, has_slot, equipment, cap0, cap1 in cases:
            cpu.uc.mem_write(character, bytes(0x2200))
            cpu.uc.mem_write(saved, bytes(0x1000))
            cpu.uc.mem_write(rows, bytes(64))
            cpu.uc.mem_write(skill, bytes(64))
            cpu.pointer(character + 0x14e8, saved)
            cpu.pointer(saved + 0x80, rows)
            cpu.pointer(saved + 0x84, 1)
            cpu.uc.mem_write(rows + 4, struct.pack("<H", saved_level))
            cpu.pointer(skill + 0x20, required & 0xffffffff)
            cpu.uc.mem_write(character + 0x37c + 0x2e, bytes([equipment]))
            effects.clear()
            facts.clear()
            facts.update(points=points, level=level, difficulty=difficulty, caps=(cap0, cap1, cap1),
                         has_slot=has_slot, slot0=0 if has_slot else -1, slot_updates=0, increments=0, source_return=0)
            source_return = cpu.invoke(0x3b3a90, [character])
            expected = {"status": 0, "saved_level": half(rows + 4), "points_raw": facts["points"] * 256,
                        "slot0": facts["slot0"], "map1_size": 0,
                        "equipment": cpu.uc.mem_read(character + 0x3aa, 1)[0],
                        "slot_updates": facts["slot_updates"], "increments": facts["increments"],
                        "source_return": source_return if facts["increments"] else 0, "effects": effects.copy()}
            fields = [points, level, required, saved_level, difficulty, has_slot, equipment, cap0, cap1]
            actual = json.loads(run([exe, cache, "--oracle", *fields], env))
            assert expected == actual, (fields, expected, actual)
            records.append({"input": fields, "original": expected, "compiled": actual})
    finally:
        cpu.uc.hook_del(hook_id)
    return {"validation": "PASS", "comparisons": len(records), "mismatches": 0,
            "instruction_addresses": [hex(at) for at in sorted(instructions)],
            "results": records,
            "scope": "Original _InitSkillsSlots/IncSkill/predicate instructions and SwapEquipmentSet/SG_GetSkillLevel execute. Saved-map writer, design/property/GetLevel/GetCharSkill/AI/recalc/debug/string services are explicit dependency fixtures. The compiled binding uses the selected saved-map/property/inventory implementations. No original body credit is added by composition."}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", required=True, type=Path)
    parser.add_argument("--original-elf", required=True, type=Path)
    parser.add_argument("--cache", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--report", type=Path, default=MODULE / "build/player-initial-skill-grants-v1/validation.json")
    args = parser.parse_args()
    output = args.output.resolve()
    wrapper, build = output / "wrapper", output / "selected"
    wrapper.mkdir(parents=True, exist_ok=True)
    compiler = args.compiler.resolve()
    cmake, ninja = shutil.which("cmake"), shutil.which("ninja")
    source = MODULE / "player_initial_skill_grants_v1.cpp"
    test = MODULE / "tests/player_initial_skill_grants_v1.cpp"
    cmake_text = f'''cmake_minimum_required(VERSION 3.22)
project(initial_skill_grants_audit LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-world)
# Stage only missing caller TUs into the actual selected targets. Once parent
# selects them, the same fixture detects and links the existing TUs once.
function(require_one_source target source)
 get_target_property(inputs ${{target}} SOURCES)
 get_target_property(owner_source ${{target}} SOURCE_DIR)
 set(matches 0)
 foreach(input IN LISTS inputs)
  get_filename_component(absolute "${{input}}" ABSOLUTE BASE_DIR "${{owner_source}}")
  if(absolute STREQUAL source)
   math(EXPR matches "${{matches}} + 1")
  endif()
 endforeach()
 if(matches EQUAL 0)
  target_sources(${{target}} PRIVATE "${{source}}")
  set_property(GLOBAL APPEND PROPERTY initial_skill_staged_sources "${{source}}")
 elseif(NOT matches EQUAL 1)
  message(FATAL_ERROR "Initial skill source duplicated: ${{source}}")
 endif()
endfunction()
require_one_source(dh2_level_world "{source.as_posix()}")
require_one_source(dh2_level_world "{(MODULE / 'player_skill_progression_v1.cpp').as_posix()}")
require_one_source(dh2_game_data "{(ROOT / 'port/game-data/player_saved_skill_slots_v1.cpp').as_posix()}")
get_property(staged GLOBAL PROPERTY initial_skill_staged_sources)
file(WRITE "${{CMAKE_BINARY_DIR}}/initial-skill-staged-sources.txt" "${{staged}}")
add_executable(initial_skill_grants_audit "{test.as_posix()}")
target_compile_options(initial_skill_grants_audit PRIVATE -Wall -Wextra -Werror -pedantic)
target_link_libraries(initial_skill_grants_audit PRIVATE dh2_level_world)
'''
    (wrapper / "CMakeLists.txt").write_text(cmake_text, encoding="utf-8")
    logs = [run([cmake, "-S", wrapper, "-B", build, "-G", "Ninja",
                 f"-DCMAKE_MAKE_PROGRAM={Path(ninja).as_posix()}", f"-DCMAKE_CXX_COMPILER={compiler.as_posix()}",
                 f"-DCMAKE_C_COMPILER={compiler.with_name('gcc.exe').as_posix()}", "-DCMAKE_BUILD_TYPE=Release",
                 "-DCMAKE_CXX_FLAGS_RELEASE=-O1", "-DCMAKE_C_FLAGS_RELEASE=-O1"])]
    commands = run([ninja, "-C", build, "-t", "commands", "initial_skill_grants_audit"])
    database = json.loads((build / "compile_commands.json").read_text())
    normalized = commands.replace("\\", "/")
    sources = {Path(row["file"]).resolve() for row in database if str(row["file"]).replace("\\", "/") in normalized}
    assert {source, MODULE / "player_skill_progression_v1.cpp", ROOT / "port/game-data/player_saved_skill_slots_v1.cpp",
            ROOT / "port/game-data/player_savegame_v1.cpp", ROOT / "port/game-data/properties.cpp",
            ROOT / "port/game-data/fresh_inventory_owned_v4.cpp"} <= sources
    closure = (source, MODULE / "player_skill_progression_v1.cpp", ROOT / "port/game-data/player_saved_skill_slots_v1.cpp")
    for unit in closure:
        assert sum(Path(row["file"]).resolve() == unit for row in database) == 1, f"TU must compile exactly once: {unit}"
    paths = sources | {Path(__file__).resolve(), MANIFEST, ROOT / "port/engine-resources/tests/cpu.py",
                       ROOT / "port/game-data/reference/player-creation-v2/original/initial-grants/reference/original-functions.asm"}
    for directory in {path.parent for path in sources}:
        paths.update(directory.glob("*.h")); paths.update(directory.glob("*.hpp"))
        if (directory / "CMakeLists.txt").exists():
            paths.add(directory / "CMakeLists.txt")
    before = {path.relative_to(ROOT).as_posix(): sha(path) for path in paths if path.is_relative_to(ROOT)}
    cache_hashes = {name: sha(args.cache / name) for name in ("loot_table_pyarray.bin", "loot_table_pyarraynames.bin", "loot_table_pystructnames.bin")}
    manifest = verify(args.original_elf.resolve())
    logs.append(run([cmake, "--build", build, "--target", "initial_skill_grants_audit", "--parallel", "1"]))
    assert all(sha(ROOT / name) == value for name, value in before.items()), "source changed during build"
    exe = build / "initial_skill_grants_audit.exe"
    libraries = sorted(build.rglob("*.dll"))
    env = os.environ.copy()
    env["PATH"] = os.pathsep.join([str(compiler.parent), *(str(path.parent) for path in libraries), env.get("PATH", "")])
    imports = {path.relative_to(output).as_posix(): re.findall(r"DLL Name: (\S+)", run([compiler.with_name("objdump.exe"), "-p", path])) for path in [exe, *libraries]}
    assert "libdh2_level_world.dll" in imports[exe.relative_to(output).as_posix()]
    binding = next(path for path in libraries if path.name == "libdh2_level_world.dll")
    assert "libdh2_game_data.dll" in imports[binding.relative_to(output).as_posix()]
    host = json.loads(run([exe, args.cache], env))
    assert host["validation"] == "PASS" and host["normal_cases"] == 113 and host["failure_cases"] == 15 and host["guards"] == 5
    arm = oracle(args.original_elf.resolve(), exe, args.cache, env, manifest)
    assert all(sha(ROOT / name) == value for name, value in before.items()), "source changed during validation"
    assert all(sha(args.cache / name) == value for name, value in cache_hashes.items())
    staged_text = (build / "initial-skill-staged-sources.txt").read_text()
    report = {"validation": "PASS", "host_report": host, "original_arm_comparison": arm,
              "original_sha256": sha(args.original_elf), "source_functions": manifest["functions"],
              "source_sha256": before, "source_cache_sha256": cache_hashes,
              "source_guard_scope": "Guarded superset: selected build target TU closure, every adjacent .h/.hpp header, corresponding CMake files, fixture/runner, pinned manifest/assembly and CPU helper. This is a build stability guard; it does not claim every guarded header is a runtime dependency.",
              "build_commands": commands, "dso_imports": imports, "wrapper_cmake": cmake_text,
              "binary_sha256": {path.relative_to(output).as_posix(): sha(path) for path in [exe, *libraries]},
              "build_stdout": logs, "staged_sources": staged_text.split(";") if staged_text else [],
              "production_module_selected": not staged_text, "native_wired": False,
              "new_complete_original_bodies": 0,
              "scope": "Reusable borrowed save/property/inventory initial-grant composition through actual selected dh2_level_world and dh2_game_data targets. Missing caller TUs are staged once in the wrapper only; production selection is detected on rerun. Outer SG_Load/InitPost/profile/class and native inventory/AI producers remain external. No starter skill or free SkillPoints are synthesized."}
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"host": host, "original_cases": arm["comparisons"], "instructions": len(arm["instruction_addresses"]), "mismatches": 0}))


if __name__ == "__main__":
    main()
