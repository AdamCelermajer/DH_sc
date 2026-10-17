"""Verify the bounded original acquisition prefix and reused timing kernel."""
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
ORIGINAL_SHA = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
MANIFEST = MODULE / 'reference/character-aggro-acquisition-prefix/original-functions.json'
AI, OWNER, OWNER_B = 0x10010000, 0x10014000, 0x10018000
TABLE, NEW_TABLE, TABLE_VAR = 0x10020000, 0x10024000, 0x10028000
COUNT_VAR, VTABLE, APP = 0x10028004, 0x10029000, 0x10030000
SEED, COUNTER, STACK = 0x10032000, 0x10032004, 0x1007e000


def scenarios():
    default = [0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,0]
    cases = []
    def add(name, changes=None):
        words = default.copy()
        for key, value in (changes or {}).items(): words[key] = value
        cases.append((name, words))
    add('local_monster_normal_no_target')
    add('has_aggro_radius', {11: 9})
    add('first_player_skips', {0: 7})
    add('second_player_bypasses_timing', {1: 9})
    add('faerie_bypasses_timing', {2: 1})
    add('npc_after_timing', {3: 1})
    add('local_monster_target_diverts', {6: 1})
    add('nonmonster_target_does_not_divert', {4: 0, 6: 1})
    add('remote_monster_target_does_not_divert', {5: 1, 6: 1})
    add('faerie_target418_skips', {7: 1, 8: 1})
    add('not_turn_wait', {13: 0, 14: 0})
    add('delay_wait', {14: 0, 15: 17})
    add('ordinary_random_then_acquisition', {14: 0})
    add('elapsed_500_forces_then_acquisition', {13: 0, 16: 500})
    for name, word in [('plus_zero',0),('minus_zero',0x80000000),('negative',0xbf800000),
                       ('nan',0x7fc00000),('minus_infinity',0xff800000),
                       ('positive',0x3f800000),('plus_infinity',0x7f800000)]:
        add('spawn_override_' + name, {10: 1, 12: word})
    for mutation in range(1, 16):
        changes = {17: mutation}
        if mutation == 9: changes[7] = 1
        if mutation == 14: changes.update({10: 1, 12: 0x3f800000})
        add('source_mutation_' + str(mutation), changes)
    add('aggro_radius_snapshot_before_awaiting', {11: 1, 17: 12})
    add('aggro_radius_read_after_aiid', {11: 1, 17: 11})
    add('negative_aiid_fallback8', {9: 0xffffffff})
    add('large_aiid_fallback8', {9: 99})
    return cases


def oracle(original: Path, executable: Path):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0, str(ROOT / 'port/engine-resources/tests'))
    from cpu import Cpu
    raw = original.read_bytes()
    assert hashlib.sha256(raw).hexdigest() == ORIGINAL_SHA
    evidence = json.loads(MANIFEST.read_text())
    with original.open('rb') as stream:
        elf = ELFFile(stream)
        symbols = {s.name: s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        segments = [s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
        def source_range(address, size):
            segment = next(s for s in segments if s['p_vaddr'] <= address and
                           address + size <= s['p_vaddr'] + s['p_filesz'])
            offset = int(segment['p_offset']) + address - int(segment['p_vaddr'])
            return raw[offset:offset + size]
        for row in evidence['functions'] + evidence['vtable_ranges']:
            address, size = int(row['elf_address'], 0), row['size']
            symbol = symbols[row['original_symbol']]
            assert (int(symbol['st_value']), int(symbol['st_size'])) == (address, size)
            assert hashlib.sha256(source_range(address, size)).hexdigest() == row['sha256']
        def source_word(address): return struct.unpack('<I', source_range(address, 4))[0]
        for row in evidence['vtable_ranges']:
            point = int(row['address_point'], 0)
            for slot in row['slots']:
                assert source_word(point + int(slot['byte_offset'], 0)) == int(slot['target'], 0)
        got = (0x3cf408 + source_word(0x3cfbb0)) & 0xffffffff
    old = Cpu(original, False, evidence)
    old.uc.mem_map(0x10000000, 0x80000)
    def get(at): return struct.unpack('<I', old.uc.mem_read(at, 4))[0]
    results = []
    for name, words in scenarios():
        mutation = words[17]
        old.uc.mem_write(STACK - 0x2000, bytes(0x2000))
        for actor in (OWNER, OWNER_B):
            old.pointer(actor, VTABLE)
            old.pointer(actor + 0x408, words[6])
            old.pointer(actor + 0x418, words[8])
            old.pointer(actor + 0x143c, words[12])
            old.pointer(actor + 0xffc, words[9])
        old.pointer(VTABLE + 0x28, 0x3a49f0)
        old.pointer(VTABLE + 0x54, 0x33dd10)
        old.pointer(AI + 4, OWNER)
        old.pointer(AI + 8, words[15]); old.pointer(AI + 12, words[16])
        for row in range(16):
            old.pointer(TABLE + row * 0x44 + 0x3c, 0x41200000)
            old.pointer(TABLE + row * 0x44 + 0x40, 0x41a00000)
            old.pointer(NEW_TABLE + row * 0x44 + 0x3c, 0x41f00000)
            old.pointer(NEW_TABLE + row * 0x44 + 0x40, 0x42200000)
        old.pointer(TABLE_VAR, TABLE); old.pointer(COUNT_VAR, 16)
        old.pointer(got + source_word(0x3cfbc4), TABLE_VAR)
        old.pointer(got + 0x112c, COUNT_VAR)
        old.pointer(got + source_word(0x3cfbb4), 0x10028010)
        old.pointer(0x10028010, 1)
        old.pointer(got + source_word(0x3cfbbc), 0x1002a000)
        old.pointer(got + source_word(0x3cfbc8), APP)
        old.pointer(got + source_word(0x3cfbe8), SEED)
        old.pointer(got + source_word(0x3cfbec), COUNTER)
        old.pointer(SEED, 123); old.pointer(COUNTER, 4)
        calls, end = [], []
        players, faeries, delta_reads = [0], [0], [0]
        getter_actor = [0]
        def returned(word=0):
            old.put(0, word); old.uc.reg_write(old.pc, old.uc.reg_read(old.lr))
        def record(op, subject, value):
            calls.append([op, subject, value])
            if ((mutation == 1 and op == 0 and players[0] == 1) or
                (mutation == 2 and op == 0 and players[0] == 2) or
                (mutation == 3 and op == 1 and faeries[0] == 1) or
                (mutation == 4 and op == 2) or (mutation == 5 and op == 3) or
                (mutation == 6 and op == 4) or (mutation == 7 and op == 5) or
                (mutation == 8 and op == 1 and faeries[0] == 2) or
                (mutation == 9 and op == 6) or (mutation == 15 and op == 11)):
                old.pointer(AI + 4, OWNER_B)
            if mutation == 11 and op == 7:
                old.pointer(TABLE + value * 0x44 + 0x3c, 0x42480000)
            if mutation == 12 and op == 8:
                old.pointer(TABLE + 0x3c, 0x42480000)
                old.pointer(TABLE + 0x40, 0x42700000)
                old.pointer(AI + 4, OWNER_B)
            if mutation == 13 and op == 9:
                old.pointer(TABLE + 0x40, 0x42700000); old.pointer(AI + 4, OWNER_B)
            if mutation == 14 and op == 10: old.pointer(AI + 4, OWNER_B)
        def observe(_, address, __, ___):
            if address == 0x3cf430:
                if len(calls) == 1: decision = 1
                elif calls[-1][0] == 2: decision = 4
                elif calls[-1][0] == 6: decision = 6
                else: decision = 3 if any(c[0] == 12 for c in calls) else 2
                end.append((0, decision, 0, 0)); old.uc.emu_stop()
            elif address == 0x3cf4b4:
                end.append((5, 5, 0, 0)); old.uc.emu_stop()
            elif address == 0x4a2730:
                assert old.reg(2) == 0x7fffffff and old.reg(3) == 2
                assert get(old.uc.reg_read(old.sp)) == 1
                end.append((0, 7, old.reg(8), old.reg(1))); old.uc.emu_stop()
            elif address == 0x3a49f0:
                index = players[0]; players[0] += 1
                value = words[index]; record(0, old.reg(0), value); returned(value)
            elif address in (0x3a3094, 0x3a310c, 0x3a3064, 0x33dd10):
                if address == 0x3a3094:
                    index = 2 if old.uc.reg_read(old.lr) == 0x3cf754 else 7
                    faeries[0] += 1; op = 1
                else:
                    index, op = {0x3a310c:(3,2),0x3a3064:(4,3),0x33dd10:(5,4)}[address]
                value = words[index]; record(op, old.reg(0), value); returned(value)
            elif address in (0x3cf4a8, 0x3cf5b4, 0x3cf8d0):
                op, offset = {0x3cf4a8:(5,0x408),0x3cf5b4:(6,0x418),0x3cf8d0:(10,0x143c)}[address]
                subject = old.reg(10) if op == 10 else old.reg(0)
                value = get(subject + offset); record(op, subject, value)
            elif address == 0x3cf5cc:
                calls.append([14,0,int(old.reg(7) == NEW_TABLE)])
                if mutation == 10:
                    old.pointer(AI + 4, OWNER_B); old.pointer(TABLE_VAR, NEW_TABLE)
            elif address == 0x3a2fec:
                getter_actor[0] = old.reg(0)
            elif address in (0x3a3010, 0x3a3018):
                if address == 0x3a3010 and old.reg(0) >= 16: return
                record(7, getter_actor[0], old.reg(0))
            elif address == 0x3c0230:
                value = words[10]; record(8, old.reg(0) - 0x4fc, value); returned(value)
            elif address == 0x3d49f0:
                value = words[11]; record(9, old.reg(0), value); returned(value)
            elif address == 0x3cc484:
                value = words[13]; record(11, old.reg(0), value); returned(value)
            elif address in (0x337888, 0x3140ec): returned()
            elif address == 0x337a88:
                value = words[14]; record(12,0,value); returned(value)
            elif address == 0x31f66c:
                value = 19 if delta_reads[0] else 16; delta_reads[0] += 1
                record(13,0,value); old.pointer(APP + 0x8c, value)
                # Real original GetDt leaf executes.
            elif address == 0x30e2f8:
                # Source external __aeabi_fcmpgt PLT boundary. Its library
                # implementation is absent from this ELF; preserve raw operand
                # words and provide IEEE comparison fixture, including NaN/zeros.
                assert old.reg(1) == 0
                value = struct.unpack('<f', struct.pack('<I', old.reg(0)))[0]
                returned(int(value > 0.0))
        hook = old.uc.hook_add(UC_HOOK_CODE, observe)
        old.put(0, AI); old.uc.reg_write(old.sp, STACK); old.uc.reg_write(old.lr, old.stop)
        old.uc.emu_start(0x3cf3f0, old.stop, count=5000)
        old.uc.hook_del(hook)
        assert len(end) == 1, (name,end)
        status, decision, radius, list_owner = end[0]
        expected = {'status':status,'decision':decision,'radius':radius,'list_owner':list_owner,
                    'owner':get(AI+4),'countdown':get(AI+8),'elapsed':get(AI+12),
                    'seed':get(SEED),'counter':get(COUNTER),'sync_seed':0x76543210,
                    'sync_counter':0x01234567,'calls':calls}
        actual = json.loads(subprocess.check_output([str(executable),*map(str,words)],text=True))
        assert actual == expected, (name,actual,expected)
        results.append({'case':name,'matched':True,'source_result':expected})
    return {'validation':'PASS','comparisons':len(results),'mismatches':0,'results':results,
            'executed_scope':'Original entry/gate/radius preparation through normal TargetList ctor call; original timed arithmetic/RNG and GetCharAIId/GetDt instructions execute. No TargetList ctor/search/event loop or Monster existing-target retarget body.',
            'observed_services':'IsPlayer/IsFaerie/IsNPC/IsMonster/IsRemotelyUpdated/Awaiting/HasAggro/IsMyTurn/debug and external fcmpgt dependencies supplied fixtures; direct field/table source reads observed for ordered comparison.'}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler')
    parser.add_argument('--c-compiler')
    parser.add_argument('--original-elf',type=Path,required=True)
    parser.add_argument('--output',type=Path,default=MODULE/'build/character-aggro-acquisition-prefix/host.exe')
    parser.add_argument('--report',type=Path,default=MODULE/'build/character-aggro-acquisition-prefix/validation.json')
    args = parser.parse_args()
    cxx = args.compiler or os.environ.get('CXX') or shutil.which('g++')
    if not cxx: parser.error('pass --compiler')
    cc = args.c_compiler or str(Path(cxx).with_name(Path(cxx).name.replace('g++','gcc').replace('clang++','clang')))
    output = args.output.resolve(); output.parent.mkdir(parents=True,exist_ok=True)
    random_object = output.with_suffix('.random.o')
    sources = [MODULE/'character_aggro_acquisition_prefix.cpp',MODULE/'character_aggro_delay.cpp',
               MODULE/'tests/character_aggro_acquisition_prefix.cpp']
    commands = [[cc,'-std=c99','-O1','-Wall','-Wextra','-Werror','-c',str(ROOT/'port/random/random.c'),'-o',str(random_object)],
                [cxx,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),str(random_object),'-o',str(output)]]
    for command in commands: subprocess.run(command,cwd=ROOT,check=True,capture_output=True,text=True)
    host = json.loads(subprocess.check_output([str(output)],text=True))
    assert host['validation']=='PASS' and host['acquisition_prefix_cases']==45 and host['mismatches']==0
    for flag in ('delay_kernel_reused','fresh_owner_and_table_phases','float_override_guards','unsupported_retarget_explicit'):
        assert host[flag] is True
    original = oracle(args.original_elf.resolve(),output)
    paths = sources+[ROOT/'port/random/random.c',ROOT/'port/random/random.h',MODULE/'character_aggro_delay.hpp',
                     MODULE/'character_aggro_acquisition_prefix.hpp',Path(__file__).resolve(),MANIFEST,
                     MANIFEST.with_name('NOTES.md')]
    report = {'validation':'PASS','host_report':host,'original_arm_comparison':original,
              'original_sha256':ORIGINAL_SHA,'native_wired':False,'compiler_commands':commands,
              'source_sha256':{p.relative_to(ROOT).as_posix():hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},
              'scope':'One bounded acquisition prefix; existing timing/RNG reused. Retarget branch, TargetList/search/events, actor providers and native wiring external.'}
    args.report.parent.mkdir(parents=True,exist_ok=True)
    args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'validation':'PASS','host_cases':host['acquisition_prefix_cases'],'original_arm_cases':original['comparisons'],'mismatches':0}))


if __name__=='__main__': main()
