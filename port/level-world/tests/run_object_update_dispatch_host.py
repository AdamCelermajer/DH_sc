"""Host guards and execute the bounded ObjectManager per-object slice in ARM."""
from __future__ import annotations
import argparse
import hashlib
import json
import os
from pathlib import Path
import struct
import subprocess
import sys

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
MANIFEST = MODULE / 'reference/object-update-dispatch/original-functions.json'
SHA = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
OBJECT_MANAGER = '_ZN13ObjectManager6UpdateEf'
OBJECT = 0x02001000
NODE = 0x02002000
VTABLE = 0x02003000
CHARACTER_CALLBACK = 0x02004000
REMOTE_CALLBACK = 0x02004010
UPDATE_CALLBACK = 0x02004020
ONLINE_RECORD = 0x02005000
MANAGER_DATA = 0x02006000
STOP_UPDATE = 0x34A850
STOP_SKIP_NO_UNLOAD = 0x34A8CC
STOP_SKIP_UNLOAD = 0x34AA5C
STOP_DELETION = 0x34A9E0


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def fixtures():
    # first/second IsCharacter, gate85, gate8a, online, remote, deletion,
    # initial phase, initial marker, AIPointers mutates gate85, manager arg1,
    # manager arg3. Deletion rows stop at the explicit unsupported boundary.
    rows = [
        ('character_update', [1, 1, 1, 1, 0, 0, 0, 2, 77, 0, 0xabc, 0xdef]),
        ('noncharacter_update', [0, 0, 1, 1, 0, 0, 0, 2, 77, 0, 0xabc, 0xdef]),
        ('aipointer_mutates_gate_offline', [1, 1, 1, 1, 0, 0, 0, 9, 12, 1, 0xabc, 0xdef]),
        ('offline_before_online_query', [0, 0, 0, 1, 0, 0, 0, 9, 12, 0, 0xabc, 0xdef]),
        ('online_nonremote_unloads_character', [1, 1, 0, 1, 1, 0, 0, 9, 12, 0, 0xabc, 0xdef]),
        ('online_nonremote_second_not_character', [1, 0, 0, 1, 1, 0, 0, 9, 12, 0, 0xabc, 0xdef]),
        ('online_remote_updates', [1, 1, 0, 1, 1, 0x80000000, 0, 9, 12, 0, 0xabc, 0xdef]),
        ('remote_raw_one_updates', [0x80000000, 0, 1, 0, 255, 0xffffffff, 0, 255, 201, 0, 0xabc, 0xdef]),
        ('phase_reset_unconditional', [0, 0, 0, 0, 0, 0, 0, 255, 255, 0, 0xabc, 0xdef]),
        ('gate8a_alone_requests_online', [0, 0, 1, 0, 0, 0, 0, 12, 31, 0, 0xabc, 0xdef]),
        ('online_zero_ignores_remote', [0, 0, 0, 0, 0, 0xffffffff, 0, 3, 4, 0, 0xabc, 0xdef]),
        ('phase_nonzero_reset_after_aipointer_mutation', [1, 1, 1, 1, 0, 0, 0, 128, 255, 1, 0xabc, 0xdef]),
    ]
    for mask in (2,4,8,16,32,64,128,256,1024,2048):
        rows.append((f'callback_mutation_{mask}',
            [1,1,0 if mask in (4,8,16,32,64,256) else 1,1,
             1 if mask in (16,32) else 0,1,0,7,9,mask,0xabc,0xdef]))
    for gate85,gate8a in ((255,255),(128,1),(0,255),(255,0)):
        rows.append((f'raw_gates_{gate85}_{gate8a}',[0x80000000,0xffffffff,
            gate85,gate8a,255,0x80000000,0,255,255,0,0xabc,0xdef]))
    for byte in (1,128,255):
        rows.append((f'deletion_boundary_{byte}',[1,1,1,1,0,0,byte,7,9,0,0xabc,0xdef]))
    rows.append(('offline_deletion_still_unloads',[1,1,0,1,0,0,255,7,9,0,0xabc,0xdef]))
    return rows


def verify_original(path: Path):
    from elftools.elf.elffile import ELFFile
    manifest = json.loads(MANIFEST.read_text(encoding='utf-8'))
    raw = path.read_bytes()
    assert hashlib.sha256(raw).hexdigest() == SHA
    with path.open('rb') as stream:
        elf = ELFFile(stream)
        assert elf.elfclass == 32 and elf['e_machine'] == 'EM_ARM' and elf.little_endian
        symbols = {s.name: s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        segments = [(int(s['p_vaddr']), int(s['p_filesz']), int(s['p_offset']))
                    for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
        for row in manifest['functions']:
            sym = symbols[row['original_symbol']]
            address, size = int(row['elf_address'], 0), int(row['size'])
            assert (int(sym['st_value']), int(sym['st_size'])) == (address, size)
            start = next(off + address - base for base, count, off in segments
                         if base <= address and address + size <= base + count)
            assert hashlib.sha256(raw[start:start + size]).hexdigest() == row['sha256']
        for row in manifest['vtable_symbols']:
            sym = symbols[row['symbol']]
            address, size = int(row['address'], 0), int(row['size'])
            assert (int(sym['st_value']), int(sym['st_size'])) == (address, size)
            start = next(off + address - base for base, count, off in segments
                         if base <= address and address + size <= base + count)
            assert hashlib.sha256(raw[start:start + size]).hexdigest() == row['sha256']
        for row in manifest['source_ranges']:
            address, size = int(row['start'], 0), int(row['size'])
            start = next(off + address - base for base, count, off in segments
                         if base <= address and address + size <= base + count)
            assert hashlib.sha256(raw[start:start + size]).hexdigest() == row['sha256']
        for row in manifest['vtable_entries']:
            address = int(row['entry_address'], 0)
            start = next(off + address - base for base, count, off in segments
                         if base <= address and address + 4 <= base + count)
            value = struct.unpack_from('<I', raw, start)[0]
            assert value == int(row['entry_value'], 0)
            assert hashlib.sha256(raw[start:start + 4]).hexdigest() == row['sha256']
    return manifest


def host_result(binary: Path, row):
    name, v = row
    # The executable's first/second Character facts are test services, while
    # all bytes and calls remain independently compared to the source slice.
    args = [v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], 0,
            1, v[10], v[11]]
    return json.loads(subprocess.check_output([str(binary), *map(str, args)], text=True))


def original_result(cpu, manifest, row):
    from unicorn import UC_HOOK_CODE, UC_HOOK_MEM_WRITE
    from unicorn.arm_const import (UC_ARM_REG_PC, UC_ARM_REG_LR, UC_ARM_REG_SP,
                                   UC_ARM_REG_R4, UC_ARM_REG_R7)
    name, v = row
    first, second, gate85, gate8a, online, remote, deletion, phase, marker, mutate, arg1, arg3 = v
    uc = cpu.uc
    # Reuse the verified ELF image, resetting all fixture data/frame registers.
    # No provider body executes or mutates ELF-owned globals in this slice.
    uc.mem_write(cpu.data,bytes(0x8000))
    uc.mem_write(cpu.stack+0xe000,bytes(0x100))
    for index in range(13):cpu.put(index,0)
    source_object = cpu.data + 0x1000
    node = cpu.data + 0x2000
    vtable = cpu.data + 0x3000
    character_entry, remote_entry, update_entry = cpu.data + 0x4000, cpu.data + 0x4010, cpu.data + 0x4020
    online_record = cpu.data + 0x5000
    manager = cpu.data + 0x6000
    stack = cpu.stack + 0xe000
    for address, value in ((node + 8, source_object),
                           (source_object, vtable + 8),
                           (manager + 0x44, arg3), (stack + 0x18, 0x44),
                           (stack + 0x1c, arg1)):
        cpu.pointer(address, value)
    uc.mem_write(source_object + 0x81, bytes([deletion, 0, 0, 0, gate85,
                                              phase, 0, marker, 0, gate8a]))
    uc.mem_write(online_record, bytes(5) + bytes([online]))
    # Copy the actual Character vtable before installing only explicit caller
    # boundary callbacks. This pins the source address point and virtual slots.
    table_bytes = uc.mem_read(cpu.base + 0x965f30, 812)
    uc.mem_write(vtable, bytes(table_bytes))
    cpu.pointer(vtable + 8 + 0x24, character_entry)
    cpu.pointer(vtable + 8 + 0x2c, update_entry)
    cpu.pointer(vtable + 8 + 0x54, remote_entry)
    next_vtable=vtable+0x800
    next_character,next_remote,next_update=character_entry+0x100,remote_entry+0x100,update_entry+0x100
    uc.mem_write(next_vtable,bytes(table_bytes))
    cpu.pointer(next_vtable+8+0x24,next_character)
    cpu.pointer(next_vtable+8+0x54,next_remote)
    cpu.pointer(next_vtable+8+0x2c,next_update)
    calls, char_calls, writes, covered, virtual_targets = [], [0], [], set(), []
    source_ranges=[(int(r['start'],0),int(r['start'],0)+r['size']) for r in manifest['source_ranges']]
    stop = [None]
    def ret(address, value=0):
        cpu.put(0, value)
        uc.reg_write(UC_ARM_REG_PC, uc.reg_read(UC_ARM_REG_LR))
    def callback(uc, address, size, unused):
        if address in (STOP_UPDATE, STOP_SKIP_NO_UNLOAD, STOP_SKIP_UNLOAD,STOP_DELETION):
            stop[0] = address
            uc.emu_stop()
            return
        if any(start<=address<end for start,end in source_ranges):
            covered.add(address)
        if address in (character_entry,next_character):
            this = cpu.reg(0)
            assert this == source_object
            result = first if char_calls[0] == 0 else second
            virtual_targets.append(hex(address))
            if not char_calls[0]:
                if mutate&4:
                    uc.mem_write(source_object+0x85,b'\1');uc.mem_write(source_object+0x8a,b'\1')
                if mutate&1024:cpu.pointer(node+8,source_object+0x800)
                if mutate&2048:cpu.pointer(source_object,next_vtable+8)
            else:
                if mutate&64:uc.mem_write(source_object+0x86,bytes([111]))
                if mutate&256:
                    cpu.pointer(stack+0x1c,0x123);cpu.pointer(manager+0x44,0x456)
            char_calls[0] += 1
            calls.append([0, 0x24, 0, 0, 0])
            ret(address, result)
        elif address == 0x3a4344:
            assert cpu.reg(0) == source_object
            calls.append([1, 0x3a4344, 0, 0, 0])
            if mutate&1: uc.mem_write(source_object + 0x85, b'\0')
            if mutate&2: uc.mem_write(source_object + 0x8a, b'\0')
            ret(address, 0xfeed)
        elif address == 0x7fd794:
            calls.append([2, 0x7fd794, 0, 0, 0])
            if mutate&8:
                uc.mem_write(source_object+0x85,b'\1');uc.mem_write(source_object+0x8a,b'\1')
            if mutate&16:uc.mem_write(source_object+0x81,bytes([7]))
            cpu.put(0, online_record)
            uc.reg_write(UC_ARM_REG_PC, uc.reg_read(UC_ARM_REG_LR))
        elif address in (remote_entry,next_remote):
            assert cpu.reg(0) == source_object
            calls.append([3, 0x54, 0, 0, 0])
            virtual_targets.append(hex(address))
            if mutate&32:uc.mem_write(source_object+0x81,bytes([7]))
            ret(address, remote)
        elif address == 0x3a7b24:
            this = cpu.reg(0)
            source_arg1, source_arg2, source_arg3 = [cpu.reg(i) for i in (1, 2, 3)]
            assert this == source_object
            calls.append([4, 0x3a7b24, source_arg1, source_arg2, source_arg3])
            ret(address)
        elif address in (update_entry,next_update):
            assert cpu.reg(0) == source_object
            calls.append([5, 0x2c, 0, 0, 0])
            virtual_targets.append(hex(address))
            if mutate&128:uc.mem_write(source_object+0x88,bytes([9]))
            ret(address, 0xa5)
        elif address in (0x3a4344, 0x7fd794, 0x3a7b24):
            raise AssertionError(f'unexpected duplicate callback {address:#x}')
    def write_hook(uc, access, address, size, value, unused):
        if address in (source_object + 0x86, source_object + 0x88):
            writes.append(address - source_object)
    code_hook=uc.hook_add(UC_HOOK_CODE, callback)
    memory_hook=uc.hook_add(UC_HOOK_MEM_WRITE, write_hook)
    uc.reg_write(UC_ARM_REG_SP, stack)
    uc.reg_write(UC_ARM_REG_R4, node)
    uc.reg_write(UC_ARM_REG_R7, manager)
    uc.reg_write(UC_ARM_REG_LR, cpu.stop)
    uc.emu_start(0x34a7f4, cpu.stop + 4, count=2000)
    uc.hook_del(code_hook);uc.hook_del(memory_hook)
    assert stop[0] in (STOP_UPDATE, STOP_SKIP_NO_UNLOAD, STOP_SKIP_UNLOAD,STOP_DELETION), (name, hex(uc.reg_read(UC_ARM_REG_PC)), stop[0])
    expected_stop = (STOP_DELETION if stop[0]==STOP_DELETION else STOP_UPDATE if calls and calls[-1][0] == 5 else
                     STOP_SKIP_UNLOAD if calls and calls[-1][0] == 4 else STOP_SKIP_NO_UNLOAD)
    assert stop[0] == expected_stop, (name, stop[0], calls)
    status = 6 if expected_stop==STOP_DELETION else 0
    route = 0 if expected_stop==STOP_DELETION else 2 if expected_stop == STOP_UPDATE else 1
    return {'status': status, 'route': route, 'calls': len(calls),
            'phase_writes': writes.count(0x86), 'marker_writes': writes.count(0x88),
            'phase': uc.mem_read(source_object + 0x86, 1)[0],
            'marker': uc.mem_read(source_object + 0x88, 1)[0],
            'trace': calls, 'instructions': len(covered),
            'instruction_addresses':[hex(at) for at in sorted(covered)],
            'virtual_targets':virtual_targets,
            'stop_address':hex(stop[0])}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--original-elf', type=Path, required=True)
    parser.add_argument('--compiler', default=os.environ.get('CXX', 'g++'))
    parser.add_argument('--output', type=Path, default=MODULE / 'build/object-update-dispatch/host.exe')
    parser.add_argument('--report', type=Path, default=MODULE / 'build/object-update-dispatch/validation.json')
    args = parser.parse_args()
    sources = [MODULE / 'object_update_dispatch.cpp', MODULE / 'tests/object_update_dispatch.cpp']
    inputs=[MODULE/'object_update_dispatch.hpp',*sources,Path(__file__).resolve(),MANIFEST,
        MODULE/'reference/object-update-dispatch/NOTES.md',ROOT/'port/engine-resources/tests/cpu.py']
    before={p.relative_to(ROOT).as_posix():digest(p) for p in inputs}
    manifest = verify_original(args.original_elf)
    sys.path.insert(0, str(ROOT / 'port/engine-resources/tests'))
    from cpu import Cpu
    args.output.parent.mkdir(parents=True, exist_ok=True)
    command = [args.compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror', '-O2',
               *(str(path) for path in sources), '-o', str(args.output)]
    subprocess.run(command, check=True, capture_output=True, text=True)
    guards=json.loads(subprocess.check_output([str(args.output.resolve()), '--guards'],text=True))
    assert guards['validation']=='PASS' and guards['checks']>0
    cases = []
    original_cpu=Cpu(args.original_elf,False,manifest)
    for row in fixtures():
        host = host_result(args.output.resolve(), row)
        source = original_result(original_cpu, manifest, row)
        comparable = {k: source[k] for k in host}
        assert host == comparable, (row[0], host, comparable)
        cases.append({'name': row[0], 'host': host, 'original': source,
                      'source_instruction_count': source['instructions']})
    after={p.relative_to(ROOT).as_posix():digest(p) for p in inputs}
    if before!=after:raise ValueError('source changed during compile/test')
    covered=sorted({int(at,0) for case in cases for at in case['original']['instruction_addresses']})
    # 0x34aa5c/60 advance the manager list after this slice and never execute.
    expected_addresses={at for row in manifest['source_ranges']
        for at in range(int(row['start'],0),int(row['start'],0)+row['size'],4)}-{0x34aa5c,0x34aa60}
    assert set(covered)==expected_addresses,(set(covered)^expected_addresses)
    report = {
        'result': 'PASS', 'scope': 'bounded inline ObjectManager::Update per-object slice; not full ObjectManager::Update',
        'original_sha256': SHA, 'source_files':before,
        'host_binary_sha256':digest(args.output),
        'compiler_command': command, 'host_guard_suite':guards,
        'executed_original_instruction_count':len(covered),
        'executed_original_instruction_addresses':[hex(at) for at in covered],
        'original_ARM_case_count': len(cases), 'cases': cases,
        'boundaries': ['active-list traversal', 'current-Level outer gate', 'deletion+0x81 mark/unlink/free',
                       'manager post-update handle bookkeeping', 'Character::Update/CanUpdate body'],
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({'result': 'PASS', 'cases': len(cases), 'report': str(args.report),
                      'host_binary_sha256': digest(args.output)}, indent=2))


if __name__ == '__main__':
    main()
