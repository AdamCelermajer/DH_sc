#!/usr/bin/env python3
"""Original ARM32 engine instructions versus compiled ARM64 reconstruction.

External __aeabi/libm dependencies use the same host dependency model on both
sides. No original engine algorithm is replaced or mocked. This checks engine
flow, arithmetic order, memory output, and pointer returns for sampled inputs;
it does not prove the behavior of historical Android libm or run the game.
"""
import argparse
import ctypes
import hashlib
import json
import math
from pathlib import Path
import random
import struct
import time

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_ARM, UC_ARCH_ARM64, UC_MODE_ARM, UC_HOOK_CODE, UC_HOOK_BLOCK
from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
    UC_ARM_REG_R3, UC_ARM_REG_LR, UC_ARM_REG_SP, UC_ARM_REG_PC)
from unicorn.arm64_const import (UC_ARM64_REG_X0, UC_ARM64_REG_X1, UC_ARM64_REG_X2,
    UC_ARM64_REG_LR, UC_ARM64_REG_SP, UC_ARM64_REG_PC,
    UC_ARM64_REG_S0, UC_ARM64_REG_S1, UC_ARM64_REG_S2, UC_ARM64_REG_D0)

ROOT = Path(__file__).resolve().parent.parent
ORIGINAL_SHA256 = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'

def f32(value):
    return ctypes.c_float(value).value

def floats(values):
    return struct.pack('<' + 'f' * len(values), *values)

def bits(value):
    return struct.unpack('<I', floats([value]))[0]

def scalar(raw):
    return struct.unpack('<f', struct.pack('<I', raw))[0]

class Dependencies:
    def __init__(self, path):
        self.library = ctypes.CDLL(str(path.resolve()))
        self.functions = {}
        specs = {
            'fadd': ('f', 2), 'fsub': ('f', 2), 'fmul': ('f', 2), 'fdiv': ('f', 2),
            'sqrtf': ('f', 1), 'sinf': ('f', 1), 'cosf': ('f', 1),
            'asinf': ('f', 1), 'acosf': ('f', 1), 'atan2f': ('f', 2),
            'sin': ('d', 1), 'cos': ('d', 1), 'atan2': ('d', 2),
            'dadd': ('d', 2), 'dsub': ('d', 2), 'dmul': ('d', 2), 'ddiv': ('d', 2),
            'f2d': ('f', 1), 'd2f': ('d', 1),
        }
        for name, (kind, argc) in specs.items():
            function = getattr(self.library, 'oracle_' + name)
            argtype = ctypes.c_float if kind == 'f' else ctypes.c_double
            function.argtypes = [argtype] * argc
            function.restype = (ctypes.c_double if name == 'f2d' else
                                ctypes.c_float if name == 'd2f' else argtype)
            self.functions[name] = (function, kind, argc)

    def call(self, cpu, name):
        name = name.removeprefix('__aeabi_')
        if name == 'memcpy':
            dst, src, length = [cpu.reg(i) for i in range(3)]
            cpu.uc.mem_write(dst, bytes(cpu.uc.mem_read(src, length)))
            cpu.write_reg(0, dst)
        elif name == 'memset':
            dst, byte, length = [cpu.reg(i) for i in range(3)]
            cpu.uc.mem_write(dst, bytes([byte & 255]) * length)
            cpu.write_reg(0, dst)
        elif name == 'fabs':
            cpu.put_float(abs(cpu.get_float('d', 0)), 'd')
        elif name.startswith(('fcmp', 'dcmp')):
            kind = name[0]
            a, b = [cpu.get_float(kind, i) for i in range(2)]
            compare = {'eq': a == b, 'lt': a < b, 'le': a <= b,
                       'gt': a > b, 'ge': a >= b,
                       'un': math.isnan(a) or math.isnan(b)}
            cpu.write_reg(0, int(compare[name[4:]]))
        elif name in self.functions:
            function, kind, argc = self.functions[name]
            arguments = [cpu.get_float(kind, i) for i in range(argc)]
            outkind = 'd' if name == 'f2d' else 'f' if name == 'd2f' else kind
            cpu.put_float(function(*arguments), outkind)
        else:
            raise AssertionError('Unmodeled imported dependency executed: ' + name)
        cpu.uc.reg_write(cpu.pc_reg, cpu.uc.reg_read(cpu.lr_reg))

class Cpu:
    def __init__(self, path, arm64, dependencies, provenance):
        self.arm64 = arm64
        self.uc = Uc(UC_ARCH_ARM64 if arm64 else UC_ARCH_ARM, UC_MODE_ARM)
        self.base = 0x100000000 if arm64 else 0
        self.data = 0x200000000 if arm64 else 0x2000000
        self.stack = 0x400000000 if arm64 else 0x4000000
        self.stop = 0x500000000 if arm64 else 0x5000000
        self.extern = self.stop + 0x10000
        self.int_regs = ((UC_ARM64_REG_X0, UC_ARM64_REG_X1, UC_ARM64_REG_X2)
                         if arm64 else
                         (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3))
        self.fp_regs = (UC_ARM64_REG_S0, UC_ARM64_REG_S1, UC_ARM64_REG_S2)
        self.lr_reg = UC_ARM64_REG_LR if arm64 else UC_ARM_REG_LR
        self.sp_reg = UC_ARM64_REG_SP if arm64 else UC_ARM_REG_SP
        self.pc_reg = UC_ARM64_REG_PC if arm64 else UC_ARM_REG_PC
        self.symbols, self.imports = {}, {}
        self.import_calls, self.seen = {}, set()
        self.original_ranges = [(int(r['elf_address'], 16), r['size'])
                                for r in provenance['functions']]
        with path.open('rb') as stream:
            elf = ELFFile(stream)
            loads = [s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
            low = min(s['p_vaddr'] for s in loads) & ~4095
            high = (max(s['p_vaddr'] + s['p_memsz'] for s in loads) + 4095) & ~4095
            self.uc.mem_map(self.base + low, high - low)
            for segment in loads:
                self.uc.mem_write(self.base + segment['p_vaddr'], segment.data())
            for section_name in ('.dynsym', '.symtab'):
                section = elf.get_section_by_name(section_name)
                if section:
                    for symbol in section.iter_symbols():
                        if symbol['st_shndx'] != 'SHN_UNDEF':
                            self.symbols[symbol.name] = self.base + symbol['st_value']
            # Resolve each imported JUMP_SLOT to an explicit external stub.
            # Both CPUs execute their real PLT instructions before the hook.
            for section in elf.iter_sections():
                if section['sh_type'] not in ('SHT_REL', 'SHT_RELA'):
                    continue
                symbols = elf.get_section(section['sh_link'])
                for relocation in section.iter_relocations():
                    kind = relocation['r_info_type']
                    if arm64 and kind == 1027:  # R_AARCH64_RELATIVE
                        target = self.base + relocation['r_addend']
                        self.uc.mem_write(self.base + relocation['r_offset'],
                                          struct.pack('<Q', target))
                    elif kind == (1026 if arm64 else 22):  # JUMP_SLOT
                        symbol = symbols.get_symbol(relocation['r_info_sym'])
                        if symbol['st_shndx'] != 'SHN_UNDEF':
                            target = self.symbols[symbol.name]
                        else:
                            target = self.extern + len(self.imports) * 16
                            self.imports[target] = symbol.name
                        self.uc.mem_write(self.base + relocation['r_offset'],
                                          struct.pack('<Q' if arm64 else '<I', target))
        self.uc.mem_map(self.data, 0x10000)
        self.uc.mem_map(self.stack, 0x10000)
        self.uc.mem_map(self.stop, 0x1000)
        self.uc.mem_map(self.extern, 0x10000)
        def imported(uc, address, size, unused):
            name = self.imports.get(address)
            if name is None:
                raise AssertionError(f'Unresolved external instruction: {address:#x}')
            self.import_calls[name] = self.import_calls.get(name, 0) + 1
            dependencies.call(self, name)
        self.uc.hook_add(UC_HOOK_CODE, imported, begin=self.extern,
                         end=self.extern + 0xffff)
        if not arm64:
            def executed(uc, address, size, unused):
                if any(a <= address < a + n for a, n in self.original_ranges):
                    self.seen.update(range(address, address + size, 4))
            self.uc.hook_add(UC_HOOK_BLOCK, executed)

    def reg(self, i):
        return self.uc.reg_read(self.int_regs[i])

    def write_reg(self, i, value):
        self.uc.reg_write(self.int_regs[i], value)

    def get_float(self, kind, i):
        if self.arm64:
            raw = self.uc.reg_read(self.fp_regs[i] if kind == 'f' else UC_ARM64_REG_D0 + i)
        elif kind == 'f':
            raw = self.reg(i)
        else:
            raw = self.reg(2 * i) | (self.reg(2 * i + 1) << 32)
        return struct.unpack('<f' if kind == 'f' else '<d',
                             raw.to_bytes(4 if kind == 'f' else 8, 'little'))[0]

    def put_float(self, value, kind):
        raw = int.from_bytes(struct.pack('<f' if kind == 'f' else '<d', value), 'little')
        if self.arm64:
            self.uc.reg_write(self.fp_regs[0] if kind == 'f' else UC_ARM64_REG_D0, raw)
        else:
            self.write_reg(0, raw & 0xffffffff)
            if kind == 'd':
                self.write_reg(1, raw >> 32)

    def invoke(self, name, integers, fp=(), stack=b''):
        entry_sp = self.stack + 0xe000
        self.uc.reg_write(self.sp_reg, entry_sp)
        self.uc.reg_write(self.lr_reg, self.stop)
        for i, value in enumerate(integers):
            self.write_reg(i, value)
        if self.arm64:
            for i, (kind, value) in enumerate(fp):
                raw = int.from_bytes(struct.pack('<f' if kind == 'f' else '<d', value), 'little')
                self.uc.reg_write(self.fp_regs[i] if kind == 'f' else UC_ARM64_REG_D0 + i, raw)
        if stack:
            self.uc.mem_write(entry_sp, stack)
        self.uc.emu_start(self.symbols[name], self.stop, count=10000)
        if self.uc.reg_read(self.pc_reg) != self.stop:
            raise AssertionError('Routine did not return within instruction budget: ' + name)
        if self.uc.reg_read(self.sp_reg) != entry_sp:
            raise AssertionError('Stack not restored: ' + name)
        return self.reg(0)

def compare(original, ported, row, inputs, label):
    """Pack explicit original calling conventions; compare complete outputs."""
    name = row['port_symbol']
    kind = row['test_kind']
    returned = kind not in ('rotate', 'matrix', 'quat_euler_output')
    size = 68 if kind in ('matrix', 'matrix_value') else 16 if name.startswith('dh2_quat_') and kind not in ('transform', 'quat_euler_output') else 12
    results = []
    for cpu in (original, ported):
        out, a, b = [cpu.data + offset for offset in (0x100, 0x1000, 0x2000)]
        sentinel = b'\xa5' * (size + 32)
        cpu.uc.mem_write(out - 16, sentinel)
        for address, payload in zip((a, b), inputs):
            if isinstance(payload, bytes):
                cpu.uc.mem_write(address, payload)
        old = row['original_symbol']
        pointer_result = out
        if kind == 'normalize':
            cpu.uc.mem_write(out, inputs[0]); ints = [out]; fp = (); extra = b''
        elif kind == 'divide':
            ints = [out, a, b]; fp = (); extra = b''
        elif kind == 'divide_assign':
            cpu.uc.mem_write(out, inputs[0]); ints = [out, b]; fp = (); extra = b''
            if label.endswith(':self'):
                ints[1] = out
        elif kind == 'rotate':
            cpu.uc.mem_write(out, inputs[0]); angle = inputs[2]
            center = out if label.endswith(':self') else b
            if cpu.arm64:
                ints = [out, center]; fp = [('d', angle)]; extra = b''
            else:
                lo, hi = struct.unpack('<II', struct.pack('<d', angle))
                ints = [out, 0, lo, hi]; fp = (); extra = struct.pack('<I', center)
        elif kind == 'euler':
            values = struct.unpack('<3f', inputs[0])
            ints = [out] if cpu.arm64 else [out, *map(bits, values)]
            fp = [('f', x) for x in values] if cpu.arm64 else (); extra = b''
        elif kind == 'axis':
            angle = inputs[1]
            ints = [out, a] if cpu.arm64 else [out, bits(angle), a]
            fp = [('f', angle)] if cpu.arm64 else (); extra = b''
        elif kind in ('unary_output', 'matrix_value'):
            ints = [out, a]; fp = (); extra = b''
        elif kind in ('multiply', 'transform'):
            ints = [out, a, b]; fp = (); extra = b''
        elif kind in ('matrix', 'quat_euler_output'):
            ints = [a, out]; fp = (); extra = b''
        elif kind == 'slerp':
            t = inputs[2]
            if cpu.arm64:
                ints = [out, a, b]; fp = [('f', t)]; extra = b''
            else:
                raw = struct.unpack('<4I', inputs[0])
                ints = [out, *raw[:3]]; fp = ()
                extra = struct.pack('<I', raw[3]) + inputs[1] + floats([t])
        else:
            raise AssertionError('Unknown test kind: ' + kind)
        r0 = cpu.invoke(name if cpu.arm64 else old, ints, fp, extra)
        if returned and r0 != pointer_result:
            raise AssertionError(f'{name}: pointer return changed on {label}: {r0:#x}')
        if bytes(cpu.uc.mem_read(out - 16, 16)) != b'\xa5' * 16 or bytes(cpu.uc.mem_read(out + size, 16)) != b'\xa5' * 16:
            raise AssertionError(f'{name}: output guard overwritten on {label}')
        if kind in ('matrix', 'matrix_value'):
            if bytes(cpu.uc.mem_read(out + 65, 3)) != b'\xa5' * 3:
                raise AssertionError(f'{name}: original matrix padding overwritten')
        results.append(bytes(cpu.uc.mem_read(out, size)))
        # The original algorithms here may read inputs but never modify them.
        for address, payload in zip((a, b), inputs):
            if isinstance(payload, bytes) and bytes(cpu.uc.mem_read(address, len(payload))) != payload:
                raise AssertionError(f'{name}: input unexpectedly modified on {label}')
    left, right = results
    count = 16 if size == 68 else size // 4
    for i in range(count):
        a, b = left[4*i:4*i+4], right[4*i:4*i+4]
        # NaN propagation payload/sign can differ by CPU/runtime. All other
        # values, including signed zero and infinities, compare bit for bit.
        if a != b and not (math.isnan(struct.unpack('<f', a)[0]) and math.isnan(struct.unpack('<f', b)[0])):
            raise AssertionError(f'{name} mismatch at float {i}, {label}, inputs={inputs!r}: original={a.hex()}, port={b.hex()}')
    if size == 68 and left[64:] != right[64:]:
        raise AssertionError(f'{name}: matrix hint/padding changed on {label}')

def cases(row, rng, random_cases):
    kind, name = row['test_kind'], row['port_symbol']
    vectors = [(0., 0., 0.), (-0., 0., -0.), (1., 0., 0.), (0., 1., 0.),
               (0., 0., 1.), (3., 4., 0.), (-1., -2., -3.),
               (1.e-30, -1.e-30, 1.e-30), (1.e20, 1.e20, -1.e20)]
    quats = [(0., 0., 0., 1.), (1., 0., 0., 0.), (0., 1., 0., 0.),
             (0., 0., 1., 0.), (0., 0., 0., 0.), (0., 0., 0., -1.),
             (.5, .5, .5, .5), (.1, -.2, .3, .4)]
    edge = [scalar(x) for x in (0, 0x80000000, 1, 0x80000001, 0x007fffff,
            0x00800000, 0x7f7fffff, 0xff7fffff, 0x7f800000, 0xff800000,
            0x7fc12345)]
    def matrix(diagonal, rotation_entry=0.):
        m = [0.] * 16
        for i, x in zip((0, 5, 10), diagonal): m[i] = x
        m[2] = rotation_entry; m[15] = 1.
        return floats(m) + b'\x01\x5a\x5a\x5a'
    if kind == 'normalize':
        inputs = quats if name.startswith('dh2_quat') else vectors
        for i, value in enumerate(inputs): yield (floats(value),), f'fixed:{i}'
        n = 4 if name.startswith('dh2_quat') else 3
        for i, x in enumerate(edge): yield (floats([x] * n),), f'edge:{i}'
        for i in range(random_cases):
            yield (floats([rng.uniform(-1000, 1000) for _ in range(n)]),), f'random:{i}'
    elif kind in ('divide', 'divide_assign'):
        for i, a in enumerate(vectors):
            for j, b in enumerate(vectors): yield (floats(a), floats(b)), f'fixed:{i}:{j}'
        for i, x in enumerate(edge):
            yield (floats([x, 1., 0.]), floats([1., x, x])), f'edge:{i}'
        for i in range(random_cases):
            values = [floats([rng.uniform(-100, 100) for _ in range(3)]) for _ in range(2)]
            yield tuple(values), f'random:{i}'
        if kind == 'divide_assign':
            for i, a in enumerate(vectors): yield (floats(a), floats(a)), f'alias:{i}:self'
    elif kind == 'rotate':
        for i, v in enumerate(vectors[:7]):
            for angle in (-360., -90., -0., 0., 45., 90., 180., 360., 1.e6):
                yield (floats(v), floats((1., 2., 3.)), angle), f'fixed:{i}:{angle}'
        for i in range(random_cases):
            yield (floats([rng.uniform(-100, 100) for _ in range(3)]),
                   floats([rng.uniform(-10, 10) for _ in range(3)]),
                   rng.uniform(-720, 720)), f'random:{i}'
        yield (floats((1., 2., 3.)), floats((1., 2., 3.)), 90.), 'alias:self'
    elif kind == 'euler':
        for i, v in enumerate(vectors[:7]): yield (floats(v),), f'fixed:{i}'
        for i in range(random_cases): yield (floats([rng.uniform(-8, 8) for _ in range(3)]),), f'random:{i}'
    elif kind == 'axis':
        for i, v in enumerate(vectors[:7]):
            for angle in (0., -0., 1., -1., 3.1415927, 6.2831855):
                yield (floats(v), angle), f'fixed:{i}:{angle}'
        for i in range(random_cases):
            yield (floats([rng.uniform(-1, 1) for _ in range(3)]), f32(rng.uniform(-10, 10))), f'random:{i}'
    elif kind in ('multiply', 'transform'):
        for i, q in enumerate(quats):
            for j, value in enumerate(quats if kind == 'multiply' else vectors[:7]):
                yield (floats(q), floats(value)), f'fixed:{i}:{j}'
        for i in range(random_cases):
            yield (floats([rng.uniform(-2, 2) for _ in range(4)]),
                   floats([rng.uniform(-2, 2) for _ in range(4 if kind == 'multiply' else 3)])), f'random:{i}'
    elif kind == 'slerp':
        for i, q in enumerate(quats):
            for j, v in enumerate(quats):
                for t in (-.25, 0., .5, 1., 1.25):
                    yield (floats(q), floats(v), t), f'fixed:{i}:{j}:{t}'
        yield (floats([math.nan, 0, 0, 1]), floats([0, 0, 0, 1]), .5), 'nan-orthogonal'
        # Around the exact float threshold, preserve the >= branch.
        for d in (scalar(0x3f733332), scalar(0x3f733333), scalar(0x3f733334)):
            yield (floats([0, 0, 0, 1]), floats([0, 0, math.sqrt(1-d*d), d]), .25), f'threshold:{bits(d):x}'
        for i in range(random_cases):
            pair = []
            for _ in range(2):
                q = [rng.uniform(-1, 1) for _ in range(4)]
                length = math.sqrt(sum(x*x for x in q))
                pair.append(floats([x / length for x in q]))
            yield (*pair, f32(rng.uniform(-.5, 1.5))), f'random:{i}'
    elif kind == 'unary_output' and name in ('dh2_quat_from_matrix', 'dh2_matrix_rotation_degrees'):
        for diagonal in ((1, 1, 1), (1, -1, -1), (-1, 1, -1), (-1, -1, 1),
                         (0, 0, 0), (-2, -2, -2), (2, 1, 0), (0, 2, 1)):
            yield (matrix(diagonal),), f'diagonal:{diagonal}'
        for e in (-1., 1., 0., math.nan, -1.00001, 1.00001):
            yield (matrix((1, 1, 1), e),), f'asin-boundary:{e}'
        for i in range(random_cases):
            values = [rng.uniform(-1, 1) for _ in range(16)]
            yield (floats(values) + b'\x00\x5a\x5a\x5a',), f'random:{i}'
    else:
        is_vector = name in ('dh2_vec3_horizontal_angles', 'dh2_quat_from_fixed_angle_axis')
        fixed = vectors if is_vector else quats
        for i, v in enumerate(fixed): yield (floats(v),), f'fixed:{i}'
        if name == 'dh2_vec3_horizontal_angles':
            # A tiny negative angle plus 360 rounds to exactly 360 in float.
            # These exercise the original second wrap branch for yaw/pitch.
            yield (floats([-1.e-9, 0., 1.]),), 'round-to-360-yaw'
            yield (floats([1., 1.e-9, 0.]),), 'round-to-360-pitch'
        for i in range(random_cases):
            yield (floats([rng.uniform(-1, 1) for _ in range(3 if is_vector else 4)]),), f'random:{i}'

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--original', type=Path, required=True)
    parser.add_argument('--ported', type=Path, default=ROOT / 'build/libdh2_engine_math_arm64.so')
    parser.add_argument('--oracle', type=Path, default=ROOT / 'build/libfp_oracle.so')
    parser.add_argument('--report', type=Path, default=ROOT.parent.parent / 'reports/engine-math-validation.json')
    parser.add_argument('--random-cases', type=int, default=1024)
    args = parser.parse_args()
    if args.random_cases < 0: parser.error('--random-cases must not be negative')
    original_hash = hashlib.sha256(args.original.read_bytes()).hexdigest()
    if original_hash != ORIGINAL_SHA256:
        raise SystemExit('Wrong original engine hash; refusing this reference binary.')
    provenance = json.loads((ROOT / 'original-functions.json').read_text())
    deps = Dependencies(args.oracle)
    original = Cpu(args.original, False, deps, provenance)
    ported = Cpu(args.ported, True, deps, provenance)
    for row in provenance['functions']:
        address = int(row['elf_address'], 16)
        if original.symbols[row['original_symbol']] != address:
            raise AssertionError('Original symbol address disagrees with provenance')
        code = bytes(original.uc.mem_read(address, row['size']))
        if hashlib.sha256(code).hexdigest() != row['original_code_sha256']:
            raise AssertionError('Original routine bytes disagree with provenance')
    rng = random.Random(20631002)
    start, rows = time.monotonic(), []
    for row in provenance['functions']:
        count = 0
        for inputs, label in cases(row, rng, args.random_cases):
            compare(original, ported, row, inputs, label)
            count += 1
        rows.append({'original_symbol': row['original_symbol'],
                     'elf_address': row['elf_address'], 'port_symbol': row['port_symbol'],
                     'cases': count, 'mismatches': 0})
        print(row['port_symbol'], count, 'cases match', flush=True)
    for row, evidence in zip(provenance['functions'], rows):
        address, size = int(row['elf_address'], 16), row['size']
        evidence['original_instruction_addresses_executed'] = sum(
            1 for a in original.seen if address <= a < address + size)
        evidence['original_arm_words_in_range'] = size // 4
    with args.ported.open('rb') as stream:
        elf = ELFFile(stream)
        align = [s['p_align'] for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
        if not align or any(n < 16384 for n in align):
            raise AssertionError('ARM64 load alignment is below 16 KiB')
        if elf['e_machine'] != 'EM_AARCH64': raise AssertionError('Wrong port ABI')
    report = {
        'validation': 'CPU-emulated original ARM32 instructions versus compiled ARM64 source',
        'original_sha256': original_hash,
        'arm64_sha256': hashlib.sha256(args.ported.read_bytes()).hexdigest(),
        'source_sha256': hashlib.sha256((ROOT / 'math.cpp').read_bytes()).hexdigest(),
        'source_files_sha256': {name: hashlib.sha256((ROOT / name).read_bytes()).hexdigest()
                               for name in ('math.cpp', 'math.hpp', 'original-functions.json',
                                            'tests/differential.py', 'tests/fp_oracle.c')},
        'oracle_binary_sha256': hashlib.sha256(args.oracle.read_bytes()).hexdigest(),
        'seed': 20631002, 'random_cases_per_function': args.random_cases,
        'functions': rows, 'total_comparisons': sum(r['cases'] for r in rows),
        'mismatches': 0, 'arm64_pointers_above_4gib': True,
        'arm64_load_alignments': align,
        'comparison': 'Exact float bits except NaN sign/payload; matrix hint, padding, output guards, input preservation, stack restoration and pointer returns checked',
        'external_dependency_model': 'Host C float/double arithmetic and libm for imported __aeabi/libm; same transcendental implementations on both CPUs',
        'import_calls_original': original.import_calls,
        'import_calls_ported': ported.import_calls,
        'elapsed_seconds': round(time.monotonic() - start, 3),
        'historical_android_libm_validated': False,
        'android_device_tested': False, 'complete_engine': False,
        'gameplay_validated': False,
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + '\n')
    print('Total', report['total_comparisons'], 'comparisons; report:', args.report)

if __name__ == '__main__':
    main()
