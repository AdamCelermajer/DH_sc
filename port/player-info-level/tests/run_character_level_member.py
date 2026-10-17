"""Differentially run PlayerInfo's source level setter and int member kernel."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess

from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE, UC_HOOK_MEM_READ_UNMAPPED
from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_PC,
                               UC_ARM_REG_LR, UC_ARM_REG_SP)
from elftools.elf.elffile import ELFFile

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
MANIFEST = MODULE / 'reference/character-level-member/original-functions.json'
ORIGINAL_SHA = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
SET_LEVEL = 0x370e48
SET_VALUE = 0x36d9f0
SET_CHANGED = 0x814f84
OWNER = 0x10001000
TABLE = 0x10004000
SERIAL = 0x10008000
STACK_TOP = 0x20008000
STOP = 0x30000000


def original_code(path):
    raw = path.read_bytes()
    assert hashlib.sha256(raw).hexdigest() == ORIGINAL_SHA
    code = {}
    with path.open('rb') as stream:
        elf = ELFFile(stream)
        segments = [s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
        symbols = {s.name: s for s in elf.get_section_by_name('.dynsym').iter_symbols()}
        for item in json.loads(MANIFEST.read_text(encoding='utf-8'))['functions']:
            address = int(item['elf_address'], 0)
            size = item['size']
            symbol = symbols[item['original_symbol']]
            assert (int(symbol['st_value']), int(symbol['st_size'])) == (address, size)
            segment = next(s for s in segments if s['p_vaddr'] <= address and
                           address + size <= s['p_vaddr'] + s['p_filesz'])
            offset = segment['p_offset'] + address - segment['p_vaddr']
            code[address] = raw[offset:offset + size]
            assert hashlib.sha256(code[address]).hexdigest() == item['sha256']
    return code


def put32(uc, address, value):
    uc.mem_write(address, struct.pack('<I', value & 0xffffffff))


def get32(uc, address):
    return struct.unpack('<I', uc.mem_read(address, 4))[0]


def put64(uc, address, value):
    uc.mem_write(address, struct.pack('<Q', value & 0xffffffffffffffff))


def get64(uc, address):
    return struct.unpack('<Q', uc.mem_read(address, 8))[0]


def original(code, initial, incoming, residue, serial, stamp, changed):
    uc = Uc(UC_ARCH_ARM, UC_MODE_ARM)
    for base, size in ((0x36d000, 0x1000), (0x370000, 0x1000),
                       (0x814000, 0x1000), (0x990000, 0x10000),
                       (0x10000000, 0x20000), (0x20000000, 0x10000),
                       (0x30000000, 0x1000)):
        uc.mem_map(base, size)
    uc.mem_write(SET_LEVEL, code[SET_LEVEL])
    uc.mem_write(SET_VALUE, code[SET_VALUE])
    uc.mem_write(SET_CHANGED, code[SET_CHANGED])

    # SetCharacterLevel's two literal GOT references resolve to the concrete
    # NetStructMemberType<int> vtable family in the pinned source image.
    got_base = 0x994a8c
    put32(uc, got_base + 0x2984, TABLE - 8)
    put32(uc, got_base + 0x353c, TABLE - 8)
    # The original SetChanged body resolves this global through its own GOT
    # literal pair and updates the pointed-to 64-bit serial.
    put32(uc, 0x995914, SERIAL)
    put32(uc, TABLE + 0x1c, SET_VALUE)
    put32(uc, OWNER + 0x310, TABLE)
    put32(uc, OWNER + 0x320, 0x11111111)
    put32(uc, OWNER + 0x324, 0x22222222)
    put32(uc, OWNER + 0x328, stamp)
    uc.mem_write(OWNER + 0x32c, bytes([changed & 0xff, 0, 0, 0]))
    put32(uc, OWNER + 0x330, initial)
    put64(uc, OWNER + 0x318, 0x1122334455667788)
    put64(uc, SERIAL, serial)
    # The ARM body reads its not-yet-written local temporary value here.
    put32(uc, STACK_TOP - 0x20, residue)

    marks = []
    def hook(machine, address, size, context):
        if address == SET_CHANGED:
            member = machine.reg_read(UC_ARM_REG_R0)
            member_offset = -1 if member == STACK_TOP - 0x40 else member - OWNER
            marks.append({'member_offset': member_offset})
        elif address == STOP:
            machine.emu_stop()

    uc.hook_add(UC_HOOK_CODE, hook)
    def unmapped(machine, access, address, size, value, context):
        print(f"unmapped read address=0x{address:x} pc=0x{machine.reg_read(UC_ARM_REG_PC):x}")
        return False
    uc.hook_add(UC_HOOK_MEM_READ_UNMAPPED, unmapped)
    uc.reg_write(UC_ARM_REG_R0, OWNER)
    uc.reg_write(UC_ARM_REG_R1, incoming)
    uc.reg_write(UC_ARM_REG_SP, STACK_TOP)
    uc.reg_write(UC_ARM_REG_LR, STOP)
    uc.emu_start(SET_LEVEL, STOP + 4, count=256)
    assert uc.reg_read(UC_ARM_REG_PC) == STOP
    return {
        'value': struct.unpack('<i', uc.mem_read(OWNER + 0x330, 4))[0],
        'serial': get64(uc, SERIAL),
        'revision': get64(uc, OWNER + 0x318),
        'field10': get32(uc, OWNER + 0x320),
        'field14': get32(uc, OWNER + 0x324),
        'stamp': get32(uc, OWNER + 0x328),
        'changed': uc.mem_read(OWNER + 0x32c, 1)[0],
        'temporary_marked': sum(1 for mark in marks if mark['member_offset'] < 0),
        'member_marked': sum(1 for mark in marks if mark['member_offset'] == 0x310),
        'member_value_changed': 1 if struct.unpack('<i', uc.mem_read(OWNER + 0x330, 4))[0] != initial else 0,
        'marks': marks,
    }


def run_host(executable, args):
    return json.loads(subprocess.check_output([str(executable), *map(str, args)], text=True))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', default='g++')
    parser.add_argument('--original-elf', type=Path,
                        default=ROOT.parent / 'test_strategy/libDungeonHunter2.so')
    parser.add_argument('--output', type=Path, default=MODULE / 'build/character-level-member/host.exe')
    parser.add_argument('--report', type=Path, default=MODULE / 'build/character-level-member/validation.json')
    args = parser.parse_args()
    original_path = args.original_elf.resolve()
    code = original_code(original_path)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    sources = [MODULE / 'character_level_member.cpp', MODULE / 'tests/character_level_member.cpp']
    command = [args.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror', '-pedantic',
               '-I', str(MODULE), *map(str, sources), '-o', str(args.output.resolve())]
    subprocess.run(command, cwd=ROOT, check=True, capture_output=True, text=True)
    exe = args.output.resolve()
    host = json.loads(subprocess.check_output([str(exe)], text=True))
    assert host == {'validation': 'PASS', 'host_cases': 8}

    scenarios = [
        (-1, 100, 0, 10, 55, 0),
        (10, 10, 10, 22, 7, 0),
        (7, 7, 0, 31, 99, 0),
        (-1, -1, -1, 100, 4, 0),
        (5, 12, 12, 0xfffffffffffffffe, 0xabcdef01, 0xff),
        (-1, 12, 13, 0xffffffffffffffff, 0x12345678, 1),
        (10, -3, 9, 0x123456789abcdef0, 0x0, 0),
        (8, 17, 8, 0, 0xffffffff, 255),
    ]
    comparisons = []
    for scenario in scenarios:
        initial, incoming, residue, serial, stamp, changed = scenario
        actual = run_host(exe, scenario)
        expected = original(code, *scenario)
        compact = {k: expected[k] for k in actual}
        assert actual == compact, (scenario, actual, compact, expected)
        expected_marks = [x['member_offset'] for x in expected['marks']]
        actual_marks = ([-1] if actual['temporary_marked'] else []) + \
                       ([0x310] if actual['member_marked'] else [])
        assert actual_marks == expected_marks, (scenario, actual_marks, expected_marks)
        comparisons.append({'inputs': list(scenario), 'mark_order': expected_marks,
                            'matched': True, 'result': expected})

    files = sources + [MODULE / 'character_level_member.hpp', Path(__file__).resolve(),
                       MANIFEST, MANIFEST.with_name('NOTES.md')]
    report = {
        'validation': 'PASS', 'host_cases': host['host_cases'],
        'original_arm_cases': len(comparisons), 'mismatches': 0,
        'original_sha256': ORIGINAL_SHA,
        'scope': ('Executes the original PlayerInfo::SetCharacterLevel, '
                  'NetStructMemberType<int>::SetValue and NetStructMember::SetChanged '
                  'instructions. The only adapter is the synthetic vtable/global-serial '
                  'binding; SetChanged itself is not intercepted. Stack residue at the '
                  'source local read is explicit.'),
        'comparisons': comparisons, 'compiler_command': command,
        'source_sha256': {f.relative_to(ROOT).as_posix(): hashlib.sha256(f.read_bytes()).hexdigest()
                          for f in files},
        'native_wired': False,
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({k: report[k] for k in ('validation', 'host_cases', 'original_arm_cases', 'mismatches')}))


if __name__ == '__main__':
    main()
