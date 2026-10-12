from pathlib import Path
import os,subprocess,sys,json
root=Path(__file__).resolve().parents[1]
env=dict(os.environ,TEMP=str(root/'.local-inputs'),TMP=str(root/'.local-inputs'))
runner=root/'.local-inputs/root_android_linked_owner_test.py'
for name,test in [('room-zone-byte87-safety-v4','room_zone_byte87_safety_v4.cpp'),('room-zone-nine-module-v4','canonical_module_graph_v3.cpp')]:
 receipt='canonical-module-graph-v3' if 'nine' in name else 'canonical-room-zone-bounds-v3'
 rows=json.loads((root/('port/level-world/reports/android-native-owner-tests/'+receipt+'/receipt.json')).read_text())['sources']
 sources=[row['path'].replace('\\','/') for row in rows[1:]]
 r=subprocess.run([sys.executable,str(runner),name,'port/level-world/tests/'+test,*sources],cwd=root,env=env)
 if r.returncode:raise SystemExit(r.returncode)
