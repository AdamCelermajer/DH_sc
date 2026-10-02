#!/usr/bin/env python3
"""Installed APK: exact combat script with owned actors/constants and recovery."""
import argparse,hashlib,importlib.util,json,struct,time,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
spec=importlib.util.spec_from_file_location('combat_ui',ROOT/'tests/animation_runtime.py')
ui=importlib.util.module_from_spec(spec);spec.loader.exec_module(ui)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser()
    for n in ('adb','apk','ui-helper','cache','controlled','real','evidence','report'):p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--serial',required=True);a=p.parse_args();a.animation='';a.blend_animation=None
    check=ui.Check(a);run=check.run
    device={'serial':a.serial,'release':run('shell','getprop','ro.build.version.release').strip(),'sdk':int(run('shell','getprop','ro.build.version.sdk')),'page_size':int(run('shell','getconf','PAGE_SIZE')),'abi':run('shell','getprop','ro.product.cpu.abi').strip()}
    assert run('shell','getprop','ro.kernel.qemu').strip()=='1'and device['release']=='17'and device['sdk']==37 and device['abi']=='x86_64'and device['page_size']in (4096,16384)
    assert sha(a.apk)==json.loads((ROOT/'build-validation.json').read_text(encoding='utf-8'))['apk']['sha256']
    with zipfile.ZipFile(a.apk)as z:assert hashlib.sha256(z.read('assets/dh2/scripts/combat-formulas.lua')).hexdigest()=='f83639a12c5c1910f9a23fff494f4013330943ad4ef8f4d91c33fe1eb975565f'
    reports={mode:REPO/f'port/lua-runtime/combat-{mode}-host-validation.json'for mode in ('controlled','real')}
    corpus={mode:json.loads(path.read_text(encoding='utf-8'))for mode,path in reports.items()}
    assert corpus['controlled']['combat_cases']==216 and corpus['real']['combat_cases']==3568
    run('install','-r',a.ui_helper.resolve());run('install','-r',a.apk.resolve());start=run('shell','date','+%m-%d_%H:%M:%S.000').strip().replace('_',' ')
    run('shell','am','force-stop',ui.PACKAGE);run('shell','am','start','-n',ui.PACKAGE+'/.MainActivity');time.sleep(1)
    pid=run('shell','pidof',ui.PACKAGE).strip();assert pid.isdigit();results=[];directory='/sdcard/Download/dh2-source-combat-qa';run('shell','mkdir','-p',directory)
    def imported(name,source,kind='script',expected=None):
        path=a.evidence/('dh2qa_combat_'+name+('.lua'if kind=='script'else'.bin'));path.write_bytes(source);remote=directory+'/'+path.name
        run('push',path.resolve(),remote);assert run('shell','sha256sum',remote).split()[0]==sha(path)
        button={'script':'IMPORT SCRIPT SOURCE','properties':'IMPORT CHARACTER PROPERTIES','items':'IMPORT ITEM DATA','constants':'IMPORT SCRIPT CONSTANTS'}[kind]
        label={'script':'Script','properties':'Properties','items':'Items','constants':'Constants'}[kind]
        check.import_file(button,path.name);text=check.find(**{'content-desc':'Script status'}).get('text');assert(expected or label+' loaded.')in text,(name,text)
        assert run('shell','pidof',ui.PACKAGE).strip()==pid
        results.append({'name':name,'kind':kind,'bytes':len(source),'sha256':sha(path),'status':text})
    imported('before_constants',b"assert(GetPyCst('AIStates','Stunned')==0);assert(type(Rand)=='function')")
    for name in ('ai','design'):imported(name,(a.cache/f'data/pydata/{name}_pycst.bin').read_bytes(),'constants')
    imported('constant_values',b"assert(GetPyCst('AIStates','Stunned')==9);assert(GetPyCst('CombatAttackTypes','Magic')==2);assert(GetPyCst('CombatConstants','COMBAT_MAGIC_ARMOR_ABSORBTION')==51)")
    for mode,stage in (('real',a.real),('controlled',a.controlled)):
        known={row['name']:row['sha256']for row in corpus[mode]['files']}
        for name,kind in (('properties.bin','properties'),('loot.bin','items')):
            path=stage/name;assert sha(path)==known[name];imported(mode+'_'+kind,path.read_bytes(),kind)
        indices=(0,55,111)if mode=='real'else range(7)
        for index in indices:
            path=stage/f'assertions-{index:03}.lua';assert sha(path)==known[path.name];imported(mode+'_'+str(index),path.read_bytes())
    imported('retained_actor',b"keeper=DH2CreatePropertyState(2);keeper:SetCombatContext(9,7,'retained');assert(keeper:GetState()==9 and keeper:GetHitCount()==7 and keeper:GetName()=='retained')")
    imported('malformed',b'\0','constants','Constants rejected:')
    imported('retained_constants',b"assert(GetPyCst('AIStates','Stunned')==9 and GetPyCst('CombatAttackTypes','Magic')==2);assert(keeper:GetName()=='retained');assert(pcall(function()keeper:SetCombatContext(0,65536,'changed')end)==false);assert(keeper:GetState()==9 and keeper:GetHitCount()==7)")
    # Real design group replacement, followed by reimport/recovery; no original
    # game object or event loop is implied by these authored assertions.
    group=b'CombatAttackTypes';key=b'Magic';raw=struct.pack('<II',1,len(group))+group+struct.pack('<II',1,len(key))+key+struct.pack('<i',9999)
    imported('replacement',raw,'constants');imported('replacement_query',b"assert(GetPyCst('CombatAttackTypes','Magic')==9999 and GetPyCst('AIStates','Stunned')==9)")
    imported('oversized',bytes(4*1024*1024+1),'constants','exceeds 4 MiB limit')
    imported('recovery',(a.cache/'data/pydata/design_pycst.bin').read_bytes(),'constants')
    imported('recovered',b"assert(GetPyCst('CombatAttackTypes','Magic')==2);DH2SeedRandom(0);assert(Rand(100)==49);CF_ClearCombatants();CF_SetCombatants(keeper,keeper,-1,false,false);assert(CF_CalcDamage(1234,3)==1234);CF_ClearCombatants();assert(keeper:GetName()=='retained')")
    visuals=[]
    for i,(button,relative)in enumerate([(ui.FIXTURES[0][0],ui.FIXTURES[0][1]),(ui.FIXTURES[1][0],ui.FIXTURES[1][1]),('IMPORT CHARACTER ANIMATION','data/3d/characters/prince/animations/prince_walk_dual.bdae')]):
        path=a.cache/relative;name='dh2qa_combat_visual_'+str(i)+path.suffix;remote=directory+'/'+name;run('push',path.resolve(),remote);assert run('shell','sha256sum',remote).split()[0]==sha(path)
        statuses=check.import_file(button,name);expected='335 vertices, 1092 indices, 18 bones'if 'BRES'in button else'25 tracks'if 'ANIMATION'in button else'256';assert any(expected in s for s in statuses)
        visuals.append({'path':relative,'sha256':sha(path),'statuses':statuses})
    shot=check.capture('combat-ready.png');installed_path=run('shell','pm','path',ui.PACKAGE).strip().split(':',1)[1];installed=a.evidence/'installed.apk';run('pull',installed_path,installed.resolve());assert sha(installed)==sha(a.apk)
    log=run('logcat','-d','-T',start,'--pid='+pid,'-v','brief');(a.evidence/'combat-logcat.txt').write_text(log,encoding='utf-8');assert 'FATAL EXCEPTION'not in log and 'Fatal signal'not in log
    report={'complete_game':False,'original_gameplay_equivalence_tested':False,'device':device,'apk_sha256':sha(a.apk),'installed_apk_hash_matches':True,'real_damage_cases':80,'controlled_combat_cases':216,'cases':results,'visual_imports':visuals,'process_survived_all_cases':True,'fatal_in_run':False,'screenshot':shot,'test_sha256':sha(Path(__file__)),
            'corpus_report_sha256':{mode:sha(path)for mode,path in reports.items()},'source_sha256':{n:sha(ROOT/n)for n in ('scripts.cpp','src/local/dh2/sourceviewer/MainActivity.java','build.py')},
            'scope':'Built-in exact recovered combat formula uses source offline Rand, owned actor projections and real design/AI constants. Selected 80 real damage cases and all 216 controlled cases match host assertion chunks; retained actor context and malformed/replacement/oversized constant import recovery pass. Source render remains a diagnostic preview. No original Character/state machine/HP application, combat events, AI or game loop is connected.'}
    a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({k:v for k,v in report.items()if k not in ('cases','scope','source_sha256')}))
if __name__=='__main__':main()
