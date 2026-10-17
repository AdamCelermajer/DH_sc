"""Run the Character::Kill tail against the selected host libraries and real cache."""
from __future__ import annotations
import argparse, hashlib, json, os, re, subprocess
from pathlib import Path

ROOT=Path(__file__).resolve().parents[3]

def sha(path:Path)->str:return hashlib.sha256(path.read_bytes()).hexdigest()

def main()->None:
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ('compiler','cache','output','library','world-library'):
        parser.add_argument('--'+name,required=True,type=Path)
    args=parser.parse_args();out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    compiler=args.compiler.resolve();library=args.library.resolve();world=args.world_library.resolve()
    env=os.environ.copy();env['PATH']=str(compiler.parent)+os.pathsep+env.get('PATH','')
    def run(command,environment=env):
        result=subprocess.run(list(map(str,command)),cwd=ROOT,capture_output=True,text=True,env=environment)
        if result.returncode:raise RuntimeError(result.stdout+'\n'+result.stderr)
        return result.stdout
    source=ROOT/'port/android-native/app/src/main/cpp/native_quest_owner.cpp'
    cursor=source.with_name('native_quest_cursor.cpp')
    test=ROOT/'port/android-native/tests/character_kill_quest_owner_integration.cpp'
    runner=Path(__file__).resolve()
    evidence={source,source.with_suffix('.hpp'),cursor,cursor.with_suffix('.hpp'),
      ROOT/'port/level-world/character_gameplay_save_v1.hpp',
      ROOT/'port/quest-kill/quest.c',test,runner}
    before={path.relative_to(ROOT).as_posix():sha(path) for path in sorted(evidence)}
    binaries=sorted(library.parent.parent.rglob('*.dll'))
    binary_before={str(path):sha(path) for path in [library,world,*binaries]}
    cache=args.cache.resolve()
    cache_files=[cache/name for name in ('v2quests_pyarray.bin','v2quests_pyarraynames.bin','v2quests_pycst.bin')]
    cache_before={path.name:sha(path) for path in cache_files}
    exe=out/'character_kill_quest_owner_integration.exe'
    command=[compiler,'-std=c++17','-Wall','-Wextra','-Werror','-O2',
      '-I'+str(ROOT/'port/game-data'),'-I'+str(ROOT/'port/pydata-constants'),'-I'+str(ROOT/'port/level-world'),test,source,cursor,
      ROOT/'port/quest-kill/quest.c',library,world,'-o',exe]
    run(command)
    live=env.copy();live['PATH']=os.pathsep.join([*(str(path.parent) for path in binaries),env['PATH']])
    output=run([exe,cache],live)
    if not output.startswith('PASS: one Character::Kill tail run updated actual KillX and in-memory projected ClearEnemies'):
        raise RuntimeError('host integration emitted unexpected result: '+output)
    if before!={path.relative_to(ROOT).as_posix():sha(path) for path in sorted(evidence)}:
        raise RuntimeError('tracked inputs changed during test')
    if binary_before!={str(path):sha(path) for path in [library,world,*binaries]}:
        raise RuntimeError('selected libraries changed during test')
    if cache_before!={path.name:sha(path) for path in cache_files}:
        raise RuntimeError('source cache changed during test')
    imports=re.findall(r'DLL Name: (\S+)',run([compiler.with_name('objdump.exe'),'-p',exe]))
    if 'libdh2_game_data.dll' not in imports or 'libdh2_level_world.dll' not in imports:
        raise RuntimeError('test did not load the selected game-data and level-world DLLs')
    report={'validation':'PASS','output':output.strip(),'host_test_only':True,
      'live_gameplay':False,'android_compilation':False,'source_inputs_unchanged':True,
      'selected_libraries_unchanged':True,'cache_unchanged':True,
      'source_sha256':before,'cache_sha256':cache_before,
      'binary_sha256':{**binary_before,str(exe):sha(exe)},
      'compiler_command':list(map(str,command)),'binary_imports':imports}
    (out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({'validation':'PASS','output':output.strip(),'report':str(out/'report.json')}))

if __name__=='__main__':main()
