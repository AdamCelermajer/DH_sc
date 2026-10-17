"""Bundle original native monster initialization constants and Debug seed."""
import argparse
import hashlib
import json
from pathlib import Path
import zipfile

CACHE_SHA256 = '3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
NAMES = ('design_pycst.bin', 'DebugSwitches.savegame')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('cache', type=Path)
    parser.add_argument('--project', type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()
    with args.cache.open('rb') as source:
        assert hashlib.file_digest(source, 'sha256').hexdigest() == CACHE_SHA256
    pending = {}
    provenance = {'cache_sha256': CACHE_SHA256, 'unchanged_original_data': True, 'inputs': {}}
    with zipfile.ZipFile(args.cache) as archive:
        for name in NAMES:
            suffix = name if name=='DebugSwitches.savegame' else 'data/pydata/'+name
            entries = [row for row in archive.infolist() if row.filename.replace('\\', '/') == suffix
                       or row.filename.replace('\\', '/').endswith('/'+suffix)]
            assert len(entries) == 1, (suffix, len(entries))
            raw = archive.read(entries[0])
            pending[name] = raw
            provenance['inputs'][name] = {'entry': entries[0].filename, 'bytes': len(raw),
                                          'sha256': hashlib.sha256(raw).hexdigest()}
    assets = args.project/'app/src/main/assets/data'
    assets.mkdir(parents=True, exist_ok=True)
    for name, raw in pending.items():
        (assets/name).write_bytes(raw)
    report = args.project/'app/build/monster-initialization-data-provenance.json'
    report.parent.mkdir(parents=True, exist_ok=True)
    report.write_text(json.dumps(provenance, indent=2)+'\n', encoding='utf-8')
    print(json.dumps({'validation': 'PASS', 'unchanged_original_initialization_files': len(pending)}))


if __name__ == '__main__':
    main()
