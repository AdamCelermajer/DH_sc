"""Replay AISExternal::OnUpdate and compare the maintained caller prefix."""
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

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
MANIFEST = MODULE / 'reference/ais-external-update/original-functions.json'
ORIGINAL_SHA = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
AIS, OWNER_A, OWNER_B = 0x10001000, 0x10002000, 0x10003000
CONTROLLER_A, CONTROLLER_B = 0x10004000, 0x10005000
TABLE, NAMES, STACK, STOP = 0x10020000, 0x10024000, 0x1003e000, 0x1003f000
DEFAULT_UPDATE, EXTERNAL_UPDATE = 0x3dc798, 0x3dce64
PAUSE, CONTROLLER_STOP, LUA_CALL = 0x3cb748, 0x40559c, 0x37c514


def fixtures():
    return [
        ('empty_no_override', 0, 0, 0, 0),
        ('counter_199_no_pause', 199, 0, 0, 0),
        ('counter_200_pause_stop', 200, 0, 0, 0),
        ('counter_max_pause_stop', 0xffffffff, 0, 0, 0),
        ('state_callbacks_no_override', 0, 0, 1, 0),
        ('override_and_state_callbacks', 0, 1, 1, 0),
        ('noncanonical_flag_without_bit0', 0, 2, 1, 0),
        ('pause_replaces_owner_before_stop', 200, 0, 0, 1),
        ('pause_sets_live_override_flag', 200, 0, 0, 2),
        ('pause_clears_live_override_flag', 200, 1, 1, 3),
        ('override_registers_live_state_table', 0, 1, 0, 4),
        ('state_update_removes_table_before_conditions', 0, 0, 1, 5),
        ('override_mutates_counter', 0, 1, 0, 6),
    ]


def original_image(path: Path):
    from elftools.elf.elffile import ELFFile
    raw = path.read_bytes()
    if hashlib.sha256(raw).hexdigest() != ORIGINAL_SHA:
        raise RuntimeError('original ELF SHA-256 differs from pinned source')
    with path.open('rb') as stream:
        elf = ELFFile(stream)
        if elf.elfclass != 32 or elf['e_machine'] != 'EM_ARM' or not elf.little_endian:
            raise RuntimeError('expected little-endian ARM32 original library')
        segments = [(int(s['p_vaddr']), int(s['p_filesz']), int(s['p_memsz']), int(s['p_offset']))
                    for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
        symbols = {s.name: s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        for item in json.loads(MANIFEST.read_text(encoding='utf-8'))['functions']:
            address = int(item['elf_address'], 0)
            symbol = symbols[item['original_symbol']]
            if (int(symbol['st_value']), int(symbol['st_size'])) != (address, item['size']):
                raise RuntimeError(f"manifest symbol mismatch: {item['original_symbol']}")
            seg = next(s for s in segments if s[0] <= address and address + item['size'] <= s[0] + s[1])
            offset = seg[3] + address - seg[0]
            if hashlib.sha256(raw[offset:offset + item['size']]).hexdigest() != item['sha256']:
                raise RuntimeError(f"manifest byte hash mismatch: {item['original_symbol']}")
    return raw, segments


def original_case(original: Path, values):
    from elftools.elf.elffile import ELFFile
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_PC,
                                   UC_ARM_REG_LR, UC_ARM_REG_SP)
    raw, segments = original_image(original)
    with original.open('rb') as stream:
        elf = ELFFile(stream)
        image_end = max(base + memory for base, _, memory, _ in segments)
        machine = Uc(UC_ARCH_ARM, UC_MODE_ARM)
        machine.mem_map(0, (image_end + 4095) & ~4095)
        for base, size, _, offset in segments:
            machine.mem_write(base, raw[offset:offset + size])
        machine.mem_map(0x10000000, 0x40000)

    def put(address, word):
        machine.mem_write(address, struct.pack('<I', word & 0xffffffff))
    def get(address):
        return struct.unpack('<I', machine.mem_read(address, 4))[0]
    def cstr(address):
        out = bytearray()
        while len(out) < 128:
            value = machine.mem_read(address + len(out), 1)[0]
            if not value:
                break
            out.append(value)
        return out.decode('ascii')

    counter, flags, table_present, mutation = values
    put(AIS + 0x98, OWNER_A)
    put(AIS + 0xb8, flags)
    put(AIS + 0xbc, counter)
    put(AIS + 0xb4, TABLE if table_present else 0)
    put(OWNER_A + 0x378, CONTROLLER_A)
    put(OWNER_B + 0x378, CONTROLLER_B)
    put(TABLE + 0x14, NAMES)
    put(TABLE + 0x2c, NAMES + 10)
    machine.mem_write(NAMES, b'TickState\0CheckState\0')
    calls = []
    def hook(uc, address, size, context):
        if address == STOP:
            uc.emu_stop()
            return
        if address == PAUSE:
            owner_ai = uc.reg_read(UC_ARM_REG_R0)
            delay = uc.reg_read(UC_ARM_REG_R1)
            calls.append([0, owner_ai, delay, ''])
            if mutation == 1:
                put(AIS + 0x98, OWNER_B)
            elif mutation == 2:
                put(AIS + 0xb8, get(AIS + 0xb8) | 1)
            elif mutation == 3:
                put(AIS + 0xb8, get(AIS + 0xb8) & ~1)
            uc.reg_write(UC_ARM_REG_PC, uc.reg_read(UC_ARM_REG_LR))
            return
        if address == CONTROLLER_STOP:
            calls.append([1, uc.reg_read(UC_ARM_REG_R0), 0, ''])
            uc.reg_write(UC_ARM_REG_PC, uc.reg_read(UC_ARM_REG_LR))
            return
        if address == LUA_CALL:
            name = cstr(uc.reg_read(UC_ARM_REG_R1))
            calls.append([2, uc.reg_read(UC_ARM_REG_R0), 0, name])
            if name == 'OnUpdate' and mutation == 4:
                put(AIS + 0xb4, TABLE)
            elif name == 'OnUpdate' and mutation == 6:
                put(AIS + 0xbc, 0x12345678)
            elif name == 'TickState' and mutation == 5:
                put(AIS + 0xb4, 0)
            uc.reg_write(UC_ARM_REG_PC, uc.reg_read(UC_ARM_REG_LR))

    machine.hook_add(UC_HOOK_CODE, hook)
    machine.reg_write(UC_ARM_REG_R0, AIS)
    machine.reg_write(UC_ARM_REG_SP, STACK)
    machine.reg_write(UC_ARM_REG_LR, STOP)
    machine.emu_start(EXTERNAL_UPDATE, STOP + 4, count=1000)
    if machine.reg_read(UC_ARM_REG_PC) != STOP:
        raise RuntimeError('original OnUpdate did not reach return sentinel')
    return {
        'status': 0,
        'counter': get(AIS + 0xbc),
        'owner': get(AIS + 0x98),
        'flags': get(AIS + 0xb8),
        'table': int(bool(get(AIS + 0xb4))),
        'pause_due': int(any(call[0] == 0 for call in calls)),
        'on_update': int(any(call[3] == 'OnUpdate' for call in calls)),
        'calls': calls,
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--original-elf', type=Path, required=True)
    parser.add_argument('--compiler', default=os.environ.get('CXX') or shutil.which('g++'))
    parser.add_argument('--output', type=Path, default=MODULE/'build/ais-external-update/host.exe')
    parser.add_argument('--report', type=Path, default=MODULE/'build/ais-external-update/validation.json')
    args = parser.parse_args()
    if not args.compiler:
        parser.error('pass --compiler or install g++')
    original = args.original_elf.resolve()
    original_image(original)
    sources = [MODULE/'ais_external_update.cpp', MODULE/'tests/ais_external_update_host.cpp']
    args.output.parent.mkdir(parents=True, exist_ok=True)
    command = [args.compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror', '-pedantic',
               '-O2', *(str(source) for source in sources), '-o', str(args.output.resolve())]
    subprocess.run(command, cwd=ROOT, check=True, capture_output=True, text=True)
    exe = str(args.output.resolve())
    host = json.loads(subprocess.check_output([exe], text=True))
    if host.get('validation') != 'PASS' or host.get('ais_external_update_host_cases') != 15:
        raise RuntimeError(f'host fixture failure: {host}')
    rows = []
    for name, counter, flags, table_present, mutation in fixtures():
        args_words = [counter, flags, table_present, mutation, 0]
        compiled = json.loads(subprocess.check_output([exe, *map(str, args_words)], text=True))
        actual = {key: compiled[key] for key in ('status', 'counter', 'owner', 'flags', 'table',
                                                  'pause_due', 'on_update', 'calls')}
        expected = original_case(original, args_words[:4])
        if actual != expected:
            raise RuntimeError(f'{name}: compiled/original mismatch\n{actual}\n{expected}')
        rows.append({'case': name, 'matched': True, 'result': expected})
    paths = [*sources, MODULE/'ais_external_update.hpp', Path(__file__).resolve(), MANIFEST,
             MANIFEST.with_name('NOTES.md')]
    report = {
        'validation': 'PASS', 'host_cases': host['ais_external_update_host_cases'],
        'original_arm_cases': len(rows), 'mismatches': 0, 'original_sha256': ORIGINAL_SHA,
        'compiler_command': command, 'results': rows,
        'source_sha256': {path.relative_to(ROOT).as_posix(): hashlib.sha256(path.read_bytes()).hexdigest()
                          for path in paths},
        'scope': 'Original AISExternal::OnUpdate and AISDefault::OnUpdate caller instructions execute. AI_PauseUpdate, Cmd_Stop and LuaScript::Call are observed fixture boundaries; CallStateUpdate/Conditions original null-check wrappers execute and their Lua calls are hooked. The fixture does not execute Android gameplay or source VM callback bodies.',
        'native_wired': False,
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({key: report[key] for key in ('validation', 'host_cases', 'original_arm_cases', 'mismatches')}))


if __name__ == '__main__':
    main()
