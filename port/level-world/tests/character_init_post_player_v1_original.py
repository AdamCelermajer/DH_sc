"""Execute InitPost's SG_Load(4)-through-skills caller block on pinned ARM."""
from __future__ import annotations
import hashlib
import json
import struct
import sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / 'engine-resources/tests'))
from cpu import Cpu

ELF_SHA = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
INITPOST_SHA = '80de661077e88ee5df672be65b792776cb2c859e38d32304019289422c6ca079'
START = 0x3b513c
CUTOFF = 0x3b51bc
SYMBOL = '_ZN9Character8InitPostEv'

def cases():
    # level-present, difficulty-bits, first-local, second-local, save-present,
    # initial signed Character+13c8, value after _InitEquipment.
    return [
        (0, 0, 0, 0, 0, -7, 91),
        (1, 2, 1, 1, 1, 263, -33),
        (1, 0xFFFFFFFF, 1, 0, 0, -1, 41),
        (0, 1, 0, 1, 1, 32767, -17),
    ]

def provenance(path: Path):
    blob = path.read_bytes()
    assert hashlib.sha256(blob).hexdigest() == ELF_SHA, 'original ELF changed'
    with path.open('rb') as stream:
        elf = ELFFile(stream)
        symbols = {s.name: s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        sym = symbols[SYMBOL]
        start, size = int(sym['st_value']), int(sym['st_size'])
        assert start == 0x3b4d60 and size == 2164
        for segment in elf.iter_segments():
            if segment['p_type'] == 'PT_LOAD' and segment['p_vaddr'] <= start and start + size <= segment['p_vaddr'] + segment['p_filesz']:
                offset = segment['p_offset'] + start - segment['p_vaddr']
                body = blob[offset:offset + size]
                break
        else:
            raise AssertionError('InitPost body not file-backed')
    assert hashlib.sha256(body).hexdigest() == INITPOST_SHA, 'InitPost body changed'
    return {'original_sha256': ELF_SHA, 'symbol': SYMBOL, 'address': hex(start),
            'size': size, 'sha256': INITPOST_SHA, 'executed_start': hex(START),
            'exclusive_cutoff': hex(CUTOFF),
            'scope': 'Original ARM instructions execute from SG_Load(4) at 0x3b513c through the second locality branch to 0x3b51bc. Direct service callees and Level/locality/property/quest/equipment/skills bodies are declared provider fixtures.'}

def capture(path: Path, out: Path):
    manifest = provenance(path)
    rows = []
    for case in cases():
        level_present, difficulty, local1, local2, save_present, property_id, after_equipment = case
        cpu = Cpu(path, False, {'functions': [{'original_symbol': SYMBOL, 'elf_address': hex(0x3b4d60), 'size': 2164}]})
        char, app, manager = cpu.data + 0x1000, cpu.data + 0x7000, cpu.data + 0x7100
        level, prop = cpu.data + 0x8000, cpu.data + 0x2000
        cpu.uc.mem_write(char, bytes(0x1600))
        cpu.uc.mem_write(app, bytes(0x100))
        cpu.uc.mem_write(level, bytes(0x200))
        cpu.pointer(app + 0x40, manager)
        cpu.pointer(cpu.base + 0x997a98 + 0x37f4, app)
        cpu.uc.mem_write(char + 0x13c8, struct.pack('<h', property_id))
        cpu.pointer(char + 0x14e8, cpu.data + 0x7200 if save_present else 0)
        if level_present:
            cpu.uc.mem_write(level + 0x118, struct.pack('<I', difficulty & 0xffffffff))

        trace = []
        def record(op, callsite, subject, ordinal=0, argument=0):
            trace.append((op, callsite, subject, ordinal, argument))
        def done(value=0):
            cpu.put(0, value & 0xffffffff)
            cpu.uc.reg_write(cpu.pc, cpu.uc.reg_read(cpu.lr))
        def hook(uc, address, size, unused):
            address -= cpu.base
            if address == CUTOFF:
                uc.emu_stop(); return
            ret = uc.reg_read(cpu.lr) - cpu.base
            if address == 0x3bc4d0:
                assert [cpu.reg(0), cpu.reg(1)] == [char, 4] and ret == 0x3b5140, (hex(cpu.reg(0)), hex(cpu.reg(1)), hex(ret))
                record(0, 0x3b513c, 1, argument=4); done()
            elif address == 0x31f594:
                assert cpu.reg(0) == app and ret == 0x3b514c
                record(1, 0x3b5148, 0)
                done(level if level_present else 0)
            elif address == 0x3bb950:
                assert cpu.reg(0) == char and ret == 0x3b5160
                record(2, 0x3b515c, 1, argument=struct.unpack('<i', struct.pack('<I', difficulty & 0xffffffff))[0]); done()
            elif address == 0x36effc:
                assert cpu.reg(0) == manager and cpu.reg(1) == char
                if ret == 0x3b5174:
                    record(3, 0x3b5170, 1, ordinal=1); done(local1)
                elif ret == 0x3b51b4:
                    record(3, 0x3b51b0, 1, ordinal=2); done(local2)
                else:
                    raise AssertionError(f'unexpected IsLocalPlayer caller {ret:#x}')
            elif address == 0x3b395c:
                assert cpu.reg(0) == char and ret == 0x3b54e4
                record(4, 0x3b54e0, 1); cpu.uc.mem_write(char + 0x13c8, struct.pack('<h', after_equipment)); done()
            elif address == 0x3defac:
                assert cpu.reg(0) == prop and ret == 0x3b54ec
                record(5, 0x3b54e8, 2); done()
            elif address == 0x4679e8:
                assert cpu.reg(0) == cpu.data + 0x7200 and ret == 0x3b5500
                record(6, 0x3b54fc, 3); done()
            elif address == 0x3df2a4:
                assert cpu.reg(0) == prop and ret == 0x3b518c
                class_id = struct.unpack('<h', cpu.uc.mem_read(char + 0x13c8, 2))[0]
                assert cpu.reg(1) == (class_id & 0xffffffff)
                record(7, 0x3b5188, 2, argument=class_id); done()
            elif address == 0x3df480:
                assert cpu.reg(0) == prop and ret == 0x3b5194
                record(8, 0x3b5190, 2); done()
            elif address == 0x3e0810:
                assert cpu.reg(0) == prop and cpu.reg(1) == 1 and ret == 0x3b51a0
                record(9, 0x3b519c, 2, argument=1); done()
            elif address == 0x3b3a90:
                assert cpu.reg(0) == char and ret == 0x3b54d8
                record(10, 0x3b54d4, 1); done()

        cpu.uc.hook_add(UC_HOOK_CODE, hook)
        sp = cpu.stack + 0xe000
        cpu.uc.reg_write(cpu.sp, sp)
        cpu.uc.reg_write(cpu.lr, cpu.stop)
        cpu.put(4, char); cpu.put(5, cpu.base + 0x997a98); cpu.put(7, prop)
        cpu.put(0, char); cpu.put(1, 4)
        # The sliced entry has already stored the original global offset here.
        cpu.uc.mem_write(sp + 0x10, struct.pack('<I', 0x37f4))
        cpu.uc.reg_write(cpu.pc, cpu.base + START)
        cpu.uc.emu_start(cpu.base + START, cpu.stop, count=10000)
        assert cpu.uc.reg_read(cpu.pc) == cpu.base + CUTOFF, 'original slice missed the selected cutoff'
        assert not cpu.import_calls, f'unexpected import calls: {cpu.import_calls}'
        rows.append({'input': case, 'trace': trace})

    out.mkdir(parents=True, exist_ok=True)
    (out / 'original-functions.json').write_text(json.dumps(manifest, indent=2) + '\n')
    (out / 'original-capture.json').write_text(json.dumps({'validation': 'PASS', 'cases': rows}, indent=2) + '\n')
    words = [0x31504943, len(rows)]
    for row in rows:
        case = row['input']
        inputs = [case[0], case[1] & 0xffffffff, case[2], case[3], case[4], case[5] & 0xffffffff, case[6] & 0xffffffff]
        words += inputs + [len(row['trace'])]
        for trace in row['trace']:
            words += [trace[0], trace[1], trace[2], trace[3], trace[4] & 0xffffffff]
    (out / 'original-cases.bin').write_bytes(struct.pack('<' + 'I' * len(words), *words))
    return {'validation': 'PASS', 'cases': len(rows), 'calls': sum(len(r['trace']) for r in rows)}

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('--original-elf', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    print(json.dumps(capture(args.original_elf.resolve(), args.output.resolve())))
