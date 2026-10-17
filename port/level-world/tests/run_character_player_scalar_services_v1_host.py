"""Borrowed LuaScript dictionary callers, original ARM and retained real-Lua gate."""
from __future__ import annotations
import argparse,hashlib,itertools,json,os,re,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-player-scalar-services-v1/original-functions.json'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def run(args,env=None):
    p=subprocess.run(list(map(str,args)),cwd=ROOT,env=env,capture_output=True,text=True)
    if p.returncode:raise RuntimeError(f'{args}\nexit={p.returncode}\n{p.stdout}\n{p.stderr}')
    return p.stdout
def arm(original,exe,env,manifest):
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu,i32
    sys.path.insert(0,str(MODULE/'tests'));from elf_import_identity import verify_imports
    from unicorn import UC_HOOK_CODE
    imports=verify_imports(original,{0x30e4cc:'__aeabi_f2iz',0x30de54:'strlen',0x30e76c:'__cxa_guard_acquire',0x30ea3c:'__cxa_guard_release',0x30e304:'__aeabi_atexit'})
    class ScalarCpu(Cpu):
        # Reused guard/import contracts from component_applicator_differential.
        # HashString itself executes; temporary string allocation/copy/free is
        # a named dependency, not a replacement hash or new allocator claim.
        def external(self,uc,address,size,unused):
            name=self.imports.get(address)
            if name=='__cxa_guard_acquire':self.put(0,1)
            elif name=='__cxa_guard_release':self.pointer(self.reg(0),1)
            elif name=='__aeabi_atexit':self.put(0,0)
            elif name=='strlen':
                n=0
                while uc.mem_read(self.reg(0)+n,1)[0]:
                    n+=1;assert n<4096
                self.put(0,n)
            else:return super().external(uc,address,size,unused)
            self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
    cpu=ScalarCpu(original,False,manifest);S=cpu.data+0x1000;A=cpu.data+0x3000;V=cpu.data+0x4000;W=cpu.data+0x6000;R=cpu.data+0x8000;N=cpu.data+0x9000;T=cpu.data+0xa000
    allocation_cursor=[cpu.data+0x100000];allocations={};allocation_counts=[0,0]
    def hash_storage(_,at,__,___):
        if at==0x3116e8:
            dest,begin,end=[cpu.reg(i) for i in range(3)];n=end-begin;assert 0<=n<4096
            address=allocation_cursor[0];allocation_cursor[0]+=(n+16)&~15
            allocations[address]=n;allocation_counts[0]+=1
            cpu.uc.mem_write(address,bytes(cpu.uc.mem_read(begin,n))+b'\0')
            cpu.uc.mem_write(dest,bytes(24));cpu.pointer(dest,address+n)
            cpu.pointer(dest+16,address+n);cpu.pointer(dest+20,address)
            cpu.put(0,dest);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
        elif at in (0x708f00,0x310440):
            assert cpu.reg(0) in allocations
            del allocations[cpu.reg(0)];allocation_counts[1]+=1
            cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
    cpu.uc.hook_add(UC_HOOK_CODE,hash_storage)
    name=b'Faery_Cooldown';cpu.uc.mem_write(N,name+b'\0')
    key=cpu.invoke(0x37c164,[N]);assert key not in [0,0xffffffff]
    def put(at,w):cpu.pointer(at,w&0xffffffff)
    def word(at):return struct.unpack('<I',cpu.uc.mem_read(at,4))[0]
    def returned(w):cpu.put(0,w);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
    records=[];seen=set();assert_skipped={*range(0x37defc,0x37df24,4)}
    fwords=[0,0x80000000,0x3fe00000,0xbfe00000,0x4b800001,0xcf000000,0x4effffff]
    cases=list(itertools.product(range(2),[0,1,2,3],fwords,range(4),range(2)))
    # Source constants and original helper integer conversion are independently
    # pinned; only coercion/map insertion/ReturnValues boundaries are modeled.
    for is_set,count,bits,layout,mutate in cases:
        converted=struct.unpack('<f',struct.pack('<I',bits^(0x80000000 if mutate else 0)))[0]
        if is_set and count>=2 and not -2147483648<=converted<2147483648:continue
        initial=[0,0xffffffff,0x80000000,0x7fffffff][layout]
        entries={key-1:41,key+1:43} if layout&2 else {}
        if layout&1:entries[key]=initial
        nodes={};heap=[T];events=[];hashes=[0];pushed=[]
        def tree():
            order=sorted(entries)
            def build(ids,parent):
                if not ids:return 0
                middle=len(ids)//2;k=ids[middle]
                if k not in nodes:nodes[k]=heap[0];heap[0]+=32
                at=nodes[k];put(at,0);put(at+4,parent);put(at+16,k);put(at+20,entries[k])
                put(at+8,build(ids[:middle],at));put(at+12,build(ids[middle+1:],at));return at
            root=build(order,S+0x1c);put(S+0x20,root);put(S+0x24,nodes[order[0]] if order else S+0x1c);put(S+0x28,nodes[order[-1]] if order else S+0x1c);put(S+0x2c,len(order))
        cpu.uc.mem_write(S,bytes(0x200));tree();put(A+4,A+0x100);put(A+0x100,V);put(A+0x104,V+count*112)
        cpu.uc.mem_write(V,bytes(4*112));cpu.uc.mem_write(W,bytes(4*112));put(V+112+8,bits);put(W+112+8,bits^0x80000000)
        def hook(_,at,__,___):
            if any(begin<=at<end for begin,end in [(0x37da30,0x37dac4),(0x37d990,0x37da30),(0x37ec14,0x37ec70),(0x37de5c,0x37df30)]):seen.add(at)
            if at==0x37c164:hashes[0]+=1;assert cpu.reg(0)==N
            elif at==0x31c49c:
                assert cpu.reg(0)==V;events.append('string')
                if mutate:put(A+0x100,W)
                returned(N)
            elif at==0x31bbf0:
                assert cpu.reg(0)==(W if mutate else V)+112;events.append('number');returned(bits^(0x80000000 if mutate else 0))
            elif at==0x30e4cc:
                value=struct.unpack('<f',struct.pack('<I',cpu.reg(0)))[0];returned(int(value))
            elif at==0x37d61c:
                # std::_Rb_tree insertion is an explicit bounded provider. The
                # complete GetInt/SetInt caller tree searches execute unchanged.
                out,header,hint,pair=[cpu.reg(i) for i in range(4)];assert header==S+0x1c
                k=word(pair);assert word(pair+4)==0 and k==key and k not in entries
                entries[k]=0;tree();put(out,nodes[k]);returned(out)
            elif at==0x37cb24:
                assert cpu.reg(0)==R;pushed.append(i32(cpu.reg(1)));returned(0)
        h=cpu.uc.hook_add(UC_HOOK_CODE,hook)
        try:cpu.invoke(0x37de5c if is_set else 0x37ec14,[A,R,S])
        finally:cpu.uc.hook_del(h)
        final={k:i32(word(at+20)) for k,at in nodes.items()}
        value=0 if is_set or not count else pushed[0]
        expected={'status':0,'value':(int(struct.unpack('<f',struct.pack('<I',bits^(0x80000000 if mutate else 0)))[0]) if is_set and count>=2 else value),
                  'returned':0 if is_set or not count else 1,'size':word(S+0x2c),'entry':final.get(key,0),'hash_reads':hashes[0]}
        actual=json.loads(run([exe,'--oracle',is_set,count,bits,initial,layout,mutate],env))
        assert actual==expected,(is_set,count,bits,layout,mutate,expected,actual)
        assert events==([] if (is_set and count<2) or (not is_set and not count) else ['string','number'] if is_set else ['string'])
        records.append({'input':[is_set,count,bits,initial,layout,mutate],'original':expected,'compiled':actual})
    executable_get=set(range(0x37ec14,0x37ec70,4));assert executable_get<=seen
    get=set(range(0x37da30,0x37dac4,4));set_=set(range(0x37d990,0x37da30,4));assert get<=seen and set_<=seen
    set_wrapper=set(range(0x37de5c,0x37df28,4))-assert_skipped;assert set_wrapper<=seen
    assert not allocations and allocation_counts[0]==allocation_counts[1]
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'cases':records,'instruction_addresses':[hex(x) for x in sorted(seen)],
            'GetInt_instruction_count':len(get),'SetInt_instruction_count':len(set_),'_GetInt_instruction_count':len(executable_get),'_SetInt_normal_instruction_count':len(set_wrapper),'excluded_set_assertion_addresses':[hex(x) for x in sorted(assert_skipped)],
            'verified_imports':imports,'hash_Faery_Cooldown':key,'hash_temporary_allocations':allocation_counts[0],'hash_temporary_frees':allocation_counts[1],
            'scope':'Original92B GetInt wrapper and148/160B map callers execute; SetInt wrapper212B normal count domain only, post-coercion shrink/assert logging excluded. Source hash executes, with explicit guard/atexit and temporary string allocation/copy/free dependencies. Value conversion, signed conversion (verified PLT), insertion allocator/tree provider and pushInteger are named boundaries; no allocator/tree/Value body credit.'}
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler',type=Path,required=True);p.add_argument('--cache',type=Path,required=True);p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--report',type=Path,default=MODULE/'build/character-player-scalar-services-v1/validation.json');a=p.parse_args()
    cxx=a.compiler.resolve();cc=cxx.with_name(cxx.name.replace('g++','gcc'));out=a.output.resolve();wrapper=out/'wrapper';build=out/'selected';wrapper.mkdir(parents=True,exist_ok=True);build.mkdir(exist_ok=True)
    source=MODULE/'character_player_scalar_services_v1.cpp';test=MODULE/'tests/character_player_scalar_services_v1.cpp';backend=ROOT/'port/android-native/app/src/main/cpp/native_debug_files.cpp';names=ROOT/'port/pydata-names/names.c'
    # The scalar implementation and its dependencies must all come from the
    # actual selected world/game-data/sole-Lua shared-library graph.
    cm=f'''cmake_minimum_required(VERSION 3.22)
project(player_scalar_audit LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-world)
add_executable(player_scalar_audit "{test.as_posix()}" "{backend.as_posix()}" "{names.as_posix()}")
target_compile_features(player_scalar_audit PRIVATE cxx_std_17)
target_compile_options(player_scalar_audit PRIVATE -Wall -Wextra -Werror -pedantic -fno-fast-math -ffp-contract=off)
target_link_libraries(player_scalar_audit PRIVATE dh2_level_world dh2_script_runtime)
'''
    (wrapper/'CMakeLists.txt').write_text(cm);cmake=shutil.which('cmake');ninja=shutil.which('ninja')
    logs=[run([cmake,'-S',wrapper,'-B',build,'-G','Ninja',f'-DCMAKE_MAKE_PROGRAM={ninja}',f'-DCMAKE_CXX_COMPILER={cxx.as_posix()}',f'-DCMAKE_C_COMPILER={cc.as_posix()}','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_FLAGS_RELEASE=-O1','-DCMAKE_C_FLAGS_RELEASE=-O1'])]
    commands=run([ninja,'-C',build,'-t','commands','player_scalar_audit']);database=json.loads((build/'compile_commands.json').read_text());sources={Path(x['file']).resolve() for x in database if str(x['file']).replace('\\','/') in commands.replace('\\','/')}
    assert ROOT/'port/adam-script-runtime/script_runtime.c' in sources and MODULE/'character_current_spell_v1.cpp' in sources and source in sources
    scalar_commands=[x for x in database if Path(x['file']).resolve()==source]
    assert len(scalar_commands)==1 and 'dh2_level_world.dir' in scalar_commands[0]['command'], 'Scalar implementation is not selected exactly once in world DLL'
    paths=sources|{Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md'),MODULE/'tests/player_skill_session_v1.cpp',MODULE/'tests/elf_import_identity.py',MODULE/'tests/run_character_ai_set_skills_and_spells_host.py',ROOT/'port/engine-resources/tests/cpu.py'}
    for folder in {x.parent for x in sources}:
        paths.update(folder.glob('*.hpp'));paths.update(folder.glob('*.h'))
        if (folder/'CMakeLists.txt').exists():paths.add(folder/'CMakeLists.txt')
    before={x.relative_to(ROOT).as_posix():sha(x) for x in paths if x.is_relative_to(ROOT)}
    compile_inputs={x.relative_to(ROOT).as_posix():sha(x) for x in sources if x.is_relative_to(ROOT)}
    cache=a.cache.resolve();cache_inputs={}
    for pattern in ['data/pydata/skills_*.bin','data/pydata/faeries_*.bin','data/pydata/character_properties_*.bin','data/pydata/character_classes_*.bin','data/pydata/effects_pyarraynames.bin','data/pydata/projectiles_pyarraynames.bin','data/pydata/design_pycst.bin','data/pydata/ai_pycst.bin','DebugSwitches.savegame']:
        for f in cache.glob(pattern):cache_inputs[f.relative_to(cache).as_posix()]=sha(f)
    prior=json.loads((MODULE/'build/player-skill-session-v1/validation.json').read_text());scripts=[n for n in prior['cache_inputs_sha256'] if 'scripts/' in n]
    for name in scripts:assert sha(cache/name)==sha(ROOT/'recovered/scripts/original'/name);cache_inputs[name]=sha(cache/name)
    manifest=json.loads(MANIFEST.read_text());sys.path.insert(0,str(MODULE/'tests'));from run_character_ai_set_skills_and_spells_host import source_symbols
    source_symbols(a.original_elf.resolve(),manifest)
    logs.append(run([cmake,'--build',build,'--target','player_scalar_audit','--parallel','1']));assert all(sha(ROOT/n)==h for n,h in before.items()),'Source changed during build'
    exe=build/'player_scalar_audit.exe';dsos=sorted(build.rglob('*.dll'));env=os.environ.copy();env['PATH']=os.pathsep.join([str(cxx.parent),*(str(x.parent) for x in dsos),env.get('PATH','')])
    imports={x.relative_to(out).as_posix():re.findall(r'DLL Name: (\S+)',run([cxx.with_name('objdump.exe'),'-p',x])) for x in [exe,*dsos]}
    assert sum(x.name=='libdh2_script_runtime.dll' for x in dsos)==1 and 'libdh2_level_world.dll' in imports[exe.relative_to(out).as_posix()] and 'libdh2_script_runtime.dll' in imports[exe.relative_to(out).as_posix()]
    executable_symbols=run([cxx.with_name('objdump.exe'),'-p',exe])
    world=next(x for x in dsos if x.name=='libdh2_level_world.dll')
    world_symbols=run([cxx.with_name('objdump.exe'),'-p',world])
    for name in ['get_int','set_int','get_callback','set_callback']:
        assert re.search(r'character_player_scalar_services_v1\d+'+name,executable_symbols) and re.search(r'character_player_scalar_services_v1\d+'+name,world_symbols), 'Selected scalar export/import missing: '+name
    host=json.loads(run([exe,cache,out/'real-debug-files'],env));assert host['validation']=='PASS' and host['actual_faery_cases']==5 and host['actual_faery_updates_completed']==4 and host['actual_faery_updates_blocked']==1 and host['real_lua_cases']==3
    original=arm(a.original_elf.resolve(),exe,env,manifest)
    assert all(sha(ROOT/n)==h for n,h in before.items()) and all(sha(cache/n)==h for n,h in cache_inputs.items())
    report={'validation':'PASS','host_report':host,'original_arm_comparison':original,'implementation_from_selected_world':True,'source_sha256':before,'compile_translation_units_sha256':compile_inputs,'source_pin_scope':'source_sha256 includes conservative adjacent header/environment guards; it is not an Android compiler dependency list. compile_translation_units_sha256 names the actual selected host translation units.',
            'cache_inputs_sha256':cache_inputs,'original_sha256':sha(a.original_elf),'selected_commands':commands,'selected_dso_imports':imports,'binary_sha256':{x.relative_to(out).as_posix():sha(x) for x in [exe,*dsos]},'wrapper_cmake':cm,'build_stdout':logs,'attribution':manifest['attribution'],'new_complete_original_tree_allocator_bodies':0,'live_native_validation':False,'scope':'Adam integer dictionary semantics adapted over borrowed same-AIS store and current VM. Implementation imported from selected world DLL; reused current game-data/Lua exports. Four unchanged actual faery update bodies reach GetInt; Celest intentionally lacks an element provider in this focused fixture. No complete13 callbacks, grants, native activation or generic default response.'}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'host':host,'original':original['comparisons'],'mismatches':0}))
if __name__=='__main__':main()
