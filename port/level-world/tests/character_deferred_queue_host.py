"""Isolated sanitized replay of the source-bound owned concurrent-AI queue."""
import hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/'+p.drive[0].lower()+p.as_posix()[2:]
def main():
 scratch=REPO/'.local-inputs/character-deferred-queue';scratch.mkdir(exist_ok=True)
 names=['character_deferred_queue.hpp','character_deferred_queue.cpp','tests/character_deferred_queue.cpp','tests/character_deferred_queue_host.py','tests/character_deferred_queue_differential.py','tools/build_character_deferred_queue_oracle.ps1','reference/character-deferred-queue/original-functions.json','reference/character-deferred-queue/reference/original-functions.asm','reference/character-deferred-queue/deferred-queue-fixtures.bin','reports/character-deferred-queue-arm64-differential.json']
 paths=[ROOT/x for x in names];before={p.relative_to(REPO).as_posix():sha(p) for p in paths};commands=[]
 def run(args):
  result=subprocess.run(['wsl.exe','--',*args],text=True,capture_output=True,timeout=60);commands.append(dict(arguments=args,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr))
  assert result.returncode==0 and not result.stderr,(result.stdout,result.stderr);return result.stdout.strip()
 exe=scratch/'deferred_queue_host';flags=['-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror']
 run(['g++',*flags,linux(paths[1]),linux(paths[2]),'-o',linux(exe)])
 audit=json.loads(run(['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',linux(exe),linux(paths[8])]))
 source=json.loads(paths[9].read_text());assert audit['validation']=='PASS'
 for field in ('comparisons','ordered_callbacks','ordered_entries'):assert audit[field]==source[field],field
 assert before=={p.relative_to(REPO).as_posix():sha(p) for p in paths}
 report=dict(validation='PASS',host_audit=audit,source_and_input_sha256=before,executable_sha256=sha(exe),commands=commands,sanitizer_findings=0,sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer','LeakSanitizer'],central_DSO_rebuilt=False,scope=__doc__,whole_character_frame=False,whole_Application_constructor=False,AI_unload_and_kill_body_providers='explicit caller fixtures',borrowed_actor_lifetimes='caller pinned; container never owns actors')
 (ROOT/'reports/character-deferred-queue-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(audit))
if __name__=='__main__':main()
