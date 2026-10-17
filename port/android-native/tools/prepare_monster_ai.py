#!/usr/bin/env python3
"""Bundle the exact external monster AI sources from the supplied cache."""
import argparse
import hashlib
import json
from pathlib import Path
import zipfile

CACHE_SHA256 = '3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
SCRIPTS = {
    '_commons.luac': '20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c',
    'monster.luac': '84f07caaeb2c04f2024cc3e27d41f33806b2c53d6e8861bb3d8b371118fd0e1d',
}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('cache', type=Path)
    parser.add_argument('--project', type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()
    with args.cache.open('rb') as stream:
        assert hashlib.file_digest(stream, 'sha256').hexdigest() == CACHE_SHA256
    # Verify every input before changing the asset bundle.
    pending = {}
    inputs = []
    with zipfile.ZipFile(args.cache) as archive:
        for name, expected in SCRIPTS.items():
            suffix = 'data/scripts/ai/' + name
            entries = [entry for entry in archive.infolist()
                       if entry.filename.replace('\\', '/').endswith('/' + suffix)
                       or entry.filename.replace('\\', '/') == suffix]
            assert len(entries) == 1, (suffix, len(entries))
            raw = archive.read(entries[0])
            assert hashlib.sha256(raw).hexdigest() == expected, suffix
            pending[name] = raw
            inputs.append({'entry': entries[0].filename, 'source_path': suffix,
                           'asset_path': 'scripts/ai/' + name,
                           'bytes': len(raw), 'sha256': expected})
    out = args.project / 'app/src/main/assets/scripts/ai'
    out.mkdir(parents=True, exist_ok=True)
    for name, raw in pending.items():
        (out / name).write_bytes(raw)
    provenance = {
        'cache_sha256': CACHE_SHA256,
        'inputs': inputs,
        'unchanged_original_source': True,
        'native_actor_wired': False,
        'scope': 'Exact baseline sources for AISExternal monster callbacks; packaging alone does not establish live actor execution or complete AIS lifecycle.',
    }
    (out / 'monster-ai-provenance.json').write_text(json.dumps(provenance, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({'validation': 'PASS', 'unchanged_original_scripts': len(inputs),
                      'native_actor_wired': False}))


if __name__ == '__main__':
    main()
