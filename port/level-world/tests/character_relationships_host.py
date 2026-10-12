"""Bound sanitizer replay of source relationships with genuine native provider backends."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def wsl(p):return '/mnt/'+p.drive[0].lower()+str(p)[2:].replace('\\','/')
def main():
 p=argparse.ArgumentParser();p.add_argument('--main-linked',nargs='?',const='/home/adampalace/dh2-world-build/character_relationships_audit');a=p.parse_args()
 inputs=[ROOT/'character_relationships.cpp',ROOT/'character_relationships.hpp',ROOT/'character_target_providers.cpp',ROOT/'character_target_providers.hpp',ROOT/'tests/character_relationships.cpp',Path(__file__),ROOT/'tests/character_relationships_differential.py',ROOT/'reference/character-relationships/relationship-fixtures.bin',ROOT/'reports/character-relationships-arm64-differential.json',REPO/'port/game-data/ai.hpp',ROOT/'character_combat_queries.hpp']
 before={str(x.relative_to(REPO)):sha(x) for x in inputs};out=REPO/'.local-inputs/character-relationships-discovery/character_relationships_host';command=None;binaries={}
 if a.main_linked:
  dep=subprocess.run(['wsl.exe','--','ldd',a.main_linked],capture_output=True,text=True,timeout=30);assert dep.returncode==0 and 'not found' not in dep.stdout;paths={a.main_linked}
  for line in dep.stdout.splitlines():
   m=re.search(r'=>\s+(/\S+)|^\s*(/\S+)',line)
   if m:paths.add(m.group(1) or m.group(2))
  def hashes():
   r=subprocess.run(['wsl.exe','--','sha256sum',*sorted(paths)],capture_output=True,text=True,timeout=30);assert r.returncode==0;return {line.split(maxsplit=1)[1].strip():line.split()[0] for line in r.stdout.splitlines()}
  binaries=hashes();assert any('libdh2_level_world.so' in x for x in binaries);executable=a.main_linked
 else:
  command=['wsl.exe','--','g++','-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-ffp-contract=off','-Wall','-Wextra','-Werror','-I'+wsl(ROOT),wsl(inputs[0]),wsl(inputs[2]),wsl(inputs[4]),'-o',wsl(out)]
  build=subprocess.run(command,capture_output=True,text=True,timeout=60);assert build.returncode==0,(build.stdout,build.stderr);executable=wsl(out)
 run=subprocess.run(['wsl.exe','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',executable,wsl(inputs[7])],capture_output=True,text=True,timeout=60);assert run.returncode==0 and not run.stderr,(run.returncode,run.stdout,run.stderr)
 assert before=={str(x.relative_to(REPO)):sha(x) for x in inputs};result=json.loads(run.stdout);result.update(asan_ubsan=True,sanitizer_findings=0,source_bindings=before,scope=__doc__)
 if a.main_linked:assert binaries==hashes();result.update(binary_bindings=binaries,ldd_output=dep.stdout,compiler_provenance_claim=False);suffix='main-linked-host-audit'
 else:result.update(compiler_command=command,executable_sha256=sha(out));suffix='host-audit'
 (ROOT/f'reports/character-relationships-{suffix}.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
