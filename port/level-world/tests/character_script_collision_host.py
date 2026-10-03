"""Build, freeze source hashes, and replay original collision fixtures under ASan/UBSan."""
import argparse,hashlib,json,shlex,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def run(*args):
 r=subprocess.run(['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc',*args],capture_output=True,text=True,timeout=120)
 assert r.returncode==0,(r.returncode,r.stdout,r.stderr)
 assert not r.stderr.strip(),r.stderr
 return r.stdout.strip()
def main():
 p=argparse.ArgumentParser();p.add_argument('--rebuild',action='store_true');p.add_argument('--executable',default='/home/adampalace/dh2-world-build/character_script_collision_isolated_audit');p.add_argument('--output',type=Path,default=ROOT/'reports/character-script-collision-host-audit.json');a=p.parse_args()
 ref=ROOT/'reference/character-script-collision';e=json.loads((ref/'original-arm64.json').read_text());assert e['validation']=='PASS' and e['mismatches']==0
 assert sha(ref/'collision-fixtures.bin')==e['corpus_sha256'] and sha(ref/'original-functions.json')==e['manifest_sha256']
 for n,h in e['source_sha256'].items():assert sha(REPO/n)==h,n
 names=['port/level-world/character_script_collision.cpp','port/level-world/character_script_collision.hpp','port/level-world/tests/character_script_collision.cpp','port/level-world/tests/character_script_collision_differential.py','port/level-world/tests/character_script_collision_host.py']
 hashes={n:sha(REPO/n) for n in names};command=['g++','-std=c++17','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-fno-omit-frame-pointer','-g',names[0],names[2],'-o',a.executable]
 if a.rebuild:run(*command)
 inputs={'source_sha256':hashes,'executable_sha256':run('sha256sum',a.executable).split()[0],'compiler':run('g++','--version').splitlines()[0],'command':shlex.join(command)}
 assert all(sha(REPO/n)==h for n,h in hashes.items()),'Source changed during build'
 snapshot=REPO/'.local-inputs/character-script-collision-discovery/host-build-inputs.json'
 if a.rebuild:snapshot.write_text(json.dumps(inputs,indent=2)+'\n')
 else:assert json.loads(snapshot.read_text())==inputs,'Rebuild with stable inputs'
 host=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',a.executable,'port/level-world/reference/character-script-collision/collision-fixtures.bin'))
 for n in ('comparisons','ordered_callbacks','nested_callback_cases','mismatches'):assert host[n]==e[n],n
 assert host['atomic_rejection_checks']==14 and host['counter_lifecycle_checks']==3
 assert all(sha(REPO/n)==h for n,h in hashes.items()),'Source changed during replay'
 report={'validation':'PASS',**inputs,'build_inputs_sha256':sha(snapshot),'host_audit':host,'sanitizers':['AddressSanitizer','UndefinedBehaviorSanitizer'],'sanitizer_findings':0,'original_instruction_evidence':{'report_sha256':sha(ref/'original-arm64.json'),**e},'scope':e['scope']+' Explicit counter lifecycle checks and atomic native boundary guards; no contact/AI backend or Application wall-clock producer claim.'}
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(host))
if __name__=='__main__':main()
