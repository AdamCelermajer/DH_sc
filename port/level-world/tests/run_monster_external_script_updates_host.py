"""Real retained monster VM update/state calls and resolved-path load cache."""
from __future__ import annotations
import argparse,hashlib,json,os,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1];RUNTIME=ROOT/'port/adam-script-runtime'
MANIFEST=MODULE/'reference/monster-external-script-updates/original-functions.json'
CORE='lapi lcode ldebug ldo ldump lfunc lgc llex lmem lobject lopcodes lparser lstate lstring ltable ltm lundump lvm lzio lauxlib lbaselib ltablib lstrlib lmathlib'.split()
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def run(command):
    r=subprocess.run(command,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    return r.stdout
def verify(original):
    from elftools.elf.elffile import ELFFile
    m=json.loads(MANIFEST.read_text());raw=original.read_bytes();assert digest(original)==m['original_sha256']
    with original.open('rb') as stream:
        elf=ELFFile(stream);syms={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()};loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        def read(at,n):
            load=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz']);off=int(load['p_offset'])+at-int(load['p_vaddr']);return raw[off:off+n]
        for row in m['functions']:
            at,n=int(row['elf_address'],0),row['size'];s=syms[row['original_symbol']];assert(s['st_value'],s['st_size'])==(at,n);assert hashlib.sha256(read(at,n)).hexdigest()==row['sha256']
        for site in m['constructor_null_state_stores']:
            assert read(int(site['elf_address'],0),4).hex()==site['instruction_bytes']
    for row in m['scripts']:
        path=ROOT/row['path'];assert path.stat().st_size==row['size'] and digest(path)==row['sha256']
    return m
def oracle(original,exe,commons,monster,m):
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    cpu=Cpu(original,False,m);ais=cpu.data+0x1000;A=cpu.data+0x2000;B=cpu.data+0x3000;strings=cpu.data+0x4000
    names=['UpdateKey','ConditionKey','ArmStateUpdate','ArmStateConditions','ArmStateReplacement'];pointers={name:strings+i*64 for i,name in enumerate(names)}
    for name,at in pointers.items():cpu.uc.mem_write(at,name.encode()+b'\0')
    def text(at):return bytes(cpu.uc.mem_read(at,64)).split(b'\0')[0].decode()
    def word(at):return struct.unpack('<I',cpu.uc.mem_read(at,4))[0]
    instructions=set();records=[]
    for kind in range(3):
      for present in range(2):
       for alias in range(2):
        for mutation in range(4):
            cpu.pointer(ais+0xb4,A if present else 0);cpu.pointer(A+0x14,pointers['UpdateKey' if alias else 'ArmStateUpdate']);cpu.pointer(A+0x2c,pointers['ConditionKey' if alias else 'ArmStateConditions']);cpu.pointer(B+0x14,pointers['ArmStateUpdate']);cpu.pointer(B+0x2c,pointers['ArmStateReplacement'])
            trace=[];requested=[]
            def observe(_,at,__,___):
                if 0x3d8ea0<=at<0x3d8ec8:instructions.add(at)
                if at==0x37c514:
                    assert cpu.reg(0)==ais;name=text(cpu.reg(1));requested.append(name)
                    resolved={'UpdateKey':'ArmStateUpdate','ConditionKey':'ArmStateConditions'}.get(name,name)
                    action='GetState' if resolved=='ArmStateConditions' else 'HasTarget';trace.append(action)
                    if action=='HasTarget' and mutation in (1,2):cpu.pointer(ais+0xb4,B if mutation==1 else 0)
                    if action=='HasTarget' and mutation==3:cpu.pointer(A+0x2c,pointers['ArmStateReplacement'])
                    # LuaScript::Call(char*) has zero explicit source args and
                    # discards returns. Callee fixture observes name/callback
                    # effects only; no original Lua Call body credit.
                    cpu.put(0,0xffffffff);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
            hook=cpu.uc.hook_add(UC_HOOK_CODE,observe)
            try:
                cpu.invoke(0x3d8ea0 if kind==1 else 0x3d8eb4,[ais])
                if kind==2:cpu.invoke(0x3d8ea0,[ais])
            finally:cpu.uc.hook_del(hook)
            expected={'status':0,'calls':len(trace),'trace':trace}
            actual=json.loads(run([str(exe),str(commons),str(monster),'--oracle',str(kind),str(present),str(alias),str(mutation)]));assert actual==expected,(kind,present,alias,mutation,expected,actual)
            records.append({'input':{'kind':kind,'present':present,'alias':alias,'mutation':mutation},'requested_names':requested,'original_current_state_nonnull':word(ais+0xb4)!=0,'original_result':expected,'compiled_result':actual,'matched':True})
    assert instructions==set(range(0x3d8ea0,0x3d8ec8,4))
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'state_wrapper_instructions_observed':len(instructions),'instruction_addresses':[hex(a) for a in sorted(instructions)],'results':records,'scope':'All10 exact original instructions of the independent20B CallStateUpdate/Conditions helpers run; LuaScript::Call is a named observation/mutation fixture. Real alias/VM bodies execute in separate host test. No new whole AISExternal or Lua Call body claim.'}
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler');p.add_argument('--c-compiler');p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--output',type=Path,default=MODULE/'build/monster-external-script-updates/host.exe');p.add_argument('--report',type=Path,default=MODULE/'build/monster-external-script-updates/validation.json');a=p.parse_args()
    cxx=a.compiler or os.environ.get('CXX') or shutil.which('g++');assert cxx
    cc=a.c_compiler or str(Path(cxx).with_name('gcc.exe' if Path(cxx).suffix=='.exe' else 'gcc'));exe=a.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True);objdir=exe.parent/'objects';objdir.mkdir(exist_ok=True)
    m=verify(a.original_elf.resolve());commons=ROOT/m['scripts'][0]['path'];monster=ROOT/m['scripts'][1]['path']
    c_sources=[RUNTIME/'lua'/(name+'.c') for name in CORE]+[RUNTIME/'script_runtime.c',ROOT/'port/lua-numeric/numeric.c']
    cpp_sources=[RUNTIME/'script_function_alias.cpp',MODULE/'ais_native_bindings.cpp',MODULE/'ais_external_init_callbacks.cpp',MODULE/'ais_state_callbacks.cpp',MODULE/'lua_script_load_once.cpp',MODULE/'character_oid_cache_v1.cpp',MODULE/'monster_external_script_session.cpp',MODULE/'tests/monster_external_script_updates.cpp']
    commands=[];objects=[];warnings=[]
    for source in c_sources+cpp_sources:
        obj=objdir/(source.parent.name+'-'+source.stem+'.o');strict=source in cpp_sources[1:]
        command=[cxx if source.suffix=='.cpp' else cc,'-std=c++17' if source.suffix=='.cpp' else '-std=c99','-O1','-fno-fast-math','-ffp-contract=off','-I',str(RUNTIME/'lua'),'-Wall',*(['-Wextra','-Werror','-pedantic'] if strict else []),'-c',str(source),'-o',str(obj)]
        result=subprocess.run(command,capture_output=True,text=True);assert result.returncode==0,result.stdout+result.stderr
        if result.stderr:warnings.append({'path':source.relative_to(ROOT).as_posix(),'diagnostics':result.stderr})
        commands.append(command);objects.append(obj)
    command=[cxx,*map(str,objects),'-lm','-o',str(exe)];run(command);commands.append(command)
    reports=[json.loads(line) for line in run([str(exe),str(commons),str(monster)]).splitlines() if line.startswith('{')];assert len(reports)==2
    legacy,host=reports;assert legacy['monster_external_session_cases']==32 and legacy['mismatches']==0 and legacy['staged_same_vm_lifecycle'] and legacy['staged_errors_stop_without_fallback']
    assert host=={'validation':'PASS','update_cases':10,'failure_cases':7,'guard_cases':10,'resolved_load_cases':19,'zero_argument_update':True,'independent_live_state_reads':True,'fresh_alias_and_discarded_return_effects':True,'actual_same_vm_load_cache':True,'load_failure_prefix_and_retry':True,'native_wired':False,'mismatches':0},host
    comparison=oracle(a.original_elf.resolve(),exe,commons,monster,m)
    owned=[MODULE/'monster_external_script_session.hpp',MODULE/'monster_external_script_session.cpp',MODULE/'tests/monster_external_script_updates.cpp',Path(__file__).resolve(),MANIFEST,MODULE/'reference/monster-external-script-updates/NOTES.md']
    owned += [MODULE/'tests'/name for name in ('run_monster_external_script_session_host.py','run_ghost_ai_session_host.py','run_monster_initialization_session_host.py','run_monster_initialization_debug_persistence_host.py','run_character_level_runtime_host.py')]
    owned += [ROOT/'port/android-native/tests/run_ghost_ai_owner_host.py']
    reused=[*c_sources,*cpp_sources[:-2],MODULE/'tests/monster_external_script_session.cpp',MODULE/'ais_native_bindings.hpp',MODULE/'ais_external_init_callbacks.hpp',MODULE/'ais_state_callbacks.hpp',MODULE/'lua_script_load_once.hpp',MODULE/'character_oid_cache_v1.hpp',RUNTIME/'script_runtime.h',RUNTIME/'script_function_alias.h',ROOT/'port/lua-numeric/numeric.h',ROOT/'port/engine-resources/tests/cpu.py',*sorted((RUNTIME/'lua').glob('*.h'))]
    report={'validation':'PASS','legacy_session_report':legacy,'host_report':host,'original_arm_comparison':comparison,'original_sha256':digest(a.original_elf),'compiler_commands':commands,'source_sha256':{p.relative_to(ROOT).as_posix():digest(p) for p in owned},'reused_source_sha256':{p.relative_to(ROOT).as_posix():digest(p) for p in reused},'original_script_sha256':{p.relative_to(ROOT).as_posix():digest(p) for p in (commons,monster)},'dependency_warnings':warnings,'reused_complete_state_wrapper_bodies':2,'new_complete_caller_bodies':0,'new_whole_ais_external_update_bodies':0,'new_complete_lua_dependency_bodies':0,'native_wired':False,'whole_original_vm_parity':False}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k in ('validation','legacy_session_report','host_report','source_sha256','reused_complete_state_wrapper_bodies','new_complete_caller_bodies','native_wired')},indent=2))
if __name__=='__main__':main()
