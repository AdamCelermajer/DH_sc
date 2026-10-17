"""Selected source skill dispatch; original ARM caller branch comparisons."""
from __future__ import annotations
import argparse,hashlib,json,os,re,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-ai-skill-dispatch-v1/original-functions.json'
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def run(command,env=None):
    p=subprocess.run(list(map(str,command)),cwd=ROOT,env=env,capture_output=True,text=True)
    if p.returncode:raise RuntimeError(f'{command}\nexit={p.returncode}\n{p.stdout}\n{p.stderr}')
    return p.stdout
def verify(original):
    from elftools.elf.elffile import ELFFile
    manifest=json.loads(MANIFEST.read_text());assert sha(original)==manifest['original_sha256'];raw=original.read_bytes()
    with original.open('rb') as f:
        elf=ELFFile(f);symbols={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()};loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        for row in manifest['functions']:
            at,n=int(row['elf_address'],0),row['size'];s=symbols[row['original_symbol']];assert (s['st_value'],s['st_size'])==(at,n)
            load=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz']);off=int(load['p_offset'])+at-int(load['p_vaddr']);assert hashlib.sha256(raw[off:off+n]).hexdigest()==row['sha256']
    return manifest
def oracle(original,exe,env,manifest):
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    sys.path.insert(0,str(MODULE/'tests'));from elf_import_identity import verify_imports
    imports=verify_imports(original,{0x30e004:'fprintf'})
    cpu=Cpu(original,False,manifest);ai=cpu.data+0x1000;a=cpu.data+0x2000;b=cpu.data+0x3000;slots=cpu.data+0x4000;alt=cpu.data+0x5000
    entries=[0x3cb458,0x3d8358,0x3d808c,0x3d8bf8,0x3d8b7c]
    records=[];instructions=set();ranges=manifest['functions'][:5]
    def store(at,value):cpu.pointer(at,value)
    def returned(value):cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
    def text(at):return bytes(cpu.uc.mem_read(at,160)).split(b'\0')[0].decode()
    scenarios=[]
    for phase in [0,5,6,7,8,0xffffffff,0x80000000,0x7fffffff]:scenarios.append([0,phase,0,0,0,0,0,0,0,0,0])
    for phase in [6,7,0xffffffff]:
        for using in [0,1,0xffffffff]:
            for casting in [0,1]:
                for flags in [0,0x8000]:
                    for tail in [0,1,0xdeadbeef]:scenarios.append([1,phase,using,casting,flags,0,1,0,0,0,tail])
    for op in [1,2,3,4]:
        for slot in [0,1,2,0xffffffff]:
            for count in [0,1,3]:
                for null in [0,1]:
                    for level in ([0,1,3] if op==1 else [0]):scenarios.append([op,7,0,0,0,slot,count,null,level,0,0xffffffff])
    for mut in [1,2,6,7]:
        scenarios += [[1,6 if mut==2 else 7,1 if mut in [1,7] else 0,0,0x8000,0,1,0,0,mut,0x12345678],
                      [1,7,0,0,0,0,1,0,0,mut,0x12345678]]
    for mut in [3,8]:scenarios.append([1,7,0,0,0,2,0,0,1,mut,0x12345678])
    for op in [2,3,4]:scenarios.append([op,0,0,0,0,0,1,0,0,4,0xffffffff])
    for row in scenarios:
        op,phase,using,casting,flags,slot,count,null,level,mut,tail=row
        store(ai+4,a);store(ai+0x28,phase);store(a+0x520,flags);store(b+0x520,0x8000)
        store(ai+0xb4,slots);store(ai+0xb8,slots+count*4);store(ai+0xcc,slot);store(cpu.symbols['gAssertLevel'],level)
        for i in range(4):store(slots+i*4,0 if null else 11+i);store(alt+i*4,21+i)
        trace=[]
        def hook(_,at,__,___):
            for r in ranges:
                start=int(r['elf_address'],0)
                if start<=at<start+r['size']:instructions.add(at)
            if at==0x3c02e8:
                assert cpu.reg(0)==a+0x4fc;trace.append([0,1])
                if mut==1:store(ai+4,b)
                if mut==7:store(a+0x520,0)
                returned(using)
            elif at==0x3c0334:
                assert cpu.reg(0) in [a+0x4fc,b+0x4fc];trace.append([1,1 if cpu.reg(0)==a+0x4fc else 2])
                if mut==2:store(ai+0x28,7)
                if mut==6:store(ai+0x28,6)
                returned(casting)
            elif at==0x30e004:
                assert text(cpu.reg(1))=='ASSERT(%s) FAILED: %s:%d\n' and text(cpu.reg(2))=='skillId < m_skillScripts.size()'
                assert text(cpu.reg(3))=='..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Characters\\AI\\CharAI_Skills.cpp'
                assert struct.unpack('<I',cpu.uc.mem_read(cpu.uc.reg_read(cpu.sp),4))[0]==181
                trace.append([6,9])
                if mut in [3,8]:
                    store(ai+0xb4,alt);store(ai+0xb8,alt+12)
                    if mut==8:store(alt+8,0)
                returned(0xdeadffff)
            elif at in [0x3da9dc,0x3da8b8,0x3da794,0x3da6c0]:
                service={0x3da9dc:2,0x3da8b8:3,0x3da794:4,0x3da6c0:5}[at]
                trace.append([service,cpu.reg(0)])
                if mut==4:store(ai+0xcc,3);store(ai+0xb4,alt);store(ai+0xb8,alt+16)
                returned(tail)
        h=cpu.uc.hook_add(UC_HOOK_CODE,hook)
        try:value=cpu.invoke(entries[op],[ai,slot])
        finally:cpu.uc.hook_del(h)
        expected={'status':0,'value':value if op in [0,1] else 0,'trace':trace}
        actual=json.loads(run([exe,'--oracle',*row],env));assert expected==actual,(row,expected,actual)
        records.append({'input':row,'original':expected,'compiled':actual})
    covered={hex(at) for at in instructions};required={hex(at) for r in [ranges[0],*ranges[2:]] for at in range(int(r['elf_address'],0),int(r['elf_address'],0)+r['size'],4)}
    assert required<=covered
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'instruction_addresses':sorted(covered,key=lambda s:int(s,0)),
            'verified_imports':imports,'results':records,'scope':'Actual five callers; UsingSkill/Casting, inner skill callbacks and fprintf are mandatory modeled caller boundaries. Four small bodies all instructions covered; Usable fatal level2 null-store excluded. No CSSkill/BeginSkill or Player loading publication is executed.'}
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler',type=Path,required=True);p.add_argument('--cache',type=Path,required=True);p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--report',type=Path,default=MODULE/'build/character-ai-skill-dispatch-v1/validation.json');a=p.parse_args()
    out=a.output.resolve();wrapper=out/'wrapper';build=out/'selected';wrapper.mkdir(parents=True,exist_ok=True);build.mkdir(exist_ok=True);cxx=a.compiler.resolve();cc=cxx.with_name(cxx.name.replace('g++','gcc'));cmake=shutil.which('cmake');ninja=shutil.which('ninja')
    test=MODULE/'tests/character_ai_skill_dispatch_v1.cpp';backend=ROOT/'port/android-native/app/src/main/cpp/native_debug_files.cpp';names=ROOT/'port/pydata-names/names.c'
    cm=f'''cmake_minimum_required(VERSION 3.22)
project(dispatch_selected LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-world)
add_executable(dispatch_audit "{test.as_posix()}" "{backend.as_posix()}" "{names.as_posix()}")
target_compile_features(dispatch_audit PRIVATE cxx_std_17)
target_compile_options(dispatch_audit PRIVATE -Wall -Wextra -Werror -pedantic -fno-fast-math -ffp-contract=off)
target_link_libraries(dispatch_audit PRIVATE dh2_level_world dh2_script_runtime)
'''
    (wrapper/'CMakeLists.txt').write_text(cm,encoding='utf-8')
    logs=[run([cmake,'-S',wrapper,'-B',build,'-G','Ninja',f'-DCMAKE_MAKE_PROGRAM={ninja}',f'-DCMAKE_CXX_COMPILER={cxx}',f'-DCMAKE_C_COMPILER={cc}','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_FLAGS_RELEASE=-O1','-DCMAKE_C_FLAGS_RELEASE=-O1'])]
    commands=run([ninja,'-C',build,'-t','commands','dispatch_audit']);db=json.loads((build/'compile_commands.json').read_text());sources={Path(x['file']).resolve() for x in db if str(x['file']).replace('\\','/') in commands.replace('\\','/')}
    assert MODULE/'character_ai_skill_dispatch_v1.cpp' in sources and ROOT/'port/adam-script-runtime/script_runtime.c' in sources
    assert not any(x.name=='player_skill_vm_services.cpp' for x in sources)
    fixtures=[MODULE/'tests/player_skill_use_session_v1.cpp',MODULE/'tests/player_skill_session_v1.cpp'];paths=sources|set(fixtures)|{Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md'),MODULE/'tests/elf_import_identity.py',ROOT/'port/engine-resources/tests/cpu.py'}
    for folder in {x.parent for x in sources}:
        paths.update(folder.glob('*.h'));paths.update(folder.glob('*.hpp'))
        if (folder/'CMakeLists.txt').exists():paths.add(folder/'CMakeLists.txt')
    before={x.relative_to(ROOT).as_posix():sha(x) for x in paths if x.is_relative_to(ROOT)};manifest=verify(a.original_elf.resolve())
    prior=json.loads((MODULE/'build/player-skill-session-v1/validation.json').read_text());inputs=prior['cache_inputs_sha256'];assert all(sha(a.cache/name)==h for name,h in inputs.items())
    logs.append(run([cmake,'--build',build,'--target','dispatch_audit','--parallel','1']));assert all(sha(ROOT/name)==h for name,h in before.items()),'Sources changed during compilation'
    dsos=sorted(build.rglob('*.dll'));exe=build/'dispatch_audit.exe';env=os.environ.copy();env['PATH']=os.pathsep.join([str(cxx.parent),*(str(x.parent) for x in dsos),env.get('PATH','')])
    imports={x.relative_to(out).as_posix():re.findall(r'DLL Name: (\S+)',run([cxx.with_name('objdump.exe'),'-p',x])) for x in [exe,*dsos]}
    world=next(x for x in dsos if x.name=='libdh2_level_world.dll');vm=next(x for x in dsos if x.name=='libdh2_script_runtime.dll')
    assert sum(x.name==vm.name for x in dsos)==1 and world.name in imports[exe.relative_to(out).as_posix()] and vm.name in imports[world.relative_to(out).as_posix()]
    host=json.loads(run([exe,a.cache.resolve(),out/'real-debug'],env));assert host['validation']=='PASS' and host['actual_script_cases']==6 and host['guards']>=15 and host['failure_cases']==16
    arm=oracle(a.original_elf.resolve(),exe,env,manifest)
    assert all(sha(ROOT/name)==h for name,h in before.items()) and all(sha(a.cache/name)==h for name,h in inputs.items())
    report={'validation':'PASS','host_report':host,'original_arm_comparison':arm,'original_sha256':sha(a.original_elf),'new_complete_original_bodies':4,
            'source_sha256':before,'cache_inputs_sha256':inputs,'selected_commands':commands,'selected_dso_imports':imports,'binary_sha256':{x.relative_to(out).as_posix():sha(x) for x in [exe,*dsos]},'wrapper_cmake':cm,'build_stdout':logs,
            'scope':'Borrowed source gates and callbacks over current selected FSM/Owner/Use/sole Player VM. Actual passive check/nil-target use plus required mana/target-search failures. No published Player AIS lifecycle, CSSkill state activation, animation timing or native wiring.'}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({'host':host,'original_cases':arm['comparisons'],'instructions':len(arm['instruction_addresses']),'mismatches':0}))
if __name__=='__main__':main()
