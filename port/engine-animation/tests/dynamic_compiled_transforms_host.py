"""Replay prebuilt sanitized dynamic/static audits; no build or Android action."""
import argparse,hashlib,json,subprocess,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def run(*args):
 result=subprocess.run(['wsl','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc',*args],capture_output=True,text=True)
 assert result.returncode==0,(result.returncode,result.stdout,result.stderr)
 assert not result.stderr.strip(),result.stderr
 return result.stdout.strip()
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--executable',default='/home/adampalace/dh2-world-build/engine-skinning/engine-animation/dynamic_compiled_transforms_audit');p.add_argument('--static-executable',default='/home/adampalace/dh2-world-build/dynamic-static-regression-audit')
 p.add_argument('--cache',type=Path,default=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip'));p.add_argument('--output',type=Path,default=ROOT/'reports/dynamic-compiled-transforms-host-audit.json');a=p.parse_args()
 kernel_path=ROOT/'reports/dynamic-compiled-transforms-arm64-differential.json';kernel=json.loads(kernel_path.read_text());assert kernel['validation']=='PASS' and kernel['mismatches']==0
 for path,value in kernel['source_sha256'].items():assert sha(REPO/path)==value,path
 gold=ROOT/'reference/dynamic-compiled-transforms/original-corpus.bin';static_gold=ROOT/'reference/compiled-transforms/original-corpus.bin'
 assert sha(gold)==kernel['reference_sha256'];assert sha(static_gold)==kernel['input_corpus_sha256']=='d5999b9cd6f9bec206d13dfd17b4e24101623f9a0126c0fb8831d6e15ca2ca31'
 audit=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',a.executable,'port/engine-animation/reference/dynamic-compiled-transforms/original-corpus.bin'))
 assert audit['validation']=='PASS' and audit['samples']==kernel['samples'] and audit['bindings']==kernel['bindings'] and audit['ordered_targets']==kernel['targets'] and audit['libm']==kernel['libm'] and audit['retained']==kernel['retained'] and audit['atomic_rejections']==27 and audit['event_names']==37 and audit['input_player_independence'] and audit['sanitizer_findings']==0
 static=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',a.static_executable,'port/engine-animation/reference/compiled-transforms/original-corpus.bin'))
 assert static['validation']=='PASS' and static['original_raw_samples']==11656 and static['atomic_rejections']==8 and static['retained_event_names']==9 and static['sanitizer_findings']==0
 cache_hash=sha(a.cache);assert cache_hash=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679';cache_inputs=[]
 with zipfile.ZipFile(a.cache) as archive:
  for relative in ['models/prince_modular.bdae']+['animations/prince_'+suffix+'.bdae' for suffix in ('idle_shield','walk_1hand','1hand_combo_01','1hand_combo_01_moving','dying_01')]:
   asset=REPO/'port/android-native/app/src/main/assets'/relative;entries=[name for name in archive.namelist() if '/characters/prince/' in name and name.endswith('/'+asset.name)];assert len(entries)==1 and archive.read(entries[0])==asset.read_bytes()
   cache_inputs.append({'asset':relative,'entry':entries[0],'sha256':sha(asset)})
 sources=dict(kernel['source_sha256']);sources['port/engine-animation/tests/dynamic_compiled_transforms_host.py']=sha(Path(__file__));sources['port/engine-animation/tests/compiled_transforms.cpp']=sha(ROOT/'tests/compiled_transforms.cpp')
 report={'validation':'PASS','host_audit':audit,'static_compiler_regression':static,'static_reference_sha256':sha(static_gold),'sanitizers':['AddressSanitizer','UndefinedBehaviorSanitizer'],'sanitizer_findings':0,'executable':a.executable,'executable_sha256':run('sha256sum',a.executable).split()[0],'static_executable':a.static_executable,'static_executable_sha256':run('sha256sum',a.static_executable).split()[0],'source_sha256':sources,'compiler':run('g++','--version').splitlines()[0],'original_instruction_evidence':{'original_sha256':kernel['original_sha256'],'report':str(kernel_path.relative_to(REPO)).replace('\\','/'),'report_sha256':sha(kernel_path),'reference_sha256':sha(gold),'manifest_sha256':kernel['manifest_sha256']},'cache_sha256':cache_hash,'cache_inputs':cache_inputs,'libm_contract':'Test-only sinf/acosf/sqrtf wrappers verify original kind and input bits, then supply audited UCRT outputs. Direct source linkage, no DSO and no arbitrary output tolerance; production math unchanged.','scope':'Full node1/5/10 dynamic ordered union/pruning/default/bounds and raw target sampling. Caller registration schedule, cache mutation, unsupported component/material/compressed channels, whole blended scene and historical Bionic libm remain outside this proof.'}
 for path,value in sources.items():assert sha(REPO/path)==value,path
 a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS',**audit,'static_samples':static['original_raw_samples'],'executable_sha256':report['executable_sha256']}))
if __name__=='__main__':main()
