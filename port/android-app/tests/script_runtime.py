#!/usr/bin/env python3
"""Check the APK-owned Lua session through its live Android document picker."""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import time
import zipfile
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
spec=importlib.util.spec_from_file_location('viewer_ui',ROOT/'tests/animation_runtime.py')
ui=importlib.util.module_from_spec(spec);spec.loader.exec_module(ui)
PROBE=b'''assert(GetPyStruct("CharacterProperties","HP")==36)
assert(GetPyOID("CharacterProperties","Level")==19)
assert(type(CF_CalcDamage)=="function" and type(DeclareSkill)=="function")
CF_ClearCombatants();assert(CF_CalcDamage(0,0)==false)
local usable,active=OnSkillCheck();assert(usable==false and active==false)
local n=0;AttachToAnimEvent("apk-probe",function()n=n+1 end)
OnAnimEvent("apk-probe");DetachFromAnimEvent("apk-probe");OnAnimEvent("apk-probe");assert(n==1)
assert(PlayAnim==nil and Rand==nil and GetProp==nil)
'''

def main():
    p=argparse.ArgumentParser()
    for name in ('adb','apk','ui-helper','evidence','report'):p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--serial',required=True);a=p.parse_args()
    a.animation='';a.blend_animation=None
    check=ui.Check(a);run=check.run;sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
    assert run('shell','getprop','ro.kernel.qemu').strip()=='1'
    device={'serial':a.serial,'android_release':run('shell','getprop','ro.build.version.release').strip(),
            'sdk':int(run('shell','getprop','ro.build.version.sdk')),
            'page_size':int(run('shell','getconf','PAGE_SIZE')),
            'abi':run('shell','getprop','ro.product.cpu.abi').strip()}
    assert device['android_release']=='17'and device['sdk']==37 and device['abi']=='x86_64'
    assert device['page_size']in (4096,16384)
    expected=json.loads((ROOT/'build-validation.json').read_text())
    assert sha(a.apk)==expected['apk']['sha256']
    with zipfile.ZipFile(a.apk)as apk:
        for name,row in expected['script_assets'].items():
            raw=apk.read(name);assert len(raw)==row['bytes']and hashlib.sha256(raw).hexdigest()==row['sha256']
    run('install','-r',a.ui_helper.resolve());run('install','-r',a.apk.resolve())
    start=run('shell','date','+%m-%d_%H:%M:%S.000').strip().replace('_',' ')
    run('shell','am','force-stop',ui.PACKAGE)
    run('shell','am','start','-n',ui.PACKAGE+'/.MainActivity');time.sleep(1)
    def status():return check.find(**{'content-desc':'Script status'}).get('text')
    initial=status();assert initial=='Scripts ready: 3 shared files. Game objects are not connected yet.',initial
    pid=run('shell','pidof',ui.PACKAGE).strip();assert pid.isdigit()
    evidence=check.capture('scripts-ready.png')
    directory='/sdcard/Download/dh2-source-script-qa';run('shell','mkdir','-p',directory)
    inputs=[('probe',PROBE,'Script loaded.'),('loop',b'while true do end','instruction budget exhausted'),
            ('nonstring',b'error({})','non-string Lua error'),
            ('invalidbytes',b'error(string.char(255,254))','Script rejected:'),
            ('bytecode',b'\x1bLua','bytecode input is unsupported'),
            ('memory',b'local t={};local i=0;while true do i=i+1;t[i]=string.rep("a",65536)..i end','memory'),
            ('recovery',b'collectgarbage("collect");\n'+PROBE,'Script loaded.')]
    results=[]
    for name,source,expected_text in inputs:
        path=a.evidence/('dh2qa_script_'+name+'.lua');path.write_bytes(source)
        remote=directory+'/'+path.name;run('push',path.resolve(),remote)
        remote_hash=run('shell','sha256sum',remote).split()[0];assert remote_hash==sha(path)
        check.import_file('IMPORT SCRIPT SOURCE',path.name)
        text=status();assert expected_text in text,(name,text)
        assert run('shell','pidof',ui.PACKAGE).strip()==pid
        results.append({'case':name,'source_sha256':sha(path),'bytes':len(source),'status':text})
    installed_path=run('shell','pm','path',ui.PACKAGE).strip().split(':',1)[1]
    installed=a.evidence/'installed.apk';run('pull',installed_path,installed.resolve());assert sha(installed)==sha(a.apk)
    log=run('logcat','-d','-T',start,'--pid='+pid,'-v','brief');(a.evidence/'scripts-logcat.txt').write_text(log,encoding='utf-8')
    assert 'FATAL EXCEPTION'not in log and 'Fatal signal'not in log,log[-2000:]
    result={'complete_game':False,'scope':'APK-owned source runtime initializes three exact shared scripts; controlled source imports test fields, shared helpers, rejection and recovery. No native game objects, real combat or game loop.',
            'device':device,'apk_sha256':sha(a.apk),'installed_apk_hash_matches':True,
            'script_assets_hash_checked':expected['script_assets'],'initial_status':initial,
            'cases':results,'process_survived_all_cases':True,'fatal_in_run':False,
            'log_start_emulator_gmt':start,
            'screenshot':evidence,'test_sha256':sha(Path(__file__)),
            'source_sha256':{name:sha(ROOT/name)for name in ('scripts.cpp','src/local/dh2/sourceviewer/MainActivity.java','build.py')}}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items()if k not in ('cases','script_assets_hash_checked')}))
if __name__=='__main__':main()
