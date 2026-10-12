"""Sanitized isolated replay of source-bound Character startup prefixes."""
import hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/'+p.drive[0].lower()+p.as_posix()[2:]
def main():
 scratch=REPO/'.local-inputs/character-update-startup';scratch.mkdir(exist_ok=True)
 names=['character_update_startup.hpp','character_update_startup.cpp','tests/character_update_startup.cpp','tests/character_update_startup_host.py','tests/character_update_startup_differential.py','tools/build_character_update_startup_oracle.ps1','character_can_update.hpp','character_can_update.cpp','reference/character-update-startup/original-functions.json','reference/character-update-startup/reference/original-functions.asm','reference/character-update-startup/startup-prefix-fixtures.bin','reports/character-update-startup-arm64-differential.json']
 paths=[ROOT/x for x in names];before={p.relative_to(REPO).as_posix():sha(p) for p in paths};commands=[]
 def run(args):
  result=subprocess.run(['wsl.exe','--',*args],text=True,capture_output=True,timeout=60);commands.append(dict(arguments=args,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0 and not result.stderr,(result.stdout,result.stderr);return result.stdout.strip()
 exe=scratch/'startup_host';flags=['-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror']
 run(['g++',*flags,linux(paths[1]),linux(paths[7]),linux(paths[2]),'-o',linux(exe)])
 audit=json.loads(run(['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',linux(exe),linux(paths[10])]))
 assert audit['comparisons']==1404 and audit['validation']=='PASS'
 assert before=={p.relative_to(REPO).as_posix():sha(p) for p in paths}
 report=dict(validation='PASS',host_audit=audit,source_and_input_sha256=before,executable_sha256=sha(exe),commands=commands,sanitizer_findings=0,sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer','LeakSanitizer'],central_DSO_rebuilt=False,scope=__doc__,whole_character_frame=False,owned_queue=False)
 (ROOT/'reports/character-update-startup-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(audit))
if __name__=='__main__':main()
