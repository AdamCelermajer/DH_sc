"""Retained Item lifetime and migrated callers through actual selected DSOs."""
from __future__ import annotations
import argparse,json,os,re,shutil
from pathlib import Path
from run_player_skill_cleanup_session_v1_host import sha,run,selected_entries,actual_dependencies,hashes
from player_inventory_retained_lifetime_v1_original import capture
from player_saved_inventory_v1_original import capture as saved_capture
from player_initial_equipment_v1_original import capture as initial_capture
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]

def main():
 p=argparse.ArgumentParser(description=__doc__)
 for name in ['compiler','cache','profile','original-elf','output']:p.add_argument('--'+name,required=True,type=Path)
 p.add_argument('--report',type=Path,default=MODULE/'reports/player-inventory-retained-lifetime-v1-selected-host.json');a=p.parse_args()
 out=a.output.resolve();wrapper=out/'wrapper';build=out/'build';wrapper.mkdir(parents=True,exist_ok=True)
 compiler=a.compiler.resolve();cc=compiler.with_name(compiler.name.replace('g++','gcc'));cmake=shutil.which('cmake');ninja=shutil.which('ninja');cache=a.cache.resolve()
 sources_required=[MODULE/'player_saved_inventory_v1.cpp',ROOT/'port/game-data/fresh_inventory_owned_v4.cpp']
 for source in sources_required:assert source.name in (source.parent/'CMakeLists.txt').read_text(),'centrally selected source required'
 original=capture(a.original_elf.resolve(),cache,out/'original')
 saved=saved_capture(a.original_elf.resolve(),cache,a.profile.resolve(),out/'saved-original')
 initial=initial_capture(a.original_elf.resolve(),out/'initial-original')
 fixtures=ROOT/'port/game-data/reference'
 targets={
  'retained_lifetime_audit':[MODULE/'tests/player_inventory_retained_lifetime_v1.cpp'],
  'saved_inventory_audit':[MODULE/'tests/player_saved_inventory_v1.cpp'],
  'initial_equipment_audit':[MODULE/'tests/player_initial_equipment_v1.cpp'],
  'inventory_text_audit':[ROOT/'port/engine-ui/tests/inventory_binding_text_queries_v1.cpp'],
  'inventory_v4_audit':[ROOT/'port/game-data/tests/fresh_inventory_owned_v4.cpp'],
  'equipment_live_audit':[ROOT/'port/game-data/tests/player_equipment_live_services_v1.cpp'],
  'gear_v5_audit':[ROOT/'port/game-data/tests/player_gear_effects_v5.cpp'],
  'loot_v7_audit':[ROOT/'port/game-data/tests/item_loot_backbone_v7.cpp',ROOT/'port/game-data/tests/loot_power_creation_v7_fixture.cpp']}
 args={
  'retained_lifetime_audit':[cache,out/'original/original-cases.bin'],
  'saved_inventory_audit':[cache,out/'saved-original/original-cases.bin'],
  'initial_equipment_audit':[cache,out/'initial-original/original-cases.bin'],
  'inventory_text_audit':[cache,fixtures/'adam-791e961-inventory-binding/query-fixtures.bin'],
  'inventory_v4_audit':[fixtures/'player-inventory-owned-v4/fixtures.bin',fixtures/'player-creation-v2/fresh-fixtures.bin',cache],
  'equipment_live_audit':[fixtures/'adam-791e961-equipment-live/requirements-fixtures.bin',fixtures/'player-item-effects-v5/starter-effects-fixtures.bin',cache],
  'gear_v5_audit':[fixtures/'player-item-effects-v5/starter-effects-fixtures.bin',cache,cache,cache,cache],
  'loot_v7_audit':[fixtures/'adam-791e961-item-loot/loot-power-creation-v7/fixtures.bin',cache]}
 body=f'''cmake_minimum_required(VERSION 3.22)
project(retained_inventory_selected LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-world)
include("{(ROOT/'port/engine-ui/inventory_text_v1.cmake').as_posix()}")
'''
 for name,files in targets.items():
  body+='add_executable('+name+' '+' '.join('"'+f.as_posix()+'"' for f in files)+')\n'
  body+='target_compile_features('+name+' PRIVATE cxx_std_17)\n'
  body+='target_compile_options('+name+' PRIVATE -fno-fast-math -ffp-contract=off)\n'
  if name in ['retained_lifetime_audit','saved_inventory_audit','initial_equipment_audit']:body+='target_compile_options('+name+' PRIVATE -Wall -Wextra -Werror)\n'
  body+='target_link_libraries('+name+' PRIVATE dh2_level_world dh2_inventory_text_v1)\n'
 (wrapper/'CMakeLists.txt').write_text(body)
 logs=[run([cmake,'-S',wrapper,'-B',build,'-G','Ninja',f'-DCMAKE_MAKE_PROGRAM={ninja}',f'-DCMAKE_CXX_COMPILER={compiler}',f'-DCMAKE_C_COMPILER={cc}','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_FLAGS_RELEASE=-O1','-DCMAKE_C_FLAGS_RELEASE=-O1'])]
 print('Discovering actual selected libraries and migrated stateful callers',flush=True)
 logs.append(run([cmake,'--build',build,'--target',*targets,'--parallel','2']))
 commands=run([ninja,'-C',build,'-t','commands',*targets]);entries=selected_entries(build,commands);sources=[Path(r['file']).resolve() for r in entries]
 assert all(sources.count(source.resolve())==1 for source in sources_required) and sum(p.name=='script_runtime.c' for p in sources)==1
 text_names=['localization.cpp','hud_text_v1.cpp','hud_text_format_v1.cpp','item_text_varargs_v5.cpp','item_text_owner_v5.cpp']
 assert all(sources.count((ROOT/'port/engine-ui'/name).resolve())==1 for name in text_names)
 dlls=sorted(build.rglob('*.dll'));exes=[build/(name+'.exe') for name in targets];env=os.environ.copy();env['PATH']=os.pathsep.join([str(compiler.parent),*(str(p.parent) for p in dlls),env.get('PATH','')])
 def execute():return {name:json.loads(run([build/(name+'.exe'),*values],env)) for name,values in args.items()}
 discovery=execute()
 evidence={Path(__file__).resolve(),MODULE/'tests/player_inventory_retained_lifetime_v1_original.py',MODULE/'tests/player_saved_inventory_v1_original.py',MODULE/'tests/player_initial_equipment_v1_original.py',MODULE/'tests/run_player_skill_cleanup_session_v1_host.py',ROOT/'port/engine-resources/tests/cpu.py',ROOT/'port/game-data/tests/fresh_inventory_owned_v4_original.py',ROOT/'port/game-data/tests/fresh_inventory_v2_original.py'}
 evidence|={v for values in args.values() for v in values if v.is_file() and ROOT in v.parents}
 before=hashes(actual_dependencies(build,ninja,entries)|evidence)
 cache_names=[f'{table}_{suffix}.bin' for table in ['loot_table','item_powers','item_powers_monopoly','character_properties','character_classes','common_text'] for suffix in ['pyarray','pyarraynames','pystructnames']]+['common_text_pycst.bin','fonts_pycst.bin']
 reached={v for report in discovery.values() for v in report.get('text_files',[])}
 input_paths={cache/name for name in cache_names}|{cache.parent/name for name in reached}
 inputs={p.relative_to(cache.parent).as_posix():sha(p) for p in sorted(input_paths)};profile_hash=sha(a.profile.resolve())
 print(f'Behavior PASS; clean selected rebuild from {len(before)} project inputs and {len(inputs)} reached cache inputs',flush=True)
 logs.append(run([cmake,'--build',build,'--target',*targets,'--clean-first','--parallel','2']))
 imports={p.relative_to(out).as_posix():re.findall(r'DLL Name: (\S+)',run([compiler.with_name('objdump.exe'),'-p',p])) for p in [*exes,*dlls]}
 for exe in exes:
  exe_imports=imports[exe.relative_to(out).as_posix()];assert 'libdh2_game_data.dll' in exe_imports
 for exe in exes[:3]:
  exe_imports=imports[exe.relative_to(out).as_posix()];assert 'libdh2_level_world.dll' in exe_imports and 'libdh2_inventory_text_v1.dll' in exe_imports
 assert sum(p.name=='libdh2_script_runtime.dll' for p in dlls)==1
 host=execute();assert all(r['validation']=='PASS' for r in host.values())
 assert host['retained_lifetime_audit']['class_caller_comparisons']==45 and host['saved_inventory_audit']['original_caller_cases']==19
 after_commands=run([ninja,'-C',build,'-t','commands',*targets]);after=hashes(actual_dependencies(build,ninja,selected_entries(build,after_commands))|evidence)
 changes={name:{'before':before.get(name),'after':after.get(name)} for name in before.keys()|after.keys() if before.get(name)!=after.get(name)}
 cache_changes=[name for name,value in inputs.items() if sha(cache.parent/name)!=value]
 if sha(a.profile.resolve())!=profile_hash:cache_changes.append('private-profile')
 stable=not changes and not cache_changes and commands==after_commands and discovery==host
 (out/'raw-execution.json').write_text(json.dumps({'validation':'PASS' if stable else 'PROVENANCE_FAILED','host':host,'input_changes':changes,'cache_changes':cache_changes,'commands_stable':commands==after_commands,'host_stable':discovery==host},indent=2)+'\n')
 assert stable,('Selected provenance changed; raw execution retained',changes,cache_changes)
 report={'validation':'PASS','host_reports':host,'original_capture':original,'saved_original_capture':saved,'initial_equipment_original_capture':initial,'source_sha256':before,'cache_inputs_sha256':inputs,'private_profile_sha256':profile_hash,'source_before_after_equal':True,'separate_direct_production_object':False,'production_tu_counts':{p.relative_to(ROOT).as_posix():sources.count(p.resolve()) for p in sources_required},'script_runtime_tu_count':sum(p.name=='script_runtime.c' for p in sources),'shared_text_tu_count':5,'selected_dso_imports':imports,'binary_sha256':{p.relative_to(out).as_posix():sha(p) for p in [*exes,*dlls]},'selected_commands':commands,'selected_compiler_objects':{Path(r['output']).resolve().relative_to(build).as_posix():Path(r['file']).resolve().relative_to(ROOT).as_posix() for r in entries},'wrapper_cmake':body,'build_stdout':logs,'scope':'Original Constructor/Split/Equip callers and required callee-prefix fixtures; actual selected sole V4 inventory, V5 Presentation, same property/equipment/text owners. Explicit synthetic Stackable row664 only for otherwise absent equippable stack source branch. Existing saved GEAR, initial equipment, V4, V5 gear, V7 loot, live equipment and shared text regressions. No native SG4/InitPost, menu, deployment or gameplay claim.'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({'host':host,'pinned_words':original['distinct_original_instruction_words'],'project_inputs':len(before),'cache_inputs':len(inputs)}))
if __name__=='__main__':main()
