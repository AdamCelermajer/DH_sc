from pathlib import Path
import hashlib,json
ROOT=Path(__file__).resolve().parents[3]
paths=['port/engine-ui/authored_character_panel_v2.hpp','port/engine-ui/authored_character_panel_v2.cpp','port/android-native/app/src/main/cpp/character_panel_session_v1.hpp','port/android-native/app/src/main/cpp/character_panel_session_v1.cpp','port/android-native/app/src/main/cpp/character_panel_authored_v2.cpp','port/engine-ui/tests/authored_character_panel_v2.cpp','port/engine-ui/tools/run_authored_character_panel_v2.py','port/engine-ui/tools/build_authored_character_panel_v2.sh','port/engine-ui/reference/authored-character-panel-v2/create-original.json','port/engine-ui/reference/authored-character-panel-v2/integration.md','port/engine-ui/reports/authored-character-panel-v2/input-manifest.json','port/level-world/reports/android-native-owner-tests/authored-character-panel-v2/receipt.json']
files=[{'path':p,'sha256':hashlib.sha256((ROOT/p).read_bytes()).hexdigest()} for p in paths]
receipt=json.loads((ROOT/paths[-1]).read_text())
assert receipt['status']=='PASS'
for source in receipt['sources']:
 assert hashlib.sha256((ROOT/source['path']).read_bytes()).hexdigest()==source['sha256'],source['path']
out=ROOT/'port/engine-ui/reference/authored-character-panel-v2/freeze-manifest-v2.json'
out.write_text(json.dumps({'version':2,'scope':'Sole session authored movie/MenuStack/source shape release; APK-linked real cache movie proof. External profile/application/GPU fixture scope preserved; live Android menu acceptance pending.','apk_sha256':receipt['apk_sha256'],'files':files},indent=2)+'\n')
print(str(out));print(hashlib.sha256(out.read_bytes()).hexdigest())
