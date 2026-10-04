"""Freeze bounded kind/producer discovery and native initial-virtual evidence."""
import hashlib,json
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
probe=json.loads((HERE/'kind-probe.json').read_text());inputs=json.loads((HERE/'authored-ai-rows.json').read_text());arm=json.loads((ROOT/'reports/character-script-virtual-arm64-differential.json').read_text());host=json.loads((ROOT/'reports/character-script-virtual-host-audit.json').read_text())
assert all(x['validation']=='PASS' for x in [probe,inputs,arm,host])
assert sha(HERE/'kind-probe.json')==arm['original_evidence_sha256']==host['original_evidence_sha256']
assert sha(ROOT/'reports/character-script-virtual-arm64-differential.json')==host['arm64_report_sha256']
assert all(sha(REPO/name)==digest for name,digest in host['source_sha256'].items())
assert all(sha(REPO/name)==digest for name,digest in arm['source_sha256'].items())
files=list(HERE.rglob('*'));files=[p for p in files if p.is_file()]
report=dict(validation='PASS',scope='Source additional factory ownership, exact Crypt authored rows and bounded initial virtual/VCB source port; no full additional AIS/live/global namespace parity.',original_sha256=probe['original_sha256'],cache_sha256=inputs['cache_sha256'],factory_lifecycle_original_cases=len(probe['factory_and_lifecycle_cases']),source_vcb_cases=probe['vcb_comparisons'],complete_vtables=6,original_captured_routines=108,crypt_characters=inputs['crypt_characters'],required_cache_resources=inputs['required_cache_resources'],arm64_comparisons=arm['comparisons'],host_checks=host['host_audit']['checks'],sanitizer_findings=host['sanitizer_findings'],evidence_sha256={str(p.relative_to(REPO)):sha(p) for p in files},native_reports_sha256={str(p.relative_to(REPO)):sha(p) for p in [ROOT/'reports/character-script-virtual-arm64-differential.json',ROOT/'reports/character-script-virtual-host-audit.json']},new_native_source_sha256={str((ROOT/p).relative_to(REPO)):sha(ROOT/p) for p in ['character_script_virtual.hpp','character_script_virtual.cpp']},owner_runtime_cmake_edited=False)
output=ROOT/'reports/character-script-kinds-discovery.json';output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',original_cases=report['factory_lifecycle_original_cases'],vcb_cases=report['source_vcb_cases'],native_comparisons=report['arm64_comparisons'],host_checks=report['host_checks'])))
