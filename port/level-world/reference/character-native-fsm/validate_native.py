"""Bind additive native FSM proofs to current source and original capture; no build or historical-report mutation."""
import hashlib,json
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
arm_path=ROOT/'reports/character-native-fsm-arm64-differential.json';host_path=ROOT/'reports/character-native-fsm-host-audit.json';arm=json.loads(arm_path.read_text());host=json.loads(host_path.read_text());original=json.loads((HERE/'native-fsm-probe.json').read_text())
assert arm['validation']==host['validation']==original['validation']=='PASS'
assert sha(REPO/'.local-inputs/libDungeonHunter2.so')==arm['original_sha256']==host['original_sha256']==original['original_sha256']
assert original['manifest_sha256']==sha(HERE/'original-functions.json') and original['probe_sha256']==sha(HERE/'probe.py') and original['factory_table_sha256']==sha(HERE/'state-factory-table.json')
assert arm['original_probe_sha256']==sha(HERE/'native-fsm-probe.json') and host['arm64_report_sha256']==sha(arm_path)
assert sha(HERE/'native-update-fixtures.bin')==arm['gold_sha256']==host['original_gold_sha256']
for report in [arm,host]:
 for name,digest in report['source_sha256'].items():assert sha(REPO/name)==digest,name
assert arm['comparisons']==1662 and arm['mismatches']==0 and host['sanitizer_findings']==0
paths=['character_native_fsm.hpp','character_native_fsm.cpp','tests/character_native_fsm.cpp','tests/character_native_fsm_differential.py','tests/character_native_fsm_host.py','tools/build_character_native_fsm_oracle.ps1']
result=dict(validation='PASS',scope=__doc__,original_sha256=arm['original_sha256'],module_source_sha256={str((ROOT/name).relative_to(REPO)):sha(ROOT/name) for name in paths},proof_sha256={str(path.relative_to(REPO)):sha(path) for path in [arm_path,host_path,HERE/'native-fsm-probe.json',HERE/'native-update-fixtures.bin',HERE/'IMPLEMENTATION.md',Path(__file__)]},optimized_arm64_comparisons=arm['comparisons'],ordered_original_requests=arm['ordered_services'],sanitized_host_checks=host['host_audit']['checks'],full_native_FSM=False,packaged_APK=False)
(ROOT/'reports/character-native-fsm-source-validation.json').write_text(json.dumps(result,indent=2)+'\n');(HERE/'native-source-freeze.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(dict(validation='PASS',module_source_sha256=result['module_source_sha256'])))
