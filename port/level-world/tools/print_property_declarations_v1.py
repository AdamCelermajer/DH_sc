from pathlib import Path
import json
p=Path(__file__).resolve().parents[1]/'reference/canonical-object-factory-v1/property-declarations-original.json'
for c in json.loads(p.read_text())['classes']:
 print(c['kind'])
 for d in c['declarations']:print(d['name'],d['offset'],d.get('kind',d.get('descriptor_vtable')),d.get('default',d.get('descriptor_default_raw',d.get('default_raw'))))
