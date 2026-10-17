"""Compile and run the source-ordered Limbus Blur group-status producer."""
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

LEVEL_WORLD = Path(__file__).resolve().parents[1]
REPOSITORY = LEVEL_WORLD.parents[1]


def original_branch_oracle(path: Path) -> dict:
    """Run the bounded original ARM group branch and its two real query leaves."""
    from elftools.elf.elffile import ELFFile
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE
    from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_R4, UC_ARM_REG_R6, UC_ARM_REG_SP

    original = path.read_bytes()
    original_sha = hashlib.sha256(original).hexdigest()
    assert original_sha == "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"
    with path.open("rb") as stream:
        elf = ELFFile(stream)
        segments = [(int(s["p_vaddr"]), int(s["p_filesz"]), int(s["p_offset"]))
                    for s in elf.iter_segments() if s["p_type"] == "PT_LOAD"]

    def image(address: int, size: int) -> bytes:
        for base, length, offset in segments:
            if base <= address and address + size <= base + length:
                return original[offset + address - base:offset + address - base + size]
        raise ValueError(f"original ELF range missing: {address:#x}/{size}")

    # These cases exercise original machine code, separately from the C++
    # fixture. Member callbacks are observed at entry, not replaced by stubs.
    cases = [
        ("other_role_no_group_read", 0, ["a"], {"a": 0}, 7, [], None),
        ("empty", 3, [], {}, 0, [], None),
        ("self_only", 3, ["owner", "owner"], {}, 0, [], None),
        ("no_limbus", 3, ["a", "b"], {"a": 3, "b": 4}, 0, ["a", "b"], None),
        ("mixed_no_short_circuit", 3, ["owner", "a", "b", "owner", "c"],
         {"a": 0, "b": 3, "c": 0}, 2, ["a", "b", "c"], None),
        ("duplicate_peer", 3, ["a", "a"], {"a": 0}, 2, ["a", "a"], None),
        ("captured_count_live_begin", 3, ["owner", "a", "b", "c"],
         {"a": 3, "b": 3, "c": 3, "d": 0, "e": 4}, 2, ["a", "d", "e"],
         ("a", ["owner", "a", "d", "e"], 1)),
    ]
    reports = []
    for name, role, members, states, expected_status, expected_queries, redirect in cases:
        machine = Uc(UC_ARCH_ARM, UC_MODE_ARM)
        for base in (0x3c0000, 0x3c2000):
            machine.mem_map(base, 0x1000)
            machine.mem_write(base, image(base, 0x1000))
        machine.mem_map(0x100000, 0x30000)
        machine.mem_map(0x200000, 0x2000)
        actors = {label: 0x100000 + index * 0x2000
                  for index, label in enumerate(("owner", "a", "b", "c", "d", "e"))}
        names = {address: label for label, address in actors.items()}
        group, member_array, redirected_array = 0x120000, 0x121000, 0x122000

        def word(address: int, value: int) -> None:
            machine.mem_write(address, struct.pack("<I", value & 0xffffffff))

        def words(address: int, labels: list[str]) -> None:
            for index, label in enumerate(labels):
                word(address + 4 * index, actors[label])

        for label, address in actors.items():
            # Real SM_GetState reads its current-state pointer at SM+0x20,
            # then the state ID at that pointer. SM is Character+0x4fc here.
            node = address + 0x1000
            word(address + 0x4fc + 0x20, node)
            word(node, states.get(label, 3))
        word(actors["owner"] + 0x400, role)
        word(actors["owner"] + 0x3fc, group if role == 3 else 1)
        word(group + 0x18, member_array)
        word(group + 0x1c, member_array + 4 * len(members))
        word(group + 0x24, 7)
        words(member_array, members)
        if redirect:
            words(redirected_array, redirect[1])
        queries = []
        finished = []

        def observe(machine, address, size, _):
            if address == 0x3c2ca0:
                finished.append(True)
                machine.emu_stop()
            elif address == 0x3c01c0:
                actor = machine.reg_read(UC_ARM_REG_R0) - 0x4fc
                label = names[actor]
                assert label != "owner"
                assert struct.unpack("<I", machine.mem_read(group + 0x24, 4))[0] == 7
                queries.append(label)
                if redirect and label == redirect[0]:
                    word(group + 0x18, redirected_array)
                    word(group + 0x1c, redirected_array + 4 * redirect[2])

        machine.hook_add(UC_HOOK_CODE, observe)
        machine.reg_write(UC_ARM_REG_R4, actors["owner"])
        machine.reg_write(UC_ARM_REG_R6, 0)
        machine.reg_write(UC_ARM_REG_SP, 0x201000)
        machine.emu_start(0x3c2c94, 0xffffffff, count=1000)
        actual_status = struct.unpack("<I", machine.mem_read(group + 0x24, 4))[0]
        assert finished and actual_status == expected_status and queries == expected_queries, (
            name, actual_status, queries)
        reports.append({"case": name, "group_status": actual_status, "member_queries": queries})
    return {"validation": "PASS", "original_sha256": original_sha,
            "cases": len(reports), "executed_scope":
            "original ARM role/group branch 0x3c2c94 through normal convergence 0x3c2ca0; "
            "real SM_IsInLimbus and SM_GetState leaves; earlier OnBlur prefix not executed",
            "results": reports,
            "original_range_sha256": {
                "0x3c2be4/344": hashlib.sha256(image(0x3c2be4, 344)).hexdigest(),
                "0x3c01c0/20": hashlib.sha256(image(0x3c01c0, 20)).hexdigest(),
                "0x3c01ac/20": hashlib.sha256(image(0x3c01ac, 20)).hexdigest()}}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler")
    parser.add_argument("--report", type=Path)
    parser.add_argument("--original-elf", type=Path,
                        help="optional verified original ELF for bounded ARM branch oracle")
    parser.add_argument("--output", type=Path,
                        default=LEVEL_WORLD / "build" / "character_group_limbus_blur_host")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        parser.error("pass --compiler or set CXX")
    output = args.output.resolve()
    if os.name == "nt" and not output.suffix:
        output = output.with_suffix(".exe")
    output.parent.mkdir(parents=True, exist_ok=True)
    sources = [LEVEL_WORLD / "tests" / "character_group_limbus_blur.cpp",
               LEVEL_WORLD / "character_group_limbus_blur.cpp"]
    command = [compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror",
               *(str(path) for path in sources), "-o", str(output)]
    for command_to_run in [command, [str(output)]]:
        run = subprocess.run(command_to_run, cwd=REPOSITORY, text=True, capture_output=True)
        sys.stdout.write(run.stdout)
        sys.stderr.write(run.stderr)
        if run.returncode:
            return run.returncode
    report = json.loads(run.stdout)
    assert report["group_limbus_blur_cases"] == 16 and report["mismatches"] == 0
    for key in ["role3_only", "any_other_limbus_selects2", "all_other_queries_in_order",
                "count_snapshot_pointer_live", "owner_skipped", "aliases_and_errors_guarded"]:
        assert report[key] is True, (key, report)
    assert report["native_group_wired"] is False
    dependencies = sources + [LEVEL_WORLD / "character_group_limbus_blur.hpp",
                              LEVEL_WORLD / "character_group_respawn.hpp", Path(__file__).resolve()]
    evidence = {
        "validation": "PASS", "host_report": report, "compiler_command": command,
        "source_sha256": {str(path.relative_to(REPOSITORY)).replace("\\", "/"):
                          hashlib.sha256(path.read_bytes()).hexdigest() for path in dependencies},
        "native_group_wired": False,
    }
    if args.original_elf:
        evidence["original_branch_oracle"] = original_branch_oracle(args.original_elf.resolve())
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(evidence, indent=2) + "\n", encoding="utf8")
    print(json.dumps(evidence))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
