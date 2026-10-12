"""Original-derived Idle gold plus genuine frozen world FSM/State/IsPlayer DSO composition."""
import argparse,hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/c/'+str(p.resolve()).replace('\\','/')[3:]
def main():
 p=argparse.ArgumentParser();p.add_argument('--main-linked',action='store_true');p.add_argument('--dependency-directory',type=Path,default=REPO/'.local-inputs/character-idle-update-discovery/dependencies');p.add_argument('--output',type=Path);a=p.parse_args();scratch=REPO/'.local-inputs/character-idle-update-discovery';commands=[]
 def run(*args):
  r=subprocess.run(['wsl','--cd',linux(REPO),*args],capture_output=True,text=True,timeout=120);commands.append(dict(arguments=args,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0 and not r.stderr.strip(),dict(args=args,stdout=r.stdout[-1800:],stderr=r.stderr[-6000:]);return r.stdout.strip()
 dependencies=list(a.dependency_directory.glob('*.so'));assert len(dependencies)>=3;before={linux(p):sha(p)for p in dependencies};dep=linux(a.dependency_directory);flags=['-std=c++17','-O1','-g','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror','-Wno-misleading-indentation'];links=['-L'+dep,'-ldh2_level_world','-ldh2_game_data','-ldh2_script_runtime','-Wl,-rpath,'+dep];module=linux(scratch/'libcharacter_idle_update_audit.so');exe=linux(scratch/'host');extra=[]
 files=[ROOT/x for x in ['character_idle_update.hpp','character_idle_update.cpp','tests/character_idle_update.cpp','tests/character_idle_update_host.py','tests/character_idle_update_differential.py','character_state.hpp','character_state.cpp','character_native_fsm.hpp','character_native_fsm.cpp','character_target_providers.hpp','character_target_providers.cpp']];sources={x.relative_to(REPO).as_posix():sha(x)for x in files}
 if a.main_linked:module=dep+'/libdh2_level_world.so'
 else:
  run('g++',*flags,'-fPIC','-shared',linux(ROOT/'character_idle_update.cpp'),'-Wl,--no-undefined','-o',module);extra=['-L'+linux(scratch),'-lcharacter_idle_update_audit','-Wl,-rpath,'+linux(scratch)]
 run('g++',*flags,linux(ROOT/'tests/character_idle_update.cpp'),*extra,*links,'-ldl','-o',exe)
 env=['env','LD_LIBRARY_PATH='+dep+':'+linux(scratch),'ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1'];gold=ROOT/'reference/character-idle-update/idle-fixtures.bin';audit=json.loads(run(*env,exe,linux(gold)));linked=run(*env,'ldd',exe);assert audit['validation']=='PASS' and audit['module_library']==module and audit['world_library']==audit['provider_library']==dep+'/libdh2_level_world.so';assert dep+'/libdh2_level_world.so' in linked and 'libasan.so' in linked and 'libubsan.so' in linked
 assert all(sha(p)==before[linux(p)]for p in dependencies) and all(sha(REPO/x)==v for x,v in sources.items());arm=ROOT/'reports/character-idle-update-arm64-differential.json';proof=json.loads(arm.read_text());assert proof['binary_gold_sha256']==sha(gold) and all(sha(REPO/x)==v for x,v in proof['source_sha256'].items())
 report=dict(validation='PASS',host_audit=audit,scope=__doc__,source_sha256=sources,binary_sha256={**before,module:run('sha256sum',module).split()[0],exe:run('sha256sum',exe).split()[0]},binary_gold_sha256=sha(gold),original_sha256=proof['original_sha256'],arm64_report_sha256=sha(arm),sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer'],sanitizer_findings=0,actual_frozen_world_library_executed=True,module_in_main_world=a.main_linked,packaged_APK=False,required_player_manager_online_constant_controller_event_providers_complete=False,commands=commands,linked_dependencies=linked)
 output=a.output or ROOT/('reports/character-idle-update-main-linked-host-audit.json'if a.main_linked else 'reports/character-idle-update-host-audit.json');output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',host=audit)))
if __name__=='__main__':main()
