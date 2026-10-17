"""Compose native Quest startup with selected source bodies and original cache.

All3 factories, Quest/child constructors, assignment/ReInit and destructors are
real selected implementations. Constants query the existing world C decoder.
With --payload, a synthetic whole-campaign profile carries nonempty SKIL, FAES,
QEST and PROP sections through Character::load masks 2 and 4, borrowing the
exact Save, LoadOwner and embedded Quest fields already owned by one
PlayerProfile Transport. The same Save receives real SkillTables, faery state
and the native Quest owner with one borrowed cursor, plus typed properties on
the same PropertyState. Offline Online is explicit test state; Android is
smoke-tested, while live Character::InitPost/profile restore remains separate
verification.
"""
from __future__ import annotations
import argparse,hashlib,json,os,re,shutil,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from run_player_skill_cleanup_session_v1_host import selected_entries,actual_dependencies
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser(description=__doc__)
 for name in ['compiler','cache','output','library','world-library']:p.add_argument('--'+name,required=True,type=Path)
 p.add_argument('--payload',action='store_true',help='Compose Character masks 2/4 and SKIL/FAES/QEST/PROP on the Transport-owned Save')
 args=p.parse_args();out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
 library=args.library.resolve();world=args.world_library.resolve();build=next(path for path in library.parents if(path/'compile_commands.json').is_file())
 env=os.environ.copy();env['PATH']=str(args.compiler.parent)+os.pathsep+env.get('PATH','');ninja=shutil.which('ninja');assert ninja
 def run(command,environment=env):
  result=subprocess.run(list(map(str,command)),cwd=ROOT,capture_output=True,text=True,env=environment)
  assert result.returncode==0,result.stdout+'\n'+result.stderr
  return result.stdout
 commands=run([ninja,'-C',build,'-t','commands','dh2_level_world']);entries=selected_entries(build,commands)
 names=['quest_savegame_v1.cpp','quest_runtime_fields_v1.cpp','quest_instance_v1.cpp',
  'quest_table_bindings_v1.cpp','quest_condition_list_v1.cpp','quest_objective_list_v1.cpp',
  'quest_reward_list_v1.cpp','quest_condition_factory_v1.cpp','quest_objective_factory_v1.cpp','quest_reward_factory_v1.cpp',
  'quest_stream_read_v1.cpp','quest_objective_payload_v1.cpp','quest_compile_v1.cpp']
 for name in names:assert sum(Path(row['file']).resolve()==(ROOT/'port/game-data'/name).resolve() for row in entries)==1
 assert sum(Path(row['file']).resolve()==(ROOT/'port/pydata-constants/constants.c').resolve() for row in entries)==1
 source=ROOT/'port/android-native/app/src/main/cpp/native_quest_owner.cpp';test=ROOT/'port/android-native/tests'/('native_quest_owner_payload.cpp' if args.payload else 'native_quest_owner.cpp')
 cursor=source.with_name('native_quest_cursor.cpp')
 character_list_source=ROOT/'port/android-native/app/src/main/cpp/native_character_list.cpp'
 quest_completed_source=ROOT/'port/game-data/quest_update_completed_v1.cpp'
 profile_source=ROOT/'port/android-native/app/src/main/cpp/native_player_profile.cpp'
 profile_header=profile_source.with_suffix('.hpp')
 gameplay_header=ROOT/'port/level-world/character_gameplay_save_v1.hpp'
 kill_kernel=ROOT/'port/quest-kill/quest.c'
 evidence={source,source.with_suffix('.hpp'),source.with_name('native_current_level_character_population_v1.hpp'),cursor,cursor.with_suffix('.hpp'),character_list_source,quest_completed_source,profile_source,profile_header,gameplay_header,kill_kernel,test,Path(__file__).resolve()}
 inputs=actual_dependencies(build,ninja,entries)|evidence
 before={path.relative_to(ROOT).as_posix():sha(path) for path in sorted(inputs)}
 # Desktop CMake places shared libraries beside the executable rather than
 # beside their import libraries. Keep both layouts in the runtime evidence.
 dsos=sorted(set(library.parent.parent.rglob('*.dll'))|set((build/'bin').glob('*.dll')));binary_before={str(path):sha(path) for path in [library,world,*dsos]}
 cache=args.cache.resolve();files=[cache/name for name in ['v2quests_pyarray.bin','v2quests_pyarraynames.bin','v2quests_pycst.bin']]
 condition_cache=cache.parent/'original-cache'/'data'/'pydata'
 files += [condition_cache/name for name in ['v2conditions_pyarray.bin','v2conditions_pyarraynames.bin','v2conditions_pystructnames.bin','v2conditions_pycst.bin']]
 cache_before={str(path.relative_to(cache.parent)):sha(path) for path in files}
 exe=out/'host.exe'
 command=[args.compiler,'-std=c++17','-Wall','-Wextra','-Werror','-O2','-I'+str(ROOT/'port/game-data'),'-I'+str(ROOT/'port/pydata-constants'),'-I'+str(ROOT/'port/level-world'),'-I'+str(ROOT/'port/engine-animation'),'-I'+str(ROOT/'port/engine-skinning'),'-I'+str(ROOT/'port/scene-materials'),test,source,cursor,profile_source,character_list_source,quest_completed_source,kill_kernel,library,world,'-o',exe]
 run(command)
 live=env.copy();live['PATH']=os.pathsep.join([*(str(path.parent) for path in dsos),env['PATH']]);host=json.loads(run([exe,cache],live));assert host['validation']=='PASS'
 assert before=={path.relative_to(ROOT).as_posix():sha(path) for path in sorted(inputs)}
 assert commands==run([ninja,'-C',build,'-t','commands','dh2_level_world'])
 assert inputs==actual_dependencies(build,ninja,entries)|evidence
 assert binary_before=={str(path):sha(path) for path in [library,world,*dsos]}
 assert cache_before=={str(path.relative_to(cache.parent)):sha(path) for path in files}
 imports=re.findall(r'DLL Name: (\S+)',run([args.compiler.with_name('objdump.exe'),'-p',exe]))
 assert 'libdh2_game_data.dll' in imports and 'libdh2_level_world.dll' in imports
 report=dict(validation='PASS',host=host,scope=__doc__,source_before_after_equal=True,commands_before_after_equal=True,
  binary_before_after_equal=True,source_sha256=before,binary_sha256={**binary_before,str(exe):sha(exe)},
  cache_sha256=cache_before,selected_commands=commands,binary_imports=imports,compiler_command=list(map(str,command)),android_compilation=False,live_gameplay=False)
 (out/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(host))
if __name__=='__main__':main()
