"""Original ARM GameObject::Stop versus the portable service-backed caller."""
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

from unicorn import UC_HOOK_CODE

MODULE = Path(__file__).resolve().parents[1]
REPO = MODULE.parents[1]
ROOT = MODULE
MANIFEST = MODULE / "reference/game-object-stop/original-functions.json"
ORIGINAL_SHA = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"
OFFSETS = (4, 8, 0x38, 0x40, 0x44, 0x48, 0x4C, 0x50, 0x54, 0x8C)

sys.path.insert(0, str(MODULE / "tests"))
from navigation_differential import Cpu as BaseCpu


def words(*values: int) -> bytes:
    return struct.pack("<" + "I" * len(values), *(value & 0xFFFFFFFF for value in values))


def float_word(value: float) -> int:
    return struct.unpack("<I", struct.pack("<f", value))[0]


class OriginalCpu(BaseCpu):
    pass


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--original-elf", type=Path, required=True)
    parser.add_argument("--compiler", default=None)
    parser.add_argument("--cases", type=int, default=192)
    parser.add_argument("--build-dir", type=Path, default=MODULE / "build/game-object-stop-host")
    parser.add_argument("--report", type=Path, default=MODULE / "build/game-object-stop-host/validation.json")
    parser.add_argument("--reference-output", type=Path,
                        default=MODULE / "build/game-object-stop-host/original.bin")
    args = parser.parse_args()
    for name in ("build_dir", "report", "reference_output"):
        value = getattr(args, name)
        if not value.is_absolute():
            setattr(args, name, (Path.cwd() / value).resolve())
    started = time.monotonic()
    compiler = args.compiler or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        parser.error("pass --compiler or install a C++17 compiler")
    original = args.original_elf.resolve()
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    assert hashlib.sha256(original.read_bytes()).hexdigest() == ORIGINAL_SHA
    from elftools.elf.elffile import ELFFile
    with original.open("rb") as stream:
        elf = ELFFile(stream)
        symbols = {symbol.name: symbol for symbol in elf.get_section_by_name(".symtab").iter_symbols()}
        segments = [segment for segment in elf.iter_segments() if segment["p_type"] == "PT_LOAD"]
        verified_functions = manifest["functions"]
        verified_boundaries = [item for item in manifest.get("external_boundaries", [])
                               if item.get("size") and item.get("original_symbol")]
        for item in verified_functions + verified_boundaries:
            address, size = int(item["elf_address"], 0), item["size"]
            symbol = symbols[item["original_symbol"]]
            assert (int(symbol["st_value"]), int(symbol["st_size"])) == (address, size)
            segment = next(s for s in segments if s["p_vaddr"] <= address and
                           address + size <= s["p_vaddr"] + s["p_filesz"])
            raw = segment.data()[address - segment["p_vaddr"]:
                                 address - segment["p_vaddr"] + size]
            assert hashlib.sha256(raw).hexdigest() == item["sha256"]

        def image_bytes(address: int, size: int) -> bytes:
            segment = next(s for s in segments if s["p_vaddr"] <= address and
                           address + size <= s["p_vaddr"] + s["p_filesz"])
            return segment.data()[address - segment["p_vaddr"]:
                                  address - segment["p_vaddr"] + size]

        def relative_relocation(address: int) -> bool:
            return any(reloc["r_offset"] == address and reloc["r_info_type"] == 23
                       for section in elf.iter_sections()
                       if section["sh_type"] in ("SHT_REL", "SHT_RELA")
                       for reloc in section.iter_relocations())

        for row in manifest["virtual_dispatch"]:
            table = symbols[row["vtable_symbol"]]
            assert (int(table["st_value"]), int(table["st_size"])) == (
                int(row["vtable_address"], 0), row["vtable_size"])
            assert int(row["address_point"], 0) == int(row["vtable_address"], 0) + 8
            assert int(row["entry_address"], 0) == (
                int(row["address_point"], 0) + int(row["slot_offset"], 0))
            entry = image_bytes(int(row["entry_address"], 0), 4)
            assert struct.unpack("<I", entry)[0] == int(row["entry_value"], 0)
            assert hashlib.sha256(entry).hexdigest() == row["entry_sha256"]
            assert relative_relocation(int(row["entry_address"], 0))
            method = symbols[row["method_symbol"]]
            assert (int(method["st_value"]), int(method["st_size"])) == (
                int(row["entry_value"], 0), row["method_size"])
            assert hashlib.sha256(image_bytes(int(row["entry_value"], 0),
                                              row["method_size"])).hexdigest() == row["method_sha256"]

        constructor = manifest["character_constructor"]
        ctor = symbols[constructor["symbol"]]
        assert (int(ctor["st_value"]), int(ctor["st_size"])) == (
            int(constructor["address"], 0), constructor["size"])
        assert hashlib.sha256(image_bytes(int(constructor["address"], 0),
                                          constructor["size"])).hexdigest() == constructor["sha256"]
        assert struct.unpack("<I", image_bytes(int(constructor["address_point_instruction"], 0), 4))[0] == 0xE28E0008
        assert struct.unpack("<I", image_bytes(int(constructor["vptr_store_instruction"], 0), 4))[0] == 0xE8840003
        got_slot = int(constructor["vtable_got_entry"], 0)
        assert struct.unpack("<I", image_bytes(got_slot, 4))[0] == int(constructor["vtable_symbol_value"], 0)
        assert relative_relocation(got_slot)
    old = OriginalCpu(original, False, manifest)
    body, physical, game = [old.data + at for at in (0x1000, 0x2000, 0x3000)]
    old.uc.mem_write(physical, bytes(0x40))
    old.pointer(physical + 0x14, body)
    old.uc.mem_write(game, bytes(0x600))
    got = (0x393910 + struct.unpack("<I", old.uc.mem_read(0x3939E4, 4))[0]) & 0xFFFFFFFF
    heading_slot = got + struct.unpack("<I", old.uc.mem_read(0x3939EC, 4))[0]
    origin = struct.unpack("<I", old.uc.mem_read(heading_slot, 4))[0]
    source_origin = bytes(old.uc.mem_read(origin, 12))
    source_origin_words = struct.unpack("<3I", source_origin)
    assert source_origin_words == (0, 0, 0), source_origin.hex()

    # These are logical identities used by the 64-bit adapter. They are
    # intentionally unrelated to the 32-bit addresses in the original VM.
    OBJECT_ID, PATH_ID, PHYSICAL_ID = 0x100000001, 0x200000003, 0x300000005
    rng = random.Random(20261024)
    float_boundaries = [
        0x00000000, 0x80000000, 0x00000001, 0x80000001, 0x00800000,
        0x3F800000, 0xBF800000, 0x7F7FFFFF, 0xFF7FFFFF,
        0x7F800000, 0xFF800000, 0x7FC01234, 0xFFC01234,
    ]
    records: list[bytes] = []
    trace_totals = [0] * 6
    virtual_calls = character_physical_resets = transform_calls = 0

    def source_state(pos: tuple[int, ...], dest: tuple[int, ...], heading: tuple[int, ...],
                     present: int) -> bytes:
        # State layout is asserted by C++; explicit tail padding is zeroed.
        return struct.pack("<QQQ9I2B6x4x", OBJECT_ID, PATH_ID,
                           PHYSICAL_ID if present else 0,
                           *pos, *dest, *heading, 1, 1)

    def body_bytes(flag: int, values: tuple[int, ...]) -> bytes:
        return struct.pack("<I11I", flag, *values)

    def physical_initial(old_float_words: tuple[int, ...]) -> bytes:
        old.uc.mem_write(body, bytes([0xA5]) * 0xA0)
        old.uc.mem_write(body, struct.pack("<H", 0x5A5A))
        for index, offset in enumerate(OFFSETS):
            old.uc.mem_write(body + offset, words(old_float_words[index]))
        old.uc.mem_write(physical + 12, words(old_float_words[10]))
        return body_bytes(0x5A5A, old_float_words)

    def run_case(case_index: int, pos: tuple[int, ...], dest: tuple[int, ...],
                 heading: tuple[int, ...], physical_values: tuple[int, ...],
                 present: int, character_flags: int) -> None:
        nonlocal virtual_calls, character_physical_resets, transform_calls
        updates_position_from_physics = (character_flags >> 1) & 1
        initial_state = source_state(pos, dest, heading, present)
        initial_body = physical_initial(physical_values)
        old.pointer(game, 0x965F38)  # Original Character constructor address point.
        old.pointer(game + 0x520, character_flags)
        old.pointer(game + 0x2DC, physical if present else 0)
        old.uc.mem_write(game + 0x160, words(*pos))
        old.uc.mem_write(game + 0x1A8, words(*dest))
        old.uc.mem_write(game + 0x1B8, words(*heading))
        old.uc.mem_write(game + 0x1B4, b"\x01\x01")

        events: list[int] = []
        xforms: list[bytes] = []

        def observe(uc, address, size, user):
            if address == 0x3A2E44:
                old.virtual_calls += 1
                events.append(1)  # Character::IsUpdatingPositionFromPhysics.
                return
            if address == 0x52AAE4:
                assert old.reg(1) == game + 0x1C8
                events.append(0)  # DropPath boundary.
                old.put(0, 0)
                uc.reg_write(old.pc, uc.reg_read(old.lr))
                return
            if address in (0x46E918, 0x46E978, 0x46EA80):
                assert old.reg(0) == physical
                events.append({0x46E918: 2, 0x46E978: 3, 0x46EA80: 4}[address])
            elif address == 0x7E164C:
                assert old.reg(0) == body
                xform = (bytes(uc.mem_read(old.reg(1), 8)) + words(old.reg(2), 1))
                xforms.append(xform)
                events.append(5)
                # The Stop oracle treats Box2D SetXForm as the exact transform
                # argument observer, then applies its position fields before
                # the source's direct final body writes.
                if present and updates_position_from_physics:
                    uc.mem_write(body + 4, xform[:8])
                old.put(0, 1)
                uc.reg_write(old.pc, uc.reg_read(old.lr))

        hook = old.uc.hook_add(UC_HOOK_CODE, observe)
        old.virtual_calls = 0
        old.invoke(0x3938F8, [game], budget=100000)
        old.uc.hook_del(hook)
        virtual_calls += old.virtual_calls
        character_physical_resets += bool(present and updates_position_from_physics)
        transform_calls += len(xforms)
        assert old.uc.reg_read(old.pc) == old.stop

        expected_state = struct.pack("<QQQ9I2B6x4x", OBJECT_ID, PATH_ID,
            PHYSICAL_ID if present else 0, *pos, *pos, *source_origin_words, 0, 0)
        after_body_words = tuple(struct.unpack("<I", old.uc.mem_read(body + offset, 4))[0]
                                  for offset in OFFSETS) + (
            struct.unpack("<I", old.uc.mem_read(physical + 12, 4))[0],)
        expected_body = body_bytes(struct.unpack("<H", old.uc.mem_read(body, 2))[0], after_body_words)
        expected_xform = xforms[0] if xforms else bytes(16)
        for event in events:
            trace_totals[event] += 1

        # PathFixture is a native service-fixture snapshot, not a claim that
        # the original PFWorld allocator ran here. DropPath call/order is
        # executed by the original caller and its full body is audited in the
        # navigation-path corpus.
        p_position = struct.unpack("<3f", struct.pack("<3I", *pos))
        path_in = struct.pack("<II6f", 2, 1, *p_position, 1000., 2000., 3000.)
        if case_index % 5 == 0:
            path_in = struct.pack("<II6f", 0, 0, *p_position, 1000., 2000., 3000.)
            path_out = path_in
        else:
            path_out = struct.pack("<II6f", 0, 0, *p_position, *p_position)
        args = [initial_state, expected_state, initial_body, expected_body,
                path_in, path_out, words(len(events), *events), expected_xform]
        record = words(present, updates_position_from_physics) + b"".join(args)
        records.append(record)
        assert len(xforms) == int(bool(present and updates_position_from_physics)), (
            case_index, len(xforms), present, character_flags)

    # All physical branches and representative IEEE values, plus seeded raw
    # body/position snapshots. Float payload equality is exact for copies and
    # classified for NaNs by the host consumer.
    for i, word in enumerate(float_boundaries):
        pos = (word, float_boundaries[(i + 3) % len(float_boundaries)],
               float_boundaries[(i + 5) % len(float_boundaries)])
        dest = tuple(float_boundaries[(i + j + 1) % len(float_boundaries)] for j in range(3))
        heading = tuple(float_boundaries[(i + j + 6) % len(float_boundaries)] for j in range(3))
        body_words = tuple(float_boundaries[(i + j) % len(float_boundaries)] for j in range(11))
        for present in (0, 1):
            for character_flags in (0, 2):
                run_case(len(records), pos, dest, heading, body_words, present, character_flags)
    for n in range(args.cases):
        pick = lambda: rng.choice(float_boundaries) if n % 2 else rng.getrandbits(32)
        pos = tuple(pick() for _ in range(3))
        dest = tuple(pick() for _ in range(3))
        heading = tuple(pick() for _ in range(3))
        body_words = tuple(pick() for _ in range(11))
        run_case(len(records), pos, dest, heading, body_words, n % 2, rng.getrandbits(32))
    for character_flags in (0x00000001, 0xFFFFFFFF):
        pos = tuple(float_word(value) for value in (117., -300., 19.))
        dest = tuple(float_word(value) for value in (-1., 2., 3.))
        heading = tuple(float_word(value) for value in (0.25, -0.5, 1.))
        body_words = tuple(float_word(value) for value in (1., 2., 0.5, 4., 5., 6., 7., 8., 9., 10., 11.))
        run_case(len(records), pos, dest, heading, body_words, 1, character_flags)

    coverage = []
    stop_row = next(row for row in manifest["functions"] if row["elf_address"] == "0x3938f8")
    for row in manifest["functions"]:
        start = int(row["elf_address"], 0)
        addresses = set(range(start, start + row.get("executable_bytes", row["size"]), 4))
        executed = old.seen & addresses
        if row is stop_row:
            assert executed == addresses, ("GameObject::Stop instruction coverage",
                                           sorted(addresses - executed))
        if row["original_symbol"] == "_ZNK9Character29IsUpdatingPositionFromPhysicsEv":
            assert executed == addresses, ("Character override instruction coverage",
                                           sorted(addresses - executed))
        coverage.append({"symbol": row["original_symbol"], "instructions": len(addresses),
                         "executed": len(executed)})

    reference = b"GOS1" + words(len(records)) + b"".join(records)
    args.reference_output.parent.mkdir(parents=True, exist_ok=True)
    args.reference_output.write_bytes(reference)
    sources = [MODULE / "game_object_stop.cpp", MODULE / "physical_controls.cpp",
               MODULE / "tests/game_object_stop.cpp", MODULE / "navigation_path.cpp"]
    args.build_dir.mkdir(parents=True, exist_ok=True)
    executable = args.build_dir / "game_object_stop_host.exe"
    objects = [args.build_dir / f"game_object_stop_{i}.o" for i in range(len(sources))]
    commands = []
    for index, (source, obj) in enumerate(zip(sources, objects)):
        warning_policy = ["-Wno-misleading-indentation"] if index in (1, 3) else []
        commands.append([compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror",
                         "-pedantic", "-ffunction-sections", "-fdata-sections",
                         *warning_policy, "-I", str(MODULE), "-c",
                         str(source), "-o", str(obj)])
    commands.append([compiler, *map(str, objects), "-Wl,--gc-sections", "-o", str(executable)])
    for command in commands:
        subprocess.run(command, cwd=REPO, check=True)
    guards = json.loads(subprocess.check_output([str(executable), "--guards"], text=True))
    replay = json.loads(subprocess.check_output([str(executable), str(args.reference_output)], text=True))
    assert replay["validation"] == "PASS" and replay["original_arm_cases"] == len(records)
    args.report.parent.mkdir(parents=True, exist_ok=True)
    tracked = sources + [MODULE / "game_object_stop.hpp", MODULE / "navigation_path.hpp", Path(__file__), MANIFEST,
                         MANIFEST.with_name("NOTES.md")]
    report = {
        "validation": "PASS",
        "original_sha256": ORIGINAL_SHA,
        "original_function": "GameObject::Stop at 0x3938f8 (248 bytes)",
        "original_arm_cases": len(records),
        "original_instruction_coverage": coverage,
        "verified_external_boundaries": [row["original_symbol"] for row in verified_boundaries],
        "virtual_dispatch": manifest["virtual_dispatch"],
        "character_constructor": manifest["character_constructor"],
        "source_observed_virtual_queries": virtual_calls,
        "source_character_physical_reset_cases": character_physical_resets,
        "source_set_xform_boundaries": transform_calls,
        "source_ordered_event_counts": trace_totals,
        "source_origin_vector": {"got_slot": hex(heading_slot), "pointer": hex(origin),
                                 "words": list(source_origin_words)},
        "native_guards": guards,
        "native_replay": replay,
        "original_drop_path_body": "not in this Stop corpus; required provider boundary; independently audited by navigation-path",
        "box2d_setxform_backend": "boundary observer applies exact returned XY to body snapshot; solver/proxies/contacts not asserted",
        "source_stop_body_and_physical_setters_execute": True,
        "mismatches": 0,
        "scope": "Original GameObject::Stop and the Character::IsUpdatingPositionFromPhysics override execute through pinned Character address point 0x965f38. Base GameObject override and Character constructor/address-point ownership are byte-pinned. DropPath is intercepted at the original caller boundary. The native callback must own route release; the direct physical+0x14 projection is resolved after the source setter sequence. No live native Ghost wiring is claimed.",
        "reference_sha256": hashlib.sha256(reference).hexdigest(),
        "source_sha256": {p.relative_to(REPO).as_posix(): hashlib.sha256(p.read_bytes()).hexdigest()
                          for p in tracked},
        "compiler_commands": commands,
        "elapsed_seconds": round(time.monotonic() - started, 2),
    }
    args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"validation": "PASS", "original_arm_cases": len(records),
                      "character_physical_reset_cases": character_physical_resets,
                      "set_xform_boundaries": transform_calls,
                      "mismatches": 0}))


if __name__ == "__main__":
    main()
