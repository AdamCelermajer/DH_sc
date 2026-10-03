"""Bind a prebuilt sanitized TransformSet replay to its source and original gold.

Runs host executables only. No Android build, deployment or prior report edits.
"""
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
    p=argparse.ArgumentParser();p.add_argument('--executable',default='/home/adampalace/dh2-world-build/engine-skinning/engine-animation/compiled_transforms_audit')
    p.add_argument('--legacy-executable',default='/home/adampalace/dh2-world-build/compiled-transforms-legacy-audit');p.add_argument('--cache',type=Path,default=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip'))
    p.add_argument('--output',type=Path,default=ROOT/'reports/compiled-transforms-host-audit.json');a=p.parse_args()
    kernel_path=ROOT/'reports/compiled-transforms-arm64-differential.json';kernel=json.loads(kernel_path.read_text());assert kernel['validation']=='PASS' and kernel['mismatches']==0
    gold=ROOT/'reference/compiled-transforms/original-corpus.bin';assert sha(gold)==kernel['reference_sha256']
    for path,expected in kernel['source_sha256'].items():assert sha(REPO/path)==expected,path
    cache_hash=sha(a.cache);assert cache_hash=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
    inputs=[]
    with zipfile.ZipFile(a.cache) as archive:
        for relative in ['models/prince_modular.bdae']+['animations/'+row['asset'] for row in kernel['inputs']]:
            asset=REPO/'port/android-native/app/src/main/assets'/relative;raw=asset.read_bytes()
            names=[n for n in archive.namelist() if '/characters/prince/' in n and n.endswith('/'+asset.name)]
            assert len(names)==1 and archive.read(names[0])==raw,relative
            inputs.append({'asset':relative,'cache_entry':names[0],'sha256':sha(asset),'bytes':len(raw)})
    audit=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',a.executable,'port/engine-animation/reference/compiled-transforms/original-corpus.bin'))
    assert audit['validation']=='PASS' and audit['original_raw_samples']==kernel['original_raw_samples'] and audit['atomic_rejections']==8 and audit['retained_event_names']==9 and audit['sanitizer_findings']==0
    legacy=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',a.legacy_executable,'.local-inputs/compiled-transform-discovery/legacy-candle.bdae'))
    assert legacy['milliseconds_sampled']==2033 and legacy['mutated_inputs']==5000 and legacy['move_ownership_checked'] and legacy['invalid_image_rejected']
    source_hashes=dict(kernel['source_sha256']);source_hashes['port/engine-animation/CMakeLists.txt']=sha(ROOT/'CMakeLists.txt');source_hashes['port/engine-animation/tests/compiled_transforms_host.py']=sha(Path(__file__))
    report={'validation':'PASS','host_audit':audit,'sanitizers':['AddressSanitizer','UndefinedBehaviorSanitizer'],'sanitizer_findings':0,'executable':a.executable,'executable_sha256':run('sha256sum',a.executable).split()[0],'source_sha256':source_hashes,'original_instruction_evidence':{'original_sha256':kernel['original_sha256'],'report':str(kernel_path.relative_to(REPO)).replace('\\','/'),'report_sha256':sha(kernel_path),'reference_sha256':sha(gold),'manifest_sha256':kernel['original_manifest_sha256']},'cache_sha256':cache_hash,'cache_inputs':inputs,'legacy_player_regression':legacy,'legacy_executable_sha256':run('sha256sum',a.legacy_executable).split()[0],'legacy_fixture_sha256':sha(REPO/'.local-inputs/compiled-transform-discovery/legacy-candle.bdae'),'libm_contract':'Test-only --wrap=sinf/acosf/sqrtf verifies every original call kind and input bits then supplies audited UCRT output. Direct source compilation; no DSO or arbitrary output tolerance. Production math source unchanged.','scope':'Compiled ordered raw node1/5/10 target values/defaults/cursors, independent events/resource backing and caller contracts. Legacy Player API replay preserved. No whole generic compiler, timeline/event producer, BlenderPlayback, GPU or full game parity claim.'}
    for path,expected in source_hashes.items():assert sha(REPO/path)==expected,path
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS',**audit,'legacy_milliseconds':legacy['milliseconds_sampled'],'legacy_mutated_inputs':legacy['mutated_inputs'],'executable_sha256':report['executable_sha256']}))
if __name__=='__main__':main()
