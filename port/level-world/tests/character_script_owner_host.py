"""Actual ownership DSO and Lua runtime audit; unimplemented gameplay registrations are explicit delivery fixtures."""
import argparse,hashlib,json,subprocess,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/c/'+str(p.resolve()).replace('\\','/')[3:]
def main():
 p=argparse.ArgumentParser();p.add_argument('--runtime-build',default='/home/adampalace/dh2-world-build/script-runtime');p.add_argument('--world-build',default='/home/adampalace/dh2-world-build');p.add_argument('--main-linked',action='store_true');p.add_argument('--output',type=Path);a=p.parse_args()
 if a.output is None:a.output=ROOT/('reports/character-script-owner-main-linked-host-audit.json' if a.main_linked else 'reports/character-script-owner-host-audit.json')
 scratch=REPO/'.local-inputs/character-script-owner-discovery';scratch.mkdir(exist_ok=True);commands=[]
 def run(*args):
  r=subprocess.run(['wsl','--cd',linux(REPO),*args],capture_output=True,text=True,timeout=120);commands.append(dict(arguments=list(args),returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0 and not r.stderr.strip(),commands[-1];return r.stdout.strip()
 sources=[ROOT/x for x in ('character_script_owner.hpp','character_script_owner.cpp','character_script_owner_bindings.inc','character_script_lifecycle.hpp','character_script_lifecycle.cpp','character_script_selection.hpp','character_script_selection.cpp','character_timers.hpp','character_timers.cpp','character_script_call_timer.hpp','character_script_call_timer.cpp','tests/character_script_owner.cpp','tests/character_script_owner_differential.py','tests/character_script_owner_host.py')]
 sources += [REPO/'port/script-runtime'/x for x in ('script_runtime.h','script_runtime.c','script_function_alias.h','script_function_alias.cpp','script_game_bindings.h','script_game_bindings.c')]
 bound={str(s.relative_to(REPO)):sha(s) for s in sources};flags=['-std=c++17','-O1','-g','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror']
 library=linux(scratch/'libcharacter_script_owner_audit.so');exe=linux(scratch/'host_audit');runtime=a.runtime_build+'/libdh2_script_runtime.so';world=a.world_build+'/libdh2_level_world.so'
 before={x:run('sha256sum',x).split()[0] for x in (runtime,world)}
 links=['-L'+a.world_build,'-ldh2_level_world','-L'+a.runtime_build,'-ldh2_script_runtime','-Wl,-rpath,'+a.world_build,'-Wl,-rpath,'+a.runtime_build]
 if a.main_linked:library=world;owner_links=[]
 else:
  run('g++',*flags,'-shared','-fPIC',linux(ROOT/'character_script_owner.cpp'),*links,'-o',library)
  owner_links=['-L'+linux(scratch),'-lcharacter_script_owner_audit']
 run('g++',*flags,linux(ROOT/'tests/character_script_owner.cpp'),'-I'+linux(ROOT),*owner_links,*links,'-ldl','-Wl,-rpath,'+linux(scratch),'-o',exe)
 deps=run('ldd',exe);assert runtime in deps and world in deps and library in deps and 'libasan.so' in deps and 'libubsan.so' in deps
 gold=ROOT/'reference/character-script-owner/owner-fixtures.bin';common=scratch/'ai-commons-source.luac'
 cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
 assert sha(cache)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
 with zipfile.ZipFile(cache) as archive:common.write_bytes(archive.read('com.gameloft.android.GAND.GloftD2SS/files/data/scripts/ai/_commons.luac'))
 assert sha(common)=='20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c'
 checks=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,linux(gold),linux(common)));assert checks['validation']=='PASS' and checks['owner_library']==library and checks['runtime_library']==runtime and checks['lifecycle_library']==world and checks['timer_library']==world
 hashes={x:run('sha256sum',x).split()[0] for x in (library,exe,runtime,world)};assert all(hashes[x]==h for x,h in before.items())
 assert all(sha(REPO/x)==h for x,h in bound.items())
 report=dict(validation='PASS',scope=__doc__,host_audit=checks,source_sha256=bound,binary_sha256=hashes,original_sha256='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80',gold_sha256=sha(gold),commons_sha256=sha(common),cache_sha256=sha(cache),original_ownership_probe_sha256=sha(ROOT/'reference/character-script-ownership/ownership-probe.json'),sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer'],sanitizer_findings=0,main_world_library_executed=True,owner_module_in_main_world_library=a.main_linked,linked_dependencies=deps,commands=commands,full_gameplay_namespace_bound=False,production_global_stubs=False)
 a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',report=str(a.output),checks=checks)))
if __name__=='__main__':main()
