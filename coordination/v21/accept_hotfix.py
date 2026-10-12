"""Record root acceptance after scoped tests and independent frozen review."""
import argparse
import importlib.util
import json
from pathlib import Path
import shutil
spec = importlib.util.spec_from_file_location('hotfix_packager', Path(__file__).with_name('package_hotfix.py'))
packager = importlib.util.module_from_spec(spec)
spec.loader.exec_module(packager)
sha = packager.sha


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--target', type=Path, required=True)
    parser.add_argument('--base', type=Path, required=True)
    parser.add_argument('--review', type=Path, required=True)
    parser.add_argument('--progress-receipt', type=Path, required=True)
    args = parser.parse_args()
    target = args.target
    receipt = json.loads((target/'package-receipt.json').read_text(encoding='utf-8'))
    review = args.review.read_text(encoding='utf-8')
    if 'READY' not in review or receipt['exe_sha256'] not in review:
        raise ValueError('Independent READY review must identify this frozen executable')
    tests = receipt['ctest']
    log = Path(tests['log']).read_text(encoding='utf-8-sig')
    if tests['failed'] != 0 or tests['passed'] != tests['total'] or (
            f"100% tests passed, 0 tests failed out of {tests['total']}" not in log):
        raise ValueError('Recorded scoped tests do not pass')
    manifest = json.loads((target/'manifest.json').read_text(encoding='utf-8'))
    for entry in manifest['files']:
        path = target/entry['path']
        if path.stat().st_size != entry['bytes'] or sha(path) != entry['sha256']:
            raise ValueError('Candidate changed: ' + entry['path'])
    status = 'Accepted ' + receipt['version'] + ' hotfix; inherited preview caveats remain'
    receipt.update(status=status, independent_verdict='READY', independent_report=str(args.review.resolve()))
    receipt['verification']['status'] = 'PASS frozen normal production checks and independent runtime/package review'
    notes = json.loads((target/'release-notes.json').read_text(encoding='utf-8'))
    notes.update(status=status, independent_verdict='READY', independent_report=receipt['independent_report'])
    for filename, value in [('package-receipt.json', receipt), ('release-notes.json', notes)]:
        (target/filename).write_text(json.dumps(value, indent=2)+'\n', encoding='utf-8')
    files = [{'path': path.relative_to(target).as_posix(), 'bytes': path.stat().st_size, 'sha256': sha(path)}
             for path in sorted(target.rglob('*')) if path.is_file() and path.name != 'manifest.json']
    (target/'manifest.json').write_text(json.dumps(dict(version=receipt['version'], files=files), indent=2)+'\n', encoding='utf-8')
    # Progress is copied separately from the frozen roster. Verify an unchanged
    # source snapshot before reporting success; never write to the old package.
    progress = []
    for source in sorted(args.base.iterdir()):
        name = source.name.lower()
        if source.is_file() and (name.endswith('.save') or '.gsave' in name or name == 'dh2_settings.tutorials'):
            destination = target/source.name
            if destination.exists():
                raise FileExistsError('Preserving existing progress: ' + str(destination))
            before = sha(source)
            shutil.copy2(source, destination)
            if before != sha(source) or before != sha(destination):
                raise ValueError('Progress changed during copy: ' + source.name)
            progress.append(dict(file=source.name, sha256=before))
    args.progress_receipt.write_text(json.dumps(progress, indent=2)+'\n', encoding='utf-8')
    print(json.dumps(dict(status=status, frozen_files=len(files), copied_progress=len(progress))))


if __name__ == '__main__':
    main()
