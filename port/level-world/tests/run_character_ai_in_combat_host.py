"""Compare the reconstructed AI_IsInCombat caller to its original ARM body.

The five dependency services supply fixture words and can replace owner.
This executes a component on the host, not the original game or Android.
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
ORIGINAL_SHA = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
MANIFEST = MODULE / 'reference/character-ai-in-combat/original-functions.json'
AI, OWNER, STOP, STACK = 0x10010000, 0x10014000, 0x1003f100, 0x1003e000
ENTRY = 0x3d4bc4
QUERIES = (0x3d49f0, 0x3d4a00, 0x3c02d0, 0x3c02e8, 0x3c0334)


def fixtures():
    return [
        ('outgoing_true', [7,1,1,1,1,0]),
        ('incoming_true', [0,0xffffffff,1,1,1,0]),
        ('attacking_true', [0,0,9,1,1,0]),
        ('using_skill_true', [0,0,0,0x80000000,1,0]),
        ('casting_raw', [0,0,0,0,0x13579bdf,0]),
        ('all_false', [0,0,0,0,0,0]),
        ('fresh_owner_all_queries', [0,0,0,0,0xffffffff,1]),
        ('fresh_owner_attacking', [0,0,3,0,0,1]),
        ('fresh_owner_using_skill', [0,0,0,3,0,1]),
    ]


def source(original):
    from elftools.elf.elffile import ELFFile
    raw = original.read_bytes()
    assert hashlib.sha256(raw).hexdigest() == ORIGINAL_SHA
    with original.open('rb') as stream:
        elf = ELFFile(stream)
        assert elf.elfclass == 32 and elf['e_machine'] == 'EM_ARM' and elf.little_endian
        segments = [(int(s['p_vaddr']), int(s['p_filesz']), int(s['p_memsz']), int(s['p_offset']))
                    for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
        symbols = {s.name: s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        evidence = json.loads(MANIFEST.read_text())
        for item in evidence['functions']:
            symbol = symbols[item['original_symbol']]
            address = int(item['elf_address'], 0)
            assert (int(symbol['st_value']), int(symbol['st_size'])) == (address, item['size'])
            start, length, _, offset = next(s for s in segments if s[0] <= address and address+item['size'] <= s[0]+s[1])
            data = raw[offset+address-start:offset+address-start+item['size']]
            assert hashlib.sha256(data).hexdigest() == item['sha256']
    return raw, segments


def original_case(raw, segments, values):
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE
    from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_SP, UC_ARM_REG_LR, UC_ARM_REG_PC
    machine = Uc(UC_ARCH_ARM, UC_MODE_ARM)
    image_end = max(base + memory for base, _, memory, _ in segments)
    machine.mem_map(0, (image_end + 4095) & ~4095)
    for base, size, _, offset in segments:
        machine.mem_write(base, raw[offset:offset+size])
    machine.mem_map(0x10000000, 0x40000)
    def put(address, value): machine.mem_write(address, struct.pack('<I', value & 0xffffffff))
    def get(address): return struct.unpack('<I', machine.mem_read(address, 4))[0]
    put(AI+4, OWNER)
    calls = []
    def hook(uc, address, size, context):
        if address == STOP:
            uc.emu_stop(); return
        if address not in QUERIES: return
        index = QUERIES.index(address)
        argument = uc.reg_read(UC_ARM_REG_R0)
        subject = argument if index < 2 else argument - 0x4fc
        calls.append([index, subject])
        if values[5]: put(AI+4, OWNER+(index+1)*0x1000)
        uc.reg_write(UC_ARM_REG_R0, values[index])
        uc.reg_write(UC_ARM_REG_PC, uc.reg_read(UC_ARM_REG_LR))
    machine.hook_add(UC_HOOK_CODE, hook)
    machine.reg_write(UC_ARM_REG_R0, AI)
    machine.reg_write(UC_ARM_REG_SP, STACK)
    machine.reg_write(UC_ARM_REG_LR, STOP)
    machine.emu_start(ENTRY, STOP+4, count=500)
    assert machine.reg_read(UC_ARM_REG_PC) == STOP
    return {'status': 0, 'value': machine.reg_read(UC_ARM_REG_R0), 'owner': get(AI+4), 'calls': calls}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--original-elf', type=Path, required=True)
    parser.add_argument('--compiler', default=os.environ.get('CXX', 'g++'))
    parser.add_argument('--output', type=Path, default=MODULE/'build/character-ai-in-combat/host.exe')
    parser.add_argument('--report', type=Path, default=MODULE/'build/character-ai-in-combat/validation.json')
    args = parser.parse_args()
    raw, segments = source(args.original_elf)
    sources = [MODULE/'character_ai_in_combat.cpp', MODULE/'tests/character_ai_in_combat.cpp']
    args.output.parent.mkdir(parents=True, exist_ok=True)
    command = [args.compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror', '-O2',
               *(str(p) for p in sources), '-o', str(args.output)]
    subprocess.run(command, check=True, capture_output=True, text=True)
    guards = json.loads(subprocess.check_output([str(args.output.resolve())], text=True))
    assert guards['validation'] == 'PASS'
    results = []
    for name, values in fixtures():
        expected = original_case(raw, segments, values)
        actual = json.loads(subprocess.check_output([str(args.output.resolve()), *map(str, values)], text=True))
        assert actual == expected, (name, actual, expected)
        results.append({'case': name, 'matched': True, 'source_result': expected})
    paths = [*sources, MODULE/'character_ai_in_combat.hpp', Path(__file__).resolve()]
    report = {
        'validation': 'PASS', 'original_arm_cases': len(results), 'guard_cases': guards['guard_cases'],
        'mismatches': 0, 'original_sha256': ORIGINAL_SHA, 'results': results,
        'compiler_command': command,
        'source_sha256': {p.relative_to(ROOT).as_posix(): hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},
        'scope': 'Original 112-byte AI_IsInCombat caller instructions compared with compiled source; outgoing/incoming/state dependency calls are supplied fixtures, including owner replacement. Full callee storage bodies, native wiring and whole-game AI excluded.',
        'native_wired': False,
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8')
    print(json.dumps({k:report[k] for k in ('validation','original_arm_cases','guard_cases','mismatches')}))


if __name__ == '__main__': main()
