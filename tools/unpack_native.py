#!/usr/bin/env python3
"""Expand hash-verified full native evidence bundles shipped in this repository."""
import argparse
import hashlib
import json
import tarfile
from pathlib import Path, PurePosixPath


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--repo-root', type=Path, default=Path(__file__).resolve().parents[1])
    a = p.parse_args()
    root = a.repo_root.resolve()
    manifest = json.loads((root / 'recovered/native/bundles/manifest.json').read_text())
    for bundle in manifest['bundles']:
        archive = root / bundle['path']
        if hashlib.sha256(archive.read_bytes()).hexdigest() != bundle['sha256']:
            raise ValueError(f'bundle SHA-256 mismatch: {archive.name}')
        with tarfile.open(archive, 'r:gz') as t:
            members = t.getmembers()
            if len(members) != bundle['files']:
                raise ValueError(f'bundle entry count mismatch: {archive.name}')
            for member in members:
                name = PurePosixPath(member.name)
                if not member.isfile() or name.is_absolute() or '..' in name.parts:
                    raise ValueError(f'unsafe member: {member.name}')
                if name.parts[:3] not in [('recovered', 'native', 'assembly'), ('recovered', 'native', 'symbols')]:
                    raise ValueError(f'unexpected member destination: {member.name}')
                dest = root.joinpath(*name.parts)
                if not dest.resolve().is_relative_to(root):
                    raise ValueError(f'member escapes repository: {member.name}')
                dest.parent.mkdir(parents=True, exist_ok=True)
                source = t.extractfile(member)
                if source is None: raise ValueError(f'missing member bytes: {member.name}')
                dest.write_bytes(source.read())
        print(f'{archive.name}: {len(members)} evidence files restored')


if __name__ == '__main__':
    main()
