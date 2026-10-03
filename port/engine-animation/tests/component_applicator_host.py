"""Replay the sanitized component/scene bridge against original-derived gold."""
import argparse,hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--executable',default='.local-inputs/component-applicator-discovery/host-audit');p.add_argument('--output',type=Path,default=ROOT/'reports/component-applicator-host-audit.json');a=p.parse_args()
 kernel_path=ROOT/'reports/component-applicator-arm64-differential.json';kernel=json.loads(kernel_path.read_text());assert kernel['validation']=='PASS' and kernel['mismatches']==0 and kernel['factory_type_checks']==10
 for name,value in kernel['source_sha256'].items():assert sha(REPO/name)==value,name
 gold=ROOT/'reference/component-applicator/original-corpus.bin';assert sha(gold)==kernel['reference_sha256']
 command=['wsl','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',a.executable,'port/engine-animation/reference/component-applicator/original-corpus.bin']
 result=subprocess.run(command,capture_output=True,text=True);assert result.returncode==0,(result.stdout,result.stderr);assert not result.stderr.strip(),result.stderr;audit=json.loads(result.stdout)
 assert audit['validation']=='PASS' and audit['cases']==kernel['cases'] and audit['atomic_rejections']==14 and audit['world_refresh_checks']==1 and audit['sanitizer_findings']==0
 for field in ('direct_applies','blended_applies','contributions'):assert audit[field]==kernel[field]
 assert audit['scene_bridge_applies']==kernel['direct_applies']+kernel['blended_applies']
 sources=dict(kernel['source_sha256'])
 for name in ('port/scene-materials/scene.cpp','port/engine-resources/resources.hpp','port/engine-resources/resources.cpp','port/asset-payloads/payloads.hpp','port/asset-payloads/payloads.cpp','port/engine-animation/tests/component_applicator_host.py'):
  sources[name]=sha(REPO/name)
 report={'validation':'PASS','host_audit':audit,'sanitizers':['AddressSanitizer','UndefinedBehaviorSanitizer'],'sanitizer_findings':0,'executable':a.executable,'executable_sha256':sha(REPO/a.executable),'compiler':'WSL g++','compiler_flags':['-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror','-fno-fast-math','-ffp-contract=off','-fno-builtin'],'source_sha256':sources,'original_instruction_evidence':{'original_sha256':kernel['original_sha256'],'kernel_report_sha256':sha(kernel_path),'reference_sha256':sha(gold),'manifest_sha256':kernel['manifest_sha256'],'factory_type_checks':10},'scope':'Original-derived concrete component typed/apply replay and borrowed C++ Scene::Node bridge, full XYZ ordered overwrite with caller-owned dirty flags and separately requested genuine scene::update_world. Non-component factory tags6..10 reject atomically. Arithmetic NaNs compare classification; copy operations preserve bytes. No raw compiler/default/accessor, quaternion/Euler, whole scene animation, gameplay/GPU or APK parity claim.'}
 a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(audit))
if __name__=='__main__':main()
