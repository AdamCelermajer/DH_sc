from pathlib import Path
import subprocess,json
reports=Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc\port\level-loader\reports')
adb=[r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe','-P','5039','-s','emulator-5590']
def run(args):
 assert subprocess.check_output(adb+['emu','avd','name'],text=True,timeout=15).replace('\r','').splitlines()[0]=='DH2_Loader_API37'
 return subprocess.check_output(adb+args,timeout=30)
pid=run(['shell','pidof','local.dh2.loader']).decode().strip();assert pid
lines=run(['logcat','-d','--pid='+pid,'-t','600']).decode(errors='replace').splitlines();selected=[line for line in lines if 'DH2Loader' in line]
(reports/'visible-entities-v32-device.log').write_text('\n'.join(selected)+'\n');print('\n'.join(selected[-40:]))
png=run(['exec-out','screencap','-p']);assert png.startswith(b'\x89PNG');(reports/'visible-entities-v32-whole.png').write_bytes(png)
