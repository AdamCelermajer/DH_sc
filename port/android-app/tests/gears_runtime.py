#!/usr/bin/env python3
"""Exact-package owned power imports and gear lifecycle on Android 17."""
import argparse,hashlib,importlib.util,json,struct,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
spec=importlib.util.spec_from_file_location('viewer_ui',ROOT/'tests/animation_runtime.py')
ui=importlib.util.module_from_spec(spec);spec.loader.exec_module(ui)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def fixtures(replacement=False):
    props=bytearray(2700);struct.pack_into('<I',props,0,3);struct.pack_into('<224i',props,900,*([4]*224));struct.pack_into('<i',props,1796+79*4,7);struct.pack_into('<i',props,1796+80*4,8)
    items=bytearray(479);struct.pack_into('<I',items,12,3)
    for i,(type,a,b) in enumerate([(0,10,20),(6,5,2),(7,3,0)]):
        tail=85+i*149
        for off,val in [(4,type),(56,a),(60,b)]:struct.pack_into('<i',items,tail+off,val)
    powers=bytearray(90);struct.pack_into('<I',powers,4,2)
    for i,(op,value) in enumerate([(28,12 if replacement else 4),(9,256)]):struct.pack_into('<I3i',powers,8+i*41+5,1,op,value,0)
    return bytes(props),struct.pack('<II',1,0),bytes(items),bytes(powers)
def main():
    p=argparse.ArgumentParser()
    for n in ('adb','apk','ui-helper','cache','assertions','evidence','report'):p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--serial',required=True);a=p.parse_args();a.animation='';a.blend_animation=None
    check=ui.Check(a);run=check.run
    device={'serial':a.serial,'android_release':run('shell','getprop','ro.build.version.release').strip(),'sdk':int(run('shell','getprop','ro.build.version.sdk')),'page_size':int(run('shell','getconf','PAGE_SIZE')),'abi':run('shell','getprop','ro.product.cpu.abi').strip()}
    assert run('shell','getprop','ro.kernel.qemu').strip()=='1' and device['android_release']=='17' and device['sdk']==37 and device['abi']=='x86_64' and device['page_size'] in (4096,16384)
    assert sha(a.apk)==json.loads((ROOT/'build-validation.json').read_text())['apk']['sha256']
    corpus_path=REPO/'port/lua-runtime/gears-host-execution-validation.json';corpus=json.loads(corpus_path.read_text());assert corpus['cases']==4518 and corpus['final_queries']==1012032
    raw_paths=[a.cache/('data/pydata/'+n+'_pyarray.bin') for n in ('character_properties','character_classes','loot_table','item_powers')]
    assert dict(zip(('properties','classes','loot','powers'),map(sha,raw_paths)))==corpus['cache_sha256']
    run('install','-r',a.ui_helper.resolve());run('install','-r',a.apk.resolve());start=run('shell','date','+%m-%d_%H:%M:%S.000').strip().replace('_',' ')
    run('shell','am','force-stop',ui.PACKAGE);run('shell','am','start','-n',ui.PACKAGE+'/.MainActivity');time.sleep(1)
    pid=run('shell','pidof',ui.PACKAGE).strip();assert pid.isdigit();results=[];directory='/sdcard/Download/dh2-source-gears-qa';run('shell','mkdir','-p',directory)
    def imported(name,source,kind='script',expected=None):
        path=a.evidence/('dh2qa_gears_'+name+('.lua' if kind=='script' else '.bin'));path.write_bytes(source);remote=directory+'/'+path.name
        run('push',path.resolve(),remote);assert run('shell','sha256sum',remote).split()[0]==sha(path)
        buttons={'script':'IMPORT SCRIPT SOURCE','properties':'IMPORT CHARACTER PROPERTIES','classes':'IMPORT CHARACTER CLASSES','items':'IMPORT ITEM DATA','powers':'IMPORT ITEM POWERS'}
        labels={'script':'Script','properties':'Properties','classes':'Classes','items':'Items','powers':'Powers'}
        check.import_file(buttons[kind],path.name);text=check.find(**{'content-desc':'Script status'}).get('text');assert (expected or labels[kind]+' loaded.') in text,(name,text)
        assert run('shell','pidof',ui.PACKAGE).strip()==pid
        results.append({'case':name,'kind':kind,'bytes':len(source),'sha256':sha(path),'status':text})
    for kind,path in zip(('properties','classes','items'),raw_paths[:3]):imported('real_'+kind,path.read_bytes(),kind)
    imported('before_power',b"before=DH2CreatePropertyState(2);assert(pcall(function()before:SetItemPowers(0,1,0)end)==false);assert(pcall(function()before:UpdateGearsProperties()end)==false)")
    imported('real_powers',raw_paths[3].read_bytes(),'powers')
    real_cases=0
    for index in (0,83,141):
        entry=corpus['assertions'][index];path=a.assertions/entry['name'];assert sha(path)==entry['sha256'];imported('real_queries_'+str(index),path.read_bytes());real_cases+=min(32,4518-index*32)
    for kind,raw in zip(('properties','classes','items','powers'),fixtures()):imported('fixture_'+kind,raw,kind)
    imported('lifecycle',b"old=DH2CreatePropertyState(2);old:EquipItem(0,1,0);old:EquipGear(0,2,1);old:EquipGear(0,8,2);old:SetItemPowers(0,1,0,1);old:UpdateGearsProperties();assert(old:GetProp(79,false)==21 and old:GetProp(80,false)==32 and old:GetProp(97,false)==256);assert(old:GetProp(71,false)==8 and old:GetProp(61,false)==2 and old:HasShield());old:UpdateGearsProperties();old:RecalculateProperties(false);assert(old:GetProp(79,false)==21);assert(pcall(function()old:SetItemPowers(0,1,0,2)end)==false);assert(pcall(function()old:SetItemPowers(0,1,0/0)end)==false);assert(pcall(function()old:EquipGear(0,16,0)end)==false);assert(pcall(function()old:UpdateBaseProperties(0/0)end)==false);old:UpdateGearsProperties();assert(old:GetProp(79,false)==21);old:UpdateBaseProperties(-1);assert(old:GetProp(79,false)==14);old:UpdateBaseProperties(2);old:EquipGear(1,1,0);old:SelectEquipmentSet(1);old:UpdateGearsProperties();assert(old:GetProp(79,false)==17 and old:GetProp(71,false)==3);old:SelectEquipmentSet(0);old:UpdateGearsProperties();assert(old:GetProp(79,false)==21)")
    imported('malformed',b'\0','powers','Powers rejected:')
    imported('retained',b"old:UpdateGearsProperties();assert(old:GetProp(79,false)==21 and pcall(function()before:UpdateGearsProperties()end)==false)")
    imported('replacement',fixtures(True)[3],'powers')
    imported('generations',b"fresh=DH2CreatePropertyState(2);fresh:EquipGear(0,1,0);fresh:SetItemPowers(0,1,0);fresh:UpdateGearsProperties();assert(fresh:GetProp(79,false)==29);collectgarbage('collect');old:UpdateGearsProperties();assert(old:GetProp(79,false)==21);fresh:EquipItem(0,1,0);fresh:UpdateGearsProperties();assert(fresh:GetProp(79,false)==17)")
    imported('oversize',b'\0'*(4*1024*1024+1),'powers','exceeds 4 MiB limit')
    imported('recovery',fixtures()[3],'powers')
    imported('recovered',b"local c=DH2CreatePropertyState(2);c:EquipItem(0,1,0);c:SetItemPowers(0,1,0);c:UpdateGearsProperties();assert(c:GetProp(79,false)==21);old:UpdateGearsProperties();assert(old:GetProp(79,false)==21)")
    visuals=[]
    for i,(button,relative) in enumerate([(ui.FIXTURES[0][0],ui.FIXTURES[0][1]),(ui.FIXTURES[1][0],ui.FIXTURES[1][1]),('IMPORT CHARACTER ANIMATION','data/3d/characters/prince/animations/prince_walk_dual.bdae')]):
        path=a.cache/relative;name='dh2qa_gears_visual_'+str(i)+path.suffix;remote=directory+'/'+name;run('push',path.resolve(),remote);assert run('shell','sha256sum',remote).split()[0]==sha(path)
        statuses=check.import_file(button,name);expected='335 vertices, 1092 indices, 18 bones' if 'BRES' in button else '25 tracks' if 'ANIMATION' in button else '256'
        assert any(expected in s for s in statuses);visuals.append({'path':relative,'sha256':sha(path),'statuses':statuses})
    shot=check.capture('gears-ready.png');installed_path=run('shell','pm','path',ui.PACKAGE).strip().split(':',1)[1];installed=a.evidence/'installed.apk';run('pull',installed_path,installed.resolve());assert sha(installed)==sha(a.apk)
    log=run('logcat','-d','-T',start,'--pid='+pid,'-v','brief');(a.evidence/'gears-logcat.txt').write_text(log,encoding='utf-8');assert 'FATAL EXCEPTION' not in log and 'Fatal signal' not in log
    result={'complete_game':False,'scope':'Exact APK imports real four datasets and checks selected real gear/lifecycle queries, retained generations, typed/index rejection, malformed/oversize recovery and source character render. Authored snapshot controls have empty buffs and do not implement inventory ownership/equip requirements or gameplay.', 'device':device,'apk_sha256':sha(a.apk),'installed_apk_hash_matches':True,'corpus_report_sha256':sha(corpus_path),'real_cases':real_cases,'real_final_queries':real_cases*224,'cases':results,'visual_imports':visuals,'process_survived_all_cases':True,'fatal_in_run':False,'screenshot':shot,'log_start_emulator_gmt':start,'test_sha256':sha(Path(__file__)),'source_sha256':{n:sha(ROOT/n) for n in ('scripts.cpp','src/local/dh2/sourceviewer/MainActivity.java','build.py')}}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items() if k!='cases'}))
if __name__=='__main__':main()
