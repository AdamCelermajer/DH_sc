"""Host and bounded original ARM checks for normal aggro candidate events."""
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
REPOSITORY = MODULE.parents[1]
ORIGINAL_SHA = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def original_oracle(path: Path, executable: Path) -> dict:
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    from unicorn.arm_const import UC_ARM_REG_R4, UC_ARM_REG_R5, UC_ARM_REG_R7, UC_ARM_REG_R9, UC_ARM_REG_SP
    sys.path.insert(0, str(REPOSITORY / "port/engine-resources/tests"))
    from cpu import Cpu

    assert digest(path) == ORIGINAL_SHA
    manifest = json.loads((MODULE / "reference/character-aggro-candidate-events/original-functions.json").read_text())
    raw = path.read_bytes()
    with path.open("rb") as stream:
        elf = ELFFile(stream)
        symbols = {s.name: (s["st_value"], s["st_size"])
                   for s in elf.get_section_by_name(".dynsym").iter_symbols()}
        segments = [(s["p_vaddr"], s["p_filesz"], s["p_offset"])
                    for s in elf.iter_segments() if s["p_type"] == "PT_LOAD"]
    for row in manifest["functions"]:
        address, size = int(row["elf_address"], 16), row["size"]
        assert symbols[row["original_symbol"]] == (address, size)
        segment = next((b, n, o) for b, n, o in segments if b <= address and address + size <= b + n)
        body = raw[segment[2] + address - segment[0]:segment[2] + address - segment[0] + size]
        assert hashlib.sha256(body).hexdigest() == row["sha256"]

    # Each source TargetInfo here is a valid Character (flag1). The fixture
    # presents a known nearest-first dequeue. Pop is observed, not executed:
    # original deque/heap mechanics are explicitly outside this branch oracle.
    # The host fixture separately runs the copied source search/heap/pop code.
    ai, owner, owner_b = 0x2001000, 0x2004000, 0x2008000
    identity40, new40 = 0x2020000, 0x2024000
    ids = [0x200c000, 0x2010000, 0x2014000]
    cases = [
        ("enemy", 1, [1, 0, 0], identity40, 0),
        ("friend", 1, [2, 0, 0], identity40, 0),
        ("neutral", 1, [4, 0, 0], identity40, 0),
        ("no_relation", 1, [0, 0, 0], identity40, 0),
        ("enemy_precedence", 1, [7, 0, 0], identity40, 0),
        ("friend_precedence", 1, [6, 0, 0], identity40, 0),
        ("three_enemies_no_early_break", 3, [1, 1, 1], identity40, 0),
        ("mixed_nearest_order", 3, [1, 2, 4], identity40, 0),
        ("all_no_enemy_source12", 3, [2, 4, 0], identity40, 0),
        ("all_no_enemy_no_source40", 3, [2, 4, 0], 0, 0),
        ("empty_source12", 0, [0, 0, 0], identity40, 0),
        ("empty_no_event", 0, [0, 0, 0], 0, 0),
        ("fresh_owner_each_classifier", 1, [4, 0, 0], identity40, 1),
        ("fresh_owner_enemy_event", 1, [1, 0, 0], identity40, 1),
        ("fresh_source40_after_friend", 1, [2, 0, 0], identity40, 2),
        ("enemy_suppresses_source12_even_new40", 1, [1, 0, 0], identity40, 3),
    ]
    old = Cpu(path, False, manifest)
    reports = []
    for name, count, masks, source40, mutation in cases:
        rows, stack = old.data + 0x1000000, old.stack + 0xe000
        source_list = stack + 0x18
        old.pointer(ai + 4, owner)
        old.pointer(ai + 0x40, source40)
        old.pointer(source_list, rows)
        old.pointer(stack + 0x28, rows + count * 20)
        for i in range(count):
            old.uc.mem_write(rows + i * 20, struct.pack("<IffII", ids[i], float(i + 1), 0.0, 1, 0))
        calls, pops, completed = [], [], []
        current = [0]
        pending_event = [None]

        def returned(value: int = 0) -> None:
            old.put(0, value)
            old.uc.reg_write(old.pc, old.uc.reg_read(old.lr))

        def observe(uc, address, size, unused):
            if address == 0x3cf92c:
                completed.append(True)
                old.uc.emu_stop()
            elif address in (0x3d574c, 0x3d511c, 0x3d5a98):
                kind = {0x3d574c: 0, 0x3d511c: 1, 0x3d5a98: 2}[address]
                character = old.reg(0) - 0x3c8
                candidate = old.reg(1)
                assert current[0] < count and candidate == ids[current[0]]
                calls.append([kind, character, candidate, count - current[0]])
                if mutation == 1 and kind == 0:
                    old.pointer(ai + 4, owner_b)
                returned(7 if masks[current[0]] & (1 << kind) else 0)
            elif address == 0x3a4d5c:
                character, event, payload = old.reg(0), old.reg(1), old.reg(2)
                if event == 12:
                    assert current[0] == count and payload == struct.unpack("<I", old.uc.mem_read(ai + 0x40, 4))[0]
                else:
                    assert current[0] < count and payload == ids[current[0]]
                calls.append([event, character, payload, count - current[0]])
                pending_event[0] = (character, event, payload)
                # Real original Character::RaiseEvent instructions execute,
                # preserving event/payload and adding owner+0x3c8 before relay.
            elif address == 0x3cbb34:
                character, event, payload = pending_event[0]
                assert (old.reg(0), old.reg(1), old.reg(2)) == (character + 0x3c8, event, payload)
                if mutation == 2 and event != 12:
                    old.pointer(ai + 4, owner_b)
                    old.pointer(ai + 0x40, new40)
                if mutation == 3 and event == 9:
                    old.pointer(ai + 0x40, new40)
                returned()
            elif address == 0x38fb18:
                assert old.reg(0) == source_list and current[0] < count
                pops.append({"candidate": ids[current[0]], "calls_before_pop": len(calls)})
                current[0] += 1
                old.pointer(source_list, rows + current[0] * 20)
                returned()

        observer = old.uc.hook_add(UC_HOOK_CODE, observe)
        old.uc.reg_write(UC_ARM_REG_SP, stack)
        old.uc.reg_write(UC_ARM_REG_R4, old.data + 0x40000)
        old.uc.reg_write(UC_ARM_REG_R5, ai)
        old.uc.reg_write(UC_ARM_REG_R7, source_list)
        old.uc.reg_write(UC_ARM_REG_R9, 1)  # Source constructor sets sb=1 at0x3cf618.
        old.uc.emu_start(0x3cf670, old.stop, count=10000)
        old.uc.hook_del(observer)
        assert completed and current[0] == count, name
        tested = subprocess.run([str(executable), str(count), *(str(mask) for mask in masks),
                                 str(source40), str(mutation)], cwd=REPOSITORY,
                                capture_output=True, text=True, check=True)
        host = json.loads(tested.stdout)
        assert host["status"] == 0 and host["remaining"] == 0 and host["consumed"] == count, (name, host)
        assert host["calls"] == calls, (name, host, calls)
        reports.append({"case": name, "source_calls": calls, "pop_observations": pops})
    return {"validation": "PASS", "original_sha256": ORIGINAL_SHA,
            "comparisons": len(reports), "mismatches": 0,
            "executed_scope": "original normal _UpdateAggro branch0x3cf670 through0x3cf744 "
                              "and0x3cf8ec through0x3cf98c until cleanup convergence0x3cf92c; "
                              "real Character::RaiseEvent wrapper instructions",
            "external_observers": "relationship classifiers, CharAI event relay, and priority-queue pop; "
                                  "known nearest-first queue fixture; original search/heap/assertion/cleanup bodies excluded",
            "results": reports}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler")
    parser.add_argument("--original-elf", type=Path)
    parser.add_argument("--output", type=Path, default=MODULE / "build/character-aggro-candidate-events/host")
    parser.add_argument("--report", type=Path, default=MODULE / "build/character-aggro-candidate-events/validation.json")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        parser.error("pass --compiler or set CXX")
    output = args.output.resolve()
    if os.name == "nt" and not output.suffix:
        output = output.with_suffix(".exe")
    output.parent.mkdir(parents=True, exist_ok=True)
    sources = [MODULE / "character_aggro_candidate_events.cpp", MODULE / "character_aggro_target_search.cpp",
               MODULE / "tests/character_aggro_candidate_events.cpp"]
    command = [compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror", "-pedantic",
               *(str(p) for p in sources), "-o", str(output)]
    subprocess.run(command, cwd=REPOSITORY, check=True)
    tested = subprocess.run([str(output)], cwd=REPOSITORY, capture_output=True, text=True, check=True)
    sys.stdout.write(tested.stdout)
    host = json.loads(tested.stdout)
    assert host["aggro_candidate_event_cases"] == 33 and host["mismatches"] == 0
    for key in ("source_object_identity_preserved", "nearest_search_heap_reused",
                "all_candidates_no_first_enemy_break", "enemy_friend_neutral_precedence",
                "event_precedes_pop", "fresh_owner_and_source40", "independent_list_reentry",
                "partial_failures_and_atomic_guards"):
        assert host[key] is True
    assert host["native_wired"] is False
    dependencies = sources + [MODULE / "character_aggro_candidate_events.hpp",
                              MODULE / "character_aggro_target_search.hpp", Path(__file__).resolve(),
                              MODULE / "reference/character-aggro-candidate-events/NOTES.md",
                              MODULE / "reference/character-aggro-candidate-events/original-functions.json"]
    report = {"validation": "PASS", "host_report": host, "compiler_command": command,
              "source_sha256": {str(p.relative_to(REPOSITORY)).replace("\\", "/"): digest(p)
                                for p in dependencies}, "native_wired": False,
              "scope": "one bounded source candidate-consumption branch using source TargetList; no native wiring"}
    if args.original_elf:
        report["original_arm_comparison"] = original_oracle(args.original_elf.resolve(), output)
    destination = args.report.resolve()
    destination.parent.mkdir(parents=True, exist_ok=True)
    destination.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"report: {destination}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
