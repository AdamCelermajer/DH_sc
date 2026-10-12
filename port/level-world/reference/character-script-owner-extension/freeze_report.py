import hashlib,json
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
original=HERE/'external-owner-probe.json';arm=ROOT/'reports/character-script-owner-external-arm64-differential.json';host=ROOT/'reports/character-script-owner-external-host-audit.json'
e,a,h=[json.loads(x.read_text()) for x in [original,arm,host]];assert all(x['validation']=='PASS' for x in [e,a,h])
assert sha(original)==a['original_stage_order_evidence_sha256']==h['original_order_evidence_sha256']
assert all(sha(REPO/name)==digest for proof in [a,h] for name,digest in proof['source_sha256'].items())
report=dict(validation='PASS',scope='Owned AISExternal constructor/initial/VCB/load-stage composition; genuine follower initialization and explicit monster/rene provider-failure prefixes; loader stream/chunk-name boundary remains.',source_sha256=h['source_sha256'],original_sha256=e['original_sha256'],original_order_cases=len(e['cases']),arm64_helper_comparisons=a['comparisons'],host_extension_checks=h['host_audit']['checks'],legacy_host_checks=h['legacy_host_regression']['checks'],sanitizer_findings=h['sanitizer_findings'],actual_follower_callback_flags=h['host_audit']['actual_follower_callback_flags'],fake_gameplay_globals=False,full_AI_dispatch=False,proof_sha256={str(x.relative_to(REPO)):sha(x) for x in [original,arm,host]},notes_sha256=sha(HERE/'NOTES.md'),historical_gold_sha256=sha(ROOT/'reference/character-script-owner/owner-fixtures.bin'),exact_root_loader_bound=False)
output=ROOT/'reports/character-script-owner-external-source-validation.json';output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',original_cases=len(e['cases']),arm64=a['comparisons'],host=h['host_audit']['checks'],legacy=h['legacy_host_regression']['checks'])))
