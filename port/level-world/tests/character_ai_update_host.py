"""Standalone genuine DSO wrapper replay plus production selected-AIS composition."""
import argparse,hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/c/'+str(p.resolve()).replace('\\','/')[3:]
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--build',default='/home/adampalace/dh2-world-build');p.add_argument('--main-linked',action='store_true',help='Wrapper and selected kernel both execute from the already built production world DSO.');p.add_argument('--output',type=Path,default=ROOT/'reports/character-ai-update-host-audit.json');a=p.parse_args()
 scratch=REPO/'.local-inputs/character-ai-update-discovery';scratch.mkdir(parents=True,exist_ok=True);commands=[]
 def run(*args):
  r=subprocess.run(['wsl','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc',*args],capture_output=True,text=True,timeout=120);commands.append({'arguments':list(args),'returncode':r.returncode,'stdout':r.stdout,'stderr':r.stderr});assert r.returncode==0 and not r.stderr.strip(),commands[-1];return r.stdout.strip()
 sources=[ROOT/'character_ai_update.hpp',ROOT/'character_ai_update.cpp',ROOT/'tests/character_ai_update.cpp',ROOT/'tests/character_ai_update_differential.py',Path(__file__),ROOT/'character_script_update.hpp',ROOT/'character_script_update.cpp',ROOT/'CMakeLists.txt']
 before={x.relative_to(REPO).as_posix():sha(x) for x in sources};flags=['-std=c++17','-O1','-g','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror'];library=linux(scratch/'libcharacter_ai_update_audit.so');exe=linux(scratch/'host_audit')
 if a.main_linked:
  library=a.build+'/libdh2_level_world.so'
  exe=a.build+'/character_ai_update_audit'
  configuration=run('cmake','-LA','-N',a.build);assert '-fsanitize=address,undefined' in configuration
  run('ninja','-C',a.build,'-t','commands','character_ai_update_audit')
 else:
  run('g++',*flags,'-shared','-fPIC',linux(ROOT/'character_ai_update.cpp'),'-o',library)
  run('g++',*flags,linux(ROOT/'tests/character_ai_update.cpp'),'-L'+linux(scratch),'-L'+a.build,'-lcharacter_ai_update_audit','-ldh2_level_world','-ldl','-Wl,-rpath,'+linux(scratch)+':'+a.build,'-o',exe)
 dependencies=run('ldd',exe);assert library in dependencies and a.build+'/libdh2_level_world.so' in dependencies and 'libasan.so' in dependencies and 'libubsan.so' in dependencies
 gold=ROOT/'reference/character-ai-update/update-fixtures.bin';original=ROOT/'reports/character-ai-update-arm64-differential.json';evidence=json.loads(original.read_text());assert evidence['validation']=='PASS' and sha(gold)==evidence['corpus_sha256']
 checks=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,linux(gold)));assert checks['validation']=='PASS' and checks['comparisons']==evidence['comparisons'] and checks['ordered_service_requests']==evidence['ordered_service_requests']
 if a.main_linked:assert checks['wrapper_library']==checks['selected_library']==library
 bindings={path:run('sha256sum',path).split()[0] for path in [exe,library]+sorted({v for v in dependencies.split() if v.startswith(a.build+'/') and v.endswith('.so')})}
 assert all(sha(REPO/name)==value for name,value in before.items()),'Source changed during DSO build/replay'
 for name,value in evidence['source_sha256'].items():assert sha(REPO/name)==value,name
 scope='All actual-original wrapper gold replayed through production libdh2_level_world.so; its actual selected AISDefault kernel also executes in the composed callback.' if a.main_linked else 'All actual-original wrapper gold replayed through a standalone genuine source DSO; source-built AISDefault kernel executes from production libdh2_level_world.so in the selected-script composition.'
 report={'validation':'PASS','main_world_library_executed':a.main_linked,'host_audit':checks,'source_sha256':before,'binary_sha256':bindings,'linked_dependencies':dependencies,'sanitizers':['AddressSanitizer','UndefinedBehaviorSanitizer'],'sanitizer_findings':0,'original_sha256':evidence['original_sha256'],'corpus_sha256':sha(gold),'original_instruction_report_sha256':sha(original),'sources_unchanged_through_replay':True,'commands':commands,'scope':scope+' Timer/Stop/zoning/position are explicit service fixtures. No original full-frame, Lua/backend, live renderer or APK claim.'}
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','report':str(a.output),'checks':checks}))
if __name__=='__main__':main()
