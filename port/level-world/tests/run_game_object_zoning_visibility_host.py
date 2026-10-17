"""Replay the source zoning/visibility callers against the original ARM ELF."""
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

from unicorn import UC_HOOK_CODE
from elftools.elf.elffile import ELFFile

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
MANIFEST = MODULE / 'reference/game-object-zoning-visibility/original-functions.json'
ELF_SHA = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'

OBJ, ROOM_A, ROOM_B = 0x02110000, 0x02440000, 0x02450000
VIS_A, VIS_B, VT = 0x02220000, 0x02230000, 0x02500000
IS_ZONABLE_GAMEOBJECT = 0x3883b8
IS_ZONABLE_CHARACTER = 0x3a36e4
SET_UPDATING = 0x33dcf0

IS_ZONABLE_OP, ROOM_REMOVE_OP, ADD_NO_ROOM_OP, REMOVE_NO_ROOM_OP = 0, 1, 2, 3
ROOM_ADD_OP, ROOM_ZONED_OP, ENTER_OP, EXIT_OP = 4, 5, 6, 7
UPDATE_OP, VISIBLE_OP = 8, 9


def host_cases(exe: Path):
    rows = []
    for mode, scenario in [('d', 0), ('d', 1), ('e', 2), ('e', 3), ('e', 4),
                           ('e', 5), ('e', 6), ('e', 9), ('e', 10), ('e', 11),
                           ('d', 16), ('s', 7), ('s', 8), ('s', 15), ('s', 17)]:
        value = json.loads(subprocess.check_output([str(exe), mode, str(scenario)], text=True))
        assert value['status'] == 0, (mode, scenario, value)
        rows.append({'mode': mode, 'scenario': scenario, 'result': value})
    guard = subprocess.run([str(exe), 'g', '0'], check=False, capture_output=True, text=True)
    assert guard.returncode == 0, ('host guard suite failed', guard.returncode, guard.stdout)
    guard_cases = json.loads(guard.stdout)['host_guard_cases']
    assert guard_cases == 12, guard_cases
    return rows, guard_cases


def elf_read(elf, address: int, size: int) -> bytes:
    for segment in elf.iter_segments():
        if segment['p_type'] == 'PT_LOAD' and segment['p_vaddr'] <= address and \
                address + size <= segment['p_vaddr'] + segment['p_filesz']:
            data = segment.data()
            offset = address - segment['p_vaddr']
            return data[offset:offset + size]
    raise AssertionError(f'ELF range not file-backed: {address:#x}+{size:#x}')


def verify_manifest(elf_path: Path, manifest: dict):
    with elf_path.open('rb') as stream:
        elf = ELFFile(stream)
        symbols = {item.name: item for item in elf.get_section_by_name('.symtab').iter_symbols()}
        for row in manifest['functions'] + manifest['supporting_functions']:
            symbol = symbols[row['original_symbol']]
            address, size = int(row['elf_address'], 0), row['size']
            assert (symbol['st_value'], symbol['st_size']) == (address, size), row
            assert hashlib.sha256(elf_read(elf, address, size)).hexdigest() == row['sha256'], row
        for row in manifest['vtable_ranges']:
            symbol = symbols[row['symbol']]
            address, size = int(row['elf_address'], 0), row['size']
            assert (symbol['st_value'], symbol['st_size']) == (address, size), row
            assert hashlib.sha256(elf_read(elf, address, size)).hexdigest() == row['sha256'], row
            for slot in row['slots']:
                storage = address + 8 + int(slot['byte_offset'], 0)
                target = struct.unpack('<I', elf_read(elf, storage, 4))[0]
                assert target == int(slot['target'], 0), slot
                assert symbols[slot['symbol']]['st_value'] == target, slot


class ZoningCpu:
    def __init__(self, elf_path: Path, manifest: dict):
        sys.path.insert(0, str(ROOT / 'port/engine-resources/tests'))
        from cpu import Cpu
        self.cpu = Cpu(elf_path, False, manifest)
        self.is_zonable_targets = {
            self.cpu.symbols['_ZNK10GameObject9IsZonableEv'],
            self.cpu.symbols['_ZNK9Character9IsZonableEv'],
        }
        self.records = []
        self.scenario = -1
        self.zonable_calls = 0
        self.raw = []
        self.cpu.uc.hook_add(UC_HOOK_CODE, self.hook)

    def get32(self, address):
        return struct.unpack('<I', self.cpu.uc.mem_read(address, 4))[0]

    def get8(self, address):
        return self.cpu.uc.mem_read(address, 1)[0]

    def set32(self, address, value):
        self.cpu.pointer(address, value)

    def record(self, operation, obj, related=0, value=0):
        self.records.append([operation, obj, related, value])

    def return_from_hook(self, value=None):
        if value is not None:
            self.cpu.put(0, value)
        self.cpu.uc.reg_write(self.cpu.pc, self.cpu.uc.reg_read(self.cpu.lr))

    def hook(self, uc, address, size, unused):
        c = self.cpu
        if address in self.is_zonable_targets:
            self.zonable_calls += 1
            obj = c.reg(0)
            target = address
            value = self.raw[(self.zonable_calls - 1) % len(self.raw)]
            if self.scenario == 1 and self.zonable_calls == 1:
                uc.mem_write(obj + 0x2ee, b'\x01')
                uc.mem_write(obj + 0x2f0, b'\x80')
            elif self.scenario == 6 and self.zonable_calls == 1:
                self.set32(obj + 0x2d8, VIS_B)
            elif self.scenario == 11 and self.zonable_calls == 2:
                uc.mem_write(obj + 0x2ee, b'\x00')
                uc.mem_write(obj + 0x2f0, b'\x80')
            elif self.scenario == 16 and self.zonable_calls == 1:
                self.set32(VT + 0x3c, 0xdeadbeef)
            self.record(IS_ZONABLE_OP, obj, target, value)
            self.return_from_hook(value)
            return
        if address == SET_UPDATING:
            obj = c.reg(0)
            self.record(UPDATE_OP, obj, address, c.reg(1))
            self.return_from_hook()
            return
        direct = {
            c.symbols['_ZN8RoomZone12RemoveObjectEP10GameObject']: ROOM_REMOVE_OP,
            c.symbols['_ZN13ObjectManager15AddNoRoomObjectEP10GameObject']: ADD_NO_ROOM_OP,
            c.symbols['_ZN13ObjectManager18RemoveNoRoomObjectEP10GameObject']: REMOVE_NO_ROOM_OP,
            c.symbols['_ZN8RoomZone9AddObjectEP10GameObject']: ROOM_ADD_OP,
            c.symbols['_ZN10GameObject11ZoneEnteredEv']: ENTER_OP,
            c.symbols['_ZN10GameObject10ZoneExitedEv']: EXIT_OP,
            c.symbols['_ZN12VisualObject10SetVisibleEb']: VISIBLE_OP,
        }
        if address in direct:
            operation = direct[address]
            if operation == ROOM_REMOVE_OP or operation == ROOM_ADD_OP:
                room, obj = c.reg(0), c.reg(1)
                self.record(operation, room, obj)
                if operation == ROOM_ADD_OP and self.scenario == 3:
                    self.set32(obj + 0x2f4, ROOM_B)
            elif operation in (ADD_NO_ROOM_OP, REMOVE_NO_ROOM_OP):
                self.record(operation, c.reg(1))
            elif operation in (ENTER_OP, EXIT_OP):
                obj = c.reg(0)
                self.record(operation, obj, self.get32(obj + 0x2f4))
            else:
                self.record(operation, c.reg(0), self.get32(c.reg(0) + 4), c.reg(1))
            self.return_from_hook()
            return
        if address == 0x38c850:
            room, obj = c.reg(3), c.reg(4)
            self.record(ROOM_ZONED_OP, room, obj, self.get8(room + 0x389))

    def run(self, mode: str, scenario: int):
        c = self.cpu
        self.records = []
        self.scenario = scenario
        self.zonable_calls = 0
        raw_sequences = {
            0: [0x80000000], 1: [1], 2: [0, 1], 3: [0], 4: [1, 0],
            5: [1, 1, 0], 6: [1, 0], 7: [0], 8: [0xffffffff], 9: [0],
            10: [0, 1], 11: [1, 1], 15: [1], 16: [1], 17: [],
        }
        self.raw = raw_sequences[scenario]
        c.pointer(OBJ, VT)
        c.pointer(VT + 0x3c, SET_UPDATING)
        c.pointer(VT + 0xc4, IS_ZONABLE_GAMEOBJECT)
        c.pointer(OBJ + 0x2f4, 0)
        c.pointer(OBJ + 0x2d8, 0)
        c.uc.mem_write(OBJ + 0x2ee, b'\x00')
        c.uc.mem_write(OBJ + 0x2ef, b'\x00')
        c.uc.mem_write(OBJ + 0x2f0, b'\x00')
        c.uc.mem_write(OBJ + 0x80, b'\x01')
        c.uc.mem_write(ROOM_A + 0x389, b'\x01')
        c.uc.mem_write(ROOM_B + 0x389, b'\x00')
        c.pointer(VIS_A + 4, OBJ)
        c.pointer(VIS_A + 8, 0)
        c.pointer(VIS_B + 4, OBJ)
        c.pointer(VIS_B + 8, 0)

        if scenario == 0:
            c.pointer(OBJ + 0x2f4, ROOM_A); c.uc.mem_write(OBJ + 0x2ee, b'\x01')
            c.uc.mem_write(OBJ + 0x2f0, b'\x80')
        elif scenario == 1:
            c.uc.mem_write(OBJ + 0x2ee, b'\x01'); c.uc.mem_write(OBJ + 0x2f0, b'\x02')
        elif scenario == 2:
            c.pointer(OBJ + 0x2f4, ROOM_A); c.uc.mem_write(OBJ + 0x2f0, b'\x80')
        elif scenario == 3:
            c.pointer(OBJ + 0x2f4, ROOM_A)
        elif scenario in (4, 5, 6, 10):
            c.uc.mem_write(OBJ + 0x2ee, b'\x01')
            if scenario in (4, 6): c.pointer(OBJ + 0x2d8, VIS_A)
            if scenario == 4: c.uc.mem_write(OBJ + 0x80, b'\x00')
            if scenario == 5:
                c.pointer(OBJ + 0x2d8, VIS_A); c.uc.mem_write(OBJ + 0x2f0, b'\x00')
            if scenario == 6: c.uc.mem_write(OBJ + 0x2f0, b'\x00')
            if scenario == 10: c.uc.mem_write(OBJ + 0x2f0, b'\x80')
        elif scenario == 7:
            c.pointer(OBJ + 0x2d8, VIS_A); c.uc.mem_write(OBJ + 0x80, b'\x00')
        elif scenario == 8:
            c.pointer(OBJ + 0x2d8, VIS_A)
        elif scenario == 9:
            c.pointer(OBJ + 0x2f4, ROOM_B)
        elif scenario == 11:
            c.pointer(OBJ + 0x2d8, VIS_A); c.uc.mem_write(OBJ + 0x2ee, b'\x01')
            c.uc.mem_write(OBJ + 0x2f0, b'\x01')
        elif scenario == 15:
            c.pointer(OBJ + 0x2d8, VIS_A); c.uc.mem_write(OBJ + 0x2ee, b'\x01')
            c.uc.mem_write(OBJ + 0x2f0, b'\x80')
        elif scenario == 16:
            c.uc.mem_write(OBJ + 0x2ee, b'\x01')
        elif scenario == 17:
            c.pointer(VIS_A + 4, 0)

        symbol = {'d': '_ZN10GameObject13DisableZoningEv',
                  'e': '_ZN10GameObject12EnableZoningEv',
                  's': '_ZN12VisualObject14SyncVisibilityEv'}[mode]
        if mode == 's':
            value = c.invoke(symbol, [VIS_A])
        else:
            value = c.invoke(symbol, [OBJ])
        final = {'zoning': self.get8(OBJ + 0x2ee), 'in_zone': self.get8(OBJ + 0x2f0),
                 'room': self.get32(OBJ + 0x2f4), 'return': value}
        return {'calls': self.records, **final}


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--compiler')
    ap.add_argument('--original-elf', type=Path, required=True)
    ap.add_argument('--output', type=Path, default=MODULE / 'build/game-object-zoning-visibility/host.exe')
    ap.add_argument('--report', type=Path, default=MODULE / 'build/game-object-zoning-visibility/validation.json')
    args = ap.parse_args()
    compiler = args.compiler or os.environ.get('CXX') or shutil.which('g++')
    if not compiler: ap.error('pass --compiler or set CXX')
    exe = args.output.resolve(); exe.parent.mkdir(parents=True, exist_ok=True)
    cpp = MODULE / 'game_object_zoning_visibility.cpp'
    host_test = MODULE / 'tests/game_object_zoning_visibility.cpp'
    subprocess.run([compiler, '-std=c++17', '-O1', '-Wall', '-Wextra', '-Werror', '-pedantic',
                    str(cpp), str(host_test), '-o', str(exe)], check=True, cwd=ROOT)
    host, guard_cases = host_cases(exe)
    if hashlib.sha256(args.original_elf.read_bytes()).hexdigest() != ELF_SHA:
        raise SystemExit('pinned original ELF SHA-256 mismatch')
    manifest = json.loads(MANIFEST.read_text(encoding='utf-8'))
    verify_manifest(args.original_elf.resolve(), manifest)
    cpu = ZoningCpu(args.original_elf.resolve(), manifest)
    source_rows = []
    mismatches = []
    for row in host:
        mode, scenario = row['mode'], row['scenario']
        if scenario in (7, 8, 15) and mode != 's':
            continue
        actual = cpu.run(mode, scenario)
        wanted = row['result']
        expected = {k: wanted[k] for k in ('calls', 'zoning', 'in_zone', 'room')}
        observed = {k: actual[k] for k in ('calls', 'zoning', 'in_zone', 'room')}
        if observed != expected:
            mismatches.append({'mode': mode, 'scenario': scenario, 'host': expected, 'original': observed})
        source_rows.append({'mode': mode, 'scenario': scenario, 'matched': observed == expected,
                            'source': actual, 'host': expected})
    if mismatches:
        raise AssertionError(json.dumps(mismatches, indent=2))

    active_code = set()
    covered = set()
    for row in manifest['functions']:
        start = int(row['elf_address'], 0)
        active_code.update(range(start, start + row['active_code_size'], 4))
    covered = active_code.intersection(cpu.cpu.seen)
    missing = sorted(active_code - covered)
    if missing:
        raise AssertionError('original active-instruction bytes not reached: ' +
                             ', '.join(hex(address) for address in missing))

    # ARM host-independent facts that cannot be represented as 32-bit ELF
    # pointers: the C++ guard suite checks >32-bit opaque identities, malformed
    # projections, known-range alias rejection and retained prior effects.
    report = {
        'validation': 'PASS', 'original_arm_cases': len(source_rows), 'mismatches': 0,
        'host_guard_suite': 'PASS', 'host_guard_cases': guard_cases,
        'pinned_original_sha256': ELF_SHA,
        'active_instructions': len(active_code), 'covered_active_instructions': len(covered),
        'functions': manifest['functions'],
        'vtable_ranges': manifest['vtable_ranges'],
        'source_sha256': {str(p.relative_to(ROOT)).replace('\\', '/'): hashlib.sha256(p.read_bytes()).hexdigest()
                          for p in (cpp, MODULE / 'game_object_zoning_visibility.hpp', host_test,
                                    Path(__file__).resolve(), MANIFEST)},
        'scope': ('Full bounded DisableZoning, EnableZoning and SyncVisibility instruction bodies. '
                  'Room/list/ObjectManager/ZoneEntered/ZoneExited/SetVisible and vtable leaves are typed '
                  'synchronous service boundaries; their own source behavior is covered by separate maps. '
                  'RoomZone identity is a runtime pointer and never inferred from DACT room indices. '
                  'RoomZone AABB construction, native owner wiring, scene-node SetVisible body and allocator/list '
                  'storage remain outside this kernel.'),
        'source_results': source_rows,
    }
    report_path = args.report.resolve(); report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({k: report[k] for k in ('validation', 'original_arm_cases', 'mismatches', 'host_guard_suite')}))


if __name__ == '__main__': main()
