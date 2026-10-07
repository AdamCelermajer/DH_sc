"""Isolated original-gold state relay and optional retained private-VM composition."""
import argparse,hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/'+p.drive[0].lower()+p.as_posix()[2:]
def main():
 p=argparse.ArgumentParser();p.add_argument('--with-vm',action='store_true');a=p.parse_args()
 scratch=REPO/'.local-inputs/character-ai-state-changed';scratch.mkdir(exist_ok=True)
 sources=[ROOT/x for x in ['character_ai_state_changed.cpp','character_ai_state_changed.hpp','character_ai_state_changed_vm.cpp','character_ai_state_changed_vm.hpp','tests/character_ai_state_changed.cpp','tests/character_ai_state_changed_vm.cpp','tests/character_ai_state_changed_host.py','tests/character_script_owner.cpp','character_script_owner.hpp','character_ai_events.hpp','object_identity.hpp']]
 evidence=[ROOT/'reference/character-ai-state-changed'/x for x in ['original-functions.json','reference/original-functions.asm','vtables.json','state-changed-fixtures.bin']]+[ROOT/'reports/character-ai-state-changed-arm64-differential.json']
 common=REPO/'.local-inputs/character-script-owner-discovery/ai-commons-source.luac'
 paths=sources+evidence+([common] if a.with_vm else []);before={p.relative_to(REPO).as_posix():sha(p) for p in paths};commands=[]
 def run(args):
  r=subprocess.run(['wsl.exe','--',*args],text=True,capture_output=True,timeout=60);commands.append(dict(arguments=args,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0 and not r.stderr,(r.stdout,r.stderr);return r.stdout.strip()
 flags=['-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror']
 exe=scratch/'state_changed_host';run(['g++',*flags,linux(sources[0]),linux(sources[4]),'-o',linux(exe)])
 audit=json.loads(run(['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',linux(exe),linux(evidence[3])]))
 assert audit['cases']==1578 and audit['validation']=='PASS';vm=None;binaries={linux(exe):sha(exe)};dependencies=''
 if a.with_vm:
  world='/home/adampalace/dh2-world-build/libdh2_level_world.so';runtime='/home/adampalace/dh2-world-build/script-runtime/libdh2_script_runtime.so';libs=[world,runtime]
  hashes={x:run(['sha256sum',x]).split()[0] for x in libs};vexe=scratch/'state_changed_vm_host'
  run(['g++',*flags,linux(sources[0]),linux(sources[2]),linux(sources[5]),'-I'+linux(ROOT),'-L/home/adampalace/dh2-world-build','-ldh2_level_world','-L/home/adampalace/dh2-world-build/script-runtime','-ldh2_script_runtime','-Wl,-rpath,/home/adampalace/dh2-world-build','-Wl,-rpath,/home/adampalace/dh2-world-build/script-runtime','-ldl','-o',linux(vexe)])
  vm=json.loads(run(['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',linux(vexe),linux(common)]));assert vm['validation']=='PASS'
  assert hashes=={x:run(['sha256sum',x]).split()[0] for x in libs};dependencies=run(['ldd',linux(vexe)]);assert world in dependencies and runtime in dependencies
  binaries.update(hashes);binaries[linux(vexe)]=sha(vexe)
 assert before=={p.relative_to(REPO).as_posix():sha(p) for p in paths}
 report=dict(validation='PASS',host_audit=audit,private_VM_audit=vm,source_and_input_sha256=before,binary_sha256=binaries,commands=commands,linked_dependencies=dependencies,sanitizer_findings=0,sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer','LeakSanitizer'],central_DSO_rebuilt=False,source_helpers_in_main_world=False,whole_Application_or_full_FSM=False,scope=__doc__)
 name='character-ai-state-changed-host-audit.json' if a.with_vm else 'character-ai-state-changed-pure-host-audit.json';(ROOT/'reports'/name).write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',host=audit,vm=vm)))
if __name__=='__main__':main()
