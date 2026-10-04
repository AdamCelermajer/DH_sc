"""Isolated native owned-state replay; no central DSO or Android/device claim."""
import argparse,hashlib,json,os,shlex,subprocess
from pathlib import Path
R=Path(__file__).resolve().parents[1];REPO=R.parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def posix(p):return '/mnt/'+p.drive[0].lower()+str(p.resolve())[2:].replace('\\','/')
def run(args):
 p=subprocess.run(args,capture_output=True,text=True,encoding='utf-8',errors='replace');assert p.returncode==0,(p.returncode,p.stdout,p.stderr);return p
p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.parent.mkdir(parents=True,exist_ok=True)
paths=['port/level-world/character_state_owner.cpp','port/level-world/character_state_owner.hpp','port/level-world/character_state_owner_data.inc','port/level-world/tests/character_state_owner.cpp','port/level-world/tests/character_state_owner_host.py','port/level-world/character_native_fsm.hpp','port/level-world/character_state.hpp','port/script-runtime/script_runtime.h']
sources={x:sha(REPO/x) for x in paths};scratch=REPO/'.local-inputs/character-monster-state-ownership';exe=scratch/'state_owner_audit_final';flags=['-std=c++17','-O2','-g','-fno-omit-frame-pointer','-fsanitize=address,undefined','-fno-fast-math','-ffp-contract=off','-Wall','-Wextra','-Werror']
command=['g++',*flags,posix(R/'character_state_owner.cpp'),posix(R/'tests/character_state_owner.cpp'),'-o',posix(exe)]
run(['wsl','-e','bash','-lc',shlex.join(command)]);
gold=R/'reference/character-monster-state-ownership/state-owner-fixtures.bin';pred=R/'reference/character-monster-state-ownership/predicate-fixtures.bin'
result=run(['wsl','-e','env','ASAN_OPTIONS=detect_leaks=1','UBSAN_OPTIONS=halt_on_error=1',posix(exe),posix(gold),posix(pred)]);host=json.loads(result.stdout.strip());assert not result.stderr and all(sha(REPO/x)==h for x,h in sources.items())
proofs={}
for name in ('character-state-owner-arm64-differential.json','character-state-owner-predicates-arm64-differential.json'):
 f=R/'reports'/name;proofs[name]=dict(sha256=sha(f),report=json.loads(f.read_text()))
r=dict(validation='PASS',host_audit=host,sanitizers=dict(address=True,undefined=True,diagnostics=0,leak_detection=True),source_sha256=sources,executable_sha256=sha(exe),reference_sha256={str(x.relative_to(REPO)).replace('\\','/'):sha(x) for x in (gold,pred)},original_sha256='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80',original_arm64_proofs=proofs,compiler_command=command,scope=__doc__,full_AI=False,behavior_virtuals='Required explicit callback backends; fixture deliveries are not reconstructed20-state behaviors',shared_central_DSO_used=False)
a.output.write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(r))
