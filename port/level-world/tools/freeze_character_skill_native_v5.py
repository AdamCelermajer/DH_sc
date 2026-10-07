"""Freeze new V5 sources and receipts; never modify earlier versions."""
from pathlib import Path
import hashlib,json
base=Path(__file__).resolve().parents[1]
paths=[
'character_skill_native_v5.hpp','character_skill_native_v5.cpp',
'character_skill_mana_v5.cpp','character_target_search_v5.hpp','character_target_search_v5.cpp',
'tests/character_skill_mana_v5_differential.py','tests/character_skill_native_v5_host.cpp',
'tests/character_target_search_v5_corpus.cpp','tests/character_target_search_v5_host.cpp',
'tools/extract_character_skill_native_v5.py','tools/prepare_character_target_search_v5.py',
'tools/run_character_skill_native_v5_host.py','tools/freeze_character_skill_native_v5.py',
'reference/character-skill-native-v5/NOTES.md',
'reference/character-skill-native-v5/original-functions.json',
'reference/character-skill-native-v5/original-functions.asm',
'reference/character-skill-native-v5/search-versioning-map-v5.json',
'reference/character-skill-native-v5/mana-gold-v5.json',
'reports/character-skill-mana-v5-arm64-differential.json',
'reports/character-skill-native-v5-host-audit.json']
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
for name in ['reports/character-skill-mana-v5-arm64-differential.json','reports/character-skill-native-v5-host-audit.json']:
    report=json.loads((base/name).read_text())
    assert report['validation']=='PASS',name
host=json.loads((base/'reports/character-skill-native-v5-host-audit.json').read_text())
for name,digest in host['source_sha256'].items():
    assert sha(base.parents[1]/name)==digest,name
v4=json.loads((base/'reference/character-skill-state-v4/freeze-manifest-v4.json').read_text())
for name,record in v4['files'].items():
    assert sha(base.parents[1]/name)==record['sha256'],name
manifest={'version':5,'scope':'private borrowed native bindings; full_campaign=false',
 'sha256':{p:sha(base/p) for p in paths},
 'preserved_v4_manifest_sha256':sha(base/'reference/character-skill-state-v4/freeze-manifest-v4.json')}
output=base/'reference/character-skill-native-v5/freeze-manifest-v5.json'
output.write_text(json.dumps(manifest,indent=2)+'\n')
print(json.dumps({'validation':'PASS','frozen_files':len(paths),'manifest':str(output)}))
