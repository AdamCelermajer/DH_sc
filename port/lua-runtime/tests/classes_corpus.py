#!/usr/bin/env python3
"""Owned Lua class binding on all real classes; not original gameplay."""
import argparse,ctypes as c,hashlib,importlib.util,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
spec=importlib.util.spec_from_file_location('class_test',REPO/'port/character-classes/tests/differential.py')
# Its definitions need Unicorn; this corpus runs under the checked WSL test venv.
module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
State,Sheet,Table,Classes=module.State,module.Sheet,module.Table,module.Classes
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
class Inputs(c.Structure):_fields_=[('base',c.POINTER(Sheet)),('saved',c.POINTER(Sheet)),('gears',c.POINTER(Sheet)),('groups',c.c_void_p),('count',c.c_uint32)]
def main():
    p=argparse.ArgumentParser()
    for n in ('runner','cache','reference','report'):p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--adb',type=Path);p.add_argument('--serial');a=p.parse_args()
    if bool(a.adb)!=bool(a.serial):p.error('--adb and --serial must be paired')
    trace=REPO/'reports/character-class-reader-trace.json';assert sha(trace)=='83007e4d2ae222e538e79ffb1bad384866591ea17b9535b2f745540999be2308'
    comparison=json.loads((REPO/'port/character-classes/arm-differential-validation.json').read_text());assert sha(a.reference)==comparison['host_sha256']
    assert comparison['comparisons']==3270 and comparison['mismatches']==0
    props=a.cache/'data/pydata/character_properties_pyarray.bin';classes=a.cache/'data/pydata/character_classes_pyarray.bin'
    assert sha(props)==comparison['property_cache_sha256'] and sha(classes)==comparison['class_cache_sha256']
    lib=c.CDLL(str(a.reference.resolve()));pt=Table();ct=Classes();pb=c.create_string_buffer(props.read_bytes());cb=c.create_string_buffer(classes.read_bytes())
    lib.dh2_property_open.argtypes=[c.POINTER(Table),c.c_void_p,c.c_uint32];lib.dh2_class_open.argtypes=[c.POINTER(Classes),c.c_void_p,c.c_uint32]
    lib.dh2_property_reset.argtypes=[c.POINTER(Table),c.POINTER(Sheet)];lib.dh2_property_load.argtypes=[c.POINTER(Table),c.c_uint32,c.POINTER(Sheet)]
    lib.dh2_property_recalc.argtypes=[c.POINTER(Table),c.POINTER(Inputs),c.c_uint32,c.POINTER(Sheet)]
    lib.dh2_class_apply.argtypes=[c.POINTER(Classes),c.POINTER(Table),c.POINTER(State),c.c_uint32,c.POINTER(Sheet),c.c_int32,c.c_uint32]
    assert not lib.dh2_property_open(c.byref(pt),pb,len(props.read_bytes())) and not lib.dh2_class_open(c.byref(ct),cb,len(classes.read_bytes()))
    chunks=[];lines=[]
    for class_id in range(260):
        row=2+class_id%446
        for flag in range(2):
            state=State()
            for name in ('base','saved','gears','final'):assert not lib.dh2_property_reset(c.byref(pt),c.byref(getattr(state,name)))
            assert not lib.dh2_property_load(c.byref(pt),row,c.byref(state.base))
            inputs=Inputs(c.pointer(state.base),c.pointer(state.saved),c.pointer(state.gears),None,0)
            for field in range(224):assert not lib.dh2_property_recalc(c.byref(pt),c.byref(inputs),field,c.byref(state.final))
            assert not lib.dh2_class_apply(c.byref(ct),c.byref(pt),c.byref(state),0,None,class_id,flag)
            for field in range(224):assert not lib.dh2_property_recalc(c.byref(pt),c.byref(inputs),field,c.byref(state.final))
            lines.append(f'do local c=DH2CreatePropertyState({row});c:ApplyClass({class_id},{str(bool(flag)).lower()})')
            lines.extend(f'assert(c:GetProp({field})=={value})'for field,value in enumerate(state.final.values));lines.append('end')
        if class_id%10==9:chunks.append('\n'.join(lines)+'\n');lines=[]
    assert not lines and len(chunks)==26
    staging=ROOT/('build/classes-corpus-'+(a.serial or 'host'));staging.mkdir(parents=True,exist_ok=True)
    base='/data/local/tmp/dh2-lua-classes-qa';files=[];paths=[]
    for name,source in [('properties.bin',props),('classes.bin',classes)]:target=staging/name;target.write_bytes(source.read_bytes());files.append(target)
    for index,source in enumerate(chunks):
        target=staging/f'{index:03}.lua';target.write_text(source,encoding='ascii',newline='\n');assert target.stat().st_size<1024*1024
        files.append(target);paths.append(base+'/'+target.name if a.adb else str(target.resolve()))
    listing=staging/'list.txt';listing.write_text('\n'.join(paths)+'\n',encoding='ascii',newline='\n');files.append(listing)
    def run(*args):return subprocess.run(list(map(str,args)),capture_output=True,text=True,check=True).stdout.strip()
    hashes={};device=None
    if a.adb:
        def adb(*args):
            argv=list(args)
            # WSL host reference + Windows SDK adb: only local push paths need
            # translation. Remote Android paths retain their original spelling.
            if a.adb.suffix.lower()=='.exe' and argv[0]=='push':
                contents=str(argv[1]).endswith('/.')
                argv[1]=run('wslpath','-w',Path(argv[1]).resolve())+('/.'if contents else '')
            return run(a.adb,'-s',a.serial,*argv)
        assert adb('shell','getprop','ro.kernel.qemu')=='1'
        device={'serial':a.serial,'android_release':adb('shell','getprop','ro.build.version.release'),'sdk':int(adb('shell','getprop','ro.build.version.sdk')),
                'page_size':int(adb('shell','getconf','PAGE_SIZE')),'abi':adb('shell','getprop','ro.product.cpu.abi'),'fingerprint':adb('shell','getprop','ro.build.fingerprint')}
        assert device['android_release']=='17' and device['sdk']==37 and device['abi']=='x86_64' and device['page_size'] in (4096,16384)
        adb('shell','mkdir','-p',base);adb('push',str(staging)+'/.' ,base+'/');adb('push',a.runner,base+'/runner');adb('shell','chmod','755',base+'/runner')
        locals=[*files,a.runner];remotes=[base+'/'+f.name for f in files]+[base+'/runner']
        for line in adb('shell','sha256sum',*remotes).splitlines():digest,path=line.split(None,1);hashes[path.strip()]=digest
        for remote,local in zip(remotes,locals):assert hashes[remote]==sha(local)
        output=adb('shell',base+'/runner','--classes',base+'/properties.bin',base+'/classes.bin',base+'/list.txt')
    else:output=run(a.runner.resolve(),'--classes',files[0].resolve(),files[1].resolve(),listing.resolve())
    assert output.splitlines()[:2]==['SELFTEST PASS','NUMERIC PASS 504'] and output.splitlines()[-1]=='CLASS CORPUS PASS 26'
    result={'complete_game':False,'gameplay_object_callbacks_installed':False,'class_binding_tested':True,'original_lua_gameplay_equivalence_tested':False,
            'classes':260,'application_cases':520,'final_field_queries':116480,'assertion_files':26,
            'reference_sha256':sha(a.reference),'standalone_original_comparison_sha256':sha(REPO/'port/character-classes/arm-differential-validation.json'),
            'class_trace_sha256':sha(trace),'property_cache_sha256':sha(props),'class_cache_sha256':sha(classes),
            'runner_sha256':sha(a.runner),'test_sha256':sha(Path(__file__)),'listing_sha256':sha(listing),
            'assertions':[{'name':f.name,'bytes':f.stat().st_size,'sha256':sha(f)}for f in files[2:-1]],'stdout':output,
            'source_sha256':{n:sha(ROOT/n)for n in ('runtime.c','runtime.h','tests/runner.c','tests/classes.c','../lua-character/bridge.c','../character-classes/classes.c')},
            'scope':'Authored ApplyClass on owned diagnostic property userdata uses retained class/property generations, applies to base and recomposes all final fields with empty buffs. Every real class and both final-source flags are checked against the standalone C implementation separately matched to actual original ARM bodies. Selftests cover caller release, generations, malformed/OOM import retention and cyclic-application atomic rejection. No original Character lifecycle, combat or gameplay.'}
    if device:result['device']=device;result['pushed_input_hashes_verified']=len(hashes)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items()if k not in ('assertions','stdout')}))
if __name__=='__main__':main()
