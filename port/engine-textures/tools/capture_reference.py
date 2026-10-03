"""Capture exact original texture routines. This does not decompile or execute them."""
import argparse
import hashlib
import json
from pathlib import Path
from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM
from elftools.elf.elffile import ELFFile

EXPECTED = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
ROOT = Path(__file__).resolve().parents[1]

def main():
    p = argparse.ArgumentParser()
    p.add_argument('engine', type=Path)
    a = p.parse_args()
    assert hashlib.sha256(a.engine.read_bytes()).hexdigest() == EXPECTED
    rows, assembly = [], []
    with a.engine.open('rb') as f:
        elf = ELFFile(f)
        symbols = list(elf.get_section_by_name('.symtab').iter_symbols())
        wanted = [s for s in symbols if s['st_info']['type'] == 'STT_FUNC' and
                  ('readPVRHeader' in s.name or 'loadTextureHeader' in s.name and 'CImageLoaderPVR' in s.name
                   or s.name in ('_Z15PVRTCDecompressPKviiiPh','_ZL18InterpolateColoursPKiS0_S0_S0_iiiPi','_ZL9TwiddleUVmmmm'))]
        for symbol in wanted:
            addr, size = symbol['st_value'], symbol['st_size']
            segment = next(s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD' and
                           s['p_vaddr'] <= addr and addr+size <= s['p_vaddr']+s['p_filesz'])
            f.seek(segment['p_offset']+addr-segment['p_vaddr'])
            raw = f.read(size)
            rows.append({'original_symbol': symbol.name, 'elf_address': hex(addr), 'size': size,
                         'sha256': hashlib.sha256(raw).hexdigest()})
            assembly.append(f'\n# {symbol.name}, ELF VA {addr:#x}, {size} bytes\n')
            assembly.extend(f'{i.address:08x}: {i.bytes.hex():8} {i.mnemonic:8} {i.op_str}\n'
                            for i in Cs(CS_ARCH_ARM, CS_MODE_ARM).disasm(raw, addr))
        print('FORMAT TABLE CANDIDATES', [(s.name, hex(s['st_value']), s['st_size']) for s in symbols
              if 'Format' in s.name and s['st_info']['type'] == 'STT_OBJECT' and s['st_size'] >= 40])
    (ROOT/'reference').mkdir(exist_ok=True)
    (ROOT/'reference/original-functions.asm').write_text(''.join(assembly), encoding='utf-8')
    (ROOT/'original-functions.json').write_text(json.dumps({'original_sha256': EXPECTED, 'functions': rows}, indent=2)+'\n')
    print(json.dumps(rows, indent=2))

if __name__ == '__main__':
    main()
