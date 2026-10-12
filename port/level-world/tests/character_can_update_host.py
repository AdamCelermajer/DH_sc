"""Isolated sanitized replay of complete CanUpdate original gold; no central rebuild."""
import hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/'+p.drive[0].lower()+p.as_posix()[2:]
def main():
 scratch=REPO/'.local-inputs/character-can-update';scratch.mkdir(exist_ok=True)
 paths=[ROOT/x for x in ['character_can_update.hpp','character_can_update.cpp','tests/character_can_update.cpp','tests/character_can_update_host.py','tests/character_can_update_differential.py','tools/build_character_can_update_oracle.ps1','reference/character-can-update/original-functions.json','reference/character-can-update/reference/original-functions.asm','reference/character-can-update/can-update-fixtures.bin','reports/character-can-update-arm64-differential.json']]
 before={p.relative_to(REPO).as_posix():sha(p) for p in paths};commands=[]
 def run(args):
  r=subprocess.run(['wsl.exe','--',*args],text=True,capture_output=True,timeout=60);commands.append(dict(arguments=args,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0 and not r.stderr,(r.stdout,r.stderr);return r.stdout.strip()
 exe=scratch/'can_update_host';flags=['-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror']
 run(['g++',*flags,linux(paths[1]),linux(paths[2]),'-o',linux(exe)])
 audit=json.loads(run(['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',linux(exe),linux(paths[8])]))
 assert audit['comparisons']==4752 and audit['ordered_services']==13833
 assert before=={p.relative_to(REPO).as_posix():sha(p) for p in paths}
 report=dict(validation='PASS',host_audit=audit,source_and_input_sha256=before,executable_sha256=sha(exe),commands=commands,sanitizer_findings=0,sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer','LeakSanitizer'],central_DSO_rebuilt=False,scope=__doc__)
 (ROOT/'reports/character-can-update-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(audit))
if __name__=='__main__':main()
