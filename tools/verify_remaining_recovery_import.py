#!/usr/bin/env python3
"""Verify recovered shader/configuration/debug evidence and optional pinned ZIP.

This checks archive identity and exact bytes. It does not validate semantics or
turn the recovered evidence into a complete source-built game.
"""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import subprocess
import zipfile

ARCHIVE_SHA256='b3ff974e2b74f50387465d5665f60d56ac79c29a449c6299745461998045c4d8'
PREFIXES=('recovered/assets/source-data/','recovered/native/debug/')
EXTRA='recovered/assets/cache-manifest.json'

def digest(data):return hashlib.sha256(data).hexdigest()

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--repo-root',type=Path,default=Path(__file__).resolve().parents[1])
    p.add_argument('--archive',type=Path)
    p.add_argument('--git-index',action='store_true',help='Also verify every imported file is tracked with these exact bytes')
    a=p.parse_args();root=a.repo_root.resolve()
    ledger=json.loads((root/'reports/remaining-recovery-evidence-import.json').read_text())
    if ledger.get('schema_version')!=1 or ledger.get('source_archive_sha256')!=ARCHIVE_SHA256:
        raise ValueError('Unexpected import identity')
    rows=ledger['files'];seen=set()
    for row in rows:
        path=row['path'];rel=PurePosixPath(path)
        if (not path.startswith(PREFIXES) and path!=EXTRA) or rel.is_absolute() or '..' in rel.parts or any(':' in x for x in rel.parts):
            raise ValueError('Unexpected evidence path: '+path)
        if path in seen:raise ValueError('Duplicate evidence path: '+path)
        seen.add(path);target=root.joinpath(*rel.parts)
        if target.is_symlink() or not target.is_file() or not target.resolve().is_relative_to(root):
            raise ValueError('Missing or unsafe evidence path: '+path)
        raw=target.read_bytes()
        if len(raw)!=row['bytes'] or digest(raw)!=row['sha256']:
            raise ValueError('Evidence differs: '+path)
        if row['archive_member']!='dh2-reconstruction/'+path:
            raise ValueError('Archive member differs: '+path)
    actual={p.relative_to(root).as_posix()for prefix in PREFIXES
            for p in (root/prefix).rglob('*')if p.is_file()}|{EXTRA}
    if actual!=seen:raise ValueError('Unrecorded or missing evidence: '+str(sorted(actual^seen)))
    if a.git_index:
        raw_index=subprocess.check_output(['git','-C',str(root),'ls-files','--stage','-z','--',*PREFIXES,EXTRA])
        index={}
        for item in raw_index.split(b'\0'):
            if not item:continue
            metadata,path=item.split(b'\t',1);mode,identity,stage=metadata.split()
            if stage!=b'0' or mode!=b'100644':raise ValueError('Unexpected index mode/stage')
            index[path.decode('utf-8')]=identity.decode('ascii')
        if set(index)!=seen:raise ValueError('Git index evidence set differs')
        for row in rows:
            raw=(root/row['path']).read_bytes()
            identity=hashlib.sha1(b'blob '+str(len(raw)).encode('ascii')+b'\0'+raw).hexdigest()
            if index[row['path']]!=identity:raise ValueError('Git index content differs: '+row['path'])
    provenance=json.loads((root/'recovered/assets/source-data/provenance.json').read_text())
    if len(provenance['files'])!=2164 or provenance['cache_archive_complete']:
        raise ValueError('Unexpected historical cache provenance')
    names=set()
    for row in provenance['files']:
        path='recovered/assets/source-data/'+row['output_path']
        if path not in seen or path in names:raise ValueError('Unexpected resource provenance path')
        names.add(path);raw=(root/path).read_bytes()
        if len(raw)!=row['size'] or digest(raw)!=row['sha256']:
            raise ValueError('Recovered resource provenance differs: '+path)
    if a.archive:
        if digest(a.archive.read_bytes())!=ARCHIVE_SHA256:raise ValueError('Source ZIP hash differs')
        with zipfile.ZipFile(a.archive)as source:
            bad=source.testzip()
            if bad is not None:raise ValueError('Source ZIP CRC failed: '+bad)
            members={name.removeprefix('dh2-reconstruction/')for name in source.namelist()
                     if name.startswith('dh2-reconstruction/') and not name.endswith('/')
                     and (name.removeprefix('dh2-reconstruction/').startswith(PREFIXES)
                          or name=='dh2-reconstruction/'+EXTRA)}
            if members!=seen:raise ValueError('Archive evidence set differs')
            for row in rows:
                raw=source.read(row['archive_member'])
                if len(raw)!=row['bytes'] or digest(raw)!=row['sha256']:
                    raise ValueError('Source ZIP member differs: '+row['path'])
    print(json.dumps({'files_verified':len(rows),'bytes_verified':sum(r['bytes']for r in rows),
                      'resource_provenance_verified':len(names),'archive_verified':bool(a.archive),
                      'git_index_verified':a.git_index,
                      'complete_game_source':False,'historical_cache_complete':False},indent=2))

if __name__=='__main__':main()
