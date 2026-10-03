"""Genuine DSO replay of original AI event gold, with ASan and UBSan."""
import argparse,hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def linux(path):return '/mnt/c/'+str(path.resolve()).replace('\\','/')[3:]
def main():
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--build',default='/home/adampalace/dh2-world-build');parser.add_argument('--main-linked',action='store_true');parser.add_argument('--output',type=Path,default=ROOT/'reports/character-ai-events-host-audit.json');args=parser.parse_args()
 scratch=REPO/'.local-inputs/character-ai-events-discovery';scratch.mkdir(parents=True,exist_ok=True);commands=[]
 def run(*arguments):
  result=subprocess.run(['wsl','--cd',linux(REPO),*arguments],capture_output=True,text=True,timeout=120);command={'arguments':list(arguments),'returncode':result.returncode,'stdout':result.stdout,'stderr':result.stderr};commands.append(command);assert result.returncode==0 and not result.stderr.strip(),command;return result.stdout.strip()
 sources=[ROOT/'character_ai_events.hpp',ROOT/'character_ai_events.cpp',ROOT/'tests/character_ai_events.cpp',ROOT/'tests/character_ai_events_differential.py',Path(__file__),ROOT/'tools/build_character_ai_events_oracle.ps1',ROOT/'CMakeLists.txt'];bindings={str(path.relative_to(REPO)).replace('\\','/'):sha(path) for path in sources}
 flags=['-std=c++17','-O1','-g','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror'];library=linux(scratch/'libcharacter_ai_events_audit.so');executable=linux(scratch/'host_audit')
 if args.main_linked:
  library=args.build+'/libdh2_level_world.so';executable=args.build+'/character_ai_events_audit';assert '-fsanitize=address,undefined' in run('cmake','-LA','-N',args.build);run('ninja','-C',args.build,'-t','commands','character_ai_events_audit')
 else:
  run('g++',*flags,'-shared','-fPIC',linux(ROOT/'character_ai_events.cpp'),'-o',library)
  run('g++',*flags,linux(ROOT/'tests/character_ai_events.cpp'),'-L'+linux(scratch),'-lcharacter_ai_events_audit','-ldl','-Wl,-rpath,'+linux(scratch),'-o',executable)
 before_library=run('sha256sum',library).split()[0];dependencies=run('ldd',executable);assert library in dependencies and 'libasan.so' in dependencies and 'libubsan.so' in dependencies
 gold=ROOT/'reference/character-ai-events/event-fixtures.bin';original=ROOT/'reports/character-ai-events-arm64-differential.json';evidence=json.loads(original.read_text());assert evidence['validation']=='PASS' and sha(gold)==evidence['corpus_sha256']
 checks=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',executable,linux(gold)));assert checks['validation']=='PASS' and checks['comparisons']==evidence['comparisons'] and checks['ordered_service_requests']==evidence['ordered_service_requests'];assert checks['dispatcher_library']==checks['relay_library']==library
 binary_paths={library,executable};binary_paths.update(v for v in dependencies.split() if v.startswith(args.build+'/') and v.endswith('.so'));binary_hashes={path:run('sha256sum',path).split()[0] for path in sorted(binary_paths)};assert before_library==binary_hashes[library],'DSO changed during replay';assert all(sha(REPO/path)==value for path,value in bindings.items()),'Source changed during replay';assert all(sha(REPO/path)==value for path,value in evidence['source_sha256'].items()),'Original source proof stale'
 refs=['original-functions.json','reference/original-functions.asm','dispatch-producer.json','discover.py']
 report={'validation':'PASS','main_world_library_executed':args.main_linked,'host_audit':checks,'source_sha256':bindings,'binary_sha256':binary_hashes,'linked_dependencies':dependencies,'reference_sha256':{path:sha(ROOT/'reference/character-ai-events'/path) for path in refs},'original_sha256':evidence['original_sha256'],'corpus_sha256':sha(gold),'original_instruction_report_sha256':sha(original),'sanitizers':['AddressSanitizer','UndefinedBehaviorSanitizer'],'sanitizer_findings':0,'sources_unchanged_through_replay':True,'commands':commands,'scope':'Actual original full AI dispatcher gold replayed through the genuine source DSO. Composed cases call the actual native active-gated script-timer relay; recursive callbacks execute the actual event31 kernel. Deeper helpers, virtual AIS/VM and FSM are explicit service fixtures. No Lua/timer/full Character frame/renderer/APK equivalence claim.'}
 args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','report':str(args.output),'checks':checks}))
if __name__=='__main__':main()
