#!/usr/bin/env python3
"""Owned property userdata on every real character record; no gameplay."""
import argparse,hashlib,json,re,struct,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
TRACE_SHA='d8d70e780756d63e931ad5c2555abeda283dd51d81ff6bd049c8bbcbfd621420'
def main():
    p=argparse.ArgumentParser()
    for n in ('runner','cache','report'):p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--adb',type=Path);p.add_argument('--serial');a=p.parse_args()
    if bool(a.adb)!=bool(a.serial):p.error('--adb and --serial must be paired')
    trace_path=REPO/'reports/character-property-reader-trace.json';assert sha(trace_path)==TRACE_SHA
    trace=json.loads(trace_path.read_text());source=a.cache/trace['cache']['path'];assert sha(source)==trace['cache']['sha256']
    raw=source.read_bytes();count=struct.unpack_from('<I',raw)[0];assert count==448 and len(raw)==401608
    records=[list(struct.unpack_from('<224i',raw,4+row*896)) for row in range(count)]
    defaults,types=records[:2]
    def expected(value,default,type):
        if type==-1:type=16
        if type&7 or type&16 and not type&32:return value
        if type&32:
            if value==default:return default
            bits=(value+default)&0xffffffff;return bits if bits<0x80000000 else bits-0x100000000
        return default
    staging=ROOT/('build/properties-corpus-'+(a.serial or 'host'));staging.mkdir(parents=True,exist_ok=True)
    base='/data/local/tmp/dh2-lua-properties-qa';data=staging/'character.bin';data.write_bytes(raw)
    files=[];paths=[]
    for start in range(0,count,16):
        lines=[]
        for row in range(start,min(start+16,count)):
            lines.append(f'do local c=DH2CreatePropertyState({row})')
            for id,value in enumerate(records[row]):lines.append(f'assert(c:GetProp({id})=={expected(value,defaults[id],types[id])})')
            if row==0:
                for id,value in enumerate(defaults):lines.append(f'assert(c:GetProp({id},true)=={value})')
            lines.append('end')
        f=staging/f'{start//16:03}.lua';f.write_text('\n'.join(lines)+'\n',encoding='ascii',newline='\n');assert f.stat().st_size<1024*1024
        files.append(f);paths.append(base+'/'+f.name if a.adb else str(f.resolve()))
    listing=staging/'list.txt';listing.write_text('\n'.join(paths)+'\n',encoding='ascii',newline='\n')
    def run(*args):return subprocess.run(list(map(str,args)),check=True,capture_output=True,text=True).stdout.strip()
    hashes={};device=None
    if a.adb:
        adb=lambda *args:run(a.adb,'-s',a.serial,*args)
        assert adb('shell','getprop','ro.kernel.qemu')=='1'
        device={'serial':a.serial,'android_release':adb('shell','getprop','ro.build.version.release'),'sdk':int(adb('shell','getprop','ro.build.version.sdk')),
                'page_size':int(adb('shell','getconf','PAGE_SIZE')),'abi':adb('shell','getprop','ro.product.cpu.abi'),'fingerprint':adb('shell','getprop','ro.build.fingerprint')}
        assert device['android_release']=='17' and device['sdk']==37 and device['abi']=='x86_64' and device['page_size'] in (4096,16384)
        adb('shell','mkdir','-p',base);adb('push',str(staging)+'/.' ,base+'/');adb('push',a.runner,base+'/runner');adb('shell','chmod','755',base+'/runner')
        locals=[data,*files,listing,a.runner];remotes=[base+'/'+x.name for x in locals[:-1]]+[base+'/runner']
        for line in adb('shell','sha256sum',*remotes).splitlines():digest,path=line.split(None,1);hashes[path.strip()]=digest
        for remote,local in zip(remotes,locals):assert hashes[remote]==sha(local)
        output=adb('shell',base+'/runner','--properties',base+'/character.bin',base+'/list.txt')
    else:output=run(a.runner.resolve(),'--properties',data.resolve(),listing.resolve())
    assert output.splitlines()[:2]==['SELFTEST PASS','NUMERIC PASS 504']
    assert output.splitlines()[-1]=='PROPERTY CORPUS PASS 28'
    r={'complete_game':False,'gameplay_object_callbacks_installed':False,'property_userdata_binding_tested':True,'original_lua_equivalence_tested':False,
       'scope':'Authored Lua property objects own four native sheets and dataset generations. All 448 real rows and 100352 composed final fields are checked against an authored reference of separately reconstructed composition rules, plus 224 serialized default queries. Runtime selftests cover typed methods, buffer release, malformed/OOM import retention and old-object dataset lifetime. No real Character, derived base stats, buffs or combat/game loop.',
       'rows':448,'final_field_queries':100352,'default_queries':224,'assertion_files':28,'reader_trace_sha256':sha(trace_path),'cache_sha256':sha(source),
       'runner_sha256':sha(a.runner),'test_sha256':sha(Path(__file__)),'listing_sha256':sha(listing),
       'assertions':[{'name':x.name,'sha256':sha(x),'bytes':x.stat().st_size} for x in files],
       'source_sha256':{n:sha(ROOT/n) for n in ('runtime.c','runtime.h','tests/runner.c','tests/properties.c','../lua-character/bridge.c','../lua-character/methods.c')},'stdout':output}
    if device:r['device']=device;r['pushed_input_hashes_verified']=len(hashes)
    a.report.write_text(json.dumps(r,indent=2)+'\n');print(json.dumps({k:v for k,v in r.items() if k not in ('assertions','stdout')}))
if __name__=='__main__':main()
