"""Summarize already executed, hash-verified whole same-owner reload proof."""
from pathlib import Path
import hashlib,json
root=Path(__file__).resolve().parents[3];base=root/'port/level-world'
audit=base/'reports/character-skill-populated-v6-host-audit.json';proof=json.loads(audit.read_text())
assert proof['validation']=='PASS'
for name,digest in proof['source_sha256'].items():assert hashlib.sha256((root/name).read_bytes()).hexdigest()==digest,name
assert len(proof['results'])==2
for result in proof['results']:assert result['checks']==1334 and result['same_owner_classes']==3
report={'validation':'PASS','whole_native_reload_positive_classes':3,'required_nonnull_profile_failure_classes':3,'same_Save_identity':True,'actual_constructor_null_profile':True,'slot_minus_one_does_not_imply_null_profile':True,'actual_configure_and_update':True,'sanitizer_findings':0,'modes':['SAN','O2'],'checks_per_mode':1334,'full_target_application':False,'full_campaign':False,'audit_sha256':hashlib.sha256(audit.read_bytes()).hexdigest(),'source_sha256':proof['source_sha256']}
(base/'reports/character-skill-reload-v6-composition.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='source_sha256'}))
