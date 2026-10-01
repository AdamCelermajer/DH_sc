#!/usr/bin/env python3
"""Restore the pinned snapshot and current Git sources into a NEW work directory.

No downloads, builds, device operations, or changes to the checkout are made.
The destination must not exist. Run from a Git clone with Python 3.10+.
"""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import shutil
import subprocess
import zipfile

ROOT = Path(__file__).resolve().parents[1]
ARCHIVE_SHA = '9ce519019b2e34d4b777455a10b57a50f4dd0a32c82aa9b1892abc9d9c522440'
ORIGINALS = {
    'libDungeonHunter2.so': '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80',
    'libStormGLOFT.so': 'be6beaab782944e5ce39cca8e850fd03de654e4236c9329621043adcbee291e1',
    'libnativeinterface.so': '180b582cbb7e7004c94fe771f8c8013dad4c74a9317ee4baff08c542da4ff0da',
}


def sha(data):
    return hashlib.sha256(data).hexdigest()


def safe_path(name):
    path = PurePosixPath(name)
    if path.is_absolute() or not path.parts or path.parts[0] != 'compatibility':
        raise ValueError('Unexpected snapshot path: ' + name)
    if any(part in ('.', '..', '.git') for part in path.parts):
        raise ValueError('Unsafe snapshot path: ' + name)
    return path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('destination', type=Path)
    args = parser.parse_args()
    dest = args.destination.expanduser().absolute()
    if dest.exists() or dest.is_symlink():
        parser.error('Destination already exists; choose a new directory.')
    dest = dest.resolve()
    if dest.is_relative_to(ROOT):
        parser.error('Choose a directory outside the Git checkout.')
    archive = ROOT / 'compatibility-work-test5.zip'
    if sha(archive.read_bytes()) != ARCHIVE_SHA:
        raise SystemExit('Snapshot archive checksum mismatch; nothing restored.')
    tracked = subprocess.check_output(
        ['git', '-C', str(ROOT), 'ls-files', '-z', '--', 'compatibility'])
    overlays = []
    for raw in tracked.split(b'\0'):
        if not raw:
            continue
        name = raw.decode('utf-8')
        rel = safe_path(name)
        source = ROOT.joinpath(*rel.parts)
        if source.is_symlink() or not source.is_file():
            raise SystemExit('Expected an ordinary tracked file: ' + name)
        overlays.append((rel, source.read_bytes()))
    # Validate every archive blob before making the output directory.
    pending = []
    with zipfile.ZipFile(archive) as z:
        manifest = json.loads(z.read('manifest.json'))
        for item in manifest['files']:
            rel = safe_path(item['path'])
            data = z.read('blobs/' + item['sha256'])
            if len(data) != item['bytes'] or sha(data) != item['sha256']:
                raise SystemExit('Corrupt snapshot member: ' + item['path'])
            pending.append((rel, data))
    dest.mkdir(parents=True)
    for rel, data in pending + overlays:
        path = dest.joinpath(*rel.parts)
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(data)
    work = dest / 'compatibility/work'
    # Historical snapshot kept the original ELF files in decoded-original/lib,
    # while patch_engine.py and patch_storm.py expect original/lib.
    target = work / 'original/lib/armeabi-v7a'
    target.mkdir(parents=True, exist_ok=True)
    for name, expected in ORIGINALS.items():
        source = work / 'decoded-original/lib/armeabi-v7a' / name
        if sha(source.read_bytes()) != expected:
            raise SystemExit('Original library hash mismatch: ' + name)
        shutil.copyfile(source, target / name)
    with zipfile.ZipFile(work / 'fold7-build/runtime-bundle.zip') as z:
        runtime = json.loads((work / 'fold7-build/runtime-manifest.json').read_text())
        for name, item in runtime.items():
            data = z.read(name)
            if sha(data) != item['sha256'] or len(data) != item['bytes']:
                raise SystemExit('Runtime bundle mismatch: ' + name)
    report = {
        'checkout_head': subprocess.check_output(
            ['git', '-C', str(ROOT), 'rev-parse', 'HEAD'], text=True).strip(),
        'snapshot_sha256': ARCHIVE_SHA,
        'snapshot_files_verified': len(pending),
        'tracked_overlay_files': len(overlays),
        'overlay_sha256': {str(rel): sha(data) for rel, data in overlays},
        'original_elf_sha256': ORIGINALS,
        'runtime_bundle_files_verified': len(runtime),
        'note': 'Current tracked compatibility sources overlay the historical snapshot. '
                'FILE-MANIFEST.json describes that snapshot, not subsequent edits. '
                'SDK/NDK/JDK, upstream checkout, APK and cache are separate inputs.',
    }
    (dest / 'RESTORE-REPORT.json').write_text(json.dumps(report, indent=2) + '\n')
    print('Verified snapshot files:', len(pending))
    print('Applied current tracked compatibility files:', len(overlays))
    print('Verified original ELF files:', len(ORIGINALS))
    print('Verified runtime bundle files:', len(runtime))
    print('Work directory:', work)
    print('Next: follow LOCAL_AGENT_HANDOFF.md in the Git checkout.')


if __name__ == '__main__':
    main()
