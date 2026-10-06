from pathlib import Path
import os,subprocess,sys,json,hashlib
root=Path(__file__).resolve().parents[1];env=dict(os.environ,TEMP=str(root/'.local-inputs'),TMP=str(root/'.local-inputs'))
adb=r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe';remote='/data/local/tmp/dh2-character-family-v4';subprocess.run([adb,'-s','emulator-5554','shell','mkdir','-p',remote],check=True)
names=[p+s for p in ['character_properties','character_classes','ai','ai_factions','levels','loot_table'] for s in ['_pyarray.bin','_pyarraynames.bin','_pystructnames.bin']]+['character_models_dictionary_pyarraynames.bin','character_models_dictionary_pyarray.bin']
manifest=[]
for name in names:
 p=root/'port/android-native/app/src/main/assets/data'/name;subprocess.run([adb,'-s','emulator-5554','push',str(p),remote+'/'+name],capture_output=True,check=True);manifest.append({'path':str(p.relative_to(root)),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()})
out=root/'port/level-world/reference/canonical-character-family-v4';(out/'cache-fixture-manifest.json').write_text(json.dumps(manifest,indent=2))
sources=['port/level-world/tests/canonical_character_family_v4.cpp','port/level-world/canonical_character_family_v4.cpp']
raise SystemExit(subprocess.run([sys.executable,str(root/'.local-inputs/root_android_linked_owner_test.py'),'canonical-character-family-v4',*sources,'--',remote],cwd=root,env=env).returncode)
