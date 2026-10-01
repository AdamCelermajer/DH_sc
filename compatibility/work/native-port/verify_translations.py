#!/usr/bin/env python3
"""Differential execution: original ARM32 instructions versus hand-ported ARM64.

Unicorn emulates both CPUs on the build host. This is NOT a phone/game test.
The checked addresses belong to one exact uploaded library hash.
"""
import argparse
import hashlib
import json
import random
import struct
import time
from pathlib import Path

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_ARM, UC_ARCH_ARM64, UC_MODE_ARM
from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_LR, UC_ARM_REG_SP, UC_ARM_REG_PC
from unicorn.arm64_const import UC_ARM64_REG_X0, UC_ARM64_REG_X1, UC_ARM64_REG_LR, UC_ARM64_REG_SP, UC_ARM64_REG_PC

ROOT = Path(__file__).resolve().parent
ORIGINAL_SHA256 = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
FUNCTIONS = {
    'luaO_log2': 'dh2_log2_u32',
    'luaO_int2fb': 'dh2_int2fb_u32',
    'luaO_fb2int': 'dh2_fb2int_u32',
    '_ZNK6glitch3gui11IGUIElement13isPointInsideERKNS_4core10position2dIiEE': 'dh2_ui_contains_point_legacy32',
}


class Cpu:
    def __init__(self, path, arm64):
        self.uc = Uc(UC_ARCH_ARM64 if arm64 else UC_ARCH_ARM, UC_MODE_ARM)
        self.base = 0x100000000 if arm64 else 0
        self.data = 0x200000000 if arm64 else 0x2000000
        self.stack = 0x400000000 if arm64 else 0x4000000
        self.stop = 0x500000000 if arm64 else 0x5000000
        self.regs = (UC_ARM64_REG_X0, UC_ARM64_REG_X1, UC_ARM64_REG_LR,
                     UC_ARM64_REG_SP, UC_ARM64_REG_PC) if arm64 else (
                     UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_LR,
                     UC_ARM_REG_SP, UC_ARM_REG_PC)
        with path.open('rb') as f:
            elf = ELFFile(f)
            loads = [s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
            low = min(s['p_vaddr'] for s in loads) & ~4095
            high = (max(s['p_vaddr'] + s['p_memsz'] for s in loads) + 4095) & ~4095
            self.uc.mem_map(self.base + low, high - low)
            for segment in loads:
                self.uc.mem_write(self.base + segment['p_vaddr'], segment.data())
            symbols = elf.get_section_by_name('.symtab') or elf.get_section_by_name('.dynsym')
            self.symbols = {s.name: self.base + s['st_value'] for s in symbols.iter_symbols()
                            if s['st_shndx'] != 'SHN_UNDEF'}
        self.uc.mem_map(self.data, 0x10000)
        self.uc.mem_map(self.stack, 0x10000)
        self.uc.mem_map(self.stop, 0x1000)

    def call(self, name, a, b=0):
        r0, r1, lr, sp, pc = self.regs
        self.uc.reg_write(r0, a)
        self.uc.reg_write(r1, b)
        self.uc.reg_write(lr, self.stop)
        self.uc.reg_write(sp, self.stack + 0xff00)
        self.uc.emu_start(self.symbols[name], self.stop, count=2000)
        if self.uc.reg_read(pc) != self.stop:
            raise AssertionError('Function did not return within instruction budget: ' + name)
        return self.uc.reg_read(r0) & 0xffffffff


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--original', type=Path, default=ROOT.parent / 'original/lib/armeabi-v7a/libDungeonHunter2.so')
    parser.add_argument('--ported', type=Path, default=ROOT / 'build/libdh2_port_prototype.so')
    args = parser.parse_args()
    if hashlib.sha256(args.original.read_bytes()).hexdigest() != ORIGINAL_SHA256:
        raise SystemExit('Input engine differs from the audited APK; refusing to test the wrong build.')
    original = Cpu(args.original, False)
    ported = Cpu(args.ported, True)
    rng = random.Random(20631001)
    values = set(range(4096))
    values.update(rng.getrandbits(32) for _ in range(8192))
    values.update((1 << bit) + delta for bit in range(32) for delta in (-1, 0, 1)
                  if 0 <= (1 << bit) + delta <= 0xffffffff)
    values.update([0xfffffffe, 0xffffffff, 0x80000000, 0x7fffffff])
    results = []
    started = time.monotonic()
    for old, new in list(FUNCTIONS.items())[:3]:
        count = 0
        for value in sorted(values):
            a = original.call(old, value)
            b = ported.call(new, value)
            if a != b:
                raise AssertionError(f'{old} mismatch: input={value:#x}, original={a:#x}, ported={b:#x}')
            count += 1
        results.append({'original': old, 'ported': new, 'cases': count, 'mismatches': 0})
        print(new, count, 'cases match', flush=True)

    old, new = list(FUNCTIONS.items())[3]
    cases = []
    for rect in [(0, 0, 10, 20), (-10, -20, 10, 20), (2, 3, 2, 3),
                 (20, 20, -20, -20), (-2147483648, -2147483648, 2147483647, 2147483647)]:
        for x in {rect[0], rect[2], max(-2147483648, rect[0]-1), min(2147483647, rect[2]+1), 0}:
            for y in {rect[1], rect[3], max(-2147483648, rect[1]-1), min(2147483647, rect[3]+1), 0}:
                cases.append((rect, (x, y)))
    for _ in range(8192):
        rect = tuple(rng.randint(-2147483648, 2147483647) for _ in range(4))
        point = tuple(rng.randint(-2147483648, 2147483647) for _ in range(2))
        cases.append((rect, point))
    for rect, point in cases:
        for cpu in (original, ported):
            cpu.uc.mem_write(cpu.data + 0x48, struct.pack('<4i', *rect))
            cpu.uc.mem_write(cpu.data + 0x2000, struct.pack('<2i', *point))
        a = original.call(old, original.data, original.data + 0x2000)
        b = ported.call(new, ported.data, ported.data + 0x2000)
        if a != b:
            raise AssertionError(f'{old} mismatch: rect={rect}, point={point}, original={a}, ported={b}')
    results.append({'original': old, 'ported': new, 'cases': len(cases), 'mismatches': 0,
                    'arm64_data_pointers_above_4gib': True})
    print(new, len(cases), 'cases match', flush=True)
    report = {
        'kind': 'CPU-emulated differential checks; not a mathematical proof or device test',
        'original_sha256': hashlib.sha256(args.original.read_bytes()).hexdigest(),
        'ported_sha256': hashlib.sha256(args.ported.read_bytes()).hexdigest(),
        'functions': results,
        'total_comparisons': sum(x['cases'] for x in results),
        'elapsed_seconds': round(time.monotonic() - started, 3),
        'full_game_ported': False,
        'android_load_tested': False,
        'performance_measured': False,
    }
    (ROOT / 'translation-results.json').write_text(json.dumps(report, indent=2))
    print(json.dumps(report, indent=2))


if __name__ == '__main__':
    main()
