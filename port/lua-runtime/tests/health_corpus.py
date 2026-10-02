#!/usr/bin/env python3
"""Actual original health calls generate assertions for owned source Lua actors."""
import argparse,ctypes as c,hashlib,importlib.util,json,struct,subprocess
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
spec=importlib.util.spec_from_file_location('health_diff',REPO/'port/character-health/tests/differential.py')
h=importlib.util.module_from_spec(spec);spec.loader.exec_module(h)
State,Sheet,Table=h.State,h.base.Sheet,h.Table
class Inputs(c.Structure):_fields_=[('base',c.POINTER(Sheet)),('saved',c.POINTER(Sheet)),('gears',c.POINTER(Sheet)),('groups',c.c_void_p),('count',c.c_uint32)]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def run(*args):
    r=subprocess.run(list(map(str,args)),capture_output=True,text=True)
    if r.returncode:raise RuntimeError(r.stdout+'\n'+r.stderr)
    return r.stdout.strip()
def main():
    p=argparse.ArgumentParser()
    for n in ('runner','cache','reference','original','oracle','report'):p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--mode',choices=['controlled','real'],required=True);p.add_argument('--adb',type=Path);p.add_argument('--serial');a=p.parse_args()
    if bool(a.adb)!=bool(a.serial):p.error('--adb and --serial must be paired')
    evidence_path=REPO/'port/character-health/differential-validation.json';evidence=json.loads(evidence_path.read_text(encoding='utf-8'))
    assert sha(a.reference)==evidence['host_sha256']and evidence['comparisons']==4156 and not evidence['mismatches']
    assert sha(a.original)==evidence['original_sha256']and sha(a.oracle)==evidence['oracle_sha256']
    props=a.cache/'data/pydata/character_properties_pyarray.bin';raw=props.read_bytes();assert sha(props)==evidence['cache_sha256']
    dep=h.base.original.cpu.Dependencies;h.base.original.cpu.Dependencies=h.Dependencies
    try:old=h.base.original.Original(a.original,a.oracle,raw)
    finally:h.base.original.cpu.Dependencies=dep
    m=old.machine;character=m.data+0x3000;owner=character+0x560;offs=(8,0x38c,0x710,0xa94)
    arguments,vector,values,returns,online=[m.data+x for x in (0x6000,0x6100,0x7000,0x9000,0x2000)]
    m.uc.mem_write(arguments,struct.pack('<II',0,vector));m.uc.mem_write(online,bytes(6));emitted=[]
    def boundary(uc,address,size,unused):
        if address==0x7fd794:m.write_reg(0,online)
        elif address==0x320e14 or address==0x337a88:m.write_reg(0,0)
        elif address in (0x37cb24,0x37c7e4):
            assert m.reg(0)==returns;emitted.append(bool(m.reg(1))if address==0x37c7e4 else c.c_float(c.c_int32(m.reg(1)).value).value)
        uc.reg_write(m.pc_reg,uc.reg_read(m.lr_reg))
    for addr in (0x7fd794,0x320e14,0x337888,0x3140ec,0x337a88,0x318254,0x37cb24,0x37c7e4):m.uc.hook_add(UC_HOOK_CODE,boundary,begin=addr,end=addr)
    lib=c.CDLL(str(a.reference.resolve()));table=Table();buffer=c.create_string_buffer(raw)
    lib.dh2_property_open.argtypes=[c.POINTER(Table),c.c_void_p,c.c_uint32];lib.dh2_character_props_init.argtypes=[c.POINTER(Table),c.POINTER(State)]
    lib.dh2_property_load.argtypes=[c.POINTER(Table),c.c_uint32,c.POINTER(Sheet)];lib.dh2_property_recalc.argtypes=[c.POINTER(Table),c.POINTER(Inputs),c.c_uint32,c.POINTER(Sheet)]
    assert not lib.dh2_property_open(c.byref(table),buffer,len(raw))
    def prepare(state):
        m.uc.mem_write(character,bytes(0x1600))
        for name,off in zip(('base','saved','gears','final'),offs):m.uc.mem_write(owner+off+4,bytes(getattr(state,name)))
        header=owner+0xe18;m.uc.mem_write(header,struct.pack('<4I',0,0,header,header))
    def lit(value):return str(value).lower()if isinstance(value,bool)else repr(c.c_float(value).value)
    chunks=[];lines=[];queries=0;count=0;calls=0
    def check_return(call,result):
        names=','.join('v'+str(i)for i in range(max(1,len(result))))
        return 'do local function check(...) assert(select("#",...)=='+str(len(result))+');local '+names+'=...;'+ ';'.join(f'assert(v{i}=={lit(v)})'for i,v in enumerate(result))+' end;check('+call+') end'
    def script(address,args,tags):
        packed=bytearray(112*len(args))
        for index,(v,t)in enumerate(zip(args,tags)):struct.pack_into('<If',packed,112*index+4,t,v)
        m.uc.mem_write(vector,struct.pack('<II',values,values+len(packed)));m.uc.mem_write(values,bytes(packed)or bytes(4));emitted.clear();old.invoke(address,[arguments,returns,character]);assert bytes(m.uc.mem_read(values,len(packed)))==bytes(packed)
        return emitted[:]
    def finish():
        nonlocal queries,count,lines
        final=struct.unpack('<224i',m.uc.mem_read(owner+0xa94+4,896))
        lines.extend(f'assert(p:GetProp({f})=={lit(v)})'for f,v in enumerate(final));queries+=224;lines.append('end');count+=1
        if count%16==0:chunks.append('\n'.join(lines)+'\n');lines=[]
    if a.mode=='controlled':
        raw=struct.pack('<I',3)+bytes(896)+struct.pack('<224i',*([8]*224))+bytes(904)
        m.uc.mem_write(old.character_pointer+904,struct.pack('<224i',*([8]*224)))
        initials=[(0,25600,0,5120),(12800,25600,2560,5120),(-256,25600,-256,5120),(257,768,257,512),(0,0,0,0),
                  (2147483520,2147483520,2147483520,2147483520),(-2147483648,-256,-2147483648,-256),(65536,256,65536,256)]
        parameters=[([],[]),([1],[1]),([1],[0]),([1],[4])]+[([v],[3])for v in (-2147483648.,2147483520.,-257.9,-1.9,0,1.9,257.9,100000.)]+[([123.9,7,8],[3,1,0])]
        for initial in initials:
            for method,address in (('RegenHP',0x3b77ec),('RegenMP',0x3b7774),('HasMana',0x3b78ec),('UseMana',0x3b7864),('GetHP',0x3b6cd8)):
                for args,tags in parameters:
                    state=State();lines.append('do local p=DH2CreatePropertyState(2)')
                    for field,v in zip((36,38,41,43),initial):state.final.values[field]=v;lines.append(f'p:SetProp({field},{lit(v)})')
                    prepare(state);result=script(address,args,tags);calls+=1
                    literals=[lit(v)if t==3 else'true'if t==1 else'nil'if t==0 else'"ignored"'for v,t in zip(args,tags)]
                    lines.append(check_return('p:'+method+'('+','.join(literals)+')',result));finish()
    else:
        for row in range(2,448):
            state=State();assert not lib.dh2_character_props_init(c.byref(table),c.byref(state));assert not lib.dh2_property_load(c.byref(table),row,c.byref(state.base))
            inputs=Inputs(c.pointer(state.base),c.pointer(state.saved),c.pointer(state.gears),None,0)
            for field in range(224):assert not lib.dh2_property_recalc(c.byref(table),c.byref(inputs),field,c.byref(state.final))
            prepare(state);lines.append(f'do local p=DH2CreatePropertyState({row})')
            for method,address,argument in (('SetHP',0x3bd0d0,3),('SetMP',0x3bd0c0,2),('ValidateHPMP',0x3bd140,None)):
                old.invoke(address,[character]+([]if argument is None else[argument]));lines.append(f'p:{method}('+(''if argument is None else str(argument))+')');calls+=1
            for method,address,value in (('RegenHP',0x3b77ec,256),('RegenMP',0x3b7774,257),('HasMana',0x3b78ec,256),('UseMana',0x3b7864,256),('GetHP',0x3b6cd8,0)):
                lines.append(check_return('p:'+method+'('+str(value)+')',script(address,[value],[3])));calls+=1
            for method,address in (('GetMP',0x3bd100),('GetTotalHP',0x3bd0f0),('GetTotalMP',0x3bd0e0)):
                value=c.c_int32(old.invoke(address,[character])).value;lines.append(f'assert(p:{method}()=={lit(value)})');calls+=1
            finish()
    if lines:chunks.append('\n'.join(lines)+'\n')
    staging=ROOT/('build/health-'+a.mode+'-'+(a.serial or'host'));staging.mkdir(parents=True,exist_ok=True);file=staging/'properties.bin';file.write_bytes(raw);files=[file];paths=[];remote='/data/local/tmp/dh2-health-'+a.mode
    for index,source in enumerate(chunks):
        path=staging/f'assertions-{index:03}.lua';path.write_text(source,encoding='ascii',newline='\n');assert path.stat().st_size<1024*1024;files.append(path);paths.append(remote+'/'+path.name if a.adb else str(path.resolve()))
    listing=staging/'list.txt';listing.write_text('\n'.join(paths)+'\n',encoding='ascii',newline='\n');files.append(listing);device=None;hashes={}
    if a.adb:
        def adb(*args):
            argv=list(args)
            if a.adb.suffix.lower()=='.exe'and argv[0]=='push':
                contents=str(argv[1]).endswith('/.');argv[1]=run('wslpath','-w',Path(argv[1]).resolve())+('/.'if contents else'')
            return run(a.adb,'-s',a.serial,*argv)
        assert adb('shell','getprop','ro.kernel.qemu')=='1'
        device={'serial':a.serial,'release':adb('shell','getprop','ro.build.version.release'),'sdk':int(adb('shell','getprop','ro.build.version.sdk')),'page_size':int(adb('shell','getconf','PAGE_SIZE')),'abi':adb('shell','getprop','ro.product.cpu.abi')}
        assert device['release']=='17'and device['sdk']==37 and device['abi']=='x86_64'and device['page_size']in (4096,16384)
        adb('shell','mkdir','-p',remote);adb('push',str(staging)+'/.',remote+'/');adb('push',a.runner,remote+'/runner');adb('shell','chmod','755',remote+'/runner')
        remotes=[remote+'/'+f.name for f in files]+[remote+'/runner']
        for line in adb('shell','sha256sum',*remotes).splitlines():digest,path=line.split(None,1);hashes[path.strip()]=digest
        for path,file in zip(remotes,[*files,a.runner]):assert hashes[path]==sha(file)
        output=adb('shell',remote+'/runner','--properties',remote+'/properties.bin',remote+'/list.txt')
    else:output=run(a.runner.resolve(),'--properties',files[0].resolve(),listing.resolve())
    assert output.splitlines()[:2]==['SELFTEST PASS','NUMERIC PASS 504']and output.splitlines()[-1]=='PROPERTY CORPUS PASS '+str(len(chunks))
    result={'complete_game':False,'mode':a.mode,'actor_cases':count,'original_health_calls':calls,'final_field_queries':queries,'assertion_files':len(chunks),
            'mismatches':0,'original_sha256':sha(a.original),'oracle_sha256':sha(a.oracle),'reference_sha256':sha(a.reference),'comparison_report_sha256':sha(evidence_path),'property_input_sha256':sha(files[0]),
            'runner_sha256':sha(a.runner),'test_sha256':sha(Path(__file__)),'stdout':output,'files':[{'name':f.name,'bytes':f.stat().st_size,'sha256':sha(f)}for f in files],
            'source_sha256':{n:sha(ROOT/n)for n in ('runtime.c','../lua-character/bridge.c','../character-health/health.c','tests/combat.c','build.py')},
            'scope':'Original health/mana instruction bodies and first numeric argument callbacks generate expected return values and all 224 final fields for owned source Lua actors. Controlled fixtures use type8 fields, finite conversions and eight signed/zero/overflow/cap scenarios; real fixtures initialize all 446 character rows through previously checked owned property loading/composition, then execute actual original health writes, validation, regeneration and mana methods. Initialization is authored, empty buffs, offline normal config/debug flags; original Character constructor, Lua VM, combat/death/events and gameplay remain absent.'}
    if device:result.update(device=device,pushed_hashes_verified=len(hashes))
    a.report.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8');print(json.dumps({k:v for k,v in result.items()if k not in ('files','stdout','scope')}))
if __name__=='__main__':main()
