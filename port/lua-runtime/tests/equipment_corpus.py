#!/usr/bin/env python3
"""All cache items through owned Lua snapshots against checked native projection."""
import argparse,ctypes as c,hashlib,json,struct,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
class Tables(c.Structure):_fields_=[('bytes',c.c_void_p),('size',c.c_uint32),('counts',c.c_uint32*8),('offsets',c.c_uint32*8)]
class Slot(c.Structure):_fields_=[('present',c.c_uint32),('item_id',c.c_int32),('type',c.c_int32),('slotting',c.c_int32),('weapon_kind',c.c_int32)]
class Equipment(c.Structure):_fields_=[('slots',(Slot*3)*2),('current_set',c.c_uint32),('owner_hand_rule',c.c_uint32)]
class Sheet(c.Structure):_fields_=[('values',c.c_int32*224)]
def property_fixture():
    raw=bytearray(2700);struct.pack_into('<I',raw,0,3)
    values=[(i*7919)%32768-16384 for i in range(224)]
    struct.pack_into('<224i',raw,900,*([16]*224));struct.pack_into('<224i',raw,1796,*values)
    return bytes(raw),values
def item_fixture(shield=False):
    raw=bytearray(479);struct.pack_into('<I',raw,12,3)
    for i in range(3):
        tail=16+i*149+69
        struct.pack_into('<i',raw,tail+4,6 if i==1 or (i==0 and shield) else 4 if i==2 else 0)
        struct.pack_into('<i',raw,tail+20,-4 if i==2 else 2 if i==1 else 1)
        struct.pack_into('<i',raw,tail+64,i)
    return bytes(raw)
def main():
    p=argparse.ArgumentParser()
    for n in ('runner','cache','reference','report'):p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--adb',type=Path);p.add_argument('--serial');a=p.parse_args()
    if bool(a.adb)!=bool(a.serial):p.error('--adb and --serial must be paired')
    comparison_path=REPO/'port/equipment-bonuses/differential-validation.json';comparison=json.loads(comparison_path.read_text())
    assert sha(a.reference)==comparison['host_sha256'] and comparison['comparisons']==24670 and comparison['mismatches']==0
    items=a.cache/'data/pydata/loot_table_pyarray.bin';assert sha(items)==comparison['cache_sha256']
    lib=c.CDLL(str(a.reference.resolve()));v=Tables();e=Equipment();sheet=Sheet();props,values=property_fixture();sheet.values[:]=values
    lib.dh2_loot_open.argtypes=[c.POINTER(Tables),c.c_void_p,c.c_uint32]
    lib.dh2_equipment_load.argtypes=[c.POINTER(Tables),c.POINTER(c.c_int32),c.c_uint32,c.c_uint32,c.POINTER(Equipment)]
    lib.dh2_equipment_flag.argtypes=[c.POINTER(Equipment),c.c_uint32,c.POINTER(c.c_int32)]
    lib.dh2_equipment_bonus.argtypes=[c.POINTER(Equipment),c.POINTER(Sheet),c.c_uint32,c.c_uint32,c.POINTER(c.c_int32)]
    buf=c.create_string_buffer(items.read_bytes());assert not lib.dh2_loot_open(c.byref(v),buf,len(items.read_bytes()));assert v.counts[3]==1322
    chunks=[];lines=[];queries=0
    for id in range(1322):
        ids=[id,(id+17)%1322,-1 if id%3==0 else (id+1)%1322,-1,id,-1 if id%5==0 else (id+31)%1322]
        lines.append('do local c=DH2CreatePropertyState(2)')
        for i,row in enumerate(ids):lines.append(f'c:EquipItem({i//3},{i%3},{row})')
        array=(c.c_int32*6)(*ids)
        for set in range(2):
            assert not lib.dh2_equipment_load(c.byref(v),array,set,values[203]&0xffffffff,c.byref(e))
            lines.append(f'c:SelectEquipmentSet({set})');value=c.c_int32();assert not lib.dh2_equipment_flag(c.byref(e),0,c.byref(value))
            lines.append('assert(c:HasShield()=='+str(bool(value.value)).lower()+')');queries+=1
            for op,method in enumerate(('GetCritRatingBonus','GetAttackRatingBonus','GetDamageBonus')):
                for off in range(2):
                    assert not lib.dh2_equipment_bonus(c.byref(e),c.byref(sheet),op,off,c.byref(value))
                    lines.append(f'assert(c:{method}({str(bool(off)).lower()})=={value.value})');queries+=1
        lines.append('end')
        if id%64==63 or id==1321:chunks.append('\n'.join(lines)+'\n');lines=[]
    assert len(chunks)==21 and queries==18508
    staging=ROOT/('build/equipment-corpus-'+(a.serial or 'host'));staging.mkdir(parents=True,exist_ok=True)
    base='/data/local/tmp/dh2-lua-equipment-qa';files=[];paths=[]
    for name,data in [('properties.bin',props),('items.bin',items.read_bytes())]:target=staging/name;target.write_bytes(data);files.append(target)
    for index,source in enumerate(chunks):
        target=staging/f'{index:03}.lua';target.write_text(source,encoding='ascii',newline='\n');assert target.stat().st_size<1024*1024
        files.append(target);paths.append(base+'/'+target.name if a.adb else str(target.resolve()))
    listing=staging/'list.txt';listing.write_text('\n'.join(paths)+'\n',encoding='ascii',newline='\n');files.append(listing)
    def run(*args):return subprocess.run(list(map(str,args)),capture_output=True,text=True,check=True).stdout.strip()
    hashes={};device=None
    if a.adb:
        def adb(*args):
            argv=list(args)
            if a.adb.suffix.lower()=='.exe' and argv[0]=='push':
                contents=str(argv[1]).endswith('/.');argv[1]=run('wslpath','-w',Path(argv[1]).resolve())+('/.'if contents else '')
            return run(a.adb,'-s',a.serial,*argv)
        assert adb('shell','getprop','ro.kernel.qemu')=='1'
        device={'serial':a.serial,'android_release':adb('shell','getprop','ro.build.version.release'),'sdk':int(adb('shell','getprop','ro.build.version.sdk')),
                'page_size':int(adb('shell','getconf','PAGE_SIZE')),'abi':adb('shell','getprop','ro.product.cpu.abi'),'fingerprint':adb('shell','getprop','ro.build.fingerprint')}
        assert device['android_release']=='17' and device['sdk']==37 and device['abi']=='x86_64' and device['page_size'] in (4096,16384)
        adb('shell','mkdir','-p',base);adb('push',str(staging)+'/.' ,base+'/');adb('push',a.runner,base+'/runner');adb('shell','chmod','755',base+'/runner')
        locals=[*files,a.runner];remotes=[base+'/'+f.name for f in files]+[base+'/runner']
        for line in adb('shell','sha256sum',*remotes).splitlines():digest,path=line.split(None,1);hashes[path.strip()]=digest
        for remote,local in zip(remotes,locals):assert hashes[remote]==sha(local)
        output=adb('shell',base+'/runner','--equipment',base+'/properties.bin',base+'/items.bin',base+'/list.txt')
    else:output=run(a.runner.resolve(),'--equipment',files[0].resolve(),files[1].resolve(),listing.resolve())
    assert output.splitlines()[:2]==['SELFTEST PASS','NUMERIC PASS 504'] and output.splitlines()[-1]=='EQUIPMENT CORPUS PASS 21'
    result={'complete_game':False,'original_lua_gameplay_equivalence_tested':False,'equipment_binding_tested':True,
            'actual_items':1322,'equipment_set_queries':2644,'bonus_and_shield_queries':queries,'assertion_files':21,
            'reference_sha256':sha(a.reference),'standalone_original_comparison_sha256':sha(comparison_path),'item_cache_sha256':sha(items),
            'runner_sha256':sha(a.runner),'test_sha256':sha(Path(__file__)),'listing_sha256':sha(listing),'property_fixture_sha256':sha(files[0]),
            'assertions':[{'name':f.name,'bytes':f.stat().st_size,'sha256':sha(f)}for f in files[2:-1]],'stdout':output,
            'source_sha256':{n:sha(ROOT/n)for n in ('runtime.c','runtime.h','tests/runner.c','tests/equipment.c','../lua-character/bridge.c','../equipment-bonuses/equipment.c','../loot-tables/loot.c')},
            'scope':'All actual cache items pass through retained owned Lua item generations, authored two-set snapshot controls and named bonus/shield methods, using synthetic property data. Expected values come from the standalone C projection separately matched against original ARM instructions. Native bonus callbacks/Value coercion and the original Lua/game are not executed by this corpus. Selftests cover typed arguments, caller release, generations, malformed/OOM retention and recovery. No gear contributions, equip restrictions, original inventory or Character lifecycle.'}
    if device:result['device']=device;result['pushed_input_hashes_verified']=len(hashes)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items()if k not in ('assertions','stdout')}))
if __name__=='__main__':main()
