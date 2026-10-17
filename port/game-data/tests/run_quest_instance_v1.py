"""Selected Quest/list/condition-factory composition using all64 original rows.

This adds no ARM body claim: separate pinned caller/list/factory gates cover
those bodies. Objective/reward factories, text allocation and virtual stream
actions are declared observing providers here. Their native implementation,
quest scripts and live gameplay are not validated by this host composition.
"""
from __future__ import annotations
import argparse,hashlib,json,os,re,shutil,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
MODULE=ROOT/'port/game-data'
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from run_player_skill_cleanup_session_v1_host import selected_entries,actual_dependencies
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser(description=__doc__)
 for name in ['compiler','cache','output','library']:p.add_argument('--'+name,required=True,type=Path)
 args=p.parse_args();out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
 library=args.library.resolve();build=next(path for path in library.parents if(path/'compile_commands.json').is_file())
 env=os.environ.copy();env['PATH']=str(args.compiler.parent)+os.pathsep+env.get('PATH','')
 ninja=shutil.which('ninja');assert ninja
 def run(command,environment=env):
  result=subprocess.run(list(map(str,command)),capture_output=True,text=True,env=environment,cwd=ROOT)
  assert result.returncode==0,result.stdout+'\n'+result.stderr
  return result.stdout
 commands=run([ninja,'-C',build,'-t','commands','dh2_game_data']);entries=selected_entries(build,commands)
 required=['quest_instance_v1.cpp','quest_runtime_fields_v1.cpp','quest_table_bindings_v1.cpp',
  'quest_condition_list_v1.cpp','quest_condition_factory_v1.cpp','quest_objective_list_v1.cpp','quest_reward_list_v1.cpp']
 assert all(sum(Path(row['file']).resolve()==(MODULE/name).resolve() for row in entries)==1 for name in required)
 assert sum(Path(row['file']).resolve()==(ROOT/'port/quest-data/quests.c').resolve() for row in entries)==1
 test=MODULE/'tests/quest_instance_v1_host.cpp'
 inputs=actual_dependencies(build,ninja,entries)|{test,Path(__file__).resolve()}
 before={path.relative_to(ROOT).as_posix():sha(path) for path in sorted(inputs)}
 dsos=sorted(library.parent.parent.rglob('*.dll'));binary_before={str(path):sha(path) for path in [library,*dsos]}
 cache=args.cache.resolve();cache_files=[cache/name for name in ['v2quests_pyarray.bin','v2quests_pyarraynames.bin']]
 cache_before={path.name:sha(path) for path in cache_files}
 exe=out/'host.exe'
 command=[args.compiler,'-std=c++17','-Wall','-Wextra','-Werror','-O2',test,library,'-o',exe]
 run(command)
 live=env.copy();live['PATH']=os.pathsep.join([*(str(path.parent) for path in dsos),env['PATH']])
 host=json.loads(run([exe,cache],live));assert host['validation']=='PASS' and host['instances']==192
 assert before=={path.relative_to(ROOT).as_posix():sha(path) for path in sorted(inputs)}
 assert commands==run([ninja,'-C',build,'-t','commands','dh2_game_data'])
 assert inputs==actual_dependencies(build,ninja,selected_entries(build,commands))|{test,Path(__file__).resolve()}
 assert binary_before=={str(path):sha(path) for path in [library,*dsos]}
 assert cache_before=={path.name:sha(path) for path in cache_files}
 imports=re.findall(r'DLL Name: (\S+)',run([args.compiler.with_name('objdump.exe'),'-p',exe]))
 assert 'libdh2_game_data.dll' in imports
 report=dict(validation='PASS',host=host,scope=__doc__,source_before_after_equal=True,
  commands_before_after_equal=True,binary_before_after_equal=True,source_sha256=before,
  binary_sha256={**binary_before,str(exe):sha(exe)},cache_sha256=cache_before,
  selected_commands=commands,binary_imports=imports,compiler_command=list(map(str,command)),android_compilation=False,live_gameplay=False)
 (out/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(host))
if __name__=='__main__':main()
