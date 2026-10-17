"""Borrowed dead focus composition over actual selected world and VM DSOs."""
from __future__ import annotations
import argparse,hashlib,json,os,re,shutil,subprocess
from pathlib import Path
from run_player_skill_cleanup_session_v1_host import selected_entries,actual_dependencies
from character_dead_focus_source import capture
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def run(command,env=None):
 r=subprocess.run(list(map(str,command)),cwd=ROOT,env=env,capture_output=True,text=True)
 if r.returncode:raise RuntimeError(f'{command}\n{r.stdout}\n{r.stderr}')
 return r.stdout
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler',type=Path,required=True);p.add_argument('--cache',type=Path,required=True);p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--state-reference',type=Path);p.add_argument('--report',type=Path,default=MODULE/'reports/character-dead-focus-services-v1-selected-host.json');a=p.parse_args()
 out=a.output.resolve();out.mkdir(parents=True,exist_ok=True);reference=MODULE/'reference/character-dead-focus-services-v1';original=capture(a.original_elf.resolve(),reference)
 cxx=a.compiler.resolve();cc=cxx.with_name(cxx.name.replace('g++','gcc'));cmake=shutil.which('cmake');ninja=shutil.which('ninja');wrapper=out/'wrapper';build=out/'build';wrapper.mkdir(exist_ok=True)
 test=MODULE/'tests/character_dead_focus_services_v1.cpp';source=MODULE/'character_dead_focus_services_v1.cpp';backend=ROOT/'port/android-native/app/src/main/cpp/native_debug_files.cpp';names=ROOT/'port/pydata-names/names.c'
 assert source.name in (MODULE/'CMakeLists.txt').read_text(),'Root must select actual source centrally'
 body=f'''cmake_minimum_required(VERSION 3.22)
project(dead_focus_selected LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-world)
add_executable(dead_focus_audit "{test.as_posix()}" "{backend.as_posix()}" "{names.as_posix()}")
target_compile_features(dead_focus_audit PRIVATE cxx_std_17)
target_compile_options(dead_focus_audit PRIVATE -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
target_link_libraries(dead_focus_audit PRIVATE dh2_level_world dh2_script_runtime)
add_executable(existing_state_audit "{(MODULE/'tests/character_state.cpp').as_posix()}")
target_compile_features(existing_state_audit PRIVATE cxx_std_17)
target_compile_options(existing_state_audit PRIVATE -Wall -Wextra -Werror)
target_link_libraries(existing_state_audit PRIVATE dh2_level_world)
''';(wrapper/'CMakeLists.txt').write_text(body)
 logs=[run([cmake,'-S',wrapper,'-B',build,'-G','Ninja',f'-DCMAKE_MAKE_PROGRAM={ninja}',f'-DCMAKE_CXX_COMPILER={cxx}',f'-DCMAKE_C_COMPILER={cc}','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_FLAGS_RELEASE=-O1','-DCMAKE_C_FLAGS_RELEASE=-O1'])]
 targets=['dead_focus_audit']+(['existing_state_audit'] if a.state_reference else [])
 print('Building actual selected world/VM dead-focus gate',flush=True);logs.append(run([cmake,'--build',build,'--target',*targets,'--parallel','2']))
 commands=run([ninja,'-C',build,'-t','commands',*targets]);entries=selected_entries(build,commands);files=[Path(r['file']).resolve() for r in entries];assert files.count(source.resolve())==1 and sum(p.name=='script_runtime.c' for p in files)==1
 evidence={Path(__file__).resolve(),MODULE/'tests/character_dead_focus_source.py',MODULE/'tests/run_player_skill_cleanup_session_v1_host.py',reference/'original-functions.json',reference/'original-functions.asm',reference/'NOTES.md'}
 if a.state_reference:evidence|={MODULE/'tests/character_state_differential.py',MODULE/'reference/character-state/original-functions.json'}
 paths=actual_dependencies(build,ninja,entries)|evidence;before={p.relative_to(ROOT).as_posix():sha(p) for p in paths}
 dsos=sorted(build.rglob('*.dll'));exe=build/'dead_focus_audit.exe';env=os.environ.copy();env['PATH']=os.pathsep.join([str(cxx.parent),*(str(x.parent) for x in dsos),env.get('PATH','')])
 host=json.loads(run([exe,a.cache.resolve(),out/'real-debug'],env));assert host['validation']=='PASS' and host['actual_skill_selection_cases']==3
 prior=json.loads(run([build/'existing_state_audit.exe',a.state_reference.resolve()],env)) if a.state_reference else None
 assert before=={p:sha(ROOT/p) for p in before},'Selected source changed during execution'
 imports={p.relative_to(out).as_posix():re.findall(r'DLL Name: (\S+)',run([cxx.with_name('objdump.exe'),'-p',p])) for p in [exe,*dsos]}
 assert 'libdh2_level_world.dll' in imports[exe.relative_to(out).as_posix()] and sum(p.name=='libdh2_script_runtime.dll' for p in dsos)==1
 cache=a.cache.resolve();names={'DebugSwitches.savegame',*host['resolved_cache_resources']}
 for stem in ['skills','faeries','character_properties','character_classes','ai','ai_factions']:
  names.update('data/pydata/'+stem+suffix+'.bin' for suffix in ['_pyarray','_pyarraynames','_pystructnames'])
 names.update('data/pydata/'+name+'.bin' for name in ['effects_pyarraynames','projectiles_pyarraynames','faeries_pycst','design_pycst','ai_pycst']);inputs={name:sha(cache/name) for name in sorted(names)}
 report={'validation':'PASS','host_report':host,'existing_state_report':prior,'existing_state_fixture_sha256':sha(a.state_reference) if a.state_reference else None,'original_evidence':original,'source_sha256':before,'cache_inputs_sha256':inputs,'new_module_selected':True,'scoped_source_tu_count':1,'same_selected_vm_dso':True,'staged_source':False,'selected_commands':commands,'selected_dso_imports':imports,'binary_sha256':{p.relative_to(out).as_posix():sha(p) for p in [exe,*dsos,*( [build/'existing_state_audit.exe'] if a.state_reference else [])]},'wrapper_cmake':body,'build_stdout':logs,'scope':'Actual cache three Player property/class/AI/skill tables, sole BuffOwner/Coordinator and actual Debug filesystem/map owner. Selected adapter drives exact dead-focus services; state animation/controller/events and positive FX drop are explicit test callees. Positive cached Sneak is a declared read fact to test boundaries, not a native producer. Actual immutable tables/same VM are retained for cancellation. Static original byte/xref evidence; optional existing state fixture replay counts the added mandatory prelude separately because original Debug callees remain fixtures. No native positive FX, full AI frame or Kill continuation claim.'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(host))
if __name__=='__main__':main()
