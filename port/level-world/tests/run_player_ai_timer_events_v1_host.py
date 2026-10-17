"""Selected-world Player timer source proof and Coordinator route regressions."""
from __future__ import annotations
import argparse,hashlib,json,os,re,shutil,subprocess
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def run(command,env=None):
 r=subprocess.run(list(map(str,command)),cwd=ROOT,env=env,capture_output=True,text=True)
 if r.returncode:raise RuntimeError(f'{command}\n{r.stdout}\n{r.stderr}')
 return r.stdout
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler',type=Path,required=True);p.add_argument('--cache',type=Path,required=True);p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
 p.add_argument('--report',type=Path,default=MODULE/'reports/player-ai-timer-events-v1-selected-host.json');a=p.parse_args();out=a.output.resolve();out.mkdir(parents=True,exist_ok=True)
 from player_ai_timer_events_v1_original import capture
 original=capture(a.original_elf.resolve(),out/'original')
 compiler=a.compiler.resolve();cmake=shutil.which('cmake');ninja=shutil.which('ninja');cc=compiler.with_name(compiler.name.replace('g++','gcc'))
 selected=(MODULE/'CMakeLists.txt').read_text();modules=['character_regen_tick_v1','player_ai_timer_events_v1']
 assert all(m+'.cpp' in selected for m in modules),'Parent must select both scoped source modules centrally'
 test=MODULE/'tests/player_ai_timer_events_v1.cpp';backend=ROOT/'port/android-native/app/src/main/cpp/native_debug_files.cpp';names=ROOT/'port/pydata-names/names.c'
 wrapper=out/'wrapper';wrapper.mkdir(exist_ok=True);build=out/'build'
 body=f'''cmake_minimum_required(VERSION 3.22)
project(player_ai_timer_events_audit LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-world)
add_executable(player_ai_timer_events_audit "{test.as_posix()}" "{backend.as_posix()}" "{names.as_posix()}")
add_executable(existing_coordinator_audit "{(MODULE/'tests/character_coordinator.cpp').as_posix()}")
foreach(t player_ai_timer_events_audit existing_coordinator_audit)
 target_compile_features(${{t}} PRIVATE cxx_std_17)
 target_compile_options(${{t}} PRIVATE -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 target_link_libraries(${{t}} PRIVATE dh2_level_world dh2_script_runtime)
endforeach()
'''
 (wrapper/'CMakeLists.txt').write_text(body)
 configure=[cmake,'-S',wrapper,'-B',build,'-G','Ninja',f'-DCMAKE_MAKE_PROGRAM={ninja}',f'-DCMAKE_CXX_COMPILER={compiler}',f'-DCMAKE_C_COMPILER={cc}','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_FLAGS_RELEASE=-O1','-DCMAKE_C_FLAGS_RELEASE=-O1'];logs=[run(configure)]
 commands=run([ninja,'-C',build,'-t','commands','player_ai_timer_events_audit','existing_coordinator_audit'])
 compiled={Path(r['file']).resolve() for r in json.loads((build/'compile_commands.json').read_text()) if str(r['file']).replace('\\','/') in commands.replace('\\','/')}
 for m in modules+['character_coordinator','character_ai_events','character_ai_initialization','character_ai_association','character_regeneration','character_dot_tick','character_dot_attack','object_update_culling','player_skill_session_v1']:assert MODULE/(m+'.cpp') in compiled,m
 assert sum(p.name=='script_runtime.c' for p in compiled)==1
 paths=set(compiled)|{Path(__file__).resolve(),MODULE/'tests/player_ai_timer_events_v1_original.py',MODULE/'tests/player_skill_session_v1.cpp',MODULE/'CMakeLists.txt'}
 for folder in {p.parent for p in compiled}:paths.update(folder.glob('*.h'));paths.update(folder.glob('*.hpp'))
 before={p.relative_to(ROOT).as_posix():sha(p) for p in paths if p.is_relative_to(ROOT)}
 build_command=[cmake,'--build',build,'--target','player_ai_timer_events_audit','existing_coordinator_audit','--parallel','2'];logs.append(run(build_command))
 dlls=list(build.rglob('*.dll'));exe=build/'player_ai_timer_events_audit.exe';regression=build/'existing_coordinator_audit.exe'
 env=os.environ.copy();env['PATH']=os.pathsep.join([str(compiler.parent),*(str(p.parent) for p in dlls),env.get('PATH','')])
 host=json.loads(run([exe,a.cache.resolve(),out/'runtime',out/'original/original-cases.bin'],env));prior=json.loads(run([regression],env))
 assert host['validation']=='PASS' and host['original_regen_traces']==11 and host['failure_prefixes']==93
 assert before=={p:sha(ROOT/p) for p in before},'compiled input changed during gate'
 imports={p.relative_to(out).as_posix():re.findall(r'DLL Name: (\S+)',run([compiler.with_name('objdump.exe'),'-p',p])) for p in [exe,regression,*dlls]}
 assert 'libdh2_level_world.dll' in imports[exe.relative_to(out).as_posix()] and 'libdh2_script_runtime.dll' in imports[exe.relative_to(out).as_posix()]
 assert len([p for p in dlls if p.name=='libdh2_script_runtime.dll'])==1
 report={'validation':'PASS','host_report':host,'existing_coordinator_report':prior,'original_capture':original,'source_sha256':before,'selected_world_modules':modules,'separate_direct_module_object':False,'selected_native_build':False,'live_gameplay':False,'commands':[list(map(str,configure)),list(map(str,build_command))],'compiled_commands':commands,'build_stdout':logs,'wrapper_cmake':body,'binary_sha256':{p.relative_to(out).as_posix():sha(p) for p in [exe,regression,*dlls]},'dso_imports':imports,'scope':'Actual selected world/VM DSOs. Real cache Player classes, Character construction/association, Coordinator sole timer traversal/typed routing, unchanged same Session, actual Debug/files and canonical HP/MP/property/DoT attack callers. Positive application is an explicit test provider, not complete native Player F_ApplyResult or live gameplay.'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(host));print(json.dumps(prior))
if __name__=='__main__':main()
