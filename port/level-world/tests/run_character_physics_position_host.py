"""Verify the Character vtable +0x64 policy against original ARM instructions."""
from __future__ import annotations

import argparse
import hashlib
import json
import random
import shutil
import struct
import subprocess
import sys
import time
from pathlib import Path

MODULE = Path(__file__).resolve().parents[1]
REPO = MODULE.parents[1]
MANIFEST = MODULE / "reference/character-physics-position/original-functions.json"
ORIGINAL_SHA = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"
CHARACTER_AP = 0x965F38
GAME_OBJECT_AP = 0x964750
SLOT = 0x64

sys.path.insert(0, str(MODULE / "tests"))
from navigation_differential import Cpu as OriginalCpu


def sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--original-elf", type=Path, required=True)
    parser.add_argument("--compiler", default=None)
    parser.add_argument("--cases", type=int, default=4096,
                        help="additional seeded 32-bit Character policy words")
    parser.add_argument("--build-dir", type=Path,
                        default=MODULE / "build/character-physics-position-host")
    parser.add_argument("--report", type=Path,
                        default=MODULE / "build/character-physics-position-host/validation.json")
    parser.add_argument("--reference-output", type=Path,
                        default=MODULE / "build/character-physics-position-host/original.bin")
    args = parser.parse_args()
    for name in ("build_dir", "report", "reference_output"):
        value = getattr(args, name)
        if not value.is_absolute():
            setattr(args, name, (Path.cwd() / value).resolve())
    started = time.monotonic()
    original = args.original_elf.resolve()
    if sha(original.read_bytes()) != ORIGINAL_SHA:
        parser.error("original ELF hash does not match pinned source image")
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    from elftools.elf.elffile import ELFFile

    with original.open("rb") as stream:
        elf = ELFFile(stream)
        symbols = {symbol.name: symbol for symbol in elf.get_section_by_name(".symtab").iter_symbols()}
        segments = [segment for segment in elf.iter_segments() if segment["p_type"] == "PT_LOAD"]

        def image_bytes(address: int, size: int) -> bytes:
            segment = next(s for s in segments if s["p_vaddr"] <= address and
                           address + size <= s["p_vaddr"] + s["p_filesz"])
            return segment.data()[address - segment["p_vaddr"]:
                                  address - segment["p_vaddr"] + size]

        def has_relative_relocation(address: int) -> bool:
            return any(reloc["r_offset"] == address and
                       reloc["r_info_type"] == 23
                       for section in elf.iter_sections()
                       if section["sh_type"] in ("SHT_REL", "SHT_RELA")
                       for reloc in section.iter_relocations())

        for function in manifest["functions"]:
            symbol = symbols[function["original_symbol"]]
            address, size = int(function["elf_address"], 0), function["size"]
            assert (int(symbol["st_value"]), int(symbol["st_size"])) == (address, size)
            assert sha(image_bytes(address, size)) == function["sha256"]

        for entry in manifest["vtable_dispatch"]:
            table = symbols[entry["vtable_symbol"]]
            table_address = int(entry["vtable_address"], 0)
            assert (int(table["st_value"]), int(table["st_size"])) == (
                table_address, entry["vtable_size"])
            address_point = int(entry["address_point"], 0)
            entry_address = int(entry["entry_address"], 0)
            assert address_point == table_address + 8
            assert entry_address == address_point + int(entry["slot_offset"], 0)
            entry_bytes = image_bytes(entry_address, 4)
            assert struct.unpack("<I", entry_bytes)[0] == int(entry["entry_value"], 0)
            assert sha(entry_bytes) == entry["entry_bytes_sha256"]
            assert has_relative_relocation(entry_address)

        constructor = manifest["character_constructor"]
        constructor_symbol = symbols[constructor["original_symbol"]]
        constructor_address = int(constructor["elf_address"], 0)
        assert (int(constructor_symbol["st_value"]),
                int(constructor_symbol["st_size"])) == (
                    constructor_address, constructor["size"])
        assert sha(image_bytes(constructor_address, constructor["size"])) == constructor["sha256"]
        got_entry = int(constructor["vtable_got_entry"], 0)
        got_bytes = image_bytes(got_entry, 4)
        assert sha(got_bytes) == constructor["got_bytes_sha256"]
        assert struct.unpack("<I", got_bytes)[0] == int(constructor["vtable_symbol_value"], 0)
        assert has_relative_relocation(got_entry)
        for name in ("address_point_instruction", "vptr_store_instruction"):
            address = int(constructor[name], 0)
            assert image_bytes(address, 4).hex() == constructor[name + "_bytes"]

    old = OriginalCpu(original, False, manifest)
    owner = old.data + 0x10000
    old.uc.mem_write(owner, bytes(0x600))
    character_target = struct.unpack("<I", old.uc.mem_read(CHARACTER_AP + SLOT, 4))[0]
    base_target = struct.unpack("<I", old.uc.mem_read(GAME_OBJECT_AP + SLOT, 4))[0]
    assert character_target == 0x3A2E44 and base_target == 0x34006C

    rng = random.Random(20261004)
    flags_corpus = list(range(0x10000))
    flags_corpus.extend((0, 1, 2, 3, 0xFFFFFFFD, 0xFFFFFFFE, 0xFFFFFFFF,
                         0x80000000, 0x80000001, 0x80000002))
    flags_corpus.extend(rng.getrandbits(32) for _ in range(args.cases))
    records = bytearray(b"CHP1" + struct.pack("<I", len(flags_corpus)))
    base_checks = 0
    character_enabled = 0
    for index, flags in enumerate(flags_corpus):
        identity = 0x100000000 + (index + 1) * 0x10001
        old.pointer(owner, CHARACTER_AP)
        old.pointer(owner + 0x520, flags)
        raw = old.invoke(character_target, [owner])
        expected = (flags >> 1) & 1
        assert raw == expected, (hex(flags), raw, expected)
        records.extend(struct.pack("<QII", identity, flags, raw))
        character_enabled += raw

        # The base implementation is independent of the Character policy
        # word. Sample it with the exact source GameObject address point.
        if index < 1024:
            old.pointer(owner, GAME_OBJECT_AP)
            old.pointer(owner + 0x520, flags ^ 0xFFFFFFFF)
            base = old.invoke(base_target, [owner])
            assert base == 1, (hex(flags), base)
            base_checks += 1

    assert old.seen
    executed = []
    for function in manifest["functions"]:
        start = int(function["elf_address"], 0)
        addresses = set(range(start, start + function["size"], 4))
        observed = old.seen & addresses
        assert observed == addresses, (function["original_symbol"],
                                       sorted(addresses - observed))
        executed.append({"symbol": function["original_symbol"],
                         "instructions": len(addresses),
                         "executed": len(observed)})

    args.reference_output.parent.mkdir(parents=True, exist_ok=True)
    args.reference_output.write_bytes(records)
    compiler = args.compiler or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        parser.error("pass --compiler or install a C++17 compiler")
    args.build_dir.mkdir(parents=True, exist_ok=True)
    sources = [MODULE / "character_physics_position.cpp",
               MODULE / "move_state.cpp",
               MODULE / "tests/character_physics_position.cpp"]
    objects = [args.build_dir / f"character_physics_position_{i}.o"
               for i in range(len(sources))]
    executable = args.build_dir / "character_physics_position_host.exe"
    commands = []
    for source, obj in zip(sources, objects):
        commands.append([compiler, "-std=c++17", "-O1", "-Wall", "-Wextra",
                         "-Werror", "-pedantic", "-ffunction-sections",
                         "-fdata-sections", "-I", str(MODULE), "-c",
                         str(source), "-o", str(obj)])
    commands.append([compiler, *map(str, objects), "-Wl,--gc-sections",
                     "-o", str(executable)])
    for command in commands:
        subprocess.run(command, cwd=REPO, check=True)
    guards = json.loads(subprocess.check_output([str(executable), "--guards"], text=True))
    native = json.loads(subprocess.check_output(
        [str(executable), str(args.reference_output)], text=True))
    assert native["validation"] == "PASS" and native["comparisons"] == len(flags_corpus)

    tracked = sources + [MODULE / "character_physics_position.hpp",
                         MODULE / "move_state.hpp", Path(__file__), MANIFEST,
                         MANIFEST.with_name("NOTES.md")]
    report = {
        "validation": "PASS",
        "original_sha256": ORIGINAL_SHA,
        "source_arm_comparisons": len(flags_corpus),
        "character_true_results": character_enabled,
        "base_game_object_checks": base_checks,
        "instruction_coverage": executed,
        "vtable_dispatch": manifest["vtable_dispatch"],
        "constructor_address_point": {
            "symbol": constructor["original_symbol"],
            "got_entry": constructor["vtable_got_entry"],
            "address_point": manifest["vtable_dispatch"][1]["address_point"],
            "stored_at_object_offset": "0x0",
        },
        "source_properties": manifest["source_properties"],
        "native_replay": native,
        "native_guards": guards,
        "reference_sha256": sha(records),
        "source_sha256": {path.relative_to(REPO).as_posix(): sha(path.read_bytes())
                          for path in tracked},
        "compiler_commands": commands,
        "scope": "Complete Character::IsUpdatingPositionFromPhysics getter adapter and its exact source vtable owner. Source ARM methods, vtable entries and constructor address-point bytes are verified; GameObject base getter is separately executed. This does not bind Ghost controller state or prove every Character policy producer.",
        "elapsed_seconds": round(time.monotonic() - started, 2),
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"validation": "PASS", "source_arm_comparisons": len(flags_corpus),
                      "base_game_object_checks": base_checks,
                      "mismatches": 0}))


if __name__ == "__main__":
    main()
