from pathlib import Path
import subprocess,sys,json,hashlib
r=Path(__file__).resolve().parent.parent
adb=r'C:/Users/adamc/AppData/Local/Android/Sdk/platform-tools/adb.exe'
remote='/data/local/tmp/dh2-chest-callback-v21'
subprocess.run([adb,'-s','emulator-5554','shell','mkdir','-p',remote],check=True)
assets=[]
for p in (r/'reference/openable-container-v1/cache').glob('go_chest*.bdae'):
 subprocess.run([adb,'-s','emulator-5554','push',str(p),remote+'/'+p.name],check=True)
 assets.append({'path':str(p.relative_to(r)),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()})
(r/'port/level-world/reference/destructible-container-v21/native-chest-assets.json').write_text(json.dumps(assets,indent=2))
sources=['tests/canonical_openable_graph_v21.cpp','canonical_openable_graph_v21.cpp','generic_animation_callbacks_v21.cpp','base_index_animation_controller_v21.cpp','retained_generic_animator_v21.cpp','retained_gameobject_visual_v1.cpp','container_animation_connection_v21.cpp']
raise SystemExit(subprocess.run([sys.executable,str(r/'.local-inputs/root_android_linked_owner_test.py'),'canonical-openable-callback-v21',*['port/level-world/'+p for p in sources],'--',remote],cwd=r).returncode)
