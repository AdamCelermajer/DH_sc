"""Real Attack button at the current touch-reached position. Records source
acquisition/animator and existing native melee HP application separately from
the failed precision-waypoint harness. No actor position or target is injected.
"""
import argparse,hashlib,json,re,subprocess,time,xml.etree.ElementTree as ET
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
ADB=Path(r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe')
APK=ROOT/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--output',type=Path,default=ROOT/'port/android-native/reports/source-attack-current-position-v1')
OUT=parser.parse_args().output;OUT.mkdir(parents=True,exist_ok=True)
def raw(*a):return subprocess.check_output([str(ADB),'-s','emulator-5554',*a],timeout=45)
def adb(*a):return raw(*a).decode('utf8','replace').strip()
pid=adb('shell','pidof','com.example.dh2')
def logs():return adb('logcat','-d','--pid='+pid,'-v','brief')
report={'validation':'FAIL','scope':__doc__,'pid':pid,'apk_sha256':hashlib.sha256(APK.read_bytes()).hexdigest(),'full_skill_damage_verified':False,'full_enemy_ai_verified':False,'physical_arm64_tested':False}
try:
 installed=adb('shell','pm','path','com.example.dh2').removeprefix('package:')
 report['installed_apk_sha256']=adb('shell','sha256sum',installed).split()[0]
 assert report['installed_apk_sha256']==report['apk_sha256']
 adb('shell','uiautomator','dump','/sdcard/dh2-source-attack.xml')
 xml=adb('shell','cat','/sdcard/dh2-source-attack.xml');OUT.joinpath('before.xml').write_text(xml)
 node=next(n for n in ET.fromstring(xml).iter('node') if n.get('content-desc')=='Attack nearby enemy')
 x0,y0,x1,y1=map(int,re.findall(r'\d+',node.get('bounds')))
 before=logs();old_commands=len(re.findall(r'Original player attack command \|',before))
 old_hits=len(re.findall(r'Prince combat hit \|',before));old_anim=len(re.findall(r'Source attack animator \|',before))
 adb('shell','input','tap',str((x0+x1)//2),str((y0+y1)//2))
 time.sleep(.3);OUT.joinpath('during.png').write_bytes(raw('exec-out','screencap','-p'))
 deadline=time.monotonic()+20
 while time.monotonic()<deadline:
  current=logs()
  if len(re.findall(r'Prince combat hit \|',current))>old_hits:break
  if re.search(r'Fatal signal|FATAL EXCEPTION|Native frame failed',current):break
  time.sleep(.2)
 OUT.joinpath('runtime.log').write_text(current)
 assert adb('shell','pidof','com.example.dh2')==pid
 assert not re.search(r'Fatal signal|FATAL EXCEPTION|Native frame failed',current)
 commands=re.findall(r'Original player attack command \| request (\d+) \| target (\d+) \| last (\d+) \| state (-?\d+)',current)[old_commands:]
 hits=re.findall(r'Prince combat hit \| target (\S+) .*?\| HP (-?\d+) (-?\d+) \| dead (\d+)',current)[old_hits:]
 animator=re.findall(r'Source attack animator \| begin (\d+) \| status (-?\d+) \| phase (\d+) \| index (-?\d+) \| continued (\d+) \| last (\d+) \| finisher (\d+) \| target (\S+)',current)[old_anim:]
 assert commands and commands[0][0]=='0' and int(commands[0][1])>0,commands
 assert hits and any(int(h[2])<int(h[1]) for h in hits),hits
 assert animator and all(a[1]=='1' for a in animator),animator
 time.sleep(2);current=logs();OUT.joinpath('runtime.log').write_text(current)
 OUT.joinpath('after.png').write_bytes(raw('exec-out','screencap','-p'))
 assert not re.search(r'Fatal signal|FATAL EXCEPTION|Native frame failed',current)
 report.update(validation='PASS',source_commands=commands,source_animation_steps=animator,native_melee_hits=hits,source_target_injected=False,existing_melee_application_verified=True)
finally:
 OUT.joinpath('source-attack-current-position-smoke.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps(report))
