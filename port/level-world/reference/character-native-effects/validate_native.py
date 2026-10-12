"""Freeze bounded native effect producers and recovered methods, with explicit unresolved StateInfo registry ownership."""
import hashlib,json
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
paths=[ROOT/'reports'/name for name in ['character-native-effects-arm64-differential.json','character-native-effects-focus-arm64-differential.json','character-native-effects-host-audit.json','character-native-effects-focus-host-audit.json']];reports=[json.loads(path.read_text()) for path in paths];original=sha(REPO/'.local-inputs/libDungeonHunter2.so')
for report in reports:
 assert report['validation']=='PASS' and report['original_sha256']==original
 for name,digest in report['source_sha256'].items():assert sha(REPO/name)==digest,name
assert reports[0]['optimized_arm64_library_sha256']==reports[1]['optimized_arm64_library_sha256']==sha(REPO/'.local-inputs/character-native-effects/oracle.so')
assert reports[0]['gold_sha256']==reports[2]['original_gold_sha256']==sha(HERE/'effect-fixtures.bin')
assert reports[1]['gold_sha256']==reports[3]['original_gold_sha256']==sha(HERE/'focus-fixtures.bin')
assert reports[2]['arm64_report_sha256']==sha(paths[0]) and reports[3]['arm64_report_sha256']==sha(paths[1])
for name in ['effect-probe.json','body-probe.json','focus-probe.json','behavior-ownership.json']:
 probe=json.loads((HERE/name).read_text());assert probe['validation']=='PASS' and probe['original_sha256']==original
 source={'effect-probe.json':'probe.py','body-probe.json':'body_probe.py','focus-probe.json':'focus_probe.py','behavior-ownership.json':'behavior_probe.py'}[name];assert probe['probe_sha256']==sha(HERE/source)
 if 'manifest_sha256' in probe:assert probe['manifest_sha256']==sha(HERE/'original-functions.json')
assert reports[0]['comparisons']==2648 and reports[1]['comparisons']==1176 and reports[2]['sanitizer_findings']==reports[3]['sanitizer_findings']==0
sources=['character_native_effects.hpp','character_native_effects.cpp','tests/character_native_effects.cpp','tests/character_native_effects_focus.cpp','tests/character_native_effects_differential.py','tests/character_native_effects_focus_differential.py','tests/character_native_effects_host.py','tests/character_native_effects_focus_host.py','tools/build_character_native_effects_oracle.ps1'];result=dict(validation='PASS',scope=__doc__,original_sha256=original,module_source_sha256={str((ROOT/name).relative_to(REPO)):sha(ROOT/name) for name in sources},proof_sha256={str(path.relative_to(REPO)):sha(path) for path in paths+[HERE/'effect-probe.json',HERE/'body-probe.json',HERE/'focus-probe.json',HERE/'behavior-ownership.json',HERE/'effect-fixtures.bin',HERE/'focus-fixtures.bin',HERE/'NOTES.md',Path(__file__)]},optimized_ARM64_comparisons=sum(r['comparisons'] for r in reports[:2]),ordered_original_service_requests=sum(r['ordered_services'] for r in reports[:2]),sanitized_host_checks=sum(r['host_audit']['checks'] for r in reports[2:]),native_registered_state_owner=False,OnInit_registration_implemented=False,full_Crypt_monster_rene=False,packaged_APK=False)
(HERE/'native-source-freeze.json').write_text(json.dumps(result,indent=2)+'\n');(ROOT/'reports/character-native-effects-source-validation.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(dict(validation='PASS',module_source_sha256=result['module_source_sha256'],comparisons=result['optimized_ARM64_comparisons'])))
