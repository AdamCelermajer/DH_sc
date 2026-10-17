"""Verify and run the three bounded source AI relationship query bodies."""
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
    sys.path.insert(0, str(REPOSITORY / "port/engine-resources/tests"))
    from cpu import Cpu

    assert digest(path) == ORIGINAL_SHA
    manifest = json.loads((MODULE / "reference/character-ai-relations/original-functions.json").read_text())
    raw = path.read_bytes()
    with path.open("rb") as stream:
        elf = ELFFile(stream)
        symbols = {s.name: (s["st_value"], s["st_size"])
                   for s in elf.get_section_by_name(".dynsym").iter_symbols()}
        segments = [(s["p_vaddr"], s["p_filesz"], s["p_offset"])
                    for s in elf.iter_segments() if s["p_type"] == "PT_LOAD"]

    def body(address: int, size: int) -> bytes:
        b, n, o = next((b, n, o) for b, n, o in segments if b <= address and address + size <= b + n)
        return raw[o + address - b:o + address - b + size]

    for row in manifest["functions"]:
        at, size = int(row["elf_address"], 16), row["size"]
        assert symbols[row["original_symbol"]] == (at, size)
        assert hashlib.sha256(body(at, size)).hexdigest() == row["sha256"]
    for row in manifest["vtable_ranges"]:
        at, size = int(row["elf_address"], 16), row["size"]
        assert symbols[row["original_symbol"]] == (at, size)
        image = body(at, size)
        assert hashlib.sha256(image).hexdigest() == row["sha256"]
        for slot in row["slots"]:
            assert struct.unpack_from("<I", image, 8 + int(slot["byte_offset"], 16))[0] == int(slot["target"], 16)

    # relation, explicit object, target fallback, handle result, word_f4,
    # owner player, target player, interactive, type, first row value, mutation,
    # owner raw faction, target raw faction.
    scenarios = [
        ("negative_first_match", 1, 1, 1, 0, 0, 0, 1, 8, -1, 0, 0, 1),
        ("zero_first_match", 1, 1, 1, 0, 0, 0, 1, 8, 0, 0, 0, 1),
        ("positive_first_match", 1, 1, 1, 0, 0, 0, 1, 8, 1, 0, 0, 1),
        ("signed_min_value", 1, 1, 1, 0, 0, 0, 1, 8, -2147483648, 0, 0, 1),
        ("signed_max_value", 1, 1, 1, 0, 0, 0, 1, 8, 2147483647, 0, 0, 1),
        ("missing_match", 1, 1, 1, 0, 0, 0, 1, 8, -1, 0, 0, 3),
        ("null_argument_fallback", 0, 1, 1, 0, 0, 0, 1, 8, -1, 0, 0, 1),
        ("null_argument_no_target", 0, 0, 1, 0, 0, 0, 1, 8, -1, 0, 0, 1),
        ("expired_handle", 1, 1, 0, 0, 0, 0, 1, 8, -1, 0, 0, 1),
        ("noncharacter_word_f4", 1, 1, 1, 7, 0, 0, 1, 8, -1, 0, 0, 1),
        ("noninteractive", 1, 1, 0, 0, 0, 0, 0, 8, -1, 0, 0, 1),
        ("interaction_type7", 1, 1, 0, 0, 0, 0, 1, 7, -1, 0, 0, 1),
        ("two_players", 1, 1, 1, 0, 7, 9, 1, 8, -1, 0, 0, 1),
        ("target_player_only", 1, 1, 1, 0, 0, 1, 1, 8, -1, 0, 0, 1),
        ("owner_player_only", 1, 1, 1, 0, 1, 0, 1, 8, -1, 0, 0, 1),
        ("fixed_candidate_after_handle", 1, 1, 1, 0, 0, 0, 1, 8, -1, 1, 0, 1),
        ("owner_changes_target_getter1", 1, 1, 1, 0, 0, 0, 1, 8, -1, 2, 0, 1),
        ("owner_changes_owner_getter1", 1, 1, 1, 0, 0, 0, 1, 8, -1, 3, 0, 1),
        ("owner_changes_count_read2", 1, 1, 1, 0, 0, 0, 1, 8, -1, 4, 0, 1),
        ("owner_and_table_change_after_capture", 1, 1, 1, 0, 0, 0, 1, 8, -1, 5, 0, 1),
        ("row_changes_target_getter3", 1, 1, 1, 0, 0, 0, 1, 8, -1, 6, 0, 1),
        ("owner_changes_player_query", 1, 1, 1, 0, 0, 0, 1, 8, -1, 7, 0, 1),
        ("fresh_interaction_owner", 1, 1, 0, 0, 0, 0, 1, 8, -1, 8, 0, 1),
        ("fresh_target_faction", 1, 1, 1, 0, 0, 0, 1, 8, -1, 9, 0, 1),
        ("negative_owner_getter_fallback10", 1, 1, 1, 0, 0, 0, 1, 8, -1, 0, -1, 1),
        ("out_of_range_target_getter_fallback10", 1, 1, 1, 0, 0, 0, 1, 8, -1, 0, 0, 99),
    ]
    old = Cpu(path, False, manifest)
    ai, owner, owner_b, target_object, target = 0x2001000, 0x2004000, 0x2008000, 0x200c000, 0x2010000
    count_var, table_var, table, new_table = 0x201a000, 0x201b000, 0x2020000, 0x2028000
    entries, alternate, vt, object_vt = 0x2030000, 0x2038000, 0x2040000, 0x2041000
    got = 0x994a98
    old.pointer(got + 0x2244, count_var)
    old.pointer(got + 0x462c, table_var)
    old.pointer(vt + 0x28, 0x3a49f0)
    old.pointer(object_vt + 0x88, 0x3883b0)
    old.pointer(object_vt + 0x90, 0x38ad74)
    for actor in (owner, owner_b, target):
        old.pointer(actor, vt)
    old.pointer(target_object, object_vt)
    reports = []
    for relation in range(3):
        entry = (0x3d574c, 0x3d511c, 0x3d5a98)[relation]
        field_point = (0x3d57c0, 0x3d5168, 0x3d5ae4)[relation]
        count_points = ((0x3d57e8, 0x3d5834), (0x3d5190, 0x3d51dc), (0x3d5b0c, 0x3d5b58))[relation]
        table_point = (0x3d588c, 0x3d5218, 0x3d5b94)[relation]
        for scenario in scenarios:
            name, explicit, fallback, resolved, f4, op, tp, interactive, interaction_type, first_value, mutation, owner_raw, target_raw = scenario
            old.pointer(ai + 4, owner)
            old.pointer(ai + 0x40, target_object if fallback else 0)
            old.pointer(target + 0xf4, f4)
            old.pointer(owner + 0xff8, owner_raw & 0xffffffff)
            old.pointer(owner_b + 0xff8, 2)
            old.pointer(target + 0xff8, target_raw & 0xffffffff)
            old.pointer(count_var, 16)
            old.pointer(table_var, table)
            old.uc.mem_write(table, bytes(16 * 12))
            old.uc.mem_write(new_table, bytes(16 * 12))
            for row in (0, 2, 10):
                old.uc.mem_write(table + row * 12, struct.pack("<III", 0, 3, entries))
            old.uc.mem_write(new_table, struct.pack("<III", 0, 1, alternate))
            for i, (id_, value) in enumerate(((1, first_value), (2, 1), (1, 1))):
                old.uc.mem_write(entries + i * 12, struct.pack("<IIi", 0, id_, value))
            old.uc.mem_write(alternate, struct.pack("<IIi", 0, 1, 1))
            calls = []
            getter_actor = [0]
            target_getters, owner_getters, extra_counts = [0], [0], [0]

            def returned(value: int = 0):
                old.put(0, value)
                old.uc.reg_write(old.pc, old.uc.reg_read(old.lr))

            def observe(uc, address, size, unused):
                if address == 0x33dd70:
                    assert old.reg(1) == target_object
                    old.pointer(old.reg(0), target_object)
                    returned(old.reg(0))
                elif address == 0x33ff8c:
                    assert old.reg(1) == 0
                    calls.append([0, target_object, 0, int(bool(resolved))])
                    if mutation == 1:
                        old.pointer(ai + 0x40, 0)
                    returned(target if resolved else 0)
                elif address == field_point:
                    calls.append([1, target, 0, f4])
                elif address == 0x3a3180:
                    getter_actor[0] = old.reg(0)
                elif address in (0x3a31a4, 0x3a31ac):
                    # At0x3a31a4 BX executes only if original signed< check
                    # passed; an untaken BX continues to fallback10 first.
                    if address == 0x3a31a4 and old.reg(0) >= 16:
                        return
                    actor, value = getter_actor[0], old.reg(0)
                    calls.append([2, actor, 0, value])
                    if actor == target:
                        target_getters[0] += 1
                        if mutation == 2 and target_getters[0] == 1:
                            old.pointer(ai + 4, owner_b)
                        if mutation == 6 and target_getters[0] == 3:
                            old.uc.mem_write(table, struct.pack("<III", 0, 1, alternate))
                        if mutation == 9 and target_getters[0] == 1:
                            old.pointer(target + 0xff8, 2)
                    else:
                        owner_getters[0] += 1
                        if mutation == 3 and owner_getters[0] == 1:
                            old.pointer(ai + 4, owner_b)
                elif address in count_points:
                    calls.append([3, 0, 0, old.reg(3)])
                    extra_counts[0] += 1
                    if mutation == 4 and extra_counts[0] == 2:
                        old.pointer(ai + 4, owner_b)
                elif address == 0x3a49f0:
                    actor = old.reg(0)
                    value = tp if actor == target else op
                    calls.append([4, actor, 0, value])
                    if mutation == 7 and actor != target:
                        old.pointer(ai + 4, owner_b)
                    returned(value)
                elif address == table_point:
                    calls.append([5, 0, 0, int(old.reg(4) == new_table)])
                    if mutation == 5:
                        old.pointer(ai + 4, owner_b)
                        old.pointer(table_var, new_table)
                elif address in (0x3883b0, 0x38ad74):
                    assert old.reg(0) == target_object
                    value = interactive if address == 0x3883b0 else interaction_type
                    calls.append([6 if address == 0x3883b0 else 7, target_object, old.reg(1), value])
                    if mutation == 8 and address == 0x3883b0:
                        old.pointer(ai + 4, owner_b)
                    returned(value)

            observer = old.uc.hook_add(UC_HOOK_CODE, observe)
            value = old.invoke(entry, [ai, target_object if explicit else 0])
            old.uc.hook_del(observer)
            args = [relation, explicit, fallback, resolved, f4, op, tp, interactive,
                    interaction_type, first_value, mutation, owner_raw, target_raw]
            run = subprocess.run([str(executable), *(str(v) for v in args)], cwd=REPOSITORY,
                                 text=True, capture_output=True, check=True)
            host = json.loads(run.stdout)
            assert host["status"] == 0 and host["value"] == value, (relation, name, host, value)
            assert host["calls"] == calls, (relation, name, host["calls"], calls)
            reports.append({"relation": relation, "case": name, "value": value, "source_calls": calls})
    return {"validation": "PASS", "original_sha256": ORIGINAL_SHA,
            "comparisons": len(reports), "mismatches": 0,
            "executed_scope": "original normal Enemy/Friend/Neutral query bodies and real GetCharAIFactionId; "
                              "original faction-array scan and signed predicates execute",
            "external_services": "const ObjectBase GetHandle/GetObject(false), IsPlayer and interactive/type virtuals observed; "
                                 "invalid-faction diagnostic/assertion bodies excluded",
            "results": reports}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler")
    parser.add_argument("--original-elf", type=Path)
    parser.add_argument("--output", type=Path, default=MODULE / "build/character-ai-relations/host")
    parser.add_argument("--report", type=Path, default=MODULE / "build/character-ai-relations/validation.json")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        parser.error("pass --compiler or set CXX")
    output = args.output.resolve()
    if os.name == "nt" and not output.suffix:
        output = output.with_suffix(".exe")
    output.parent.mkdir(parents=True, exist_ok=True)
    sources = [MODULE / "character_ai_relations.cpp", REPOSITORY / "port/game-data/ai.cpp",
               MODULE / "tests/character_ai_relations.cpp"]
    command = [compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror", "-pedantic",
               *(str(p) for p in sources), "-o", str(output)]
    subprocess.run(command, cwd=REPOSITORY, check=True)
    tested = subprocess.run([str(output)], cwd=REPOSITORY, text=True, capture_output=True, check=True)
    sys.stdout.write(tested.stdout)
    host = json.loads(tested.stdout)
    assert host["ai_relation_cases"] == 62 and host["mismatches"] == 0
    for key in ("enemy_faction_leaf_reused", "all_three_source_predicates", "null_target_and_noncharacter_branches",
                "fresh_owner_faction_and_table_reads", "row_read_after_target_getter", "partial_failure_and_alias_guards"):
        assert host[key] is True
    assert host["native_wired"] is False
    dependencies = sources + [MODULE / "character_ai_relations.hpp", REPOSITORY / "port/game-data/ai.hpp",
                              REPOSITORY / "port/game-data/data.hpp", Path(__file__).resolve(),
                              MODULE / "reference/character-ai-relations/NOTES.md",
                              MODULE / "reference/character-ai-relations/original-functions.json"]
    report = {"validation": "PASS", "host_report": host, "compiler_command": command,
              "source_sha256": {str(p.relative_to(REPOSITORY)).replace("\\", "/"): digest(p)
                                for p in dependencies}, "native_wired": False,
              "scope": "three bounded source query orchestrations; existing enemy faction leaf reused; no native wiring"}
    if args.original_elf:
        report["original_arm_comparison"] = original_oracle(args.original_elf.resolve(), output)
    destination = args.report.resolve()
    destination.parent.mkdir(parents=True, exist_ok=True)
    destination.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"report: {destination}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
