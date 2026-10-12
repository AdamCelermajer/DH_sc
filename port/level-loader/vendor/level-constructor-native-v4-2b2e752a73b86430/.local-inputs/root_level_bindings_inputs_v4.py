from pathlib import Path
import hashlib,json,subprocess
root=Path(__file__).resolve().parent.parent
adb=[r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe','-s','emulator-5554']
target='/data/local/tmp/dh2-level-c1-v4-original'
subprocess.run(adb+['shell','mkdir','-p',target+'/scripts',target+'/missing-save-files'],check=True,capture_output=True,timeout=40)
sources=[(root/'port/level-world/reference/character-game-design/real-cache-inputs.bin',target+'/design.bin')]
base=root/'.local-inputs/character-script-assets-v1/bootstrap-cache/data/scripts/level'
for name in ['combat_formulas.luac','death_scripts.luac']:sources.append((base/name,target+'/scripts/'+name))
for path,remote in sources:subprocess.run(adb+['push',str(path),remote],check=True,capture_output=True,timeout=40)
print(json.dumps({'status':'PASS','inputs':[{'source':str(p),'remote':r,'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p,r in sources],'save_directory':target+'/missing-save-files','app_private_storage_untouched':True}))
