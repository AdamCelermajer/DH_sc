"""Replay prebuilt sanitized registration adapter against original-derived gold."""
import argparse,hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--executable',default='.local-inputs/animation-registration-discovery/host-audit');p.add_argument('--output',type=Path,default=ROOT/'reports/animation-registration-host-audit.json');a=p.parse_args()
 kernel_path=ROOT/'reports/animation-registration-arm64-differential.json';kernel=json.loads(kernel_path.read_text());assert kernel['validation']=='PASS' and kernel['mismatches']==0
 for name,value in kernel['source_sha256'].items():assert sha(REPO/name)==value,name
 gold=ROOT/'reference/animation-registration/original-corpus.bin';assert sha(gold)==kernel['reference_sha256']
 command=['wsl','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',a.executable,'port/engine-animation/reference/animation-registration/original-corpus.bin']
 result=subprocess.run(command,capture_output=True,text=True);assert result.returncode==0,(result.stdout,result.stderr);assert not result.stderr.strip(),result.stderr;audit=json.loads(result.stdout)
 assert audit['validation']=='PASS' and audit['cases']==len(kernel['cases']) and audit['appends']==kernel['appends'] and audit['lookups']==kernel['lookups'] and audit['entry_checks']==kernel['entry_checks'] and audit['atomic_rejections']==kernel['atomic_rejections']+1 and audit['sanitizer_findings']==0
 sources=dict(kernel['source_sha256']);sources['port/engine-animation/tests/animation_registration_host.py']=sha(Path(__file__))
 executable=REPO/a.executable;report={'validation':'PASS','host_audit':audit,'sanitizers':['AddressSanitizer','UndefinedBehaviorSanitizer'],'sanitizer_findings':0,'executable':a.executable,'executable_sha256':sha(executable),'source_sha256':sources,'original_instruction_evidence':{'original_sha256':kernel['original_sha256'],'kernel_report_sha256':sha(kernel_path),'reference_sha256':sha(gold),'manifest_sha256':kernel['manifest_sha256'],'producer_report_sha256':kernel['producer_report_sha256']},'scope':'Original-derived registration occurrence/map/default replay, borrowed Player pointers and1024entry capacity atomic rejection. No CMake/Android/asset production changes, pose/component compiler, cache ownership, whole scene or gameplay parity claim.'}
 a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS',**audit}))
if __name__=='__main__':main()
