"""Original state gold plus genuine scoped VM/alias/Default-kernel integration; no fake Crypt state registration."""
import argparse,hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/c/'+str(p.resolve()).replace('\\','/')[3:]
def main():
 p=argparse.ArgumentParser();p.add_argument('--main-linked',action='store_true');p.add_argument('--runtime-dir',default='/home/adampalace/dh2-script-include-build');p.add_argument('--output',type=Path);a=p.parse_args();scratch=REPO/'.local-inputs/character-script-states';scratch.mkdir(exist_ok=True);commands=[]
 def run(*args):
  r=subprocess.run(['wsl','--cd',linux(REPO),*args],capture_output=True,text=True,timeout=120);commands.append(dict(arguments=args,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0 and not r.stderr.strip(),commands[-1];return r.stdout.strip()
 flags=['-std=c++17','-O1','-g','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror'];world_dir='/home/adampalace/dh2-world-build';world=world_dir+'/libdh2_level_world.so';runtime=a.runtime_dir+'/libdh2_script_runtime.so';library=linux(scratch/'libcharacter_script_states_audit.so');exe=linux(scratch/'host');gold=ROOT/'reference/character-script-states/state-fixtures.bin'
 sources=[ROOT/x for x in ['character_script_states.hpp','character_script_states.cpp','tests/character_script_states.cpp','tests/character_script_states_host.py']]+[REPO/'port/script-runtime'/x for x in ['script_runtime.h','script_runtime.c']];hashes={str(x.relative_to(REPO)):sha(x) for x in sources};before={x:run('sha256sum',x).split()[0] for x in [world,runtime]};links=['-L'+a.runtime_dir,'-ldh2_script_runtime','-L'+world_dir,'-ldh2_level_world','-Wl,-rpath,'+a.runtime_dir,'-Wl,-rpath,'+world_dir]
 if a.main_linked:library=world;owner_links=[]
 else:
  run('g++',*flags,'-shared','-fPIC',linux(ROOT/'character_script_states.cpp'),*links,'-o',library);owner_links=['-L'+linux(scratch),'-lcharacter_script_states_audit']
 run('g++',*flags,linux(ROOT/'tests/character_script_states.cpp'),'-I'+linux(ROOT),*owner_links,*links,'-Wl,-rpath,'+linux(scratch),'-ldl','-o',exe)
 env=['env','LD_LIBRARY_PATH='+a.runtime_dir+':'+world_dir+':'+linux(scratch),'ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1'];audit=json.loads(run(*env,exe,linux(gold)));assert audit['validation']=='PASS' and audit['owner_library']==library and audit['runtime_library']==runtime
 deps=run(*env,'ldd',exe);assert runtime in deps and world in deps and 'libasan.so' in deps and 'libubsan.so' in deps;binaries={x:run('sha256sum',x).split()[0] for x in [world,runtime,library,exe]};assert all(binaries[x]==digest for x,digest in before.items()) and all(sha(REPO/x)==h for x,h in hashes.items())
 arm=ROOT/'reports/character-script-states-arm64-differential.json';reference=json.loads(arm.read_text());assert reference['validation']=='PASS' and reference['gold_sha256']==sha(gold)
 authored={}
 for name in ['monster','rene','_commons']:
  file=REPO/'.local-inputs/character-script-owner-extension'/(name+'.luac');data=file.read_bytes();authored[name]=dict(sha256=sha(file),bytes=len(data),register_occurrences=data.count(b'RegisterAIState'),change_occurrences=data.count(b'ChangeAIState'));assert not authored[name]['register_occurrences'] and not authored[name]['change_occurrences']
 report=dict(validation='PASS',scope=__doc__,host_audit=audit,source_sha256=hashes,binary_sha256=binaries,original_sha256=reference['original_sha256'],original_gold_sha256=sha(gold),arm64_report_sha256=sha(arm),authored_state_inventory=authored,sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer'],sanitizer_findings=0,main_world_library_executed=True,module_in_main_world=a.main_linked,full_Crypt_script_VM_loaded=False,nonstring_Change_getString_supported=False,packaged_APK=False,commands=commands,linked_dependencies=deps)
 output=a.output or ROOT/('reports/character-script-states-main-linked-host-audit.json' if a.main_linked else 'reports/character-script-states-host-audit.json');output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',host=audit)))
if __name__=='__main__':main()
