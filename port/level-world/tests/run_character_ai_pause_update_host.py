"""Compare AI_PauseUpdate's complete original caller and timer arguments."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3, UC_ARM_REG_PC, UC_ARM_REG_LR, UC_ARM_REG_SP

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
MANIFEST = MODULE/'reference/character-ai-pause-update/original-functions.json'
SHA = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
AI, OWNER, OTHER, STACK, STOP = 0x10001000, 0x10002000, 0x10003000, 0x1003e000, 0x1003f000
PAUSE, START = 0x3cb748, 0x3dbe24


def image(path):
    raw = path.read_bytes()
    assert hashlib.sha256(raw).hexdigest() == SHA
    with path.open('rb') as stream:
        elf = ELFFile(stream)
        segments = [s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
        symbols = {s.name: s for s in elf.get_section_by_name('.dynsym').iter_symbols()}
        code = {}
        for item in json.loads(MANIFEST.read_text())['functions']:
            address, size = int(item['elf_address'], 0), item['size']
            symbol = symbols[item['original_symbol']]
            assert (int(symbol['st_value']), int(symbol['st_size'])) == (address, size)
            seg = next(s for s in segments if s['p_vaddr'] <= address and address + size <= s['p_vaddr'] + s['p_filesz'])
            offset = seg['p_offset'] + address - seg['p_vaddr']
            code[address] = raw[offset:offset + size]
            assert hashlib.sha256(code[address]).hexdigest() == item['sha256']
        return code


def original(code, paused, duration, mutation):
    uc = Uc(UC_ARCH_ARM, UC_MODE_ARM)
    uc.mem_map(0x3cb000, 4096); uc.mem_map(0x3db000, 4096)
    uc.mem_map(0x10000000, 0x40000); uc.mem_write(PAUSE, code[PAUSE])
    def put(address, word): uc.mem_write(address, struct.pack('<I', word & 0xffffffff))
    def get(address): return struct.unpack('<I', uc.mem_read(address, 4))[0]
    put(AI+4, OWNER); uc.mem_write(AI+0x18, bytes([paused]))
    result = {}
    def hook(machine, address, size, context):
        if address == START:
            result.update(status=0, calls=1, paused_at_call=machine.mem_read(AI+0x18, 1)[0],
                          timer_owner=machine.reg_read(UC_ARM_REG_R0)-0x3b4,
                          duration=machine.reg_read(UC_ARM_REG_R1), repeat=machine.reg_read(UC_ARM_REG_R2),
                          event=machine.reg_read(UC_ARM_REG_R3), user_ref=get(machine.reg_read(UC_ARM_REG_SP)),
                          timer_id=-17)
            if mutation == 1: put(AI+4, OTHER)
            if mutation == 2: machine.mem_write(AI+0x18, b'\0')
            if mutation == 3: put(AI+4, OTHER); machine.mem_write(AI+0x18, b'\xff')
            machine.reg_write(UC_ARM_REG_R0, 0xffffffef)
            machine.reg_write(UC_ARM_REG_PC, machine.reg_read(UC_ARM_REG_LR))
        if address == STOP: machine.emu_stop()
    uc.hook_add(UC_HOOK_CODE, hook)
    uc.reg_write(UC_ARM_REG_R0, AI); uc.reg_write(UC_ARM_REG_R1, duration)
    uc.reg_write(UC_ARM_REG_SP, STACK); uc.reg_write(UC_ARM_REG_LR, STOP)
    uc.emu_start(PAUSE, STOP+4, count=64)
    assert uc.reg_read(UC_ARM_REG_PC) == STOP
    result.update(paused=uc.mem_read(AI+0x18, 1)[0], owner=get(AI+4))
    return result


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--compiler', required=True); p.add_argument('--original-elf', type=Path, required=True)
    p.add_argument('--output', type=Path, default=MODULE/'build/character-ai-pause-update/host.exe')
    p.add_argument('--report', type=Path, default=MODULE/'build/character-ai-pause-update/validation.json')
    a = p.parse_args(); code = image(a.original_elf.resolve())
    sources = [MODULE/'character_ai_pause_update.cpp', MODULE/'character_ai_events.cpp',
               MODULE/'character_timers.cpp', MODULE/'tests/character_ai_pause_update.cpp']
    a.output.parent.mkdir(parents=True, exist_ok=True)
    command = [a.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror', '-pedantic',
               *map(str, sources), '-o', str(a.output.resolve())]
    subprocess.run(command, cwd=ROOT, check=True, capture_output=True, text=True)
    exe = str(a.output.resolve()); host = json.loads(subprocess.check_output([exe], text=True))
    assert host == {'validation': 'PASS', 'host_cases': 12}
    rows = []
    for paused in (0, 1, 255):
        for duration in (0, 1, 1000, 0x80000000, 0xffffffff):
            for mutation in range(4):
                actual = json.loads(subprocess.check_output([exe, str(paused), str(duration), str(mutation)], text=True))
                expected = original(code, paused, duration, mutation)
                assert actual == expected, (paused, duration, mutation, actual, expected)
                rows.append({'paused': paused, 'duration': duration, 'mutation': mutation,
                             'matched': True, 'result': expected})
    paths = sources + [MODULE/'character_ai_pause_update.hpp', MODULE/'character_ai_frame.hpp',
                      MODULE/'character_ai_events.hpp', MODULE/'character_timers.hpp',
                      Path(__file__).resolve(), MANIFEST, MANIFEST.with_name('NOTES.md')]
    report = {'validation': 'PASS', 'host_cases': host['host_cases'], 'original_arm_cases': len(rows),
              'mismatches': 0, 'original_sha256': SHA, 'results': rows, 'compiler_command': command,
              'source_sha256': {f.relative_to(ROOT).as_posix(): hashlib.sha256(f.read_bytes()).hexdigest() for f in paths},
              'native_wired': False,
              'scope': 'Complete original52B pause caller executes; TMR_Start hooked to compare captured owner/embedded offset, duration/repeat/event/user and pre-call pause byte. Additional source-timer/AI-event composition verifies 999+1 expiry, blocked updates, zero duration and storage failure. Original Start/Event bodies and Android pursuit not ARM-replayed by this unit.'}
    a.report.parent.mkdir(parents=True, exist_ok=True)
    a.report.write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8')
    print(json.dumps({k: report[k] for k in ('validation', 'host_cases', 'original_arm_cases', 'mismatches')}))
if __name__ == '__main__': main()
