#!/usr/bin/env python3
"""Owned item imports/equipment snapshots in the exact source APK."""
import argparse,hashlib,importlib.util,json,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
spec=importlib.util.spec_from_file_location('viewer_ui',ROOT/'tests/animation_runtime.py')
ui=importlib.util.module_from_spec(spec);spec.loader.exec_module(ui)
spec=importlib.util.spec_from_file_location('equipment_corpus',REPO/'port/lua-runtime/tests/equipment_corpus.py')
fixtures=importlib.util.module_from_spec(spec);spec.loader.exec_module(fixtures)
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
    corpus_path=REPO/'port/lua-runtime/equipment-host-execution-validation.json';corpus=json.loads(corpus_path.read_text());assert corpus['actual_items']==1322
    items=a.cache/'data/pydata/loot_table_pyarray.bin';assert sha(items)==corpus['item_cache_sha256']
    props,values=fixtures.property_fixture();assert hashlib.sha256(props).hexdigest()==corpus['property_fixture_sha256']
    run('install','-r',a.ui_helper.resolve());run('install','-r',a.apk.resolve())
    start=run('shell','date','+%m-%d_%H:%M:%S.000').strip().replace('_',' ')
    run('shell','am','force-stop',ui.PACKAGE);run('shell','am','start','-n',ui.PACKAGE+'/.MainActivity');time.sleep(1)
    pid=run('shell','pidof',ui.PACKAGE).strip();assert pid.isdigit();results=[]
    directory='/sdcard/Download/dh2-source-equipment-qa';run('shell','mkdir','-p',directory)
    def imported(name,source,kind='script',expected=None):
        path=a.evidence/('dh2qa_equipment_'+name+('.lua'if kind=='script'else '.bin'));path.write_bytes(source)
        remote=directory+'/'+path.name;run('push',path.resolve(),remote);assert run('shell','sha256sum',remote).split()[0]==sha(path)
        buttons={'script':'IMPORT SCRIPT SOURCE','properties':'IMPORT CHARACTER PROPERTIES','items':'IMPORT ITEM DATA'}
        check.import_file(buttons[kind],path.name);text=check.find(**{'content-desc':'Script status'}).get('text')
        expected=expected or {'script':'Script loaded.','properties':'Properties loaded.','items':'Items loaded.'}[kind]
        assert expected in text,(name,text);assert run('shell','pidof',ui.PACKAGE).strip()==pid
        results.append({'case':name,'kind':kind,'bytes':len(source),'sha256':sha(path),'status':text})
    imported('properties',props,'properties')
    imported('before',b"before=DH2CreatePropertyState(2);assert(before:HasShield()==false);assert(pcall(function()before:EquipItem(0,1,0)end)==false)")
    imported('real_items',items.read_bytes(),'items')
    real_configurations=0
    for index in (0,20):
        entry=corpus['assertions'][index];path=a.assertions/entry['name'];assert sha(path)==entry['sha256'];imported('real_queries_'+str(index),path.read_bytes())
        real_configurations+=min(64,corpus['actual_items']-64*index)
    imported('generation_a',fixtures.item_fixture(),'items')
    source=f'''old=DH2CreatePropertyState(2);old:EquipItem(0,1,0);old:EquipItem(0,2,1)
assert(old:HasShield()==true and old:HasShield(999)==true)
assert(old:GetAttackRatingBonus(false)=={values[51]} and old:GetAttackRatingBonus(true)=={values[52]})
assert(old:GetAttackRatingBonus(0)=={values[51]} and old:GetAttackRatingBonus(-1)=={values[52]})
assert(old:GetAttackRatingBonus(nil)=={values[51]} and old:GetAttackRatingBonus(false,'ignored')=={values[51]})
assert(old:GetAttackRatingBonus(0/0)=={values[52]} and select('#',old:GetDamageBonus())==0)
assert(pcall(function()old:GetDamageBonus('false')end)==false)
old:EquipItem(1,1,2);old:EquipItem(1,2,0);old:SelectEquipmentSet(1)
assert(old:HasShield()==false and old:GetDamageBonus(false)=={values[85]+values[91]+values[90]})
assert(pcall(function()old:EquipItem(0,1,3)end)==false and pcall(function()old:EquipItem(0,1,0/0)end)==false)
assert(pcall(function()old:EquipItem(0,3,0)end)==false and pcall(function()old:EquipItem('0',1,0)end)==false)
assert(pcall(function()old:SelectEquipmentSet(2)end)==false and pcall(function()old:SelectEquipmentSet(0/0)end)==false)
assert(pcall(function()before:EquipItem(0,1,0)end)==false)
assert(old:GetDamageBonus(false)=={values[85]+values[91]+values[90]})
old:EquipItem(1,2,-1);assert(old:GetDamageBonus(false)=={values[85]+values[91]} and old:GetDamageBonus(true)==0)
'''
    imported('typed_methods',source.encode('ascii'))
    imported('malformed',b'\0','items','Items rejected:')
    imported('retained',f"old:EquipItem(1,2,0);assert(old:GetDamageBonus(false)=={values[85]+values[91]+values[90]})".encode())
    imported('generation_b',fixtures.item_fixture(True),'items')
    imported('replacement',f"new=DH2CreatePropertyState(2);new:EquipItem(1,1,2);new:EquipItem(1,2,0);new:SelectEquipmentSet(1);assert(new:HasShield()==true and new:GetDamageBonus(false)=={values[85]+values[91]});old:EquipItem(1,2,0);collectgarbage('collect');assert(old:HasShield()==false and old:GetDamageBonus(false)=={values[85]+values[91]+values[90]})".encode())
    imported('oversize',b'\0'*(4*1024*1024+1),'items','exceeds 4 MiB limit')
    imported('recovery',fixtures.item_fixture(),'items')
    imported('recovered',b"local c=DH2CreatePropertyState(2);c:EquipItem(0,2,1);assert(c:HasShield()==true and new:HasShield()==true)")
    imported('final_real_items',items.read_bytes(),'items')
    # Show the retained source rendering path together with the new import state.
    visual=[(ui.FIXTURES[0][0],ui.FIXTURES[0][1]),(ui.FIXTURES[1][0],ui.FIXTURES[1][1]),
            ('IMPORT CHARACTER ANIMATION','data/3d/characters/prince/animations/prince_walk_dual.bdae')]
    visual_results=[]
    for i,(button,relative) in enumerate(visual):
        path=a.cache/relative;assert path.is_file(),relative
        name='dh2qa_equipment_visual_'+str(i)+path.suffix;remote=directory+'/'+name
        run('push',path.resolve(),remote);assert run('shell','sha256sum',remote).split()[0]==sha(path)
        statuses=check.import_file(button,name)
        expected='335 vertices, 1092 indices, 18 bones'if 'BRES'in button else '25 tracks'if 'ANIMATION'in button else '256'
        assert any(expected in s for s in statuses),statuses
        visual_results.append({'path':relative,'sha256':sha(path),'statuses':statuses})
    shot=check.capture('equipment-ready.png')
    path=run('shell','pm','path',ui.PACKAGE).strip().split(':',1)[1];installed=a.evidence/'installed.apk';run('pull',path,installed.resolve());assert sha(installed)==sha(a.apk)
    log=run('logcat','-d','-T',start,'--pid='+pid,'-v','brief');(a.evidence/'equipment-logcat.txt').write_text(log,encoding='utf-8');assert 'FATAL EXCEPTION'not in log and 'Fatal signal'not in log
    result={'complete_game':False,'scope':f'Exact APK: real eight-table item imports, {real_configurations} actual item configurations in both sets and {real_configurations*14} bonus/shield assertions against checked host corpus; owned generations, typed methods, malformed/oversize rejection and recovery. Authored controls do not implement inventory lifecycle, restrictions or gear contributions. Source character render import is also exercised.',
            'device':device,'apk_sha256':sha(a.apk),'installed_apk_hash_matches':True,'corpus_report_sha256':sha(corpus_path),
            'real_item_configurations':real_configurations,'real_bonus_and_shield_queries':real_configurations*14,'cases':results,'visual_imports':visual_results,'process_survived_all_cases':True,'fatal_in_run':False,'screenshot':shot,
            'log_start_emulator_gmt':start,'test_sha256':sha(Path(__file__)),
            'source_sha256':{n:sha(ROOT/n)for n in ('scripts.cpp','src/local/dh2/sourceviewer/MainActivity.java','build.py')}}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items()if k!='cases'}))
if __name__=='__main__':main()
