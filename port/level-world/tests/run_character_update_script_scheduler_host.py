"""Compile the scheduler projection and compare its paths with original ARM code."""
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
MANIFEST = MODULE / "reference/character-update-script-scheduler/original-functions.json"
ORIGINAL_SHA = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"
GOT = 0x994A98
MAP_SLOT = 0x995BCC
APP_SLOT = 0x99828C
DATA = 0x02000000
MAP = DATA + 0x1000
NODE = DATA + 0x1100
CURRENT = DATA + 0x2000
VICTIM = DATA + 0x3000
CONTROLLER = DATA + 0x4000
FSM = DATA + 0x5000
LEVEL = DATA + 0x6000
ONLINE = DATA + 0x7000
NEW_NODE = DATA + 0x8000
EXITS = {0x3ABF90, 0x3ABF9C, 0x3AC608}
OPS = {
    0x31F594: 0,  # Application::GetCurrentLevel
    0x40570C: 1,  # v2Controller::Cmd_Kill
    0x3A7B24: 2,  # Character::UnLoadScriptProcess
    0x3CF3A4: 3,  # CharAI::LoadNInitScriptProcess
    0x3A3064: 4,  # Character::IsMonster
    0x3A3144: 5,  # Character::IsMiniBoss
    0x3A3158: 6,  # Character::IsBoss
    0x7FD794: 7,  # PlayerManager::GetOnline
    0x60B0CC: 8,  # Timer::getRealTime
}


def sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def fixtures():
    # Fields follow the test kernel's input projection. Map keys are generated
    # as -100.. and the single physical ARM node models map.begin().
    return {
        "state_12": dict(state=12),
        "state_2": dict(state=2),
        "field_1480": dict(field_1480=1),
        "field_3e4": dict(field_3e4=1),
        "oid29_threshold": dict(oid=29, count=24),
        "oid29_over_threshold": dict(oid=29, count=25),
        "ordinary_threshold": dict(oid=7, count=8),
        "ordinary_over_threshold": dict(oid=7, count=9),
        "load_failed": dict(load=0),
        "not_monster": dict(load=1, monster=0),
        "mini_boss": dict(load=1, monster=1, mini=0x80),
        "boss": dict(load=1, monster=1, mini=0, boss=1),
        "online_byte": dict(load=1, monster=1, online=0x80),
        "field_3ec_zero": dict(load=1, monster=1, field_3ec=0),
        "timestamp_high_bit": dict(load=1, monster=1, timestamp=0xF0000000),
        "timestamp_duplicate": dict(load=1, monster=1, count=1, map_key=100, timestamp=100),
        "timestamp_insert_after_existing": dict(load=1, monster=1, count=1, map_key=-100, timestamp=200),
        "oid29_non_special_level": dict(oid=28, count=9),
    }


def u32(value: int) -> int:
    return value & 0xFFFFFFFF


def i32(value: int) -> int:
    value = u32(value)
    return value if value < 0x80000000 else value - 0x100000000


def verify_manifest(engine: Path):
    from elftools.elf.elffile import ELFFile
    raw = engine.read_bytes()
    with engine.open("rb") as stream:
        elf = ELFFile(stream)
        symbols = {s.name: s for s in elf.get_section_by_name(".symtab").iter_symbols()}
        loads = [s for s in elf.iter_segments() if s["p_type"] == "PT_LOAD"]

        def read(address, size):
            segment = next(x for x in loads if x["p_vaddr"] <= address and
                           address + size <= x["p_vaddr"] + x["p_filesz"])
            offset = int(segment["p_offset"]) + address - int(segment["p_vaddr"])
            return raw[offset:offset + size]

        for row in json.loads(MANIFEST.read_text(encoding="utf-8"))["functions"]:
            symbol = symbols[row["original_symbol"]]
            address, size = int(row["elf_address"], 0), row["size"]
            assert (int(symbol["st_value"]), int(symbol["st_size"])) == (address, size)
            assert hashlib.sha256(read(address, size)).hexdigest() == row["sha256"]
        manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
        for row in manifest["supporting_functions"]:
            symbol = symbols[row["original_symbol"]]
            address, size = int(row["elf_address"], 0), row["size"]
            assert (int(symbol["st_value"]), int(symbol["st_size"])) == (address, size), row
            assert hashlib.sha256(read(address, size)).hexdigest() == row["sha256"], row
        for row in manifest["source_ranges"]:
            address, size = int(row["start"], 0), row["size"]
            assert hashlib.sha256(read(address, size)).hexdigest() == row["sha256"], row
    return {"verified_functions": len(manifest["functions"]),
            "verified_supporting_functions": len(manifest["supporting_functions"]),
            "verified_source_ranges": len(manifest["source_ranges"])}


class ArmOracle:
    def __init__(self, engine: Path):
        from unicorn import UC_HOOK_CODE
        from unicorn.arm_const import (UC_ARM_REG_R4, UC_ARM_REG_R5, UC_ARM_REG_R8,
                                      UC_ARM_REG_SP, UC_ARM_REG_LR, UC_ARM_REG_PC)
        sys.path.insert(0, str(ROOT / "port/engine-resources/tests"))
        from cpu import Cpu
        self.Cpu = Cpu
        self.reg_r4, self.reg_r5, self.reg_r8 = UC_ARM_REG_R4, UC_ARM_REG_R5, UC_ARM_REG_R8
        self.reg_sp, self.reg_lr, self.reg_pc = UC_ARM_REG_SP, UC_ARM_REG_LR, UC_ARM_REG_PC
        self.cpu = Cpu(engine, False, json.loads(MANIFEST.read_text(encoding="utf-8")))
        self.cpu.uc.hook_add(UC_HOOK_CODE, self.hook)
        self.events = []
        self.visited = set()
        self.coverage = set()
        self.cfg = {}
        self.node_counter = 0

    def word(self, address):
        return struct.unpack("<I", self.cpu.uc.mem_read(address, 4))[0]

    def put_word(self, address, value):
        self.cpu.pointer(address, u32(value))

    def put_byte(self, address, value):
        self.cpu.uc.mem_write(address, bytes((value & 255,)))

    def ret(self, value=0):
        c = self.cpu
        c.put(0, value)
        c.uc.reg_write(self.reg_pc, c.uc.reg_read(self.reg_lr))

    def _new_node(self, key, parent, left=False):
        c = self.cpu
        address = NEW_NODE + self.node_counter * 0x40
        self.node_counter += 1
        c.uc.mem_write(address, bytes(0x20))
        self.put_word(address + 4, parent)
        self.put_word(address + 16, key)
        self.put_word(address + 20, 0)
        if parent == MAP:
            self.put_word(MAP + 4, address)
            self.put_word(MAP + 8, address)
            self.put_word(MAP + 12, address)
        else:
            self.put_word(parent + (8 if left else 12), address)
            if left and self.word(MAP + 8) == parent:
                self.put_word(MAP + 8, address)
            if not left and self.word(MAP + 12) == parent:
                self.put_word(MAP + 12, address)
        self.put_word(MAP + 16, self.word(MAP + 16) + 1)
        return address

    def hook(self, uc, address, size, unused):
        c = self.cpu
        if 0x3AC34C <= address <= 0x3AC570 or 0x3ACA48 <= address <= 0x3ACCF4:
            self.visited.add(address)
            self.coverage.add(address)
        if address in EXITS:
            uc.emu_stop()
            return
        if address == 0x3AA8B4:  # _M_insert_unique; allocator/tree mechanics are a fixture service.
            out, pair = c.reg(0), c.reg(1)
            key = self.word(pair)
            node = self._new_node(key, MAP, False)
            self.put_word(out, node)
            self.put_word(out + 4, 1)
            self.ret()
            return
        if address == 0x3AA77C:  # _M_insert; parent/side are source call arguments.
            out, parent, pair, left = c.reg(0), c.reg(1), c.reg(2), c.reg(3)
            node = self._new_node(self.word(pair), parent, bool(left))
            self.put_word(out, node)
            self.ret()
            return
        if address not in OPS:
            return
        op = OPS[address]
        self.events.append(op)
        cfg = self.cfg
        if op == 0:
            self.ret(LEVEL)
        elif op == 1:
            assert c.reg(0) == CONTROLLER and c.reg(1) == 0 and c.reg(2) == 1
            self.ret()
        elif op == 2:
            victim = c.reg(0)
            iterator = self.word(c.reg(1))
            assert victim == VICTIM and iterator == NODE and c.reg(2) == 1
            self.put_word(MAP + 4, 0)
            self.put_word(MAP + 8, MAP)
            self.put_word(MAP + 12, MAP)
            self.put_word(MAP + 16, max(0, self.word(MAP + 16) - 1))
            self.ret()
        elif op == 3:
            assert c.reg(0) == CURRENT + 0x3C8 and c.reg(1) == 1
            self.ret(cfg.get("load", 0))
        elif op == 4:
            self.ret(cfg.get("monster", 1))
        elif op == 5:
            self.ret(cfg.get("mini", 0))
        elif op == 6:
            self.ret(cfg.get("boss", 0))
        elif op == 7:
            self.ret(ONLINE)
        elif op == 8:
            self.ret(cfg.get("timestamp", 100))

    def execute(self, name, row):
        c = self.cpu
        cfg = dict(oid=7, count=0, map_key=-100, state=1, field_1480=0,
                   field_3e4=0, field_3ec=1, load=0, monster=1, mini=0,
                   boss=0, online=0, timestamp=100)
        cfg.update(row)
        self.cfg = cfg
        self.events = []
        self.visited = set()
        self.node_counter = 0
        c.uc.mem_write(DATA, bytes(0x10000))
        self.put_word(0x995BCC, MAP)
        self.put_word(0x99828C, DATA + 0x9000)
        self.put_word(MAP + 4, 0)
        self.put_word(MAP + 8, MAP)
        self.put_word(MAP + 12, MAP)
        self.put_word(MAP + 16, cfg["count"])

        self.put_word(CURRENT + 0x378, CONTROLLER)
        self.put_word(CURRENT + 0x3C8, CURRENT + 0x3C8)
        self.put_word(CURRENT + 0x51C, DATA + 0x5100)
        self.put_word(DATA + 0x5100, cfg["state"])
        self.put_word(CURRENT + 0x3E4, cfg["field_3e4"])
        self.put_byte(CURRENT + 0x1480, cfg["field_1480"])
        self.put_byte(CURRENT + 0x3EC, cfg["field_3ec"])
        self.put_word(LEVEL + 0x3C, cfg["oid"])
        self.put_byte(ONLINE + 5, cfg["online"])

        if cfg["count"]:
            c.uc.mem_write(NODE, bytes(0x20))
            self.put_word(NODE + 4, MAP)
            self.put_word(NODE + 16, cfg["map_key"])
            self.put_word(NODE + 20, VICTIM)
            self.put_word(MAP + 4, NODE)
            self.put_word(MAP + 8, NODE)
            self.put_word(MAP + 12, NODE)
            self.put_word(VICTIM + 0x378, CONTROLLER)
            self.put_word(VICTIM + 0x3C8, VICTIM + 0x3C8)

        c.uc.reg_write(self.reg_r4, CURRENT)
        c.uc.reg_write(self.reg_r5, GOT)
        c.uc.reg_write(self.reg_r8, CURRENT + 0x4FC)
        c.put(0, CURRENT + 0x4FC)
        entry_sp = c.stack + 0xE000
        c.uc.reg_write(self.reg_sp, entry_sp)
        c.uc.reg_write(self.reg_lr, c.stop)
        c.uc.reg_write(self.reg_pc, 0x3AC34C)
        try:
            c.uc.emu_start(0x3AC34C, c.stop, count=100000)
        except Exception:
            print("ARM oracle failed", name, "pc", hex(c.uc.reg_read(self.reg_pc)),
                  "r0-r5", [hex(c.reg(i)) for i in range(6)],
                  "r8", hex(c.uc.reg_read(self.reg_r8)),
                  "fsm_word", hex(self.word(CURRENT + 0x51C)),
                  "state_word", hex(self.word(DATA + 0x5100)),
                  "calls", self.events, file=sys.stderr)
            raise

        root = self.word(MAP + 4)
        entries = []
        def visit(node, seen):
            if node in (0, MAP):
                return
            if node in seen:
                raise AssertionError("source map fixture contains a node cycle")
            seen.add(node)
            visit(self.word(node + 8), seen)
            value = self.word(node + 20)
            entries.append([i32(self.word(node + 16)),
                            0x1000 if value == CURRENT else
                            0x10000 if value == VICTIM else 0])
            visit(self.word(node + 12), seen)
        if root != 0:
            visit(root, set())
        return {
            "calls": self.events,
            "service_calls": len(self.events),
            "map_size": self.word(MAP + 16),
            "entries": entries,
            "eviction_attempted": int(1 in self.events),
            "evicted_key": i32(self.word(NODE + 16)) if 1 in self.events else 0,
            "evicted_character": 0x10000 if 1 in self.events else 0,
            "timestamp_word": cfg["timestamp"] if 8 in self.events else 0,
            "timestamp_key": i32(cfg["timestamp"]) if 8 in self.events else 0,
            "visited": sorted(self.visited),
        }


def source_arm_comparison(engine: Path, host_rows: list[dict]):
    oracle = ArmOracle(engine)
    rows = fixtures()
    comparisons = []
    for host in host_rows:
        name = host["name"]
        got = oracle.execute(name, rows[name])
        expected_calls = host["calls"]
        assert got["calls"] == expected_calls, (name, got["calls"], expected_calls)
        assert got["service_calls"] == host["service_calls"], name
        assert got["map_size"] == host["map_size"], (name, got["map_size"], host["map_size"])
        assert got["eviction_attempted"] == host["eviction_attempted"], name
        if got["eviction_attempted"]:
            assert got["evicted_key"] == host["evicted_key"], name
            assert got["evicted_character"] == host["evicted_character"], name
        if 8 in got["calls"]:
            assert got["timestamp_word"] == host["timestamp_word"], name
            assert got["timestamp_key"] == host["timestamp_key"], name
        if name in ("timestamp_high_bit", "timestamp_duplicate",
                    "timestamp_insert_after_existing"):
            assert got["entries"] == host["entries"], (name, got["entries"], host["entries"])
        if 0 in got["calls"]:
            threshold = 24 if rows[name].get("oid", 7) == 29 else 8
            assert host["eviction_threshold"] == threshold, name
        comparisons.append({"case": name, "matched": True,
                            "executed_source_ops": got["calls"],
                            "original_block_pcs": [hex(x) for x in got["visited"]]})
    visited = set(oracle.coverage)
    required = {0x3AC34C, 0x3AC3A8, 0x3ACA48, 0x3ACA54,
                0x3AC404, 0x3AC440, 0x3AC45C, 0x3AC56C}
    if not required.issubset(visited):
        raise AssertionError("required source branch instructions not reached: " +
                             repr(sorted(hex(x) for x in required - visited)))
    return {"validation": "PASS", "comparisons": len(comparisons),
            "mismatches": 0, "source_block": "0x3ac34c..0x3ac570",
            "reached_special_oid_tail": ["0x3aca48", "0x3aca54"],
            "source_instruction_coverage": len(visited),
            "calls_and_visible_effects_matched": True,
            "allocator_and_tree_helpers": "intercepted fixtures; original scheduler branch instructions execute",
            "cases": comparisons}


def main() -> int:
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--original-elf", type=Path,
                   default=ROOT.parent / "test_strategy/libDungeonHunter2.so")
    p.add_argument("--compiler", help="host C++17 compiler")
    p.add_argument("--output", type=Path,
                   default=MODULE / "build/character-update-script-scheduler/host.exe")
    p.add_argument("--report", type=Path,
                   default=MODULE / "build/character-update-script-scheduler/validation.json")
    args = p.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        p.error("no host C++ compiler found")
    if sha(args.original_elf) != ORIGINAL_SHA:
        raise SystemExit("original ELF SHA-256 mismatch")
    manifest_validation = verify_manifest(args.original_elf.resolve())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    sources = [MODULE / "character_update_script_scheduler.cpp",
               MODULE / "tests/character_update_script_scheduler.cpp"]
    command = [compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror",
               "-pedantic", *map(str, sources), "-o", str(args.output.resolve())]
    build = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    if build.returncode:
        sys.stderr.write(build.stdout + build.stderr)
        return build.returncode
    host = subprocess.run([str(args.output.resolve())], cwd=ROOT,
                          capture_output=True, text=True)
    if host.returncode:
        sys.stderr.write(host.stdout + host.stderr)
        return host.returncode
    if ("scheduler_guard_cases=15" not in host.stderr or
            "scheduler_effect_cases=3" not in host.stderr):
        raise AssertionError("scheduler host guard/effect suites did not report expected counts")
    host_rows = [json.loads(line) for line in host.stdout.splitlines() if line.startswith("{")]
    if len(host_rows) != 18:
        raise AssertionError(f"expected 18 host source cases, got {len(host_rows)}")
    comparison = source_arm_comparison(args.original_elf.resolve(), host_rows)
    owned = [MODULE / "character_update_script_scheduler.hpp", *sources,
             Path(__file__).resolve(), MANIFEST,
             MODULE / "reference/character-update-script-scheduler/NOTES.md"]
    report = {
        "validation": "PASS",
        "host_cases": len(host_rows),
        "host_guard_cases": 15,
        "host_effect_cases": 3,
        "original_arm_comparison": comparison,
        "original_sha256": sha(args.original_elf),
        "manifest_validation": manifest_validation,
        "compiler_command": command,
        "source_sha256": {str(x.relative_to(ROOT)).replace("\\", "/"): sha(x) for x in owned},
        "complete_original_functions": 0,
        "native_wired": False,
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({k: v for k, v in report.items()
                      if k not in ("compiler_command", "original_arm_comparison")}, indent=2))
    print(json.dumps({"source_arm_comparison": comparison}, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
