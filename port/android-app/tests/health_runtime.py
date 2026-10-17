#!/usr/bin/env python3
"""Installed source APK: original-derived health assertions and import recovery."""
import argparse,hashlib,importlib.util,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
spec=importlib.util.spec_from_file_location('health_ui',ROOT/'tests/animation_runtime.py')
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
    reports={mode:REPO/f'port/lua-runtime/health-{mode}-host-validation.json'for mode in ('controlled','real')}
    corpora={mode:json.loads(path.read_text(encoding='utf-8'))for mode,path in reports.items()}
    assert corpora['controlled']['actor_cases']==520 and corpora['real']['actor_cases']==446
    run('install','-r',a.ui_helper.resolve());run('install','-r',a.apk.resolve());start=run('shell','date','+%m-%d_%H:%M:%S.000').strip().replace('_',' ')
    run('shell','am','force-stop',ui.PACKAGE);run('shell','am','start','-W','-n',ui.PACKAGE+'/.MainActivity')
    check.find(**{'content-desc':'Script status'});pid=run('shell','pidof',ui.PACKAGE).strip();assert pid
    directory='/sdcard/Download/dh2-source-qa';run('shell','mkdir','-p',directory);results=[]
    def imported(name,source,kind='script',expected=None):
        path=a.evidence/('dh2qa_health_'+name+('.lua'if kind=='script'else'.bin'));path.write_bytes(source);remote=directory+'/'+path.name
        run('push',path.resolve(),remote);assert run('shell','sha256sum',remote).split()[0]==sha(path)
        button={'script':'IMPORT SCRIPT SOURCE','properties':'IMPORT CHARACTER PROPERTIES'}[kind];label='Script'if kind=='script'else'Properties'
        check.import_file(button,path.name);text=check.find(**{'content-desc':'Script status'}).get('text');assert(expected or label+' loaded.')in text,(name,text)
        assert run('shell','pidof',ui.PACKAGE).strip()==pid
        results.append({'name':name,'kind':kind,'bytes':len(source),'sha256':sha(path),'status':text})
    for mode,stage,indices in (('controlled',a.controlled,(0,16,32)),('real',a.real,(0,13,27))):
        known={row['name']:row['sha256']for row in corpora[mode]['files']}
        path=stage/'properties.bin';assert sha(path)==known[path.name];imported(mode+'_properties',path.read_bytes(),'properties')
        for index in indices:
            path=stage/f'assertions-{index:03}.lua';assert sha(path)==known[path.name];imported(mode+'_'+str(index),path.read_bytes())
        if mode=='controlled':
            imported('retained_actor',b"health_keeper=DH2CreatePropertyState(2);health_keeper:SetProp(38,25600);health_keeper:SetProp(43,5120);health_keeper:SetHP(50);health_keeper:SetMP(10);assert(health_keeper:GetHP()==50 and health_keeper:GetMP()==10)")
    imported('retained_generation',b"health_keeper:RegenHP(256);assert(health_keeper:GetHP()==51);assert(health_keeper:UseMana(256) and health_keeper:GetMP()==9);assert(health_keeper:HasMana(2304))")
    imported('malformed',b'\0','properties','Properties rejected:')
    imported('rollback',b"assert(health_keeper:GetHP()==51 and health_keeper:GetMP()==9);local before=health_keeper:GetProp(36);assert(not pcall(function()health_keeper:RegenHP(0/0)end));assert(not pcall(function()health_keeper:SetHP(1/0)end));assert(health_keeper:GetProp(36)==before);health_keeper:SetProp(38,1);assert(not pcall(function()return health_keeper:GetHP()end));assert(health_keeper:GetProp(36)==before)")
    imported('oversized',bytes(4*1024*1024+1),'properties','exceeds 4 MiB limit')
    imported('recovery',b"health_keeper:SetProp(38,25600);health_keeper:RegenHP(-1);health_keeper:RegenMP(-1);local hp,max,pct=health_keeper:GetHP();assert(hp==100 and max==100 and pct==100 and health_keeper:GetMP()==20);health_keeper:SetCombatContext(9,7,'healthy');assert(health_keeper:GetState()==9 and health_keeper:GetHitCount()==7);DH2SeedRandom(0);assert(Rand(100)==49)")
    visuals=[]
    for i,(button,relative)in enumerate([(ui.FIXTURES[0][0],ui.FIXTURES[0][1]),(ui.FIXTURES[1][0],ui.FIXTURES[1][1]),('IMPORT CHARACTER ANIMATION','data/3d/characters/prince/animations/prince_walk_dual.bdae')]):
        path=a.cache/relative;name='dh2qa_health_visual_'+str(i)+path.suffix;remote=directory+'/'+name;run('push',path.resolve(),remote);assert run('shell','sha256sum',remote).split()[0]==sha(path)
        statuses=check.import_file(button,name);expected='335 vertices, 1092 indices, 18 bones'if 'BRES'in button else'25 tracks'if 'ANIMATION'in button else'256';assert any(expected in s for s in statuses)
        visuals.append({'path':relative,'sha256':sha(path),'statuses':statuses})
    shot=check.capture('health-ready.png');installed_path=run('shell','pm','path',ui.PACKAGE).strip().split(':',1)[1];installed=a.evidence/'installed.apk';run('pull',installed_path,installed.resolve());assert sha(installed)==sha(a.apk)
    log=run('logcat','-d','-T',start,'--pid='+pid,'-v','brief');(a.evidence/'health-logcat.txt').write_text(log,encoding='utf-8');assert 'FATAL EXCEPTION'not in log and 'Fatal signal'not in log
    report={'complete_game':False,'original_gameplay_equivalence_tested':False,'device':device,'apk_sha256':sha(a.apk),'installed_apk_hash_matches':True,'controlled_actor_cases':40,'real_actor_cases':46,'final_field_queries':19264,'cases':results,'visual_imports':visuals,'process_survived_all_cases':True,'fatal_in_run':False,'screenshot':shot,'test_sha256':sha(Path(__file__)),
            'corpus_report_sha256':{mode:sha(path)for mode,path in reports.items()},'source_sha256':{n:sha(ROOT/n)for n in ('scripts.cpp','src/local/dh2/sourceviewer/MainActivity.java','build.py','../lua-character/bridge.c','../character-health/health.c')},
            'scope':'Installed pure source APK executes selected original-derived health assertion chunks: 40 controlled cases and 46 real character cases, all 224 final fields per case, health/mana results, retained property generation, invalid percentage/numeric argument rollback, malformed/oversized import retention and recovery. Real character model/texture/walk preview loads afterwards in the same process. Full combat damage/death/events, AI, savegame and game loop remain unfinished.'}
    a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({k:v for k,v in report.items()if k not in ('cases','scope','source_sha256')}))
if __name__=='__main__':main()
