"""Reused equipped-faery caller, actual canonical rows and shared Lua/owner gate."""
from __future__ import annotations
import argparse,hashlib,itertools,json,os,random,re,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-equipped-faery-element-v1/original-functions.json'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def run(command,env=None):
    p=subprocess.run(list(map(str,command)),cwd=ROOT,env=env,capture_output=True,text=True)
    if p.returncode:raise RuntimeError(f'{command}\nexit={p.returncode}\n{p.stdout}\n{p.stderr}')
    return p.stdout
def arm(original,exe,cache,env,manifest):
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu,i32
    from unicorn import UC_HOOK_CODE
    cpu=Cpu(original,False,manifest);C=cpu.data+0x1000;R=cpu.data+0x3000;save=cpu.data+0x4000;alt=cpu.data+0x5000;row=cpu.data+0x6000
    def put(at,n):cpu.pointer(at,n&0xffffffff)
    def returned(n):cpu.put(0,n);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
    seen=set();records=[]
    cases=list(itertools.product([0,1,4,0x7fffffff,0x80000000,0xffffffff],[0,1,4,65535,0xffffffff,0x80000000,0x7fffffff,16777217],range(2)))
    rng=random.Random(0x3b6dc4);cases.extend((rng.getrandbits(32),rng.getrandbits(32),rng.randrange(2)) for _ in range(16))
    for id,element,mut in cases:
        trace=[];pushed=[]
        def hook(_,at,__,___):
            if 0x3b6dc4<=at<0x3b6df8:seen.add(at)
            if at==0x3bb98c:
                assert cpu.reg(0)==C and cpu.reg(1)==0xffffffff
                trace.append([0,0,-1,1]);returned(id)
            elif at==0x3aeac0:
                assert cpu.reg(0)==C and cpu.reg(1)==id
                trace.append([1,id,-1,1]);put(row+8,element);returned(row)
            elif at==0x37cb24:
                assert cpu.reg(0)==R and cpu.reg(1)==element;pushed.append(i32(cpu.reg(1)));returned(0)
        h=cpu.uc.hook_add(UC_HOOK_CODE,hook)
        try:cpu.invoke(0x3b6dc4,[1,R,C])
        finally:cpu.uc.hook_del(h)
        expected={'status':1,'selected':id,'element':i32(element),'calls':2,'complete':1,'trace':trace}
        actual=json.loads(run([exe,'--oracle',id,element,mut],env));assert actual==expected,(id,element,mut,expected,actual)
        assert pushed==[i32(element)];records.append({'input':[id,element,mut],'original':expected,'compiled':actual})
    assert seen==set(range(0x3b6dc4,0x3b6df8,4))
    view=json.loads(run([exe,cache,'--view'],env));lists=cpu.data+0x8000;members=cpu.data+0x10000;rows=cpu.data+0x18000;member_at=members
    for i,ids in enumerate(view['lists']):
        cpu.uc.mem_write(lists+12*i,struct.pack('<III',0,len(ids),member_at))
        if ids:cpu.uc.mem_write(member_at,struct.pack('<'+'i'*len(ids),*ids));member_at+=4*len(ids)
    raw_rows=[w for row_ in view['rows'] for w in row_];cpu.uc.mem_write(rows,struct.pack('<'+'I'*len(raw_rows),*raw_rows))
    for symbol,value in [('_ZN6Arrays14FaeryListTable7membersE',lists),('_ZN6Arrays14FaeryListTable4sizeE',len(view['lists'])),('_ZN6Arrays10FaeryTable7membersE',rows),('_ZN6Arrays10FaeryTable4sizeE',len(view['rows'])),('gAssertLevel',0)]:put(cpu.symbols[symbol],value)
    difficulty=cpu.symbols['_ZN14PlayerSavegame17m_difficultyLevelE'];assert difficulty==0x9a6060
    nested=[];nested_seen=set();count_target=cpu.symbols['_ZNK15PyDataConstants11getConstantEPKcS1_']
    def cstring(at):return bytes(cpu.uc.mem_read(at,64)).split(b'\0')[0].decode()
    for class_id,diff,id,mut in itertools.product(range(3),range(3),range(5),range(4)):
        cpu.uc.mem_write(C,bytes(0x1600));list_id=view['selectors'][class_id];put(C+0x106c,list_id);put(C+0x14e8,save);put(difficulty,diff)
        for owner,current in [(save,id),(alt,0)]:
            cpu.uc.mem_write(owner,bytes(0x200))
            for d in range(3):put(owner+0xac+4*d,current)
        queries=[0];push=[];calls=[]
        def hook(_,at,__,___):
            if at in cpu.address_owner:nested_seen.add(at)
            if at in (0x3bb98c,0x3aeac0):calls.append(at);assert cpu.reg(0)==C
            if at==count_target:
                assert cstring(cpu.reg(1))=='FaeryTypes' and cstring(cpu.reg(2))=='COUNT';queries[0]+=1
                if queries[0]==1:
                    if mut==1:put(difficulty,1)
                    if mut==2:put(C+0x14e8,alt)
                    if mut==3:put(C+0x14e8,0)
                returned(view['count'])
            elif at==0x37cb24:assert cpu.reg(0)==R;push.append(i32(cpu.reg(1)));returned(0)
        h=cpu.uc.hook_add(UC_HOOK_CODE,hook)
        try:cpu.invoke(0x3b6dc4,[1,R,C])
        finally:cpu.uc.hook_del(h)
        element=i32(view['rows'][view['lists'][list_id][id]][2])
        expected={'status':1,'selected':id,'element':element,'calls':2,'queries':2,'complete':1}
        assert push==[element] and calls==[0x3bb98c,0x3aeac0]
        actual=json.loads(run([exe,cache,'--borrowed',class_id,diff,id,mut],env));assert actual==expected,(class_id,diff,id,mut,expected,actual)
        nested.append({'input':[class_id,diff,id,mut],'original':expected,'compiled':actual})
    return {'validation':'PASS','caller_comparisons':len(records),'caller_cases':records,'caller_instruction_addresses':[hex(x) for x in sorted(seen)],
            'nested_original_comparisons':len(nested),'nested_cases':nested,'nested_instruction_addresses':[hex(x) for x in sorted(nested_seen)],'actual_cache_view':view,'mismatches':0,
            'scope':'Complete52B callback, all13 instructions. SG_Current(-1)/GetCharFaery/GetCharFaeryListId execute in nested comparisons with actual retained cache rows; COUNT and pushInteger are named providers. Difficulty/save mutation after ID capture cannot replace that ID. No dependency-body credit.'}
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler',type=Path,required=True);p.add_argument('--cache',type=Path,required=True);p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--report',type=Path,default=MODULE/'build/character-equipped-faery-element-v1/validation.json');a=p.parse_args()
    out=a.output.resolve();wrapper=out/'wrapper';build=out/'selected';wrapper.mkdir(parents=True,exist_ok=True);build.mkdir(exist_ok=True)
    cxx=a.compiler.resolve();cc=cxx.with_name(cxx.name.replace('g++','gcc'));source=MODULE/'character_equipped_faery_element_v1.cpp';test=MODULE/'tests/character_equipped_faery_element_v1.cpp';constants=ROOT/'port/pydata-constants/constants.c'
    cm=f'''cmake_minimum_required(VERSION 3.22)
project(equipped_faery_audit LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-world)
add_executable(equipped_faery_audit "{test.as_posix()}" "{constants.as_posix()}")
target_compile_features(equipped_faery_audit PRIVATE cxx_std_17)
target_compile_options(equipped_faery_audit PRIVATE -Wall -Wextra -Werror -pedantic -fno-fast-math -ffp-contract=off)
target_link_libraries(equipped_faery_audit PRIVATE dh2_level_world dh2_script_runtime)
'''
    (wrapper/'CMakeLists.txt').write_text(cm,encoding='utf-8');cmake=shutil.which('cmake');ninja=shutil.which('ninja')
    logs=[run([cmake,'-S',wrapper,'-B',build,'-G','Ninja',f'-DCMAKE_MAKE_PROGRAM={ninja}',f'-DCMAKE_CXX_COMPILER={cxx.as_posix()}',f'-DCMAKE_C_COMPILER={cc.as_posix()}','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_FLAGS_RELEASE=-O1','-DCMAKE_C_FLAGS_RELEASE=-O1'])]
    commands=run([ninja,'-C',build,'-t','commands','equipped_faery_audit']);db=json.loads((build/'compile_commands.json').read_text());sources={Path(x['file']).resolve() for x in db if str(x['file']).replace('\\','/') in commands.replace('\\','/')}
    assert source in sources and MODULE/'character_current_spell_v1.cpp' in sources and ROOT/'port/adam-script-runtime/script_runtime.c' in sources
    source_commands=[x['command'].replace('\\','/') for x in db if Path(x['file']).resolve()==source]
    assert len(source_commands)==1 and 'dh2_level_world.dir' in source_commands[0] and 'equipped_faery_audit.dir' not in source_commands[0]
    paths=sources|{Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md'),MODULE/'tests/character_current_spell_v1.cpp',MODULE/'tests/run_character_ai_set_skills_and_spells_host.py',ROOT/'port/engine-resources/tests/cpu.py'}
    for folder in {x.parent for x in sources}:
        paths.update(folder.glob('*.hpp'));paths.update(folder.glob('*.h'))
        if (folder/'CMakeLists.txt').exists():paths.add(folder/'CMakeLists.txt')
    before={x.relative_to(ROOT).as_posix():sha(x) for x in paths if x.is_relative_to(ROOT)};translation_units={x.relative_to(ROOT).as_posix():sha(x) for x in sources if x.is_relative_to(ROOT)}
    cache=a.cache.resolve();cache_files=[cache/'data/pydata'/(stem+suffix+'.bin') for stem in ['skills','faeries','character_properties','character_classes'] for suffix in ['_pyarray','_pyarraynames','_pystructnames']]+[cache/'data/pydata/faeries_pycst.bin'];cache_hash={x.relative_to(cache).as_posix():sha(x) for x in cache_files}
    manifest=json.loads(MANIFEST.read_text());sys.path.insert(0,str(MODULE/'tests'));from run_character_ai_set_skills_and_spells_host import source_symbols
    source_symbols(a.original_elf.resolve(),manifest)
    logs.append(run([cmake,'--build',build,'--target','equipped_faery_audit','--parallel','1']));assert all(sha(ROOT/n)==h for n,h in before.items()),'Source changed during build'
    exe=build/'equipped_faery_audit.exe';dsos=sorted(build.rglob('*.dll'));env=os.environ.copy();env['PATH']=os.pathsep.join([str(cxx.parent),*(str(x.parent) for x in dsos),env.get('PATH','')])
    host=json.loads(run([exe,cache],env));assert host['validation']=='PASS' and host['real_lua_cases']==5
    original=arm(a.original_elf.resolve(),exe,cache,env,manifest)
    import_text={x.relative_to(out).as_posix():run([cxx.with_name('objdump.exe'),'-p',x]) for x in [exe,*dsos]};imports={n:re.findall(r'DLL Name: (\S+)',t) for n,t in import_text.items()}
    exe_text=import_text[exe.relative_to(out).as_posix()];assert 'libdh2_level_world.dll' in imports[exe.relative_to(out).as_posix()] and 'saved_services' in exe_text and 'equipped_faery_element_v1' in exe_text and sum(x.name=='libdh2_script_runtime.dll' for x in dsos)==1
    world=next(x for x in dsos if x.name=='libdh2_level_world.dll')
    assert 'equipped_faery_element_v1' in import_text[world.relative_to(out).as_posix()]
    assert all(sha(ROOT/n)==h for n,h in before.items()) and all(sha(cache/n)==h for n,h in cache_hash.items())
    report={'validation':'PASS','host_report':host,'original_arm_comparison':original,'source_sha256':before,'compile_translation_units_sha256':translation_units,'source_pin_scope':'Adjacent headers are conservative environment guards, not Android compiler inputs.',
            'cache_inputs_sha256':cache_hash,'original_sha256':sha(a.original_elf),'attribution':manifest['attribution'],'selected_commands':commands,'selected_dso_imports':imports,'binary_sha256':{x.relative_to(out).as_posix():sha(x) for x in [exe,*dsos]},'wrapper_cmake':cm,'build_stdout':logs,
            'new_complete_original_callers':1,'new_complete_dependency_bodies':0,'native_wired':False,'scope':'Adapted Adam caller imported from actual selected world DLL, using current shared save/table/Lua APIs. No executable implementation replacement or second VM/save/property/timer owner; no full faery buff lifecycle/native activation.'}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({'host':host,'caller_comparisons':original['caller_comparisons'],'nested_comparisons':original['nested_original_comparisons'],'mismatches':0}))
if __name__=='__main__':main()
