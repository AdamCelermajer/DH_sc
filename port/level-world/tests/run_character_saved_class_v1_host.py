"""Saved-class and PROP replay through actual selected world/data libraries."""
import argparse,hashlib,json,os,shutil,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/level-world';REF=MODULE/'reference/character-saved-class-v1'
sys.path.insert(0,str(MODULE/'tests'))
from run_player_skill_cleanup_session_v1_host import selected_entries,actual_dependencies
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def hashes(paths):return {p.relative_to(ROOT).as_posix():sha(p) for p in sorted(paths)}
def main():
 p=argparse.ArgumentParser(description=__doc__)
 for name in ['compiler','original-elf','cache','output']:p.add_argument('--'+name,required=True,type=Path)
 p.add_argument('--report',type=Path,default=MODULE/'reports/character-saved-class-v1-host-audit.json');a=p.parse_args()
 cxx=a.compiler.resolve();cc=cxx.with_name(cxx.name.replace('g++','gcc'));cmake=shutil.which('cmake');ninja=shutil.which('ninja');out=a.output.resolve();wrapper=out/'wrapper';build=out/'build';wrapper.mkdir(parents=True,exist_ok=True)
 env=os.environ.copy();env['PATH']=str(cxx.parent)+os.pathsep+env.get('PATH','')
 def run(command,custom_env=None):
  r=subprocess.run(list(map(str,command)),cwd=ROOT,env=custom_env or env,capture_output=True,text=True)
  if r.returncode:raise RuntimeError(f'{command}\nexit={r.returncode}\n'+ '\n'.join(r.stdout.splitlines()[-35:])+'\n'+r.stderr)
  return r.stdout
 print('Regenerating complete original caller/body gold',flush=True)
 run([sys.executable,MODULE/'tests/character_saved_class_v1_original.py','--original-elf',a.original_elf.resolve()])
 original=json.loads((REF/'original-capture.json').read_text());source=MODULE/'character_saved_class_v1.cpp';test=MODULE/'tests/character_saved_class_v1.cpp';selected=source.name in (MODULE/'CMakeLists.txt').read_text()
 body=f'''cmake_minimum_required(VERSION 3.22)
project(saved_class_selected LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-world)
'''
 if not selected:body+=f'target_sources(dh2_level_world PRIVATE "{source.as_posix()}")\n'
 helper=MODULE/'character_template_random.cpp';helper_selected=helper.name in (MODULE/'CMakeLists.txt').read_text()
 if not helper_selected:body+=f'target_sources(dh2_level_world PRIVATE "{helper.as_posix()}")\n'
 body+=f'''add_executable(saved_class_audit "{test.as_posix()}")
target_compile_features(saved_class_audit PRIVATE cxx_std_17)
target_compile_options(saved_class_audit PRIVATE -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
target_link_libraries(saved_class_audit PRIVATE dh2_level_world dh2_game_data)
'''
 (wrapper/'CMakeLists.txt').write_text(body)
 logs=[run([cmake,'-S',wrapper,'-B',build,'-G','Ninja',f'-DCMAKE_MAKE_PROGRAM={ninja}',f'-DCMAKE_CXX_COMPILER={cxx}',f'-DCMAKE_C_COMPILER={cc}','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_FLAGS_RELEASE=-O1','-DCMAKE_C_FLAGS_RELEASE=-O1'])]
 print('Discovering actual selected target dependencies',flush=True)
 logs.append(run([cmake,'--build',build,'--target','saved_class_audit','--parallel','2']))
 commands=run([ninja,'-C',build,'-t','commands','saved_class_audit']);rows=selected_entries(build,commands);reached=[Path(r['file']).resolve() for r in rows]
 expected=[source,ROOT/'port/game-data/player_savegame_v1.cpp',ROOT/'port/game-data/player_save_load_owner_v1.cpp',MODULE/'character_template_random.cpp']
 assert all(reached.count(q.resolve())==1 for q in expected)
 inherited={'aggro_differential.py','combat_application_differential.py','combat_differential.py','combat_result_differential.py','health_differential.py'}
 evidence={Path(__file__).resolve(),MODULE/'tests/run_player_skill_cleanup_session_v1_host.py',MODULE/'tests/character_saved_class_v1_original.py',ROOT/'port/game-data/tests/items_differential_v4_original.py',ROOT/'port/game-data/tests/navigation_differential_v4_original.py',*(ROOT/'port/game-data/tests'/name for name in inherited),ROOT/'port/engine-resources/tests/cpu.py',*(REF/name for name in ['original-functions.json','original-functions.asm','original-capture.json','class-fixtures.bin','prop-fixtures.bin'])}
 before=hashes(actual_dependencies(build,ninja,rows)|evidence)
 cache=a.cache.resolve();inputs={n:sha(cache/n) for n in ['dh2_000.savegame','data/pydata/character_properties_pyarray.bin','data/pydata/character_properties_pyarraynames.bin','data/pydata/character_properties_pystructnames.bin']}
 print(f'Clean selected rebuild from {len(before)} project inputs',flush=True)
 logs.append(run([cmake,'--build',build,'--target','saved_class_audit','--clean-first','--parallel','2']))
 dsos=sorted(build.rglob('*.dll'));exe=build/'saved_class_audit.exe';live_env=env.copy();live_env['PATH']=os.pathsep.join([*(str(x.parent) for x in dsos),env['PATH']])
 host=json.loads(run([exe,REF,cache],live_env));assert host['validation']=='PASS' and host['mismatches']==0
 after_commands=run([ninja,'-C',build,'-t','commands','saved_class_audit']);after_rows=selected_entries(build,after_commands);after=hashes(actual_dependencies(build,ninja,after_rows)|evidence)
 changes={n:{'before':before.get(n),'after':after.get(n)} for n in before.keys()|after.keys() if before.get(n)!=after.get(n)};cache_changes=[n for n,h in inputs.items() if sha(cache/n)!=h]
 raw={'validation':'PASS' if not changes and not cache_changes and commands==after_commands else 'PROVENANCE_FAILED','host':host,'source_changes':changes,'cache_changes':cache_changes,'commands_stable':commands==after_commands}
 (out/'raw-replay-results.json').write_text(json.dumps(raw,indent=2)+'\n');assert raw['validation']=='PASS'
 imports=run([cxx.with_name('objdump.exe'),'-p',exe]);assert 'DLL Name: libdh2_level_world.dll' in imports and 'DLL Name: libdh2_game_data.dll' in imports
 report={'validation':'PASS','host_report':host,'original_comparison':original,'original_sha256':sha(a.original_elf),'source_sha256':before,'cache_inputs_sha256':inputs,'source_before_after_equal':True,'new_module_selected':selected,'source_tu_counts':{q.name:reached.count(q.resolve()) for q in expected},'selected_compiler_objects':{Path(r['output']).resolve().relative_to(build).as_posix():Path(r['file']).resolve().relative_to(ROOT).as_posix() for r in rows},'selected_commands':commands,'wrapper_cmake':body,'binary_sha256':{p.relative_to(out).as_posix():sha(p) for p in [exe,*dsos]},'build_stdout':logs,'dependency_guard':'Actual selected object Ninja dependencies and configured CMake inputs plus exact replay scripts/reference files. Discovery, snapshot, clean selected build, execution and hash/command closure comparison; no adjacent-header superset.','scope':'Whole source SafeGetCharPropsId valid table domains, source template lookup/RNG/cache and gameplay Save load/class writeback. Whole nonnull-Character PROP reader with real nested type/property getters and bounded failure prefixes. Same borrowed Save/property/profile/RNG controls; no metadata copy, new native slot/association, full InitPost, registration, VM/vitals or native gameplay startup claim.'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','host':host,'original_words':original['executed_words'],'selected':selected,'project_inputs':len(before)}))
if __name__=='__main__':main()
