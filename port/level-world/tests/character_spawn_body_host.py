"""Isolated SAN Spawn1 gold replay plus genuine target/debug kernels with explicit missing-file/gameplay fixtures."""
import argparse,hashlib,json,shlex,subprocess
from pathlib import Path
R=Path(__file__).resolve().parents[1];REPO=R.parents[1];sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def posix(p):return '/mnt/'+p.drive[0].lower()+str(p.resolve())[2:].replace('\\','/')
def run(args):
 r=subprocess.run(args,capture_output=True,text=True);assert not r.returncode,(r.returncode,r.stdout,r.stderr);return r
def main():
 p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 names=['port/level-world/'+x for x in ('character_spawn_body.hpp','character_spawn_body.cpp','character_state.hpp','character_state_empty.hpp','character_state_empty.cpp','reference/character-state-methods/native-methods.inc','character_target_bindings.hpp','character_target_bindings.cpp','character_design_services.hpp','character_design_services.cpp','tests/character_spawn_body.cpp','tests/character_spawn_body_host.py')]+['port/script-runtime/script_runtime.h'];sources={x:sha(REPO/x) for x in names}
 exe=REPO/'.local-inputs/character-spawn-body/spawn_body_audit';files=('character_spawn_body.cpp','character_state_empty.cpp','character_target_bindings.cpp','character_design_services.cpp','tests/character_spawn_body.cpp')
 cmd=['g++','-std=c++17','-O2','-g','-fno-fast-math','-ffp-contract=off','-fno-omit-frame-pointer','-ffunction-sections','-fdata-sections','-Wl,--gc-sections','-fsanitize=address,undefined','-Wall','-Wextra','-Werror',*[posix(R/x) for x in files],'-o',posix(exe)]
 run(['wsl','-e','bash','-lc',shlex.join(cmd)]);gold=R/'reference/character-spawn-body/spawn-body-fixtures.bin';result=run(['wsl','-e','env','ASAN_OPTIONS=detect_leaks=1','UBSAN_OPTIONS=halt_on_error=1',posix(exe),posix(gold)]);assert not result.stderr and sources=={x:sha(REPO/x) for x in names}
 proof=R/'reports/character-spawn-body-arm64-differential.json';data=dict(validation='PASS',scope=__doc__,host_audit=json.loads(result.stdout),source_sha256=sources,executable_sha256=sha(exe),reference_sha256=sha(gold),original_ARM64_proof=dict(path=proof.relative_to(REPO).as_posix(),sha256=sha(proof)),sanitizers=dict(address=True,undefined=True,leaks=True,diagnostics=0),compiler_command=cmd,central_DSO_used=False,full_animation_CancelSneaking_physics_backends=False,APK=False)
 a.output.write_text(json.dumps(data,indent=2)+'\n');print(json.dumps(data))
if __name__=='__main__':main()
