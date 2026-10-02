#!/usr/bin/env python3
"""Installed source APK: melee, HP, owned death state and quest requests."""
import argparse,hashlib,importlib.util,json,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
spec=importlib.util.spec_from_file_location('damage',ROOT/'tests/damage_runtime.py')
damage=importlib.util.module_from_spec(spec);spec.loader.exec_module(damage);ui=damage.ui
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
EVENTS="""
assert(GetPyCst('v2QuestObjectiveType','KillXEnemies')==0)
assert(GetPyCst('v2QuestObjectiveType','ClearEnemies')==1)
assert(GetPyCst('v2QuestObjectiveType','KillEnemyTemplate')==10)
assert(GetPyCst('v2QuestObjectiveType','ClearEnemyTemplate')==11)
death_ctx={dead=false,network=false,suppress_events=false,target_id=734,property_id=5,template_id=7}
death_policy={forced=false,loot_manager_present=false,
 kill_enemies=GetPyCst('v2QuestObjectiveType','KillXEnemies'),clear_enemies=GetPyCst('v2QuestObjectiveType','ClearEnemies'),
 kill_template=GetPyCst('v2QuestObjectiveType','KillEnemyTemplate'),clear_template=GetPyCst('v2QuestObjectiveType','ClearEnemyTemplate')}
damage_keeper:SetDeathContext(death_ctx);damage_keeper:SetProp(9,123);assert(not damage_keeper:IsDead())
local p,d,w,r=damage_keeper:ApplyNonplayerHit(2560,hit_policy,17);assert(p and d and w==10 and r==3 and damage_keeper:GetHP()==0)
local result=damage_keeper:KillNonplayer(death_policy)
assert(result.processed and result.dead and result.drop_loot_requested and result.drop_loot_id==123 and #result.events==4)
for i,e in ipairs(result.events)do assert(e.kind==i-1 and e.target_id==734 and e.match_id==(i<3 and 5 or 7))end
assert(result.events[1].objective_id==0 and result.events[2].objective_id==1 and result.events[3].objective_id==10 and result.events[4].objective_id==11)
assert(damage_keeper:IsDead());result=damage_keeper:KillNonplayer(death_policy)
assert(not result.processed and result.dead and not result.drop_loot_requested and #result.events==0)
"""
METADATA="""
damage_keeper:SetDeathContext(death_ctx);damage_keeper:SetHP(10);local before=damage_keeper:GetProp(36)
death_ctx.property_id=32768;assert(not pcall(function()damage_keeper:SetDeathContext(death_ctx)end))
assert(not damage_keeper:IsDead() and damage_keeper:GetProp(36)==before);death_ctx.property_id=5
death_ctx.target_id=-1;assert(not pcall(function()damage_keeper:SetDeathContext(death_ctx)end))
assert(not damage_keeper:IsDead() and damage_keeper:GetProp(36)==before);death_ctx.target_id=734
death_policy.kill_enemies=0/0;assert(not pcall(function()damage_keeper:KillNonplayer(death_policy)end))
assert(not damage_keeper:IsDead() and damage_keeper:GetProp(36)==before);death_policy.kill_enemies=0
death_policy.forced=0;assert(not pcall(function()damage_keeper:KillNonplayer(death_policy)end))
assert(not damage_keeper:IsDead() and damage_keeper:GetProp(36)==before);death_policy.forced=false
death_ctx.template_id=-1;damage_keeper:SetDeathContext(death_ctx);death_policy.loot_manager_present=true
local r=damage_keeper:KillNonplayer(death_policy);assert(r.processed and r.dead and not r.drop_loot_requested and #r.events==2)
death_ctx.template_id=7;death_ctx.network=true;damage_keeper:SetDeathContext(death_ctx)
r=damage_keeper:KillNonplayer(death_policy);assert(r.processed and #r.events==0)
death_ctx.network=false;death_ctx.suppress_events=true;damage_keeper:SetDeathContext(death_ctx)
r=damage_keeper:KillNonplayer(death_policy);assert(r.processed and #r.events==0)
death_ctx.suppress_events=false;damage_keeper:SetDeathContext(death_ctx);death_policy.forced=true;death_policy.loot_manager_present=false
r=damage_keeper:KillNonplayer(death_policy);assert(r.processed and r.drop_loot_requested and #r.events==0)
death_policy.forced=false
"""
RETAINED="""
collectgarbage('collect');assert(damage_keeper:IsDead());local r=damage_keeper:KillNonplayer(death_policy)
assert(not r.processed and r.dead and not r.drop_loot_requested and #r.events==0)
damage_keeper:SetDeathContext(death_ctx);assert(not damage_keeper:IsDead());damage_keeper:SetHP(10)
hit_policy.target_dead=damage_keeper:IsDead();local p,d,w,reason=damage_keeper:ApplyNonplayerHit(2560,hit_policy,17)
assert(p and d and w==10 and reason==3 and damage_keeper:GetHP()==0)
r=damage_keeper:KillNonplayer(death_policy);assert(r.processed and damage_keeper:IsDead() and r.drop_loot_id==123 and #r.events==4)
hit_policy.target_dead=damage_keeper:IsDead();p,d,w,reason=damage_keeper:ApplyNonplayerHit(256,hit_policy,3)
assert(not p and not d and w==0 and reason==3 and damage_keeper:GetHP()==0)
"""
def main():
    p=argparse.ArgumentParser()
    for n in ('adb','apk','ui-helper','cache','evidence','report'):p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--serial',required=True);a=p.parse_args();a.animation='';a.blend_animation=None
    check=ui.Check(a);run=check.run
    device={'serial':a.serial,'release':run('shell','getprop','ro.build.version.release').strip(),'sdk':int(run('shell','getprop','ro.build.version.sdk')),'page_size':int(run('shell','getconf','PAGE_SIZE')),'abi':run('shell','getprop','ro.product.cpu.abi').strip()}
    assert run('shell','getprop','ro.kernel.qemu').strip()=='1'and device['release']=='17'and device['sdk']==37 and device['abi']=='x86_64'and device['page_size']in (4096,16384)
    assert sha(a.apk)==json.loads((ROOT/'build-validation.json').read_text(encoding='utf-8'))['apk']['sha256']
    with zipfile.ZipFile(a.apk)as z:assert hashlib.sha256(z.read('assets/dh2/scripts/combat-formulas.lua')).hexdigest()=='f83639a12c5c1910f9a23fff494f4013330943ad4ef8f4d91c33fe1eb975565f'
    run('install','-r',a.ui_helper.resolve());run('install','-r',a.apk.resolve());start=run('shell','date','+%m-%d_%H:%M:%S.000').strip().replace('_',' ')
    run('shell','am','force-stop',ui.PACKAGE);run('shell','am','start','-W','-n',ui.PACKAGE+'/.MainActivity')
    check.find(**{'content-desc':'Script status'});pid=run('shell','pidof',ui.PACKAGE).strip();assert pid
    directory='/sdcard/Download/dh2-source-death-qa';run('shell','mkdir','-p',directory);results=[]
    def imported(name,source,kind='script',expected=None):
        path=a.evidence/('dh2qa_death_'+name+('.lua'if kind=='script'else'.bin'));path.write_bytes(source);remote=directory+'/'+path.name
        run('push',path.resolve(),remote);assert run('shell','sha256sum',remote).split()[0]==sha(path)
        button={'script':'IMPORT SCRIPT SOURCE','properties':'IMPORT CHARACTER PROPERTIES','items':'IMPORT ITEM DATA','constants':'IMPORT SCRIPT CONSTANTS'}[kind]
        label={'script':'Script','properties':'Properties','items':'Items','constants':'Constants'}[kind]
        check.import_file(button,path.name);text=check.find(**{'content-desc':'Script status'}).get('text');assert(expected or label+' loaded.')in text,(name,text)
        assert run('shell','pidof',ui.PACKAGE).strip()==pid
        results.append({'name':name,'kind':kind,'bytes':len(source),'sha256':sha(path),'status':text})
    stage=REPO/'port/lua-runtime/build/combat-controlled-host'
    imported('properties',(stage/'properties.bin').read_bytes(),'properties');imported('items',(stage/'loot.bin').read_bytes(),'items')
    for name in ('ai','design','v2quests'):imported(name,(a.cache/f'data/pydata/{name}_pycst.bin').read_bytes(),'constants')
    for name,source in (('prepare',damage.PREPARE),('combat',damage.COMBAT),('policies',damage.POLICIES),('rollback',damage.ROLLBACK),('events',EVENTS),('metadata',METADATA)):imported(name,source.encode())
    imported('real_properties',(a.cache/'data/pydata/character_properties_pyarray.bin').read_bytes(),'properties')
    imported('malformed',b'\0','properties','Properties rejected:');imported('retained_recovery',RETAINED.encode())
    visuals=[]
    for i,(button,relative)in enumerate([(ui.FIXTURES[0][0],ui.FIXTURES[0][1]),(ui.FIXTURES[1][0],ui.FIXTURES[1][1]),('IMPORT CHARACTER ANIMATION','data/3d/characters/prince/animations/prince_walk_dual.bdae')]):
        path=a.cache/relative;name='dh2qa_death_visual_'+str(i)+path.suffix;remote=directory+'/'+name;run('push',path.resolve(),remote);assert run('shell','sha256sum',remote).split()[0]==sha(path)
        statuses=check.import_file(button,name);expected='335 vertices, 1092 indices, 18 bones'if 'BRES'in button else'25 tracks'if 'ANIMATION'in button else'256';assert any(expected in s for s in statuses)
        visuals.append({'path':relative,'sha256':sha(path),'statuses':statuses})
    shot=check.capture('death-ready.png');installed_path=run('shell','pm','path',ui.PACKAGE).strip().split(':',1)[1];installed=a.evidence/'installed.apk';run('pull',installed_path,installed.resolve());assert sha(installed)==sha(a.apk)
    log=run('logcat','-d','-T',start,'--pid='+pid,'-v','brief');(a.evidence/'death-logcat.txt').write_text(log,encoding='utf-8');assert 'FATAL EXCEPTION'not in log and 'Fatal signal'not in log
    report={'complete_game':False,'original_gameplay_equivalence_tested':False,'device':device,'apk_sha256':sha(a.apk),'installed_apk_hash_matches':True,
        'formula_to_health_lethal_sequence':True,'owned_dead_flag_and_repeated_kill_noop':True,'ordered_quest_requests_and_loot_id':True,
        'event_suppression_policies':True,'invalid_argument_rollback':True,'retained_generation_and_import_recovery':True,
        'cases':results,'visual_imports':visuals,'process_survived_all_cases':True,'fatal_in_run':False,'screenshot':shot,'test_sha256':sha(Path(__file__)),
        'source_sha256':{n:sha(ROOT/n)for n in ('scripts.cpp','src/local/dh2/sourceviewer/MainActivity.java','build.py','tests/damage_runtime.py','tests/animation_runtime.py','../lua-character/bridge.c','../character-damage/damage.c','../character-death/death.c')},
        'scope':'Installed pure source APK executes recovered melee formula, non-player HP damage and explicit owned KillNonplayer death transition. Real v2quest constant IDs feed four ordered semantic quest requests and loot request/id; repeated dead calls noop, two-event/suppressed/forced policy paths, argument rollback, retained death state/dataset after replacement/rejection and recovery pass. Existing model/texture/walk preview loads. Native original Character, resolved killer credit, actual loot creation/event consumers, animation/FSM/world loop, progression/saves and full source gameplay remain unfinished.'}
    a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({k:v for k,v in report.items()if k not in ('cases','scope','source_sha256')}))
if __name__=='__main__':main()
