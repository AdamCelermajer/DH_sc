"""Replay original controller corpus through the actual native world DSO."""
import argparse,hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--build',default='/home/adampalace/dh2-world-build');p.add_argument('--report',type=Path,default=ROOT/'port/level-world/reports/character-controller-commands-host-audit.json');a=p.parse_args();commands=[]
 def run(*args):
  r=subprocess.run(['wsl.exe','-e',*args],capture_output=True,text=True,timeout=60);commands.append({'arguments':args,'exit_code':r.returncode,'stdout':r.stdout,'stderr':r.stderr});assert r.returncode==0 and not r.stderr.strip(),commands[-1];return r.stdout.strip()
 names=['character_controller_commands.hpp','character_controller_commands.cpp','tests/character_controller_commands.cpp','tests/character_controller_commands_differential.py','tests/character_controller_commands_host.py','CMakeLists.txt']
 sources={'port/level-world/'+n:sha(ROOT/'port/level-world'/n) for n in names}
 config=run('cmake','-LA','-N',a.build);assert '-fsanitize=address,undefined' in config
 exe=a.build+'/character_controller_commands_audit';library=a.build+'/libdh2_level_world.so'
 deps=run('ldd',exe);assert library in deps and 'libasan.so' in deps and 'libubsan.so' in deps
 gold=ROOT/'port/level-world/reference/character-controller-commands/original-corpus.bin'
 instruction=ROOT/'port/level-world/reports/character-controller-commands-project-arm64-differential.json';evidence=json.loads(instruction.read_text());assert evidence['validation']=='PASS' and evidence['corpus_sha256']==sha(gold)
 results=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,'/mnt/c/'+str(gold).replace('\\','/')[3:]));assert results['validation']=='PASS' and results['original_corpus_cases']==evidence['comparisons'] and results['ordered_service_requests']==evidence['ordered_service_requests']
 binaries={f:run('sha256sum',f).split()[0] for f in (exe,library)};assert all(sha(ROOT/n)==v for n,v in sources.items())
 report={'validation':'PASS','host_audit':results,'source_sha256':sources,'binary_sha256':binaries,'linked_dependencies':deps,'original_sha256':evidence['original_sha256'],'original_corpus_sha256':sha(gold),'project_arm64_instruction_report_sha256':sha(instruction),'sanitizers':['AddressSanitizer','UndefinedBehaviorSanitizer'],'sanitizer_findings':0,'commands':commands,'scope':'Actual world shared library replays original controller/Character command corpus. Deeper GameObject Stop/PathTo/LookAt(Point) and Character events remain synchronous fixtures; no live renderer or complete gameplay claim.'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','host_audit':results}))
if __name__=='__main__':main()
