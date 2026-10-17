#!/usr/bin/env python3
"""Host test and original ARM32 replay for Level constructor row selection.

The runner executes only the original Level C1/C2 row-scan spans and the full
ToLowerCase helper. Imported strcpy/strlen/strstr libc calls are bounded host
dependencies. The remaining Level constructor work is outside this slice.
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
from unicorn.arm_const import (UC_ARM_REG_R4, UC_ARM_REG_R5, UC_ARM_REG_R6,
    UC_ARM_REG_R9, UC_ARM_REG_R11)

ROOT = Path(__file__).resolve().parents[3]
PROJECT = ROOT.parent
PINNED_ELF_SHA256 = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"
FUNCTIONS = [
    (0x3F3128, 920, "_ZN5LevelC1EPKcijjjbbii"),
    (0x3F34C0, 920, "_ZN5LevelC2EPKcijjjbbii"),
    (0x34E414, 108, "_Z11ToLowerCasePcii"),
]
CALL_CHAIN_FUNCTIONS = [
    (0x386190, 328, "_ZN7GSLevel4CtorEPK12StateMachine"),
    (0x386818, 160, "_ZN7GSLevel9LoadLevelEPKcijjjbbii"),
]


def load_cpu_type():
    source = ROOT / "port/engine-math/tests/differential.py"
    spec = importlib.util.spec_from_file_location("level_construction_engine_cpu", source)
    module = importlib.util.module_from_spec(spec)
    assert spec and spec.loader
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module.Cpu


def read_c_string(cpu, address, limit=0x2000):
    result = bytearray()
    for offset in range(limit):
        value = cpu.uc.mem_read(address + offset, 1)[0]
        if value == 0:
            return bytes(result)
        result.append(value)
    raise AssertionError(f"unterminated C string at {address:#x}")


class Dependencies:
    def call(self, cpu, name):
        if name == "strcpy":
            destination, source = cpu.reg(0), cpu.reg(1)
            value = read_c_string(cpu, source, 1024)
            assert len(value) < 1024, "fixture would overflow Level's local buffer"
            cpu.uc.mem_write(destination, value + b"\0")
            cpu.write_reg(0, destination)
        elif name == "strlen":
            cpu.write_reg(0, len(read_c_string(cpu, cpu.reg(0), 0x2000)))
        elif name == "strstr":
            haystack, needle = cpu.reg(0), cpu.reg(1)
            hay = read_c_string(cpu, haystack, 0x2000)
            sought = read_c_string(cpu, needle, 1024)
            offset = hay.find(sought)
            cpu.write_reg(0, 0 if offset < 0 else haystack + offset)
        else:
            raise AssertionError(f"unmodeled original import executed: {name}")
        cpu.uc.reg_write(cpu.pc_reg, cpu.uc.reg_read(cpu.lr_reg))


def function_evidence_for(original: Path, functions):
    evidence = []
    with original.open("rb") as stream:
        elf = ELFFile(stream)
        loads = [segment for segment in elf.iter_segments() if segment["p_type"] == "PT_LOAD"]
        symbols = {}
        for section_name in (".dynsym", ".symtab"):
            section = elf.get_section_by_name(section_name)
            if section:
                for symbol in section.iter_symbols():
                    if symbol["st_shndx"] != "SHN_UNDEF":
                        symbols.setdefault(symbol["st_value"], symbol.name)
        for address, size, expected_symbol in functions:
            segment = next(s for s in loads if s["p_vaddr"] <= address and
                           address + size <= s["p_vaddr"] + s["p_filesz"])
            stream.seek(segment["p_offset"] + address - segment["p_vaddr"])
            raw = stream.read(size)
            assert len(raw) == size and symbols.get(address) == expected_symbol
            evidence.append({"elf_address": f"0x{address:08x}", "size": size,
                             "original_symbol": expected_symbol,
                             "sha256": hashlib.sha256(raw).hexdigest()})
    return evidence


def function_evidence(original: Path):
    return function_evidence_for(original, FUNCTIONS)


class SourceScan:
    def __init__(self, original, evidence):
        Cpu = load_cpu_type()
        self.cpu = Cpu(original, False, Dependencies(), {"functions": evidence})
        self.data = self.cpu.data
        self.cpu.uc.mem_map(self.data + 0x10000, 0x100000)
        self.level_list_size = self.cpu.symbols["_ZN6Arrays9LevelList4sizeE"]
        self.level_list_members = self.cpu.symbols["_ZN6Arrays9LevelList7membersE"]
        self.module_id = self.cpu.symbols["_ZN6Module10s_moduleIdE"]
        self.got = self._word(0x3F3498) + 0x3F3150
        assert self.got == 0x994A98
        # C1's source PC-relative literals point to these two GOT slots. The
        # loader's GLOB_DAT relocation is supplied explicitly in this fixture.
        self._put_word(self.got + 0x18C0, self.level_list_size)
        self._put_word(self.got + 0x282C, self.module_id)
        self._put_word(self.got + 0x874, self.level_list_members)

    def _word(self, address):
        return struct.unpack("<I", self.cpu.uc.mem_read(address, 4))[0]

    def _put_word(self, address, value):
        self.cpu.uc.mem_write(address, struct.pack("<I", value & 0xFFFFFFFF))

    def replay(self, rows, filename, difficulty, variant=1):
        assert variant in (1, 2)
        level = self.data + 0x1000
        row_base = self.data + 0x4000
        string_base = self.data + 0x10000
        filename_address = self.data + 0x3000
        row_bytes = bytearray(max(1, len(rows)) * 0x48)
        for index, row in enumerate(rows):
            raw = row["level_file"].encode("utf-8")
            assert b"\0" not in raw and len(raw) < 1024
            string_address = string_base + index * 0x400
            self.cpu.uc.mem_write(string_address, raw + b"\0")
            offset = index * 0x48
            struct.pack_into("<I", row_bytes, offset + 0x10, row["hub"] & 0xFFFFFFFF)
            row_bytes[offset + 0x14] = 1 if row["is_random"] else 0
            struct.pack_into("<I", row_bytes, offset + 0x20, string_address)
        self.cpu.uc.mem_write(row_base, bytes(row_bytes))
        raw_filename = filename.encode("utf-8")
        assert b"\0" not in raw_filename and len(raw_filename) < 0x1000
        self.cpu.uc.mem_write(filename_address, raw_filename + b"\0")
        self._put_word(self.level_list_size, len(rows))
        self._put_word(self.level_list_members, row_base)
        self._put_word(level + 0x10C, filename_address)
        self._put_word(level + 0x3C, 0xFFFFFFFF)
        self._put_word(level + 0x40, 0xFFFFFFFF)
        self._put_word(level + 0x118, difficulty)
        self.cpu.uc.mem_write(level + 0xE8, b"\x7f")

        # Enter immediately before the C1/C2 constructor's actual scan. This
        # avoids invoking unrelated Lua, online, allocation, and save systems.
        if variant == 1:
            start, stop = 0x3F32CC, 0x3F3370
            fp_offset = 0x18C0
        else:
            start, stop = 0x3F3664, 0x3F3708
            fp_offset = 0x18C0
        sp = self.cpu.stack + 0xE000
        self.cpu.uc.reg_write(self.cpu.sp_reg, sp)
        self.cpu.uc.reg_write(self.cpu.lr_reg, self.cpu.stop)
        self.cpu.uc.reg_write(UC_ARM_REG_R4, level)
        self.cpu.uc.reg_write(UC_ARM_REG_R5, 0xFFFFFFFF)
        self.cpu.uc.reg_write(UC_ARM_REG_R6, 0)
        self.cpu.uc.reg_write(UC_ARM_REG_R9, self.got)
        self.cpu.uc.reg_write(UC_ARM_REG_R11, fp_offset)
        self.cpu.uc.emu_start(start, stop, count=100000)
        assert self.cpu.uc.reg_read(self.cpu.pc_reg) == stop
        return {
            "index": c.c_int32(self._word(level + 0x3C)).value,
            "hub": c.c_int32(self._word(level + 0x40)).value,
            "is_random": self.cpu.uc.mem_read(level + 0xE8, 1)[0],
            "difficulty": c.c_int32(self._word(level + 0x118)).value,
        }


class ProbeRow(c.Structure):
    _fields_ = [("level_file", c.c_char_p), ("hub", c.c_int32),
                ("is_random", c.c_uint8), ("reserved", c.c_uint8 * 3)]


class ProbeOutput(c.Structure):
    _fields_ = [("index_3c", c.c_int32), ("hub_40", c.c_int32),
                ("is_random_e8", c.c_uint8), ("reserved", c.c_uint8 * 3),
                ("difficulty_118", c.c_int32), ("status", c.c_uint32),
                ("rows_examined", c.c_uint32)]


def probe_host(dll_path, dll_directory, rows, filename, difficulty):
    runtime_path = os.add_dll_directory(str(dll_directory))
    library = c.CDLL(str(dll_path.resolve()))
    function = library.dh2_level_construction_probe
    function.argtypes = [c.POINTER(ProbeRow), c.c_uint32, c.c_char_p,
                         c.c_int32, c.POINTER(ProbeOutput)]
    function.restype = c.c_uint32
    row_values = (ProbeRow * len(rows))(*[
        ProbeRow(item["level_file"].encode("utf-8"), item["hub"],
                 int(item["is_random"]), (c.c_uint8 * 3)(0, 0, 0)) for item in rows])
    output = ProbeOutput()
    status = function(row_values, len(rows), filename.encode("utf-8"), difficulty,
                      c.byref(output))
    result = {"index": output.index_3c, "hub": output.hub_40,
            "is_random": output.is_random_e8, "difficulty": output.difficulty_118,
            "status": status, "rows_examined": output.rows_examined}
    runtime_path.close()
    return result


def load_catalogue(cache):
    parser_path = ROOT / "port/level-catalogue/catalogue.py"
    spec = importlib.util.spec_from_file_location("level_construction_catalogue", parser_path)
    module = importlib.util.module_from_spec(spec)
    assert spec and spec.loader
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    files = cache / "data/pydata"
    return module.decode_catalogue((files / "levels_pyarray.bin").read_bytes(),
                                   (files / "levels_pyarraynames.bin").read_bytes())


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", type=Path,
                        default=Path("C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe"))
    parser.add_argument("--cache", type=Path, default=ROOT.parent / "cache/files")
    parser.add_argument("--original", type=Path,
                        default=PROJECT / "test_strategy/libDungeonHunter2.so")
    parser.add_argument("--output", type=Path,
                        default=ROOT / "port/level-world/build/level-construction-fields")
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    assert hashlib.sha256(args.original.read_bytes()).hexdigest() == PINNED_ELF_SHA256
    pydata = args.cache / "data/pydata"
    cache_files = [pydata / name for name in (
        "levels_pyarray.bin", "levels_pyarraynames.bin", "levels_pystructnames.bin")]
    assert all(path.is_file() for path in cache_files)

    source = ROOT / "port/level-world/level_construction_fields.cpp"
    header = ROOT / "port/level-world/level_construction_fields.hpp"
    tables = ROOT / "port/game-data/level_tables.cpp"
    test_source = ROOT / "port/level-world/tests/level_construction_fields.cpp"
    probe_source = ROOT / "port/level-world/tests/level_construction_probe.cpp"
    runner_path = Path(__file__).resolve()
    host_exe = args.output / "host.exe"
    host_dll = args.output / "host-probe.dll"
    common = [str(args.compiler), "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror",
              "-pedantic", "-fno-fast-math", "-ffp-contract=off"]
    subprocess.run(common + [str(source), str(tables), str(test_source), "-o", str(host_exe)], check=True)
    subprocess.run(common + ["-shared", "-static-libgcc", "-static-libstdc++",
                   str(source), str(tables), str(probe_source), "-o", str(host_dll)], check=True)
    host_run = subprocess.run([str(host_exe), *map(str, cache_files)], check=True,
                              capture_output=True, text=True)
    host_report = json.loads(host_run.stdout)
    assert host_report["validation"] == "PASS" and host_report["mismatches"] == 0
    catalogue = load_catalogue(args.cache)
    rows = [{"level_file": row.level_file, "hub": row.hub,
             "is_random": row.is_random} for row in catalogue.levels]
    evidence = function_evidence(args.original)
    call_chain_evidence = function_evidence_for(args.original, CALL_CHAIN_FUNCTIONS)
    scan = SourceScan(args.original, evidence)
    cases = [
        {"name": "real-crypt-file", "rows": rows,
         "filename": "007_crypt_01.rule.xml", "difficulty": 2,
         "expected_index": 23},
        {"name": "real-crypt-substring", "rows": rows,
         "filename": "levels/007_crypt_01.rule.xml.backup", "difficulty": -11,
         "expected_index": 23},
        {"name": "real-duplicate-darkwood-first", "rows": rows,
         "filename": "prefix/003_darkwood.mlx.suffix", "difficulty": 4,
         "expected_index": 4},
        {"name": "row-lowercased-haystack-not-lowered", "rows": [
             {"level_file": "CryptA.MLX", "hub": 13, "is_random": True}],
         "filename": "maps/CRYPTA.MLX", "difficulty": 7, "expected_index": -1},
        {"name": "row-lowercased-mixed-source-case", "rows": [
             {"level_file": "CryptA.MLX", "hub": 13, "is_random": True}],
         "filename": "maps/crypta.mlx", "difficulty": 8, "expected_index": 0},
        {"name": "empty-level-list", "rows": [], "filename": "no-match.mlx",
         "difficulty": -5, "expected_index": -1},
        {"name": "empty-row-strstr-semantics", "rows": [
             {"level_file": "", "hub": 27, "is_random": False}],
         "filename": "any-file", "difficulty": 9, "expected_index": 0},
    ]
    comparisons = []
    for case in cases:
        oracle = scan.replay(case["rows"], case["filename"], case["difficulty"], variant=1)
        host = probe_host(host_dll, args.compiler.parent, case["rows"],
                          case["filename"], case["difficulty"])
        expected_index = case["expected_index"]
        assert oracle["index"] == expected_index, (case["name"], oracle)
        assert host["index"] == oracle["index"] and host["hub"] == oracle["hub"] and \
               host["is_random"] == oracle["is_random"] and host["difficulty"] == oracle["difficulty"], \
               (case["name"], host, oracle)
        # C2 is the ABI constructor variant and contains the equivalent scan;
        # execute the same input at its corresponding source span as a second
        # independent code-path check.
        c2 = scan.replay(case["rows"], case["filename"], case["difficulty"], variant=2)
        assert c2 == oracle, (case["name"], c2, oracle)
        comparisons.append({"case": case["name"], "host": host, "original_c1": oracle,
                            "original_c2": c2, "mismatches": 0})

    report = {
        "validation": "PASS", "complete_game": False, "native_wired": False,
        "source_comparison": "compiled host Level field projection versus both original ARM32 constructor scan spans",
        "original_sha256": PINNED_ELF_SHA256,
        "source_sha256": {str(path.relative_to(ROOT)).replace("\\", "/"):
                           hashlib.sha256(path.read_bytes()).hexdigest()
                           for path in (source, header, tables, ROOT / "port/game-data/level_tables.hpp",
                                        test_source, probe_source, runner_path)},
        "cache_sha256": {path.name: hashlib.sha256(path.read_bytes()).hexdigest()
                         for path in cache_files},
        "functions": evidence,
        "call_chain_functions": call_chain_evidence,
        "original_scan_spans": {"C1": ["0x3f32cc", "0x3f3370"],
                                "C2": ["0x3f3664", "0x3f3708"]},
        "original_imports": {"0x30e520": "strcpy", "0x30de54": "strlen",
                             "0x30ebd4": "strstr"},
        "host_report": host_report,
        "actual_cache_level_count": len(rows),
        "actual_crypt_level": next({"index": i, "name": r.name,
                                     "level_file": r.level_file} for i, r in enumerate(catalogue.levels)
                                    if r.name == "GOTHICUS_CRYPT_01"),
        "comparison_count": len(comparisons), "comparisons": comparisons,
        "mismatches": 0,
        "limitations": [
            "Only the constructor's field initialization and LevelList scan span is reconstructed; the rest of Level construction is not run.",
            "The original ARM scan is executed at its constructor span with initialized object fields and LevelList globals; unrelated constructor services are outside scope.",
            "The LevelTable native parser retains normalized rows, but this source kernel is not yet wired to a live GSLevel/Level lifecycle.",
            "Port-only guards reject unsafe LevelFile rows instead of reproducing the original unbounded strcpy stack overwrite.",
        ],
    }
    (args.output / "validation.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"validation": report["validation"], "host_cases": host_report["level_construction_cases"],
                      "original_comparisons": len(comparisons), "cache_crypt_index": host_report["cache_crypt_index"],
                      "mismatches": report["mismatches"]}))


if __name__ == "__main__":
    main()
