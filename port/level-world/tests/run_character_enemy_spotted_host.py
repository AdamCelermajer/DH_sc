"""Build the bounded source EnemySpotted gate and optionally compare ARM code."""
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
    """Execute 0x3d1518..normal convergence; observe explicit external services.

    Real original SM_GetState/IsAwaitingToSpawn/IsInLimbus instructions run.
    Original float comparison PLT stubs run; imported soft-float helpers are
    modeled by IEEE binary32 comparisons. Group, combat, aggro mutation and AIS
    callbacks are observers, not claims that those full bodies are implemented.
    Diagnostic branch bodies, initial debug prefix and stack canary are omitted.
    """
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    from unicorn.arm_const import UC_ARM_REG_R4, UC_ARM_REG_R7, UC_ARM_REG_R8
    sys.path.insert(0, str(REPOSITORY / "port/engine-resources/tests"))
    from cpu import Cpu

    assert digest(path) == ORIGINAL_SHA
    manifest = json.loads((MODULE / "reference/character-enemy-spotted/original-functions.json").read_text())
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

    class EnemyCpu(Cpu):
        def external(self, uc, address, size, unused):
            name = self.imports.get(address)
            if name in ("__aeabi_fcmpeq", "__aeabi_fcmpgt"):
                a, b = [struct.unpack("<f", struct.pack("<I", self.reg(i)))[0] for i in (0, 1)]
                self.put(0, int(a == b if name == "__aeabi_fcmpeq" else a > b))
                uc.reg_write(self.pc, uc.reg_read(self.lr))
            else:
                super().external(uc, address, size, unused)

    # name, enemy state, owner state, in combat, player, threat bits, delta bits,
    # group present, callback mutation, active present. Host CLI uses same values.
    cases = [
        ("zero_add", 3, 3, 0, 0, 0, 0x3f800000, 1, 0, 1),
        ("negative_zero_add", 3, 3, 0, 0, 0x80000000, 0xbf800000, 1, 0, 1),
        ("positive_existing", 3, 3, 0, 0, 0x3f800000, 0x3f800000, 1, 0, 1),
        ("negative_existing", 3, 3, 0, 0, 0xbf800000, 0x3f800000, 1, 0, 1),
        ("nan_existing", 3, 3, 0, 0, 0x7fc12345, 0x3f800000, 1, 0, 1),
        ("infinite_existing", 3, 3, 0, 0, 0x7f800000, 0x3f800000, 1, 0, 1),
        ("nan_delta_still_dispatch", 3, 3, 0, 0, 0, 0x7fc12345, 1, 0, 1),
        ("zero_delta_still_dispatch", 3, 3, 0, 0, 0, 0, 1, 0, 1),
        ("combat_nonplayer_skips_aggro", 3, 3, 7, 0, 0, 0x3f800000, 1, 0, 1),
        ("combat_player_checks_aggro", 3, 3, 1, 9, 0, 0x3f800000, 1, 0, 1),
        ("enemy_awaiting_spawn", 17, 3, 0, 0, 0, 0x3f800000, 1, 0, 1),
        ("owner_awaiting_spawn", 3, 17, 0, 0, 0, 0x3f800000, 1, 0, 1),
        ("enemy_limbus", 0, 3, 0, 0, 0, 0x3f800000, 1, 0, 1),
        ("owner_limbus", 3, 0, 0, 0, 0, 0x3f800000, 1, 0, 1),
        ("dead_state_not_vetoed_here", 12, 12, 0, 0, 0, 0x3f800000, 1, 0, 1),
        ("group_changes_owner_and_ais", 3, 17, 0, 0, 0, 0x3f800000, 1, 1, 1),
        ("enemy_query_changes_owner", 3, 17, 0, 0, 0, 0x3f800000, 1, 2, 1),
        ("enemy_limbus_query_changes_owner", 3, 3, 0, 0, 0, 0x3f800000, 1, 10, 1),
        ("get_aggro_changes_owner", 3, 3, 0, 0, 0, 0x3f800000, 1, 3, 1),
        ("owner_captured_before_amount", 3, 3, 0, 0, 0, 0x3f800000, 1, 4, 1),
        ("add_reselects_ais", 3, 3, 0, 0, 0, 0x3f800000, 1, 5, 1),
        ("positive_debug_reselects_ais", 3, 3, 0, 0, 0, 0x3f800000, 1, 6, 1),
        ("group_absent", 3, 3, 0, 0, 0, 0x3f800000, 0, 0, 1),
        ("no_active_still_adds", 3, 3, 0, 0, 0, 0x3f800000, 1, 0, 0),
    ]
    reports = []
    old = EnemyCpu(path, False, manifest)
    for name, es, os_, combat, player, threat, delta, has_group, mutation, has_active in cases:
        ai, owner, owner_b, enemy = 0x2001000, 0x2004000, 0x2008000, 0x200c000
        group, first, second = 0x2010000, 0x2011000, 0x2012000
        vtable, vt_first, vt_second, got, design = 0x2014000, 0x2015000, 0x2016000, 0x2024000, 0x2029000
        labels = {enemy: "enemy", owner: "owner", owner_b: "owner_b"}
        calls = ["debug_entry"]
        captured_add_owner, selected, decision = [0], [0], [0]
        finished = []
        old.pointer(ai + 4, owner)
        old.pointer(ai + 0x34, group if has_group else 0)
        old.pointer(ai + 0x1c, first if has_active else 0)
        for character, state in ((enemy, es), (owner, os_), (owner_b, 0 if mutation == 10 else 3)):
            old.pointer(character + 0x4fc + 0x20, character + 0x1000)
            old.pointer(character + 0x1000, state)
        old.pointer(enemy, vtable)
        old.pointer(vtable + 0x28, 0x3a49f0)  # Original Character::IsPlayer.
        old.pointer(first, vt_first)
        old.pointer(second, vt_second)
        old.pointer(vt_first + 0x34, 0x3dd2f4)
        old.pointer(vt_second + 0x34, 0x3dcde8)  # Distinct fixture observer.
        old.pointer(got + 0x32c8, design - 0x100)
        old.pointer(design - 0x100, design)
        old.pointer(design + 0x30, 0x40a00000)

        def returned(value: int = 0) -> None:
            old.put(0, value)
            old.uc.reg_write(old.pc, old.uc.reg_read(old.lr))

        def observe(uc, address, size, unused):
            if address == 0x3d1548:
                finished.append(True)
                old.uc.emu_stop()
            elif address == 0x3d27cc:
                assert (old.reg(0), old.reg(1), old.reg(2)) == (group, owner, enemy)
                calls.append("group")
                if mutation == 1:
                    old.pointer(ai + 4, owner_b)
                    old.pointer(ai + 0x1c, second)
                returned()
            elif address in (0x3c0230, 0x3c01c0):
                character = old.reg(0) - 0x4fc
                calls.append(("await_" if address == 0x3c0230 else "limbus_") + labels[character])
                if address == 0x3c0230 and character == enemy and mutation == 2:
                    old.pointer(ai + 4, owner_b)
                if address == 0x3c01c0 and character == enemy and mutation == 10:
                    old.pointer(ai + 4, owner_b)
                # Observe, then run these original leaves and SM_GetState.
            elif address in (0x3d1540, 0x3d1574, 0x3d1584, 0x3d159c):
                if old.reg(0):
                    decision[0] = {0x3d1540: 1, 0x3d1574: 2, 0x3d1584: 3, 0x3d159c: 4}[address]
            elif address == 0x3d4bc4:
                assert old.reg(0) == ai
                calls.append("combat"); returned(combat)
            elif address == 0x3a49f0:
                assert old.reg(0) == enemy
                calls.append("player"); returned(player)
            elif address == 0x3d4ac8:
                assert (old.reg(0), old.reg(1)) == (ai, enemy)
                calls.append("get_aggro")
                if mutation == 3:
                    old.pointer(ai + 4, owner_b)
                returned(threat)
            elif address == 0x3d1600:
                calls.append("amount")
                if mutation == 4:
                    old.pointer(ai + 4, owner_b)
            elif address == 0x3d7c68:
                character = old.reg(0) - 0x3c8
                assert old.reg(1) == enemy and old.reg(2) == 0x40a00000
                captured_add_owner[0] = character
                calls.append("add_" + labels[character])
                if mutation == 5:
                    old.pointer(ai + 0x1c, second)
                returned(delta)
            elif address == 0x3d163c:
                calls.append("debug_added")
                if mutation == 6:
                    old.pointer(ai + 0x1c, second)
                old.uc.reg_write(old.pc, 0x3d1618)
            elif address == 0x3d1620:
                active = struct.unpack("<I", old.uc.mem_read(ai + 0x1c, 4))[0]
                if not active:
                    decision[0] = 5
            elif address in (0x3dd2f4, 0x3dcde8):
                assert old.reg(1) == enemy
                selected[0] = old.reg(0)
                calls.append("dispatch_first" if selected[0] == first else "dispatch_second")
                decision[0] = 6
                returned()

        observer = old.uc.hook_add(UC_HOOK_CODE, observe)
        old.uc.reg_write(UC_ARM_REG_R4, got)
        old.uc.reg_write(UC_ARM_REG_R7, ai)
        old.uc.reg_write(UC_ARM_REG_R8, enemy)
        # Set SP/LR explicitly. invoke would instead require returning through
        # the omitted original prologue/stack canary, which is outside scope.
        from unicorn.arm_const import UC_ARM_REG_SP
        old.uc.reg_write(UC_ARM_REG_SP, old.stack + 0xe000)
        old.uc.emu_start(0x3d1518, old.stop, count=10000)
        old.uc.hook_del(observer)
        assert finished, name
        run = subprocess.run([str(executable), *(str(v) for v in
                            (es, os_, combat, player, threat, delta, has_group, mutation, has_active))],
                             cwd=REPOSITORY, text=True, capture_output=True, check=True)
        host = json.loads(run.stdout)
        assert host["status"] == 0 and host["decision"] == decision[0], (name, host, decision)
        assert [c for c in host["calls"] if c != "retain"] == calls, (name, host, calls)
        assert host["selected"] == selected[0] and host["added_owner"] == captured_add_owner[0], (name, host)
        reports.append({"case": name, "decision": decision[0], "source_calls": calls,
                        "selected": selected[0], "added_owner": captured_add_owner[0]})
    return {"validation": "PASS", "original_sha256": ORIGINAL_SHA,
            "comparisons": len(reports), "mismatches": 0,
            "executed_scope": "original ARM gate/dispatch 0x3d1518 to 0x3d1548 normal convergence; "
                              "real SM_IsAwaitingToSpawn, SM_IsInLimbus and SM_GetState; "
                              "original float-comparison PLT stubs with modeled imported helpers",
            "external_services": "group notification, IsInCombat, enemy IsPlayer, GetAggro, "
                                 "AddAggro, and AIS dispatch observed; diagnostic bodies omitted",
            "results": reports}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler")
    parser.add_argument("--output", type=Path, default=MODULE / "build/character-enemy-spotted/host")
    parser.add_argument("--report", type=Path, default=MODULE / "build/character-enemy-spotted/validation.json")
    parser.add_argument("--original-elf", type=Path)
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        parser.error("pass --compiler or set CXX")
    output = args.output.resolve()
    if os.name == "nt" and not output.suffix:
        output = output.with_suffix(".exe")
    output.parent.mkdir(parents=True, exist_ok=True)
    sources = [MODULE / "character_enemy_spotted.cpp", MODULE / "tests/character_enemy_spotted.cpp"]
    command = [compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror", "-pedantic",
               *(str(path) for path in sources), "-o", str(output)]
    subprocess.run(command, cwd=REPOSITORY, check=True)
    tested = subprocess.run([str(output)], cwd=REPOSITORY, check=True, capture_output=True, text=True)
    sys.stdout.write(tested.stdout)
    host = json.loads(tested.stdout)
    assert host["enemy_spotted_cases"] == 38 and host["mismatches"] == 0
    for key in ("awaiting_spawn_not_dead_gate", "group_precedes_state_queries",
                "fresh_owner_and_active_reads", "zero_aggro_adds_before_dispatch",
                "binary32_edges_checked", "held_selected_ais_survives_replacement",
                "partial_failure_and_reentry_guard_checked"):
        assert host[key] is True
    assert host["native_wired"] is False
    dependencies = sources + [MODULE / "character_enemy_spotted.hpp", Path(__file__).resolve(),
                              MODULE / "reference/character-enemy-spotted/NOTES.md",
                              MODULE / "reference/character-enemy-spotted/original-functions.json"]
    report = {"validation": "PASS", "host_report": host, "compiler_command": command,
              "source_sha256": {str(p.relative_to(REPOSITORY)).replace("\\", "/"): digest(p)
                                for p in dependencies},
              "scope": "one bounded source gate/dispatch; no native or CMake wiring",
              "native_wired": False}
    if args.original_elf:
        report["original_arm_comparison"] = original_oracle(args.original_elf.resolve(), output)
    report_path = args.report.resolve()
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"report: {report_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
