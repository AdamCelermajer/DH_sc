#!/usr/bin/env python3
"""All real item/power stat contributions through owned Lua lifecycle bindings."""
import argparse,ctypes as c,hashlib,json,struct,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
class Sheet(c.Structure):_fields_=[('values',c.c_int32*224)]
class State(c.Structure):_fields_=[(n,Sheet) for n in ('base','saved','gears','final')]
class Props(c.Structure):_fields_=[('bytes',c.c_void_p),('size',c.c_uint32),('counts',c.c_uint32*3),('offsets',c.c_uint32*3)]
class Classes(c.Structure):_fields_=[('bytes',c.c_void_p),('size',c.c_uint32),('count',c.c_uint32)]
class Loot(c.Structure):_fields_=[('bytes',c.c_void_p),('size',c.c_uint32),('counts',c.c_uint32*8),('offsets',c.c_uint32*8)]
class Powers(c.Structure):_fields_=[('bytes',c.c_void_p),('size',c.c_uint32),('counts',c.c_uint32*2),('offsets',c.c_uint32*2)]
class Entry(c.Structure):_fields_=[('item_id',c.c_int32),('power_count',c.c_uint32),('power_ids',c.POINTER(c.c_int32))]
def main():
    p=argparse.ArgumentParser()
    for n in ('runner','cache','reference','report'):p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--adb',type=Path);p.add_argument('--serial');a=p.parse_args()
    if bool(a.adb)!=bool(a.serial):p.error('--adb and --serial must be paired')
    report_path=REPO/'port/gear-properties/differential-validation.json';comparison=json.loads(report_path.read_text())
    assert sha(a.reference)==comparison['host_sha256'] and comparison['comparisons']==7147 and not comparison['mismatches']
    paths=[a.cache/('data/pydata/'+n+'_pyarray.bin') for n in ('character_properties','character_classes','loot_table','item_powers')]
    raws=[p.read_bytes() for p in paths]
    assert dict(zip(('properties','classes','loot','powers'),map(lambda x:hashlib.sha256(x).hexdigest(),raws)))==comparison['cache_sha256']
    lib=c.CDLL(str(a.reference.resolve()));views=[Props(),Classes(),Loot(),Powers()];buffers=[c.create_string_buffer(raw) for raw in raws]
    for op,v,b,raw in zip(('dh2_property_open','dh2_class_open','dh2_loot_open','dh2_power_open'),views,buffers,raws):
        fn=getattr(lib,op);fn.argtypes=[c.POINTER(type(v)),c.c_void_p,c.c_uint32];assert not fn(c.byref(v),b,len(raw))
    pt,ct,lt,wt=views;lib.dh2_character_update_gears.argtypes=[c.POINTER(Props),c.POINTER(Classes),c.POINTER(Loot),c.POINTER(Powers),c.POINTER(State),c.POINTER(Entry),c.c_uint32]
    lib.dh2_property_reset.argtypes=[c.POINTER(Props),c.POINTER(Sheet)];lib.dh2_property_load.argtypes=[c.POINTER(Props),c.c_uint32,c.POINTER(Sheet)]
    assert lt.counts[3]==1322 and wt.counts[1]==937
    chunks=[];lines=[];case_count=queries=0;categories={}
    for category,total in [('items',1322),('powers',937)]:
        categories[category]=total*2
        for id in range(total):
            for hand in range(2):
                row=2+(id%446);slot=2 if hand else 1;item=id if category=='items' else (id*17)%1322
                s=State();assert not lib.dh2_property_reset(c.byref(pt),c.byref(s.base));s.saved=s.gears=s.final=s.base
                assert not lib.dh2_property_load(c.byref(pt),row,c.byref(s.base))
                powers=[id] if category=='powers' else []
                arr=(c.c_int32*len(powers))(*powers);entries=(Entry*16)()
                for i in range(16):entries[i]=Entry(-1,0,None)
                entries[slot]=Entry(item,len(powers),arr)
                assert not lib.dh2_character_update_gears(c.byref(pt),c.byref(ct),c.byref(lt),c.byref(wt),c.byref(s),entries,16)
                lines.append(f'do local c=DH2CreatePropertyState({row});c:EquipGear({hand},{slot},{item});c:SelectEquipmentSet({hand})')
                if powers:lines.append(f'c:SetItemPowers({hand},{slot},{id})')
                lines.append('c:UpdateGearsProperties()')
                for field,value in enumerate(s.final.values):lines.append(f'assert(c:GetProp({field},false)=={value})');queries+=1
                lines.append('end');case_count+=1
                if case_count%32==0:chunks.append('\n'.join(lines)+'\n');lines=[]
            if id%200==0:print(category,id,flush=True)
    if lines:chunks.append('\n'.join(lines)+'\n')
    assert case_count==4518 and queries==1012032 and len(chunks)==142
    staging=ROOT/('build/gears-corpus-'+(a.serial or 'host'));staging.mkdir(parents=True,exist_ok=True);base='/data/local/tmp/dh2-lua-gears-qa';files=[]
    for name,raw in zip(('properties','classes','items','powers'),raws):path=staging/(name+'.bin');path.write_bytes(raw);files.append(path)
    script_paths=[]
    for i,source in enumerate(chunks):
        path=staging/f'{i:03}.lua';path.write_text(source,encoding='ascii',newline='\n');assert path.stat().st_size<1024*1024
        files.append(path);script_paths.append(base+'/'+path.name if a.adb else str(path.resolve()))
    listing=staging/'list.txt';listing.write_text('\n'.join(script_paths)+'\n',encoding='ascii',newline='\n');files.append(listing)
    def run(*args):return subprocess.run(list(map(str,args)),capture_output=True,text=True,check=True).stdout.strip()
    device=None;hashes={}
    if a.adb:
        def adb(*args):
            argv=list(args)
            if a.adb.suffix.lower()=='.exe' and argv[0]=='push':
                contents=str(argv[1]).endswith('/.');argv[1]=run('wslpath','-w',Path(argv[1]).resolve())+('/.' if contents else '')
            return run(a.adb,'-s',a.serial,*argv)
        device={'serial':a.serial,'android_release':adb('shell','getprop','ro.build.version.release'),'sdk':int(adb('shell','getprop','ro.build.version.sdk')),'page_size':int(adb('shell','getconf','PAGE_SIZE')),'abi':adb('shell','getprop','ro.product.cpu.abi')}
        assert adb('shell','getprop','ro.kernel.qemu')=='1' and device['android_release']=='17' and device['sdk']==37 and device['page_size'] in (4096,16384) and device['abi']=='x86_64'
        adb('shell','mkdir','-p',base);adb('push',a.runner,base+'/runner');adb('push',str(staging.resolve())+'/.',base+'/');adb('shell','chmod','755',base+'/runner')
        locals=[(a.runner,'runner')]+[(f,f.name) for f in files]
        for line in adb('shell','sha256sum',*[base+'/'+name for path,name in locals]).splitlines():
            digest,path=line.split(None,1);hashes[Path(path.strip()).name]=digest
        for path,name in locals:assert hashes[name]==sha(path)
        stdout=adb('shell',base+'/runner','--gears',*[base+'/'+n+'.bin' for n in ('properties','classes','items','powers')],base+'/list.txt')
    else:stdout=run(a.runner.resolve(),'--gears',*[str(staging/n).replace('\\','/') for n in ('properties.bin','classes.bin','items.bin','powers.bin')],listing.resolve())
    assert stdout.splitlines()[:2]==['SELFTEST PASS','NUMERIC PASS 504'] and stdout.splitlines()[-1]=='GEAR CORPUS PASS 142'
    result={'complete_game':False,'runner_sha256':sha(a.runner),'test_sha256':sha(Path(__file__)),'reference_sha256':sha(a.reference),'comparison_report_sha256':sha(report_path),'cache_sha256':comparison['cache_sha256'],'cases':case_count,'categories':categories,'final_queries':queries,'device':device,'remote_input_hashes_verified':len(hashes),'assertions':[{'name':f'{i:03}.lua','sha256':sha(staging/f'{i:03}.lua'),'bytes':(staging/f'{i:03}.lua').stat().st_size} for i in range(len(chunks))],'stdout':stdout,'scope':'All 1322 real items and 937 real powers in main/off hand through authored owned Lua gear controls and empty-buff lifecycle, with all 224 final fields checked. Expected queries use the separately original-checked standalone C module; returned values use Lua float32 numeric representation. Original Lua/gameplay equivalence and full inventory ownership are not claimed.'}
    result['source_sha256']={n:sha(ROOT/n) for n in ('runtime.c','runtime.h','tests/runner.c','tests/gears.c','../lua-character/bridge.c','../gear-properties/gears.c','../gear-properties/gears.h')}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items() if k not in ('assertions','stdout')}))
if __name__=='__main__':main()
