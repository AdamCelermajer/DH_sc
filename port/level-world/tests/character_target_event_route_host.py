"""Replay source target-event generic routing under ASan/UBSan.

Handler/FSM deliveries are synchronous borrowed services. This proves source control,
reload/store order and reentry, not full world, predicate or Lua ownership.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def wsl(p):return '/mnt/'+p.drive[0].lower()+str(p)[2:].replace('\\','/')
def main():
 p=argparse.ArgumentParser();p.add_argument('--main-linked',nargs='?',const='/home/adampalace/dh2-world-build/character_target_event_route_audit');a=p.parse_args()
 paths=[ROOT/'character_ai_events.cpp',ROOT/'character_ai_events.hpp',ROOT/'tests/character_target_event_route.cpp',Path(__file__),ROOT/'tests/character_target_event_route_differential.py',ROOT/'reference/character-target-event-route/target-route-fixtures.bin',ROOT/'reports/character-target-event-route-arm64-differential.json',ROOT/'character_target_bindings.hpp',ROOT/'../script-runtime/script_runtime.h',ROOT/'reference/character-target-event-route/original-functions.json',ROOT/'reference/character-target-event-route/reference/original-functions.asm',ROOT/'tests/character_target_endpoints_original.py',ROOT/'reference/character-target-event-route/endpoints-original-probe.json']
 before={str(x.relative_to(REPO)):sha(x) for x in paths};out=REPO/'.local-inputs/character-target-event-route/character_target_event_route_host';command=[];binaries={}
 if a.main_linked:
  exe=a.main_linked;r=subprocess.run(['wsl.exe','--','ldd',exe],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'not found' not in r.stdout;deps={exe}
  for line in r.stdout.splitlines():
   m=re.search(r'=>\s+(/\S+)|^\s*(/\S+)',line)
   if m:deps.add(m.group(1) or m.group(2))
  def hashes():
   r=subprocess.run(['wsl.exe','--','sha256sum',*sorted(deps)],capture_output=True,text=True,timeout=30);assert r.returncode==0;return {line.split(maxsplit=1)[1].strip():line.split()[0] for line in r.stdout.splitlines()}
  binaries=hashes();assert any('libdh2_level_world.so' in x for x in binaries)
 else:
  exe=wsl(out);command=['wsl.exe','--','g++','-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror',wsl(paths[0]),wsl(paths[2]),'-o',exe];r=subprocess.run(command,capture_output=True,text=True,timeout=60);assert r.returncode==0,(r.stdout,r.stderr)
 r=subprocess.run(['wsl.exe','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,wsl(paths[5])],capture_output=True,text=True,timeout=60);assert r.returncode==0 and not r.stderr,(r.returncode,r.stdout,r.stderr);result=json.loads(r.stdout);assert before=={str(x.relative_to(REPO)):sha(x) for x in paths}
 if a.main_linked:assert binaries==hashes()
 else:binaries={str(out):sha(out)}
 result.update(source_bindings=before,binary_bindings=binaries,compiler_command=command,asan_ubsan=True,sanitizer_findings=0,scope=__doc__,compiler_provenance='standalone compiler command recorded' if command else 'prebuilt dependency hashes only; central compile provenance belongs to caller')
 (ROOT/'reports'/('character-target-event-route-main-linked-host-audit.json' if a.main_linked else 'character-target-event-route-host-audit.json')).write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
