"""Source-root/Include owner integration through genuine sanitized world/Lua DSOs."""
import argparse,hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/c/'+str(p.resolve()).replace('\\','/')[3:]
def main():
 p=argparse.ArgumentParser();p.add_argument('--main-linked',action='store_true');p.add_argument('--runtime-dir',default='/home/adampalace/dh2-script-include-build');p.add_argument('--output',type=Path);a=p.parse_args()
 if a.output is None:a.output=ROOT/('reports/character-script-owner-include-main-linked-host-audit.json' if a.main_linked else 'reports/character-script-owner-include-host-audit.json')
 scratch=REPO/'.local-inputs/character-script-owner-include';scratch.mkdir(exist_ok=True);commands=[]
 def run(*args):
  r=subprocess.run(['wsl','--cd',linux(REPO),*args],capture_output=True,text=True,timeout=120);commands.append(dict(arguments=args,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0 and not r.stderr.strip(),commands[-1];return r.stdout.strip()
 original_resources=REPO/'.local-inputs/character-script-owner-extension';resources={name:original_resources/(name+'.luac') for name in ['_commons','follower','monster','rene']};assert all(x.exists() for x in resources.values())
 sources=[ROOT/x for x in ['character_script_owner.cpp','character_script_owner.hpp','character_script_owner_bindings.inc','tests/character_script_owner.cpp','tests/character_script_owner_external.cpp','tests/character_script_owner_include.cpp','tests/character_script_owner_include_host.py']]+[REPO/'port/script-runtime'/x for x in ['script_runtime.h','script_runtime.c']]
 hashes={str(x.relative_to(REPO)):sha(x) for x in sources};flags=['-std=c++17','-O1','-g','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror']
 world_dir='/home/adampalace/dh2-world-build';runtime=a.runtime_dir+'/libdh2_script_runtime.so';world=world_dir+'/libdh2_level_world.so';library=linux(scratch/'libcharacter_script_owner_include_audit.so');before={x:run('sha256sum',x).split()[0] for x in [world,runtime]}
 links=['-L'+a.runtime_dir,'-ldh2_script_runtime','-L'+world_dir,'-ldh2_level_world','-Wl,-rpath,'+a.runtime_dir,'-Wl,-rpath,'+world_dir];owner_links=[]
 if a.main_linked:library=world
 else:
  run('g++',*flags,'-shared','-fPIC',linux(ROOT/'character_script_owner.cpp'),*links,'-o',library);owner_links=['-L'+linux(scratch),'-lcharacter_script_owner_include_audit']
 audits={};binaries={};deps={}
 for tag,test,args in [('include','character_script_owner_include.cpp',[resources['_commons'],resources['follower']]),('external','character_script_owner_external.cpp',list(resources.values())),('legacy','character_script_owner.cpp',[ROOT/'reference/character-script-owner/owner-fixtures.bin',resources['_commons']])]:
  exe=linux(scratch/(tag+'_host'));run('g++',*flags,linux(ROOT/'tests'/test),'-I'+linux(ROOT),*owner_links,*links,'-Wl,-rpath,'+linux(scratch),'-ldl','-o',exe)
  audit=json.loads(run('env','LD_LIBRARY_PATH='+a.runtime_dir+':'+world_dir+':'+linux(scratch),'ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,*(linux(x) for x in args)))
  assert audit['validation']=='PASS' and audit['owner_library']==library and audit['runtime_library']==runtime
  audits[tag]=audit;binaries[exe]=run('sha256sum',exe).split()[0];deps[tag]=run('env','LD_LIBRARY_PATH='+a.runtime_dir+':'+world_dir+':'+linux(scratch),'ldd',exe)
  assert runtime in deps[tag] and world in deps[tag] and 'libasan.so' in deps[tag] and 'libubsan.so' in deps[tag]
 assert audits['legacy']['checks']==6283 and audits['external']['checks']==2863
 binaries.update({x:run('sha256sum',x).split()[0] for x in [world,runtime,library]});assert all(binaries[x]==h for x,h in before.items()) and all(sha(REPO/x)==h for x,h in hashes.items())
 evidence=[ROOT/'reference/character-script-ownership/ownership-probe.json',ROOT/'reference/character-script-owner-extension/external-owner-probe.json',REPO/'port/script-runtime/reports/script-source-file-host-audit.json']
 assert all(json.loads(x.read_text())['validation']=='PASS' for x in evidence)
 report=dict(validation='PASS',scope=__doc__,source_sha256=hashes,binary_sha256=binaries,host_audits=audits,original_instruction_evidence_sha256={str(x.relative_to(REPO)):sha(x) for x in evidence},original_sha256='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80',authored_resource_sha256={k:sha(v) for k,v in resources.items()},sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer'],sanitizer_findings=0,main_world_library_executed=True,owner_module_in_main_world_library=a.main_linked,exact_source_root_loader=True,persistent_include_provider=True,full_gameplay_namespace_bound=False,fake_gameplay_globals=False,commands=commands,linked_dependencies=deps)
 a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',audits=audits)))
if __name__=='__main__':main()
