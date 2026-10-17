#!/usr/bin/env python3
"""Exercise APK-owned property data and typed methods through document pickers."""
import argparse,hashlib,importlib.util,json,struct,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
spec=importlib.util.spec_from_file_location('viewer_ui',ROOT/'tests/animation_runtime.py')
ui=importlib.util.module_from_spec(spec);spec.loader.exec_module(ui)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser()
    for name in ('adb','apk','ui-helper','cache','evidence','report'):p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--serial',required=True);a=p.parse_args();a.animation='';a.blend_animation=None
    check=ui.Check(a);run=check.run
    device={'serial':a.serial,'android_release':run('shell','getprop','ro.build.version.release').strip(),
            'sdk':int(run('shell','getprop','ro.build.version.sdk')),'page_size':int(run('shell','getconf','PAGE_SIZE')),
            'abi':run('shell','getprop','ro.product.cpu.abi').strip()}
    assert run('shell','getprop','ro.kernel.qemu').strip()=='1'
    assert device['android_release']=='17' and device['sdk']==37 and device['abi']=='x86_64' and device['page_size'] in (4096,16384)
    assert sha(a.apk)==json.loads((ROOT/'build-validation.json').read_text())['apk']['sha256']
    trace=json.loads((REPO/'reports/character-property-reader-trace.json').read_text());data=a.cache/trace['cache']['path']
    assert sha(data)==trace['cache']['sha256'];raw=data.read_bytes()
    run('install','-r',a.ui_helper.resolve());run('install','-r',a.apk.resolve())
    start=run('shell','date','+%m-%d_%H:%M:%S.000').strip().replace('_',' ')
    run('shell','am','force-stop',ui.PACKAGE);run('shell','am','start','-n',ui.PACKAGE+'/.MainActivity');time.sleep(1)
    pid=run('shell','pidof',ui.PACKAGE).strip();assert pid.isdigit()
    directory='/sdcard/Download/dh2-source-properties-qa';run('shell','mkdir','-p',directory);results=[]
    def imported(name,source,properties=False,expected=None):
        path=a.evidence/('dh2qa_properties_'+name+('.bin' if properties else '.lua'));path.write_bytes(source)
        remote=directory+'/'+path.name;run('push',path.resolve(),remote);assert run('shell','sha256sum',remote).split()[0]==sha(path)
        check.import_file('IMPORT CHARACTER PROPERTIES' if properties else 'IMPORT SCRIPT SOURCE',path.name)
        text=check.find(**{'content-desc':'Script status'}).get('text')
        expected=expected or ('Properties loaded.' if properties else 'Script loaded.')
        assert expected in text,(name,text);assert run('shell','pidof',ui.PACKAGE).strip()==pid
        results.append({'case':name,'properties':properties,'bytes':len(source),'sha256':sha(path),'status':text})
    imported('not_loaded',b'assert(pcall(DH2CreatePropertyState,2)==false)')
    imported('real_cache',raw,True)
    records=[struct.unpack_from('<224i',raw,4+row*896)for row in range(448)];defaults,types=records[:2]
    def expected(v,d,t):
        if t==-1:t=16
        if t&7 or t&16 and not t&32:return v
        if t&32:
            if v==d:return d
            bits=(v+d)&0xffffffff;return bits if bits<0x80000000 else bits-0x100000000
        return d
    lines=[]
    for row in (0,2,447):
        lines.append(f'do local c=DH2CreatePropertyState({row})')
        for id,value in enumerate(records[row]):lines.append(f'assert(c:GetProp({id})=={expected(value,defaults[id],types[id])});assert(c:GetProp({id},true)=={defaults[id]})')
        lines.append('end')
    imported('real_queries',('\n'.join(lines)+'\n').encode('ascii'))
    def fixture(default,level):
        b=bytearray(2700);struct.pack_into('<I',b,0,3)
        for offset,value in ((4+36*4,default),(900+36*4,8),(900+19*4,16),(1796+19*4,level)):struct.pack_into('<i',b,offset,value)
        return bytes(b)
    imported('generation_a',fixture(0,512),True)
    imported('methods',b'''old=DH2CreatePropertyState(2);assert(type(old)=='userdata' and getmetatable(old)==false)
assert(old:GetProp(19)==512);old:SetProp(36,257.9);assert(old:GetProp(36)==257)
assert(old:GetProp(36,true)==0);old:SetProp(19,77);assert(old:GetProp(19)==512)
assert(select('#',old:GetProp())==0 and select('#',old:GetProp('36'))==0)
assert(select('#',old:GetProp(224))==0 and old:GetProp(-1.2)==0)
assert(pcall(DH2CreatePropertyState,3)==false and pcall(DH2CreatePropertyState,1.5)==false)
assert(pcall(function()old:SetProp(36,1/0)end)==false)
assert(pcall(function()old:GetProp(36,old)end)==false and old:GetProp(36)==257)
''')
    imported('malformed',b'\x00',True,'Properties rejected:')
    imported('retained',b'assert(old:GetProp(36)==257 and DH2CreatePropertyState(2):GetProp(19)==512)')
    imported('generation_b',fixture(42,768),True)
    imported('replacement',b'''new=DH2CreatePropertyState(2);assert(new:GetProp(19)==768 and new:GetProp(36)==42)
assert(new:GetProp(36,true)==42 and old:GetProp(36,true)==0)
old:SetProp(36,500);collectgarbage('collect');assert(old:GetProp(36)==500 and new:GetProp(36)==42)
''')
    imported('oversize',b'\0'*(4*1024*1024+1),True,'exceeds 4 MiB limit')
    imported('recovery',b'assert(old:GetProp(36)==500 and DH2CreatePropertyState(2):GetProp(19)==768)')
    shot=check.capture('properties-ready.png')
    installed_path=run('shell','pm','path',ui.PACKAGE).strip().split(':',1)[1];installed=a.evidence/'installed.apk'
    run('pull',installed_path,installed.resolve());assert sha(installed)==sha(a.apk)
    log=run('logcat','-d','-T',start,'--pid='+pid,'-v','brief');(a.evidence/'properties-logcat.txt').write_text(log,encoding='utf-8')
    assert 'FATAL EXCEPTION'not in log and 'Fatal signal'not in log
    result={'complete_game':False,'scope':'APK property data imports, 1344 real final/default queries, typed methods, dataset generations, malformed and size rejection/recovery. No real Character, derived stats, buffs, combat or game loop.',
            'device':device,'apk_sha256':sha(a.apk),'installed_apk_hash_matches':True,'real_cache_sha256':sha(data),
            'cases':results,'process_survived_all_cases':True,'fatal_in_run':False,'screenshot':shot,
            'log_start_emulator_gmt':start,'test_sha256':sha(Path(__file__)),
            'source_sha256':{n:sha(ROOT/n)for n in ('scripts.cpp','src/local/dh2/sourceviewer/MainActivity.java','build.py')}}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items()if k!='cases'}))
if __name__=='__main__':main()
