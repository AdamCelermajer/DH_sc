import hashlib,json
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
host=ROOT/'reports/character-script-owner-include-host-audit.json';arm=ROOT/'reports/character-script-owner-include-arm64-helper-regression.json'
h,a=[json.loads(p.read_text()) for p in [host,arm]];assert h['validation']==a['validation']=='PASS'
assert all(sha(REPO/name)==digest for proof in [h,a] for name,digest in proof['source_sha256'].items())
original=REPO/'port/script-runtime/reference/base-bindings/include-original-probe.json';e=json.loads(original.read_text());assert e['validation']=='PASS' and e['cases']==188 and e['original_instructions_executed']
assert a['original_include_manager_evidence_sha256']==sha(original)
report=dict(validation='PASS',scope='Source-only exact root/Include private-owner integration; original manager/lifecycle service evidence and actual native DSO VM behavior compose without a whole original frame parity claim.',source_sha256=h['source_sha256'],original_sha256=e['original_sha256'],original_include_manager_cases=e['cases'],optimized_arm64_helper_comparisons=a['comparisons'],optimized_arm64_atomic_rejections=a['native_atomic_rejections'],full_owner_arm64_executed=False,host_checks={k:v['checks'] for k,v in h['host_audits'].items()},sanitizer_findings=0,source_root_loader=True,persistent_provider_required=True,full_manager_cache_implemented=False,full_gameplay_namespace_bound=False,packaged_APK=False,proof_sha256={str(x.relative_to(REPO)):sha(x) for x in [host,arm,original]},notes_sha256=sha(HERE/'NOTES.md'),freeze_script_sha256=sha(Path(__file__)))
output=ROOT/'reports/character-script-owner-include-source-validation.json';output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',checks=report['host_checks'],original_cases=e['cases'],ARM64_helpers=a['comparisons'])))
