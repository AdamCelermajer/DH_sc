"""Remove exact obsolete build directories after preserving runtime evidence."""
import argparse
from datetime import datetime, timezone
import hashlib
import json
import os
from pathlib import Path
import shutil
import stat
import subprocess

def sha(path):
    digest = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b''):
            digest.update(block)
    return digest.hexdigest()

root = Path('C:/Users/adamc/Desktop/workspace/DH_sc').resolve()
build_parent = Path('C:/Users/adamc/Desktop/workspace/DH_wt').resolve()
backup = root/'.local-inputs/cleanup-preserved-20261012/build-objects'
receipt = root/'.local-inputs/v21.3/root/obsolete-build-cleanup.json'
parser = argparse.ArgumentParser()
parser.add_argument('--apply', action='store_true')
options = parser.parse_args()
assert 'Root accepts v21.3' in (root/'coordination/v21.3/FINAL-REVIEW.md').read_text()
assert subprocess.check_output(['git', '-C', str(root), 'worktree', 'list', '--porcelain'], text=True).count('worktree ') == 1
targets = sorted(p for p in build_parent.iterdir() if p.is_dir() and p.name.startswith('build-'))
qa = Path('C:/DHqbuild')
if qa.exists():
    targets.append(qa)
records = []
for target in targets:
    absolute = target.resolve()
    assert (absolute.parent == build_parent and absolute.name.startswith('build-')) or absolute == qa
    assert not (target.lstat().st_file_attributes & stat.FILE_ATTRIBUTE_REPARSE_POINT)
    files = []
    for directory, dirs, names in os.walk(target, followlinks=False):
        for name in dirs + names:
            path = Path(directory)/name
            if path.lstat().st_file_attributes & stat.FILE_ATTRIBUTE_REPARSE_POINT:
                raise ValueError('Unexpected build reparse point: ' + str(path))
        files += [Path(directory)/name for name in names]
    preserved = []
    for path in files:
        # Keep arbitrary source, saves, captures, logs and frozen executables.
        # Only reproducible compiler/debug products are discarded.
        suffix = path.suffix.lower()
        compiled_object = suffix in ('.obj', '.o', '.a') and any(part.lower().endswith('.dir') or part == 'CMakeFiles' for part in path.parts)
        disposable = suffix in ('.pdb', '.ilk', '.pch', '.lib', '.tlog', '.exp', '.idb', '.ipch') or compiled_object
        if not disposable:
            preserved.append(path)
    records.append(dict(path=str(absolute), bytes=sum(p.stat().st_size for p in files),
                        fileCount=len(files), preserve=preserved))
if not options.apply:
    print(json.dumps(dict(build_directories=len(records), bytes=sum(r['bytes'] for r in records),
                          preserve_files=sum(len(r['preserve']) for r in records))))
    raise SystemExit(0)
if receipt.exists():
    raise FileExistsError('Preserving existing cleanup receipt')
backup.mkdir(parents=True, exist_ok=True)
result = dict(timeUtc=datetime.now(timezone.utc).isoformat(), freeBefore=shutil.disk_usage(root).free, builds=[])
for record in records:
    preserved = []
    for source in record.pop('preserve'):
        digest = sha(source)
        destination = backup/(digest+'.bin')
        if not destination.exists():
            shutil.copy2(source, destination)
        if sha(destination) != digest:
            raise ValueError('Backup hash mismatch')
        if sha(source) != digest:
            raise ValueError('Build evidence changed during copy')
        preserved.append(dict(source=str(source), sha256=digest, backup=str(destination)))
    record['preserved'] = preserved
    target = Path(record['path'])
    assert target.parent == build_parent or target == qa
    shutil.rmtree(target)
    record['removed'] = not target.exists()
    result['builds'].append(record)
    receipt.write_text(json.dumps(result, indent=2)+'\n')
result['freeAfter'] = shutil.disk_usage(root).free
receipt.write_text(json.dumps(result, indent=2)+'\n')
print(json.dumps(dict(removed=len(records), freeBefore=result['freeBefore'], freeAfter=result['freeAfter'])))
