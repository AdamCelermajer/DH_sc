"""Compare the recovered SViewFrustum::setFrom body with its plane producer."""
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
MANIFEST = MODULE / 'reference/frustum-producer/original-functions.json'
ELF_SHA = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
SYMBOL = '_ZN6glitch5scene12SViewFrustum7setFromERKNS_4core8CMatrix4IfEE'
EXPECTED_IMPORTS = {
    'sqrtf': 6,
    '__aeabi_fdiv': 6,
    '__aeabi_fadd': 20,
    '__aeabi_fsub': 12,
    '__aeabi_fmul': 42,
}
EXPECTED_TRACE = ('a' * 4 + 's' * 8 + 'a' * 4 + 's' * 4 + 'mmamaqdmmmm' * 6)


def f32(value: float) -> float:
    try:
        return struct.unpack('<f', struct.pack('<f', value))[0]
    except OverflowError:
        return math.copysign(math.inf, value)


def bits(value: float) -> int:
    return struct.unpack('<I', struct.pack('<f', f32(value)))[0]


def from_bits(word: int) -> float:
    return struct.unpack('<f', struct.pack('<I', word & 0xffffffff))[0]


def fadd(a: int, b: int) -> int:
    return bits(from_bits(a) + from_bits(b))


def fsub(a: int, b: int) -> int:
    return bits(from_bits(a) - from_bits(b))


def fmul(a: int, b: int) -> int:
    return bits(from_bits(a) * from_bits(b))


def fdiv(a: int, b: int) -> int:
    x, y = from_bits(a), from_bits(b)
    if math.isnan(x) or math.isnan(y):
        return 0x7fc00000
    if y == 0.0:
        if x == 0.0:
            return 0x7fc00000
        return bits(math.copysign(math.inf, math.copysign(1.0, x) * math.copysign(1.0, y)))
    return bits(x / y)


def sqrtf(a: int) -> int:
    x = from_bits(a)
    if math.isnan(x):
        return 0x7fc00000
    if x < 0.0:
        return 0x7fc00000
    return bits(math.sqrt(x))


def vector(*values: float) -> list[int]:
    assert len(values) == 16
    return [bits(value) for value in values]


def case_matrices(random_count: int) -> list[tuple[str, list[int]]]:
    rows = [
        ('identity', vector(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)),
        ('perspective-like', vector(0.78, 0, 0, 0, 0, 1.31, 0, 0,
                                    0, 0, 1.001, 1, 0, 0, -0.1001, 0)),
        ('signed-skew', vector(-1.25, 0.5, -0.75, 0.25, 0.125, -2, 0.625, 0.5,
                               -0.375, 1.5, -0.875, -0.25, 4, -3, 2, 1)),
        ('all-zero-degenerate', [0] * 16),
        ('negative-zero-boundary', [0x80000000 if i % 3 == 0 else 0 for i in range(16)]),
        ('zero-normal-nonzero-d', vector(0, 0, 0, 0, 0, 0, 0, 0,
                                         0, 0, 0, 0, 1, -2, 3, 4)),
        ('subnormal-boundary', [0x00000001 if i in (0, 5, 10, 15) else 0 for i in range(16)]),
        ('max-finite-and-overflow', [0x7f7fffff if i % 2 else 0xff7fffff for i in range(16)]),
        ('infinity', [0x7f800000 if i in (0, 5, 10, 15) else 0 for i in range(16)]),
        ('nan-payloads', [0x7fc12345 if i % 4 == 0 else (0xffc54321 if i % 4 == 1 else bits(float(i - 5)))
                          for i in range(16)]),
        ('tiny-finite', vector(1e-20, 0, 0, 0, 0, 2e-20, 0, 0,
                               0, 0, 3e-20, 0, 1e-20, -1e-20, 2e-20, 1)),
        ('large-finite', vector(1e20, 0, 0, 0, 0, -2e20, 0, 0,
                                0, 0, 3e20, 0, 4e20, -5e20, 6e20, 1)),
        ('cancellation', vector(1, 0, 0, 1, 0, 1, 0, 1,
                                0, 0, 1, 1, 0, 0, 0, 0)),
    ]
    rng = random.Random(5826)
    for index in range(random_count):
        values = [rng.uniform(-16.0, 16.0) for _ in range(16)]
        rows.append((f'random-{index}', vector(*values)))
    return rows


class FrustumCpu:
    def __init__(self, elf_path: Path, manifest: dict):
        sys.path.insert(0, str(ROOT / 'port/engine-resources/tests'))
        from cpu import Cpu

        class SourceCpu(Cpu):
            def external(self, uc, address, size, unused):
                name = self.imports.get(address)
                self.import_calls[name] = self.import_calls.get(name, 0) + 1
                binary = {
                    '__aeabi_fadd': (fadd, 'a'),
                    '__aeabi_fsub': (fsub, 's'),
                    '__aeabi_fmul': (fmul, 'm'),
                    '__aeabi_fdiv': (fdiv, 'd'),
                }
                if name in binary:
                    operation, marker = binary[name]
                    self.math_events.append(marker)
                    self.put(0, operation(self.reg(0), self.reg(1)))
                    self.uc.reg_write(self.pc, self.uc.reg_read(self.lr))
                    return
                if name == 'sqrtf':
                    self.math_events.append('q')
                    self.put(0, sqrtf(self.reg(0)))
                    self.uc.reg_write(self.pc, self.uc.reg_read(self.lr))
                    return
                if name in ('memcpy', 'memmove', '__aeabi_memcpy', '__aeabi_memcpy4', 'memset'):
                    super().external(uc, address, size, unused)
                    return
                raise AssertionError(f'unmodeled import {name} at {address:#x}, caller {self.uc.reg_read(self.lr) - 4:#x}')

        source_row = manifest['functions'][0]
        cpu_manifest = {'functions': [
            {**source_row, 'size': source_row['implemented_prefix_size']}
        ]}
        self.cpu = SourceCpu(elf_path, False, cpu_manifest)
        self.cpu.math_events = []
        # setFrom restores its frame then tail-branches to the separate
        # recalculateBoundingBox routine. Replace that tail branch with the
        # caller return sentinel to stop exactly at the owned plane slice.
        tail = int(manifest['tail_boundary']['tail_branch_address'], 0)
        self.cpu.uc.mem_write(tail, bytes.fromhex('1eff2fe1'))
        self.import_baseline = dict(self.cpu.import_calls)

    def run(self, matrix: list[int]) -> tuple[list[int], dict[str, int]]:
        cpu = self.cpu
        cpu.math_events = []
        matrix_address, frustum_address = cpu.data + 0x10000, cpu.data + 0x20000
        cpu.uc.mem_write(matrix_address, struct.pack('<16I', *matrix))
        cpu.uc.mem_write(frustum_address, bytes(0x80))
        cpu.invoke(SYMBOL, [frustum_address, matrix_address])
        words = struct.unpack('<24I', cpu.uc.mem_read(frustum_address + 0x0c, 24 * 4))
        counts = {name: cpu.import_calls.get(name, 0) - self.import_baseline.get(name, 0)
                  for name in EXPECTED_IMPORTS}
        self.import_baseline = dict(cpu.import_calls)
        return list(words), counts, ''.join(cpu.math_events)


def assert_equal_bits(source: list[int], port: list[int], label: str) -> None:
    if len(source) != 24 or len(port) != 24:
        raise AssertionError(f'{label}: wrong output extent')
    for index, (actual, expected) in enumerate(zip(source, port)):
        a, b = from_bits(actual), from_bits(expected)
        if math.isnan(a) or math.isnan(b):
            if not (math.isnan(a) and math.isnan(b)):
                raise AssertionError(f'{label}[{index}]: NaN classification differs: {actual:08x} {expected:08x}')
        elif actual != expected:
            raise AssertionError(f'{label}[{index}]: source={actual:08x} port={expected:08x}')


def verify_manifest(elf_path: Path, manifest: dict) -> None:
    with elf_path.open('rb') as stream:
        elf = ELFFile(stream)
        symtab = elf.get_section_by_name('.symtab')
        symbols = {symbol.name: symbol for symbol in symtab.iter_symbols()}
        row = manifest['functions'][0]
        symbol = symbols[row['original_symbol']]
        if (symbol['st_value'], symbol['st_size']) != (int(row['elf_address'], 0), row['size']):
            raise AssertionError('original frustum symbol mismatch')
        def read_range(address: int, length: int) -> bytes:
            for segment in elf.iter_segments():
                if segment['p_type'] == 'PT_LOAD' and segment['p_vaddr'] <= address and \
                        address + length <= segment['p_vaddr'] + segment['p_filesz']:
                    offset = address - segment['p_vaddr']
                    return segment.data()[offset:offset + length]
            raise AssertionError(f'original source range is not file-backed: {address:#x}+{length:#x}')

        source = read_range(int(row['elf_address'], 0), row['size'])
        if hashlib.sha256(source).hexdigest() != row['sha256']:
            raise AssertionError('pinned setFrom body SHA-256 mismatch')
        if hashlib.sha256(source[:row['implemented_prefix_size']]).hexdigest() != row['implemented_prefix_sha256']:
            raise AssertionError('pinned plane extraction/normalization slice SHA-256 mismatch')
        boundary = manifest['tail_boundary']
        boundary_row = manifest['supporting_functions'][0]
        boundary_symbol = symbols[boundary_row['original_symbol']]
        if (boundary_symbol['st_value'], boundary_symbol['st_size']) != \
                (int(boundary_row['elf_address'], 0), boundary_row['size']):
            raise AssertionError('bounding-box tail boundary symbol mismatch')
        if hashlib.sha256(read_range(int(boundary_row['elf_address'], 0), boundary_row['size'])).hexdigest() != boundary_row['sha256']:
            raise AssertionError('bounding-box boundary body SHA-256 mismatch')
        if read_range(int(boundary['tail_branch_address'], 0), 4).hex() != boundary['original_branch_bytes']:
            raise AssertionError('setFrom tail branch bytes differ')

        relocations = elf.get_section_by_name('.rel.plt')
        dynamic_symbols = elf.get_section(relocations['sh_link'])
        imported_names = [dynamic_symbols.get_symbol(relocation['r_info_sym']).name
                          for relocation in relocations.iter_relocations()]
        plt = elf.get_section_by_name('.plt')
        for imported in manifest['external_imports']:
            slot = imported_names.index(imported['symbol'])
            expected_stub = plt['sh_addr'] + 20 + slot * 12
            if expected_stub != int(imported['plt_address'], 0):
                raise AssertionError(f'PLT import identity mismatch for {imported["symbol"]}')


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--original-elf', type=Path, required=True)
    parser.add_argument('--compiler', default=os.environ.get('CXX') or shutil.which('g++'))
    parser.add_argument('--random-cases', type=int, default=128)
    parser.add_argument('--output', type=Path, default=MODULE / 'build/frustum-producer/frustum-host.exe')
    parser.add_argument('--report', type=Path, default=MODULE / 'build/frustum-producer/validation.json')
    args = parser.parse_args()
    if args.random_cases < 0:
        parser.error('--random-cases must be nonnegative')
    if not args.compiler:
        parser.error('pass --compiler or set CXX')

    source_elf = args.original_elf.resolve()
    if hashlib.sha256(source_elf.read_bytes()).hexdigest() != ELF_SHA:
        raise SystemExit('pinned original ELF SHA-256 mismatch')
    manifest = json.loads(MANIFEST.read_text(encoding='utf-8'))
    if manifest['original_sha256'] != ELF_SHA:
        raise SystemExit('manifest does not pin the expected ELF')
    verify_manifest(source_elf, manifest)

    exe = args.output.resolve()
    exe.parent.mkdir(parents=True, exist_ok=True)
    inputs = [MODULE / 'frustum.cpp', MODULE / 'frustum.hpp',
              MODULE / 'tests/frustum_host.cpp', Path(__file__).resolve(), MANIFEST]
    pins = {p.relative_to(ROOT).as_posix(): hashlib.sha256(p.read_bytes()).hexdigest()
            for p in inputs}
    command = [args.compiler, '-std=c++17', '-O1', '-Wall', '-Wextra', '-Werror',
                    '-pedantic', '-ffp-contract=off', str(MODULE / 'frustum.cpp'),
                    str(HERE / 'frustum_host.cpp'), '-o', str(exe)]
    subprocess.run(command, check=True, cwd=ROOT)
    host_guard = json.loads(subprocess.check_output([str(exe), 'guards'], text=True))
    if host_guard['host_guard_cases'] != 8 or host_guard['math_calls'] != [20, 12, 42, 6, 6]:
        raise AssertionError(f'host guard/helper call contract failed: {host_guard}')

    cpu = FrustumCpu(source_elf, manifest)
    results = []
    expected_names = set(EXPECTED_IMPORTS)
    for name, matrix in case_matrices(args.random_cases):
        host_json = subprocess.check_output(
            [str(exe), 'matrix', *[f'{word:08x}' for word in matrix]], text=True)
        host = json.loads(host_json)
        if host['status'] != 0:
            raise AssertionError(f'{name}: host source call failed')
        source, calls, source_trace = cpu.run(matrix)
        port = [word for plane in host['planes'] for word in plane]
        if set(calls) != expected_names:
            raise AssertionError(f'{name}: unexpected import schedule {calls}')
        if host['events'] != EXPECTED_TRACE or source_trace != EXPECTED_TRACE:
            raise AssertionError(f'{name}: arithmetic operation order mismatch: '
                                 f'host={host["events"]}, source={source_trace}, expected={EXPECTED_TRACE}')
        assert_equal_bits(source, port, name)
        results.append({'name': name, 'matched': True})

    row = manifest['functions'][0]
    start, size = int(row['elf_address'], 0), row['implemented_prefix_size']
    required = set(range(start, start + size, 4))
    covered = required.intersection(cpu.cpu.seen)
    if required != covered:
        missing = sorted(required - covered)
        raise AssertionError('setFrom instructions not covered: ' + ','.join(hex(x) for x in missing))
    if pins != {p.relative_to(ROOT).as_posix(): hashlib.sha256(p.read_bytes()).hexdigest()
                for p in inputs}:
        raise AssertionError('source changed during the gate')

    report = {
        'validation': 'PASS',
        'original_arm_cases': len(results),
        'mismatches': 0,
        'source_sha256': pins,
        'compile_command': command,
        'host_guards': host_guard,
        'covered_active_instructions': len(covered),
        'active_instructions': len(required),
        'import_calls_per_case': EXPECTED_IMPORTS,
        'operation_order': EXPECTED_TRACE,
        'nan_comparison': 'classification only; payload/sign propagation is outside the external libm model',
        'original_elf_sha256': ELF_SHA,
        'original_function': row,
        'tail_boundary': manifest['tail_boundary'],
        'source_results': results,
        'operation_trace_key': {'a': 'fadd', 's': 'fsub', 'm': 'fmul', 'd': 'fdiv', 'q': 'sqrtf'},
        'scope': ('576-byte SViewFrustum::setFrom plane extraction and six-plane normalization prefix. '
                  'The final 4-byte branch to recalculateBoundingBox is excluded. '
                  'Imported fadd/fsub/fmul/fdiv/sqrtf are explicit IEEE binary32 helper models; '
                  'no historical Bionic libm claim. Camera/frustum ownership and later plane '
                  'intersection/bounds routines are not implemented.'),
    }
    report_path = args.report.resolve()
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({key: report[key] for key in ('validation', 'original_arm_cases',
                                                    'mismatches', 'active_instructions',
                                                    'covered_active_instructions')}))


if __name__ == '__main__':
    main()
