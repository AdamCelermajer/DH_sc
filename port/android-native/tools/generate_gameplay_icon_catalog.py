"""Derive immutable bitmap crops from authored SWF icon timeline shapes."""
import json,hashlib,runpy
from pathlib import Path
root=Path(__file__).resolve().parents[3]
scope=runpy.run_path(str(root/'port/android-native/tools/inspect_gameplay_icon_shapes.py'))
rows=scope['rows'];selected={}
for sprite in (79,347,262,526):
 for row in rows:
  if row['sprite']!=sprite or not row.get('texture'):continue
  name=row['name'].lower()
  if name not in selected:selected[name]=row
cpp=root/'port/android-native/app/src/main/cpp/gameplay_icon_catalog.inc'
cpp.write_text('// Derived from original dqcharmenu_droid.swf bitmap fill matrices and frame labels.\n'+
 '\n'.join('{'+json.dumps(name)+','+json.dumps('data/3d/textures/'+Path(r['texture']).name.lower())+','+','.join(str(r[x]) for x in ('x','y','width','height'))+'},' for name,r in sorted(selected.items()))+'\n')
receipt=dict(source_sha256=hashlib.sha256(scope['raw']).hexdigest(),catalog_sha256=hashlib.sha256(cpp.read_bytes()).hexdigest(),rows=len(selected),
 source='original-cache/data/menus/dqcharmenu_droid.swf',shape_mapping='bounds transformed by actual bitmap fill matrix; no inferred grid',
 limits='Crops retain original atlas artwork. This is bitmap presentation, not full authored SWF vector/button rendering.')
(root/'port/android-native/reports/gameplay-icon-catalog-v1.json').write_text(json.dumps(receipt,indent=2)+'\n')
print(json.dumps(receipt))
