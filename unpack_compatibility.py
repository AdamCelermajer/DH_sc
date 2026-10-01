#!/usr/bin/env python3
"""Restore every file in the exact Fold7 test 5 work snapshot after cloning."""
from pathlib import Path, PurePosixPath
import hashlib,json,zipfile

ROOT=Path(__file__).resolve().parent
archive=ROOT/'compatibility-work-test5.zip'
expected='9ce519019b2e34d4b777455a10b57a50f4dd0a32c82aa9b1892abc9d9c522440'
assert hashlib.sha256(archive.read_bytes()).hexdigest()==expected,'Snapshot archive checksum mismatch'
with zipfile.ZipFile(archive) as z:
    manifest=json.loads(z.read('manifest.json'))
    pending=[]
    for item in manifest['files']:
        rel=PurePosixPath(item['path'])
        assert not rel.is_absolute() and rel.parts[0]=='compatibility'
        assert all(p not in ('.','..','.git') for p in rel.parts)
        data=z.read('blobs/'+item['sha256'])
        assert len(data)==item['bytes'] and hashlib.sha256(data).hexdigest()==item['sha256']
        target=ROOT.joinpath(*rel.parts)
        assert target.resolve().is_relative_to(ROOT/'compatibility')
        if target.exists():
            assert target.is_file() and target.read_bytes()==data,'Existing file differs; use a clean directory: '+str(target)
        else:pending.append((target,data))
    for target,data in pending:
        target.parent.mkdir(parents=True,exist_ok=True)
        target.write_bytes(data)
print('Verified',len(manifest['files']),'files; restored',len(pending),'missing files.')
