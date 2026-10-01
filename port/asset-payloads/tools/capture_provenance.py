#!/usr/bin/env python3
"""Capture full accessor/search routines and partial mesh/relocation evidence."""
import argparse
import csv
import hashlib
import json
from pathlib import Path
import re
import struct
from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parents[1]
COMPLETE = {
    0x669e00: 'dh2_animation_target', 0x669e10: 'dh2_animation_type',
    0x669e24: 'dh2_animation_vector (output)', 0x669e44: 'dh2_animation_channel',
    0x669e54: 'dh2_animation_has_default', 0x669e68: 'dh2_animation_default',
    0x669e78: 'dh2_animation_channels', 0x669e84: 'dh2_animation_time_type',
    0x669e9c: 'dh2_animation_interpolation', 0x669eb4: 'dh2_animation_offsets',
    0x669ec4: 'dh2_animation_scales', 0x669ed4: 'dh2_animation_scale_type',
    0x669eec: 'dh2_animation_vector (time)', 0x669f0c: 'dh2_animation_key_time',
    0x669fac: 'dh2_animation_start', 0x669fb4: 'dh2_animation_end',
    0x669fdc: 'dh2_animation_length', 0x66a004: 'dh2_animation_animator',
    0x66a1f0: 'dh2_animation_find_index (int)', 0x66a2ac: 'dh2_animation_find (int)',
    0x66a6d0: 'dh2_animation_find_raw_index (byte)', 0x66a788: 'dh2_animation_find_index (byte)',
    0x66a7c4: 'dh2_animation_find (byte)', 0x66abb8: 'dh2_animation_find_raw_index (short)',
    0x66ac78: 'dh2_animation_find_index (short)', 0x66acb4: 'dh2_animation_find (short)'
}
PARTIAL = {0x60bee4: 'inner animation-data relocation only',
           0x645588: 'deferred buffer selection and mesh fields only',
           0x6bccf0: 'interleaved stream fields only',
           0x6bcf80: 'primitive/attribute/index fields only'}

def main():
    p = argparse.ArgumentParser()
    p.add_argument('--original', required=True, type=Path)
    p.add_argument('--workspace', type=Path, default=ROOT.parents[1])
    a = p.parse_args()
    assembly = a.workspace/'recovered/native/assembly/libDungeonHunter2.so'
    index = a.workspace/'recovered/native/symbols/libDungeonHunter2.so/function-index.csv'
    rows, listings = [], []
    with a.original.open('rb') as stream:
        elf = ELFFile(stream)
        loads = [s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
        for row in csv.DictReader(index.open()):
            address, size = int(row['address']), int(row['range_size'])
            if address not in COMPLETE and address not in PARTIAL:
                continue
            symbol = json.loads(row['aliases'])[0]
            load = next(s for s in loads if s['p_vaddr'] <= address and address+size <= s['p_vaddr']+s['p_filesz'])
            stream.seek(load['p_offset']+address-load['p_vaddr'])
            rows.append({'original_symbol': symbol['name'], 'demangled': symbol['demangled'],
                         'elf_address': f'0x{address:08x}', 'size': size,
                         'sha256': hashlib.sha256(stream.read(size)).hexdigest(),
                         'scope': 'complete function' if address in COMPLETE else PARTIAL[address],
                         'port_symbol': COMPLETE.get(address, 'checked schema decoder')})
            text = (assembly/row['assembly_file']).read_text()
            listings.append(next(m.group() for m in re.finditer(r'(?ms)^; FUNCTION (0x[0-9a-f]+).*?(?=^; FUNCTION |\Z)', text)
                                 if int(m.group(1), 16) == address))
    assert len(rows) == len(COMPLETE)+len(PARTIAL)
    tables = [{'elf_address': '0x008e4ca4', 'name': 'SVertexAttributeTypeInspection::ValueTypeSize', 'bytes': [1,1,2,2,4,4,4]},
              {'elf_address': '0x008eb338', 'name': 'ColladaPrimitiveMap (engine enum, not GL enum)', 'words': [6,4,3,1,2]}]
    with a.original.open('rb') as stream:
        for table in tables:
            address = int(table['elf_address'], 16)
            expected = bytes(table['bytes']) if 'bytes' in table else struct.pack('<5I', *table['words'])
            load = next(s for s in loads if s['p_vaddr'] <= address and address+len(expected) <= s['p_vaddr']+s['p_filesz'])
            stream.seek(load['p_offset']+address-load['p_vaddr'])
            assert stream.read(len(expected)) == expected
            table['size'] = len(expected)
            table['sha256'] = hashlib.sha256(expected).hexdigest()
    manifest = {'original_library': 'libDungeonHunter2.so',
                'original_sha256': hashlib.sha256(a.original.read_bytes()).hexdigest(),
                'complete_function_count': len(COMPLETE), 'partial_evidence_function_count': len(PARTIAL),
                'functions': rows, 'static_tables': tables}
    (ROOT/'original-functions.json').write_text(json.dumps(manifest, indent=2)+'\n')
    (ROOT/'reference').mkdir(exist_ok=True)
    (ROOT/'reference/original-functions.asm').write_text('; ELF evidence; literal pools retained; not assembler-ready source.\n\n'+'\n'.join(listings))
    print(json.dumps({'complete_functions': len(COMPLETE), 'partial_evidence_functions': len(PARTIAL), 'bytes': sum(r['size'] for r in rows)}))

if __name__ == '__main__':
    main()
