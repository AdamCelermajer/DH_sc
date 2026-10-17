"""Original GEAR caller against actual selected world/game-data/text DSOs."""
from __future__ import annotations
import argparse,json,os,re,shutil
from pathlib import Path
from run_player_skill_cleanup_session_v1_host import sha,run,selected_entries,actual_dependencies,hashes
from player_saved_inventory_v1_original import capture
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]

def main():
 p=argparse.ArgumentParser(description=__doc__)
 for name in ['compiler','cache','profile','original-elf','output']:p.add_argument('--'+name,required=True,type=Path)
 p.add_argument('--report',type=Path,default=MODULE/'reports/player-saved-inventory-v1-selected-host.json');a=p.parse_args()
 out=a.output.resolve();wrapper=out/'wrapper';build=out/'build';wrapper.mkdir(parents=True,exist_ok=True)
 compiler=a.compiler.resolve();cc=compiler.with_name(compiler.name.replace('g++','gcc'));cmake=shutil.which('cmake');ninja=shutil.which('ninja')
 source=MODULE/'player_saved_inventory_v1.cpp';assert source.name in (MODULE/'CMakeLists.txt').read_text(),'module must already be centrally selected'
 original=capture(a.original_elf.resolve(),a.cache.resolve(),a.profile.resolve(),out/'original')
 body=f'''cmake_minimum_required(VERSION 3.22)
project(player_saved_inventory_selected LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-world)
include("{(ROOT/'port/engine-ui/inventory_text_v1.cmake').as_posix()}")
add_executable(player_saved_inventory_audit "{(MODULE/'tests/player_saved_inventory_v1.cpp').as_posix()}")
target_compile_features(player_saved_inventory_audit PRIVATE cxx_std_17)
target_compile_options(player_saved_inventory_audit PRIVATE -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
target_link_libraries(player_saved_inventory_audit PRIVATE dh2_level_world dh2_inventory_text_v1)
'''
 (wrapper/'CMakeLists.txt').write_text(body)
 logs=[run([cmake,'-S',wrapper,'-B',build,'-G','Ninja',f'-DCMAKE_MAKE_PROGRAM={ninja}',f'-DCMAKE_CXX_COMPILER={compiler}',f'-DCMAKE_C_COMPILER={cc}','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_FLAGS_RELEASE=-O1','-DCMAKE_C_FLAGS_RELEASE=-O1'])]
 print('Discovering actual selected compiler and reached cache inputs',flush=True)
 logs.append(run([cmake,'--build',build,'--target','player_saved_inventory_audit','--parallel','2']))
 commands=run([ninja,'-C',build,'-t','commands','player_saved_inventory_audit']);entries=selected_entries(build,commands);sources=[Path(r['file']).resolve() for r in entries]
 assert sources.count(source.resolve())==1 and sum(p.name=='script_runtime.c' for p in sources)==1
 text_names=['localization.cpp','hud_text_v1.cpp','hud_text_format_v1.cpp','item_text_varargs_v5.cpp','item_text_owner_v5.cpp']
 assert all(sources.count((ROOT/'port/engine-ui'/name).resolve())==1 for name in text_names)
 dlls=sorted(build.rglob('*.dll'));exe=build/'player_saved_inventory_audit.exe';env=os.environ.copy();env['PATH']=os.pathsep.join([str(compiler.parent),*(str(p.parent) for p in dlls),env.get('PATH','')])
 cache=a.cache.resolve();discovery=json.loads(run([exe,cache,out/'original/original-cases.bin'],env))
 evidence={Path(__file__).resolve(),MODULE/'tests/player_saved_inventory_v1_original.py',MODULE/'tests/run_player_skill_cleanup_session_v1_host.py',ROOT/'port/engine-resources/tests/cpu.py'}
 before=hashes(actual_dependencies(build,ninja,entries)|evidence)
 cache_names=[f'{table}_{suffix}.bin' for table in ['loot_table','item_powers','character_properties','character_classes','common_text'] for suffix in ['pyarray','pyarraynames','pystructnames']]+['common_text_pycst.bin','fonts_pycst.bin']
 input_paths={cache/name for name in cache_names}|{cache.parent/name for name in discovery['text_files']}
 inputs={p.relative_to(cache.parent).as_posix():sha(p) for p in sorted(input_paths)}
 profile_hash=sha(a.profile.resolve())
 print(f'Clean selected rebuild from {len(before)} guarded project inputs and {len(inputs)} reached cache inputs',flush=True)
 logs.append(run([cmake,'--build',build,'--target','player_saved_inventory_audit','--clean-first','--parallel','2']))
 imports={p.relative_to(out).as_posix():re.findall(r'DLL Name: (\S+)',run([compiler.with_name('objdump.exe'),'-p',p])) for p in [exe,*dlls]}
 exe_imports=imports[exe.relative_to(out).as_posix()]
 assert 'libdh2_level_world.dll' in exe_imports and 'libdh2_inventory_text_v1.dll' in exe_imports and 'libdh2_game_data.dll' in exe_imports
 assert sum(p.name=='libdh2_script_runtime.dll' for p in dlls)==1
 host=json.loads(run([exe,cache,out/'original/original-cases.bin'],env))
 assert host['validation']=='PASS' and host['original_caller_cases']==19 and host['actual_player_classes']==3 and not host['native_SG4_InitPost']
 raw=out/'raw-execution.json';raw.write_text(json.dumps({'validation':'EXECUTED','host':host},indent=2)+'\n')
 after_commands=run([ninja,'-C',build,'-t','commands','player_saved_inventory_audit']);after=hashes(actual_dependencies(build,ninja,selected_entries(build,after_commands))|evidence)
 changes={name:{'before':before.get(name),'after':after.get(name)} for name in before.keys()|after.keys() if before.get(name)!=after.get(name)}
 cache_changes=[name for name,value in inputs.items() if sha(cache.parent/name)!=value]
 if sha(a.profile.resolve())!=profile_hash:cache_changes.append('private-profile')
 stable=not changes and not cache_changes and commands==after_commands and discovery==host
 raw.write_text(json.dumps({'validation':'PASS' if stable else 'PROVENANCE_FAILED','host':host,'input_changes':changes,'cache_changes':cache_changes,'commands_stable':commands==after_commands,'host_stable':discovery==host},indent=2)+'\n')
 assert stable,('Selected provenance changed; raw execution retained',changes,cache_changes)
 report={'validation':'PASS','host_report':host,'original_capture':original,'source_sha256':before,'cache_inputs_sha256':inputs,'private_profile_sha256':profile_hash,'source_before_after_equal':True,'new_module_selected':True,'separate_direct_module_object':False,'scoped_source_tu_count':sources.count(source.resolve()),'script_runtime_tu_count':sum(p.name=='script_runtime.c' for p in sources),'shared_text_tu_count':5,'dependency_guard':'Actual selected compiler Ninja dependencies and configured project CMake inputs. Discovery compile and reached-file execution, snapshot, clean selected rebuild, exact closure/hash/command/cache/result comparison.','selected_dso_imports':imports,'binary_sha256':{p.relative_to(out).as_posix():sha(p) for p in [exe,*dlls]},'selected_commands':commands,'selected_compiler_objects':{Path(r['output']).resolve().relative_to(build).as_posix():Path(r['file']).resolve().relative_to(ROOT).as_posix() for r in entries},'wrapper_cmake':body,'build_stdout':logs,'scope':'Whole original 1024B GEAR caller and actual SetValue store/tail with declared stream/storage/Item callee fixtures. Actual selected sole V4 inventory, same live equipment services, V5 ItemPresentation/text/power resources, five shared text TUs, all three player classes and private actual GEAR250B. Native profile/full SG_Load4/InitPost/locality integration remains unbound; no deployment or live gameplay claim.'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({'host':host,'pinned_words':original['distinct_pinned_words'],'project_inputs':len(before),'cache_inputs':len(inputs)}))
if __name__=='__main__':main()
