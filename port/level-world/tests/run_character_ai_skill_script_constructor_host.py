"""Compare the bounded CharAISkillScript constructor against original ARM."""
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
MANIFEST = MODULE / "reference/character-ai-skill-script-constructor/original-functions.json"
ORIGINAL_SHA = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"
CONSTRUCTOR = "_ZN17CharAISkillScriptC1EP9CharacterPKcj"
ARGUMENTS_CTOR = "_ZN3sfc6script3lua9ArgumentsC1Ev"
PUSH_STRING = "_ZN3sfc6script3lua9Arguments10pushStringEPKc"
PUSH_INTEGER = "_ZN3sfc6script3lua9Arguments11pushIntegerEi"
VTABLE = "_ZTV17CharAISkillScript"
ASSERT_LEVEL = "gAssertLevel"
CASES = ("valid", "null_owner", "null_name", "both_null")


def setup(cpu, name):
    base = cpu.data + 0x10000
    object_address = base + 0x100
    owner = 0 if name in ("null_owner", "both_null") else base + 0x300
    script_name = 0 if name in ("null_name", "both_null") else base + 0x500
    if script_name:
        cpu.uc.mem_write(script_name, b"Faery_Bolt\0")
    cpu.pointer(cpu.symbols[ASSERT_LEVEL], 0)
    return object_address, owner, script_name


def read_cstring(cpu, address):
    if not address:
        return None
    result = bytearray()
    for offset in range(256):
        value = cpu.uc.mem_read(address + offset, 1)[0]
        if value == 0:
            return result.decode("ascii")
        result.append(value)
    raise AssertionError("unterminated skill script name")


def arm_trace(original: Path, name: str):
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0, str(ROOT / "port/engine-resources/tests"))
    from cpu import Cpu

    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    cpu = Cpu(original, False, manifest)
    object_address, owner, script_name = setup(cpu, name)
    calls = []
    addresses = {
        cpu.symbols[ARGUMENTS_CTOR]: "construct",
        cpu.symbols[PUSH_STRING]: "push_string",
        cpu.symbols[PUSH_INTEGER]: "push_integer",
    }

    def observe(uc, address, _size, _user):
        operation = addresses.get(address)
        if operation is None:
            return
        args = cpu.reg(0)
        if operation == "push_string":
            text_pointer = cpu.reg(1)
            calls.append({"op": operation, "args_offset": args - object_address,
                          "text": read_cstring(cpu, text_pointer), "value": 0})
        elif operation == "push_integer":
            calls.append({"op": operation, "args_offset": args - object_address,
                          "text": None, "value": cpu.reg(1)})
        else:
            calls.append({"op": operation, "args_offset": args - object_address,
                          "text": None, "value": 0})
        cpu.uc.reg_write(cpu.pc, cpu.uc.reg_read(cpu.lr))

    hook = cpu.uc.hook_add(UC_HOOK_CODE, observe)
    returned = cpu.invoke(CONSTRUCTOR, [object_address, owner, script_name, 6], budget=500_000)
    cpu.uc.hook_del(hook)
    source_vtable = cpu.symbols[VTABLE] + 8
    object_words = struct.unpack("<7I", cpu.uc.mem_read(object_address, 28))
    assert returned == object_address
    assert object_words[0] == source_vtable and object_words[1] == owner
    assert object_words[2] == script_name and object_words[5] == 6 and object_words[6] == 0xffffffff
    assert [call["op"] for call in calls] == ["construct", "push_string", "push_integer"]
    assert all(call["args_offset"] == 12 for call in calls)
    assert cpu.symbols[CONSTRUCTOR] in cpu.seen
    return {"owner_is_null": owner == 0, "name_is_null": script_name == 0,
            "calls": calls, "return_this": True, "vtable_address_point": hex(source_vtable)}


def original_comparison(original: Path, executable: Path):
    from elftools.elf.elffile import ELFFile

    raw = original.read_bytes()
    assert hashlib.sha256(raw).hexdigest() == ORIGINAL_SHA
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
    for name in CASES:
        host = json.loads(subprocess.check_output([str(executable), name], text=True))
        arm = arm_trace(original, name)
        assert host["status"] == 0 and host["dispatch"] == 1, (name, host)
        assert host["index"] == 6 and host["last_id"] == -1 and host["returned"] == 0x1000
        assert host["owner"] == (0 if arm["owner_is_null"] else 0x2000)
        assert len(host["calls"]) == len(arm["calls"]) == 3
        for expected, actual in zip(host["calls"], arm["calls"]):
            assert {key: expected[key] for key in ("op", "args_offset", "text", "value")} == actual, (name, expected, actual)
        rows.append({"case": name, "matched": True,
                     "owner_is_null": arm["owner_is_null"],
                     "name_is_null": arm["name_is_null"],
                     "call_order": [call["op"] for call in arm["calls"]]})
    return {"validation": "PASS", "comparisons": len(rows), "mismatches": 0,
            "results": rows,
            "executed_scope": (
                "The original 336-byte CharAISkillScript constructor executes. Its embedded "
                "Arguments constructor, pushString and pushInteger callees are intercepted "
                "at their original call addresses and recorded; assertion diagnostics are "
                "outside this valid/assert-level-zero fixture scope."),
            "native_wired": False}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler")
    parser.add_argument("--original-elf", type=Path, required=True)
    parser.add_argument("--output", type=Path,
                        default=MODULE / "build/character-ai-skill-script-constructor/host.exe")
    parser.add_argument("--report", type=Path,
                        default=MODULE / "build/character-ai-skill-script-constructor/validation.json")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++")
    if not compiler:
        parser.error("pass --compiler")
    executable = args.output.resolve()
    executable.parent.mkdir(parents=True, exist_ok=True)
    sources = [MODULE / "character_ai_skill_script_constructor.cpp",
               MODULE / "tests/character_ai_skill_script_constructor.cpp"]
    command = [compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror", "-pedantic",
               *map(str, sources), "-o", str(executable)]
    compiled = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    if compiled.returncode:
        raise RuntimeError(compiled.stdout + compiled.stderr)
    host = json.loads(subprocess.check_output([str(executable)], text=True))
    assert host["validation"] == "PASS" and host["host_cases"] == 9
    arm = original_comparison(args.original_elf.resolve(), executable)
    paths = sources + [MODULE / "character_ai_skill_script_constructor.hpp", Path(__file__).resolve(),
                       MANIFEST, MANIFEST.with_name("NOTES.md")]
    report = {"validation": "PASS", "host_report": host,
              "original_arm_comparison": arm, "original_sha256": ORIGINAL_SHA,
              "compiler_command": command,
              "source_sha256": {path.relative_to(ROOT).as_posix(): hashlib.sha256(path.read_bytes()).hexdigest()
                                for path in paths},
              "native_wired": False}
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"validation": "PASS", "host_cases": host["host_cases"],
                      "original_arm_cases": arm["comparisons"], "mismatches": 0}))


if __name__ == "__main__":
    main()
