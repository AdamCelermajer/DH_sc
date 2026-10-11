"""Pinned death callers against the real selected world and sole VM DSOs."""
from __future__ import annotations
import argparse,json,os,re,shutil
from pathlib import Path
from run_player_skill_cleanup_session_v1_host import sha,run,selected_entries,actual_dependencies,hashes
from player_ai_death_v1_original import capture
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler',type=Path,required=True);p.add_argument('--cache',type=Path,required=True);p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--report',type=Path,default=MODULE/'reports/player-ai-death-v1-selected-host.json');a=p.parse_args()
 out=a.output.resolve();out.mkdir(parents=True,exist_ok=True);original=capture(a.original_elf.resolve(),out/'original')
 modules=['player_ai_death_v1','character_sm_set_dead_state_v1','character_aggro_cleanup','player_skill_cleanup_session_v1'];selection=(MODULE/'CMakeLists.txt').read_text();assert all(m+'.cpp' in selection for m in modules),'Parent must select modules centrally'
 compiler=a.compiler.resolve();cmake=shutil.which('cmake');ninja=shutil.which('ninja');cc=compiler.with_name(compiler.name.replace('g++','gcc'))
 wrapper=out/'wrapper';wrapper.mkdir(exist_ok=True);build=out/'build';test=MODULE/'tests/player_ai_death_v1.cpp'
 body=f'''cmake_minimum_required(VERSION 3.22)
project(player_death_selected LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-world)
add_executable(player_death_audit "{test.as_posix()}" "{(ROOT/'port/android-native/app/src/main/cpp/native_debug_files.cpp').as_posix()}" "{(ROOT/'port/pydata-names/names.c').as_posix()}")
target_compile_features(player_death_audit PRIVATE cxx_std_17)
target_compile_options(player_death_audit PRIVATE -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
target_link_libraries(player_death_audit PRIVATE dh2_level_world dh2_script_runtime)
'''
 (wrapper/'CMakeLists.txt').write_text(body);logs=[run([cmake,'-S',wrapper,'-B',build,'-G','Ninja',f'-DCMAKE_MAKE_PROGRAM={ninja}',f'-DCMAKE_CXX_COMPILER={compiler}',f'-DCMAKE_C_COMPILER={cc}','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_FLAGS_RELEASE=-O1','-DCMAKE_C_FLAGS_RELEASE=-O1'])]
 print('Discovering actual selected compiler dependencies',flush=True);logs.append(run([cmake,'--build',build,'--target','player_death_audit','--parallel','2']))
 commands=run([ninja,'-C',build,'-t','commands','player_death_audit']);entries=selected_entries(build,commands);sources=[Path(r['file']).resolve() for r in entries]
 for m in modules:assert sources.count((MODULE/(m+'.cpp')).resolve())==1,m
 assert sum(p.name=='script_runtime.c' for p in sources)==1
 evidence={Path(__file__).resolve(),MODULE/'tests/player_ai_death_v1_original.py',MODULE/'tests/run_player_skill_cleanup_session_v1_host.py',ROOT/'port/engine-resources/tests/cpu.py'}
 before=hashes(actual_dependencies(build,ninja,entries)|evidence)
 cache=a.cache.resolve()
 inputs={p.relative_to(cache).as_posix():sha(p) for p in cache.rglob('*') if p.is_file()}
 required=['data/pydata/animations_dictionary_pyarraynames.bin','data/pydata/animations_dictionary_pyarray.bin','data/pydata/animations_pyarray.bin','data/pydata/animations_pyarraynames.bin','data/pydata/animations_pystructnames.bin','data/pydata/animations_pycst.bin',
  'data/pydata/skills_pyarray.bin','data/pydata/skills_pyarraynames.bin','data/pydata/skills_pystructnames.bin','data/pydata/faeries_pyarray.bin','data/pydata/faeries_pyarraynames.bin','data/pydata/faeries_pystructnames.bin',
  'data/pydata/character_properties_pyarray.bin','data/pydata/character_properties_pyarraynames.bin','data/pydata/character_properties_pystructnames.bin','data/pydata/character_classes_pyarray.bin','data/pydata/character_classes_pyarraynames.bin','data/pydata/character_classes_pystructnames.bin']
 assert all(name in inputs for name in required),'Selected source cache lacks a Player death audit input'
 print(f'Clean selected rebuild from {len(before)} guarded project inputs',flush=True);logs.append(run([cmake,'--build',build,'--target','player_death_audit','--clean-first','--parallel','2']))
 dlls=sorted(build.rglob('*.dll'));exe=build/'player_death_audit.exe';env=os.environ.copy();env['PATH']=os.pathsep.join([str(compiler.parent),*(str(d.parent) for d in dlls),env.get('PATH','')])
 imports={p.relative_to(out).as_posix():re.findall(r'DLL Name: (\S+)',run([compiler.with_name('objdump.exe'),'-p',p])) for p in [exe,*dlls]}
 assert 'libdh2_level_world.dll' in imports[exe.relative_to(out).as_posix()] and sum(p.name=='libdh2_script_runtime.dll' for p in dlls)==1
 host=json.loads(run([exe,cache,out/'real-debug',out/'original/original-cases.bin'],env));assert host['validation']=='PASS' and host['original_route_cases']==21 and host['actual_player_classes']==3
 raw=out/'raw-execution.json';raw.write_text(json.dumps({'validation':'EXECUTED','host':host},indent=2)+'\n')
 after_commands=run([ninja,'-C',build,'-t','commands','player_death_audit']);after=hashes(actual_dependencies(build,ninja,selected_entries(build,after_commands))|evidence)
 changes={k:{'before':before.get(k),'after':after.get(k)} for k in before.keys()|after.keys() if before.get(k)!=after.get(k)};cache_changes=[name for name,value in inputs.items() if sha(cache/name)!=value]
 raw.write_text(json.dumps({'validation':'PASS' if not changes and not cache_changes and commands==after_commands else 'PROVENANCE_FAILED','host':host,'input_changes':changes,'cache_changes':cache_changes},indent=2)+'\n')
 assert not changes and not cache_changes and commands==after_commands,('Compiled input changed; raw result retained',changes,cache_changes)
 report={'validation':'PASS','host_report':host,'original_capture':original,'source_sha256':before,'cache_inputs_sha256':inputs,'selected_modules':modules,'source_before_after_equal':True,'new_module_selected':True,'separate_direct_module_object':False,'scoped_source_tu_count':sources.count((MODULE/'player_ai_death_v1.cpp').resolve()),'script_runtime_tu_count':sum(p.name=='script_runtime.c' for p in sources),'dependency_guard':'Actual selected compiler Ninja dependencies and configured project CMake inputs, snapshot then clean selected rebuild, execution and exact closure/hash/command comparison.','dso_imports':imports,'binary_sha256':{p.relative_to(out).as_posix():sha(p) for p in [exe,*dlls]},'selected_commands':commands,'wrapper_cmake':body,'build_stdout':logs,'scope':'Whole original death caller route with declared external callee fixtures; actual selected target/Coordinator/aggro/cleanup kernels. Real three player classes, real Debug and animations, same prepared Session VM callbacks. State external scene/controller/FX/Debug prelude bodies are declared callee fixtures. No complete Character::Kill, native deployment or live gameplay claim.'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'host':host,'pinned_words':original['distinct_pinned_words'],'project_inputs':len(before)}))
if __name__=='__main__':main()
