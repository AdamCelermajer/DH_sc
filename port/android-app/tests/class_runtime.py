#!/usr/bin/env python3
"""Exercise owned class generations and native calculations in the APK."""
import argparse,hashlib,importlib.util,json,struct,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
spec=importlib.util.spec_from_file_location('viewer_ui',ROOT/'tests/animation_runtime.py')
ui=importlib.util.module_from_spec(spec);spec.loader.exec_module(ui)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser()
    for n in ('adb','apk','ui-helper','cache','assertions','evidence','report'):p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--serial',required=True);a=p.parse_args();a.animation='';a.blend_animation=None
    check=ui.Check(a);run=check.run
    device={'serial':a.serial,'android_release':run('shell','getprop','ro.build.version.release').strip(),'sdk':int(run('shell','getprop','ro.build.version.sdk')),
            'page_size':int(run('shell','getconf','PAGE_SIZE')),'abi':run('shell','getprop','ro.product.cpu.abi').strip()}
    assert run('shell','getprop','ro.kernel.qemu').strip()=='1'
    assert device['android_release']=='17' and device['sdk']==37 and device['abi']=='x86_64' and device['page_size'] in (4096,16384)
    assert sha(a.apk)==json.loads((ROOT/'build-validation.json').read_text())['apk']['sha256']
    corpus=json.loads((REPO/'port/lua-runtime/classes-host-execution-validation.json').read_text());assert corpus['application_cases']==520
    pr=a.cache/'data/pydata/character_properties_pyarray.bin';cr=a.cache/'data/pydata/character_classes_pyarray.bin'
    assert sha(pr)==corpus['property_cache_sha256'] and sha(cr)==corpus['class_cache_sha256']
    run('install','-r',a.ui_helper.resolve());run('install','-r',a.apk.resolve())
    start=run('shell','date','+%m-%d_%H:%M:%S.000').strip().replace('_',' ')
    run('shell','am','force-stop',ui.PACKAGE);run('shell','am','start','-n',ui.PACKAGE+'/.MainActivity');time.sleep(1)
    pid=run('shell','pidof',ui.PACKAGE).strip();assert pid.isdigit();results=[]
    directory='/sdcard/Download/dh2-source-class-qa';run('shell','mkdir','-p',directory)
    def imported(name,source,kind='script',expected=None):
        path=a.evidence/('dh2qa_class_'+name+('.lua'if kind=='script'else '.bin'));path.write_bytes(source)
        remote=directory+'/'+path.name;run('push',path.resolve(),remote);assert run('shell','sha256sum',remote).split()[0]==sha(path)
        buttons={'script':'IMPORT SCRIPT SOURCE','properties':'IMPORT CHARACTER PROPERTIES','classes':'IMPORT CHARACTER CLASSES'}
        check.import_file(buttons[kind],path.name);text=check.find(**{'content-desc':'Script status'}).get('text')
        expected=expected or {'script':'Script loaded.','properties':'Properties loaded.','classes':'Classes loaded.'}[kind]
        assert expected in text,(name,text);assert run('shell','pidof',ui.PACKAGE).strip()==pid
        results.append({'case':name,'kind':kind,'bytes':len(source),'sha256':sha(path),'status':text})
    imported('real_properties',pr.read_bytes(),'properties')
    imported('before',b"before=DH2CreatePropertyState(2);assert(pcall(function()before:ApplyClass(0)end)==false)")
    imported('real_classes',cr.read_bytes(),'classes')
    for index in (22,25):
        entry=corpus['assertions'][index];path=a.assertions/entry['name'];assert sha(path)==entry['sha256']
        imported('real_queries_'+str(index),path.read_bytes())
    props=bytearray(2700);struct.pack_into('<I',props,0,3)
    for offset,value in ((900+19*4,16),(900+20*4,16),(1796+19*4,512),(1796+20*4,1536)):struct.pack_into('<i',props,offset,value)
    imported('synthetic_properties',bytes(props),'properties')
    fixture=lambda coefficient:struct.pack('<II5i',1,1,20,1,-666,19,coefficient)
    imported('generation_a',fixture(256),'classes')
    imported('apply',b'''old=DH2CreatePropertyState(2);old:ApplyClass(0);assert(old:GetProp(20)==2048)
assert(pcall(function()before:ApplyClass(0)end)==false)
assert(pcall(function()old:ApplyClass(1)end)==false and pcall(function()old:ApplyClass(0,'true')end)==false)
assert(pcall(function()old:ApplyClass(0/0)end)==false and old:GetProp(20)==2048)
''')
    imported('malformed',b'\0','classes','Classes rejected:')
    imported('retained',b"old:ApplyClass(0,true);assert(old:GetProp(20)==2560)")
    imported('generation_b',fixture(512),'classes')
    imported('replacement',b"new=DH2CreatePropertyState(2);new:ApplyClass(0);assert(new:GetProp(20)==2560);old:ApplyClass(0);collectgarbage('collect');assert(old:GetProp(20)==3072)")
    imported('cycle',struct.pack('<II5i',1,1,-1,0,0,-1,-1),'classes')
    imported('atomic',b"cyclic=DH2CreatePropertyState(2);assert(pcall(function()cyclic:ApplyClass(0)end)==false);assert(cyclic:GetProp(20)==1536 and new:GetProp(20)==2560)")
    imported('oversize',b'\0'*(4*1024*1024+1),'classes','exceeds 4 MiB limit')
    imported('recovery',fixture(256),'classes')
    imported('recovered',b"local c=DH2CreatePropertyState(2);c:ApplyClass(0);assert(c:GetProp(20)==2048 and old:GetProp(20)==3072)")
    shot=check.capture('classes-ready.png')
    path=run('shell','pm','path',ui.PACKAGE).strip().split(':',1)[1];installed=a.evidence/'installed.apk';run('pull',path,installed.resolve());assert sha(installed)==sha(a.apk)
    log=run('logcat','-d','-T',start,'--pid='+pid,'-v','brief');(a.evidence/'classes-logcat.txt').write_text(log,encoding='utf-8');assert 'FATAL EXCEPTION'not in log and 'Fatal signal'not in log
    result={'complete_game':False,'scope':'Exact APK: real property/class imports, 40 real class applications and 8960 final-field queries from checked host corpus; owned generations, typed arguments, malformed/oversize rejection, cyclic atomic failure and recovery. Authored ApplyClass has empty buffs and no original Character/gameplay lifecycle.',
            'device':device,'apk_sha256':sha(a.apk),'installed_apk_hash_matches':True,'corpus_report_sha256':sha(REPO/'port/lua-runtime/classes-host-execution-validation.json'),
            'real_application_cases':40,'real_final_field_queries':8960,'cases':results,'process_survived_all_cases':True,'fatal_in_run':False,'screenshot':shot,
            'log_start_emulator_gmt':start,'test_sha256':sha(Path(__file__)),
            'source_sha256':{n:sha(ROOT/n)for n in ('scripts.cpp','src/local/dh2/sourceviewer/MainActivity.java','build.py')}}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items()if k!='cases'}))
if __name__=='__main__':main()
