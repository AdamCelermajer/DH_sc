"""Adapt Adam's Player caller; replay actual selected DSOs and original ARM graph."""
from __future__ import annotations
import argparse,hashlib,json,os,re,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/ais-player-init-vcb/original-functions.json'
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
AIS=0x10014000;NODES=0x10022000
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def hash_name(name):
    value=0
    for c in name.encode('ascii'):value=(value^(c+0x9e3779b9+(value<<6)+(value>>2)))&0xffffffff
    return value
def run(command,env=None):
    p=subprocess.run(list(map(str,command)),cwd=ROOT,capture_output=True,text=True,env=env)
    if p.returncode:raise RuntimeError(f'{command}\n{p.stdout}\n{p.stderr}')
    return p.stdout,p.stderr
def original_compare(original,exe,env):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE,UC_HOOK_MEM_WRITE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    raw=original.read_bytes();assert digest(original)==SHA
    manifest=json.loads(MANIFEST.read_text(encoding='utf-8'))
    with original.open('rb') as f:
        elf=ELFFile(f);symbols={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()};loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        def data(at,n):
            s=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz']);offset=int(s['p_offset'])+at-int(s['p_vaddr']);return raw[offset:offset+n]
        for row in manifest['functions']+manifest['vtables']:
            at,n=int(row['elf_address'],0),row['size'];symbol=symbols[row.get('original_symbol',row.get('symbol'))]
            assert (int(symbol['st_value']),int(symbol['st_size']))==(at,n)
            assert hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
        for row in manifest['queries']:
            at=(int(row['pc_add_address'],0)+8+struct.unpack('<I',data(int(row['literal_address'],0),4))[0])&0xffffffff
            text=data(at,len(row['name'])+1);assert text==row['name'].encode()+b'\0'
            assert at==int(row['string_address'],0) and hashlib.sha256(text).hexdigest()==row['string_sha256']
        for row in manifest['vtables']:
            assert struct.unpack('<I',data(int(row['address_point'],0)+int(row['init_vcb_slot'],0),4))[0]==0x3dd884
    names=[r['name'] for r in manifest['queries']];keys=[hash_name(n) for n in names];assert len(set(keys))==3
    old=Cpu(original,False,manifest);old.uc.mem_map(0x10000000,0x60000)
    def get(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
    def string(at):
        value=bytearray()
        for i in range(128):
            c=old.uc.mem_read(at+i,1)
            if c==b'\0':return value.decode('ascii')
            value+=c
        raise AssertionError('query unterminated')
    def returned(value):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
    records=[];coverage=set()
    for mask in range(8):
      for mutation in range(7):
        old.uc.mem_write(old.stack,bytes(0x10000));old.uc.mem_write(AIS,bytes(0x100));old.pointer(AIS+0xb8,0xfefefefe)
        members=[mask];generation=[0];calls=[];writes=[];hashes=[]
        def replace(value):
            generation[0]+=1;members[0]=value;values=sorted(keys[i] for i in range(3) if value&(1<<i));base=NODES+generation[0]*0x1000;count=[0]
            def tree(items):
                if not items:return 0
                middle=len(items)//2;node=base+count[0]*32;count[0]+=1
                old.pointer(node+0x10,items[middle]);old.pointer(node+8,tree(items[:middle]));old.pointer(node+12,tree(items[middle+1:]));return node
            old.pointer(AIS+0x38,tree(values))
        replace(mask)
        def mutate(index):
            if mutation==1:old.pointer(AIS+0xb8,0xbeef0000+index)
            if mutation==2 and index==0:replace(members[0]^2)
            if mutation==3 and index==1:replace(members[0]^4)
            if mutation==4 and index==2:replace(0)
            if mutation==5 and index==0:replace(7)
            if mutation==6:replace(members[0]^(1<<((index+1)%3)))
        def observe(_,at,__,___):
            coverage.add(at)
            if at==0x37c2a0:
                assert old.reg(0)==AIS;index=names.index(string(old.reg(1)));calls.append([index,get(AIS+0xb8),0])
            elif at==0x37c164:
                name=string(old.reg(0));key=hash_name(name);hashes.append([name,key]);returned(key)
            elif at in (0x37c304,0x37c310):
                assert old.reg(0) in (0,1);calls[-1][2]=old.reg(0);mutate(calls[-1][0])
        def store(_,kind,at,size,value,unused):
            if at==AIS+0xb8:assert size==4;writes.append(value)
        code=old.uc.hook_add(UC_HOOK_CODE,observe);write=old.uc.hook_add(UC_HOOK_MEM_WRITE,store)
        old.invoke(0x3dd884,[AIS]);old.uc.hook_del(code);old.uc.hook_del(write)
        expected={'status':0,'flags':get(AIS+0xb8),'writes':len(writes),'calls':calls}
        actual=json.loads(run([exe,'oracle',mask,mutation,0xfefefefe],env)[0]);assert actual==expected,(mask,mutation,actual,expected)
        records.append({'membership_mask':mask,'mutation':mutation,'matched':True,'source_result':expected,'compiled_result':actual,'original_flag_stores':writes,'modeled_hash_calls':hashes})
    executed={}
    for name,at,n in [('AISPlayer',0x3dd884,14),('AISDefault',0x3dc7d8,21),('IsInVFTable',0x37c2a0,29)]:
        addresses=list(range(at,at+n*4,4));missing=[hex(a) for a in addresses if a not in coverage];assert not missing,(name,missing)
        executed[name]={'instructions':n,'covered':n,'addresses':[hex(a) for a in addresses]}
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'instruction_coverage':executed,'results':records,
        'boundary':'Full Player60B and nested Default92B callers plus actual IsInVFTable116B search/returns. Literal pools excluded from instruction count. Only hashString316B is modeled by the maintained unsigned hash algorithm; retained tree construction/mutations are fixtures, not source body claims.'}
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler',required=True);p.add_argument('--c-compiler');p.add_argument('--cmake',default=shutil.which('cmake'));p.add_argument('--ninja',default=shutil.which('ninja'));p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--output',type=Path,default=MODULE/'build/ais-player-init-vcb');p.add_argument('--report',type=Path);a=p.parse_args()
    out=a.output.resolve();out.mkdir(parents=True,exist_ok=True);wrapper=out/'wrapper';wrapper.mkdir(exist_ok=True);build=out/'selected';build.mkdir(exist_ok=True)
    test=MODULE/'tests/ais_player_init_vcb.cpp'
    cmake_text=f'''cmake_minimum_required(VERSION 3.22)
project(ais_player_vcb_selected_audit LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-world)
add_executable(player_vcb_audit "{test.as_posix()}")
target_compile_features(player_vcb_audit PRIVATE cxx_std_17)
target_compile_options(player_vcb_audit PRIVATE -Wall -Wextra -Werror -pedantic -fno-fast-math -ffp-contract=off)
target_link_libraries(player_vcb_audit PRIVATE dh2_level_world dh2_script_runtime)
'''
    (wrapper/'CMakeLists.txt').write_text(cmake_text,encoding='utf-8')
    commands=[];logs=[]
    cxx=Path(a.compiler).resolve();cc=Path(a.c_compiler).resolve() if a.c_compiler else cxx.with_name(cxx.name.replace('g++','gcc'))
    config=[a.cmake,'-S',wrapper,'-B',build,'-G','Ninja',f'-DCMAKE_MAKE_PROGRAM={a.ninja}',f'-DCMAKE_CXX_COMPILER={cxx}',f'-DCMAKE_C_COMPILER={cc}','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_FLAGS_RELEASE=-O1','-DCMAKE_C_FLAGS_RELEASE=-O1']
    commands.append(list(map(str,config)));logs.append(run(config)[0])
    ninja_commands=run([a.ninja,'-C',build,'-t','commands','player_vcb_audit'])[0]
    assert 'ais_player_init_vcb.cpp' in ninja_commands,'Parent must select caller in actual level-world CMake first'
    compilation=json.loads((build/'compile_commands.json').read_text())
    sources={Path(r['file']).resolve() for r in compilation if str(r['file']).replace('\\','/') in ninja_commands.replace('\\','/')}
    assert MODULE/'ais_player_init_vcb.cpp' in sources and ROOT/'port/adam-script-runtime/script_runtime.c' in sources
    paths=sources|{MANIFEST,MANIFEST.with_name('NOTES.md'),Path(__file__).resolve(),ROOT/'port/engine-resources/tests/cpu.py'}
    # Capture headers/CMake in participating source modules before compilation.
    directories={path.parent for path in sources};directories.discard(MODULE/'tests')
    for directory in directories:
        paths.update(directory.glob('*.h'));paths.update(directory.glob('*.hpp'))
        if (directory/'CMakeLists.txt').exists():paths.add(directory/'CMakeLists.txt')
    before={p.relative_to(ROOT).as_posix():digest(p) for p in paths if p.is_relative_to(ROOT)}
    command=[a.cmake,'--build',build,'--target','player_vcb_audit','--parallel','1'];commands.append(list(map(str,command)));logs.append(run(command)[0])
    assert all(digest(ROOT/name)==sha for name,sha in before.items()),'Participating sources changed during build'
    exe=build/('player_vcb_audit.exe' if os.name=='nt' else 'player_vcb_audit')
    dsos=sorted(build.rglob('*.dll')) if os.name=='nt' else sorted(build.rglob('*.so'))
    env=os.environ.copy();env['PATH']=os.pathsep.join([str(cxx.parent),*(str(d.parent) for d in dsos),env.get('PATH','')])
    objdump=cxx.with_name('objdump.exe' if os.name=='nt' else 'objdump')
    imports={}
    if os.name=='nt':
        for binary in [exe,*dsos]:imports[binary.relative_to(out).as_posix()]=re.findall(r'DLL Name: (\S+)',run([objdump,'-p',binary])[0])
        world=next(d for d in dsos if d.name=='libdh2_level_world.dll');vm=next(d for d in dsos if d.name=='libdh2_script_runtime.dll')
        assert sum(d.name=='libdh2_script_runtime.dll' for d in dsos)==1
        assert world.name in imports[exe.relative_to(out).as_posix()] and vm.name in imports[exe.relative_to(out).as_posix()]
        assert vm.name in imports[world.relative_to(out).as_posix()]
    commons=ROOT/'recovered/scripts/original/data/scripts/ai/_commons.luac';assert digest(commons)=='20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c'
    host=json.loads(run([exe,commons],env)[0]);assert host=={'validation':'PASS','host_cases':57,'guards':10,'failure_cases':7,'real_lua_cases':4,'full_width_identity':True,'native_player_wired':False}
    oracle=original_compare(a.original_elf.resolve(),exe,env);assert oracle['comparisons']==56
    assert all(digest(ROOT/name)==sha for name,sha in before.items()),'Participating sources changed during replay'
    report={'validation':'PASS','attribution':json.loads(MANIFEST.read_text())['attribution'],'host_report':host,'original_arm_comparison':oracle,'original_sha256':SHA,'source_sha256':before,'unchanged_commons_sha256':digest(commons),'compiler_commands':commands,'compiler_version':run([cxx,'--version'])[0].splitlines()[0],'output_directory':out.as_posix(),'build_stdout':logs,'selected_commands':ninja_commands,'selected_dso_imports':imports,'binary_sha256':{p.relative_to(out).as_posix():digest(p) for p in [exe,*dsos]},'wrapper_cmake':cmake_text,'wrapper_sha256':digest(wrapper/'CMakeLists.txt'),'scope':'Actual current CMake dh2_level_world selected library and single shared dh2_script_runtime. One reused Player caller; zero new Default/External/VM/property/timer body credit. No native player activation or skill execution.'}
    destination=a.report.resolve() if a.report else out/'validation.json';destination.parent.mkdir(parents=True,exist_ok=True)
    destination.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'validation':'PASS','host_cases':57,'guards':10,'failure_cases':7,'real_lua_cases':4,'original_arm_cases':56,'original_instructions':64,'mismatches':0}))
if __name__=='__main__':main()
