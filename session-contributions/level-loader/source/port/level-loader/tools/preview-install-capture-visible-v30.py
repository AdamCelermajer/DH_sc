from pathlib import Path
import subprocess,time,json,hashlib
root=Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc');reports=root/'port/level-loader/reports'
adb=[r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe','-P','5039','-s','emulator-5590']
def run(args,timeout=30):
 assert subprocess.check_output(adb+['emu','avd','name'],text=True,timeout=15).replace('\r','').splitlines()[0]=='DH2_Loader_API37'
 return subprocess.check_output(adb+args,timeout=timeout)
apk=root/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'
print(run(['install','-r',str(apk)],240).decode(),flush=True)
run(['shell','am','force-stop','local.dh2.loader'])
print(run(['shell','am','start','-n','local.dh2.loader/com.example.dh2.LoaderPreviewActivity','--es','level','SWAMP','--es','definition','001_swamp.mlx']).decode(),flush=True)
time.sleep(5)
png=run(['exec-out','screencap','-p']);assert png.startswith(b'\x89PNG');(reports/'visible-entities-v30-whole.png').write_bytes(png)
pid=run(['shell','pidof','local.dh2.loader']).decode().strip();assert pid
log=run(['logcat','-d','--pid='+pid,'-t','400']).decode(errors='replace')
selected=[line for line in log.splitlines() if 'DH2Loader' in line]
(reports/'visible-entities-v30-device.log').write_text('\n'.join(selected)+'\n')
result={'apk_sha256':hashlib.sha256(apk.read_bytes()).hexdigest(),'screenshot':str(reports/'visible-entities-v30-whole.png'),'loader_logs':selected[-45:],'full_loader_verified':False};(reports/'visible-entities-v30-device.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
