#!/usr/bin/env python3
"""Verify hashes and export accounting; this does not validate native semantics."""
import argparse
import csv
import hashlib
import json
from pathlib import Path


def digest(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--repo-root', type=Path, default=Path(__file__).resolve().parents[1])
    a = p.parse_args()
    root = a.repo_root.resolve()
    libraries = []
    for directory in sorted((root / 'recovered/native/decompiled').iterdir()):
        if not directory.is_dir(): continue
        rows = [json.loads(line) for line in (directory / 'function-index.jsonl').read_text().splitlines()]
        summary = json.loads((directory / 'summary.json').read_text())
        assert len(rows) == summary['attempted']
        assert sum(bool(r['success']) for r in rows) == summary['success']
        shards = {r['file']: (directory / r['file']).read_bytes() for r in rows}
        assert set(shards) == {x.name for x in directory.glob('functions-*.pseudo.c')}
        previous = {}
        for row in rows:
            data = shards[row['file']]
            start, length = row['byte_offset'], row['byte_length']
            text = data[start:start+length].decode('utf-8')
            assert text.startswith('/* address=' + row['address'] + ' ')
            assert int(row['address'], 16) - int(summary['image_base'], 16) == int(row['elf_address'], 16)
            if row['file'] in previous: assert previous[row['file']] == start
            previous[row['file']] = start + length
        assert all(previous[name] == len(data) for name, data in shards.items())
        original_path = root / 'recovered/native/symbols' / directory.name / 'function-index.csv'
        if not original_path.exists():
            raise SystemExit('Run python tools/unpack_native.py before verifying named-function coverage.')
        with original_path.open() as f:
            original = {int(r['address']) & ~1 for r in csv.DictReader(f)}
        found = {int(r['elf_address'], 16) for r in rows}
        missing = sorted(original - found)
        assert not missing, (directory.name, missing)
        failures = [{k: r[k] for k in ['elf_address', 'name', 'error']} for r in rows if not r['success']]
        libraries.append({'library': directory.name, 'original_distinct_starts': len(original),
                          'original_starts_attempted': len(original & found), 'missing_original_starts': missing,
                          'exported_functions': len(rows), 'emitted_pseudocode': summary['success'],
                          'failed': len(failures), 'failures': failures,
                          'emitted_warning_marker_count': sum(r['has_warning'] for r in rows),
                          'all_byte_indexes_verified': True, 'compilable': False, 'gameplay_validated': False})
    for row in json.loads((root / 'recovered/native/bundles/manifest.json').read_text())['bundles']:
        assert digest(root / row['path']) == row['sha256']
    sources = json.loads((root / 'recovered/assets/source-data/provenance.json').read_text())['files']
    for row in sources:
        path = root / 'recovered/assets/source-data' / row['output_path']
        assert path.stat().st_size == row['size'] and digest(path) == row['sha256']
    java = json.loads((root / 'reports/java-port-source-manifest.json').read_text())['files']
    for row in java:
        path = root / row['path']
        assert path.stat().st_size == row['bytes'] and digest(path) == row['sha256']
    report = {'scope': 'Export accounting, original-start attempts and unchanged source hashes only',
              'libraries': libraries, 'cache_source_files_hash_verified': len(sources),
              'reconstructed_java_files_hash_verified': len(java), 'gameplay_validated': False}
    (root / 'reports/decompiler-coverage.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({'libraries': [{k:r[k] for k in ['library','original_distinct_starts','original_starts_attempted','exported_functions','emitted_pseudocode','failed']} for r in libraries],
                      'cache_source_files_verified':len(sources),'java_sources_verified':len(java)}, indent=2))


if __name__ == '__main__':
    main()
