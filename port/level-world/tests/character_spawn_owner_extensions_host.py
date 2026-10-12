"""Isolated SAN owned Spawn/PreSpawn/selection/TimerStore composition, using frozen original gold."""
import argparse,hashlib,json,shlex,subprocess
from pathlib import Path
R=Path(__file__).resolve().parents[1];REPO=R.parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def posix(p):return '/mnt/'+p.drive[0].lower()+str(p.resolve())[2:].replace('\\','/')
def run(args):
 r=subprocess.run(args,capture_output=True,text=True);assert not r.returncode,(r.returncode,r.stdout,r.stderr);return r
def main():
 p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 modules=['character_spawn_owner_extensions','character_state_owner_extensions','character_state_owner_behavior','character_state_owner_frame','character_state_owner','character_native_fsm','character_state','character_pre_spawn','character_spawn_body','character_spawn_select','character_spawn_permission','character_state_empty','character_timers','character_target_bindings','character_design_services']
 names=['port/level-world/'+m+ext for m in modules for ext in ('.hpp','.cpp')]+['port/level-world/character_state_owner_data.inc','port/level-world/reference/character-state-methods/native-methods.inc','port/level-world/tests/character_spawn_body.cpp','port/level-world/tests/character_spawn_owner_extensions.cpp','port/level-world/tests/character_spawn_owner_extensions_host.py','port/game-data/combat.cpp','port/game-data/combat.hpp','port/script-runtime/script_runtime.h']
 sources={x:sha(REPO/x) for x in names}
 exe=REPO/'.local-inputs/character-spawn-owner-extensions/spawn_owner_audit';exe.parent.mkdir(parents=True,exist_ok=True)
 files=['port/level-world/'+m+'.cpp' for m in modules]+['port/game-data/combat.cpp','port/level-world/tests/character_spawn_owner_extensions.cpp']
 cmd=['g++','-std=c++17','-O2','-g','-fno-fast-math','-ffp-contract=off','-fno-omit-frame-pointer','-ffunction-sections','-fdata-sections','-Wl,--gc-sections','-fsanitize=address,undefined','-Wall','-Wextra','-Werror',*[posix(REPO/x) for x in files],'-o',posix(exe)]
 run(['wsl','-e','bash','-lc',shlex.join(cmd)])
 golds=['port/level-world/reference/character-spawn-body/spawn-body-fixtures.bin','port/level-world/reference/character-spawn-state/spawn-select-fixtures.bin','port/level-world/reference/character-spawn-state/spawn-permission-fixtures.bin']
 result=run(['wsl','-e','env','ASAN_OPTIONS=detect_leaks=1','UBSAN_OPTIONS=halt_on_error=1',posix(exe),*[posix(REPO/x) for x in golds]])
 assert not result.stderr and sources=={x:sha(REPO/x) for x in names}
 proofs=['port/level-world/reports/character-'+s+'-arm64-differential.json' for s in ('spawn-body','spawn-select','spawn-permission','state-owner','pre-spawn')]
 proofs=[x for x in proofs if (REPO/x).exists()]
 data=dict(validation='PASS',scope=__doc__,host_audit=json.loads(result.stdout),source_sha256=sources,executable_sha256=sha(exe),gold_sha256={x:sha(REPO/x) for x in golds},original_sha256=sha(REPO/'.local-inputs/libDungeonHunter2.so'),original_kernel_proofs={x:sha(REPO/x) for x in proofs},sanitizers=dict(address=True,undefined=True,leaks=True,diagnostics=0),compiler_command=cmd,central_DSO_used=False,adapter_is_original_function=False,full_scene_backends=False,APK=False)
 a.output.write_text(json.dumps(data,indent=2)+'\n');print(json.dumps(data))
if __name__=='__main__':main()
