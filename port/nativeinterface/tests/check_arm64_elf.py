#!/usr/bin/env python3
"""Validate reconstruction architecture, exports and load-segment alignment."""
import argparse
import hashlib
import json
from pathlib import Path
from elftools.elf.elffile import ELFFile

def exports(path):
    with path.open('rb') as stream:
        elf = ELFFile(stream)
        return {s.name for s in elf.get_section_by_name('.dynsym').iter_symbols()
                if s['st_info']['type'] == 'STT_FUNC' and s['st_shndx'] != 'SHN_UNDEF'
                and s['st_info']['bind'] == 'STB_GLOBAL'}

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('original', type=Path)
    parser.add_argument('rebuilt', type=Path)
    parser.add_argument('--report', type=Path)
    args = parser.parse_args()
    original_exports, rebuilt_exports = exports(args.original), exports(args.rebuilt)
    assert original_exports == rebuilt_exports, (original_exports - rebuilt_exports,
                                                 rebuilt_exports - original_exports)
    with args.rebuilt.open('rb') as stream:
        elf = ELFFile(stream)
        assert elf.elfclass == 64 and elf['e_machine'] == 'EM_AARCH64'
        loads = [s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
        assert loads
        for load in loads:
            assert load['p_align'] >= 16384
            assert (load['p_vaddr'] - load['p_offset']) % 16384 == 0
    report = {'result': 'pass', 'architecture': 'AArch64', 'elf_class': 64,
              'function_exports_preserved': sorted(rebuilt_exports),
              'function_export_count': len(rebuilt_exports),
              'load_segment_alignment': [s['p_align'] for s in loads],
              'built_sha256': hashlib.sha256(args.rebuilt.read_bytes()).hexdigest(),
              'limits': ['static ELF validation does not establish Android device execution']}
    if args.report: args.report.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report, indent=2))

if __name__ == '__main__':
    main()
