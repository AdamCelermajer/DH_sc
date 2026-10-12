"""Actual external-owner class linked to genuine world and private Lua DSOs; original failure/alias prefixes explicit."""
import argparse,hashlib,json,subprocess,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/c/'+str(p.resolve()).replace('\\','/')[3:]
def main():
 p=argparse.ArgumentParser();p.add_argument('--main-linked',action='store_true');p.add_argument('--output',type=Path);a=p.parse_args()
 if a.output is None:a.output=ROOT/('reports/character-script-owner-external-main-linked-host-audit.json' if a.main_linked else 'reports/character-script-owner-external-host-audit.json')
 scratch=REPO/'.local-inputs/character-script-owner-extension';scratch.mkdir(exist_ok=True);commands=[]
 def run(*args):
  result=subprocess.run(['wsl','--cd',linux(REPO),*args],capture_output=True,text=True,timeout=120);commands.append(dict(arguments=args,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0 and not result.stderr.strip(),commands[-1];return result.stdout.strip()
 inputs=json.loads((ROOT/'reference/character-script-kinds/authored-ai-rows.json').read_text());cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip');assert sha(cache)==inputs['cache_sha256'];resources={}
 with zipfile.ZipFile(cache) as z:
  for record in inputs['required_cache_resources']:
   data=z.read(record['entry']);assert hashlib.sha256(data).hexdigest()==record['sha256'];file=scratch/(record['requested']+'.luac');file.write_bytes(data);resources[record['requested']]=file
 sources=[ROOT/x for x in ['character_script_owner.cpp','character_script_owner.hpp','character_script_owner_bindings.inc','character_script_virtual.cpp','character_script_virtual.hpp','tests/character_script_owner.cpp','tests/character_script_owner_external.cpp','tests/character_script_owner_external_host.py']]
 hashes={str(x.relative_to(REPO)):sha(x) for x in sources};flags=['-std=c++17','-O1','-g','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror']
 world_dir='/home/adampalace/dh2-world-build';runtime_dir=world_dir+'/script-runtime';runtime=runtime_dir+'/libdh2_script_runtime.so';world=world_dir+'/libdh2_level_world.so';library=linux(scratch/'libcharacter_script_owner_external_audit.so');exe=linux(scratch/'external_host');before={x:run('sha256sum',x).split()[0] for x in [world,runtime]}
 links=['-L'+world_dir,'-ldh2_level_world','-L'+runtime_dir,'-ldh2_script_runtime','-Wl,-rpath,'+world_dir,'-Wl,-rpath,'+runtime_dir]
 if a.main_linked:library=world;owner_links=[]
 else:
  run('g++',*flags,'-shared','-fPIC',linux(ROOT/'character_script_owner.cpp'),*links,'-o',library);owner_links=['-L'+linux(scratch),'-lcharacter_script_owner_external_audit']
 run('g++',*flags,linux(ROOT/'tests/character_script_owner_external.cpp'),'-I'+linux(ROOT),*owner_links,*links,'-Wl,-rpath,'+linux(scratch),'-ldl','-o',exe)
 checks=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,*(linux(resources[x]) for x in ['_commons','follower','monster','rene'])));assert checks['validation']=='PASS' and checks['owner_library']==library and checks['virtual_library']==world and checks['runtime_library']==runtime
 legacy_exe=linux(scratch/'legacy_host')
 run('g++',*flags,linux(ROOT/'tests/character_script_owner.cpp'),'-I'+linux(ROOT),*owner_links,*links,'-Wl,-rpath,'+linux(scratch),'-ldl','-o',legacy_exe)
 legacy=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',legacy_exe,linux(ROOT/'reference/character-script-owner/owner-fixtures.bin'),linux(resources['_commons'])));assert legacy['validation']=='PASS' and legacy['checks']==6283 and legacy['owner_library']==library
 deps=run('ldd',exe);assert runtime in deps and world in deps and 'libasan.so' in deps and 'libubsan.so' in deps
 binaries={x:run('sha256sum',x).split()[0] for x in [world,runtime,library,exe,legacy_exe]};assert all(binaries[x]==h for x,h in before.items()) and all(sha(REPO/x)==h for x,h in hashes.items())
 original=ROOT/'reference/character-script-owner-extension/external-owner-probe.json';evidence=json.loads(original.read_text());assert evidence['validation']=='PASS'
 report=dict(validation='PASS',scope=__doc__,host_audit=checks,legacy_host_regression=legacy,source_sha256=hashes,binary_sha256=binaries,original_sha256=evidence['original_sha256'],original_order_evidence_sha256=sha(original),original_order_cases=len(evidence['cases']),cache_sha256=sha(cache),authored_input_evidence_sha256=sha(ROOT/'reference/character-script-kinds/authored-ai-rows.json'),authored_resource_sha256={x:sha(y) for x,y in resources.items()},sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer'],sanitizer_findings=0,main_world_library_executed=True,owner_module_in_main_world_library=a.main_linked,full_gameplay_namespace_bound=False,fake_gameplay_globals=False,commands=commands,linked_dependencies=deps)
 a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',checks=checks)))
if __name__=='__main__':main()
