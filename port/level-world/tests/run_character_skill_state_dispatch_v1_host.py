"""Selected state6 projection versus original ARM registrations and callbacks."""
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
MANIFEST = MODULE / "reference/character-skill-state-dispatch-v1/original-functions.json"


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(command, env=None):
    result = subprocess.run(list(map(str, command)), cwd=ROOT, env=env,
                            capture_output=True, text=True)
    if result.returncode:
        raise RuntimeError(f"{command}\nexit={result.returncode}\n{result.stdout}\n{result.stderr}")
    return result.stdout


def verify(original):
    from elftools.elf.elffile import ELFFile
    manifest = json.loads(MANIFEST.read_text())
    assert sha(original) == manifest["original_sha256"]
    raw = original.read_bytes()
    with original.open("rb") as stream:
        elf = ELFFile(stream)
        symbols = {s.name: s for s in elf.get_section_by_name(".symtab").iter_symbols()}
        loads = [s for s in elf.iter_segments() if s["p_type"] == "PT_LOAD"]

        def data(at, size):
            segment = next(s for s in loads if s["p_vaddr"] <= at and
                           at + size <= s["p_vaddr"] + s["p_filesz"])
            offset = int(segment["p_offset"]) + at - int(segment["p_vaddr"])
            return raw[offset:offset + size]

        for row in manifest["functions"]:
            at, size = int(row["elf_address"], 0), row["size"]
            symbol = symbols[row["original_symbol"]]
            assert (symbol["st_value"], symbol["st_size"]) == (at, size)
            assert hashlib.sha256(data(at, size)).hexdigest() == row["sha256"]
        literal = manifest["event_literal"]
        text = literal["text"].encode() + b"\0"
        assert data(int(literal["elf_address"], 0), len(text)) == text
        assert hashlib.sha256(text).hexdigest() == literal["sha256"]
        timer = manifest["source_timer"]
        assert data(int(timer["event_instruction"], 0), 4).hex() == timer["event_instruction_bytes"] == "3030a0e3"
        # Original Blur: duration10, repeat0, event48, null userRef.
        assert [data(at, 4).hex() for at in (0x3c4414, 0x3c4418, 0x3c441c, 0x3c4420, 0x3c4428)] == [
            "00c0a0e3", "0c20a0e1", "0a10a0e3", "3030a0e3", "00c08de5"]
    return manifest


def oracle(original, exe, env, manifest):
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0, str(ROOT / "port/engine-resources/tests"))
    from cpu import Cpu
    sys.path.insert(0, str(MODULE / "tests"))
    from elf_import_identity import verify_imports
    imports = verify_imports(original, {0x30e31c: "strcmp"})
    cpu = Cpu(original, False, manifest)
    character = cpu.data + 0x1000
    machine = character + 0x4fc
    payload = cpu.data + 0x2000
    next_ref = cpu.data + 0x3000
    registrations = {}
    instructions = set()

    def word(at):
        return struct.unpack("<I", cpu.uc.mem_read(at, 4))[0]

    def returned(value=0):
        cpu.put(0, value)
        cpu.uc.reg_write(cpu.pc, cpu.uc.reg_read(cpu.lr))

    def label(at):
        return bytes(cpu.uc.mem_read(at, 128)).split(b"\0")[0]

    def hook(_, at, __, ___):
        for low, high in [(0x3c0ac8, 0x3c0b00), (0x3c0018, 0x3c001c),
                          (0x3c8438, 0x3c85ac), (0x3ad290, 0x3ad29c),
                          (0x3ad244, 0x3ad280)]:
            if low <= at < high:
                instructions.add(at)
        if at == 0x3c7b18:
            assert cpu.reg(0) == machine
            registrations.setdefault(cpu.reg(1), {})[cpu.reg(2)] = [
                cpu.reg(3), word(cpu.uc.reg_read(cpu.sp)), word(cpu.uc.reg_read(cpu.sp) + 4)]
            returned()
        elif at == 0x30e31c:
            assert label(cpu.reg(1)) == b"is_stoppable"
            left, right = label(cpu.reg(0)), label(cpu.reg(1))
            returned(0 if left == right else 1)

    hook_id = cpu.uc.hook_add(UC_HOOK_CODE, hook)
    try:
        for state, at in [(3, 0x3c7e60), (4, 0x3c80ac), (5, 0x3c8284), (6, 0x3c8438)]:
            cpu.uc.mem_write(character, bytes(0x800))
            cpu.invoke(at, [0, state, character, machine])
        selected = {event: row for event, row in registrations[6].items()}
        assert selected == {
            0x22: [3, 0, 0], 0xc358: [12, 0, 0],
            0xc351: [4, 0x3ad290, 0], 0xc354: [5, 0x3ad290, 0],
            0xc355: [6, 0x3ad290, 0], 0xc35a: [11, 0x3ad244, 0],
            0xc35c: [9, 0x3ad244, 0], 0xc35d: [8, 0x3ad244, 0],
            0xc35b: [10, 0x3ad244, 0]}
        assert all(registrations[state][0xc355] == [6, 0, 0] for state in (3, 4, 5))
        events = [0x22, 0x23, 0x28, 0x30, 0x2a, 0x2b, 0x2c,
                  0xc351, 0xc354, 0xc355, 0xc358, 0xc35a, 0xc35b, 0xc35c, 0xc35d]
        cases = [[6, flags, event, "is_stoppable"] for flags, event in itertools.product(
            [0, 0x6341, 0x8000, 0x10000, 0x18000, 0xffffffff], events)]
        cases += [[6, flags, 0x28, text] for flags, text in itertools.product(
            [0, 0x6341, 0xffffffff], ["", "is_stop", "is_stoppableX", "IS_STOPPABLE", "do_skill"])]
        rng = random.Random(0x3c0ac8)
        cases += [[6, rng.getrandbits(32), rng.choice(events), rng.choice(["is_stoppable", "is_stoppableX", "x"])] for _ in range(100)]
        cases += [[state, rng.getrandbits(32), event, "unused"] for state, event in itertools.product((3, 4, 5), (0xc355, 0x30, 0xc351))]
        records = []
        for state, flags, event, text in cases:
            cpu.uc.mem_write(character, bytes(0x800))
            cpu.pointer(character + 0x520, flags)
            cpu.uc.mem_write(payload, text.encode() + b"\0" + bytes(128))
            if state == 6:
                cpu.invoke(0x3c0ac8, [0, 6, character, machine, event, payload])
            registration = registrations[state].get(event) if state == 6 or event == 0xc355 else None
            destination, predicate, accepted = -1, 0, False
            if registration:
                destination, callee, adjust = registration
                assert not adjust
                if callee:
                    predicate = cpu.invoke(callee, [character, event, payload, state, next_ref])
                    accepted = bool(predicate)
                else:
                    accepted = True
            if not accepted:
                destination = -1
            expected = {"status": 6 if destination in (8, 9, 10, 11) else 0,
                        "next": destination, "flags": word(character + 0x520),
                        "writes": int(state == 6 and event == 0x28 and text == "is_stoppable"),
                        "predicate": predicate, "registered": int(registration is not None),
                        "current": state, "elapsed": 93}
            actual = json.loads(run([exe, "--oracle", state, flags, event, 1, text], env))
            assert actual == expected, ([state, flags, event, text], expected, actual)
            records.append({"input": [state, flags, event, text], "original": expected, "compiled": actual})
        before = bytes(cpu.uc.mem_read(character, 0x800))
        cpu.invoke(0x3c0018, [0, 6, character, machine])
        assert bytes(cpu.uc.mem_read(character, 0x800)) == before
    finally:
        cpu.uc.hook_del(hook_id)
    required = (set(range(0x3c0ac8, 0x3c0b00, 4)) | {0x3c0018} |
                set(range(0x3c8438, 0x3c85ac, 4)) | set(range(0x3ad290, 0x3ad29c, 4)) |
                {0x3ad244, 0x3ad248, 0x3ad264, 0x3ad268, 0x3ad26c})
    assert instructions == required, sorted(required - instructions)
    return {"validation": "PASS", "comparisons": len(records), "mismatches": 0,
            "normal_instruction_addresses": [hex(x) for x in sorted(instructions)],
            "verified_imports": imports,
            "source_registrations": {str(state): {hex(event): row for event, row in rows.items()} for state, rows in registrations.items()},
            "results": records,
            "scope": "Actual original OnEvent/OnUpdate, full OnInit registration and source StopSkill predicate execute; Interrupted state6 branch executes. strcmp is an identified libc boundary. No unimplemented state8/9/10/11 activation or Player lifecycle is credited."}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", required=True, type=Path)
    parser.add_argument("--original-elf", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--report", type=Path, default=MODULE / "build/character-skill-state-dispatch-v1/validation.json")
    args = parser.parse_args()
    output = args.output.resolve()
    wrapper, build = output / "wrapper", output / "selected"
    wrapper.mkdir(parents=True, exist_ok=True)
    build.mkdir(parents=True, exist_ok=True)
    compiler = args.compiler.resolve()
    c_compiler = compiler.with_name(compiler.name.replace("g++", "gcc"))
    cmake, ninja = shutil.which("cmake"), shutil.which("ninja")
    source = MODULE / "character_skill_state_dispatch_v1.cpp"
    test = MODULE / "tests/character_skill_state_dispatch_v1.cpp"
    cmake_text = f'''cmake_minimum_required(VERSION 3.22)
project(skill_state_selected LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-world)
add_executable(skill_state_audit "{test.as_posix()}")
target_compile_features(skill_state_audit PRIVATE cxx_std_17)
target_compile_options(skill_state_audit PRIVATE -Wall -Wextra -Werror -pedantic)
target_link_libraries(skill_state_audit PRIVATE dh2_level_world)
'''
    (wrapper / "CMakeLists.txt").write_text(cmake_text, encoding="utf-8")
    logs = [run([cmake, "-S", wrapper, "-B", build, "-G", "Ninja",
                 f"-DCMAKE_MAKE_PROGRAM={Path(ninja).as_posix()}", f"-DCMAKE_CXX_COMPILER={compiler.as_posix()}",
                 f"-DCMAKE_C_COMPILER={c_compiler.as_posix()}", "-DCMAKE_BUILD_TYPE=Release",
                 "-DCMAKE_CXX_FLAGS_RELEASE=-O1", "-DCMAKE_C_FLAGS_RELEASE=-O1"])]
    commands = run([ninja, "-C", build, "-t", "commands", "skill_state_audit"])
    database = json.loads((build / "compile_commands.json").read_text())
    normalized = commands.replace("\\", "/")
    sources = {Path(row["file"]).resolve() for row in database if str(row["file"]).replace("\\", "/") in normalized}
    assert source in sources and MODULE / "character_skill_fsm_callbacks_v1.cpp" in sources
    # Only the host fixture is an executable source: callers are selected DLL
    # inputs, never compiled again into the executable or an audit overlay.
    assert "character_skill_state_dispatch_v1.cpp.obj" in commands
    paths = sources | {Path(__file__).resolve(), MANIFEST, MANIFEST.with_name("original-functions.asm"),
                       MODULE / "tests/elf_import_identity.py", ROOT / "port/engine-resources/tests/cpu.py"}
    for folder in {path.parent for path in sources}:
        paths.update(folder.glob("*.h")); paths.update(folder.glob("*.hpp"))
        if (folder / "CMakeLists.txt").exists():
            paths.add(folder / "CMakeLists.txt")
    before = {path.relative_to(ROOT).as_posix(): sha(path) for path in paths if path.is_relative_to(ROOT)}
    manifest = verify(args.original_elf.resolve())
    logs.append(run([cmake, "--build", build, "--target", "skill_state_audit", "--parallel", "1"]))
    assert all(sha(ROOT / name) == value for name, value in before.items()), "source changed during build"
    exe = build / "skill_state_audit.exe"
    libraries = sorted(build.rglob("*.dll"))
    env = os.environ.copy()
    env["PATH"] = os.pathsep.join([str(compiler.parent), *(str(path.parent) for path in libraries), env.get("PATH", "")])
    imports = {path.relative_to(output).as_posix(): re.findall(r"DLL Name: (\S+)", run([compiler.with_name("objdump.exe"), "-p", path])) for path in [exe, *libraries]}
    assert any("dh2_level_world" in item for item in imports[exe.relative_to(output).as_posix()])
    host = json.loads(run([exe], env))
    assert host["validation"] == "PASS" and host["functional_cases"] == 60 and host["failure_cases"] == 10 and host["guards"] == 6
    arm = oracle(args.original_elf.resolve(), exe, env, manifest)
    assert all(sha(ROOT / name) == value for name, value in before.items()), "source changed during validation"
    report = {"validation": "PASS", "host_report": host, "original_arm_comparison": arm,
              "original_sha256": sha(args.original_elf), "new_complete_bodies": 2,
              "registration_evidence": True, "source_timer": manifest["source_timer"],
              "source_sha256": before, "attribution": manifest["attribution"],
              "selected_commands": commands, "selected_dso_imports": imports,
              "binary_sha256": {path.relative_to(output).as_posix(): sha(path) for path in [exe, *libraries]},
              "wrapper_cmake": cmake_text, "build_stdout": logs, "native_wired": False,
              "scope": "Selected DLL state6 projection, reused frozen Focus/Blur, source event labels/predicates and single Coordinator/timer fields. Full Player AIS load, animation event lifecycle, target selection, use invocation and native state6 integration remain owning runtime work."}
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"host": host, "original_cases": arm["comparisons"],
                      "instructions": len(arm["normal_instruction_addresses"]), "mismatches": 0}))


if __name__ == "__main__":
    main()
