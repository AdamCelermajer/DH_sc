"""Isolated source-bound PreSpawn gold replay with ASan/UBSan; no scene or APK claim."""
import argparse,hashlib,json,shlex,subprocess
from pathlib import Path
R=Path(__file__).resolve().parents[1];REPO=R.parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def posix(p):return '/mnt/'+p.drive[0].lower()+str(p.resolve())[2:].replace('\\','/')
def run(args):
 r=subprocess.run(args,capture_output=True,text=True)
 assert not r.returncode,(r.returncode,r.stdout,r.stderr)
 return r
def main():
 p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 paths=('character_pre_spawn.hpp','character_pre_spawn.cpp','character_state.hpp','character_state_empty.hpp','character_state_empty.cpp','reference/character-state-methods/native-methods.inc','tests/character_pre_spawn.cpp','tests/character_pre_spawn_host.py')
 sources={'port/level-world/'+x:sha(R/x) for x in paths}
 exe=REPO/'.local-inputs/character-pre-spawn/pre_spawn_audit'
 cmd=['g++','-std=c++17','-O2','-g','-fno-fast-math','-ffp-contract=off','-fno-omit-frame-pointer','-fsanitize=address,undefined','-Wall','-Wextra','-Werror',posix(R/'character_pre_spawn.cpp'),posix(R/'character_state_empty.cpp'),posix(R/'tests/character_pre_spawn.cpp'),'-o',posix(exe)]
 run(['wsl','-e','bash','-lc',shlex.join(cmd)])
 ref=R/'reference/character-pre-spawn/pre-spawn-fixtures.bin'
 result=run(['wsl','-e','env','ASAN_OPTIONS=detect_leaks=1','UBSAN_OPTIONS=halt_on_error=1',posix(exe),posix(ref)])
 assert not result.stderr and sources=={'port/level-world/'+x:sha(R/x) for x in paths}
 proof=R/'reports/character-pre-spawn-arm64-differential.json'
 data=dict(validation='PASS',scope=__doc__,host_audit=json.loads(result.stdout),reference_sha256=sha(ref),source_sha256=sources,executable_sha256=sha(exe),original_arm64_proof=dict(path=proof.relative_to(REPO).as_posix(),sha256=sha(proof)),sanitizers=dict(address=True,undefined=True,leak_detection=True,diagnostics=0),compiler_command=cmd,central_DSO_used=False,full_physics_spawn_revive_backends=False,packaged_APK=False)
 a.output.write_text(json.dumps(data,indent=2)+'\n');print(json.dumps(data))
if __name__=='__main__':main()
