"""Run the selected Crypt row 23 KillXEnemies Compile/register/event path."""
from __future__ import annotations
import argparse
import os
from pathlib import Path
import shutil
import subprocess

ROOT=Path(__file__).resolve().parents[3]

def main():
    p=argparse.ArgumentParser(description=__doc__)
    for name in ("compiler","cache","output","library","world-library"):
        p.add_argument("--"+name,required=True,type=Path)
    a=p.parse_args();a.output.resolve().mkdir(parents=True,exist_ok=True)
    exe=a.output.resolve()/"native_quest_kill_objective_list_v1.exe"
    source=ROOT/"port/android-native/app/src/main/cpp/native_quest_owner.cpp"
    test=ROOT/"port/android-native/tests/native_quest_kill_objective_list_v1.cpp"
    command=[a.compiler,"-std=c++17","-Wall","-Wextra","-Werror","-O2",
      "-I"+str(ROOT/"port/game-data"),"-I"+str(ROOT/"port/pydata-constants"),
      "-I"+str(ROOT/"port/level-world"),"-I"+str(ROOT/"port/quest-kill"),
      str(test),str(source),
      str(ROOT/"port/android-native/app/src/main/cpp/native_quest_cursor.cpp"),
      str(ROOT/"port/android-native/app/src/main/cpp/native_player_profile.cpp"),
      str(ROOT/"port/quest-kill/quest.c"),str(a.library),str(a.world_library),"-o",str(exe)]
    result=subprocess.run(list(map(str,command)),cwd=ROOT,capture_output=True,text=True)
    if result.returncode:raise SystemExit(result.stdout+result.stderr)
    dll_dirs=sorted({str(path.parent) for path in a.library.parent.parent.rglob("*.dll")})
    env=os.environ.copy();env["PATH"]=os.pathsep.join([*dll_dirs,str(a.compiler.parent),env.get("PATH","")])
    result=subprocess.run([str(exe),str(a.cache.resolve())],cwd=ROOT,capture_output=True,text=True,env=env)
    if result.returncode:raise SystemExit(result.stdout+result.stderr)
    print(result.stdout.strip())

if __name__=="__main__":main()
