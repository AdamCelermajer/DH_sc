"""Differentially run original plane intersection ARM bodies and the port."""
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
MANIFEST = MODULE / 'reference/plane-intersection/original-functions.json'
ELF_SHA = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
SYMBOLS = {
    'line': '_ZNK6glitch4core7plane3dIfE23getIntersectionWithLineERKNS0_8vector3dIfEES6_RS4_',
    'two': '_ZNK6glitch4core7plane3dIfE24getIntersectionWithPlaneERKS2_RNS0_8vector3dIfEES7_',
    'three': '_ZNK6glitch4core7plane3dIfE25getIntersectionWithPlanesERKS2_S4_RNS0_8vector3dIfEE',
}
EVENT = {
    '__aeabi_fmul': 'm', '__aeabi_fadd': 'a', '__aeabi_fsub': 's',
    '__aeabi_fdiv': 'd', '__aeabi_fcmpeq': 'e', '__aeabi_f2d': 't',
    '__aeabi_d2f': 'u', 'sqrt': 'q', '__aeabi_dmul': 'M',
    '__aeabi_ddiv': 'D', '__aeabi_dcmplt': 'L',
}
HOST_SOURCE = HERE / 'plane_intersection_host.cpp'
SHARED_CPU = ROOT / 'port/engine-resources/tests/cpu.py'


def sha256_file(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def source_snapshot() -> dict[str, str]:
    paths = (MODULE / 'plane_intersection.hpp', MODULE / 'plane_intersection.cpp',
             HOST_SOURCE, Path(__file__).resolve(), MANIFEST, SHARED_CPU)
    return {str(path.relative_to(ROOT)): sha256_file(path) for path in paths}


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


def encode(value) -> int:
    return int(value) if isinstance(value, RawWord) else bits(float(value))


def fadd(a: int, b: int) -> int:
    return bits(from_bits(a) + from_bits(b))


def fsub(a: int, b: int) -> int:
    return bits(from_bits(a) - from_bits(b))


def fmul(a: int, b: int) -> int:
    return bits(from_bits(a) * from_bits(b))


def fdiv(a: int, b: int) -> int:
    x, y = from_bits(a), from_bits(b)
    if math.isnan(x) or math.isnan(y) or (x == 0.0 and y == 0.0):
        return 0x7fc00000
    if y == 0.0:
        return bits(math.copysign(math.inf,
                                  math.copysign(1.0, x) * math.copysign(1.0, y)))
    return bits(x / y)


def as_double(low: int, high: int) -> float:
    return struct.unpack('<d', struct.pack('<II', low & 0xffffffff,
                                           high & 0xffffffff))[0]


def double_words(value: float) -> tuple[int, int]:
    return struct.unpack('<II', struct.pack('<d', value))


def divide_double(x: float, y: float) -> float:
    if math.isnan(x) or math.isnan(y) or (x == 0.0 and y == 0.0):
        return math.nan
    if y == 0.0:
        return math.copysign(math.inf,
                             math.copysign(1.0, x) * math.copysign(1.0, y))
    return x / y


def plane(*components):
    assert len(components) == 4
    return [encode(value) for value in components]


def vector(*components):
    assert len(components) == 3
    return [encode(value) for value in components]


def cases(random_count: int):
    sentinel = [0x5a5a5a5a, 0xa5a5a5a5, 0xdeadbeef]
    rows = []
    def add(name, mode, values):
        rows.append((name, mode, values))

    # Infinite line intersecting, parallel, signed-zero denominator, and NaN.
    add('line-hit', 'line', plane(0, 0, 1, 0) + vector(1, 2, 3) + vector(0, 0, -1) + sentinel)
    add('line-parallel', 'line', plane(0, 0, 1, 0) + vector(1, 2, 3) + vector(1, 0, 0) + sentinel)
    add('line-negative-zero', 'line', plane(0, 0, 1, 0) + vector(1, 2, 3) + vector(0, 0, raw(0x80000000)) + sentinel)
    add('line-nan-normal', 'line', plane(raw(0x7fc12345), 1, 0, 2) + vector(1, 2, 3) + vector(1, 1, 1) + sentinel)
    add('line-infinite-dot', 'line', plane(raw(0x7f800000), 1, 0, 2) + vector(1, 2, 3) + vector(1, 1, 1) + sentinel)

    # Plane-pair line success, determinant rejection, and exceptional inputs.
    add('two-orthogonal', 'two', plane(1, 0, 0, -2) + plane(0, 1, 0, 3) + sentinel + sentinel)
    add('two-skew', 'two', plane(1, 2, 3, -4) + plane(-2, 1, 0.5, 7) + sentinel + sentinel)
    add('two-parallel', 'two', plane(1, 0, 0, -2) + plane(2, 0, 0, -9) + sentinel + sentinel)
    add('two-near-parallel', 'two', plane(1, 0, 0, 0) + plane(1, 0.0001, 0, 1) + sentinel + sentinel)
    add('two-nan', 'two', plane(raw(0x7fc12345), 0, 1, 1) + plane(0, 1, 0, -1) + sentinel + sentinel)
    add('two-subnormal', 'two', plane(raw(1), 0, 0, 1) + plane(0, raw(1), 0, -1) + sentinel + sentinel)

    # Full three-plane caller including both early-false branches.
    add('three-orthogonal', 'three', plane(1, 0, 0, -2) + plane(0, 1, 0, 3) + plane(0, 0, 1, -4) + sentinel)
    add('three-skew', 'three', plane(1, 2, 3, -4) + plane(-2, 1, 0.5, 7) + plane(0.25, -1, 4, 2) + sentinel)
    add('three-parallel-first-pair', 'three', plane(1, 0, 0, -2) + plane(2, 0, 0, -9) + plane(0, 1, 0, 0) + sentinel)
    add('three-line-parallel', 'three', plane(1, 0, 0, -2) + plane(0, 1, 0, 3) + plane(0, 0, 1, 4) + sentinel)
    add('three-nan', 'three', plane(raw(0x7fc12345), 0, 1, 1) + plane(0, 1, 0, -1) + plane(0, 0, 1, 3) + sentinel)

    rng = random.Random(0x3415d8)
    for index in range(random_count):
        def rp():
            return plane(*(rng.uniform(-10.0, 10.0) for _ in range(4)))
        def rv():
            return vector(*(rng.uniform(-10.0, 10.0) for _ in range(3)))
        if index % 3 == 0:
            add(f'random-line-{index}', 'line', rp() + rv() + rv() + sentinel)
        elif index % 3 == 1:
            add(f'random-two-{index}', 'two', rp() + rp() + sentinel + sentinel)
        else:
            add(f'random-three-{index}', 'three', rp() + rp() + rp() + sentinel)
    return rows


class PlaneCpu:
    def __init__(self, elf_path: Path, manifest: dict):
        sys.path.insert(0, str(ROOT / 'port/engine-resources/tests'))
        from cpu import Cpu

        functions = manifest['functions']
        cpu_manifest = {'functions': [
            {'original_symbol': row['original_symbol'], 'elf_address': row['elf_address'],
             'size': row['size']} for row in functions
        ]}

        class SourceCpu(Cpu):
            def external(self, uc, address, size, unused):
                name = self.imports.get(address)
                self.import_calls[name] = self.import_calls.get(name, 0) + 1
                def record(*values):
                    self.trace.append([EVENT[name], *[value & 0xffffffff for value in values]])
                if name in ('__aeabi_fmul', '__aeabi_fadd', '__aeabi_fsub', '__aeabi_fdiv'):
                    operation = {'__aeabi_fmul': fmul, '__aeabi_fadd': fadd,
                                 '__aeabi_fsub': fsub, '__aeabi_fdiv': fdiv}[name]
                    self.events.append(EVENT[name])
                    record(self.reg(0), self.reg(1))
                    self.put(0, operation(self.reg(0), self.reg(1)))
                elif name == '__aeabi_fcmpeq':
                    self.events.append(EVENT[name])
                    record(self.reg(0), self.reg(1))
                    self.put(0, int(from_bits(self.reg(0)) == from_bits(self.reg(1))))
                elif name == '__aeabi_f2d':
                    self.events.append(EVENT[name])
                    record(self.reg(0))
                    low, high = double_words(float(from_bits(self.reg(0))))
                    self.put(0, low)
                    self.put(1, high)
                elif name == '__aeabi_d2f':
                    self.events.append(EVENT[name])
                    record(self.reg(0), self.reg(1))
                    self.put(0, bits(as_double(self.reg(0), self.reg(1))))
                elif name == 'sqrt':
                    self.events.append(EVENT[name])
                    record(self.reg(0), self.reg(1))
                    value = as_double(self.reg(0), self.reg(1))
                    result = math.sqrt(value) if value >= 0 else math.nan
                    self.put(0, double_words(result)[0])
                    self.put(1, double_words(result)[1])
                elif name in ('__aeabi_dmul', '__aeabi_ddiv'):
                    self.events.append(EVENT[name])
                    record(self.reg(0), self.reg(1), self.reg(2), self.reg(3))
                    left = as_double(self.reg(0), self.reg(1))
                    right = as_double(self.reg(2), self.reg(3))
                    result = left * right if name == '__aeabi_dmul' else divide_double(left, right)
                    low, high = double_words(result)
                    self.put(0, low)
                    self.put(1, high)
                elif name == '__aeabi_dcmplt':
                    self.events.append(EVENT[name])
                    record(self.reg(0), self.reg(1), self.reg(2), self.reg(3))
                    left = as_double(self.reg(0), self.reg(1))
                    right = as_double(self.reg(2), self.reg(3))
                    self.put(0, int(left < right))
                elif name in ('memcpy', 'memmove', '__aeabi_memcpy', '__aeabi_memcpy4', 'memset'):
                    super().external(uc, address, size, unused)
                    return
                else:
                    raise AssertionError(f'unmodeled source import {name} at {address:#x}')
                self.uc.reg_write(self.pc, self.uc.reg_read(self.lr))

        self.cpu = SourceCpu(elf_path, False, cpu_manifest)

    def run(self, mode: str, words: list[int]):
        cpu = self.cpu
        cpu.events = []
        cpu.trace = []
        base = cpu.data + 0x10000
        plane_a, plane_b, plane_c = base + 0x100, base + 0x200, base + 0x300
        line_point, line_vector, output = base + 0x400, base + 0x500, base + 0x600
        cursor = 0
        def write(address, count):
            nonlocal cursor
            values = words[cursor:cursor + count]
            cpu.uc.mem_write(address, struct.pack('<' + 'I' * count, *values))
            cursor += count
        if mode == 'line':
            write(plane_a, 4); write(line_point, 3); write(line_vector, 3); write(output, 3)
            result = cpu.invoke(0x34033c, [plane_a, line_point, line_vector, output])
            out = list(struct.unpack('<3I', cpu.uc.mem_read(output, 12)))
        elif mode == 'two':
            write(plane_a, 4); write(plane_b, 4); write(line_point, 3); write(line_vector, 3)
            result = cpu.invoke(0x341260, [plane_a, plane_b, line_point, line_vector])
            out = list(struct.unpack('<3I', cpu.uc.mem_read(line_point, 12))) + \
                  list(struct.unpack('<3I', cpu.uc.mem_read(line_vector, 12)))
        else:
            write(plane_a, 4); write(plane_b, 4); write(plane_c, 4); write(output, 3)
            result = cpu.invoke(0x3415d8, [plane_a, plane_b, plane_c, output])
            out = list(struct.unpack('<3I', cpu.uc.mem_read(output, 12)))
        return result & 0xffffffff, out, ''.join(cpu.events), cpu.trace


def compare_word(source: int, host: int, label: str):
    a, b = from_bits(source), from_bits(host)
    if math.isnan(a) or math.isnan(b):
        if not (math.isnan(a) and math.isnan(b)):
            raise AssertionError(f'{label}: NaN classification differs {source:08x} {host:08x}')
    elif source != host:
        raise AssertionError(f'{label}: {source:08x} != {host:08x}')


def normalized_trace(trace: list[list]) -> list[list]:
    result = []
    for call in trace:
        operation, *words = call
        if operation in ('m', 'a', 's', 'd', 'e', 't'):
            words = [0x7fc00000 if (w & 0x7f800000) == 0x7f800000 and
                     (w & 0x007fffff) else w for w in words]
        else:
            for index in range(0, len(words), 2):
                if math.isnan(as_double(words[index], words[index + 1])):
                    canonical = double_words(math.nan)
                    words[index:index + 2] = canonical
        result.append([operation, *words])
    return result


def verify_manifest(elf_path: Path, manifest: dict):
    with elf_path.open('rb') as stream:
        elf = ELFFile(stream)
        symbols = {s.name: s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        loads = [s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
        def read_range(address: int, size: int) -> bytes:
            for segment in loads:
                if segment['p_vaddr'] <= address and address + size <= segment['p_vaddr'] + segment['p_filesz']:
                    data = segment.data()
                    offset = address - segment['p_vaddr']
                    return data[offset:offset + size]
            raise AssertionError(f'not file-backed: {address:#x}+{size:#x}')
        for row in manifest['functions']:
            sym = symbols[row['original_symbol']]
            address, size = int(row['elf_address'], 0), row['size']
            if (sym['st_value'], sym['st_size']) != (address, size):
                raise AssertionError(f'function range mismatch for {row["original_symbol"]}')
            if hashlib.sha256(read_range(address, size)).hexdigest() != row['sha256']:
                raise AssertionError(f'function hash mismatch for {row["original_symbol"]}')
        rel = elf.get_section_by_name('.rel.plt')
        dyn = elf.get_section(rel['sh_link'])
        names = [dyn.get_symbol(item['r_info_sym']).name for item in rel.iter_relocations()]
        plt = elf.get_section_by_name('.plt')
        for imported in manifest['imports']:
            slot = names.index(imported['symbol'])
            if plt['sh_addr'] + 20 + slot * 12 != int(imported['plt_address'], 0):
                raise AssertionError(f'PLT identity mismatch for {imported["symbol"]}')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--original-elf', type=Path, required=True)
    parser.add_argument('--compiler', default=os.environ.get('CXX') or shutil.which('g++'))
    parser.add_argument('--random-cases', type=int, default=64)
    parser.add_argument('--output', type=Path, default=MODULE / 'build/plane-intersection/plane-intersection-host.exe')
    parser.add_argument('--report', type=Path, default=MODULE / 'build/plane-intersection/validation.json')
    args = parser.parse_args()
    if not args.compiler:
        parser.error('pass --compiler or set CXX')
    if args.random_cases < 0:
        parser.error('--random-cases must be nonnegative')
    elf_path = args.original_elf.resolve()
    if hashlib.sha256(elf_path.read_bytes()).hexdigest() != ELF_SHA:
        raise SystemExit('original ELF SHA-256 mismatch')
    manifest = json.loads(MANIFEST.read_text(encoding='utf-8'))
    verify_manifest(elf_path, manifest)
    source_pins = source_snapshot()
    executable = args.output.resolve()
    executable.parent.mkdir(parents=True, exist_ok=True)
    compile_command = [args.compiler, '-std=c++17', '-O1', '-Wall', '-Wextra', '-Werror',
                       '-pedantic', '-ffp-contract=off', str(MODULE / 'plane_intersection.cpp'),
                       str(HOST_SOURCE), '-o', str(executable)]
    compiler_version = subprocess.check_output([args.compiler, '--version'], text=True).splitlines()[0]
    subprocess.run(compile_command, cwd=ROOT, check=True)
    if source_snapshot() != source_pins:
        raise AssertionError('source input changed during host compilation')
    guard = json.loads(subprocess.check_output([str(executable), 'guards'], text=True))
    if guard != {'guard_cases': 7, 'invalid_calls': 0}:
        raise AssertionError(f'host guard regression: {guard}')
    source = PlaneCpu(elf_path, manifest)
    results = []
    for name, mode, words in cases(args.random_cases):
        source_return, source_output, source_events, source_trace = source.run(mode, words)
        host_json = json.loads(subprocess.check_output(
            [str(executable), mode, *[f'{word:08x}' for word in words]], text=True))
        if host_json['status'] != 0:
            raise AssertionError(f'{name}: host adapter protocol error {host_json}')
        if host_json['source_boolean'] != source_return:
            raise AssertionError(f'{name}: source return {source_return} != host {host_json["source_boolean"]}')
        host_output = (host_json['line_point'] + host_json['line_vector']
                       if mode == 'two' else host_json['output'])
        if len(host_output) != len(source_output):
            raise AssertionError(f'{name}: output extent mismatch')
        for index, (actual, expected) in enumerate(zip(source_output, host_output)):
            compare_word(actual, expected, f'{name}[{index}]')
        if host_json['events'] != source_events:
            raise AssertionError(f'{name}: source/host FP call order differs\n'
                                 f'source={source_events}\nhost  ={host_json["events"]}')
        normalized_source = normalized_trace(source_trace)
        normalized_host = normalized_trace(host_json['trace'])
        if normalized_host != normalized_source:
            for call_index, (source_call, host_call) in enumerate(zip(normalized_source, normalized_host)):
                if source_call != host_call:
                    raise AssertionError(f'{name}: source/host call {call_index} differs: '
                                         f'source={source_call} host={host_call}; '
                                         f'source_neighborhood={normalized_source[max(0, call_index-2):call_index+3]} '
                                         f'host_neighborhood={normalized_host[max(0, call_index-2):call_index+3]}\n'
                                         f'source_trace={normalized_source}\n'
                                         f'host_trace={normalized_host}')
            raise AssertionError(f'{name}: source/host call trace length differs: '
                                 f'{len(normalized_source)} != {len(normalized_host)}')
        results.append({'name': name, 'mode': mode, 'matched': True,
                        'source_boolean': source_return,
                        'float_operations': len(source_events),
                        'operation_order': source_events,
                        'operand_trace': source_trace})

    coverage = {}
    for row in manifest['functions']:
        start, size = int(row['elf_address'], 0), row['size']
        required = set(range(start, start + size, 4))
        covered = required & source.cpu.seen
        coverage[row['name']] = {'covered': len(covered), 'instructions': len(required)}
        if covered != required:
            missing = sorted(required - covered)
            raise AssertionError(f'{row["name"]}: instruction coverage {len(covered)}/{len(required)}; '
                                 f'missing {missing[:8]}')
    if source_snapshot() != source_pins:
        raise AssertionError('source input changed during differential run')
    report = {
        'validation': 'PASS', 'original_arm_cases': len(results), 'mismatches': 0,
        'host_guards': guard, 'function_coverage': coverage,
        'original_elf_sha256': ELF_SHA,
        'original_elf_path': str(elf_path),
        'source_sha256_before_compile': source_pins,
        'source_sha256_after_run': source_snapshot(),
        'compiler_version': compiler_version,
        'compile_command': compile_command,
        'host_executable_sha256': sha256_file(executable),
        'imports': manifest['imports'],
        'floating_point_model': 'Original ARM bodies execute in Unicorn; the named ABI arithmetic, '
                                'sqrt, conversion, and comparison imports are explicit IEEE models. '
                                'NaN payload identity and historical Bionic implementation are not claimed.',
        'cases': results,
    }
    report_path = args.report.resolve()
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({'validation': 'PASS', 'original_arm_cases': len(results),
                      'mismatches': 0, 'function_coverage': coverage,
                      'host_guards': guard}))


if __name__ == '__main__':
    main()
