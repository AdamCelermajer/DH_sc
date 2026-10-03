"""Source-bound ASan/UBSan replay of original target-search traversal/ordering gold."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def wsl(p):return '/mnt/'+p.drive[0].lower()+str(p)[2:].replace('\\','/')
def main():
 p=argparse.ArgumentParser();p.add_argument('--main-linked',nargs='?',const='/home/adampalace/dh2-world-build/character_target_search_audit');a=p.parse_args()
 source=[ROOT/'character_target_search.cpp',ROOT/'character_target_search.hpp',ROOT/'tests/character_target_search.cpp',Path(__file__),ROOT/'tests/character_target_search_differential.py',ROOT/'tests/character_target_search_discovery.py',ROOT/'reference/character-target-search/search-fixtures.bin',ROOT/'reports/character-target-search-arm64-differential.json']
 before={str(p.relative_to(REPO)):sha(p) for p in source};out=REPO/'.local-inputs/character-target-search-discovery/character_target_search_host';binaries={};command=None
 if a.main_linked:
  ldd=subprocess.run(['wsl.exe','--','ldd',a.main_linked],capture_output=True,text=True,timeout=30);assert ldd.returncode==0 and 'not found' not in ldd.stdout;paths={a.main_linked}
  for line in ldd.stdout.splitlines():
   m=re.search(r'=>\s+(/\S+)|^\s*(/\S+)',line)
   if m:paths.add(m.group(1) or m.group(2))
  def hashes():
   r=subprocess.run(['wsl.exe','--','sha256sum',*sorted(paths)],capture_output=True,text=True,timeout=30);assert r.returncode==0;return {line.split(maxsplit=1)[1].strip():line.split()[0] for line in r.stdout.splitlines()}
  binaries=hashes();assert any('libdh2_level_world.so' in x for x in binaries);executable=a.main_linked
 else:
  command=['wsl.exe','--','g++','-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror','-I'+wsl(ROOT),wsl(source[0]),wsl(source[2]),'-Wl,--wrap=sinf','-Wl,--wrap=cosf','-Wl,--wrap=sincosf','-Wl,--wrap=acosf','-o',wsl(out)]
  build=subprocess.run(command,capture_output=True,text=True,timeout=60);assert build.returncode==0,(build.stdout,build.stderr);executable=wsl(out)
 run=subprocess.run(['wsl.exe','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',executable,wsl(source[6])],capture_output=True,text=True,timeout=60);assert run.returncode==0 and not run.stderr,(run.returncode,run.stdout,run.stderr)
 assert before=={str(p.relative_to(REPO)):sha(p) for p in source};result=json.loads(run.stdout);result.update(source_bindings=before,asan_ubsan=True,sanitizer_findings=0,scope=__doc__,libm_scope='Recorded caller import fixture, independent clock/pose/gameplay not claimed.')
 if a.main_linked:assert binaries==hashes();result.update(binary_bindings=binaries,ldd_output=ldd.stdout,compiler_provenance_claim=False);suffix='main-linked-host-audit'
 else:result.update(compiler_command=command,executable_sha256=sha(out));suffix='host-audit'
 (ROOT/f'reports/character-target-search-{suffix}.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
