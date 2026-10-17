"""CSSkill Focus/Blur normal callers with borrowed current services."""
from __future__ import annotations
import argparse,hashlib,itertools,json,os,random,re,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-skill-fsm-callbacks-v1/original-functions.json'
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def run(command,env=None):
    p=subprocess.run(list(map(str,command)),cwd=ROOT,env=env,capture_output=True,text=True)
    if p.returncode:raise RuntimeError(f'{command}\nexit={p.returncode}\n{p.stdout}\n{p.stderr}')
    return p.stdout
def verify(original):
    from elftools.elf.elffile import ELFFile
    m=json.loads(MANIFEST.read_text());assert sha(original)==m['original_sha256'];raw=original.read_bytes()
    with original.open('rb') as f:
        elf=ELFFile(f);symbols={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()};loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        def data(at,n):
            seg=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz']);off=int(seg['p_offset'])+at-int(seg['p_vaddr']);return raw[off:off+n]
        for row in m['functions']:
            at,n=int(row['elf_address'],0),row['size'];symbol=symbols[row['original_symbol']];assert (symbol['st_value'],symbol['st_size'])==(at,n)
            assert hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
        lit=m['debug_literal'];text=lit['text'].encode()+b'\0';assert data(int(lit['elf_address'],0),len(text))==text and hashlib.sha256(text).hexdigest()==lit['sha256']
        assert struct.unpack('<I',data(0x99531c,4))[0]==symbols['_ZN13DebugSwitches6s_instE']['st_value']==0x9a1d18
    return m
def oracle(original,exe,env,m):
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    sys.path.insert(0,str(MODULE/'tests'));from elf_import_identity import verify_imports
    imports=verify_imports(original,{0x30e310:'__stack_chk_fail'});cpu=Cpu(original,False,m)
    C=cpu.data+0x1000;D=0x9a1d18;D2=cpu.data+0x6000;P=cpu.data+0x7000;P2=cpu.data+0x8000;target=cpu.data+0x9000
    def store(at,value):cpu.pointer(at,value)
    def word(at):return struct.unpack('<I',cpu.uc.mem_read(at,4))[0]
    def byte(at):return bytes(cpu.uc.mem_read(at,1))[0]
    def returned(value=0xffffffff):cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
    def text(at):return bytes(cpu.uc.mem_read(at,96)).split(b'\0')[0].decode()
    def tag(id):return {0:0,C:1,C+0x3c8:2,C+0x4fc:3,C+0x49c:4,C+0x3b4:5,D:6,D2:7,P:8,P2:9}.get(id,99)
    instructions=set();records=[];scenarios=[]
    classes=[(0,0,0),(1,0,0),(2,0,0),(1,1,0),(1,0,1),(1,0,0xffffffff)]
    for cb,moving,present,group,gate,query in itertools.product(range(2),[0,1,255],range(2),classes,[0,0x140,0xffffffff],[0,0xffffffff]):
        scenarios.append([cb,0xffffffff,gate,0xaa,moving,present,*group,query,0])
    for cb,mut in itertools.product(range(2),range(1,15)):scenarios.append([cb,0x10000000,0x140,0xaa,1,1,1,0,0,0xffffffff,mut])
    rng=random.Random(0x3c4480)
    for _ in range(24):scenarios.append([rng.randrange(2),rng.getrandbits(32),rng.getrandbits(32),rng.randrange(256),rng.randrange(256),rng.randrange(2),*rng.choice(classes),rng.getrandbits(32),rng.randrange(15)])
    for row in scenarios:
        cb,flags,gate,ooi_intent,moving,present,monster,mini,boss,query,mut=row
        cpu.uc.mem_write(C,bytes(0x600));store(C+0x520,flags);store(C+0x528,gate);cpu.uc.mem_write(C+0x412,bytes([ooi_intent]));cpu.uc.mem_write(C+0x554,bytes([moving]));store(C+0x2dc,P if present else 0);store(0x99531c,D)
        store(C+0x3c8+0x40,target);store(C+0x3c8+0x44,0);trace=[];string_at=[0];live=[False]
        def mutate(op):
            if mut==1 and op==0:store(0x99531c,D2)
            if mut==2 and op==1:store(C+0x528,0x12345678)
            if mut==3 and op==2:store(C+0x528,0xabcdefaa);store(C+0x520,0x12340000)
            if mut==4 and op==3:store(C+0x528,0xfffffff0);cpu.uc.mem_write(C+0x554,b'\xff');store(C+0x2dc,P2)
            if mut==5 and op==6:store(C+0x528,word(C+0x528)^0x100);store(C+0x2dc,P2);store(C+0x520,0x87650000)
            if mut==6 and op==7:store(C+0x520,0xfedcba98);cpu.uc.mem_write(C+0x554,b'\0')
            if mut==7 and op==8:cpu.uc.mem_write(C+0x412,b'\xff')
            if mut==8 and op==9:cpu.uc.mem_write(C+0x554,b'\xff');store(C+0x2dc,P2);store(C+0x528,0x55550040)
            if mut==9 and op==4:store(C+0x528,word(C+0x528)^0x100);store(C+0x2dc,P2)
            if mut==10 and op==5:store(C+0x528,word(C+0x528)^0x100);store(C+0x2dc,P2)
            if mut==11 and op in [10,11]:store(C+0x520,0xaaaa5555);store(C+0x2dc,P2)
            if mut==12 and op==13:store(C+0x520,0x11110001)
            if mut==13 and op==14:store(C+0x520,0x22220002)
            if mut==14 and op==15:store(C+0x520,0x33330003)
        def record(op,subject,a=0,b=0,c=0,string=0):trace.append([op,tag(subject),a,b,c,string]);mutate(op)
        def hook(_,at,__,___):
            for start,end in [(0x3c4480,0x3c45b0),(0x3c434c,0x3c446c)]:
                if start<=at<end:instructions.add(at)
            if at==0x337888:assert cpu.reg(0)==D;record(0,D);returned()
            elif at==0x3140ec:
                assert text(cpu.reg(1))=='isTracingCharState';string_at[0]=cpu.reg(0);live[0]=True;record(1,0);returned()
            elif at==0x337a88:
                assert cpu.reg(0)==D and cpu.reg(1)==string_at[0] and live[0];record(2,D,string=10);returned(query)
            elif at==0x318254:
                assert cpu.reg(0)==string_at[0] and live[0];record(3,0);trace[-1][1]=10;live[0]=False;returned()
            elif at==0x3d49c4:
                assert cpu.reg(0)==C+0x3c8;record(4,cpu.reg(0)) # actual12B leaf executes
            elif at==0x3938f8:
                assert cpu.reg(0)==C;record(5,C);cpu.uc.mem_write(C+0x412,b'\0');cpu.uc.mem_write(C+0x554,b'\0');returned()
            elif at==0x3a4d5c:
                assert cpu.reg(0)==C and cpu.reg(1)==(0x1e if not cb else 0x1f) and cpu.reg(2)==0;record(6,C,cpu.reg(1));returned()
            elif at==0x3c0b50:assert cpu.reg(0)==C+0x4fc and cpu.reg(1)==0xffffffff;record(7,cpu.reg(0),cpu.reg(1));returned()
            elif at==0x3c93fc:assert cpu.reg(0)==C+0x49c and cpu.reg(1)==0x3f800000;record(8,cpu.reg(0),cpu.reg(1));returned()
            elif at==0x3bc6b8:assert cpu.reg(0)==C;record(9,C);returned()
            elif at in [0x46eae0,0x46eb20]:assert cpu.reg(0) in [P,P2];record(10 if at==0x46eae0 else 11,cpu.reg(0));returned()
            elif at==0x3dbe24:
                assert [cpu.reg(i) for i in range(4)]==[C+0x3b4,10,0,0x30] and word(cpu.uc.reg_read(cpu.sp))==0;record(12,cpu.reg(0),10,0,0x30);returned()
            elif at in [0x3a3064,0x3a3144,0x3a3158]:
                assert cpu.reg(0)==C;op={0x3a3064:13,0x3a3144:14,0x3a3158:15}[at];record(op,C);returned({13:monster,14:mini,15:boss}[op])
        h=cpu.uc.hook_add(UC_HOOK_CODE,hook)
        try:cpu.invoke(0x3c4480 if not cb else 0x3c434c,[0,0,C,0])
        finally:cpu.uc.hook_del(h)
        expected={'status':0,'flags':word(C+0x520),'gate':word(C+0x528),'ooi_intent':byte(C+0x412),'moving':byte(C+0x554),'physical':tag(word(C+0x2dc)),'debug_selection':tag(word(0x99531c)),'live_string':int(live[0]),'trace':trace}
        actual=json.loads(run([exe,'--oracle',*row],env));assert actual==expected,(row,expected,actual)
        if cb:assert word(C+0x3c8+0x44)==target
        records.append({'input':row,'original':expected,'compiled':actual})
    required=set(range(0x3c4480,0x3c45b0,4))|set(range(0x3c434c,0x3c446c,4));assert instructions==required
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'normal_instruction_addresses':[hex(x) for x in sorted(instructions)],'verified_imports':imports,'results':records,
            'scope':'All148 normal Focus/Blur instructions execute. Original SyncLastTarget12B executes too; other named dependencies are observing provider fixtures. Raw ignored query/timer returns, live field mutation, captured Debug and noncanonical classification words are checked. Compiler canary traps excluded; no new dependency-body credit/native activation.'}
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler',required=True,type=Path);p.add_argument('--cache',required=True,type=Path);p.add_argument('--original-elf',required=True,type=Path);p.add_argument('--output',required=True,type=Path);p.add_argument('--report',type=Path,default=MODULE/'build/character-skill-fsm-callbacks-v1/validation.json');a=p.parse_args()
    out=a.output.resolve();wrapper=out/'wrapper';build=out/'selected';wrapper.mkdir(parents=True,exist_ok=True);build.mkdir(exist_ok=True);cxx=a.compiler.resolve();cc=cxx.with_name(cxx.name.replace('g++','gcc'));cmake=shutil.which('cmake');ninja=shutil.which('ninja')
    source=MODULE/'character_skill_fsm_callbacks_v1.cpp';test=MODULE/'tests/character_skill_fsm_callbacks_v1.cpp';backend=ROOT/'port/android-native/app/src/main/cpp/native_debug_files.cpp'
    cm=f'''cmake_minimum_required(VERSION 3.22)
project(skill_fsm_audit LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-world)
add_executable(skill_fsm_audit "{source.as_posix()}" "{test.as_posix()}" "{backend.as_posix()}")
target_compile_features(skill_fsm_audit PRIVATE cxx_std_17)
target_compile_options(skill_fsm_audit PRIVATE -Wall -Wextra -Werror -pedantic)
target_link_libraries(skill_fsm_audit PRIVATE dh2_level_world)
'''
    (wrapper/'CMakeLists.txt').write_text(cm,encoding='utf-8')
    logs=[run([cmake,'-S',wrapper,'-B',build,'-G','Ninja',f'-DCMAKE_MAKE_PROGRAM={ninja}',f'-DCMAKE_CXX_COMPILER={cxx}',f'-DCMAKE_C_COMPILER={cc}','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_FLAGS_RELEASE=-O1','-DCMAKE_C_FLAGS_RELEASE=-O1'])]
    commands=run([ninja,'-C',build,'-t','commands','skill_fsm_audit']);db=json.loads((build/'compile_commands.json').read_text());sources={Path(x['file']).resolve() for x in db if str(x['file']).replace('\\','/') in commands.replace('\\','/')}
    assert source in sources and ROOT/'port/adam-script-runtime/script_runtime.c' in sources
    paths=sources|{Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md'),MODULE/'tests/elf_import_identity.py',ROOT/'port/engine-resources/tests/cpu.py'}
    for folder in {x.parent for x in sources}:
        paths.update(folder.glob('*.h'));paths.update(folder.glob('*.hpp'))
        if (folder/'CMakeLists.txt').exists():paths.add(folder/'CMakeLists.txt')
    before={x.relative_to(ROOT).as_posix():sha(x) for x in paths if x.is_relative_to(ROOT)};m=verify(a.original_elf.resolve());seed=a.cache/'DebugSwitches.savegame';cache_hash=sha(seed)
    logs.append(run([cmake,'--build',build,'--target','skill_fsm_audit','--parallel','1']));assert all(sha(ROOT/name)==h for name,h in before.items()),'Sources changed during build'
    exe=build/'skill_fsm_audit.exe';dsos=sorted(build.rglob('*.dll'));env=os.environ.copy();env['PATH']=os.pathsep.join([str(cxx.parent),*(str(x.parent) for x in dsos),env.get('PATH','')])
    imports={x.relative_to(out).as_posix():re.findall(r'DLL Name: (\S+)',run([cxx.with_name('objdump.exe'),'-p',x])) for x in [exe,*dsos]}
    host=json.loads(run([exe,a.cache.resolve(),out/'real-debug'],env));assert host['validation']=='PASS' and host['functional_cases']==37 and host['failure_cases']==32 and host['guards']==14
    arm=oracle(a.original_elf.resolve(),exe,env,m);assert all(sha(ROOT/name)==h for name,h in before.items()) and sha(seed)==cache_hash
    report={'validation':'PASS','host_report':host,'original_arm_comparison':arm,'original_sha256':sha(a.original_elf),'new_complete_normal_caller_bodies':2,'new_complete_dependency_bodies':0,
            'source_sha256':before,'cache_inputs_sha256':{'DebugSwitches.savegame':cache_hash},'attribution':m['attribution'],'selected_commands':commands,'selected_dso_imports':imports,
            'binary_sha256':{x.relative_to(out).as_posix():sha(x) for x in [exe,*dsos]},'wrapper_cmake':cm,'build_stdout':logs,'native_wired':False,
            'scope':'Adapted source normal caller control only; mandatory original dependency services remain outside these bodies. New caller compiled exclusively into audit executable; selected production world unchanged. Real Debug FS and Coordinator10ms storage demonstrated, not whole physical/animation/FSM/AI/skill activation.'}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({'host':host,'original_cases':arm['comparisons'],'instructions':len(arm['normal_instruction_addresses']),'mismatches':0}))
if __name__=='__main__':main()
