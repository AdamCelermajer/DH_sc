"""Compare the recovered frustum bounding-box writer with original ARM code."""
from __future__ import annotations

import argparse
import hashlib
import json
import math
import os
from pathlib import Path
import random
import shutil
import struct
import subprocess
import sys

from elftools.elf.elffile import ELFFile

HERE = Path(__file__).resolve().parent
MODULE = HERE.parent
ROOT = MODULE.parents[1]
MANIFEST = MODULE / 'reference/frustum-bounds/original-functions.json'
ELF_SHA = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
SYMBOL = '_ZN6glitch5scene12SViewFrustum22recalculateBoundingBoxEv'
EXPECTED_ORDER = 'iggglll' * 4
EXPECTED_COMPARISONS = {'__aeabi_fcmpgt': 12, '__aeabi_fcmplt': 12}


def f32(value: float) -> float:
    try:
        return struct.unpack('<f', struct.pack('<f', value))[0]
    except OverflowError:
        return math.copysign(math.inf, value)


def bits(value: float) -> int:
    return struct.unpack('<I', struct.pack('<f', f32(value)))[0]


def from_bits(word: int) -> float:
    return struct.unpack('<f', struct.pack('<I', word & 0xffffffff))[0]


class RawWord(int):
    pass


def raw(word: int) -> RawWord:
    return RawWord(word)


def fadd(a: int, b: int) -> int:
    return bits(from_bits(a) + from_bits(b))


def fsub(a: int, b: int) -> int:
    return bits(from_bits(a) - from_bits(b))


def fmul(a: int, b: int) -> int:
    return bits(from_bits(a) * from_bits(b))


def fdiv(a: int, b: int) -> int:
    x, y = from_bits(a), from_bits(b)
    if math.isnan(x) or math.isnan(y) or (x == 0 and y == 0):
        return 0x7fc00000
    if y == 0:
        return bits(math.copysign(math.inf,
                                  math.copysign(1.0, x) * math.copysign(1.0, y)))
    return bits(x / y)


def as_double(low: int, high: int) -> float:
    return struct.unpack('<d', struct.pack('<II', low & 0xffffffff,
                                           high & 0xffffffff))[0]


def double_words(value: float) -> tuple[int, int]:
    try:
        return struct.unpack('<II', struct.pack('<d', value))
    except OverflowError:
        return struct.unpack('<II', struct.pack('<d', math.copysign(math.inf, value)))


def divide_double(x: float, y: float) -> float:
    if math.isnan(x) or math.isnan(y) or (x == 0.0 and y == 0.0):
        return math.nan
    if y == 0.0:
        return math.copysign(math.inf, math.copysign(1.0, x) * math.copysign(1.0, y))
    return x / y


def box_cases(random_count: int):
    def row(name, position, planes):
        encode = lambda value: int(value) if isinstance(value, RawWord) else bits(float(value))
        words = [encode(v) for v in position]
        for plane in planes:
            words.extend(encode(v) for v in plane)
        words.extend([0xdead0000 + i for i in range(6)])
        assert len(words) == 33
        return name, words

    cases = [
        row('ordinary-axis', (3.0, -4.0, 5.0), [
            (1, 0, 0, -1), (0, 0, -1, -4), (0, 1, 0, -2),
            (0, -1, 0, -3), (0, 0, 1, -6), (-1, 0, 0, -7)]),
        row('ordinary-skew', (0.125, -2.0, 9.0), [
            (1.0, 0.25, 0.5, -3), (-0.75, 1, 0.125, 2),
            (0.25, -1, 0.5, -4), (-0.5, -0.125, 1, 3),
            (1.25, 0.5, -0.25, -2), (0.125, 1.5, 0.75, -1)]),
        row('parallel-degenerate', (0, 0, 0), [(0, 0, 0, 0)] * 6),
        row('signed-zero', (raw(0x80000000), 0, raw(0x80000000)), [
            (raw(0x80000000), 0, 0, 0), (0, raw(0x80000000), 0, 0),
            (0, 0, raw(0x80000000), 0), (0, 0, raw(0x80000000), 0),
            (raw(0x80000000), 0, 0, 0), (raw(0x80000000), 0, 0, 0)]),
        row('nan-coefficients', (1.0, -2.0, 3.0), [
            (raw(0x7fc12345), 1.0, 0.0, 0.0), (0.0, raw(0x7fc54321), 1.0, 0.0),
            (1.0, 0.0, raw(0x7fcabcde), -1.0), (0.0, 1.0, 0.0, raw(0x7fc98765)),
            (1.0, 1.0, 0.0, 0.0), (0.0, 1.0, 1.0, 1.0)]),
        row('infinity-and-subnormal', (0, 0, 0), [
            (raw(0x7f800000), 1.0, 0.0, 1.0), (1.0, raw(0x00000001), 1.0, 0.0),
            (0.0, 1.0, 0.0, 0.0), (0.0, -1.0, 0.0, 1.0),
            (0.0, 0.0, 1.0, -1.0), (0.0, 0.0, -1.0, -1.0)]),
    ]
    rng = random.Random(5823)
    for index in range(random_count):
        position = tuple(rng.uniform(-50.0, 50.0) for _ in range(3))
        planes = [tuple(rng.uniform(-4.0, 4.0) for _ in range(4)) for _ in range(6)]
        cases.append(row(f'random-{index}', position, planes))
    return cases


class BoundingCpu:
    def __init__(self, elf_path: Path, manifest: dict):
        sys.path.insert(0, str(ROOT / 'port/engine-resources/tests'))
        from cpu import Cpu
        from unicorn import UC_HOOK_CODE

        rows = manifest['functions'] + manifest['supporting_functions']
        cpu_manifest = {'functions': [
            {'original_symbol': row['original_symbol'],
             'elf_address': row['elf_address'], 'size': row['size']}
            for row in rows
        ]}

        class SourceCpu(Cpu):
            def external(self, uc, address, size, unused):
                name = self.imports.get(address)
                self.import_calls[name] = self.import_calls.get(name, 0) + 1
                if name in ('__aeabi_fadd', '__aeabi_fsub', '__aeabi_fmul', '__aeabi_fdiv'):
                    operation = {'__aeabi_fadd': fadd, '__aeabi_fsub': fsub,
                                 '__aeabi_fmul': fmul, '__aeabi_fdiv': fdiv}[name]
                    self.put(0, operation(self.reg(0), self.reg(1)))
                elif name in ('__aeabi_fcmpgt', '__aeabi_fcmplt', '__aeabi_fcmpeq'):
                    left, right = from_bits(self.reg(0)), from_bits(self.reg(1))
                    if name == '__aeabi_fcmpgt':
                        self.source_order.append('g')
                        result = left > right
                    elif name == '__aeabi_fcmplt':
                        self.source_order.append('l')
                        result = left < right
                    else:
                        result = left == right
                    self.put(0, int(result))
                elif name == '__aeabi_f2d':
                    low, high = double_words(from_bits(self.reg(0)))
                    self.put(0, low)
                    self.put(1, high)
                elif name == '__aeabi_d2f':
                    self.put(0, bits(as_double(self.reg(0), self.reg(1))))
                elif name in ('__aeabi_dmul', '__aeabi_ddiv'):
                    left = as_double(self.reg(0), self.reg(1))
                    right = as_double(self.reg(2), self.reg(3))
                    result = left * right if name == '__aeabi_dmul' else divide_double(left, right)
                    self.put(0, double_words(result)[0])
                    self.put(1, double_words(result)[1])
                elif name == '__aeabi_dcmplt':
                    left = as_double(self.reg(0), self.reg(1))
                    right = as_double(self.reg(2), self.reg(3))
                    self.put(0, int(left < right))
                elif name == 'sqrt':
                    value = as_double(self.reg(0), self.reg(1))
                    result = math.sqrt(value) if value >= 0.0 else math.nan
                    self.put(0, double_words(result)[0])
                    self.put(1, double_words(result)[1])
                elif name in ('memcpy', 'memmove', '__aeabi_memcpy', '__aeabi_memcpy4', 'memset'):
                    super().external(uc, address, size, unused)
                    return
                else:
                    raise AssertionError(f'unmodeled import {name} at {address:#x}, '
                                         f'caller {self.uc.reg_read(self.lr) - 4:#x}')
                self.uc.reg_write(self.pc, self.uc.reg_read(self.lr))

        self.cpu = SourceCpu(elf_path, False, cpu_manifest)
        self.cpu.source_points = []
        self.cpu.source_order = []
        self.pending_output = None
        self.return_for_call = {0x582434, 0x5824e0, 0x58258c, 0x582634}

        def trace(uc, address, size, user):
            if address == 0x3415d8:
                self.cpu.source_order.append('i')
                self.pending_output = self.cpu.reg(3)
            elif address in self.return_for_call:
                if self.pending_output is None:
                    raise AssertionError('intersection helper returned without a captured output')
                raw = bytes(uc.mem_read(self.pending_output, 12))
                self.cpu.source_points.append(list(struct.unpack('<3I', raw)))
                self.pending_output = None

        self.cpu.uc.hook_add(UC_HOOK_CODE, trace)
        self.import_baseline = dict(self.cpu.import_calls)

    def run(self, words: list[int]):
        cpu = self.cpu
        cpu.source_points = []
        cpu.source_order = []
        address = cpu.data + 0x10000
        cpu.uc.mem_write(address, struct.pack('<33I', *words))
        cpu.invoke(int('0x5823d4', 16), [address])
        updated = list(struct.unpack('<33I', cpu.uc.mem_read(address, 33 * 4)))
        counts = {name: cpu.import_calls.get(name, 0) - self.import_baseline.get(name, 0)
                  for name in ('__aeabi_fcmpgt', '__aeabi_fcmplt')}
        all_counts = {name: cpu.import_calls.get(name, 0) - self.import_baseline.get(name, 0)
                      for name in sorted(cpu.imports.values())}
        self.import_baseline = dict(cpu.import_calls)
        return updated[27:33], cpu.source_points[:], ''.join(cpu.source_order), counts, all_counts


def verify_manifest(elf_path: Path, manifest: dict):
    with elf_path.open('rb') as stream:
        elf = ELFFile(stream)
        symbols = {s.name: s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        loads = [s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
        def source_bytes(address, size):
            for segment in loads:
                if segment['p_vaddr'] <= address and address + size <= segment['p_vaddr'] + segment['p_filesz']:
                    data = segment.data()
                    offset = address - segment['p_vaddr']
                    return data[offset:offset + size]
            raise AssertionError(f'non-file-backed function {address:#x}+{size:#x}')
        for row in manifest['functions'] + manifest['supporting_functions']:
            symbol = symbols[row['original_symbol']]
            address, size = int(row['elf_address'], 0), row['size']
            if (symbol['st_value'], symbol['st_size']) != (address, size):
                raise AssertionError(f'symbol body changed: {row["original_symbol"]}')
            if hashlib.sha256(source_bytes(address, size)).hexdigest() != row['sha256']:
                raise AssertionError(f'body hash mismatch: {row["original_symbol"]}')
        reloc = elf.get_section_by_name('.rel.plt')
        dynsym = elf.get_section(reloc['sh_link'])
        names = [dynsym.get_symbol(r['r_info_sym']).name for r in reloc.iter_relocations()]
        plt = elf.get_section_by_name('.plt')
        for item in manifest['external_imports']:
            index = names.index(item['symbol'])
            if plt['sh_addr'] + 20 + index * 12 != int(item['plt_address'], 0):
                raise AssertionError(f'wrong PLT identity for {item["symbol"]}')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--original-elf', type=Path, required=True)
    parser.add_argument('--compiler', default=os.environ.get('CXX') or shutil.which('g++'))
    parser.add_argument('--random-cases', type=int, default=32)
    parser.add_argument('--output', type=Path, default=MODULE / 'build/frustum-bounds/frustum-bounds-host.exe')
    parser.add_argument('--report', type=Path, default=MODULE / 'build/frustum-bounds/validation.json')
    args = parser.parse_args()
    if not args.compiler:
        parser.error('pass --compiler or set CXX')
    if args.random_cases < 0:
        parser.error('--random-cases must be nonnegative')
    elf_path = args.original_elf.resolve()
    if hashlib.sha256(elf_path.read_bytes()).hexdigest() != ELF_SHA:
        raise SystemExit('pinned original ELF SHA-256 mismatch')
    manifest = json.loads(MANIFEST.read_text(encoding='utf-8'))
    verify_manifest(elf_path, manifest)

    executable = args.output.resolve()
    executable.parent.mkdir(parents=True, exist_ok=True)
    tracked_inputs = [MODULE / 'frustum_bounds.hpp', MODULE / 'frustum_bounds.cpp',
                      HERE / 'frustum_bounds_host.cpp', Path(__file__).resolve(), MANIFEST]
    source_sha256 = {
        path.relative_to(ROOT).as_posix(): hashlib.sha256(path.read_bytes()).hexdigest()
        for path in tracked_inputs
    }
    subprocess.run([args.compiler, '-std=c++17', '-O1', '-Wall', '-Wextra', '-Werror',
                    '-pedantic', '-ffp-contract=off', str(MODULE / 'frustum_bounds.cpp'),
                    str(HERE / 'frustum_bounds_host.cpp'), '-o', str(executable)],
                   check=True, cwd=ROOT)
    guard = json.loads(subprocess.check_output([str(executable), 'guards'], text=True))
    if guard != {'host_guard_cases': 9, 'service_calls': [4, 12, 12],
                 'same_frustum_reentry': 'rejected', 'service_table_snapshot': 'stable',
                 'unaligned_byte_context': 'accepted'}:
        raise AssertionError(f'host guard contract mismatch: {guard}')

    source = BoundingCpu(elf_path, manifest)
    results = []
    for name, words in box_cases(args.random_cases):
        source_bounds, points, source_order, comparison_calls, all_imports = source.run(words)
        if len(points) != 4 or source_order != EXPECTED_ORDER:
            raise AssertionError(f'{name}: source helper/compare order mismatch {source_order!r}')
        if comparison_calls != EXPECTED_COMPARISONS:
            raise AssertionError(f'{name}: source comparison imports differ: {comparison_calls}')
        args_vector = [f'{word:08x}' for word in words + [word for point in points for word in point]]
        host = json.loads(subprocess.check_output([str(executable), 'bounds', *args_vector], text=True))
        if host['status'] != 0 or host['bad_planes']:
            raise AssertionError(f'{name}: host adapter rejected source inputs: {host}')
        if host['events'] != EXPECTED_ORDER or host['calls'] != [4, 12, 12]:
            raise AssertionError(f'{name}: host callback schedule differs: {host}')
        if host['bounds'] != source_bounds:
            raise AssertionError(f'{name}: source={source_bounds!r}, host={host["bounds"]!r}')
        results.append({'name': name, 'matched': True, 'source_intersections': points,
                        'comparison_imports': comparison_calls,
                        'helper_imports': all_imports})

    body = manifest['functions'][0]
    start, size = int(body['elf_address'], 0), body['size']
    required = set(range(start, start + size, 4))
    covered = required.intersection(source.cpu.seen)
    if covered != required:
        raise AssertionError(f'function instruction coverage incomplete: {len(covered)}/{len(required)}')
    after_sha256 = {
        path.relative_to(ROOT).as_posix(): hashlib.sha256(path.read_bytes()).hexdigest()
        for path in tracked_inputs
    }
    if source_sha256 != after_sha256:
        raise AssertionError('implementation/test source changed during original ARM replay')
    report = {
        'validation': 'PASS', 'original_arm_cases': len(results), 'mismatches': 0,
        'host_guard': guard, 'source_instructions_covered': len(covered),
        'source_instructions': len(required), 'callback_order': EXPECTED_ORDER,
        'intersection_boundary': 'Four exact source helper call sites execute in original ARM; their '
                                 'output words feed the host body. The host has not reconstructed the '
                                 'three-plane intersection arithmetic itself.',
        'import_boundary': 'The original helper instructions run in Unicorn, while imported floating '
                           'arithmetic, sqrt, conversion, and comparison routines are modeled IEEE '
                           'operations; imported-library details and NaN payload propagation are excluded.',
        'comparison_boundary': 'Strict ordered binary32 greater/less semantics match the observed '
                               '__aeabi_fcmpgt and __aeabi_fcmplt calls.',
        'original_elf_sha256': ELF_SHA, 'function': body,
        'supporting_functions': manifest['supporting_functions'],
        'implementation_source_sha256': source_sha256,
        'cases': results,
    }
    report_path = args.report.resolve()
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({k: report[k] for k in ('validation', 'original_arm_cases', 'mismatches',
                                             'source_instructions_covered', 'source_instructions')}))


if __name__ == '__main__':
    main()
