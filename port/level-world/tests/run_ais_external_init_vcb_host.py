"""Original InitVCB callers plus unchanged Lua registration-map fixture."""
from __future__ import annotations
import argparse,hashlib,json,os,re,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
RUNTIME=ROOT/'port/adam-script-runtime'
CORE='lapi lcode ldebug ldo ldump lfunc lgc llex lmem lobject lopcodes lparser lstate lstring ltable ltm lundump lvm lzio lauxlib lbaselib ltablib lstrlib lmathlib'.split()
MANIFEST=MODULE/'reference/ais-external-init-vcb/original-functions.json'
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
AIS=0x10014000;NODES=0x10022000
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def hash_name(name):
    value=0
    for c in name.encode('ascii'):value=(value^(c+0x9e3779b9+(value<<6)+(value>>2)))&0xffffffff
    return value
def cases():
    masks=[0,0xfff,0x555,0xaaa,0xc3,0x0fc,0xf3]+[1<<i for i in range(12)]+[0xfff^(1<<i) for i in range(12)]
    rows=[]
    for external in (0,1):
        for mask in masks:rows.append((external,mask,0,0xfefefefe))
        for mutation in range(1,8):
            for mask in (0,0xfff,0xf3):rows.append((external,mask,mutation,0xffffffff))
    return rows
def original_oracle(original,exe):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE,UC_HOOK_MEM_WRITE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    raw=original.read_bytes();assert digest(original)==SHA
    manifest=json.loads(MANIFEST.read_text())
    with original.open('rb') as f:
        elf=ELFFile(f);symbols={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        def data(at,n):
            s=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz'])
            offset=int(s['p_offset'])+at-int(s['p_vaddr']);return raw[offset:offset+n]
        for row in manifest['functions']:
            at,n=int(row['elf_address'],0),row['size'];symbol=symbols[row['original_symbol']]
            assert (int(symbol['st_value']),int(symbol['st_size']))==(at,n)
            assert hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
        for row in manifest['queries']:
            pc,literal=int(row['pc_add_address'],0),int(row['literal_address'],0)
            at=(pc+8+struct.unpack('<I',data(literal,4))[0])&0xffffffff
            text=data(at,len(row['name'])+1);assert text==row['name'].encode()+b'\0'
            assert at==int(row['string_address'],0) and hashlib.sha256(text).hexdigest()==row['string_sha256']
    names=[row['name'] for row in manifest['queries']];keys=[hash_name(name) for name in names];assert len(set(keys))==12
    old=Cpu(original,False,manifest);old.uc.mem_map(0x10000000,0x60000)
    def get(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
    def string(at):
        value=bytearray()
        while old.uc.mem_read(at,1)!=b'\0':value+=old.uc.mem_read(at,1);at+=1
        return value.decode('ascii')
    def returned(value):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
    records=[]
    for external,initial_mask,mutation,prior in cases():
        old.uc.mem_write(old.stack,bytes(0x10000));old.uc.mem_write(AIS,bytes(0x100));old.pointer(AIS+0xb8,prior)
        members=[initial_mask];generation=[0];calls=[];writes=[];hash_trace=[]
        def replace(mask):
            generation[0]+=1;members[0]=mask;values=sorted(keys[i] for i in range(12) if mask&(1<<i))
            base=NODES+generation[0]*0x1000;count=[0]
            def tree(items):
                if not items:return 0
                middle=len(items)//2;node=base+count[0]*32;count[0]+=1
                old.pointer(node+0x10,items[middle]);old.pointer(node+8,tree(items[:middle]));old.pointer(node+12,tree(items[middle+1:]));return node
            old.pointer(AIS+0x38,tree(values))
        replace(initial_mask)
        def mutate(index):
            if mutation==1:old.pointer(AIS+0xb8,0xbeef0000+index)
            if mutation==2 and index==0:replace(members[0]^2)
            if mutation==3 and index==1:replace(members[0]^4)
            if mutation==4 and index==3:replace(0)
            if mutation==5 and index==4:replace(0xfff)
            if mutation==6 and index==5:replace(members[0]^(1<<11))
            if mutation==7:replace(members[0]^(1<<((index+1)%12)))
        def observe(_,at,__,___):
            if at==0x37c2a0:
                assert old.reg(0)==AIS;name=string(old.reg(1));index=names.index(name);calls.append([index,get(AIS+0xb8),0])
            elif at==0x37c164:
                name=string(old.reg(0));key=hash_name(name);hash_trace.append([name,key]);returned(key)
            elif at in (0x37c304,0x37c310):
                index=calls[-1][0];assert old.reg(0) in (0,1);calls[-1][2]=old.reg(0);mutate(index)
        def store(_,kind,at,size,value,unused):
            if at==AIS+0xb8:assert size==4;writes.append(value)
        code=old.uc.hook_add(UC_HOOK_CODE,observe);write=old.uc.hook_add(UC_HOOK_MEM_WRITE,store)
        old.invoke(0x3dcec8 if external else 0x3dc7d8,[AIS]);old.uc.hook_del(code);old.uc.hook_del(write)
        expected={'status':0,'flags':get(AIS+0xb8),'writes':len(writes),'calls':calls}
        actual=json.loads(subprocess.check_output([str(exe),'oracle',str(external),str(initial_mask),str(mutation),str(prior)],text=True))
        assert actual==expected,(external,initial_mask,mutation,actual,expected)
        records.append({'external':bool(external),'membership_mask':initial_mask,'mutation':mutation,'matched':True,
            'source_result':expected,'compiled_result':actual,'original_flag_stores':writes,'modeled_hash_dependency':hash_trace})
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'results':records,
        'executed_scope':'Complete92B AISDefault and408B AISExternal InitVCB caller instructions; actual116B IsInVFTable binary-search/boolean return instructions over retained original-layout nodes. hashString316B is a named unsigned source-hash fixture dependency; tree creation/mutation is fixture ownership. No membership result, flag branch or flag store is mocked.',
        'native_wired':False}
def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--compiler');parser.add_argument('--c-compiler');parser.add_argument('--original-elf',type=Path,required=True)
    parser.add_argument('--output',type=Path,default=MODULE/'build/ais-external-init-vcb/host.exe');parser.add_argument('--report',type=Path,default=MODULE/'build/ais-external-init-vcb/validation.json');args=parser.parse_args()
    cxx=args.compiler or os.environ.get('CXX') or shutil.which('g++')
    if not cxx:parser.error('pass --compiler')
    cc=args.c_compiler or str(Path(cxx).with_name(Path(cxx).name.replace('g++','gcc')))
    output=args.output.resolve();output.parent.mkdir(parents=True,exist_ok=True);objects=output.parent/'objects';objects.mkdir(exist_ok=True)
    original=ROOT/'recovered/scripts/original/data/scripts/ai';commons,monster=original/'_commons.luac',original/'monster.luac'
    assert digest(commons)=='20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c'
    assert digest(monster)=='84f07caaeb2c04f2024cc3e27d41f33806b2c53d6e8861bb3d8b371118fd0e1d'
    registration={}
    for path in (commons,monster):registration.update(dict(re.findall(r'AddToVFTable\("([^"]+)",\s*"([^"]+)"\)',path.read_text())))
    manifest=json.loads(MANIFEST.read_text());derived_mask=sum(row['bit'] for row in manifest['queries'] if row['name'] in registration)
    assert derived_mask==0x183c and 'OnUpdate' not in registration and 'OnFriendSpotted' not in registration
    property_schema=ROOT/'port/game-data/reference/properties/property-rules.json'
    schema=json.loads(property_schema.read_text());properties=schema['rows'] if isinstance(schema,dict) else schema
    assert next(row['id'] for row in properties if row['name']=='SkillTree')==28
    c_sources=[RUNTIME/'lua'/(name+'.c') for name in CORE]+[RUNTIME/'script_runtime.c',ROOT/'port/lua-numeric/numeric.c']
    cpp_sources=[RUNTIME/'script_function_alias.cpp',MODULE/'ais_external_init_vcb.cpp',MODULE/'tests/ais_external_init_vcb.cpp']
    compiled=[];commands=[];warnings=[]
    for path in c_sources+cpp_sources:
        obj=objects/(path.parent.name+'-'+path.stem+'.o');own=path in cpp_sources[1:]
        command=[cxx if path.suffix=='.cpp' else cc,'-std=c++17' if path.suffix=='.cpp' else '-std=c99','-O1','-fno-fast-math','-ffp-contract=off','-I',str(RUNTIME/'lua'),'-Wall',*( ['-Wextra','-Werror','-pedantic'] if own else []),'-c',str(path),'-o',str(obj)]
        built=subprocess.run(command,cwd=ROOT,capture_output=True,text=True)
        if built.returncode:raise RuntimeError(built.stdout+built.stderr)
        if built.stderr:warnings.append({'source':path.relative_to(ROOT).as_posix(),'diagnostics':built.stderr})
        commands.append(command);compiled.append(obj)
    command=[cxx,*map(str,compiled),'-lm','-o',str(output)];commands.append(command)
    built=subprocess.run(command,cwd=ROOT,capture_output=True,text=True)
    if built.returncode:raise RuntimeError(built.stdout+built.stderr)
    host=json.loads(subprocess.check_output([str(output),str(commons),str(monster)],text=True));assert host['validation']=='PASS' and host['host_cases']==149 and host['plain_monster_mask']==derived_mask
    comparison=original_oracle(args.original_elf.resolve(),output);assert comparison['comparisons']==104
    paths=c_sources+cpp_sources+[MODULE/'ais_external_init_vcb.hpp',RUNTIME/'script_runtime.h',RUNTIME/'script_function_alias.h',ROOT/'port/lua-numeric/numeric.h',Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md'),property_schema]+list((RUNTIME/'lua').glob('*.h'))
    report={'validation':'PASS','host_report':host,'original_arm_comparison':comparison,'original_sha256':SHA,'compiler_commands':commands,'dependency_warnings':warnings,
        'source_sha256':{p.relative_to(ROOT).as_posix():digest(p) for p in paths},'unchanged_scripts':{p.relative_to(ROOT).as_posix():digest(p) for p in (commons,monster)},
        'executed_registration_aliases':registration,'derived_original_mask':derived_mask,'native_wired':False,'whole_original_vm_parity':False}
    args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'validation':'PASS','host_cases':host['host_cases'],'original_arm_cases':comparison['comparisons'],'mask':hex(derived_mask),'mismatches':0}))
if __name__=='__main__':main()
