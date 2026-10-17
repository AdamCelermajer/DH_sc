"""Borrowed cooldown adaptation: selected libraries, authored Lua and original ARM."""
from __future__ import annotations
import argparse,hashlib,json,math,os,random,re,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-skill-cooldown-services/original-functions.json'
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
OWNER,ARGS,VECTOR,VALUES,LIST,SLOTS,ALT,INSTANCES,ALT_INSTANCES=0x10010000,0x10014000,0x10014100,0x10015000,0x10018000,0x10019000,0x1001a000,0x10020000,0x10022000
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def bits(f):return struct.unpack('<I',struct.pack('<f',f))[0]
def floating(w):return struct.unpack('<f',struct.pack('<I',w))[0]
def signed_number(w):
    f=floating(w)
    if math.isnan(f):return 0
    if f>=2147483648:return 0x7fffffff
    if f< -2147483648:return 0x80000000
    return int(f)&0xffffffff
def run(command,env=None):
    p=subprocess.run(list(map(str,command)),cwd=ROOT,capture_output=True,text=True,env=env)
    if p.returncode:raise RuntimeError(f'{command}\n{p.stdout}\n{p.stderr}')
    return p.stdout,p.stderr
def cases():
    rng=random.Random(20261007);choices=[bits(f) for f in [-math.inf,-3,-.1,-0.,0.,.999,1.,2.9,16777216.,2147483648.,4294967296.,math.inf,math.nan]]+[0xffc00000]
    rows=[]
    for i in range(512):
        spell=i%2;count=i%6;mask=rng.randrange(32);argc=i%3
        tag=rng.choice([0,1,2,3,4,5,7,99]);value=bits(rng.choice([0.,1.,2.9,3.,4.5]))
        if tag==0:value=0
        timer_tag=rng.choice([0,1,3,4,5]);timer=rng.choice(choices)
        if spell:tag,timer_tag,value=timer_tag,0,timer
        rows.append([spell,argc,tag,value,timer_tag,timer,count,mask,0])
    for timer in choices:
        for spell in (0,1):rows.append([spell,1 if spell else 2,3,timer if spell else 0,3,timer,5,31,0])
    rows.extend([[0,2,4,0,3,bits(100),3,31,1],[0,2,3,0,3,bits(100),3,31,2],[1,1,3,bits(100),0,0,3,31,3],[1,1,3,bits(100),0,0,3,31,4],[0,2,3,0,3,bits(100),3,31,5]])
    return rows
def compare(original,exe,env):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    sys.path.insert(0,str(MODULE/'tests'));from elf_import_identity import verify_imports
    manifest=json.loads(MANIFEST.read_text());imports=verify_imports(original,{0x30e4cc:'__aeabi_f2iz'})
    raw=original.read_bytes();assert digest(original)==SHA
    with original.open('rb') as f:
        elf=ELFFile(f);syms={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()};loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        for row in manifest['functions']:
            at,n=int(row['elf_address'],0),row['size'];s=syms[row['original_symbol']];assert (int(s['st_value']),int(s['st_size']))==(at,n)
            load=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz']);offset=int(load['p_offset'])+at-int(load['p_vaddr']);assert hashlib.sha256(raw[offset:offset+n]).hexdigest()==row['sha256']
    class OracleCpu(Cpu):
        def external(self,uc,at,size,user):
            if self.imports.get(at)=='__aeabi_f2iz':self.put(0,signed_number(self.reg(0)));uc.reg_write(self.pc,uc.reg_read(self.lr));return
            return super().external(uc,at,size,user)
    cpu=OracleCpu(original,False,manifest);cpu.uc.mem_map(0x10000000,0x60000);coverage=set();results=[]
    def word(at):return struct.unpack('<I',cpu.uc.mem_read(at,4))[0]
    def ret(value=0):cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
    for row in cases():
        spell,argc,tag,value,timer_tag,timer,count,mask,mutation=row
        cpu.uc.mem_write(OWNER,bytes(0x1000));cpu.uc.mem_write(VALUES,bytes(224));cpu.uc.mem_write(cpu.stack,bytes(0x10000));cpu.pointer(ARGS+4,VECTOR)
        cpu.uc.mem_write(VECTOR,struct.pack('<3I',VALUES,VALUES+112*argc,VALUES+224));cpu.pointer(VALUES+4,tag);cpu.pointer(VALUES+8,value);cpu.pointer(VALUES+116,timer_tag);cpu.pointer(VALUES+120,timer)
        cpu.pointer(LIST+4,count);cpu.pointer(OWNER+0x47c,SLOTS);cpu.pointer(OWNER+0x488,SLOTS)
        for i in range(5):
            cpu.pointer(SLOTS+i*4,INSTANCES+i*32 if mask&(1<<i) else 0);cpu.pointer(ALT+i*4,ALT_INSTANCES+i*32 if mask&(1<<i) else 0)
            cpu.pointer(INSTANCES+i*32+24,0x11223344);cpu.pointer(ALT_INSTANCES+i*32+24,0x55667788)
        lists=[];slot_reads=[0];external_numbers=[]
        def observe(uc,at,size,user):
            coverage.add(at)
            if at in (0x3bc5fc,0x3ae5dc):
                assert cpu.reg(0)==OWNER;lists.append(int(at==0x3ae5dc));ret(LIST)
                if mutation==1:cpu.pointer(VALUES+8,bits(1))
                if mutation==3:cpu.pointer(VALUES+8,bits(7))
            elif at==0x31bbf0 and word(cpu.reg(0)+4) in (2,4,7):
                p=cpu.reg(0);external_numbers.append({'argument':(p-VALUES)//112,'value_bits':word(p+8)});ret(word(p+8))
                if mutation==5 and p==VALUES+112:cpu.pointer(OWNER+0x47c,ALT)
            elif at==0x3b990c:
                if mutation==2:cpu.pointer(VALUES+116,0)
                if mutation==5:cpu.pointer(VALUES+116,4)
            elif at==0x3b9164:
                slot_reads[0]+=1
                if mutation==4 and slot_reads[0]==1:cpu.pointer(OWNER+0x488,ALT)
            elif at==0x708eb0:raise AssertionError('Excluded source assertion reached')
        hook=cpu.uc.hook_add(UC_HOOK_CODE,observe);cpu.invoke(0x3b90e4 if spell else 0x3b97e0,[ARGS,0,OWNER]);cpu.uc.hook_del(hook)
        expected={'lists':lists,'fields':[word(base+i*32+24) for base in (INSTANCES,ALT_INSTANCES) for i in range(5)]}
        actual=json.loads(run([exe,'oracle',*row],env)[0]);assert actual==expected,(row,actual,expected)
        results.append({'input':row,'matched':True,'result':actual,'modeled_nontrivial_number_services':external_numbers})
    excluded={int(a,0) for a in manifest['excluded_skill_assert_instructions']};ordinary=set(range(0x3b97e0,0x3b99c0,4))-excluded
    skill_missing=sorted(ordinary-coverage);spell_expected=set(range(0x3b90e4,0x3b91a0,4));assert not skill_missing,[hex(a) for a in skill_missing];assert not spell_expected-coverage
    return {'validation':'PASS','comparisons':len(results),'mismatches':0,'verified_imports':imports,'skill_ordinary_instructions':len(ordinary),'spell_instructions':len(spell_expected),'excluded_skill_assert_instructions':sorted(hex(a) for a in excluded),'results':results,'executed_scope':'Actual ordinary Skill496B branches (assert recovery excluded), complete Spell188B, safe Arguments::operator[]88B branches, scalar Value::getNumber and getUInteger16B, actual ELF __aeabi_f2uiz84B. Nontrivial Value numbers and selected-list lookup are named fixture providers; imported f2iz identity verified before exact saturation model. No callback/timer/whole Value body credit.'}
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler',required=True);p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--report',type=Path,default=MODULE/'build/character-skill-cooldown-services/validation.json');a=p.parse_args()
    out=a.output.resolve();wrapper=out/'wrapper';build=out/'selected';wrapper.mkdir(parents=True,exist_ok=True);build.mkdir(exist_ok=True);cxx=Path(a.compiler).resolve();cc=cxx.with_name(cxx.name.replace('g++','gcc'));cmake=shutil.which('cmake');ninja=shutil.which('ninja')
    text=f'''cmake_minimum_required(VERSION 3.22)
project(cooldown_selected_audit LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-world)
add_executable(cooldown_audit "{(MODULE/'tests/character_skill_cooldown_services.cpp').as_posix()}")
add_executable(vcb_regression "{(MODULE/'tests/ais_player_init_vcb.cpp').as_posix()}")
foreach(test cooldown_audit vcb_regression)
target_compile_features(${{test}} PRIVATE cxx_std_17)
target_compile_options(${{test}} PRIVATE -Wall -Wextra -Werror -pedantic -fno-fast-math -ffp-contract=off)
target_link_libraries(${{test}} PRIVATE dh2_level_world dh2_script_runtime)
endforeach()
'''
    (wrapper/'CMakeLists.txt').write_text(text,encoding='utf-8');commands=[];logs=[]
    config=[cmake,'-S',wrapper,'-B',build,'-G','Ninja',f'-DCMAKE_MAKE_PROGRAM={ninja}',f'-DCMAKE_CXX_COMPILER={cxx}',f'-DCMAKE_C_COMPILER={cc}','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_FLAGS_RELEASE=-O1','-DCMAKE_C_FLAGS_RELEASE=-O1'];commands.append(list(map(str,config)));logs.append(run(config)[0])
    selected=run([ninja,'-C',build,'-t','commands','cooldown_audit','vcb_regression'])[0];assert 'character_skill_cooldown_services.cpp' in selected
    compilation=json.loads((build/'compile_commands.json').read_text());sources={Path(r['file']).resolve() for r in compilation if str(r['file']).replace('\\','/') in selected.replace('\\','/')}
    paths=sources|{MANIFEST,MANIFEST.with_name('NOTES.md'),Path(__file__).resolve(),ROOT/'port/engine-resources/tests/cpu.py',MODULE/'tests/elf_import_identity.py'}
    for d in {p.parent for p in sources}-{MODULE/'tests'}:
        paths.update(d.glob('*.h'));paths.update(d.glob('*.hpp'))
        if (d/'CMakeLists.txt').exists():paths.add(d/'CMakeLists.txt')
    before={p.relative_to(ROOT).as_posix():digest(p) for p in paths if p.is_relative_to(ROOT)}
    command=[cmake,'--build',build,'--target','cooldown_audit','vcb_regression','--parallel','1'];commands.append(list(map(str,command)));logs.append(run(command)[0]);assert all(digest(ROOT/n)==h for n,h in before.items()),'Sources changed during build'
    dsos=sorted(build.rglob('*.dll'));exe=build/'cooldown_audit.exe';vcb=build/'vcb_regression.exe';env=os.environ.copy();env['PATH']=os.pathsep.join([str(cxx.parent),*(str(d.parent) for d in dsos),env.get('PATH','')]);objdump=cxx.with_name('objdump.exe')
    imports={p.relative_to(out).as_posix():re.findall(r'DLL Name: (\S+)',run([objdump,'-p',p])[0]) for p in [exe,vcb,*dsos]}
    world=next(d for d in dsos if d.name=='libdh2_level_world.dll');vm=next(d for d in dsos if d.name=='libdh2_script_runtime.dll');assert sum(d.name==vm.name for d in dsos)==1
    for test in (exe,vcb):assert world.name in imports[test.relative_to(out).as_posix()] and vm.name in imports[test.relative_to(out).as_posix()]
    assert vm.name in imports[world.relative_to(out).as_posix()]
    ai=ROOT/'recovered/scripts/original/data/scripts/ai/_commons.luac';skills=ROOT/'recovered/scripts/original/data/scripts/skills/_commons.luac'
    host=json.loads(run([exe,ai,skills],env)[0]);assert host['validation']=='PASS' and host['host_cases']==15 and host['guards']==11 and host['failure_cases']==14 and host['real_lua_cases']==5 and host['slot_contract_cases']==6
    legacy=json.loads(run([vcb,ai],env)[0]);assert legacy['validation']=='PASS' and legacy['host_cases']==57 and legacy['real_lua_cases']==4
    oracle=compare(a.original_elf.resolve(),exe,env);assert all(digest(ROOT/n)==h for n,h in before.items()),'Sources changed during replay'
    report={'validation':'PASS','attribution':json.loads(MANIFEST.read_text())['attribution'],'host_report':host,'vcb_regression':legacy,'original_arm_comparison':oracle,'original_sha256':SHA,'source_sha256':before,'unchanged_script_sha256':{p.relative_to(ROOT).as_posix():digest(p) for p in (ai,skills)},'compiler_commands':commands,'compiler_version':run([cxx,'--version'])[0].splitlines()[0],'output_directory':out.as_posix(),'build_stdout':logs,'selected_commands':selected,'selected_dso_imports':imports,'binary_sha256':{p.relative_to(out).as_posix():digest(p) for p in [exe,vcb,*dsos]},'wrapper_cmake':text,'wrapper_sha256':digest(wrapper/'CMakeLists.txt'),'scope':'Borrowed adaptation of Adam cooldown wrappers; existing source Character timers and coordinator plus unchanged Lua commons create/expire cooldowns through sole VM. No new owner/property/timer store, active combat or native player activation. Skill assertion-recovery not implemented; zero newly reconstructed complete-body credit.'}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({'validation':'PASS',**host,'original_arm_cases':oracle['comparisons'],'ordinary_skill_instructions':oracle['skill_ordinary_instructions'],'spell_instructions':oracle['spell_instructions'],'mismatches':0}))
if __name__=='__main__':main()
