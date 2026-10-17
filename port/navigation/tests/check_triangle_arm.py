#!/usr/bin/env python3
"""Differential checks for the original ARM32 triangle line intersection.

The harness enters the original `triangle3d<float>::getIntersectionWithLine`
at 0x58615c. Tiny IEEE-754 helper shims stand in for the unrelocated PLT
imports; the triangle, plane, containment, and intersection instructions run
from the supplied ELF.
"""
import argparse
import ctypes as c
import hashlib
import importlib.util
import json
import math
from pathlib import Path
import struct

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE
from unicorn.arm_const import (
    UC_ARM_REG_LR, UC_ARM_REG_PC, UC_ARM_REG_R0, UC_ARM_REG_R1,
    UC_ARM_REG_R2, UC_ARM_REG_R3, UC_ARM_REG_SP,
)


GET_INTERSECTION_WITH_LINE = 0x0058615C
TRIANGLE_LINE_PLANE = 0x00585E80
TRIANGLE_POINT_INSIDE = 0x00585DE8
HELPERS = {
    0x0030E3AC: 'float_sub',
    0x0030EBA4: 'float_add',
    0x0030ED6C: 'float_mul',
    0x0030EC94: 'float_div',
    0x0030E124: 'float_sqrt',
    0x0030DF8C: 'float_equal',
    0x0030E9AC: 'float_less_equal',
    0x0030E4B4: 'float_greater_equal',
}


def float_from_bits(bits):
    return struct.unpack('<f', struct.pack('<I', bits & 0xFFFFFFFF))[0]


def float_bits(value):
    return struct.unpack('<I', struct.pack('<f', value))[0]


class OriginalArm:
    def __init__(self, path):
        self.path = path.resolve()
        with self.path.open('rb') as stream:
            elf = ELFFile(stream)
            if elf.elfclass != 32 or elf.header['e_machine'] != 'EM_ARM':
                raise AssertionError('Expected the original ARM32 ELF')
            segments = [s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
            low = min(s['p_vaddr'] for s in segments) & ~0xFFF
            high = (max(s['p_vaddr'] + s['p_memsz'] for s in segments) + 0xFFF) & ~0xFFF
            self.uc = Uc(UC_ARCH_ARM, UC_MODE_ARM)
            self.uc.mem_map(low, high - low)
            for segment in segments:
                self.uc.mem_write(segment['p_vaddr'], segment.data())
        self.sha256 = hashlib.sha256(self.path.read_bytes()).hexdigest()
        self.data = (high + 0x10000 + 0xFFFF) & ~0xFFFF
        self.stack = self.data + 0x10000
        self.return_to = self.data + 0x20000
        self.uc.mem_map(self.data, 0x10000)
        self.uc.mem_map(self.stack, 0x10000)
        self.uc.mem_map(self.return_to, 0x1000)
        self.triangle = self.data + 0x1000
        self.start = self.data + 0x2000
        self.end = self.data + 0x2100
        self.result = self.data + 0x2200
        self.helper_calls = {name: 0 for name in HELPERS.values()}
        self.uc.hook_add(UC_HOOK_CODE, self._helper_hook)

    def _helper_hook(self, uc, address, size, _):
        if address not in HELPERS:
            return
        name = HELPERS[address]
        self.helper_calls[name] += 1
        left = float_from_bits(uc.reg_read(UC_ARM_REG_R0))
        right = float_from_bits(uc.reg_read(UC_ARM_REG_R1))
        if name == 'float_sub':
            result = float_bits(left - right)
        elif name == 'float_add':
            result = float_bits(left + right)
        elif name == 'float_mul':
            result = float_bits(left * right)
        elif name == 'float_div':
            result = float_bits(left / right) if right else float_bits(
                math.copysign(math.inf, left * math.copysign(1.0, right)))
        elif name == 'float_sqrt':
            result = float_bits(math.sqrt(left)) if left >= 0 else float_bits(math.nan)
        elif name == 'float_equal':
            result = int(left == right)
        elif name == 'float_less_equal':
            result = int(left <= right)
        else:
            result = int(left >= right)
        uc.reg_write(UC_ARM_REG_R0, result)
        uc.reg_write(UC_ARM_REG_PC, uc.reg_read(UC_ARM_REG_LR))

    def call_triangle_line(self, triangle, start, direction, sentinel=0x4F123456):
        triangle_data = struct.pack('<9f', *(triangle[0] + triangle[1] + triangle[2]))
        self.uc.mem_write(self.triangle, triangle_data)
        self.uc.mem_write(self.start, struct.pack('<3f', *start))
        self.uc.mem_write(self.end, struct.pack('<3f', *direction))
        self.uc.mem_write(self.result, struct.pack('<3I', sentinel, sentinel, sentinel))
        stack = self.stack + 0xF000
        self.uc.reg_write(UC_ARM_REG_SP, stack)
        self.uc.reg_write(UC_ARM_REG_LR, self.return_to)
        self.uc.reg_write(UC_ARM_REG_R0, self.triangle)
        self.uc.reg_write(UC_ARM_REG_R1, self.start)
        self.uc.reg_write(UC_ARM_REG_R2, self.end)
        self.uc.reg_write(UC_ARM_REG_R3, self.result)
        self.uc.emu_start(GET_INTERSECTION_WITH_LINE, self.return_to, count=30000)
        if self.uc.reg_read(UC_ARM_REG_PC) != self.return_to:
            raise AssertionError('Native triangle line function did not return')
        if self.uc.reg_read(UC_ARM_REG_SP) != stack:
            raise AssertionError('Native triangle line function did not restore SP')
        hit = bool(self.uc.reg_read(UC_ARM_REG_R0) & 0xFFFFFFFF)
        point = struct.unpack('<3f', self.uc.mem_read(self.result, 12))
        return hit, point


def main():
    default_elf = (Path(__file__).resolve().parents[4]
                   / 'standalone-build/compatibility/work/decoded-original/lib/armeabi-v7a'
                   / 'libDungeonHunter2.so')
    parser = argparse.ArgumentParser()
    parser.add_argument('--original', type=Path, default=default_elf)
    parser.add_argument('--library', type=Path,
                        help='Optional host navigation library for point-by-point differential checks')
    parser.add_argument('--report', type=Path)
    args = parser.parse_args()
    if not args.original.is_file():
        raise SystemExit(f'Original ELF not found: {args.original}')
    arm = OriginalArm(args.original)
    host_query = None
    nav_checks = None
    if args.library:
        spec = importlib.util.spec_from_file_location(
            'navigation_host_types', Path(__file__).with_name('check_navigation.py'))
        nav_checks = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(nav_checks)
        host = c.CDLL(str(args.library.resolve()))
        host_query = host.dh2_nav_query_segment
        host_query.restype = c.c_uint32
        host_query.argtypes = [c.POINTER(nav_checks.Navigation), c.POINTER(c.c_float),
            c.POINTER(c.c_float), c.c_bool, c.POINTER(nav_checks.SegmentHit),
            c.POINTER(c.c_bool)]
    triangle = ((0.0, 0.0, 0.0), (2.0, 0.0, 0.0), (0.0, 2.0, 0.0))
    host_surface = host_triangles = host_nav = None
    if host_query:
        host_surface = (nav_checks.Surface*1)()
        host_triangles = (nav_checks.SurfaceTriangle*1)()
        host_surface[0].triangle_count = 1
        host_surface[0].floor_type_flags_known = True
        for vertex, values in zip(('a', 'b', 'c'), triangle):
            getattr(host_triangles[0], vertex)[:] = values
        host_triangles[0].surface_index = 0
        host_nav = nav_checks.Navigation(host_surface, 1, 1, host_triangles, 1, 1)
    cases = []

    def check(name, start, end, expected_hit, expected_point=None):
        direction = tuple(b - a for a, b in zip(start, end))
        native_hit, native_point = arm.call_triangle_line(triangle, start, direction)
        assert native_hit is expected_hit, (name, native_hit, expected_hit, native_point)
        if expected_point is not None:
            assert all(math.isclose(actual, expected, abs_tol=1.0e-5)
                       for actual, expected in zip(native_point, expected_point)), (
                name, native_point, expected_point)
        host_point = None
        host_fraction = None
        if host_query:
            host_start = (c.c_float*3)(*start)
            host_end = (c.c_float*3)(*end)
            host_hit = nav_checks.SegmentHit()
            host_found = c.c_bool()
            status = host_query(c.byref(host_nav), host_start, host_end, True,
                                c.byref(host_hit), c.byref(host_found))
            assert status == 0 and host_found.value is native_hit, (
                name, status, host_found.value, native_hit)
            if native_hit:
                host_point = list(host_hit.position)
                host_fraction = float(host_hit.fraction)
                assert all(math.isclose(actual, expected, abs_tol=1.0e-5)
                           for actual, expected in zip(host_point, native_point)), (
                    name, host_point, native_point)
                if 'endpoint' in name:
                    assert host_fraction == 1.0, (name, host_fraction)
        cases.append({'name': name, 'native_hit': native_hit,
                      'native_point': list(native_point) if native_hit else None,
                      'host_point': host_point, 'host_fraction': host_fraction})

    check('interior-line-hit', (0.5, 0.5, 1.0), (0.5, 0.5, 0.0), True,
          (0.5, 0.5, 0.0))
    check('line-through-finite-segment-endpoint', (0.5, 0.5, 1.0),
          (0.5, 0.5, 0.0), True,
          (0.5, 0.5, 0.0))
    check('parallel-line', (0.25, 0.25, 1.0), (1.0, 0.25, 1.0), False)
    check('triangle-edge-hit', (1.0, 0.0, 1.0), (1.0, 0.0, 0.0), True,
          (1.0, 0.0, 0.0))
    check('outside-triangle', (1.5, 1.5, 1.0), (1.5, 1.5, 0.0), False)
    report = {
        'result': 'PASS',
        'original_sha256': arm.sha256,
        'entry_address': hex(GET_INTERSECTION_WITH_LINE),
        'plane_intersection_address': hex(TRIANGLE_LINE_PLANE),
        'point_inside_address': hex(TRIANGLE_POINT_INSIDE),
        'cases': cases,
        'assertions': len(cases) * 2 + (len(cases) if host_query else 0)
                       + (sum(c['native_hit'] for c in cases) if host_query else 0),
        'host_differential': bool(host_query),
        'shimmed_helper_calls': sum(arm.helper_calls.values()),
        'plt_helper_calls': arm.helper_calls,
        'shimmed_operations': list(HELPERS.values()),
        'scope': 'original triangle3d line-intersection instructions under Unicorn; helper imports shimmed; finite-segment clipping is separately host-tested',
    }
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
