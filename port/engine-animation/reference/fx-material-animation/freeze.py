"""Bind this isolated source stage without touching frozen shared artifacts."""
import hashlib,json,os
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 arm=ROOT/'reports/material-color-arm64-differential.json';host=ROOT/'reports/material-color-host-audit.json';a=json.loads(arm.read_text());h=json.loads(host.read_text());assert a['validation']==h['validation']=='PASS'and a['comparisons']==h['original_gold_cases']==7899 and a['atomic_guards']==h['atomic_guards']==13 and h['sanitizer_findings']==0
 for report in [a,h]:
  for name,value in report['source_sha256'].items():assert sha(REPO/name)==value,name
 assert h['original_instruction_report_sha256']==sha(arm)
 assert a['reference_sha256']==h['reference_sha256']==sha(HERE/'material-color-fixtures.bin')
 assert a['original_manifest_sha256']==sha(HERE/'original-functions.json')
 assert json.loads((HERE/'probe.json').read_text())['original_manifest_sha256']==sha(HERE/'original-functions.json')
 inputs=[ROOT/'material_color.hpp',ROOT/'material_color.cpp',ROOT/'tests/material_color.cpp',ROOT/'tests/material_color_differential.py',ROOT/'tests/material_color_host.py',ROOT/'tools/build_material_color_oracle.ps1',HERE/'NOTES.md',HERE/'original-functions.json',HERE/'reference/original-functions.asm',HERE/'probe.py',HERE/'probe.json',HERE/'material-color-fixtures.bin',Path(__file__),arm,host]
 artifact=REPO/'.local-inputs/fx-material-animation-discovery';lib=artifact/'material_color64.so';hostlib=artifact/'host/libdh2_material_color_audit.so';exe=artifact/'host/material_color_audit';assert sha(lib)==a['arm64_library_sha256'];assert sha(hostlib)==h['library_sha256'];assert sha(exe)==h['executable_sha256']
 clang=Path(os.environ['LOCALAPPDATA'])/'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
 result={'validation':'PASS','stage':'isolated FX77 type86 uchar4 component3/math/material setter','original_sha256':a['original_sha256'],'source_sha256':{str(p.relative_to(REPO)).replace('\\','/'):sha(p)for p in inputs},'artifact_sha256':{str(p.relative_to(REPO)).replace('\\','/'):sha(p)for p in [lib,hostlib,exe]},'ndk_compiler':str(clang),'ndk_compiler_sha256':sha(clang),'comparisons':7899,'atomic_guards':13,'sanitizer_findings':0,'remaining_required_backends':['runtime material directory/shader factory/name conversion/ownership','source factory/typed material compile attachment and clip/timeline producer','particle28 emitter/scene/runtime services','GPU rendering and complete FX lifecycle'],'shared_sources_changed':False,'central_dependencies_used':False,'APK_changed':False}
 (HERE/'native-source-freeze.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items()if k not in ('source_sha256','artifact_sha256','remaining_required_backends')}))
if __name__=='__main__':main()
