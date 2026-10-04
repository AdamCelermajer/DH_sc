"""Original FSM prelude gold replay; genuine linked four-state producer and private float32 Lua getter bridge."""
import argparse,hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/c/'+str(p.resolve()).replace('\\','/')[3:]
def main():
 p=argparse.ArgumentParser();p.add_argument('--main-linked',action='store_true');p.add_argument('--runtime-dir',default='/home/adampalace/dh2-script-callback-native-build');p.add_argument('--output',type=Path);a=p.parse_args();scratch=REPO/'.local-inputs/character-native-fsm';scratch.mkdir(exist_ok=True);commands=[]
 def run(*args):
  r=subprocess.run(['wsl','--cd',linux(REPO),*args],capture_output=True,text=True,timeout=120);commands.append(dict(arguments=args,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0 and not r.stderr.strip(),commands[-1];return r.stdout.strip()
 flags=['-std=c++17','-O1','-g','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror'];world_dir='/home/adampalace/dh2-world-build';world=world_dir+'/libdh2_level_world.so';runtime=a.runtime_dir+'/libdh2_script_runtime.so';library=linux(scratch/'libcharacter_native_fsm_audit.so');exe=linux(scratch/'host');gold=ROOT/'reference/character-native-fsm/native-update-fixtures.bin'
 paths=[ROOT/x for x in ['character_native_fsm.hpp','character_native_fsm.cpp','tests/character_native_fsm.cpp','tests/character_native_fsm_host.py','character_state.hpp','character_state.cpp']]+[REPO/'port/script-runtime'/x for x in ['script_runtime.h','script_runtime.c']];sources={str(x.relative_to(REPO)):sha(x) for x in paths};before={x:run('sha256sum',x).split()[0] for x in [world,runtime]};links=['-L'+a.runtime_dir,'-ldh2_script_runtime','-L'+world_dir,'-ldh2_level_world','-Wl,-rpath,'+a.runtime_dir,'-Wl,-rpath,'+world_dir]
 if a.main_linked:library=world;module_links=[]
 else:
  run('g++',*flags,'-shared','-fPIC',linux(ROOT/'character_native_fsm.cpp'),*links,'-o',library);module_links=['-L'+linux(scratch),'-lcharacter_native_fsm_audit']
 run('g++',*flags,linux(ROOT/'tests/character_native_fsm.cpp'),'-I'+linux(ROOT),*module_links,*links,'-Wl,-rpath,'+linux(scratch),'-ldl','-o',exe)
 env=['env','LD_LIBRARY_PATH='+a.runtime_dir+':'+world_dir+':'+linux(scratch),'ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1'];audit=json.loads(run(*env,exe,linux(gold)));assert audit['validation']=='PASS' and audit['module_library']==library and audit['world_library']==world and audit['runtime_library']==runtime
 dependencies=run(*env,'ldd',exe);assert runtime in dependencies and world in dependencies and 'libasan.so' in dependencies and 'libubsan.so' in dependencies;binaries={x:run('sha256sum',x).split()[0] for x in [world,runtime,library,exe]};assert all(binaries[x]==digest for x,digest in before.items()) and all(sha(REPO/x)==h for x,h in sources.items())
 arm=ROOT/'reports/character-native-fsm-arm64-differential.json';reference=json.loads(arm.read_text());assert reference['validation']=='PASS' and reference['gold_sha256']==sha(gold)
 report=dict(validation='PASS',scope=__doc__,host_audit=audit,source_sha256=sources,binary_sha256=binaries,original_sha256=reference['original_sha256'],original_gold_sha256=sha(gold),arm64_report_sha256=sha(arm),sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer'],sanitizer_findings=0,main_world_library_executed=True,module_in_main_world=a.main_linked,full_native_FSM=False,stun_scare_and_idle_common_services_explicit=True,script_registration_provider_bound=False,packaged_APK=False,commands=commands,linked_dependencies=dependencies)
 output=a.output or ROOT/('reports/character-native-fsm-main-linked-host-audit.json' if a.main_linked else 'reports/character-native-fsm-host-audit.json');output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',host=audit)))
if __name__=='__main__':main()
