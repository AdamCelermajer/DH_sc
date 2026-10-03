"""Bind actual CMake world-library collision replay to stable source and DSO bytes."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def run(*args):
 r=subprocess.run(['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc',*args],capture_output=True,text=True,timeout=120)
 assert r.returncode==0,(r.returncode,r.stdout,r.stderr)
 assert not r.stderr.strip(),r.stderr
 return r.stdout.strip()
def main():
 p=argparse.ArgumentParser();p.add_argument('--build',default='/home/adampalace/dh2-world-build');p.add_argument('--output',type=Path,default=ROOT/'reports/character-script-collision-main-linked-host-audit.json');a=p.parse_args()
 ref=ROOT/'reference/character-script-collision';e=json.loads((ref/'original-arm64.json').read_text());assert e['validation']=='PASS' and not e['mismatches']
 assert sha(ref/'collision-fixtures.bin')==e['corpus_sha256'] and sha(ref/'original-functions.json')==e['manifest_sha256']
 for n,h in e['source_sha256'].items():assert sha(REPO/n)==h,n
 roots=['level-world','engine-skinning','engine-animation','engine-math','scene-materials','engine-resources','engine-textures','game-data','physics-backend']
 sources=[]
 for name in roots:
  for path in (REPO/'port'/name).rglob('*'):
   if path.is_file() and not any(part in ('build','.git','CMakeFiles') for part in path.relative_to(REPO/'port'/name).parts) and (path.suffix in ('.cpp','.hpp','.h','.c') or path.name=='CMakeLists.txt'):sources.append(path)
 sources.append(Path(__file__));hashes={str(path.relative_to(REPO)).replace('\\','/'):sha(path) for path in sorted(set(sources))}
 cache=run('cat',a.build+'/CMakeCache.txt');flags=re.search(r'^CMAKE_CXX_FLAGS:STRING=(.*)$',cache,re.M).group(1);assert '-fsanitize=address,undefined' in flags,flags
 command=['/usr/bin/cmake','--build',a.build,'--target','character_script_collision_audit','-j','4'];stdout=run(*command)
 assert all(sha(REPO/n)==h for n,h in hashes.items()),'Source changed during build; rerun after edits settle'
 exe=a.build+'/character_script_collision_audit';ldd=run('ldd',exe);assert a.build+'/libdh2_level_world.so' in ldd,ldd
 bindings={exe:run('sha256sum',exe).split()[0]}
 for line in ldd.splitlines():
  match=re.search(r'=> (\S+) ',line)
  if match and match.group(1).startswith(a.build+'/'):
   path=match.group(1);bindings[path]=run('sha256sum',path).split()[0]
 assert 'libasan' in ldd and 'libubsan' in ldd,ldd
 host=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1',exe,'port/level-world/reference/character-script-collision/collision-fixtures.bin'))
 for n in ('comparisons','ordered_callbacks','nested_callback_cases','mismatches'):assert host[n]==e[n],n
 assert host['atomic_rejection_checks']==14 and host['counter_lifecycle_checks']==3
 assert all(sha(REPO/n)==h for n,h in hashes.items()),'Source changed during replay'
 assert all(run('sha256sum',path).split()[0]==h for path,h in bindings.items()),'Linked binaries changed during replay'
 assert run('cat',a.build+'/CMakeCache.txt')==cache,'CMake configuration changed'
 report={'validation':'PASS','scope':'Actual CMake production level-world DSO replay under ASan/UBSan; named virtual/enemy/sneaking/target services remain explicit. No original contact solver, Application clock-loop, APK or device claim.','original_sha256':e['original_sha256'],'host_audit':host,'sanitizers':['AddressSanitizer','UndefinedBehaviorSanitizer'],'sanitizer_findings':0,'build_command':command,'build_stdout':stdout,'compiler':run('/usr/bin/c++','--version').splitlines()[0],'cmake_cache_sha256':hashlib.sha256(cache.encode()).hexdigest(),'source_sha256':hashes,'binary_sha256':bindings,'ldd':ldd,'original_instruction_evidence':{'report_sha256':sha(ref/'original-arm64.json'),**e}}
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS',**host,'world_sha256':bindings[a.build+'/libdh2_level_world.so']}))
if __name__=='__main__':main()
