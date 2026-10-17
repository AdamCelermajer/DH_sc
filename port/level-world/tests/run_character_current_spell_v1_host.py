"""Selected-library current-spell caller, live borrowed save/table and real Lua gate."""
from __future__ import annotations
import argparse,hashlib,itertools,json,os,random,re,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-current-spell-v1/original-functions.json'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def run(command,env=None):
    p=subprocess.run(list(map(str,command)),cwd=ROOT,env=env,capture_output=True,text=True)
    if p.returncode:raise RuntimeError(f'{command}\nexit={p.returncode}\n{p.stdout}\n{p.stderr}')
    return p.stdout
def arm(original,exe,cache,env,m):
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu,i32
    sys.path.insert(0,str(MODULE/'tests'));from elf_import_identity import verify_imports
    from unicorn import UC_HOOK_CODE
    imports=verify_imports(original,{0x31056c:'malloc'});cpu=Cpu(original,False,m)
    C=cpu.data+0x1000;R=cpu.data+0x3000;save=cpu.data+0x4000;alt=cpu.data+0x5000
    def put(at,n):cpu.pointer(at,n&0xffffffff)
    def word(at):return struct.unpack('<I',cpu.uc.mem_read(at,4))[0]
    def returned(n):cpu.put(0,n);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
    seen=set();comparisons=[]
    cases=list(itertools.product([0,1,4,0x7fffffff,0x80000000,0xffffffff],[0,2,4,0xffffffff],[0,1,65535,0xffffffff,16777217],range(2)))
    rng=random.Random(0x3b6e30);cases.extend((rng.getrandbits(32),rng.getrandbits(32),rng.getrandbits(32),rng.randrange(2)) for _ in range(16))
    for first,second,level,mut in cases:
        trace=[];selections=[0];pushed=[]
        def hook(_,at,__,___):
            if 0x3b6e30<=at<0x3b6e80:seen.add(at)
            if at==0x3bb98c:
                assert cpu.reg(0)==C and cpu.reg(1)==0xffffffff
                trace.append([0,0,-1,1]);returned(first if not selections[0] else second);selections[0]+=1
            elif at==0x3aeac0:
                assert cpu.reg(0)==C and cpu.reg(1)==first;trace.append([1,first,-1,1]);returned(0)
            elif at==0x3bbc18:
                assert [cpu.reg(i) for i in range(3)]==[C,second,0xffffffff];trace.append([2,second,-1,1]);returned(level)
            elif at==0x37cb24:
                assert cpu.reg(0)==R and cpu.reg(1)==level;pushed.append(i32(cpu.reg(1)));returned(0)
        h=cpu.uc.hook_add(UC_HOOK_CODE,hook)
        try:cpu.invoke(0x3b6e30,[1,R,C])
        finally:cpu.uc.hook_del(h)
        assert pushed==[i32(level)]
        expected={'status':1,'first':first,'second':second,'level':i32(level),'calls':4,'complete':1,'trace':trace}
        actual=json.loads(run([exe,'--oracle',first,second,level,mut],env));assert actual==expected,(first,second,level,mut,actual,expected)
        comparisons.append({'input':[first,second,level,mut],'original':expected,'compiled':actual})
    assert seen==set(range(0x3b6e30,0x3b6e80,4))
    # Compose actual original SG wrappers, GetCharFaery/selector and the saved
    # level leaf using canonical cache rows exported by the selected decoder.
    view=json.loads(run([exe,cache,'--view'],env));lists=cpu.data+0x8000;members=cpu.data+0x10000;rows=cpu.data+0x18000
    member_at=members
    for i,ids in enumerate(view['lists']):
        cpu.uc.mem_write(lists+12*i,struct.pack('<III',0,len(ids),member_at))
        if ids:cpu.uc.mem_write(member_at,struct.pack('<'+'i'*len(ids),*ids));member_at+=4*len(ids)
    raw_rows=[w for row in view['rows'] for w in row];cpu.uc.mem_write(rows,struct.pack('<'+'I'*len(raw_rows),*raw_rows))
    for symbol,value in [('_ZN6Arrays14FaeryListTable7membersE',lists),('_ZN6Arrays14FaeryListTable4sizeE',len(view['lists'])),('_ZN6Arrays10FaeryTable7membersE',rows),('_ZN6Arrays10FaeryTable4sizeE',len(view['rows'])),('gAssertLevel',0)]:put(cpu.symbols[symbol],value)
    difficulty=cpu.symbols['_ZN14PlayerSavegame17m_difficultyLevelE'];assert difficulty==0x9a6060
    nested=[];nested_seen=set();count_target=cpu.symbols['_ZNK15PyDataConstants11getConstantEPKcS1_']
    def cstring(at):return bytes(cpu.uc.mem_read(at,64)).split(b'\0')[0].decode()
    for class_id,diff,id,mut in itertools.product(range(3),range(3),range(5),range(4)):
        cpu.uc.mem_write(C,bytes(0x1600));put(C+0x106c,view['selectors'][class_id]);put(C+0x14e8,save);put(difficulty,diff)
        for base,offset,current in [(save,0,id),(alt,1000,0)]:
            cpu.uc.mem_write(base,bytes(0x200))
            for d in range(3):
                data=cpu.data+0x20000+offset*0x100+d*0x100
                put(base+0x94+4*d,data);put(base+0xa0+4*d,5);put(base+0xac+4*d,current)
                for i in range(5):cpu.uc.mem_write(data+4*i,struct.pack('<BBH',0,0,offset+100*d+i))
        queries=[0];push=[];source_calls=[]
        def nested_hook(_,at,__,___):
            if at in cpu.address_owner:nested_seen.add(at)
            if at in [0x3bb98c,0x3aeac0,0x3bbc18]:source_calls.append(at);assert cpu.reg(0)==C
            if at==count_target:
                assert cstring(cpu.reg(1))=='FaeryTypes' and cstring(cpu.reg(2))=='COUNT';queries[0]+=1
                if queries[0]==1:
                    if mut==1:put(difficulty,1)
                    if mut==2:put(C+0x14e8,alt)
                    if mut==3:put(C+0x14e8,0)
                returned(view['count'])
            elif at==0x37cb24:assert cpu.reg(0)==R;push.append(i32(cpu.reg(1)));returned(0)
        h=cpu.uc.hook_add(UC_HOOK_CODE,nested_hook)
        try:cpu.invoke(0x3b6e30,[1,R,C])
        finally:cpu.uc.hook_del(h)
        expected={'status':1,'first':id,'second':0 if mut in [2,3] else id,'level':-1 if mut==3 else (1000+100*diff if mut==2 else 100*(1 if mut==1 else diff)+id),'calls':4,'queries':2,'complete':1}
        assert push==[expected['level']] and source_calls==[0x3bb98c,0x3aeac0,0x3bb98c,0x3bbc18]
        actual=json.loads(run([exe,cache,'--borrowed',class_id,diff,id,mut],env));assert actual==expected,(class_id,diff,id,mut,expected,actual)
        nested.append({'input':[class_id,diff,id,mut],'original':expected,'compiled':actual})
    # _InitFaeries executes unchanged, with only its verified malloc import
    # modeled as bounded owned storage. Repeated initialization retains rows.
    init_seen=set();allocations=[];heap=[cpu.data+0x180000]
    cpu.uc.mem_write(save,bytes(0x200))
    for d in range(3):put(save+0xac+4*d,d)
    def init_hook(_,at,__,___):
        if 0x4694c8<=at<0x46954c:init_seen.add(at)
        if at==0x31056c:
            assert cpu.reg(0)==20;at=heap[0];heap[0]+=0x100;cpu.uc.mem_write(at,b'\xff'*20);allocations.append(at);returned(at)
    h=cpu.uc.hook_add(UC_HOOK_CODE,init_hook)
    try:
        cpu.invoke(0x4694c8,[save]);initial=[word(save+0x94+4*d) for d in range(3)]
        for d in range(3):assert word(save+0xa0+4*d)==5 and word(save+0xac+4*d)==d
        for at in initial:
            raw=bytes(cpu.uc.mem_read(at,20));assert all(raw[4*i]==0 and raw[4*i+2:4*i+4]==b'\0\0' and raw[4*i+1]==255 for i in range(5))
        cpu.uc.mem_write(initial[1]+2,struct.pack('<H',321));cpu.invoke(0x4694c8,[save]);assert len(allocations)==3 and [word(save+0x94+4*d) for d in range(3)]==initial and struct.unpack('<H',cpu.uc.mem_read(initial[1]+2,2))[0]==321
    finally:cpu.uc.hook_del(h)
    assert init_seen==set(range(0x4694c8,0x46954c,4))-{0x46951c}
    return {'validation':'PASS','caller_comparisons':len(comparisons),'caller_instruction_addresses':[hex(x) for x in sorted(seen)],'caller_cases':comparisons,
            'nested_original_comparisons':len(nested),'nested_cases':nested,'nested_instruction_addresses':[hex(x) for x in sorted(nested_seen)],
            'actual_cache_view':view,'init_faeries_executions':2,'init_faeries_instruction_addresses':[hex(x) for x in sorted(init_seen)],'verified_imports':imports,'mismatches':0,
            'scope':'Complete80B callback control; SG/table/ReturnValues dependencies separately named. Nested original SG(-1) wrappers/GetCharFaery/SG level execute with actual cache rows and only COUNT/pushInteger providers. InitFaeries normal malloc-backed paths32/33 instructions corroborate reused saved backing (zero-count mutation path excluded); no duplicate body credit/native activation.'}
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler',required=True,type=Path);p.add_argument('--cache',required=True,type=Path);p.add_argument('--original-elf',required=True,type=Path);p.add_argument('--output',required=True,type=Path);p.add_argument('--report',type=Path,default=MODULE/'build/character-current-spell-v1/validation.json');a=p.parse_args()
    out=a.output.resolve();wrapper=out/'wrapper';build=out/'selected';wrapper.mkdir(parents=True,exist_ok=True);build.mkdir(exist_ok=True)
    cxx=a.compiler.resolve();cc=cxx.with_name(cxx.name.replace('g++','gcc'));cmake=shutil.which('cmake');ninja=shutil.which('ninja')
    source=MODULE/'character_current_spell_v1.cpp';test=MODULE/'tests/character_current_spell_v1.cpp';constants=ROOT/'port/pydata-constants/constants.c'
    cm=f'''cmake_minimum_required(VERSION 3.22)
project(current_spell_audit LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-world)
add_executable(current_spell_audit "{test.as_posix()}" "{constants.as_posix()}")
target_compile_features(current_spell_audit PRIVATE cxx_std_17)
target_compile_options(current_spell_audit PRIVATE -Wall -Wextra -Werror -pedantic -fno-fast-math -ffp-contract=off)
target_link_libraries(current_spell_audit PRIVATE dh2_level_world dh2_script_runtime)
'''
    (wrapper/'CMakeLists.txt').write_text(cm,encoding='utf-8')
    logs=[run([cmake,'-S',wrapper,'-B',build,'-G','Ninja',f'-DCMAKE_MAKE_PROGRAM={ninja}',f'-DCMAKE_CXX_COMPILER={cxx.as_posix()}',f'-DCMAKE_C_COMPILER={cc.as_posix()}','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_FLAGS_RELEASE=-O1','-DCMAKE_C_FLAGS_RELEASE=-O1'])]
    commands=run([ninja,'-C',build,'-t','commands','current_spell_audit']);db=json.loads((build/'compile_commands.json').read_text());sources={Path(x['file']).resolve() for x in db if str(x['file']).replace('\\','/') in commands.replace('\\','/')}
    assert source in sources and ROOT/'port/adam-script-runtime/script_runtime.c' in sources and ROOT/'port/game-data/player_savegame_v1.cpp' in sources
    paths=sources|{Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md'),MODULE/'tests/elf_import_identity.py',MODULE/'tests/run_character_ai_set_skills_and_spells_host.py',ROOT/'port/engine-resources/tests/cpu.py'}
    for folder in {x.parent for x in sources}:
        paths.update(folder.glob('*.h'));paths.update(folder.glob('*.hpp'))
        if (folder/'CMakeLists.txt').exists():paths.add(folder/'CMakeLists.txt')
    before={x.relative_to(ROOT).as_posix():sha(x) for x in paths if x.is_relative_to(ROOT)}
    cache_files=[a.cache/'data/pydata'/(stem+suffix+'.bin') for stem in ['skills','faeries','character_properties','character_classes'] for suffix in ['_pyarray','_pyarraynames','_pystructnames']]+[a.cache/'data/pydata/faeries_pycst.bin']
    cache_hash={x.relative_to(a.cache).as_posix():sha(x) for x in cache_files}
    m=json.loads(MANIFEST.read_text());sys.path.insert(0,str(MODULE/'tests'));from run_character_ai_set_skills_and_spells_host import source_symbols
    source_symbols(a.original_elf.resolve(),m)
    logs.append(run([cmake,'--build',build,'--target','current_spell_audit','--parallel','1']));assert all(sha(ROOT/name)==h for name,h in before.items()),'Sources changed during build'
    exe=build/'current_spell_audit.exe';dsos=sorted(build.rglob('*.dll'));env=os.environ.copy();env['PATH']=os.pathsep.join([str(cxx.parent),*(str(x.parent) for x in dsos),env.get('PATH','')])
    host=json.loads(run([exe,a.cache.resolve()],env));assert host['validation']=='PASS'
    original=arm(a.original_elf.resolve(),exe,a.cache.resolve(),env,m)
    assert all(sha(ROOT/name)==h for name,h in before.items()) and all(sha(a.cache/name)==h for name,h in cache_hash.items())
    import_text={x.relative_to(out).as_posix():run([cxx.with_name('objdump.exe'),'-p',x]) for x in [exe,*dsos]}
    imports={name:re.findall(r'DLL Name: (\S+)',text) for name,text in import_text.items()}
    exe_import=import_text[exe.relative_to(out).as_posix()]
    assert 'libdh2_level_world.dll' in imports[exe.relative_to(out).as_posix()]
    assert 'current_spell_info_v1' in exe_import and 'saved_services' in exe_import and 'query' in exe_import
    # The selected source is compiled inside the shared target, never as an
    # executable replacement of the production callback.
    assert 'CMakeFiles/current_spell_audit.dir/'+source.as_posix() not in commands.replace('\\','/')
    report={'validation':'PASS','host_report':host,'original_arm_comparison':original,'source_sha256':before,'cache_inputs_sha256':cache_hash,'original_sha256':sha(a.original_elf),
            'new_complete_original_caller_bodies':1,'new_complete_dependency_bodies':0,'attribution':m['attribution'],'selected_commands':commands,'selected_dso_imports':imports,
            'binary_sha256':{x.relative_to(out).as_posix():sha(x) for x in [exe,*dsos]},'wrapper_cmake':cm,'build_stdout':logs,'native_wired':False,
            'scope':'Adam four-call algorithm adapted to current single VM and source owned save/table APIs. Actual class/COUNT/selected save backing, live difficulty/save mutation, required errors and same VM checked. No grant/profile lifecycle/new faery store/native skill activation.'}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'host':host,'original_caller_cases':original['caller_comparisons'],'nested_cases':original['nested_original_comparisons'],'instructions':len(original['caller_instruction_addresses']),'mismatches':0}))
if __name__=='__main__':main()
