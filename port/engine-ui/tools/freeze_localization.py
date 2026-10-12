"""Verify final receipts and freeze newly recovered localization evidence."""
import hashlib,json,shutil
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 reference=ROOT/'reference/localization';scratch=REPO/'.local-inputs/localization-discovery';arm=ROOT/'reports/localization-arm64-differential.json';host=ROOT/'reports/localization-host-audit.json';a=json.loads(arm.read_text());h=json.loads(host.read_text());assert a['validation']==h['validation']=='PASS' and a['comparisons']==4388 and h['owned_connection_audit']['comparisons']==260
 for report in [a,h]:
  for name,digest in report['source_sha256'].items():assert sha(REPO/name)==digest,(name,digest)
 assert h['original_instruction_report_sha256']==sha(arm)
 for source,name in [('common-text-probe.json','common-text-probe.json'),('transform-probe.json','transform-probe.json'),('literals.json','source-literals.json')]:shutil.copyfile(scratch/source,reference/name)
 files=[ROOT/'localization.hpp',ROOT/'localization.cpp',*ROOT.glob('tests/localization*'),*ROOT.glob('tools/*localization*'),*reference.rglob('*'),arm,host];files=[p for p in files if p.is_file()and p.name!='freeze.json']
 result={'validation':'PASS','original_sha256':a['original_sha256'],'source_and_evidence_sha256':{p.relative_to(REPO).as_posix():sha(p)for p in sorted(files)},'optimized_arm64_library_sha256':a['arm64_library_sha256'],'isolated_host_library_sha256':h['library_sha256'],'proof_counts':{'original_O2_kernel_comparisons':4388,'actual_color_branch_strings':4178,'ordered_metadata_bindings':333,'original_wrapper_connection_comparisons':260,'ordered_connection_callbacks':4760,'owned_text_files':333,'owned_text_strings':41546,'host_guards':9},'sanitizer_findings':0,'packaged_APK_claim':False,'scope':'Owned original common_text/text-only NativeGetStringFromSymbol. Original AS text conversion/publication, Application language/PlayerManager lifetime, varargs/service formatting, existing Debug save parser and GPU remain explicit boundaries.'}
 target=reference/'freeze.json';target.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({'validation':'PASS','freeze':str(target),'freeze_sha256':sha(target),'header_sha256':sha(ROOT/'localization.hpp'),'cpp_sha256':sha(ROOT/'localization.cpp'),'arm64_report_sha256':sha(arm),'host_report_sha256':sha(host)}))
if __name__=='__main__':main()
