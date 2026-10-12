"""Isolated sanitized Spawn selection gold + real TimerStore/owned StateInfo routing; other state methods explicit fixtures."""
import argparse,hashlib,json,shlex,subprocess
from pathlib import Path
R=Path(__file__).resolve().parents[1];REPO=R.parents[1];sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def posix(p):return '/mnt/'+p.drive[0].lower()+str(p.resolve())[2:].replace('\\','/')
def run(args):
 r=subprocess.run(args,capture_output=True,text=True);assert not r.returncode,(r.returncode,r.stdout,r.stderr);return r
def main():
 p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 names=['port/level-world/'+x for x in ('character_spawn_select.hpp','character_spawn_select.cpp','character_spawn_permission.hpp','character_spawn_permission.cpp','character_native_fsm.hpp','character_state.hpp','character_state_owner.hpp','character_state_owner.cpp','character_state_owner_data.inc','character_timers.hpp','character_timers.cpp','tests/character_spawn_select.cpp','tests/character_spawn_select_host.py')]+['port/game-data/combat.hpp','port/game-data/combat.cpp'];sources={x:sha(REPO/x) for x in names}
 exe=REPO/'.local-inputs/character-spawn-state/spawn_select_audit'
 files=['port/level-world/character_spawn_select.cpp','port/level-world/character_spawn_permission.cpp','port/game-data/combat.cpp','port/level-world/character_state_owner.cpp','port/level-world/character_timers.cpp','port/level-world/tests/character_spawn_select.cpp']
 cmd=['g++','-std=c++17','-O2','-g','-fno-fast-math','-ffp-contract=off','-fno-omit-frame-pointer','-fsanitize=address,undefined','-Wall','-Wextra','-Werror',*[posix(REPO/x) for x in files],'-o',posix(exe)]
 run(['wsl','-e','bash','-lc',shlex.join(cmd)]);gold=R/'reference/character-spawn-state/spawn-select-fixtures.bin';permission=R/'reference/character-spawn-state/spawn-permission-fixtures.bin';result=run(['wsl','-e','env','ASAN_OPTIONS=detect_leaks=1','UBSAN_OPTIONS=halt_on_error=1',posix(exe),posix(gold),posix(permission)])
 assert not result.stderr and sources=={x:sha(REPO/x) for x in names}
 proof=R/'reports/character-spawn-select-arm64-differential.json';permission_proof=R/'reports/character-spawn-permission-arm64-differential.json';data=dict(validation='PASS',scope=__doc__,host_audit=json.loads(result.stdout),source_sha256=sources,executable_sha256=sha(exe),reference_sha256=sha(gold),permission_reference_sha256=sha(permission),original_ARM64_proof=dict(path=proof.relative_to(REPO).as_posix(),sha256=sha(proof)),permission_original_ARM64_proof=dict(path=permission_proof.relative_to(REPO).as_posix(),sha256=sha(permission_proof)),sanitizers=dict(address=True,undefined=True,leaks=True,diagnostics=0),compiler_command=cmd,central_DSO_used=False,full_Spawn_virtual_methods=False,APK=False)
 a.output.write_text(json.dumps(data,indent=2)+'\n');print(json.dumps(data))
if __name__=='__main__':main()
