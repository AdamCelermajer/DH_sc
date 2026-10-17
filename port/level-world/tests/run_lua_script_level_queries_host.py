#!/usr/bin/env python3
"""Host checks and original ARM caller replay for three LuaScript level queries.

The original ARM32 callback bodies execute from the pinned game ELF. Only the
external current-level/hosting-player providers and ReturnValues push boundary
are intercepted. Numeric Value::getNumber executes from the original image
for the exercised number case. The f2iz import identity is independently
checked by executing its relocated PLT stub; finite numerical conversion is
modeled as a bounded dependency in the caller replay.
"""
from __future__ import annotations

import argparse
import ctypes as c
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import struct
import subprocess
import sys

from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R4

ROOT = Path(__file__).resolve().parents[1]
REPO = ROOT.parents[1]
PROJECT = ROOT.parents[2]
PINNED_ELF_SHA256 = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"
ROWS = [
    (0x37CC00, 60, "_ZN9LuaScript19_GetHostPlayerLevelERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv"),
    (0x37CB8C, 76, "_ZN9LuaScript24_GetHostPlayerDifficultyERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv"),
    (0x37F1F0, 356, "_ZN9LuaScript21_GetCurrentLevelRangeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv"),
]
SUPPORT_ROWS = [
    (0x31F594, 32, "_ZNK11Application15GetCurrentLevelEv"),
    (0x36E09C, 112, "_ZN13PlayerManager16GetHostingPlayerEv"),
    (0x31BBF0, 144, "_ZNK3sfc6script3lua5Value9getNumberEv"),
    (0x37CB24, 104, "_ZN3sfc6script3lua12ReturnValues11pushIntegerEi"),
]


def load_engine_cpu():
    path = ROOT.parents[0] / "animation-values" / "tests" / "differential.py"
    spec = importlib.util.spec_from_file_location("lua_level_animation_cpu", path)
    module = importlib.util.module_from_spec(spec)
    assert spec and spec.loader
    spec.loader.exec_module(module)
    return module.EngineCpu


class Dependencies:
    def call(self, cpu, name: str) -> None:
        if name == "__aeabi_f2iz":
            raw = cpu.reg(0)
            number = struct.unpack("<f", struct.pack("<I", raw))[0]
            # Test corpus excludes NaN/infinity and out-of-range values; those
            # are outside this checked source-query slice.
            assert -2147483648.0 <= number < 2147483648.0
            cpu.write_reg(0, int(number) & 0xFFFFFFFF)
        elif name in ("memcpy", "memmove", "__aeabi_memcpy", "__aeabi_memcpy4"):
            dst, src, size = (cpu.reg(i) for i in range(3))
            cpu.uc.mem_write(dst, bytes(cpu.uc.mem_read(src, size)))
            cpu.write_reg(0, dst)
        else:
            raise AssertionError(f"unmodeled original import executed: {name}")
        cpu.uc.reg_write(cpu.pc_reg, cpu.uc.reg_read(cpu.lr_reg))


def evidence_for(path: Path, rows):
    output = []
    with path.open("rb") as stream:
        elf = ELFFile(stream)
        loads = [s for s in elf.iter_segments() if s["p_type"] == "PT_LOAD"]
        symbols = {}
        for section_name in (".dynsym", ".symtab"):
            section = elf.get_section_by_name(section_name)
            if section:
                for symbol in section.iter_symbols():
                    if symbol["st_shndx"] != "SHN_UNDEF":
                        symbols.setdefault(symbol["st_value"], symbol.name)
        for address, size, expected_name in rows:
            segment = next(s for s in loads if s["p_vaddr"] <= address and
                           address + size <= s["p_vaddr"] + s["p_filesz"])
            stream.seek(segment["p_offset"] + address - segment["p_vaddr"])
            raw = stream.read(size)
            assert len(raw) == size
            assert symbols.get(address) == expected_name, (hex(address), symbols.get(address), expected_name)
            output.append({"elf_address": f"0x{address:08x}", "size": size,
                           "original_symbol": expected_name,
                           "sha256": hashlib.sha256(raw).hexdigest()})
    return output


class SourceReplay:
    def __init__(self, original: Path, rows):
        self.EngineCpu = load_engine_cpu()
        self.cpu = self.EngineCpu(original, False, Dependencies(), {"functions": rows})
        self.level = self.cpu.data + 0x1000
        self.player = self.cpu.data + 0x2000
        self.manager = self.cpu.data + 0x3000
        self.returns = self.cpu.data + 0x4000
        self.arguments = self.cpu.data + 0x5000
        self.vector = self.cpu.data + 0x6000
        self.values = self.cpu.data + 0x7000
        self.table_object = self.cpu.data + 0x8000
        self.rows_a = self.cpu.data + 0x9000
        self.rows_b = self.cpu.data + 0xA000
        self.calls = []
        self.pushed = []
        self.mutate_rows_after_first_push = False
        self._install_hooks()

    def _word(self, address, value):
        self.cpu.uc.mem_write(address, struct.pack("<I", value & 0xFFFFFFFF))

    def _install_hooks(self):
        def intercept(uc, address, size, unused):
            if address == 0x31F594:  # current-level getter is a borrowed game service
                self.calls.append("GetCurrentLevel")
                self.cpu.write_reg(0, self.level)
                uc.reg_write(self.cpu.pc_reg, uc.reg_read(self.cpu.lr_reg))
            elif address == 0x36E09C:  # hosting-player selector is a borrowed service
                self.calls.append("GetHostingPlayer")
                assert self.cpu.reg(0) == self.manager
                self.cpu.write_reg(0, self.player)
                uc.reg_write(self.cpu.pc_reg, uc.reg_read(self.cpu.lr_reg))
            elif address in (0x37F25C, 0x37F2C8, 0x37F304):
                # Preserve source table capture but route its selected object
                # to the mapped test object before the source reads [table].
                uc.reg_write(UC_ARM_REG_R4, self.table_object)
            elif address == 0x37CB24:
                self.calls.append("pushInteger")
                value = c.c_int32(self.cpu.reg(1)).value
                self.pushed.append(value)
                if self.mutate_rows_after_first_push and len(self.pushed) == 1:
                    self._word(self.table_object, self.rows_b)
                uc.reg_write(self.cpu.pc_reg, uc.reg_read(self.cpu.lr_reg))

        self.cpu.uc.hook_add(UC_HOOK_CODE, intercept, begin=0x31F594, end=0x37F304)

    def reset(self):
        self.calls.clear()
        self.pushed.clear()
        self._word(self.table_object, self.rows_a)

    def query_host_level(self, player_level: int):
        self.reset()
        # Singleton<Application> is statically allocated at 0x99f72c; its
        # +0x40 slot supplies PlayerManager to the real source callback.
        self._word(0x99F72C + 0x40, self.manager)
        self._word(self.player + 0x330, player_level)
        self.cpu.call(0x37CC00, [0, self.returns, 0])
        return list(self.pushed)

    def query_difficulty(self, level_ptr: int | None, difficulty: int = 0):
        self.reset()
        self.level = level_ptr or 0
        if level_ptr:
            self._word(self.level + 0x118, difficulty)
        self.cpu.call(0x37CB8C, [0, self.returns, 0])
        return list(self.pushed)

    def query_range(self, oid, arg_type, arg_number, values, mutate_storage=False):
        self.reset()
        self.level = self.cpu.data + 0x1000
        self._word(self.level + 0x3C, oid)
        if oid == -1:
            # The source sentinel returns before touching arguments or the
            # level table. Keep those mappings deliberately unused here.
            self.mutate_rows_after_first_push = False
            self.cpu.call(0x37F1F0, [self.arguments, self.returns, 0])
            return list(self.pushed)
        row_offset = ((oid & 0xFFFFFFFF) * 72) & 0xFFFFFFFF
        self.mutate_rows_after_first_push = mutate_storage
        for address in (self.rows_a, self.rows_b):
            self.cpu.uc.mem_write(address, b"\0" * 0x100)
        # Both source tables are row arrays. Place the selected record at the
        # row selected by the captured source OID.
        for base, (minimum, maximum) in values.items():
            for index, offset in enumerate((0x3C, 0x40, 0x44)):
                self._word(base + row_offset + offset, minimum + index * 100)
            for index, offset in enumerate((0x30, 0x34, 0x38)):
                self._word(base + row_offset + offset, maximum + index * 100)
        self._word(self.table_object, self.rows_a)
        self.cpu.uc.mem_write(self.arguments, b"\0" * 0x20)
        self._word(self.arguments + 4, self.vector)
        if arg_type:
            self.cpu.uc.mem_write(self.vector, struct.pack("<II", self.values, self.values + 16))
            self.cpu.uc.mem_write(self.values, struct.pack("<II f II", 0, arg_type,
                                                           float(arg_number), 0, 0))
        else:
            self.cpu.uc.mem_write(self.vector, struct.pack("<II", self.values, self.values))
        self.cpu.call(0x37F1F0, [self.arguments, self.returns, 0])
        return list(self.pushed)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--original", type=Path,
                        default=PROJECT / "test_strategy" / "libDungeonHunter2.so")
    parser.add_argument("--compiler", type=Path,
                        default=Path("C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe"))
    parser.add_argument("--output", type=Path,
                        default=ROOT / "build" / "lua-script-level-queries")
    parser.add_argument("--report", type=Path)
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    original_sha = hashlib.sha256(args.original.read_bytes()).hexdigest()
    assert original_sha == PINNED_ELF_SHA256, "unexpected source ELF"
    sys.path.insert(0, str(ROOT / "tests"))
    from elf_import_identity import verify_imports
    verified_imports = verify_imports(args.original, {0x30E4CC: "__aeabi_f2iz"})

    host_exe = args.output / "host.exe"
    host_dll = args.output / "host-probe.dll"
    source = ROOT / "lua_script_level_queries.cpp"
    tests = ROOT / "tests" / "lua_script_level_queries.cpp"
    common = [str(args.compiler), "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror", "-pedantic",
              str(source), str(tests)]
    subprocess.run(common + ["-o", str(host_exe)], check=True)
    subprocess.run(common + ["-shared", "-static-libgcc", "-static-libstdc++", "-o", str(host_dll)], check=True)
    host_summary = subprocess.run([str(host_exe)], check=True, text=True, capture_output=True)
    host_report = json.loads(host_summary.stdout)
    assert host_report["mismatches"] == 0 and host_report["lua_script_level_query_cases"] >= 17

    dll_directory = os.add_dll_directory(str(args.compiler.parent))
    probe = c.CDLL(str(host_dll.resolve()))
    probe.dh2_lua_script_range_probe.argtypes = [c.c_int32, c.c_uint32, c.c_float,
        c.c_int32, c.c_int32, c.c_int32, c.c_int32, c.c_uint32,
        c.POINTER(c.c_int32), c.POINTER(c.c_uint32)]
    probe.dh2_lua_script_range_probe.restype = c.c_uint32
    probe.dh2_lua_script_host_level_probe.argtypes = [c.c_int32, c.POINTER(c.c_int32)]
    probe.dh2_lua_script_host_level_probe.restype = c.c_uint32

    evidence = evidence_for(args.original, ROWS)
    support_evidence = evidence_for(args.original, SUPPORT_ROWS)
    machine = SourceReplay(args.original, evidence + support_evidence)
    comparisons = []

    def range_case(name, oid, arg_type, number, store_a, store_b, mutate):
        oracle_values = {machine.rows_a: store_a, machine.rows_b: store_b}
        expected = machine.query_range(oid, arg_type, number, oracle_values, mutate)
        host_values = (c.c_int32 * 2)()
        host_count = c.c_uint32()
        status = probe.dh2_lua_script_range_probe(oid, arg_type, number,
            store_a[0], store_a[1], store_b[0], store_b[1], int(mutate),
            host_values, c.byref(host_count))
        assert status == 0
        actual = list(host_values[:host_count.value])
        assert actual == expected, (name, actual, expected)
        comparisons.append({"case": name, "outputs": actual, "mismatches": 0})

    range_case("normal-and-fresh-backing-after-push", 0, 0, 0.0, (11, 21), (12, 22), True)
    range_case("nonzero-level-row-index", 7, 0, 0.0, (23, 33), (24, 34), False)
    range_case("numeric-hard-from-original-getNumber-and-f2iz", 0, 3, 1.9, (31, 41), (31, 41), False)
    range_case("non-number-defaults-to-normal", 0, 4, 9.0, (51, 61), (51, 61), False)
    range_case("unsupported-numeric-difficulty-pushes-nothing", 0, 3, 3.0, (71, 81), (71, 81), False)
    range_case("sentinel-oid-bypasses-table-and-arguments", -1, 4, 7.0, (91, 101), (91, 101), False)

    for value in (0, 1, 42, 0x7FFFFFFF, -4):
        oracle = machine.query_host_level(value)
        host_value = c.c_int32()
        status = probe.dh2_lua_script_host_level_probe(value, c.byref(host_value))
        assert status == 0 and oracle == [value] and host_value.value == oracle[0]
        comparisons.append({"case": f"host-player-level-{value}", "outputs": oracle, "mismatches": 0})

    for level_exists, difficulty in ((True, 0), (True, 2), (True, -1), (False, 0)):
        oracle = machine.query_difficulty(machine.level if level_exists else None, difficulty)
        expected = [difficulty if level_exists else 0]
        assert oracle == expected, (level_exists, difficulty, oracle)
        comparisons.append({"case": f"current-level-difficulty-{level_exists}-{difficulty}",
                            "outputs": oracle, "mismatches": 0})

    report = {
        "complete_game": False,
        "native_wired": False,
        "comparison": "Original ARM32 LuaScript caller outputs versus the compiled host source kernel on identical typed query inputs",
        "original_sha256": original_sha,
        "host_executable_sha256": hashlib.sha256(host_exe.read_bytes()).hexdigest(),
        "host_probe_sha256": hashlib.sha256(host_dll.read_bytes()).hexdigest(),
        "source_sha256": {str(p.relative_to(REPO)).replace("\\", "/"): hashlib.sha256(p.read_bytes()).hexdigest()
                          for p in (source, ROOT / "lua_script_level_queries.hpp", tests,
                                    ROOT / "tests" / "run_lua_script_level_queries_host.py")},
        "functions": evidence,
        "supporting_functions": support_evidence,
        "verified_imports": verified_imports,
        "host_report": host_report,
        "comparisons": comparisons,
        "comparison_count": len(comparisons),
        "mismatches": 0,
        "provider_boundaries": ["Application::GetCurrentLevel", "PlayerManager::GetHostingPlayer", "ReturnValues::pushInteger", "global LevelTable object/backing pointer"],
        "actual_original_bodies_executed": ["three LuaScript callers", "Value::getNumber for numeric case", "relocated PLT stub identity for __aeabi_f2iz"],
        "import_model": {"__aeabi_f2iz": "identity independently executed; bounded finite signed conversion modeled by host dependency handler", "other_imports": machine.cpu.import_calls},
        "limitations": ["No complete LuaScript VM/gameplay execution claim.", "Missing Level, host Player, or LevelTable service results fail closed in the port adapter; the source callbacks themselves do not all have null guards.", "Level+0x3c/+0x118 and Player+0x330 are consumed fields; their live native producers remain a separate unresolved integration boundary."],
    }
    report_path = args.report or args.output / "validation.json"
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(report, separators=(",", ":")))
    dll_directory.close()


if __name__ == "__main__":
    main()
