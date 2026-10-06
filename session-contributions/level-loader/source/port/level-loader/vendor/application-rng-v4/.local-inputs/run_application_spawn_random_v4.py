from pathlib import Path
import os,subprocess,sys,json
root=Path(__file__).resolve().parents[1];env=dict(os.environ,TEMP=str(root/'.local-inputs'),TMP=str(root/'.local-inputs'))
rows=json.loads((root/'port/level-world/reports/android-native-owner-tests/canonical-room-zone-bounds-v3/receipt.json').read_text())['sources']
sources=[r['path'].replace('\\','/') for r in rows[1:] if not r['path'].endswith('canonical_room_zone_v3.cpp')]
for p in ['port/level-world/application_spawn_random_owner_v4.cpp','port/game-data/loot_tables_v2.cpp']:
 if p not in sources:sources.append(p)
raise SystemExit(subprocess.run([sys.executable,str(root/'.local-inputs/root_android_linked_owner_test.py'),'application-spawn-random-v4','port/level-world/tests/application_spawn_random_owner_v4.cpp',*sources],cwd=root,env=env).returncode)
