"""Bind the bounded host-context source/kernel/real-VM proofs without packaging or changing production."""
import hashlib,json
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
original='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
assert sha(REPO/'.local-inputs/libDungeonHunter2.so')==original
arm_path=ROOT/'reports/character-host-context-arm64-differential.json';host_path=ROOT/'reports/character-host-context-host-audit.json';arm=json.loads(arm_path.read_text());host=json.loads(host_path.read_text());hosting=json.loads((HERE/'hosting-probe.json').read_text());assert arm['validation']==host['validation']==hosting['validation']=='PASS'
assert arm['original_sha256']==host['original_sha256']==hosting['original_sha256']==original
assert arm['comparisons']==1233 and arm['ordered_services']==4041 and arm['actual_decoded_row_tier_cases']==153 and arm['mismatches']==0 and arm['native_atomic_rejections']==5
assert host['host_audit']['checks']==31004 and host['host_audit']['original_cases']==1233 and host['host_audit']['genuine_VM_cases']==179 and host['host_audit']['decoded_VM_row_tiers']==153 and host['sanitizer_findings']==0 and host['main_world_library_executed']
assert hosting['cases']==128 and hosting['ordered_services']==424 and hosting['probe_sha256']==sha(HERE/'hosting_probe.py')
assert arm['gold_sha256']==host['original_gold_sha256']==sha(HERE/'host-context-fixtures.bin') and host['arm64_report_sha256']==sha(arm_path)
assert arm['optimized_arm64_library_sha256']==sha(REPO/'.local-inputs/character-host-context/oracle.so')
for report in [arm,host]:
 for path,digest in report['source_sha256'].items():assert sha(REPO/path)==digest,path
for path,digest in host['Level_assets_sha256'].items():assert sha(REPO/path)==digest,path
level=REPO/'port/game-data/reference/level-tables/original-loader-capture.json';assert host['Level_original_loader_capture_sha256']==arm['Level_original_capture_sha256']==sha(level)
assert arm['Level_original_gold_sha256']==sha(level.parent/'level-table-fixtures.bin')
paths=['character_host_context.hpp','character_host_context.cpp','tests/character_host_context.cpp','tests/character_host_context_host.py','tests/character_host_context_differential.py','tools/build_character_host_context_oracle.ps1','reference/character-host-context/NOTES.md','reference/character-host-context/hosting_probe.py','reference/character-host-context/validate_native.py'];sources={str((ROOT/p).relative_to(REPO)):sha(ROOT/p) for p in paths}
proofs={str(path.relative_to(REPO)):sha(path) for path in [arm_path,host_path,HERE/'hosting-probe.json',HERE/'original-probe.json',HERE/'host-context-fixtures.bin',level]}
manifest_paths=[HERE/'original-functions.json',HERE/'producers/original-functions.json',HERE/'player/original-functions.json',HERE/'difficulty/original-functions.json',HERE/'level/original-functions.json'];manifests={str(path.relative_to(REPO)):sha(path) for path in manifest_paths}
for path in manifest_paths:assert json.loads(path.read_text())['original_sha256']==original
report=dict(validation='PASS',original_sha256=original,source_sha256=sources,proof_sha256=proofs,original_manifests_sha256=manifests,original_O2_comparisons=1233,ordered_services=4041,original_hosting_gate_cases=128,host_checks=31004,genuine_VM_cases=179,actual_decoded_Level_rows=51,decoded_original_and_VM_tiers=153,sanitizer_findings=0,source_manager_Application_and_session_ownership_bound=False,whole_monster_Init=False,packaged_APK=False)
(HERE/'native-source-freeze.json').write_text(json.dumps(report,indent=2)+'\n');(ROOT/'reports/character-host-context-source-validation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ['validation','original_O2_comparisons','host_checks','genuine_VM_cases']}))
