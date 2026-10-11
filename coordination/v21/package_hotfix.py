"""Copy only frozen release files; leave the played baseline and saves untouched."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil


def sha(path):
    digest = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b''):
            digest.update(block)
    return digest.hexdigest().upper()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--base', type=Path, required=True)
    parser.add_argument('--target', type=Path, required=True)
    parser.add_argument('--exe', type=Path, required=True)
    parser.add_argument('--metadata', type=Path, required=True)
    args = parser.parse_args()
    metadata = json.loads(args.metadata.read_text(encoding='utf-8-sig'))
    for key in ('version', 'source_commit', 'ctest', 'verification', 'fix', 'readme'):
        if key not in metadata:
            raise ValueError('Missing evidence: ' + key)
    if args.target.exists():
        raise FileExistsError('Preserving existing folder: ' + str(args.target))
    if not args.exe.is_file():
        raise FileNotFoundError(args.exe)
    manifest = json.loads((args.base / 'manifest.json').read_text(encoding='utf-8-sig'))
    frozen = []
    for entry in manifest['files']:
        relative = Path(entry['path'])
        if relative.is_absolute() or '..' in relative.parts:
            raise ValueError('Unsafe manifest path')
        if relative.suffix.lower() in ('.save', '.gsave') or relative.name.lower() == 'dh2_settings.tutorials':
            raise ValueError('Save data in frozen manifest')
        source = args.base / relative
        if source.stat().st_size != entry['bytes'] or sha(source) != entry['sha256'].upper():
            raise ValueError('Baseline frozen file changed: ' + entry['path'])
        frozen.append((source, relative))
    # Ignore played-baseline extras: copy manifest entries, never the directory tree.
    args.target.mkdir(parents=True)
    for source, relative in frozen:
        destination = args.target / relative
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, destination)
    shutil.copy2(args.exe, args.target / 'dh-foundation.exe')
    executable_sha = sha(args.target / 'dh-foundation.exe')
    status = 'Frozen candidate; independent verification pending'
    receipt = {key: metadata[key] for key in ('version', 'source_commit', 'ctest', 'verification')}
    receipt.update(exe_sha256=executable_sha, base_package=args.base.name, status=status)
    notes = json.loads((args.base / 'release-notes.json').read_text(encoding='utf-8-sig'))
    notes.pop('independent_verdict', None)
    notes.pop('independent_report', None)
    notes.update(version=metadata['version'], source_commit=metadata['source_commit'],
                 exe_sha256=executable_sha, status=status)
    notes['fixes'].append(metadata['fix'])
    (args.target / 'README.txt').write_text(metadata['readme'], encoding='utf-8')
    for filename, value in (('package-receipt.json', receipt), ('release-notes.json', notes)):
        (args.target / filename).write_text(json.dumps(value, indent=2) + '\n', encoding='utf-8')
    files = [{'path': path.relative_to(args.target).as_posix(), 'bytes': path.stat().st_size,
              'sha256': sha(path)} for path in sorted(args.target.rglob('*'))
             if path.is_file() and path.name != 'manifest.json']
    (args.target / 'manifest.json').write_text(
        json.dumps({'version': metadata['version'], 'files': files}, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({'folder': str(args.target), 'exe_sha256': executable_sha, 'files': len(files)}))


if __name__ == '__main__':
    main()
