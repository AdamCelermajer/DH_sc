#!/usr/bin/env python3
"""Build the flat CharacterList adapter and exercise its pinned ARM source methods."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys

HERE = Path(__file__).resolve().parent
MODULE = HERE.parent
REPO = HERE.parents[2]
MANIFEST = MODULE / "reference" / "character-aggro-character-list" / "original-functions.json"
EXPECTED_SHA = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"


def require(value: bool, message: str) -> None:
    if not value:
        raise RuntimeError(message)


def sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def source_verify(original: Path, cxx: str, output: Path) -> dict:
    try:
        from elftools.elf.elffile import ELFFile
        from unicorn import UC_HOOK_CODE
        from unicorn.arm_const import UC_ARM_REG_LR, UC_ARM_REG_PC, UC_ARM_REG_R1
    except ImportError as exc:
        raise RuntimeError(f"ARM oracle dependencies unavailable: {exc}") from exc

    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    image = original.read_bytes()
    require(sha(image) == EXPECTED_SHA, "wrong original libDungeonHunter2.so SHA-256")
    require(manifest["original_sha256"] == EXPECTED_SHA, "manifest is not pinned to original ELF")

    with original.open("rb") as stream:
        elf = ELFFile(stream)
        symbols = {symbol.name: symbol for symbol in elf.get_section_by_name(".symtab").iter_symbols()}
        for row in manifest["functions"]:
            symbol = symbols.get(row["original_symbol"])
            require(symbol is not None, "missing ELF symbol: " + row["original_symbol"])
            address = int(row["elf_address"], 0)
            size = int(row["size"])
            require((int(symbol["st_value"]), int(symbol["st_size"])) == (address, size),
                    "symbol range mismatch: " + row["original_symbol"])
            require(sha(image[address:address + size]) == row["sha256"],
                    "instruction bytes changed: " + row["original_symbol"])
        table = symbols["_ZTVN14ObjectSearcher13CharacterListE"]
        table_address = int(table["st_value"])
        table_size = int(table["st_size"])
        table_row = manifest["vtable_ranges"][0]
        require((table_address, table_size) ==
                (int(table_row["elf_address"], 0), int(table_row["size"])),
                "CharacterList vtable range mismatch")
        require(sha(image[table_address:table_address + table_size]) == table_row["sha256"],
                "CharacterList vtable bytes changed")

    # Compile the host adapter first; this is the independent, normalized
    # behavior check for direct flat-list traversal and search results.
    compiler = shutil.which(cxx)
    require(compiler is not None, f"C++ compiler not found: {cxx}")
    output.parent.mkdir(parents=True, exist_ok=True)
    compile_command = [
        compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror", "-pedantic",
        str(MODULE / "character_aggro_target_search.cpp"),
        str(MODULE / "character_aggro_character_list.cpp"),
        str(HERE / "character_aggro_character_list.cpp"),
        "-o", str(output),
    ]
    build = subprocess.run(compile_command, cwd=REPO, text=True, capture_output=True)
    require(build.returncode == 0, "host compile failed:\n" + build.stdout + build.stderr)
    host = subprocess.run([str(output)], cwd=REPO, text=True, capture_output=True)
    require(host.returncode == 0, "host checks failed:\n" + host.stdout + host.stderr)

    # Load the original ARM image and execute the actual CharacterList
    # Reset/AtEnd/Get/GetChar/Next bodies over a 32-bit sentinel fixture.
    sys.path.insert(0, str(REPO / "port" / "engine-resources" / "tests"))
    from cpu import Cpu

    cpu = Cpu(original, False, manifest)
    base = cpu.data
    wrapper = base + 0x1000
    sentinel = base + 0x2000
    first = base + 0x2020
    second = base + 0x2040
    object_a = base + 0x7000
    object_b = base + 0x7100
    put = cpu.pointer
    put(wrapper, 0x964870)
    put(wrapper + 4, sentinel)
    put(wrapper + 8, sentinel)
    put(wrapper + 12, sentinel)
    # Source node layout used by the original instructions: next at +0,
    # previous at +4 (not read here), Character* at +8.
    put(sentinel, first)
    put(sentinel + 4, second)
    put(sentinel + 8, 0)
    put(first, second)
    put(first + 4, sentinel)
    put(first + 8, object_a)
    put(second, sentinel)
    put(second + 4, first)
    put(second + 8, object_b)

    vtable_slots = {8: 0x38d4a4, 12: 0x38d4b8, 16: 0x38d4d0,
                    24: 0x38d50c, 28: 0x38d518}
    for offset, expected in vtable_slots.items():
        actual = struct.unpack("<I", cpu.uc.mem_read(0x964870 + offset, 4))[0]
        require(actual == expected, f"CharacterList vtable +{offset:#x}: {actual:#x}")

    sequence = []
    cpu.invoke(0x38d4a4, [wrapper])  # Reset
    for expected in (object_a, object_b):
        require(cpu.invoke(0x38d4b8, [wrapper]) == 0, "original AtEnd rejected live entry")
        got = cpu.invoke(0x38d50c, [wrapper])
        got_char = cpu.invoke(0x38d518, [wrapper])
        require(got == got_char == expected, "original Get/GetChar value mismatch")
        sequence.append(got)
        cpu.invoke(0x38d4d0, [wrapper])  # Next
    require(cpu.invoke(0x38d4b8, [wrapper]) == 1, "original AtEnd missed sentinel")
    require(sequence == [object_a, object_b], "original CharacterList order mismatch")

    # Execute Search(float,float,IObjectList&) through its real source owner
    # getters and stop at the exact point-query call. This verifies argument
    # propagation and the stack-passed CharacterList object without requiring
    # initialized game-global diagnostic services inside the downstream body.
    target_list = base + 0x8000
    owner_object = base + 0x9000
    owner_position = owner_object + 0x160
    look_vector = base + 0xa000
    cpu.pointer(target_list + 0x2c, owner_object)
    cpu.pointer(owner_object + 0x180, 0)  # no target override
    cpu.pointer(owner_object + 0x80, 0)   # normal owner-position path
    cpu.uc.mem_write(owner_position, struct.pack("<fff", 3.0, 4.0, 5.0))
    cpu.pointer(owner_object + 0x174, base + 0xb000)
    radius = struct.unpack("<I", struct.pack("<f", 123.5))[0]
    cone = struct.unpack("<I", struct.pack("<f", 6.2831855))[0]
    observed: dict[str, int | tuple[float, float, float]] = {}

    def stop_at_point_query(uc, address, _size, _user):
        if address == 0x393ae4:  # source transform/look-vector service boundary
            output_pointer = uc.reg_read(UC_ARM_REG_R1)
            uc.mem_write(output_pointer, struct.pack("<fff", 0.0, 1.0, 0.0))
            uc.reg_write(UC_ARM_REG_PC, uc.reg_read(UC_ARM_REG_LR))
        elif address == 0x4a2f34:
            observed["this"] = cpu.reg(0)
            observed["center"] = cpu.reg(1)
            observed["radius"] = cpu.reg(2)
            observed["forward"] = cpu.reg(3)
            sp = cpu.uc.reg_read(cpu.sp)
            observed["cone"] = struct.unpack("<I", uc.mem_read(sp, 4))[0]
            observed["list"] = struct.unpack("<I", uc.mem_read(sp + 4, 4))[0]
            observed["center_xyz"] = struct.unpack("<fff", uc.mem_read(cpu.reg(1), 12))
            observed["forward_xyz"] = struct.unpack("<fff", uc.mem_read(cpu.reg(3), 12))
            uc.emu_stop()

    hook = cpu.uc.hook_add(UC_HOOK_CODE, stop_at_point_query,
                           begin=0x393ae4, end=0x4a2f34)
    entry_sp = cpu.stack + 0xe000
    cpu.uc.reg_write(cpu.sp, entry_sp)
    cpu.uc.reg_write(cpu.lr, cpu.stop)
    cpu.put(0, target_list)
    cpu.put(1, radius)
    cpu.put(2, cone)
    cpu.put(3, wrapper)
    cpu.uc.emu_start(0x4a3428, cpu.stop, count=100000)
    cpu.uc.hook_del(hook)
    require(cpu.uc.reg_read(cpu.pc) == 0x4a2f34,
            "original Search(float,float,IObjectList&) missed point-query call")
    require(observed.get("this") == target_list and observed.get("center") == owner_position,
            "source Search owner/position arguments differ")
    require(observed.get("radius") == radius and observed.get("cone") == cone,
            "source Search radius/cone bits differ")
    require(observed.get("list") == wrapper,
            "source Search did not pass the CharacterList at stack argument 2")
    require(observed.get("center_xyz") == (3.0, 4.0, 5.0) and
            observed.get("forward_xyz") == (0.0, 1.0, 0.0),
            "source Search position/look-vector contents differ")

    source_paths = [MODULE / "character_aggro_target_search.hpp",
                    MODULE / "character_aggro_target_search.cpp",
                    MODULE / "character_aggro_character_list.hpp",
                    MODULE / "character_aggro_character_list.cpp",
                    HERE / "character_aggro_character_list.cpp",
                    Path(__file__).resolve(), MANIFEST,
                    MANIFEST.with_name("NOTES.md")]
    return {
        "status": "passed",
        "original_sha256": EXPECTED_SHA,
        "host_output": host.stdout.strip(),
        "arm32_character_list_virtual_sequence": ["Reset", "AtEnd", "Get", "GetChar", "Next"],
        "arm32_character_list_values": [hex(item) for item in sequence],
        "arm32_search_call_boundary": {key: (list(value) if isinstance(value, tuple) else hex(value))
                                       for key, value in observed.items()},
        "full_search_body_executed": False,
        "scope": "The exact source Search(float,float,IObjectList&) body was executed through its point-query call; the actual source CharacterList virtual methods were executed over two linked entries. The downstream point-query candidate body was byte-pinned but not run in this oracle because it depends on game-global diagnostics/runtime initialization. Host tests execute the new flat-list candidate kernel and compare it with the historical one-room kernel.",
        "compile_command": compile_command,
        "source_sha256": {path.relative_to(REPO).as_posix(): sha(path.read_bytes())
                          for path in source_paths},
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--original-elf", type=Path,
                        default=REPO.parent / "test_strategy" / "libDungeonHunter2.so")
    parser.add_argument("--compiler", default="g++")
    parser.add_argument("--output", type=Path,
                        default=MODULE / "build" / "character-aggro-character-list" /
                        "character-aggro-character-list-host.exe")
    parser.add_argument("--report", type=Path,
                        default=MODULE / "build" / "character-aggro-character-list" /
                        "validation.json")
    args = parser.parse_args()
    result = source_verify(args.original_elf.resolve(), args.compiler, args.output.resolve())
    report = args.report.resolve()
    report.parent.mkdir(parents=True, exist_ok=True)
    report.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print("flat CharacterList host + original ARM caller checks passed")
    print("original ARM methods: Reset/AtEnd/Get/GetChar/Next; Search call boundary: 4a3428 -> 4a2f34")
    print(f"report: {report}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, KeyError, RuntimeError, AssertionError) as exc:
        raise SystemExit(f"ERROR: {exc}") from exc
