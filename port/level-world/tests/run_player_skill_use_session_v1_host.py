"""Same selected Player VM: original skill checks and bounded use callbacks."""
from __future__ import annotations
import argparse,hashlib,json,os,re,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
ORIGINAL='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
RANGES=[('_ZN17CharAISkillScript10OnPreSkillEv',0x3da8b8,292),('_ZN17CharAISkillScript7OnSkillEv',0x3da794,292),('_ZN17CharAISkillScript11OnPostSkillEv',0x3da6c0,212)]
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def run(command,env=None):
    p=subprocess.run(list(map(str,command)),cwd=ROOT,env=env,capture_output=True,text=True)
    if p.returncode:raise RuntimeError(f'{command}\nexit={p.returncode}\n{p.stdout}\n{p.stderr}')
    return p.stdout
def evidence(original):
    from elftools.elf.elffile import ELFFile
    assert sha(original)==ORIGINAL
    raw=original.read_bytes();rows=[]
    with original.open('rb') as f:
        elf=ELFFile(f);symbols={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()};loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        for name,address,size in RANGES:
            symbol=symbols[name];assert (symbol['st_value'],symbol['st_size'])==(address,size)
            load=next(s for s in loads if s['p_vaddr']<=address and address+size<=s['p_vaddr']+s['p_filesz']);off=int(load['p_offset'])+address-int(load['p_vaddr'])
            rows.append({'original_symbol':name,'elf_address':hex(address),'size':size,'sha256':hashlib.sha256(raw[off:off+size]).hexdigest(),'scope':'borrowed same-VM adapter control; no new complete-body credit'})
    return rows
def oracle(original,exe,cache,temp,env,rows):
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    sys.path.insert(0,str(MODULE/'tests'));from elf_import_identity import verify_imports
    imports=verify_imports(original,{0x30df8c:'__aeabi_fcmpeq',0x30e310:'__stack_chk_fail'})
    class FloatCpu(Cpu):
        def external(self,uc,address,size,unused):
            if self.imports.get(address)=='__aeabi_fcmpeq':
                self.import_calls['__aeabi_fcmpeq']=self.import_calls.get('__aeabi_fcmpeq',0)+1
                a,b=(struct.unpack('<f',struct.pack('<I',self.reg(i)))[0] for i in range(2));self.put(0,int(a==b));uc.reg_write(self.pc,uc.reg_read(self.lr))
            else:super().external(uc,address,size,unused)
    cpu=FloatCpu(original,False,{'original_sha256':ORIGINAL,'functions':rows})
    skill=cpu.data+0x1000;owner=cpu.data+0x2000;script=cpu.data+0x3000;vector=cpu.data+0x4000;data=cpu.data+0x5000;string=cpu.data+0x6000;lua=cpu.data+0x7000
    instructions=set();records=[]
    def word(at):return struct.unpack('<I',cpu.uc.mem_read(at,4))[0]
    def store(at,value):cpu.pointer(at,value)
    def returned(value):cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
    def text(at):return bytes(cpu.uc.mem_read(at,48)).split(b'\0')[0].decode()
    profiles=[(0,0),(1,0),(1,0x3f800000),(3,0),(3,0x80000000),(3,0x7fc00000),(4,0),(4,0)]
    scenarios=[]
    for phase in range(3):
        for set_error in (0,7):
            for set_count in (0,2):
                for call_error in (0,9):
                    for count in (0,1,2):scenarios.append((phase,set_error,set_count,call_error,count,2))
        for typ in range(8):scenarios.append((phase,0,2,0,1,typ))
    for phase,set_error,set_count,call_error,count,typ in scenarios:
        store(skill+4,owner);store(owner+0x3e4,script);store(vector,data);store(vector+4,data);cpu.uc.mem_write(string,b'\0' if typ==6 else b'0\0')
        state={'temp':None,'calls':0,'erased':0,'destroyed':0}
        def fill(n,final):
            store(vector,data);store(vector+4,data+n*0x70)
            for i in range(n):
                cpu.uc.mem_write(data+i*0x70,bytes(0x70));store(data+i*0x70+4,3);store(data+i*0x70+8,0x3f800000)
            if final and n:
                tag,bits=profiles[typ];store(data+4,tag);store(data+8,bits);store(data+0x20,string)
        def observe(_,at,__,___):
            for _,address,size in RANGES:
                if address<=at<address+size:instructions.add(at)
            if at==0x31b434:
                state['temp']=cpu.reg(0);store(state['temp']+8,0);store(state['temp']+0x24,vector);returned(0xffffffff)
            elif at==0x37c390:
                assert cpu.reg(0)==script and cpu.reg(2)==skill+0xc and cpu.reg(3)==state['temp'] and text(cpu.reg(1))=='SetSkill'
                state['calls']+=1;store(state['temp']+8,set_error);fill(set_count,False);returned(0xffffffff)
            elif at==0x31c3cc:
                assert cpu.reg(0)==vector and cpu.reg(1)==data and cpu.reg(2)==data+set_count*0x70
                state['erased']=1;fill(0,False);returned(0xffffffff)
            elif at==0x37c494:
                assert cpu.reg(0)==script and cpu.reg(2)==state['temp'] and text(cpu.reg(1))==('OnPreSkill','OnSkill','OnPostSkill')[phase]
                state['calls']+=1;store(state['temp']+8,call_error);fill(count,True);returned(0xffffffff)
            elif at==0x84c7e0:returned(lua)
            elif at==0x84c04c:assert cpu.reg(0)==lua and cpu.reg(1)==string;returned(0xffffffff)
            elif at==0x84b320:assert cpu.reg(0)==lua and cpu.reg(1)==0xffffffff;returned(1)
            elif at==0x85797c:assert cpu.reg(0)==lua;returned(0xffffffff)
            elif at==0x31b398:assert cpu.reg(0)==state['temp'];state['destroyed']=1;fill(0,False);returned(0xdeadffff)
        hook=cpu.uc.hook_add(UC_HOOK_CODE,observe)
        try:value=cpu.invoke(RANGES[phase][1],[skill])
        finally:cpu.uc.hook_del(hook)
        expected={'value':0 if phase==2 else value,'calls':state['calls'],'erased':state['erased'],'destroyed':state['destroyed'],'retained':0}
        actual=json.loads(run([exe,'--oracle',cache,temp,phase,int(bool(set_error)),set_count,int(bool(call_error)),count,typ],env));assert actual==expected,(phase,set_error,set_count,call_error,count,typ,expected,actual)
        records.append({'input':[phase,set_error,set_count,call_error,count,typ],'original_result':expected,'compiled_result':actual})
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'instructions_observed':[hex(x) for x in sorted(instructions)],'verified_imports':imports,'imports_executed':cpu.import_calls,'results':records,'scope':'Retained nonnull owner/script Pre/Use/Post paths only; source resource/Call/erase/destructor and four string-Lua dependencies are explicit fixtures. Actual original getBool/index instructions execute. Post void return is ignored. Null script/owner replacement/canary traps unclaimed; zero new complete-body credit.'}
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler',required=True,type=Path);p.add_argument('--cache',required=True,type=Path);p.add_argument('--original-elf',required=True,type=Path);p.add_argument('--output',required=True,type=Path);p.add_argument('--report',type=Path,default=MODULE/'build/player-skill-use-session-v1/validation.json');a=p.parse_args()
    out=a.output.resolve();wrapper=out/'wrapper';build=out/'selected';wrapper.mkdir(parents=True,exist_ok=True);build.mkdir(exist_ok=True);cache=a.cache.resolve();cxx=a.compiler.resolve();cc=cxx.with_name(cxx.name.replace('g++','gcc'));cmake=shutil.which('cmake');ninja=shutil.which('ninja')
    source=MODULE/'player_skill_use_session_v1.cpp';test=MODULE/'tests/player_skill_use_session_v1.cpp';fixture=MODULE/'tests/player_skill_session_v1.cpp';backend=ROOT/'port/android-native/app/src/main/cpp/native_debug_files.cpp';names=ROOT/'port/pydata-names/names.c'
    wrapper_text=f'''cmake_minimum_required(VERSION 3.22)
project(player_use_selected LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-world)
add_executable(player_use_audit "{test.as_posix()}" "{backend.as_posix()}" "{names.as_posix()}")
target_compile_features(player_use_audit PRIVATE cxx_std_17)
target_compile_options(player_use_audit PRIVATE -Wall -Wextra -Werror -pedantic -fno-fast-math -ffp-contract=off)
target_link_libraries(player_use_audit PRIVATE dh2_level_world dh2_script_runtime)
'''
    (wrapper/'CMakeLists.txt').write_text(wrapper_text,encoding='utf-8')
    logs=[run([cmake,'-S',wrapper,'-B',build,'-G','Ninja',f'-DCMAKE_MAKE_PROGRAM={ninja}',f'-DCMAKE_CXX_COMPILER={cxx}',f'-DCMAKE_C_COMPILER={cc}','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_FLAGS_RELEASE=-O1','-DCMAKE_C_FLAGS_RELEASE=-O1'])]
    commands=run([ninja,'-C',build,'-t','commands','player_use_audit']);db=json.loads((build/'compile_commands.json').read_text());sources={Path(x['file']).resolve() for x in db if str(x['file']).replace('\\','/') in commands.replace('\\','/')}
    assert source.resolve() in sources,'Parent must select use session in current level-world library'
    assert ROOT/'port/adam-script-runtime/script_runtime.c' in sources
    assert not any(x.name=='player_skill_vm_services.cpp' for x in sources)
    paths=sources|{fixture,Path(__file__).resolve(),MODULE/'tests/elf_import_identity.py',ROOT/'port/engine-resources/tests/cpu.py'}
    for folder in {x.parent for x in sources}:
        paths.update(folder.glob('*.h'));paths.update(folder.glob('*.hpp'))
        if (folder/'CMakeLists.txt').exists():paths.add(folder/'CMakeLists.txt')
    before={x.relative_to(ROOT).as_posix():sha(x) for x in paths if x.is_relative_to(ROOT)}
    prior=json.loads((MODULE/'build/player-skill-session-v1/validation.json').read_text());inputs=prior['cache_inputs_sha256'];assert all(sha(cache/name)==h for name,h in inputs.items())
    rows=evidence(a.original_elf.resolve());logs.append(run([cmake,'--build',build,'--target','player_use_audit','--parallel','1']))
    assert all(sha(ROOT/name)==h for name,h in before.items()),'Source changed during build'
    dsos=sorted(build.rglob('*.dll'));exe=build/'player_use_audit.exe';env=os.environ.copy();env['PATH']=os.pathsep.join([str(cxx.parent),*(str(x.parent) for x in dsos),env.get('PATH','')])
    imports={x.relative_to(out).as_posix():re.findall(r'DLL Name: (\S+)',run([cxx.with_name('objdump.exe'),'-p',x])) for x in [exe,*dsos]}
    world=next(x for x in dsos if x.name=='libdh2_level_world.dll');vm=next(x for x in dsos if x.name=='libdh2_script_runtime.dll')
    assert sum(x.name==vm.name for x in dsos)==1 and world.name in imports[exe.relative_to(out).as_posix()] and vm.name in imports[world.relative_to(out).as_posix()]
    host=json.loads(run([exe,cache,out/'real-debug'],env));assert host['validation']=='PASS' and host['actual_script_cases']==6 and host['guards']>=9
    arm=oracle(a.original_elf.resolve(),exe,cache,out/'original-debug',env,rows)
    assert all(sha(ROOT/name)==h for name,h in before.items()) and all(sha(cache/name)==h for name,h in inputs.items())
    report={'validation':'PASS','host_report':host,'original_comparison':arm,'original_sha256':sha(a.original_elf),'original_ranges':rows,'new_complete_original_bodies':0,'attribution':{'commit':'c3ae797332a82a30a586b9156cddc25445e36a4c','reused':['character_skill_callbacks_v3.cpp','character_skill_callback_session_v3.cpp'],'changes':'Current selected check caller, native ReturnValues/strings, exact getBool/temporary Lua and same-VM array observer. ClearTarget now forwards to the retained CharAI target owner; mana/filter and live combat integration remain incomplete.'},'source_sha256':before,'cache_inputs_sha256':inputs,'selected_commands':commands,'selected_dso_imports':imports,'binary_sha256':{x.relative_to(out).as_posix():sha(x) for x in [exe,*dsos]},'wrapper_cmake':wrapper_text,'build_stdout':logs,'scope':'Actual unchanged passive two-result checks and initial Bashdown nil-target Use; Ground Slam source replay uses typed mana/target/damage probes and the source-backed ClearTarget adapter over one retained CharAI target owner. No native/FSM/complete Bashdown combat activation.'}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({'host':host,'original_cases':arm['comparisons'],'mismatches':0}))
if __name__=='__main__':main()
