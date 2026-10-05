"""Exercise repaired keyboard textures and real key actions on visible emulator5580."""
from pathlib import Path
import argparse,subprocess,time,json,hashlib,re
from PIL import Image

def main():
 p=argparse.ArgumentParser();p.add_argument('--apk',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
 a=p.parse_args();a.output.mkdir(parents=True,exist_ok=True)
 cmd=[r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe','-P','5038','-s','emulator-5580']
 def adb(*args):return subprocess.check_output(cmd+list(args),encoding='utf-8',timeout=45)
 def logs():return adb('logcat','-d','--pid='+adb('shell','pidof','com.example.dh2').strip(),'-v','brief')
 def shot(name):
  f=a.output/(name+'.png');f.write_bytes(subprocess.check_output(cmd+['exec-out','screencap','-p'],timeout=30))
  with Image.open(f) as im:return im.size
 def point(x,y):
  w,h=shot('point');cw=min(w,h*3//2);ch=min(h,w*2//3)
  return str(round((w-cw)/2+x*cw/480)),str(round((h-ch)/2+y*ch/320))
 def tap(x,y):adb('shell','input','tap',*point(x,y));time.sleep(.2)
 marker='Original name state inspected | path menu_EnterName.buttons.btn_character_name.text | member text | found 1 | text '
 def inspect(expected):
  before=logs().count(marker);adb('shell','am','broadcast','-a','com.example.dh2.DEBUG_MENU_SOUND','-p','com.example.dh2','--es','probe','inspect-name')
  end=time.monotonic()+20
  while time.monotonic()<end:
   s=logs()
   if s.count(marker)>before:
    value=s[s.rfind(marker)+len(marker):].splitlines()[0];assert value==expected,(value,expected);return
   time.sleep(.2)
  raise RuntimeError('Missing name inspection')
 report=dict(status='FAIL',apk_sha256=hashlib.sha256(a.apk.read_bytes()).hexdigest(),scope='Compatibility artwork, typing, Shift, Space, Delete, digits, eight-character limit, Back and retained resize',surfaces=[])
 oldwm=adb('shell','wm','size');override=re.search(r'Override size: (\d+x\d+)',oldwm)
 try:
  adb('install','-r',str(a.apk));time.sleep(1)
  adb('shell','am','force-stop','com.example.dh2');adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity','--es','front_screen','main');time.sleep(6)
  for size in ['1080x1920','1080x2400','1968x2184']:
   adb('shell','wm','size',size);time.sleep(1.5);tap(410,94);time.sleep(.8);inspect('')
   tap(40,219);inspect('a');tap(66,299);tap(20,180);inspect('aQ')
   for x in [115,215,330]:
    tap(x,299);inspect('aQ ');tap(320,260);inspect('aQ')
   tap(391,180);inspect('aQ7')
   tap(320,260);inspect('aQ');tap(66,299);tap(40,219);inspect('aQa')
   for _ in range(6):tap(40,219)
   inspect('aQaaaaaa');shot('keyboard-'+size)
   # Capture the held state on the final surface after regular navigation checks.
   if size=='1968x2184':
    xy=point(40,219);adb('shell','input','motionevent','DOWN',*xy);time.sleep(.25);shot('pressed-key-'+size);adb('shell','input','motionevent','UP',*xy)
    time.sleep(.2);shot('space-normal-'+size)
    xy=point(215,299);adb('shell','input','motionevent','DOWN',*xy);time.sleep(.25);shot('space-pressed-'+size);adb('shell','input','motionevent','UP',*xy)
   inspect('aQaaaaaa');before=logs().count('Owned menu navigation | pop menu_EnterName')
   tap(18,18);time.sleep(1.5)
   assert logs().count('Owned menu navigation | pop menu_EnterName')>before
   report['surfaces'].append(dict(size=size,shift='PASS',space='PASS',delete='PASS',digit='PASS',limit='aQaaaaaa',back='PASS'))
  tap(410,94);time.sleep(.8);tap(40,219);inspect('a');pid=adb('shell','pidof','com.example.dh2').strip()
  adb('shell','wm','size','1080x2400');time.sleep(1.5);inspect('a');assert adb('shell','pidof','com.example.dh2').strip()==pid
  report['retained_resize']=dict(pid=pid,text='a');report['status']='PASS'
 finally:
  adb('shell','wm','size',override[1] if override else 'reset');time.sleep(1.5)
  shot('visible-keyboard');s=logs();(a.output/'keyboard-validation.log').write_text(s,encoding='utf-8')
  assert 'Keyboard atlas compatibility movie connected | seven repaired shapes' in s
  assert 'Original UI display failed:' not in s and 'Original menu input failed:' not in s
  (a.output/'keyboard-validation.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps(report))
if __name__=='__main__':main()
