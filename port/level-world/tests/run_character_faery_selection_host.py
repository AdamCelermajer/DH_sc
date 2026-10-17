"""Compare the bounded Faery-list selector with its original ARM body."""
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
MANIFEST = MODULE / "reference/character-faery-selection/original-functions.json"
ORIGINAL_SHA = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"
GET_CHAR_FAERY = "_ZNK9Character12GetCharFaeryEi"
GET_COUNT = "_ZNK15PyDataConstants11getConstantEPKcS1_"
LIST_MEMBERS = "_ZN6Arrays14FaeryListTable7membersE"
LIST_SIZE = "_ZN6Arrays14FaeryListTable4sizeE"
FAERY_MEMBERS = "_ZN6Arrays10FaeryTable7membersE"
FAERY_SIZE = "_ZN6Arrays10FaeryTable4sizeE"
ASSERT_LEVEL = "gAssertLevel"
CASES = ("valid_explicit", "invalid_list_fallback", "first_count_small",
         "type_mismatch", "list_size_mismatch")


def setup(cpu, name: str):
    import struct
    base = cpu.data + 0x10000
    character = base + 0x100
    lists = base + 0x200
    members = base + 0x400
    faeries = base + 0x500
    member_ids = (2, 4, 5, 6, 3)
    if name == "valid_explicit":
        list_id, index, counts, list_size = 1, 2, (7, 7), 5
    elif name == "invalid_list_fallback":
        list_id, index, counts, list_size = -1, 4, (7, 7), 5
    elif name == "first_count_small":
        list_id, index, counts, list_size = 0, 4, (2, 7), 5
    elif name == "type_mismatch":
        list_id, index, counts, list_size = 0, 2, (7, 7), 5
    elif name == "list_size_mismatch":
        list_id, index, counts, list_size = 0, 2, (7, 6), 5
    else:
        raise ValueError(name)
    cpu.uc.mem_write(character + 0x106c, struct.pack("<i", list_id))
    for row in range(2):
        cpu.uc.mem_write(lists + row * 12,
                         struct.pack("<IiI", row, list_size, members))
    cpu.uc.mem_write(members, struct.pack("<5i", *member_ids))
    table_words = [0] * (7 * 9)
    for list_slot, table_row in enumerate(member_ids):
        table_words[table_row * 9 + 8] = list_slot
    if name == "type_mismatch":
        table_words[5 * 9 + 8] = 6
    cpu.uc.mem_write(faeries, struct.pack("<" + "I" * len(table_words), *table_words))
    cpu.pointer(cpu.symbols[LIST_MEMBERS], lists)
    cpu.pointer(cpu.symbols[LIST_SIZE], 2)
    cpu.pointer(cpu.symbols[FAERY_MEMBERS], faeries)
    cpu.pointer(cpu.symbols[FAERY_SIZE], 7)
    cpu.pointer(cpu.symbols[ASSERT_LEVEL], 0)
    return character, index, counts, faeries


def read_cstring(cpu, address):
    result = bytearray()
    for offset in range(128):
        value = cpu.uc.mem_read(address + offset, 1)[0]
        if value == 0:
            return result.decode("ascii")
        result.append(value)
    raise AssertionError("unterminated source constant name")


def arm_trace(original: Path, name: str):
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0, str(ROOT / "port/engine-resources/tests"))
    from cpu import Cpu

    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    cpu = Cpu(original, False, manifest)
    character, index, counts, faery_base = setup(cpu, name)
    queries = []
    returned_counts = []
    count_address = cpu.symbols[GET_COUNT]

    def observe(uc, address, _size, _user):
        if address != count_address:
            return
        manager, category, key = [cpu.reg(i) for i in range(3)]
        category_text, key_text = read_cstring(cpu, category), read_cstring(cpu, key)
        queries.append({"category": category_text, "key": key_text,
                        "manager": manager})
        value = counts[min(len(queries) - 1, len(counts) - 1)]
        returned_counts.append(value)
        cpu.put(0, value)
        cpu.uc.reg_write(cpu.pc, cpu.uc.reg_read(cpu.lr))

    hook = cpu.uc.hook_add(UC_HOOK_CODE, observe)
    result_pointer = cpu.invoke(GET_CHAR_FAERY, [character, index], budget=1_000_000)
    cpu.uc.hook_del(hook)
    assert len(queries) == len(counts) == 2
    expected_member = (2, 4, 5, 6, 3)[index]
    expected_row = (result_pointer - faery_base) // 36
    assert result_pointer == faery_base + expected_member * 36
    assert all(q["category"] == "FaeryTypes" and q["key"] == "COUNT" for q in queries)
    assert cpu.symbols[GET_CHAR_FAERY] in cpu.seen
    return {"row_index": expected_row, "queries": len(queries),
            "returned_counts": returned_counts, "calls": queries,
            "source_instructions_seen": len(cpu.seen)}


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
        assert host["status"] == 0, (name, host)
        assert host["row_index"] == arm["row_index"], (name, host, arm)
        assert host["queries"] == arm["queries"], (name, host, arm)
        rows.append({"case": name, "matched": True,
                     "row_index": arm["row_index"],
                     "constant_queries": arm["queries"],
                     "returned_counts": arm["returned_counts"]})
    return {"validation": "PASS", "comparisons": len(rows),
            "mismatches": 0, "results": rows,
            "executed_scope": (
                "The original 540-byte Character::GetCharFaery body and its 60-byte "
                "GetCharFaeryListId leaf execute as ARM. Each PyDataConstants::getConstant "
                "callee is intercepted with controlled COUNT results; all list fallback, "
                "assert-level-zero continuation, member lookup and row selection instructions "
                "execute from the original image. Diagnostics are outside the fixture scope."),
            "native_wired": False}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler")
    parser.add_argument("--original-elf", type=Path, required=True)
    parser.add_argument("--output", type=Path,
                        default=MODULE / "build/character-faery-selection/host.exe")
    parser.add_argument("--report", type=Path,
                        default=MODULE / "build/character-faery-selection/validation.json")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++")
    if not compiler:
        parser.error("pass --compiler")
    executable = args.output.resolve()
    executable.parent.mkdir(parents=True, exist_ok=True)
    sources = [MODULE / "character_faery_selection.cpp",
               MODULE / "tests/character_faery_selection.cpp"]
    command = [compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror", "-pedantic",
               *map(str, sources), "-o", str(executable)]
    compiled = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    if compiled.returncode:
        raise RuntimeError(compiled.stdout + compiled.stderr)
    host = json.loads(subprocess.check_output([str(executable)], text=True))
    assert host["validation"] == "PASS" and host["host_cases"] == 12
    arm = original_comparison(args.original_elf.resolve(), executable)
    paths = sources + [MODULE / "character_faery_selection.hpp", Path(__file__).resolve(),
                       MANIFEST, MANIFEST.with_name("NOTES.md")]
    report = {
        "validation": "PASS", "host_report": host,
        "original_arm_comparison": arm, "original_sha256": ORIGINAL_SHA,
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
