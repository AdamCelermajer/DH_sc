#!/usr/bin/env python3
"""Reproducible ELF inventory and disassembly of the manually ported routines."""
import argparse
import csv
import hashlib
import json
from pathlib import Path

from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM, CS_MODE_THUMB
from elftools.elf.elffile import ELFFile
from verify_translations import FUNCTIONS, ORIGINAL_SHA256

ROOT = Path(__file__).resolve().parent
parser = argparse.ArgumentParser()
parser.add_argument('--original', type=Path, default=ROOT.parent / 'original/lib/armeabi-v7a/libDungeonHunter2.so')
parser.add_argument('--output', type=Path, default=ROOT / 'audit')
args = parser.parse_args()
if hashlib.sha256(args.original.read_bytes()).hexdigest() != ORIGINAL_SHA256:
    raise SystemExit('This inventory is pinned to the supplied engine hash.')
args.output.mkdir(parents=True, exist_ok=True)
with args.original.open('rb') as f:
    elf = ELFFile(f)
    symbols = elf.get_section_by_name('.symtab')
    functions = [s for s in symbols.iter_symbols()
                 if s['st_info']['type'] == 'STT_FUNC'
                 and s['st_shndx'] != 'SHN_UNDEF' and s['st_size'] > 0]
    with (args.output / 'functions.csv').open('w', newline='') as out:
        writer = csv.writer(out)
        writer.writerow(['symbol', 'virtual_address', 'byte_size', 'binding', 'instruction_set'])
        for s in sorted(functions, key=lambda s: (s['st_value'], s.name)):
            writer.writerow([s.name, hex(s['st_value']), s['st_size'],
                             s['st_info']['bind'], 'Thumb' if s['st_value'] & 1 else 'ARM'])
    units = []
    dwarf = elf.get_dwarf_info()
    for cu in dwarf.iter_CUs():
        die = cu.get_top_DIE()
        attributes = {}
        for key in ('DW_AT_name', 'DW_AT_comp_dir', 'DW_AT_producer', 'DW_AT_language'):
            if key in die.attributes:
                value = die.attributes[key].value
                attributes[key] = value.decode(errors='replace') if isinstance(value, bytes) else value
        attributes['dwarf_offset'] = hex(cu.cu_offset)
        units.append(attributes)
    dynamic = elf.get_section_by_name('.dynamic')
    needed = [t.needed for t in dynamic.iter_tags() if t.entry.d_tag == 'DT_NEEDED']
    imports = sorted({s.name for s in elf.get_section_by_name('.dynsym').iter_symbols()
                      if s['st_shndx'] == 'SHN_UNDEF' and s.name})
    report = {
        'sha256': ORIGINAL_SHA256,
        'elf_class': elf.elfclass,
        'machine': elf['e_machine'],
        'defined_nonempty_function_symbols': len(functions),
        'distinct_function_symbol_values': len({s['st_value'] for s in functions}),
        'function_count_note': 'Symbol-based inventory; aliases and wrappers occur. This is not a count of independent source functions.',
        'dwarf_compilation_units': units,
        'dwarf_note': 'Available units cover supporting license/network code, STLport, and compiler support; full game source/types were not recovered.',
        'needed': needed,
        'undefined_symbols': imports,
        'load_segments': [{'vaddr': hex(s['p_vaddr']), 'alignment': s['p_align'],
                           'file_size': s['p_filesz'], 'memory_size': s['p_memsz']}
                          for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD'],
    }
    (args.output / 'engine-inventory.json').write_text(json.dumps(report, indent=2) + '\n')
    snippets = ['Original ARM32 disassembly of the four ported routines.',
                'Trailing literal pools may decode as instructions; consult source comments.', '']
    for name in FUNCTIONS:
        symbol = next(s for s in functions if s.name == name)
        address = symbol['st_value'] & ~1
        size = symbol['st_size']
        segment = next(s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD'
                       and s['p_vaddr'] <= address < s['p_vaddr'] + s['p_filesz'])
        offset = address - segment['p_vaddr']
        data = segment.data()[offset:offset+size]
        cs = Cs(CS_ARCH_ARM, CS_MODE_THUMB if symbol['st_value'] & 1 else CS_MODE_ARM)
        snippets.append(f'{name} @ {address:#010x}, {size} bytes')
        snippets.append('Raw bytes: ' + data.hex())
        snippets.extend(f'  {i.address:08x}  {i.mnemonic:<8} {i.op_str}' for i in cs.disasm(data, address))
        snippets.append('')
    (args.output / 'original-routines.txt').write_text('\n'.join(snippets) + '\n')
print('Wrote native symbol, dependency, DWARF and disassembly inventory:', args.output)
