"""Bind the isolated particle source/capture/probe/oracles without shared edits."""
import hashlib,json,os
from pathlib import Path
HERE=Path(__file__).resolve().parent;REPO=HERE.parents[3];ROOT=HERE.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 files=[ROOT/'particle_parameter.hpp',ROOT/'particle_parameter.cpp',ROOT/'tests/particle_parameter.cpp',ROOT/'tests/particle_parameter_differential.py',ROOT/'tests/particle_parameter_host.py',ROOT/'tools/build_particle_parameter_oracle.ps1',HERE/'original-functions.json',HERE/'reference/original-functions.asm',HERE/'probe.py',HERE/'probe.json',HERE/'NOTES.md',HERE/'particle-parameter-fixtures.bin',ROOT/'reports/particle-parameter-arm64-differential.json',ROOT/'reports/particle-parameter-host-audit.json',Path(__file__)]
 for report in ('particle-parameter-arm64-differential.json','particle-parameter-host-audit.json'):
  row=json.loads((ROOT/'reports'/report).read_text());assert row['validation']=='PASS'
  for path,digest in row['source_sha256'].items():assert sha(REPO/path)==digest,(path,digest)
 compiler=Path(os.environ['LOCALAPPDATA'])/'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
 binaries=[REPO/'.local-inputs/fx-particle-animation-discovery/particle_parameter64.so',REPO/'.local-inputs/fx-particle-animation-discovery/host/libdh2_particle_parameter_audit.so',REPO/'.local-inputs/fx-particle-animation-discovery/host/particle_parameter_audit']
 runtime=[ROOT/'tests/compiled_transforms_differential.py',ROOT/'tests/animation_blend_differential.py',REPO/'port/engine-resources/tests/cpu.py']
 material={str(p.relative_to(REPO)).replace('\\','/'):sha(p)for p in [ROOT/'material_color.hpp',ROOT/'material_color.cpp']}
 assert list(material.values())==['c8c0f1572a0706c448a28e0112ca50d4497562e7e21ced2da33c0c1b2da59083','240bab4957913bbd41082d7b8ab5d13d804e09c8bbaa0e98a7bd7c511e92ef51']
 report={'validation':'PASS','original_sha256':sha(REPO/'.local-inputs/libDungeonHunter2.so'),'source_and_evidence_sha256':{str(p.relative_to(REPO)).replace('\\','/'):sha(p)for p in files},'executed_binaries_sha256':{str(p.relative_to(REPO)).replace('\\','/'):sha(p)for p in binaries},'oracle_services_source_sha256':{str(p.relative_to(REPO)).replace('\\','/'):sha(p)for p in runtime},'compiler_sha256':sha(compiler),'frozen_material_color_unchanged':material,'shared_sources_modified':False,'android_artifacts_modified':False,'scope':'Isolated CFloatEx float parameter kernels and original binding/attachment/constructor probes; complete particle factory/emission/rendering are explicit future work.'}
 (HERE/'native-source-freeze.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','freeze_sha256':sha(HERE/'native-source-freeze.json'),'source_sha256':{k:v for k,v in report['source_and_evidence_sha256'].items()if k.endswith('particle_parameter.hpp')or k.endswith('particle_parameter.cpp')}}))
if __name__=='__main__':main()
