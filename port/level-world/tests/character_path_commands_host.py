"""Replay path/facing gold and bind genuine authored-floor route service."""
import argparse,hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/c/'+str(p.resolve()).replace('\\','/')[3:]
def main():
 p=argparse.ArgumentParser();p.add_argument('--build',default='/home/adampalace/dh2-world-build');p.add_argument('--output',type=Path,default=ROOT/'port/level-world/reports/character-path-commands-host-audit.json');a=p.parse_args();commands=[]
 def run(*args):
  r=subprocess.run(['wsl.exe','-e',*args],capture_output=True,text=True,timeout=60);commands.append({'arguments':args,'exit_code':r.returncode,'stdout':r.stdout,'stderr':r.stderr});assert r.returncode==0 and not r.stderr.strip(),commands[-1];return r.stdout.strip()
 files=['character_path_commands.hpp','character_path_commands.cpp','navigation_heading.hpp','navigation_heading.cpp','tests/character_path_commands.cpp','tests/character_path_commands_differential.py','tests/character_path_commands_host.py','CMakeLists.txt'];sources={'port/level-world/'+n:sha(ROOT/'port/level-world'/n) for n in files}
 assert '-fsanitize=address,undefined' in run('cmake','-LA','-N',a.build)
 exe=a.build+'/character_path_commands_audit';library=a.build+'/libdh2_level_world.so';dependencies=run('ldd',exe);assert library in dependencies and 'libasan.so' in dependencies and 'libubsan.so' in dependencies
 before={v:run('sha256sum',v).split()[0] for v in (exe,library)}
 gold=ROOT/'port/level-world/reference/character-path-commands/original-corpus.bin';bres=ROOT/'.local-inputs/world/crypt.bdae';descriptor=ROOT/'.local-inputs/world/crypt01/crypt01.dwld';evidence=ROOT/'port/level-world/reports/character-path-commands-arm64-differential.json';original=json.loads(evidence.read_text());assert original['validation']=='PASS' and original['corpus_sha256']==sha(gold)
 results=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,linux(gold),linux(bres),linux(descriptor)));assert results['validation']=='PASS' and results['path_comparisons']==original['path_comparisons'] and results['look_comparisons']==original['look_comparisons'] and results['genuine_authored_floor_route_sessions']==64
 assert all(sha(ROOT/n)==v for n,v in sources.items());assert before=={v:run('sha256sum',v).split()[0] for v in before}
 report={'validation':'PASS','host_audit':results,'source_sha256':sources,'binary_sha256':before,'linked_dependencies':dependencies,'original_sha256':original['original_sha256'],'original_corpus_sha256':sha(gold),'original_instruction_report_sha256':sha(evidence),'authored_inputs_sha256':{str(v.relative_to(ROOT)).replace('\\','/'):sha(v) for v in (bres,descriptor)},'sanitizers':['AddressSanitizer','UndefinedBehaviorSanitizer'],'sanitizer_findings':0,'commands':commands,'scope':'Actual world DSO replays original PathTo/LookAt gold. The extra64 floor-pair sessions use genuine native FindPath/graph/collision, report actual found/failed outcomes, and check reuse after success; these extra outcomes are not original floor-pair differentials. No live pursuit or whole-game parity claim.'}
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','host_audit':results}))
if __name__=='__main__':main()
