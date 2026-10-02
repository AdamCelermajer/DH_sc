#!/usr/bin/env python3
"""Import exact Lua 5.1.4 C/header sources and license from the official archive."""
import argparse
import hashlib
import json
from pathlib import Path
import tarfile

ROOT=Path(__file__).resolve().parents[1]/'port/lua-runtime'
SHA='b038e225eaf2a5b57c9bcc35cd13aa8c6c8288ef493d52970c9545074098af3a'
def main():
    p=argparse.ArgumentParser();p.add_argument('--archive',type=Path,required=True);a=p.parse_args()
    assert a.archive.stat().st_size==216679
    assert hashlib.sha256(a.archive.read_bytes()).hexdigest()==SHA
    files=[]
    with tarfile.open(a.archive,'r:gz')as archive:
        for member in archive.getmembers():
            path=Path(member.name)
            selected=member.name=='lua-5.1.4/COPYRIGHT' or (
                path.parts[:2]==('lua-5.1.4','src') and len(path.parts)==3 and path.suffix in ('.c','.h'))
            if not selected:continue
            assert member.isfile() and not member.issym() and '..'not in path.parts
            data=archive.extractfile(member).read();target=ROOT/'vendor'/path
            if target.exists():assert target.read_bytes()==data
            else:target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(data)
            files.append({'path':target.relative_to(ROOT).as_posix(),'archive_member':member.name,
                          'bytes':len(data),'sha256':hashlib.sha256(data).hexdigest()})
    result={'upstream':'https://www.lua.org/ftp/lua-5.1.4.tar.gz','checksum_source':'https://www.lua.org/ftp/',
            'archive_bytes':216679,'archive_sha256':SHA,'license':'MIT, exact upstream COPYRIGHT retained',
            'files':sorted(files,key=lambda x:x['path']),'import_tool_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest()}
    (ROOT/'vendor-manifest.json').write_text(json.dumps(result,indent=2)+'\n')
    print('Exact upstream files:',len(files))
if __name__=='__main__':main()
