"""Isolated actual source owner/frame/body composition; ASan/UBSan/leak replay.
No central DSO, APK, emulator or full game-service parity claim.
"""
import argparse,hashlib,json,shlex,subprocess
from pathlib import Path
R=Path(__file__).resolve().parents[1];REPO=R.parents[1];sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def posix(p):return '/mnt/'+p.drive[0].lower()+str(p.resolve())[2:].replace('\\','/')
def run(args):
 r=subprocess.run(args,capture_output=True,text=True,encoding='utf8',errors='replace');assert not r.returncode,(r.returncode,r.stdout,r.stderr);return r
p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args()
paths=['port/level-world/'+x for x in ('character_state.cpp','character_state.hpp','character_native_fsm.cpp','character_native_fsm.hpp','character_state_owner.cpp','character_state_owner.hpp','character_state_owner_data.inc','character_state_owner_behavior.cpp','character_state_owner_behavior.hpp','character_state_owner_frame.cpp','character_state_owner_frame.hpp','tests/character_state_owner_frame.cpp','tests/character_state_owner_frame_host.py')]+['port/script-runtime/script_runtime.h'];sources={x:sha(REPO/x) for x in paths}
exe=REPO/'.local-inputs/character-state-owner-frame/frame_audit_final';flags=['-std=c++17','-O2','-g','-fno-omit-frame-pointer','-fsanitize=address,undefined','-fno-fast-math','-ffp-contract=off','-Wall','-Wextra','-Werror'];units=('character_state.cpp','character_native_fsm.cpp','character_state_owner.cpp','character_state_owner_behavior.cpp','character_state_owner_frame.cpp','tests/character_state_owner_frame.cpp');cmd=['g++',*flags,*[posix(R/x) for x in units],'-o',posix(exe)];run(['wsl','-e','bash','-lc',shlex.join(cmd)])
references=[R/'reference/character-state/state-reference.bin',R/'reference/character-native-fsm/native-update-fixtures.bin'];assert sha(references[0])=='9e11a899a29f9682959c00e988ad4fc04ee4a3f610de9044e933d47997d27a57'
out=run(['wsl','-e','env','ASAN_OPTIONS=detect_leaks=1','UBSAN_OPTIONS=halt_on_error=1',posix(exe),*[posix(x) for x in references]]);assert not out.stderr and sources=={x:sha(REPO/x) for x in paths}
proofs=[R/'reports/character-state-owner-frame-arm64-differential.json',R/'reports/character-state-owner-frame-outer-arm64-differential.json'];assert all(json.loads(x.read_text())['validation']=='PASS' for x in proofs)
data=dict(validation='PASS',scope=__doc__,host_audit=json.loads(out.stdout.strip()),original_sha256='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80',reference_sha256={str(x.relative_to(REPO)).replace('\\','/'):sha(x) for x in references},source_sha256=sources,executable_sha256=sha(exe),sanitizers=dict(address=True,undefined=True,leak_detection=True,diagnostics=0),original_arm64_proofs=[dict(path=str(x.relative_to(REPO)).replace('\\','/'),sha256=sha(x)) for x in proofs],compiler_command=cmd,full_AI=False,full_effect_backends=False,central_DSO_used=False,other_behavior_families='All sixteen remain mandatory caller providers with original Update address; observed deliveries are explicit fixtures.',effect_corpus_mutations='Source pending-effect oracle fixtures update current projection coherently; separate nested checks use genuine owner transitions/events.')
a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(data,indent=2)+'\n');print(json.dumps(data))
