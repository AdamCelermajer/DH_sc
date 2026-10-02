#!/usr/bin/env python3
"""Verify exact cache Lua source, optional Git index and syntax-only compiler."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import zipfile

REPO=Path(__file__).resolve().parents[1]
def main():
    p=argparse.ArgumentParser()
    p.add_argument('--cache',type=Path,help='Private complete-cache files directory')
    p.add_argument('--compiler',help='Lua 5.1 luac executable; parse only, never execute scripts')
    p.add_argument('--archive',type=Path,help='Pinned private complete-cache ZIP')
    p.add_argument('--git-index',action='store_true')
    p.add_argument('--report',type=Path)
    a=p.parse_args();manifest=json.loads((REPO/'recovered/scripts/manifest.json').read_text())
    failed=[];passed=0;index_checked=0
    sha=lambda b:hashlib.sha256(b).hexdigest()
    archive=None
    if a.archive:
        digest=hashlib.sha256()
        with a.archive.open('rb')as stream:
            for block in iter(lambda:stream.read(1024*1024),b''):digest.update(block)
        assert digest.hexdigest()==manifest['cache_archive_sha256']
        assert a.archive.stat().st_size==manifest['cache_archive_bytes']
        archive=zipfile.ZipFile(a.archive)
    if a.cache:
        actual={x.relative_to(a.cache).as_posix()for x in (a.cache/'data/scripts').rglob('*.luac')}
        assert actual=={x['cache_path']for x in manifest['files']},'Cache script set differs'
    syntax=[]
    for row in manifest['files']:
        path=REPO/row['path'];data=path.read_bytes()
        assert len(data)==row['bytes'] and sha(data)==row['sha256'],row['path']
        assert not data.startswith(b'\x1bLua') and b'\0'not in data,row['path']
        if a.cache:assert data==(a.cache/row['cache_path']).read_bytes(),row['path']
        if archive:assert archive.read(row['archive_member'])==data,row['path']
        if a.git_index:
            staged=subprocess.check_output(['git','show',':'+row['path']],cwd=REPO)
            assert staged==data,row['path'];index_checked+=1
        if a.compiler:
            result=subprocess.run([a.compiler,'-p',str(path)],capture_output=True)
            error=result.stderr.decode('utf-8','replace').replace(str(path),row['path'])
            entry={'path':row['path'],'syntax_ok':result.returncode==0}
            if result.returncode:entry['diagnostic']=error;failed.append(row['path'])
            else:passed+=1
            syntax.append(entry)
    overrides=[]
    for row in manifest['overrides']:
        original=(REPO/row['original']).read_bytes();path=REPO/row['path'];data=path.read_bytes()
        assert sha(data)==row['sha256'] and len(data)==row['bytes'],row['path']
        assert original.count(row['replace_from'].encode())==1
        assert data==original.replace(row['replace_from'].encode(),row['replace_to'].encode()),row['path']
        if a.git_index:assert subprocess.check_output(['git','show',':'+row['path']],cwd=REPO)==data
        if a.compiler:
            subprocess.run([a.compiler,'-p',str(path)],check=True)
            overrides.append({'path':row['path'],'syntax_ok':True})
    if a.compiler:
        assert set(failed)=={x['original']for x in manifest['overrides']},failed
    if archive:archive.close()
    result={'complete_game':False,'gameplay_tested':False,'scripts_executed':False,
        'files_checked':len(manifest['files']),'bytes_checked':sum(x['bytes']for x in manifest['files']),
        'cache_bytes_checked':bool(a.cache),'git_index_original_files_checked':index_checked,
        'cache_archive_checked':bool(a.archive),'script_archive_members_checked':len(manifest['files'])if a.archive else 0,
        'source_manifest_sha256':sha((REPO/'recovered/scripts/manifest.json').read_bytes()),
        'test_sha256':sha(Path(__file__).read_bytes())}
    if a.compiler:
        version=subprocess.run([a.compiler,'-v'],capture_output=True,check=True)
        compiler=Path(a.compiler).resolve()
        result.update({'compiler_version':(version.stdout+version.stderr).decode().strip(),
            'compiler_sha256':sha(compiler.read_bytes()),'original_syntax_passed':passed,
            'original_syntax_failed':len(failed),'syntax_results':syntax,'overrides':overrides})
    if a.report:a.report.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:v for k,v in result.items()if k!='syntax_results'}))
if __name__=='__main__':main()
