"""Compare the maintained UpdateAllSkills caller against its recovered ARM body."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
MANIFEST = MODULE / "reference/character-ai-update-all-skills/original-functions.json"
SHA = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"
AI = 0x100143C8
OWNER_A = 0x10018000
OWNER_B = 0x10019000
SKILL = 0x10030000
SKILL_REPLACEMENT = 0x10031000
FAERY = 0x10032000
FAERY_REPLACEMENT = 0x10033000


def scenarios():
    return ["baseline", "using", "casting", "owner_mutation", "skill_rebase",
            "faery_replace_from_skill", "faery_rebase", "null_slots", "empty"]


def setup_memory(cpu, name):
    cpu.uc.mem_map(0x10000000, 0x60000)
    cpu.uc.mem_write(AI - 0x3C8, bytes(0x5000))
    cpu.pointer(AI + 4, OWNER_A)
    skill_values = [0x1101, 0, 0x1102]
    skill_new = [0x1190, 0x1191, 0x1192]
    faery_values = [0x2201, 0x2202]
    faery_new = [0x2290, 0x2291, 0x2292]
    if name == "faery_replace_from_skill":
        skill_values = [0x1101]
        faery_values = [0x2201]
    elif name == "faery_rebase":
        faery_values = [0x2201, 0x2202, 0x2203]
    elif name == "null_slots":
        skill_values = [0, 0]
        faery_values = [0]
    elif name == "empty":
        skill_values = []
        faery_values = []
    def write_words(address, values):
        if values:
            cpu.uc.mem_write(address, struct.pack("<" + "I" * len(values), *values))
    write_words(SKILL, skill_values)
    write_words(SKILL_REPLACEMENT, skill_new)
    write_words(FAERY, faery_values)
    write_words(FAERY_REPLACEMENT, faery_new)
    cpu.pointer(AI + 0xB4, SKILL if skill_values else 0)
    cpu.pointer(AI + 0xB8, SKILL + len(skill_values) * 4 if skill_values else 0)
    cpu.pointer(AI + 0xC0, FAERY if faery_values else 0)
    cpu.pointer(AI + 0xC4, FAERY + len(faery_values) * 4 if faery_values else 0)
    using = 7 if name == "using" else 0
    casting = 1 if name == "casting" else 0
    return using, casting


def arm_trace(original: Path, name: str):
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0, str(ROOT / "port/engine-resources/tests"))
    from cpu import Cpu

    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    cpu = Cpu(original, False, manifest)
    using, casting = setup_memory(cpu, name)
    calls = []
    updates = 0
    phase = "skill"

    def returned(value=0):
        cpu.put(0, value)
        cpu.uc.reg_write(cpu.pc, cpu.uc.reg_read(cpu.lr))

    def observe(uc, address, _size, _user):
        nonlocal updates, phase
        if address == 0x3D8908:
            phase = "faery"
        elif address == 0x3C02E8:
            subject = cpu.reg(0) - 0x4FC
            calls.append([0, 0, subject, 0])
            if name == "owner_mutation":
                cpu.pointer(AI + 4, OWNER_B)
            returned(using)
        elif address == 0x3C0334:
            subject = cpu.reg(0) - 0x4FC
            calls.append([1, 0, subject, 0])
            returned(casting)
        elif address == 0x3DABD0:
            script = cpu.reg(0)
            index = cpu.reg(4) - 1
            list_id = 1 if phase == "skill" else 2
            calls.append([2, list_id, script, index])
            updates += 1
            if name == "skill_rebase" and phase == "skill" and updates == 1:
                cpu.pointer(AI + 0xB4, SKILL_REPLACEMENT)
            if name == "faery_replace_from_skill" and phase == "skill" and updates == 1:
                cpu.pointer(AI + 0xC0, FAERY_REPLACEMENT)
                cpu.pointer(AI + 0xC4, FAERY_REPLACEMENT + 3 * 4)
            if name == "faery_rebase" and phase == "faery" and updates == 3:
                cpu.pointer(AI + 0xC0, FAERY_REPLACEMENT)
            returned()

    hook = cpu.uc.hook_add(UC_HOOK_CODE, observe)
    cpu.invoke(0x3D8894, [AI], budget=1_000_000)
    cpu.uc.hook_del(hook)
    return calls


def original_comparison(original: Path, executable: Path):
    from elftools.elf.elffile import ELFFile

    raw = original.read_bytes()
    assert hashlib.sha256(raw).hexdigest() == SHA
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    with original.open("rb") as stream:
        elf = ELFFile(stream)
        symbols = {symbol.name: symbol for symbol in elf.get_section_by_name(".symtab").iter_symbols()}
        segments = [segment for segment in elf.iter_segments() if segment["p_type"] == "PT_LOAD"]

        def read_virtual(address: int, size: int) -> bytes:
            segment = next(s for s in segments if int(s["p_vaddr"]) <= address and
                           address + size <= int(s["p_vaddr"]) + int(s["p_filesz"]))
            offset = int(segment["p_offset"]) + address - int(segment["p_vaddr"])
            return raw[offset:offset + size]

        for item in manifest["functions"]:
            address, size = int(item["elf_address"], 0), item["size"]
            symbol = symbols[item["original_symbol"]]
            assert (int(symbol["st_value"]), int(symbol["st_size"])) == (address, size)
            assert hashlib.sha256(read_virtual(address, size)).hexdigest() == item["sha256"]

    rows = []
    for name in scenarios():
        expected = arm_trace(original, name)
        actual = json.loads(subprocess.check_output([str(executable), name], text=True))["calls"]
        assert actual == expected, (name, actual, expected)
        rows.append({"case": name, "matched": True, "calls": actual})
    return {
        "validation": "PASS",
        "comparisons": len(rows),
        "mismatches": 0,
        "results": rows,
        "executed_scope": (
            "The original 180-byte UpdateAllSkills caller executes. The two FSM predicate "
            "callees and CharAISkillScript::OnSkillUpdate are intercepted at their call "
            "boundaries with controlled raw results/void callbacks; the caller's branch, "
            "vector count, per-slot reload, null checks and call order are original ARM."
        ),
        "native_wired": False,
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler")
    parser.add_argument("--original-elf", type=Path, required=True)
    parser.add_argument("--output", type=Path, default=MODULE / "build/character-ai-update-all-skills/host.exe")
    parser.add_argument("--report", type=Path, default=MODULE / "build/character-ai-update-all-skills/validation.json")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++")
    if not compiler:
        parser.error("pass --compiler")
    executable = args.output.resolve()
    executable.parent.mkdir(parents=True, exist_ok=True)
    sources = [MODULE / "character_ai_update_all_skills.cpp", MODULE / "tests/character_ai_update_all_skills.cpp"]
    command = [compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror", "-pedantic",
               *map(str, sources), "-o", str(executable)]
    compiled = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    if compiled.returncode:
        raise RuntimeError(compiled.stdout + compiled.stderr)
    host = json.loads(subprocess.check_output([str(executable)], text=True))
    assert host["validation"] == "PASS" and host["host_cases"] == 14
    arm = original_comparison(args.original_elf.resolve(), executable)
    paths = sources + [MODULE / "character_ai_update_all_skills.hpp", Path(__file__).resolve(),
                       MANIFEST, MANIFEST.with_name("NOTES.md")]
    report = {
        "validation": "PASS",
        "host_report": host,
        "original_arm_comparison": arm,
        "original_sha256": SHA,
        "compiler_command": command,
        "source_sha256": {path.relative_to(ROOT).as_posix(): hashlib.sha256(path.read_bytes()).hexdigest()
                          for path in paths},
        "native_wired": False,
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"validation": "PASS", "host_cases": host["host_cases"],
                      "original_arm_cases": arm["comparisons"], "mismatches": 0}))


if __name__ == "__main__":
    main()
