"""Source-bound standalone ASan/UBSan replay of actual-instruction attack corpus."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def wsl(path):return '/mnt/'+path.drive[0].lower()+str(path)[2:].replace('\\','/')
def main():
 p=argparse.ArgumentParser();p.add_argument('--main-linked',nargs='?',const='/home/adampalace/dh2-world-build/character_ai_attack_audit');a=p.parse_args()
 source=[ROOT/'character_ai_attack.cpp',ROOT/'character_ai_attack.hpp',Path(__file__),ROOT/'tests/character_ai_attack.cpp',ROOT/'tests/character_ai_attack_differential.py',ROOT/'reference/prince-live-attack/attack-fixtures.bin',ROOT/'reports/character-ai-attack-arm64-differential.json']
 before={str(p.relative_to(REPO)):sha(p) for p in source};out=REPO/'.local-inputs/prince-live-attack-discovery/character_ai_attack_host'
 if a.main_linked:
  dependencies=subprocess.run(['wsl.exe','--','ldd',a.main_linked],capture_output=True,text=True,timeout=30);assert dependencies.returncode==0 and 'not found' not in dependencies.stdout
  paths={a.main_linked}
  for line in dependencies.stdout.splitlines():
   match=re.search(r'=>\s+(/\S+)|^\s*(/\S+)',line)
   if match:paths.add(match.group(1) or match.group(2))
  def hashes():
   hashed=subprocess.run(['wsl.exe','--','sha256sum',*sorted(paths)],capture_output=True,text=True,timeout=30);assert hashed.returncode==0,hashed.stderr
   return {line.split(maxsplit=1)[1].strip():line.split()[0] for line in hashed.stdout.splitlines()}
  binaries=hashes();assert any('libdh2_level_world.so' in path for path in binaries)
  run=subprocess.run(['wsl.exe','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',a.main_linked,wsl(source[5])],capture_output=True,text=True,timeout=60)
  assert run.returncode==0 and not run.stderr,(run.returncode,run.stdout,run.stderr)
  result=json.loads(run.stdout);assert before=={str(p.relative_to(REPO)):sha(p) for p in source} and binaries==hashes()
  result.update(current_source_bindings=before,binary_bindings=binaries,executable=a.main_linked,ldd_output=dependencies.stdout,asan_ubsan=True,sanitizer_findings=0,scope='Replay actual central world-linked audit. Source/corpus and resolved binary bytes unchanged before/after; this runner does not build or assert compiler provenance or live/Android gameplay parity.')
  (ROOT/'reports/character-ai-attack-main-linked-host-audit.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result));return
 command=['wsl.exe','--','g++','-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror',wsl(source[0]),wsl(source[3]),'-o',wsl(out)]
 build=subprocess.run(command,capture_output=True,text=True,timeout=60);assert build.returncode==0,(build.stdout,build.stderr)
 run=subprocess.run(['wsl.exe','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',wsl(out),wsl(source[5])],capture_output=True,text=True,timeout=60)
 assert run.returncode==0 and not run.stderr,(run.returncode,run.stdout,run.stderr)
 result=json.loads(run.stdout);assert before=={str(p.relative_to(REPO)):sha(p) for p in source}
 result.update(source_bindings=before,compiler_command=command,executable_sha256=sha(out),asan_ubsan=True,sanitizer_findings=0,scope=__doc__)
 report=ROOT/'reports/character-ai-attack-host-audit.json';report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
