"""Source creation/writers through actual selected game-data and native transport."""
import argparse,hashlib,json,os,shutil,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/game-data';REF=MODULE/'reference/player-profile-create-v1'
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from run_player_skill_cleanup_session_v1_host import selected_entries,actual_dependencies
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def hashes(paths):return {p.relative_to(ROOT).as_posix():sha(p) for p in sorted(paths)}
def main():
 p=argparse.ArgumentParser(description=__doc__)
 for n in ['compiler','original-elf','output']:p.add_argument('--'+n,type=Path,required=True)
 p.add_argument('--report',type=Path,default=MODULE/'reports/player-profile-create-v1-host-audit.json');a=p.parse_args();cxx=a.compiler.resolve();cc=cxx.with_name(cxx.name.replace('g++','gcc'));cmake=shutil.which('cmake');ninja=shutil.which('ninja');out=a.output.resolve();wrapper=out/'wrapper';build=out/'build';wrapper.mkdir(parents=True,exist_ok=True)
 env=os.environ.copy();env['PATH']=str(cxx.parent)+os.pathsep+env['PATH']
 def run(command,live=None):
  r=subprocess.run(list(map(str,command)),cwd=ROOT,env=live or env,capture_output=True,text=True)
  if r.returncode:raise RuntimeError(str(command)+'\n'+'\n'.join(r.stdout.splitlines()[-30:])+'\n'+r.stderr)
  return r.stdout
 print('Replaying original complete creation/writer/saveAll bodies',flush=True)
 run([sys.executable,MODULE/'tests/player_profile_create_v1_original.py','--original-elf',a.original_elf.resolve()]);original=json.loads((REF/'original-capture.json').read_text())
 native=ROOT/'port/android-native/app/src/main/cpp';source=MODULE/'player_profile_create_v1.cpp';selected=source.name in (MODULE/'CMakeLists.txt').read_text()
 world=ROOT/'port/level-world'
 body=f'''cmake_minimum_required(VERSION 3.22)
project(profile_create_selected LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{world.as_posix()}" selected-world)
'''
 if not selected:body+=f'target_sources(dh2_game_data PRIVATE "{source.as_posix()}")\n'
 body+=f'''add_executable(profile_create_audit "{(MODULE/'tests/player_profile_create_v1.cpp').as_posix()}" "{(native/'native_player_profile.cpp').as_posix()}" "{(native/'native_quest_owner.cpp').as_posix()}" "{(native/'native_quest_cursor.cpp').as_posix()}")
target_include_directories(profile_create_audit PRIVATE "{native.as_posix()}")
target_compile_features(profile_create_audit PRIVATE cxx_std_17)
target_compile_options(profile_create_audit PRIVATE -Wall -Wextra -Werror -fno-fast-math)
target_link_libraries(profile_create_audit PRIVATE dh2_level_world dh2_game_data)
'''
 (wrapper/'CMakeLists.txt').write_text(body)
 logs=[run([cmake,'-S',wrapper,'-B',build,'-G','Ninja',f'-DCMAKE_MAKE_PROGRAM={ninja}',f'-DCMAKE_CXX_COMPILER={cxx}',f'-DCMAKE_C_COMPILER={cc}','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_FLAGS_RELEASE=-O1','-DCMAKE_C_FLAGS_RELEASE=-O1'])]
 logs.append(run([cmake,'--build',build,'--target','profile_create_audit','--parallel','2']))
 commands=run([ninja,'-C',build,'-t','commands','profile_create_audit']);rows=selected_entries(build,commands);reached=[Path(r['file']).resolve() for r in rows]
 expected=[source,MODULE/'player_savegame_v1.cpp',MODULE/'player_save_load_owner_v1.cpp',MODULE/'player_profile_index_v1.cpp',native/'native_player_profile.cpp',native/'native_quest_owner.cpp',native/'native_quest_cursor.cpp',world/'player_save_inventory_v1.cpp',world/'player_gear_save_writer_v1.cpp'];assert all(reached.count(p.resolve())==1 for p in expected)
 # These five modules are actually imported by the inherited original CPU
 # fixture, including its memory/import leaves; they are not a header superset.
 inherited={'aggro_differential.py','combat_application_differential.py','combat_differential.py','combat_result_differential.py','health_differential.py'}
 evidence={Path(__file__).resolve(),MODULE/'tests/player_profile_create_v1_original.py',MODULE/'tests/player_profile_index_v1_original.py',MODULE/'tests/player_savegame_v1_original.py',MODULE/'tests/items_differential_v4_original.py',MODULE/'tests/navigation_differential_v4_original.py',*(MODULE/'tests'/name for name in inherited),ROOT/'port/engine-resources/tests/cpu.py',ROOT/'port/level-world/tests/run_player_skill_cleanup_session_v1_host.py',*(REF/name for name in ['original-functions.json','original-functions.asm','original-capture.json','fixtures.bin'])}
 before=hashes(actual_dependencies(build,ninja,rows)|evidence);print(f'Clean selected rebuild of {len(before)} project inputs',flush=True)
 logs.append(run([cmake,'--build',build,'--target','profile_create_audit','--clean-first','--parallel','2']))
 exe=build/'profile_create_audit.exe';dsos=sorted(build.rglob('*.dll'));live=env.copy();live['PATH']=os.pathsep.join([*(str(p.parent) for p in dsos),env['PATH']]);host=json.loads(run([exe,REF/'fixtures.bin',out/'cases'],live));assert host['validation']=='PASS'
 after_commands=run([ninja,'-C',build,'-t','commands','profile_create_audit']);after=hashes(actual_dependencies(build,ninja,selected_entries(build,after_commands))|evidence)
 changes={n:{'before':before.get(n),'after':after.get(n)} for n in before.keys()|after.keys() if before.get(n)!=after.get(n)}
 raw={'validation':'PASS' if not changes and commands==after_commands else 'PROVENANCE_FAILED','host':host,'source_changes':changes,'commands_stable':commands==after_commands};(out/'raw-replay-results.json').write_text(json.dumps(raw,indent=2)+'\n');assert raw['validation']=='PASS',changes
 imports=run([cxx.with_name('objdump.exe'),'-p',exe]);assert 'DLL Name: libdh2_game_data.dll' in imports
 report={'validation':'PASS','host_report':host,'original_comparison':original,'original_sha256':sha(a.original_elf.resolve()),'source_sha256':before,'source_before_after_equal':True,'new_module_selected':selected,'source_tu_counts':{p.name:reached.count(p.resolve()) for p in expected},'selected_compiler_objects':{Path(r['output']).resolve().relative_to(build).as_posix():Path(r['file']).resolve().relative_to(ROOT).as_posix() for r in rows},'selected_commands':commands,'wrapper_cmake':body,'binary_sha256':{p.relative_to(out).as_posix():sha(p) for p in [exe,*dsos]},'build_stdout':logs,'dependency_guard':'Actual selected object Ninja dependencies/configured CMake plus exact original/test/reference files. Discovery, snapshot, clean rebuild, run, compare source and command closures. No adjacent-header superset.','scope':'NativeCreate two-string valid body, indexed metadata fields/SG_Load1 registration, full offline SG_Save order, seven metadata writer bodies and saveAll stream bytes. AS conversion, slot catalogue, real clocks/online/I/O/cache publication and destructor are explicit original leaves. Selected host uses actual native Transport with explicit new-file creation, flushed atomic file publication and distinct gameplay Save SG_Load1 readback; directory power-loss durability is not claimed. Full online save/volatile body, gameplay InitPost/property/VM/grants and registration remain external requirements.'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','host':host,'original_words':original['executed_words'],'project_inputs':len(before)}))
if __name__=='__main__':main()
