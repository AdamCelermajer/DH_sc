"""Compare _UpdateMaster caller instructions with compiled maintained source.

Host component oracle only. Named query/event providers are observed fixtures;
this does not run the original Android game or claim their concrete bodies.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import struct
import subprocess

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
MANIFEST = MODULE/'reference/character-ai-master-update/original-functions.json'
ORIGINAL_SHA = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
AI, OWNER, MASTER, OTHER = 0x10010000, 0x10014000, 0x10018000, 0x10019000
VTABLE, STOP, STACK, DEAD, CAN_RANGE = 0x10030000, 0x1003f100, 0x1003e000, 0x1003f200, 0x1003f204
QUERIES = {0x3a2fec: 0, DEAD: 1, 0x3d4ed8: 2, 0x3cc484: 3,
           CAN_RANGE: 4, 0x3d63d8: 5, 0x3d6604: 6, 0x3d6188: 7, 0x3a4d5c: 8}
BASE = [1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 99, 0, 99]


def fixtures():
    rows = []
    def add(name, **changes):
        values = BASE.copy()
        fields = {'master': 0, 'alive': 1, 'old_sight': 2, 'dead': 3, 'sight': 4,
                  'turn': 5, 'ranged': 6, 'close': 7, 'range': 8, 'melee': 9,
                  'mutate_op': 10, 'mutation': 11, 'fail_op': 12}
        for key, value in changes.items(): values[fields[key]] = value
        rows.append((name, values))
    add('null_master', master=0)
    add('alive_melee')
    add('dead_edge', dead=1)
    add('revived_edge', alive=0)
    add('already_dead', dead=1, alive=0)
    add('lost_sight', sight=0)
    add('gained_sight', old_sight=0)
    add('already_out_of_sight', sight=0, old_sight=0)
    add('not_our_turn', turn=0)
    add('noncanonical_turn', turn=0x80000000)
    add('melee_out', melee=0)
    add('ranged_close', ranged=1, close=1)
    add('ranged_far', ranged=1, range=1)
    add('ranged_out', ranged=1)
    add('noncanonical_range_words', ranged=0x80000000, close=0x100)
    add('noncanonical_dead_xor_byte', dead=255)
    add('dead_high_word_low_zero', dead=0x100)
    add('dead_high_word_low_one', dead=0x10000001)
    add('sight_full_word_low_zero', sight=256)
    add('sight_full_word_sign', sight=0x80000000, old_sight=0)
    add('snapshot_boolean_noncanonical', alive=255, old_sight=2)
    add('fresh_owner_every_callback', mutate_op=98, mutation=1, alive=0, old_sight=0, ranged=1, range=1)
    add('fresh_master_every_callback', mutate_op=98, mutation=2, alive=0, old_sight=0, ranged=1, range=1)
    for op in range(9):
        add(f'owner_replace_at_{op}', mutate_op=op, mutation=1, alive=0, old_sight=0, ranged=int(op!=7), range=1)
        add(f'master_replace_at_{op}', mutate_op=op, mutation=2, alive=0, old_sight=0, ranged=int(op!=7), range=1)
    for op in (1, 2, 3, 4, 5, 6, 7, 8):
        add(f'master_cleared_at_{op}', mutate_op=op, mutation=3, alive=0, old_sight=0, ranged=int(op in (5, 6)), range=1)
    add('alive_event_cached_store', alive=0, mutate_op=8, mutation=4)
    add('sight_event_cancels_alive_range_gate', old_sight=0, mutate_op=8, mutation=4)
    add('events_cached_bytes_after_high_snapshot', alive=0, old_sight=0, mutate_op=8, mutation=5)
    add('dead_query_mutates_snapshots', mutate_op=1, mutation=4)
    add('sight_query_mutates_snapshots', mutate_op=2, mutation=4)
    return rows


def original_image(path):
    from elftools.elf.elffile import ELFFile
    raw = path.read_bytes()
    assert hashlib.sha256(raw).hexdigest() == ORIGINAL_SHA
    with path.open('rb') as stream:
        elf = ELFFile(stream)
        assert elf.elfclass == 32 and elf['e_machine'] == 'EM_ARM' and elf.little_endian
        segments = [(int(s['p_vaddr']), int(s['p_filesz']), int(s['p_memsz']), int(s['p_offset']))
                    for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
        symbols = {s.name: s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        for item in json.loads(MANIFEST.read_text())['functions']:
            address = int(item['elf_address'], 0)
            symbol = symbols[item['original_symbol']]
            assert (int(symbol['st_value']), int(symbol['st_size'])) == (address, item['size'])
            base, _, _, offset = next(s for s in segments if s[0] <= address and address+item['size'] <= s[0]+s[1])
            data = raw[offset+address-base:offset+address-base+item['size']]
            assert hashlib.sha256(data).hexdigest() == item['sha256']
    return raw, segments


def original_case(raw, segments, values):
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE
    from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_SP, UC_ARM_REG_LR, UC_ARM_REG_PC
    machine = Uc(UC_ARCH_ARM, UC_MODE_ARM)
    image_end = max(base+memory for base, _, memory, _ in segments)
    machine.mem_map(0, (image_end+4095)&~4095)
    for base, size, _, offset in segments: machine.mem_write(base, raw[offset:offset+size])
    machine.mem_map(0x10000000, 0x40000)
    def put(address, word): machine.mem_write(address, struct.pack('<I', word&0xffffffff))
    def get(address): return struct.unpack('<I', machine.mem_read(address, 4))[0]
    def byte(address): return machine.mem_read(address, 1)[0]
    put(AI+4, OWNER); put(AI+0x50, MASTER if values[0] else 0)
    machine.mem_write(AI+0x54, bytes([values[1]&255, values[2]&255]))
    for index in range(20): put(OWNER+index*0x1000, VTABLE)
    put(VTABLE+0x34, DEAD); put(VTABLE+0x124, CAN_RANGE)
    words = [777, *values[3:10], 0]
    calls = []
    def hook(uc, address, size, context):
        if address == STOP: uc.emu_stop(); return
        if address not in QUERIES: return
        op = QUERIES[address]
        subject = uc.reg_read(UC_ARM_REG_R0)
        peer = uc.reg_read(UC_ARM_REG_R1) if op in (2, 5, 6, 7) else 0
        event = 0
        if op == 8:
            event = uc.reg_read(UC_ARM_REG_R1); peer = uc.reg_read(UC_ARM_REG_R2)
        calls.append([op, subject, peer, event, byte(AI+0x54), byte(AI+0x55)])
        if values[10] in (op, 98):
            if values[11] == 1: put(AI+4, get(AI+4)+0x1000)
            if values[11] == 2: put(AI+0x50, OTHER if get(AI+0x50)==MASTER else MASTER)
            if values[11] == 3: put(AI+0x50, 0)
            if values[11] == 4: machine.mem_write(AI+0x54, b'\0\0')
            if values[11] == 5: machine.mem_write(AI+0x54, b'\7\11')
        uc.reg_write(UC_ARM_REG_R0, words[op])
        uc.reg_write(UC_ARM_REG_PC, uc.reg_read(UC_ARM_REG_LR))
    machine.hook_add(UC_HOOK_CODE, hook)
    machine.reg_write(UC_ARM_REG_R0, AI); machine.reg_write(UC_ARM_REG_SP, STACK)
    machine.reg_write(UC_ARM_REG_LR, STOP)
    machine.emu_start(0x3cc5a4, STOP+4, count=500)
    assert machine.reg_read(UC_ARM_REG_PC) == STOP
    return {'status': 0, 'owner': get(AI+4), 'master': get(AI+0x50),
            'alive': byte(AI+0x54), 'sight': byte(AI+0x55), 'calls': calls}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--original-elf', type=Path, required=True)
    parser.add_argument('--compiler', default=os.environ.get('CXX', 'g++'))
    parser.add_argument('--output', type=Path, default=MODULE/'build/character-ai-master-update/host.exe')
    parser.add_argument('--report', type=Path, default=MODULE/'build/character-ai-master-update/validation.json')
    args = parser.parse_args()
    raw, segments = original_image(args.original_elf)
    sources = [MODULE/'character_ai_master_update.cpp', MODULE/'tests/character_ai_master_update.cpp']
    args.output.parent.mkdir(parents=True, exist_ok=True)
    command = [args.compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror', '-O2',
               *(str(p) for p in sources), '-o', str(args.output)]
    subprocess.run(command, check=True, capture_output=True, text=True)
    def compiled(values):
        return json.loads(subprocess.check_output([str(args.output.resolve()), *map(str, values)], text=True))
    guards = json.loads(subprocess.check_output([str(args.output.resolve())], text=True))
    assert guards['validation'] == 'PASS'
    rows = []
    for name, values in fixtures():
        expected = original_case(raw, segments, values); actual = compiled(values)
        assert actual == expected, (name, actual, expected)
        rows.append({'case': name, 'matched': True, 'compiled_result': actual, 'original_result': expected})
    failures = []
    for op in range(9):
        values = BASE.copy(); values[1:3] = [0, 0]; values[6:9] = [int(op!=7), 0, 1]
        values[12] = op
        full = compiled([*values[:12], 99]); actual = compiled(values)
        seen = next((i for i, call in enumerate(full['calls']) if call[0]==op), None)
        if seen is None: continue
        assert actual['status']==2 and actual['calls']==full['calls'][:seen+1], (op, actual, full)
        # Event failure must retain the old cached byte: store follows delivery.
        if op==8: assert actual['alive']==0 and actual['sight']==0
        failures.append({'operation': op, 'matched_port_failure_contract': True, 'result': actual})
    null = BASE.copy(); null[10:12] = [0, 3]
    invalid = compiled(null)
    assert invalid['status']==3 and len(invalid['calls'])==1 and invalid['master']==0
    paths = [*sources, MODULE/'character_ai_master_update.hpp', Path(__file__).resolve(), MANIFEST]
    report = {'validation': 'PASS', 'original_arm_cases': len(rows), 'guard_cases': guards['guard_cases'],
              'port_failure_cases': len(failures)+1, 'mismatches': 0, 'original_sha256': ORIGINAL_SHA,
              'results': rows, 'port_failures': failures, 'invalid_source_fact': invalid,
              'compiler_command': command,
              'source_sha256': {p.relative_to(ROOT).as_posix(): hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},
              'scope': 'Complete 444-byte _UpdateMaster caller orchestration compared with original instructions. GetCharAIId, virtual IsDead/CanRangeAttack, sight, IsMyTurn, range and RaiseEvent bodies are observed fixture services; concrete callee bodies/native wiring/full game excluded. Port service-failure/argument checks are separate from original comparisons.',
              'native_wired': False}
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8')
    print(json.dumps({k: report[k] for k in ('validation', 'original_arm_cases', 'guard_cases', 'port_failure_cases', 'mismatches')}))


if __name__ == '__main__': main()
