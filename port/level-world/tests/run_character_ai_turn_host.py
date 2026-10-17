#!/usr/bin/env python3
"""Run host and original-ARM differential checks for CharAI::IsMyTurn's bounded decision."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
EXPECTED_ORIGINAL_SHA = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"

SYMBOLS = {
    "is_my_turn": ("_ZN6CharAI8IsMyTurnEPKS_", 0x3CC484, 288),
    "deque_subtract": (
        "_ZNKSt4priv20_Deque_iterator_baseIP6CharAIE11_M_subtractERKS3_",
        0x3CB49C, 68),
    "is_follower": ("_ZNK9Character10IsFollowerEv", 0x3A307C, 24),
    "is_faerie": ("_ZNK9Character8IsFaerieEv", 0x3A3094, 24),
    "is_player": ("_ZNK9Character8IsPlayerEv", 0x3A49F0, 76),
    "update_queue": ("_ZN6CharAI13s_updateQueueE", 0x9A2B48, 40),
    "update_timer": ("_ZN6CharAI13s_updateTimerE", 0x999760, 4),
    "character_vtable": ("_ZTV9Character", 0x965F30, 812),
}

AI = 0x10010000
OWNER_A = 0x10014000
OWNER_B = 0x10014100
OWNER_C = 0x10014200
MEMORY_BASE = 0x10000000
MEMORY_SIZE = 0x00040000
QUEUE_MAP = 0x10021000
QUEUE_BLOCK_BASE = 0x10022000
STOP_ADDRESS = 0x1003F100
STACK_ADDRESS = 0x1003E000

def sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha_file(path: Path) -> str:
    return sha(path.read_bytes())


def source_bytes(elf, raw: bytes, address: int, size: int) -> bytes:
    for segment in elf.iter_segments():
        if segment["p_type"] != "PT_LOAD":
            continue
        start = int(segment["p_vaddr"])
        file_size = int(segment["p_filesz"])
        memory_size = int(segment["p_memsz"])
        if start <= address and address + size <= start + memory_size:
            # PT_LOAD's tail beyond p_filesz is zero-filled storage, including
            # the original s_updateQueue object in .bss.
            available_file_bytes = max(0, min(size, start + file_size - address))
            if available_file_bytes:
                offset = int(segment["p_offset"]) + address - start
                return raw[offset:offset + available_file_bytes] + \
                    b"\0" * (size - available_file_bytes)
            return b"\0" * size
    raise ValueError(f"ELF file-backed range missing at {address:#x}/{size:#x}")


def inspect_source(original_path: Path):
    from elftools.elf.elffile import ELFFile

    raw = original_path.read_bytes()
    digest = sha(raw)
    if digest != EXPECTED_ORIGINAL_SHA:
        raise ValueError(f"original ELF SHA mismatch: {digest}")
    stream = original_path.open("rb")
    elf = ELFFile(stream)
    if elf.elfclass != 32 or elf.little_endian is not True or elf["e_machine"] != "EM_ARM":
        raise ValueError("expected the original little-endian ELF32 ARM shared library")
    symtab = elf.get_section_by_name(".symtab")
    if symtab is None:
        raise ValueError("original ELF has no .symtab for source identity checks")
    by_name = {}
    for symbol in symtab.iter_symbols():
        if symbol.name in {spec[0] for spec in SYMBOLS.values()}:
            if symbol.name in by_name:
                raise ValueError(f"duplicate exact symbol identity: {symbol.name}")
            by_name[symbol.name] = symbol

    evidence = {}
    resolved = {}
    for label, (name, address, size) in SYMBOLS.items():
        symbol = by_name.get(name)
        if symbol is None:
            raise ValueError(f"missing exact ELF symbol {name}")
        actual_address = int(symbol["st_value"])
        actual_size = int(symbol["st_size"])
        if (actual_address, actual_size) != (address, size):
            raise ValueError(
                f"{name} identity changed: {actual_address:#x}/{actual_size}, "
                f"expected {address:#x}/{size}")
        kind = symbol["st_info"]["type"]
        data = source_bytes(elf, raw, address, size)
        evidence[label] = {
            "mangled_symbol": name,
            "address": f"0x{address:x}",
            "size": size,
            "elf_type": kind,
            "sha256": sha(data),
        }
        resolved[label] = {"address": address, "size": size, "name": name}

    if evidence["update_queue"]["elf_type"] != "STT_OBJECT" or \
       evidence["update_timer"]["elf_type"] != "STT_OBJECT":
        raise ValueError("turn globals are not ELF objects")
    vtable_address = resolved["character_vtable"]["address"]
    address_point = vtable_address + 8  # Itanium ABI offset-to-top and typeinfo prefix.
    is_player_slot_address = address_point + 0x28
    is_player_slot_target = struct.unpack(
        "<I", source_bytes(elf, raw, is_player_slot_address, 4))[0]
    if is_player_slot_target != resolved["is_player"]["address"]:
        raise ValueError(
            f"Character vptr+0x28 resolves to {is_player_slot_target:#x}, "
            f"not the exact IsPlayer ELF symbol")
    relocations = []
    for section in elf.iter_sections():
        if section["sh_type"] not in ("SHT_REL", "SHT_RELA"):
            continue
        for relocation in section.iter_relocations():
            if int(relocation["r_offset"]) == is_player_slot_address:
                relocations.append({
                    "section": section.name,
                    "type_id": int(relocation["r_info_type"]),
                    "symbol_index": int(relocation["r_info_sym"]),
                })
    if relocations != [{"section": ".rel.dyn", "type_id": 23, "symbol_index": 0}]:
        raise ValueError(f"Character IsPlayer vtable slot relocation changed: {relocations}")
    evidence["character_vtable_slot"] = {
        "vtable_symbol": SYMBOLS["character_vtable"][0],
        "vtable_address": f"0x{vtable_address:x}",
        "itanium_address_point": f"0x{address_point:x}",
        "slot_offset_from_vptr": "0x28",
        "slot_address_in_elf": f"0x{is_player_slot_address:x}",
        "target_symbol": SYMBOLS["is_player"][0],
        "target_address": f"0x{is_player_slot_target:x}",
        "relocation": relocations[0],
    }
    return raw, elf, resolved, evidence


def fixtures():
    # Every case uses a live nonempty source deque and runs its real
    # _Deque_iterator_base<CharAI*>::_M_subtract before observing IsMyTurn.
    return [
        dict(name="due_front_single", queue_length=1, head=0, timer=0,
             front_matches=True, follower=0, faerie=0, player=0),
        dict(name="due_front_negative_timer_multiblock", queue_length=33, head=0,
             timer=-1, front_matches=True, follower=0, faerie=0, player=0),
        dict(name="positive_timer_skips_equal_front", queue_length=1, head=0,
             timer=1, front_matches=True, follower=0, faerie=0, player=7),
        dict(name="due_other_head_follower_noncanonical_true", queue_length=65,
             head=0, timer=0, front_matches=False, follower=2, faerie=0,
             player=0),
        dict(name="due_other_head_faerie_noncanonical_true", queue_length=8,
             head=0, timer=-2147483648, front_matches=False, follower=0,
             faerie=0x80000000, player=0),
        dict(name="due_other_head_player_false", queue_length=2, head=0,
             timer=0, front_matches=False, follower=0, faerie=0, player=0),
        dict(name="due_other_head_player_raw_value", queue_length=67, head=0,
             timer=-7, front_matches=False, follower=0, faerie=0,
             player=0x13579BDF),
        dict(name="wrapped_deque_head_offset", queue_length=9, head=26,
             timer=0, front_matches=True, follower=0, faerie=0, player=0),
        dict(name="head_offset_positive_timer_faerie", queue_length=37, head=29,
             timer=33, front_matches=False, follower=0, faerie=3, player=9),
        dict(name="owner_reloaded_between_all_fallbacks", queue_length=3, head=0,
             timer=1, front_matches=False, follower=0, faerie=0, player=5,
             after_follower=OWNER_B, after_faerie=OWNER_C),
        dict(name="owner_change_on_follower_short_circuits", queue_length=1,
             head=0, timer=1, front_matches=False, follower=0xFFFFFFFF,
             faerie=1, player=1, after_follower=OWNER_B),
        dict(name="positive_timer_not_front_follower", queue_length=8,
             head=3, timer=2147483647, front_matches=False, follower=9,
             faerie=1, player=1),
    ]


def load_arm_image(uc, elf, raw):
    segments = []
    for segment in elf.iter_segments():
        if segment["p_type"] != "PT_LOAD":
            continue
        segments.append((int(segment["p_vaddr"]), int(segment["p_filesz"]),
                         int(segment["p_memsz"]), int(segment["p_offset"])))
    maximum = max(base + memory for base, _, memory, _ in segments)
    image_size = (maximum + 0xFFF) & ~0xFFF
    uc.mem_map(0, image_size)
    for base, file_size, _, file_offset in segments:
        uc.mem_write(base, raw[file_offset:file_offset + file_size])
    uc.mem_map(MEMORY_BASE, MEMORY_SIZE)
    return segments


def arm_oracle(original_path: Path, cases):
    from unicorn import Uc, UC_ARCH_ARM, UC_HOOK_CODE, UC_HOOK_MEM_READ, UC_MODE_ARM
    from unicorn.arm_const import (
        UC_ARM_REG_LR, UC_ARM_REG_PC, UC_ARM_REG_R0, UC_ARM_REG_R1,
        UC_ARM_REG_SP,
    )

    raw, elf, resolved, evidence = inspect_source(original_path)
    def put(machine, address, value):
        machine.mem_write(address, struct.pack("<I", int(value) & 0xFFFFFFFF))

    def get(machine, address):
        return struct.unpack("<I", machine.mem_read(address, 4))[0]

    outputs = []
    source_execution = {
        "is_my_turn_calls": 0,
        "deque_subtract_calls": 0,
        "deque_lengths": [],
        "head_value_reads": 0,
        "character_query_calls": 0,
        "query_targets": [],
    }
    for case in cases:
        machine = Uc(UC_ARCH_ARM, UC_MODE_ARM)
        load_arm_image(machine, elf, raw)
        ai = AI
        owner = OWNER_A
        character_address_point = resolved["character_vtable"]["address"] + 8
        put(machine, ai, 0xAAAAAAAA)
        put(machine, ai + 4, owner)
        for char_address in (OWNER_A, OWNER_B, OWNER_C):
            put(machine, char_address, character_address_point)

        queue = resolved["update_queue"]["address"]
        machine.mem_write(queue, b"\0" * resolved["update_queue"]["size"])
        put(machine, resolved["update_timer"]["address"], case["timer"])
        head_absolute = case["head"]
        end_absolute = head_absolute + case["queue_length"]
        end_block = end_absolute // 32
        # The source libc++ deque iterator subtract is specialized for 32
        # CharAI* values per block (its shift-left-by-five scales node hops).
        block_count = max(1, end_block + 1)
        block_addresses = [QUEUE_BLOCK_BASE + 0x1000 * i for i in range(block_count)]
        for i, block_address in enumerate(block_addresses):
            put(machine, QUEUE_MAP + i * 4, block_address)
            for slot in range(32):
                absolute = i * 32 + slot
                ordinal = absolute - head_absolute
                value = AI + 0x100 + (ordinal % 0x100) * 0x100
                if ordinal == 0 and case["front_matches"]:
                    value = ai
                put(machine, block_address + slot * 4, value)

        begin_block = block_addresses[0]
        end_block_address = block_addresses[end_block]
        begin = (begin_block + head_absolute % 32 * 4, begin_block,
                 begin_block + 128, QUEUE_MAP)
        end = (end_block_address + end_absolute % 32 * 4,
               end_block_address, end_block_address + 128,
               QUEUE_MAP + end_block * 4)
        machine.mem_write(queue, struct.pack("<8I", *(begin + end)))
        head_slot = begin[0]
        observed = {"calls": [], "deque_lengths": [], "head_reads": 0,
                    "subtract_iterators": None}
        query_values = [case["follower"], case["faerie"], case["player"]]
        replacement_values = {
            0: case.get("after_follower", 0),
            1: case.get("after_faerie", 0),
        }
        symbol_to_id = {
            resolved[label]["address"]: query_id
            for label, query_id in (("is_follower", 0), ("is_faerie", 1),
                                    ("is_player", 2))
        }
        subtract_return = resolved["is_my_turn"]["address"] + 0x34

        def code_hook(cpu, address, _size, _user):
            if address == resolved["is_my_turn"]["address"]:
                source_execution["is_my_turn_calls"] += 1
            if address == resolved["deque_subtract"]["address"]:
                source_execution["deque_subtract_calls"] += 1
                end_iterator = cpu.reg_read(UC_ARM_REG_R0)
                begin_iterator = cpu.reg_read(UC_ARM_REG_R1)
                observed["subtract_iterators"] = {
                    "end": [get(cpu, end_iterator + offset) for offset in (0, 4, 8, 12)],
                    "begin": [get(cpu, begin_iterator + offset) for offset in (0, 4, 8, 12)],
                }
            if address == subtract_return:
                length = cpu.reg_read(UC_ARM_REG_R0)
                observed["deque_lengths"].append(length)
                source_execution["deque_lengths"].append(length)
            if address == STOP_ADDRESS:
                cpu.emu_stop()
                return
            query_id = symbol_to_id.get(address)
            if query_id is None:
                return
            owner_pointer = cpu.reg_read(UC_ARM_REG_R0)
            observed["calls"].append([query_id, owner_pointer])
            source_execution["character_query_calls"] += 1
            source_execution["query_targets"].append(f"0x{address:x}")
            cpu.reg_write(UC_ARM_REG_R0, query_values[query_id] & 0xFFFFFFFF)
            if query_id in replacement_values and replacement_values[query_id]:
                put(cpu, ai + 4, replacement_values[query_id])
            cpu.reg_write(UC_ARM_REG_PC, cpu.reg_read(UC_ARM_REG_LR))

        def memory_hook(_cpu, _access, address, _size, _value, _user):
            if address == head_slot:
                observed["head_reads"] += 1
                source_execution["head_value_reads"] += 1

        machine.hook_add(UC_HOOK_CODE, code_hook)
        machine.hook_add(UC_HOOK_MEM_READ, memory_hook)
        machine.reg_write(UC_ARM_REG_R0, ai)
        machine.reg_write(UC_ARM_REG_SP, STACK_ADDRESS)
        machine.reg_write(UC_ARM_REG_LR, STOP_ADDRESS)
        machine.emu_start(resolved["is_my_turn"]["address"], STOP_ADDRESS,
                          count=100000)
        if len(observed["deque_lengths"]) != 1:
            raise AssertionError(f"{case['name']}: did not execute exactly one real deque subtract")
        if observed["deque_lengths"][0] != case["queue_length"]:
            raise AssertionError(
                f"{case['name']}: real _M_subtract returned "
                f"{observed['deque_lengths'][0]}, expected {case['queue_length']}; "
                f"iters={observed['subtract_iterators']}")
        result_value = machine.reg_read(UC_ARM_REG_R0)
        final_owner = get(machine, ai + 4)
        outputs.append({
            "name": case["name"],
            "value": result_value,
            "calls": observed["calls"],
            "owner_after": final_owner,
            "queue_front_read": int(observed["head_reads"] != 0),
            "deque_length": observed["deque_lengths"][0],
        })
    return outputs, evidence, source_execution


def host_run(executable: Path, case):
    argv = [
        str(AI), str(OWNER_A), str(AI if case["front_matches"] else AI + 0x100),
        str(case["queue_length"]), str(case["timer"]), str(case["follower"]),
        str(case["faerie"]), str(case["player"]),
        str(case.get("after_follower", 0)), str(case.get("after_faerie", 0)),
        "-1", "0",
    ]
    completed = subprocess.run([str(executable), *argv], check=True, text=True,
                               capture_output=True)
    return json.loads(completed.stdout)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--original-elf", type=Path, required=True)
    parser.add_argument("--compiler", help="C++17 host compiler; defaults to CXX or g++")
    parser.add_argument("--output", type=Path,
                        default=MODULE / "build/character-ai-turn-host/character_ai_turn.exe")
    parser.add_argument("--report", type=Path,
                        default=MODULE / "build/character-ai-turn-host/validation.json")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++")
    if not compiler:
        parser.error("pass --compiler or install/select a C++17 g++ compiler")
    original = args.original_elf.resolve()
    if not original.is_file():
        parser.error(f"original ELF does not exist: {original}")
    raw, elf, _resolved, elf_evidence = inspect_source(original)
    cases = fixtures()
    oracle_results, oracle_evidence, source_execution = arm_oracle(original, cases)

    output = args.output.resolve()
    report_path = args.report.resolve()
    output.parent.mkdir(parents=True, exist_ok=True)
    report_path.parent.mkdir(parents=True, exist_ok=True)
    test_cpp = MODULE / "tests/character_ai_turn.cpp"
    production_cpp = MODULE / "character_ai_turn.cpp"
    production_header = MODULE / "character_ai_turn.hpp"
    command = [
        str(compiler), "-std=c++17", "-O2", "-Wall", "-Wextra", "-Werror",
        str(production_cpp), str(test_cpp), "-o", str(output),
    ]
    subprocess.run(command, cwd=ROOT, check=True, text=True, capture_output=True)
    guard_report = json.loads(subprocess.check_output([str(output)], cwd=ROOT, text=True))
    if guard_report.get("validation") != "PASS" or guard_report.get("mismatches") != 0:
        raise AssertionError(f"host API/error guard checks failed: {guard_report}")

    results = []
    for case, oracle in zip(cases, oracle_results):
        if oracle["name"] != case["name"]:
            raise AssertionError("fixture/oracle ordering mismatch")
        actual = host_run(output, case)
        expected = {
            "status": 0,
            "value": oracle["value"],
            "follower_queries": sum(c[0] == 0 for c in oracle["calls"]),
            "faerie_queries": sum(c[0] == 1 for c in oracle["calls"]),
            "player_queries": sum(c[0] == 2 for c in oracle["calls"]),
            "queue_front_read": oracle["queue_front_read"],
            "owner_after": oracle["owner_after"],
            "calls": oracle["calls"],
        }
        if actual != expected:
            raise AssertionError(
                f"{case['name']} differs from original ARM: actual={actual}, expected={expected}")
        if oracle["deque_length"] != case["queue_length"]:
            raise AssertionError(f"{case['name']} real source deque length mismatch")
        results.append({
            "name": case["name"],
            "queue_length": case["queue_length"],
            "head_offset": case["head"],
            "signed_timer": case["timer"],
            "matched": True,
            "source_oracle": oracle,
        })

    dependencies = [production_header, production_cpp, test_cpp, Path(__file__).resolve()]
    report = {
        "validation": "PASS",
        "scope": (
            "Bounded nonempty-queue CharAI::IsMyTurn decision; original ARM function and "
            "real CharAI* deque subtract executed under Unicorn, with explicit concrete "
            "Character query callback outcomes. Empty queue is tested only as the "
            "adapter's explicit unsupported result. No update-queue/timer producers, "
            "Character query bodies, or full AI/frame integration are claimed."
        ),
        "original_library_sha256": EXPECTED_ORIGINAL_SHA,
        "original_elf": str(original),
        "elf_source_evidence": elf_evidence,
        "original_symbol_evidence": oracle_evidence,
        "original_execution": source_execution,
        "differential_cases": len(results),
        "mismatches": 0,
        "guard_checks": guard_report["guard_checks"],
        "host_executable": str(output),
        "host_executable_sha256": sha_file(output),
        "compiler_command": command,
        "results": results,
        "source_sha256": {
            str(path.relative_to(ROOT)).replace("\\", "/"): sha_file(path)
            for path in dependencies
        },
        "oracle_runtime": {
            "pyelftools": __import__("elftools").__version__,
            "unicorn": __import__("unicorn").__version__,
            "architecture": "ARM32 little-endian",
        },
    }
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({
        "validation": report["validation"],
        "differential_cases": report["differential_cases"],
        "guard_checks": report["guard_checks"],
        "deque_subtract_calls": source_execution["deque_subtract_calls"],
        "mismatches": report["mismatches"],
        "report": str(report_path),
    }))


if __name__ == "__main__":
    main()
