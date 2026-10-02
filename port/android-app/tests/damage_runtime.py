#!/usr/bin/env python3
"""Source APK: recovered formula to non-player HP, requests and recovery."""
import argparse,hashlib,importlib.util,json,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
spec=importlib.util.spec_from_file_location('damage_ui',ROOT/'tests/animation_runtime.py')
ui=importlib.util.module_from_spec(spec);spec.loader.exec_module(ui)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
PREPARE="""
hit_policy={target_dead=false,target_monster=true,local_player_alive=true,online=false,
 manager_present=true,manager_mode=0,monster_invincible=false,force_kill_config=false,
 force_kill_switch=false,target_network=false}
damage_keeper=DH2CreatePropertyState(2);damage_keeper:SetProp(38,2560);damage_keeper:SetHP(10)
assert(damage_keeper:GetHP()==10)
"""
COMBAT="""
assert(GetPyCst('CombatAttackTypes','Melee')==0 and GetPyCst('AIStates','Stunned')==9)
local a=DH2CreatePropertyState(2);a:SetProp(79,1280);a:SetProp(80,1280);a:SetProp(97,-256)
a:SetCombatContext(0,0,'attacker');damage_keeper:SetCombatContext(0,0,'defender')
for i=1,2 do
 CF_ClearCombatants();CF_SetCombatants(a,damage_keeper,-1,false,false);DH2SeedRandom(0)
 local damage,element,hp,mp,duration,dot,doe=CF_CalcDamage(0,0)
 assert(damage==1280 and element==-1 and hp==0 and mp==0 and duration==0 and dot==0 and doe==-1)
 local processed,death,whole,reason=damage_keeper:ApplyNonplayerHit(damage,hit_policy,17)
 assert(processed and death==(i==2) and whole==5 and reason==(i==2 and 3 or 17))
 assert(damage_keeper:GetHP()==10-5*i)
end
CF_ClearCombatants();hit_policy.target_dead=true
local processed,death,whole,reason=damage_keeper:ApplyNonplayerHit(1280,hit_policy,3)
assert(not processed and not death and whole==0 and reason==3 and damage_keeper:GetHP()==0)
hit_policy.target_dead=false;damage_keeper:SetHP(10)
"""
POLICIES="""
hit_policy.local_player_alive=false;local p,d,w,r=damage_keeper:ApplyNonplayerHit(256,hit_policy,17)
assert(p and not d and w==0 and r==17 and damage_keeper:GetHP()==10)
hit_policy.local_player_alive=true;hit_policy.monster_invincible=true
p,d,w,r=damage_keeper:ApplyNonplayerHit(256,hit_policy,17);assert(p and not d and w==0 and damage_keeper:GetHP()==10)
hit_policy.monster_invincible=false;hit_policy.online=true;hit_policy.manager_mode=2
p,d,w,r=damage_keeper:ApplyNonplayerHit(256,hit_policy,17);assert(p and not d and w==0 and damage_keeper:GetHP()==10)
hit_policy.manager_mode=5;p,d,w,r=damage_keeper:ApplyNonplayerHit(256,hit_policy,17)
assert(p and not d and w==1 and r==17 and damage_keeper:GetHP()==9)
hit_policy.target_network=true;hit_policy.force_kill_switch=true
p,d,w,r=damage_keeper:ApplyNonplayerHit(0,hit_policy,17);assert(p and d and w==0 and r==17 and damage_keeper:GetHP()==0)
hit_policy.online=false;hit_policy.manager_mode=0;hit_policy.target_network=false;hit_policy.force_kill_switch=false
damage_keeper:SetHP(10)
"""
ROLLBACK="""
local before=damage_keeper:GetProp(36)
for _,v in ipairs({-1,4294967296,1/0,0/0,'10'})do
 assert(not pcall(function()damage_keeper:ApplyNonplayerHit(v,hit_policy,17)end));assert(damage_keeper:GetProp(36)==before)
end
hit_policy.manager_mode=1.5;assert(not pcall(function()damage_keeper:ApplyNonplayerHit(0,hit_policy,17)end))
assert(damage_keeper:GetProp(36)==before);hit_policy.manager_mode=0
assert(not pcall(function()damage_keeper:ApplyNonplayerHit(0,{},17)end));assert(damage_keeper:GetProp(36)==before)
hit_policy.target_dead=0;assert(not pcall(function()damage_keeper:ApplyNonplayerHit(0,hit_policy,17)end))
assert(damage_keeper:GetProp(36)==before);hit_policy.target_dead=false
collectgarbage('collect');local p,d,w,r=damage_keeper:ApplyNonplayerHit(256,hit_policy,17)
assert(p and not d and w==1 and r==17 and damage_keeper:GetHP()==9)
"""
RETAINED="""
assert(damage_keeper:GetHP()==9);damage_keeper:SetHP(1)
local p,d,w,r=damage_keeper:ApplyNonplayerHit(2147483648,hit_policy,17)
assert(p and d and w==-8388608 and r==3 and damage_keeper:GetHP()==0)
damage_keeper:RegenHP(-1);assert(damage_keeper:GetHP()==10)
p,d,w,r=damage_keeper:ApplyNonplayerHit(256,hit_policy,17);assert(p and not d and w==1 and r==17 and damage_keeper:GetHP()==9)
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
    directory='/sdcard/Download/dh2-source-damage-qa';run('shell','mkdir','-p',directory);results=[]
    def imported(name,source,kind='script',expected=None):
        path=a.evidence/('dh2qa_damage_'+name+('.lua'if kind=='script'else'.bin'));path.write_bytes(source);remote=directory+'/'+path.name
        run('push',path.resolve(),remote);assert run('shell','sha256sum',remote).split()[0]==sha(path)
        button={'script':'IMPORT SCRIPT SOURCE','properties':'IMPORT CHARACTER PROPERTIES','items':'IMPORT ITEM DATA','constants':'IMPORT SCRIPT CONSTANTS'}[kind]
        label={'script':'Script','properties':'Properties','items':'Items','constants':'Constants'}[kind]
        check.import_file(button,path.name);text=check.find(**{'content-desc':'Script status'}).get('text');assert(expected or label+' loaded.')in text,(name,text)
        assert run('shell','pidof',ui.PACKAGE).strip()==pid
        results.append({'name':name,'kind':kind,'bytes':len(source),'sha256':sha(path),'status':text})
    stage=REPO/'port/lua-runtime/build/combat-controlled-host'
    imported('properties',(stage/'properties.bin').read_bytes(),'properties');imported('items',(stage/'loot.bin').read_bytes(),'items')
    for name in ('ai','design'):imported(name,(a.cache/f'data/pydata/{name}_pycst.bin').read_bytes(),'constants')
    for name,source in (('prepare',PREPARE),('combat',COMBAT),('policies',POLICIES),('rollback',ROLLBACK)):imported(name,source.encode())
    imported('real_properties',(a.cache/'data/pydata/character_properties_pyarray.bin').read_bytes(),'properties')
    imported('malformed',b'\0','properties','Properties rejected:');imported('retained_recovery',RETAINED.encode())
    visuals=[]
    for i,(button,relative)in enumerate([(ui.FIXTURES[0][0],ui.FIXTURES[0][1]),(ui.FIXTURES[1][0],ui.FIXTURES[1][1]),('IMPORT CHARACTER ANIMATION','data/3d/characters/prince/animations/prince_walk_dual.bdae')]):
        path=a.cache/relative;name='dh2qa_damage_visual_'+str(i)+path.suffix;remote=directory+'/'+name;run('push',path.resolve(),remote);assert run('shell','sha256sum',remote).split()[0]==sha(path)
        statuses=check.import_file(button,name);expected='335 vertices, 1092 indices, 18 bones'if 'BRES'in button else'25 tracks'if 'ANIMATION'in button else'256';assert any(expected in s for s in statuses)
        visuals.append({'path':relative,'sha256':sha(path),'statuses':statuses})
    shot=check.capture('damage-ready.png');installed_path=run('shell','pm','path',ui.PACKAGE).strip().split(':',1)[1];installed=a.evidence/'installed.apk';run('pull',installed_path,installed.resolve());assert sha(installed)==sha(a.apk)
    log=run('logcat','-d','-T',start,'--pid='+pid,'-v','brief');(a.evidence/'damage-logcat.txt').write_text(log,encoding='utf-8');assert 'FATAL EXCEPTION'not in log and 'Fatal signal'not in log
    report={'complete_game':False,'original_gameplay_equivalence_tested':False,'device':device,'apk_sha256':sha(a.apk),'installed_apk_hash_matches':True,
        'formula_to_health_lethal_sequence':True,'explicit_dead_target_noop':True,'boundary_policies_and_death_reason':True,'invalid_argument_rollback':True,
        'retained_generation_and_import_recovery':True,'cases':results,'visual_imports':visuals,'process_survived_all_cases':True,'fatal_in_run':False,'screenshot':shot,'test_sha256':sha(Path(__file__)),
        'source_sha256':{n:sha(ROOT/n)for n in ('scripts.cpp','src/local/dh2/sourceviewer/MainActivity.java','build.py','../lua-character/bridge.c','../character-damage/damage.c')},
        'scope':'Installed pure source APK feeds exact recovered melee formula damage into the original-matched non-player HitFor projection on owned diagnostic actors. Authored integration assertions verify two 5-HP hits kill a 10-HP fixture, explicit dead target noop, online/local/debug policies, network death-reason retention, numeric argument rollback and retained generations after property replacement/rejection. Source model/texture/walk preview loads in the same process. No original Character, native full result dispatch, real death owner, world, enemy loop, progression or saves is claimed.'}
    a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({k:v for k,v in report.items()if k not in ('cases','scope','source_sha256')}))
if __name__=='__main__':main()
