"""Compare four maintained CharAIScript state leaves with original instructions."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_PC, UC_ARM_REG_LR

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
MANIFEST = MODULE / 'reference/ais-state-callbacks/original-functions.json'
SHA = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
ADDRESSES = (0x3d8eb4, 0x3d8ea0, 0x3d8e8c, 0x3d8e78)
OFFSETS = (0x14, 0x2c, 0x44, 0x5c)
AIS, TABLE, NEXT, NAMES, STOP = 0x10001000, 0x10002000, 0x10003000, 0x10004000, 0x1003f000
LUA_CALL = 0x37c514
NAMES_TEXT = ('TickState', 'CheckState', 'InitState', 'PostState')


def image(path):
    raw = path.read_bytes()
    assert hashlib.sha256(raw).hexdigest() == SHA, 'unexpected original ELF'
    with path.open('rb') as stream:
        elf = ELFFile(stream)
        segments = [s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
        symbols = {s.name: s for s in elf.get_section_by_name('.dynsym').iter_symbols()}
        manifest = json.loads(MANIFEST.read_text())
        code = {}
        for record in manifest['functions']:
            address = int(record['elf_address'], 0)
            size = record['size']
            symbol = symbols[record['original_symbol']]
            assert (int(symbol['st_value']), int(symbol['st_size'])) == (address, size)
            segment = next(s for s in segments if s['p_vaddr'] <= address and
                           address + size <= s['p_vaddr'] + s['p_filesz'])
            at = segment['p_offset'] + address - segment['p_vaddr']
            code[address] = raw[at:at + size]
            assert hashlib.sha256(code[address]).hexdigest() == record['sha256']
        return code


def original_case(code, index, mode, mutation):
    uc = Uc(UC_ARCH_ARM, UC_MODE_ARM)
    uc.mem_map(0x3d8000, 4096)
    uc.mem_map(0x37c000, 4096)
    uc.mem_map(0x10000000, 0x40000)
    for address in ADDRESSES:
        uc.mem_write(address, code[address])

    def put(address, value):
        uc.mem_write(address, struct.pack('<I', value))

    def get(address):
        return struct.unpack('<I', uc.mem_read(address, 4))[0]

    def string(address):
        if not address:
            return None
        data = bytearray()
        while True:
            value = uc.mem_read(address + len(data), 1)[0]
            if not value:
                return data.decode('ascii')
            data.append(value)
            assert len(data) < 80

    put(AIS + 0xb4, 0 if mode == 0 else TABLE)
    for slot, text in zip(OFFSETS, NAMES_TEXT):
        pointer = NAMES + slot * 8
        uc.mem_write(pointer, text.encode() + b'\0')
        put(TABLE + slot, pointer)
    if mode == 2:
        put(TABLE + OFFSETS[index], 0)
    if mode == 3:
        put(TABLE + OFFSETS[index], NAMES)
    captured = {}

    def hook(machine, address, size, context):
        if address == LUA_CALL:
            captured.update(ais=machine.reg_read(UC_ARM_REG_R0),
                            name=string(machine.reg_read(UC_ARM_REG_R1)))
            if mutation == 1:
                put(AIS + 0xb4, 0)
            if mutation == 2:
                put(AIS + 0xb4, NEXT)
            if mutation == 3:
                uc.mem_write(NAMES, b'ChangedCheck\0')
                put(TABLE + 0x2c, NAMES)
            machine.reg_write(UC_ARM_REG_PC, machine.reg_read(UC_ARM_REG_LR))
        if address == STOP:
            machine.emu_stop()

    uc.hook_add(UC_HOOK_CODE, hook)
    uc.reg_write(UC_ARM_REG_R0, AIS)
    uc.reg_write(UC_ARM_REG_LR, STOP)
    uc.emu_start(ADDRESSES[index], STOP + 4, count=32)
    assert uc.reg_read(UC_ARM_REG_PC) == STOP, 'wrapper did not return'
    called = bool(captured)
    return {'status': 0, 'calls': int(called), 'ais': captured.get('ais', 0),
            'table': TABLE if called else 0, 'offset': OFFSETS[index] if called else 0,
            'name': captured.get('name'), 'active': get(AIS + 0xb4)}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', required=True)
    parser.add_argument('--original-elf', type=Path, required=True)
    parser.add_argument('--output', type=Path, default=MODULE/'build/ais-state-callbacks/host.exe')
    parser.add_argument('--report', type=Path, default=MODULE/'build/ais-state-callbacks/validation.json')
    args = parser.parse_args()
    code = image(args.original_elf.resolve())
    sources = [MODULE/'ais_state_callbacks.cpp', MODULE/'tests/ais_state_callbacks.cpp']
    args.output.parent.mkdir(parents=True, exist_ok=True)
    command = [args.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror', '-pedantic',
               *map(str, sources), '-o', str(args.output.resolve())]
    subprocess.run(command, cwd=ROOT, check=True, capture_output=True, text=True)
    executable = str(args.output.resolve())
    host = json.loads(subprocess.check_output([executable], text=True))
    assert host == {'validation': 'PASS', 'host_cases': 18}
    records = []
    for index in range(4):
        for mode in range(4):
            for mutation in ((0,) if mode == 0 else (0, 1, 2, 3)):
                actual = json.loads(subprocess.check_output([executable, str(index), str(mode), str(mutation)], text=True))
                expected = original_case(code, index, mode, mutation)
                assert actual == expected, (index, mode, mutation, actual, expected)
                records.append({'callback': index, 'mode': mode, 'mutation': mutation,
                                'matched': True, 'result': expected})
    paths = sources + [MODULE/'ais_state_callbacks.hpp', Path(__file__).resolve(), MANIFEST,
                       MANIFEST.with_name('NOTES.md')]
    report = {'validation': 'PASS', 'host_cases': host['host_cases'],
              'original_arm_cases': len(records), 'mismatches': 0, 'original_sha256': SHA,
              'compiler_command': command, 'results': records,
              'source_sha256': {p.relative_to(ROOT).as_posix(): hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},
              'native_wired': False,
              'scope': 'Four complete original20B state wrappers execute; LuaScript::Call is a fixture boundary. Null-table, exact name-pointer selection, null/empty names and retained callback mutations compare. Additional host sequences verify fresh table/name reads between calls. Original state registration and VM bodies remain external.'}
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({k: report[k] for k in ('validation', 'host_cases', 'original_arm_cases', 'mismatches')}))


if __name__ == '__main__':
    main()
