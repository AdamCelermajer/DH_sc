import json,re
from pathlib import Path
root=Path(__file__).resolve().parents[3];base=root/'port/engine-ui'
manifest=json.loads((base/'reference/character-menu-native-v1/original-functions.json').read_text())
authored={f['name'] for f in manifest['functions'] if f['name'].startswith('Native')}
recognized=set(re.findall(r'equal\("([^"]+)"\)',(base/'character_menu_queries_owner_v1.cpp').read_text()))
missing=sorted(authored-recognized);assert not missing,missing
report=dict(validation='PASS',recovered_callback_names=len(authored),callbacks=sorted(authored),qualification='Name recognition only; genuine required provider and whole callback value/action proofs remain explicit candidates. NativeReloadSkills/navigation outside this recovered23 subset belongs to root coordinator.')
(base/'reports/character-menu-callback-name-coverage-v1.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report))
