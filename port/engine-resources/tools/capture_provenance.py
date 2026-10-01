#!/usr/bin/env python3
"""Capture selected original function metadata and complete assembly ranges."""
import argparse
import csv
import hashlib
import json
from pathlib import Path
import re
from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parents[1]
STREAMS = {
    0x56eb6c: 'dh2_memory_buffer', 0x56eb80: 'dh2_memory_all_in_memory',
    0x56eb8c: 'dh2_memory_valid', 0x56ebac: 'dh2_memory_async',
    0x56ebe0: 'dh2_memory_async_at', 0x56ec3c: 'dh2_memory_seek',
    0x56ec78: 'dh2_memory_size', 0x56ec80: 'dh2_memory_position',
    0x56ec88: 'dh2_memory_name', 0x56ec90: 'dh2_memory_full_path',
    0x56ecc4: 'dh2_memory_read', 0x6b44e8: 'dh2_limit_read',
    0x6b4580: 'dh2_limit_async', 0x6b45d0: 'dh2_limit_async_at',
    0x6b4644: 'dh2_limit_seek', 0x6b46d0: 'dh2_limit_size',
    0x6b46d8: 'dh2_limit_position', 0x6b46e8: 'dh2_limit_name',
    0x6b46f0: 'dh2_limit_full_path',
    0x60e2a8: 'dh2_bres_version', 0x60e334: 'dh2_bres_root_part',
    0x60e348: 'dh2_bres_root_part', 0x69a40c: 'dh2_bres_open / dh2_bres_fixups'
}
LIBRARIES = {
    0x60e35c: 0, 0x60e374: 1, 0x60e390: 2, 0x60e3ac: 3,
    0x60e3c8: 4, 0x60e3e4: 5, 0x60e400: 6, 0x60e41c: 7,
    0x60e434: 8, 0x60e468: 9, 0x60e484: 10, 0x60e450: 11, 0x60e4a0: 12
}

def main():
    p = argparse.ArgumentParser()
    p.add_argument('--original', required=True, type=Path)
    p.add_argument('--workspace', type=Path, default=ROOT.parents[1])
    a = p.parse_args()
    index = a.workspace / 'recovered/native/symbols/libDungeonHunter2.so/function-index.csv'
    assembly = a.workspace / 'recovered/native/assembly/libDungeonHunter2.so'
    functions, listings = [], []
    with a.original.open('rb') as stream:
        elf = ELFFile(stream)
        loads = [s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
        for row in csv.DictReader(index.open()):
            addr, size = int(row['address']), int(row['range_size'])
            if addr not in STREAMS and addr not in LIBRARIES:
                continue
            alias = json.loads(row['aliases'])[0]
            segment = next(s for s in loads if s['p_vaddr'] <= addr
                           and addr + size <= s['p_vaddr'] + s['p_filesz'])
            stream.seek(segment['p_offset'] + addr - segment['p_vaddr'])
            data = stream.read(size)
            item = {'original_symbol': alias['name'], 'demangled': alias['demangled'],
                    'elf_address': f'0x{addr:08x}', 'size': size,
                    'sha256': hashlib.sha256(data).hexdigest(),
                    'scope': 'whole-buffer branch only' if addr == 0x69a40c else 'complete function',
                    'port_symbol': STREAMS.get(addr, 'dh2_bres_library_item')}
            if addr in LIBRARIES:
                item['library_kind'] = LIBRARIES[addr]
            if addr in (0x60e334, 0x60e348):
                item['root_part_kind'] = int(addr == 0x60e348)
            functions.append(item)
            text = (assembly / row['assembly_file']).read_text()
            match = next(m for m in re.finditer(r'(?ms)^; FUNCTION (0x[0-9a-f]+).*?(?=^; FUNCTION |\Z)', text)
                         if int(m.group(1), 16) == addr)
            listings.append(match.group())
    manifest = {'original_library': 'libDungeonHunter2.so',
                'original_sha256': hashlib.sha256(a.original.read_bytes()).hexdigest(),
                'complete_function_count': 35, 'partial_function_count': 1,
                'functions': functions}
    assert len(functions) == 36
    (ROOT / 'original-functions.json').write_text(json.dumps(manifest, indent=2) + '\n')
    (ROOT / 'reference').mkdir(exist_ok=True)
    (ROOT / 'reference/original-functions.asm').write_text(
        '; Original ELF virtual addresses. Annotated evidence, not assembler-ready source.\n\n'
        + '\n'.join(listings))
    print(json.dumps({'functions': len(functions), 'original_bytes': sum(r['size'] for r in functions)}))

if __name__ == '__main__':
    main()
