"""Check the native APK ZIP filesystem against every supplied cache file."""
import hashlib,io,json,struct,subprocess,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
ZIP=Path(r'C:\Users\adamc\Downloads\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
PREFIX='com.gameloft.android.GAND.GloftD2SS/files/'
OUTPUT=ROOT/'port/asset-payloads/reports/zip-asset-pack-v1-host-audit.json'
SCRATCH=ROOT/'.local-inputs/zip-asset-pack-v1'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/c/'+str(p.resolve()).replace('\\','/')[3:]
def main():
    assert not OUTPUT.exists(),'Preserve accepted receipt'
    assert sha(ZIP)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
    SCRATCH.mkdir(exist_ok=True)
    rows=[]
    with zipfile.ZipFile(ZIP) as z:
        for info in z.infolist():
            if info.is_dir():continue
            assert info.filename.startswith(PREFIX)
            data=z.read(info);rows.append((info.filename[len(PREFIX):].lower(),len(data),hashlib.sha256(data).hexdigest()))
    assert len(rows)==6833 and len(set(x[0] for x in rows))==6833
    expected=SCRATCH/'expected.tsv';expected.write_text(''.join(f'{u}\t{n}\t{s}\n' for u,n,s in sorted(rows)),newline='\n')
    fixtures=[]
    def add(name,raw,mode):
        p=SCRATCH/(name+'.zip');p.write_bytes(raw);fixtures.append(f'{mode}\t{linux(p)}\n')
    def archive(method=zipfile.ZIP_DEFLATED,name='root/data/a',flag=False):
        class NonSeek(io.BytesIO):
            def seekable(self):return False
            def seek(self,*args):raise io.UnsupportedOperation()
        b=NonSeek() if flag else io.BytesIO()
        with zipfile.ZipFile(b,'w',compression=method) as z:z.writestr(name,b'native cache payload')
        return b.getvalue()
    raw=archive();add('deflate',raw,'read_pass');add('stored',archive(zipfile.ZIP_STORED),'read_pass');add('descriptor',archive(flag=True),'read_pass')
    add('truncated',raw[:-10],'mount_fail');add('traversal',archive(name='root/../a'),'mount_fail');add('foreign-root',archive(name='outside/a'),'mount_fail')
    def change(name,at,value,mode):
        b=bytearray(raw);struct.pack_into('<I',b,at,value);add(name,b,mode)
    central=raw.index(b'PK\x01\x02');change('local-crc',14,0,'read_fail');change('central-crc',central+16,0,'read_fail')
    b=bytearray(raw);b[30+len('root/data/a')]^=0xff;add('deflate-corruption',b,'read_fail')
    b=bytearray(raw);struct.pack_into('<H',b,central+8,1);add('encrypted',b,'mount_fail')
    b=bytearray(raw);struct.pack_into('<I',b,central+24,0xffffffff);add('zip64',b,'mount_fail')
    b=bytearray(raw);struct.pack_into('<H',b,central+10,99);add('compression',b,'mount_fail')
    b=io.BytesIO()
    with zipfile.ZipFile(b,'w') as z:z.writestr('root/DATA/a',b'x');z.writestr('root/data/a',b'y')
    add('duplicate',b.getvalue(),'mount_fail')
    guard=SCRATCH/'guards.tsv';guard.write_text(''.join(fixtures),newline='\n')
    paths=[ROOT/'port/asset-payloads'/p for p in ('zip_asset_pack_v1.hpp','zip_asset_pack_v1.cpp','sha256.hpp','sha256.cpp','tests/zip_asset_pack_v1.cpp','tests/zip_asset_pack_v1_host.py')]
    before={p.relative_to(ROOT).as_posix():sha(p) for p in paths}
    commands=[]
    def run(args):
        p=subprocess.run(['wsl.exe','--cd',linux(ROOT),*args],capture_output=True,text=True,timeout=120)
        commands.append(dict(arguments=args,returncode=p.returncode,stdout=p.stdout,stderr=p.stderr))
        assert p.returncode==0 and not p.stderr.strip(),commands[-1]
        return p.stdout.strip()
    binary='/home/adampalace/dh2-zip-asset-pack-v1-audit'
    run(['g++','-std=c++17','-O1','-g','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-fno-omit-frame-pointer',linux(paths[1]),linux(paths[3]),linux(paths[4]),'-lz','-o',binary])
    result=json.loads(run(['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',binary,linux(ZIP),linux(expected),linux(guard)]))
    assert result['validation']=='PASS' and result['files']==6833 and result['mismatches']==0
    assert before=={p.relative_to(ROOT).as_posix():sha(p) for p in paths}
    result.update(source_sha256=before,original_cache_sha256=sha(ZIP),expected_sha256=sha(expected),guards_sha256=sha(guard),commands=commands,sanitizer_findings=0,binary_sha256=run(['sha256sum',binary]).split()[0],live_Android_verified=False,packaged_APK=False)
    OUTPUT.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:result[k] for k in ('validation','files','uncompressed_bytes','guards','sanitizer_findings')}))
if __name__=='__main__':main()
