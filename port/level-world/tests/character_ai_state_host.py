"""Isolated source-bound ASan/UBSan replay, no CMake or packaged-library claim."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def wsl(path):return '/mnt/'+path.drive[0].lower()+str(path)[2:].replace('\\','/')
def main():
 p=argparse.ArgumentParser();p.add_argument('--main-linked',nargs='?',const='/home/adampalace/dh2-world-build/character_ai_state_audit');a=p.parse_args()
 source=[ROOT/'character_ai_state.cpp',ROOT/'character_ai_state.hpp',Path(__file__),ROOT/'tests/character_ai_state.cpp',ROOT/'reference/prince-live-ai/query-fixtures.bin',ROOT/'reports/character-ai-state-arm64-differential.json']
 before={str(p.relative_to(REPO)):sha(p) for p in source}
 if a.main_linked:
  dependencies=subprocess.run(['wsl.exe','--','ldd',a.main_linked],capture_output=True,text=True,timeout=30);assert dependencies.returncode==0 and 'not found' not in dependencies.stdout
  paths={a.main_linked}
  for line in dependencies.stdout.splitlines():
   match=re.search(r'=>\s+(/\S+)|^\s*(/\S+)',line)
   if match:paths.add(match.group(1) or match.group(2))
  def bound_hashes():
   hashed=subprocess.run(['wsl.exe','--','sha256sum',*sorted(paths)],capture_output=True,text=True,timeout=30);assert hashed.returncode==0,hashed.stderr
   return {line.split(maxsplit=1)[1].strip():line.split()[0] for line in hashed.stdout.splitlines()}
  binaries_before=bound_hashes()
  assert any('libdh2_level_world.so' in path for path in binaries_before),binaries_before
  run=subprocess.run(['wsl.exe','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',a.main_linked,wsl(source[4])],capture_output=True,text=True,timeout=60)
  assert run.returncode==0 and not run.stderr,(run.returncode,run.stdout,run.stderr)
  result=json.loads(run.stdout);after={str(p.relative_to(REPO)):sha(p) for p in source};assert before==after and binaries_before==bound_hashes()
  result.update(current_source_bindings=before,binary_bindings=binaries_before,executable=a.main_linked,ldd_output=dependencies.stdout,asan_ubsan=True,sanitizer_findings=0,scope='Replay actual central world-linked audit executable. Current source, corpus, executable and resolved DSO bytes verified unchanged before/after. This runner does not rebuild or assert compiler provenance, APK/Android parity or full AI.')
  report=ROOT/'reports/character-ai-state-main-linked-host-audit.json';report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result));return
 out=REPO/'.local-inputs/prince-live-ai-discovery/character_ai_state_host';out.parent.mkdir(parents=True,exist_ok=True)
 command=['wsl.exe','--','g++','-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror',wsl(ROOT/'character_ai_state.cpp'),wsl(ROOT/'tests/character_ai_state.cpp'),'-o',wsl(out)]
 build=subprocess.run(command,capture_output=True,text=True,timeout=60);assert build.returncode==0,(build.stdout,build.stderr)
 run=subprocess.run(['wsl.exe','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',wsl(out),wsl(source[4])],capture_output=True,text=True,timeout=60)
 assert run.returncode==0 and not run.stderr,(run.returncode,run.stdout,run.stderr)
 result=json.loads(run.stdout);after={str(p.relative_to(REPO)):sha(p) for p in source};assert before==after
 result.update(source_bindings=before,compiler_command=command,executable_sha256=sha(out),asan_ubsan=True,sanitizer_findings=0,scope=__doc__)
 report=ROOT/'reports/character-ai-state-host-audit.json';report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
