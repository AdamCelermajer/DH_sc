"""Native campaign cursor adapter over the actual selected profile-index library.

No ARM parity, complete typed readers, Android or live save loading is claimed.
All inputs are synthetic; no user save is opened.
"""
import argparse,hashlib,json,os,re,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser(description=__doc__)
 for name in ['compiler','library','output']:p.add_argument('--'+name,type=Path,required=True)
 p.add_argument('--composition',action='store_true',help='Replay synthetic QEST through selected typed readers and actual Quest/objective stores')
 p.add_argument('--cache',type=Path);p.add_argument('--world-library',type=Path)
 a=p.parse_args();out=a.output.resolve();out.mkdir(parents=True,exist_ok=True)
 if a.composition and (not a.cache or not a.world_library):p.error('--composition requires --cache and --world-library')
 library=a.library.resolve();dll=library.parent/'libdh2_game_data.dll'
 build=next(path for path in library.parents if(path/'compile_commands.json').is_file())
 commands=json.loads((build/'compile_commands.json').read_text())
 index=ROOT/'port/game-data/player_profile_index_v1.cpp'
 selected=[row for row in commands if Path(row['file']).resolve()==index.resolve()];assert len(selected)==1
 source=ROOT/'port/android-native/app/src/main/cpp/native_quest_cursor.cpp'
 test=ROOT/'port/android-native/tests'/('native_quest_payload_stream.cpp' if a.composition else 'native_quest_cursor.cpp')
 files=[source,source.with_suffix('.hpp'),test,Path(__file__).resolve(),index,index.with_suffix('.hpp'),ROOT/'port/game-data/player_saved_quests_v1.hpp']
 before={path.relative_to(ROOT).as_posix():sha(path) for path in files};binary={str(path):sha(path) for path in [library,dll]}
 exe=out/'host.exe';env=os.environ.copy();env['PATH']=os.pathsep.join([str(library.parent),str(a.compiler.parent),env['PATH']])
 if a.composition:
  world=a.world_library.resolve();dsos=sorted(library.parent.parent.rglob('*.dll'))
  binary.update({str(path):sha(path) for path in [world,*dsos]})
  env['PATH']=os.pathsep.join([*(str(path.parent) for path in dsos),env['PATH']])
  needed=['quest_objective_payload_v1.cpp','quest_stream_read_v1.cpp','player_saved_quests_v1.cpp','quest_runtime_fields_v1.cpp','quest_objective_list_v1.cpp','quest_objective_factory_v1.cpp','quest_table_bindings_v1.cpp']
  for name in needed:assert sum(Path(row['file']).resolve()==(ROOT/'port/game-data'/name).resolve() for row in commands)==1
 def run(command):
  result=subprocess.run(list(map(str,command)),cwd=ROOT,env=env,capture_output=True,text=True)
  assert result.returncode==0,result.stdout+'\n'+result.stderr
  return result.stdout
 command=[a.compiler,'-std=c++17','-Wall','-Wextra','-Werror','-O2','-I'+str(ROOT/'port/game-data'),test,source,library,'-o',exe]
 if a.composition:command.insert(-2,world)
 run(command);host=json.loads(run([exe,a.cache.resolve()] if a.composition else [exe]));assert host['validation']=='PASS'
 imports=re.findall(r'DLL Name: (\S+)',run([a.compiler.with_name('objdump.exe'),'-p',exe]));assert 'libdh2_game_data.dll' in imports
 assert before=={path.relative_to(ROOT).as_posix():sha(path) for path in files}
 assert binary=={path:sha(Path(path)) for path in binary}
 report={'validation':'PASS','host':host,'scope':__doc__,'source_sha256':before,
  'binary_sha256':{**binary,str(exe):sha(exe)},'compiler_command':list(map(str,command)),
  'selected_profile_index_commands':selected,'binary_imports':imports,'android_compilation':False,'live_gameplay':False}
 (out/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(host))
if __name__=='__main__':main()
