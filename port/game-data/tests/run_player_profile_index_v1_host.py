"""Campaign index/SG_Load/metadata proof through the actual selected game DSO."""
from __future__ import annotations
import argparse,hashlib,json,os,shutil,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
MODULE=ROOT/'port/game-data';REF=MODULE/'reference/player-profile-index-v1'
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from run_player_skill_cleanup_session_v1_host import selected_entries,actual_dependencies

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def hashes(paths):return {p.relative_to(ROOT).as_posix():sha(p) for p in sorted(paths)}
def main():
 p=argparse.ArgumentParser(description=__doc__)
 for name in ('compiler','cache','output','original-elf'):p.add_argument('--'+name,required=True,type=Path)
 p.add_argument('--report',type=Path,default=MODULE/'reports/player-profile-index-v1-host-audit.json')
 a=p.parse_args();out=a.output.resolve();wrapper=out/'wrapper';build=out/'build';wrapper.mkdir(parents=True,exist_ok=True)
 cxx=a.compiler.resolve();cc=cxx.with_name(cxx.name.replace('g++','gcc'));cmake=shutil.which('cmake');ninja=shutil.which('ninja')
 env=os.environ.copy();env['PATH']=str(cxx.parent)+os.pathsep+env.get('PATH','');logs=[]
 def run(command,custom_env=None):
  r=subprocess.run(list(map(str,command)),cwd=ROOT,env=custom_env or env,capture_output=True,text=True)
  if r.returncode:raise RuntimeError(f'{command}\nexit={r.returncode}\n{r.stdout}\n{r.stderr}')
  return r.stdout
 sources=[MODULE/'player_profile_index_v1.cpp',MODULE/'player_save_load_owner_v1.cpp',MODULE/'player_profile_filename_v1.cpp']
 selected={p.name:p.name in (MODULE/'CMakeLists.txt').read_text() for p in sources}
 body=f'''cmake_minimum_required(VERSION 3.22)
project(player_profile_selected LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-game)
'''
 for source in sources:
  if not selected[source.name]:body+=f'target_sources(dh2_game_data PRIVATE "{source.as_posix()}")\n'
 body+=f'''add_executable(player_profile_audit "{(MODULE/'tests/player_profile_index_v1.cpp').as_posix()}")
target_compile_features(player_profile_audit PRIVATE cxx_std_17)
target_compile_options(player_profile_audit PRIVATE -Wall -Wextra -Werror -fno-fast-math)
target_link_libraries(player_profile_audit PRIVATE dh2_game_data)
'''
 (wrapper/'CMakeLists.txt').write_text(body)
 logs.append(run([cmake,'-S',wrapper,'-B',build,'-G','Ninja',f'-DCMAKE_MAKE_PROGRAM={ninja}',f'-DCMAKE_CXX_COMPILER={cxx}',f'-DCMAKE_C_COMPILER={cc}','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_FLAGS_RELEASE=-O1','-DCMAKE_C_FLAGS_RELEASE=-O1']))
 print('Discovering actual selected game-data dependencies',flush=True)
 logs.append(run([cmake,'--build',build,'--target','player_profile_audit','--parallel','2']))
 commands=run([ninja,'-C',build,'-t','commands','player_profile_audit']);rows=selected_entries(build,commands)
 reached=[Path(r['file']).resolve() for r in rows]
 for source in [*sources,MODULE/'player_savegame_v1.cpp']:assert reached.count(source.resolve())==1
 assert all('selected-game/CMakeFiles/dh2_game_data.dir/' in r['output'].replace('\\','/') for r in rows if Path(r['file']).resolve() in sources)
 evidence={Path(__file__).resolve(),MODULE/'tests/player_profile_index_v1_original.py',
  MODULE/'tests/player_savegame_v1_original.py',MODULE/'tests/items_differential_v4_original.py',
  MODULE/'tests/navigation_differential_v4_original.py',ROOT/'port/engine-resources/tests/cpu.py',
  ROOT/'port/level-world/tests/run_player_skill_cleanup_session_v1_host.py',
  *(REF/name for name in ('original-functions.json','original-functions.asm','original-capture.json','fixtures.bin','load-fixtures.bin','metadata-fixtures.bin','section-callbacks.json')),
  MODULE/'reference/player-profile-filename-v1/original-capture.json'}
 evidence.update(REF/name for name in ('NOTES.md','upstream-attribution.json'))
 paths=actual_dependencies(build,ninja,rows)|evidence;before=hashes(paths)
 cache=a.cache.resolve();cache_paths=[cache/'dh2_000.savegame',*(cache/'data/pydata'/('character_properties'+suffix) for suffix in ('_pyarray.bin','_pyarraynames.bin','_pystructnames.bin'))]
 inputs={p.relative_to(cache).as_posix():sha(p) for p in cache_paths}
 # Regenerated original gold is pinned to the real ELF. Only synthetic
 # metadata bodies appear in public gold, never campaign/name payload bytes.
 from player_profile_index_v1_original import evidence as original_evidence
 functions,_=original_evidence(a.original_elf.resolve())
 original=json.loads((REF/'original-capture.json').read_text())
 assert original['original_sha256']==sha(a.original_elf) and original['reader_bodies']==7
 assert all(sha(ROOT/name)==h for name,h in original['generator_sha256'].items())
 assert original['character_names_sha256']==inputs['data/pydata/character_properties_pyarraynames.bin']
 assert json.loads((REF/'original-functions.json').read_text())['functions']==functions
 assert all(sha(REF/name)==h for name,h in original['fixture_sha256'].items())
 print(f'Rebuilding actual selected game-data from {len(before)} snapshotted project inputs',flush=True)
 logs.append(run([cmake,'--build',build,'--target','player_profile_audit','--clean-first','--parallel','2']))
 dll=build/'selected-game/libdh2_game_data.dll';exe=build/'player_profile_audit.exe'
 live_env=env.copy();live_env['PATH']=str(dll.parent)+os.pathsep+live_env['PATH']
 host=json.loads(run([exe,REF,cache],live_env));assert host['validation']=='PASS' and host['mismatches']==0
 filename_gold=json.loads((MODULE/'reference/player-profile-filename-v1/original-capture.json').read_text())
 filename_checks=0
 for case in filename_gold['cases']:
  got=json.loads(run([exe,'--filename',case['slot'],case['checkpoint'],case['multiplayer']],live_env))
  assert got==case['filename'],(case,got);filename_checks+=1
 assert filename_checks==128
 after_commands=run([ninja,'-C',build,'-t','commands','player_profile_audit']);after_rows=selected_entries(build,after_commands)
 after=hashes(actual_dependencies(build,ninja,after_rows)|evidence)
 changes={n:{'before':before.get(n),'after':after.get(n)} for n in before.keys()|after.keys() if before.get(n)!=after.get(n)}
 cache_changes=[n for n,h in inputs.items() if sha(cache/n)!=h]
 raw={'validation':'PASS' if not changes and not cache_changes and commands==after_commands else 'PROVENANCE_FAILED',
      'host':host,'filename_original_cases':filename_checks,'source_changes':changes,'cache_changes':cache_changes,'commands_stable':commands==after_commands}
 (out/'raw-replay-results.json').write_text(json.dumps(raw,indent=2)+'\n')
 assert raw['validation']=='PASS',f'Raw execution retained but production inputs changed: {changes}, cache={cache_changes}'
 imports=run([cxx.with_name('objdump.exe'),'-p',exe]);assert 'DLL Name: libdh2_game_data.dll' in imports
 report={'validation':'PASS','host_report':host,'original_comparison':original,'filename_original_cases':filename_checks,
  'upstream_commit':'791e961b12233100b303038c961666834f4beb9d','original_sha256':sha(a.original_elf),
  'source_sha256':before,'cache_inputs_sha256':inputs,'source_before_after_equal':True,
  'new_modules_selected':selected,'source_tu_counts':{p.name:reached.count(p.resolve()) for p in [*sources,MODULE/'player_savegame_v1.cpp']},
  'selected_compiler_objects':{Path(r['output']).resolve().relative_to(build).as_posix():Path(r['file']).resolve().relative_to(ROOT).as_posix() for r in rows},
  'selected_commands':commands,'wrapper_cmake':body,'binary_sha256':{p.relative_to(out).as_posix():sha(p) for p in (exe,dll)},'build_stdout':logs,
  'dependency_guard':'Actual selected target object Ninja dependencies and configured CMake inputs plus exact replay/reference scripts and fixtures; discovery build, snapshot, clean selected build, execute, compare closure/hashes/commands. No adjacent-file superset.',
  'scope':'Actual source campaign index, complete SG_Load mask/slot/profile/global ordering, all seven metadata readers through the sole borrowed Save. 128 source filename cases. Real private campaign mask1 reads with anonymized outputs only. File construction/I/O, non-metadata init/sections and online/quest globals are mandatory external services. Source writer-registration/persistence, backup policy, gameplay Save association/profile startup, VM/vitals and full campaign load remain outside this evidence.'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({'validation':'PASS','host':host,'filename_original_cases':filename_checks,'selected':selected,'project_inputs':len(before),'report':str(a.report)}))
if __name__=='__main__':main()
