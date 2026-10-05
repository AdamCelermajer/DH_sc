"""Test class UI controls in the isolated visible emulator; actors/game launch are pending."""
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
  file=a.output/(name+'.png');file.write_bytes(subprocess.check_output(cmd+['exec-out','screencap','-p'],timeout=30))
  with Image.open(file) as im:return im.size
 def tap(x,y):
  w,h=shot('point');cw=min(w,h*3//2);ch=min(h,w*2//3)
  adb('shell','input','tap',str(round((w-cw)/2+x*cw/480)),str(round((h-ch)/2+y*ch/320)))
 def wait(marker,count=0):
  end=time.monotonic()+30
  while time.monotonic()<end:
   s=logs()
   if 'Original menu input failed:' in s or 'Original UI display failed:' in s:raise RuntimeError(s[-5000:])
   if s.count(marker)>count:return s
   time.sleep(.2)
  raise RuntimeError('Missing fresh '+marker+'\n'+logs()[-3500:])
 marker='Original class state inspected | path  | member PlayerClass | found 1 | text '
 def inspect(expected):
  before=logs().count(marker)
  adb('shell','am','broadcast','-a','com.example.dh2.DEBUG_MENU_SOUND','-p','com.example.dh2','--es','probe','inspect-class')
  s=wait(marker,before);value=s[s.rfind(marker)+len(marker):].splitlines()[0]
  assert value==expected,(value,expected)
  return value
 push='Owned menu navigation | push menu_SelectClass'
 def open_class():
  before=logs().count(push);tap(410,94);time.sleep(.8);tap(40,219);time.sleep(.2);tap(240,126);wait(push,before);time.sleep(.8);inspect('KnightPlayerBase')
 def back(marker):
  before=logs().count(marker);tap(18,18);wait(marker,before);time.sleep(.4)
 report={'status':'FAIL','apk_sha256':hashlib.sha256(a.apk.read_bytes()).hexdigest(),'scope':'Name-to-class navigation, arrow selection and boundaries, original PlayerClass callback, Back and live resize. Class actors, swipe and game launch remain incomplete.','surfaces':[]}
 oldwm=adb('shell','wm','size');override=re.search(r'Override size: (\d+x\d+)',oldwm)
 try:
  adb('install','-r',str(a.apk));time.sleep(1)
  adb('shell','am','force-stop','com.example.dh2');adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity','--es','front_screen','main')
  wait('Original front/HUD screen submitted | screen main');time.sleep(.6)
  open_class()
  for size in ['1080x1920','1080x2400','1968x2184']:
   adb('shell','wm','size',size);time.sleep(1.5);inspect('KnightPlayerBase');shot('warrior-'+size)
   tap(18,190);time.sleep(.4);inspect('KnightPlayerBase')
   tap(463,190);time.sleep(.5);inspect('RoguePlayerBase');shot('rogue-'+size)
   tap(463,190);time.sleep(.5);inspect('MagePlayerBase');shot('mage-'+size)
   tap(463,190);time.sleep(.4);inspect('MagePlayerBase')
   tap(18,190);time.sleep(.5);inspect('RoguePlayerBase')
   tap(18,190);time.sleep(.5);inspect('KnightPlayerBase')
   back('Owned menu navigation | pop menu_SelectClass | current menu_EnterName');shot('back-name-'+size)
   before=logs().count(push);tap(40,219);time.sleep(.3);tap(240,126);wait(push,before);time.sleep(.8);inspect('KnightPlayerBase')
   report['surfaces'].append({'override':size,'class_order':['KnightPlayerBase','RoguePlayerBase','MagePlayerBase','RoguePlayerBase','KnightPlayerBase'],'boundaries':'PASS','back':'PASS'})
  tap(463,190);time.sleep(.4);inspect('RoguePlayerBase');pid=adb('shell','pidof','com.example.dh2').strip()
  adb('shell','wm','size','1080x2400');time.sleep(1);inspect('RoguePlayerBase');shot('live-resize-rogue')
  assert adb('shell','pidof','com.example.dh2').strip()==pid
  report['live_resize']={'pid':pid,'class_preserved':'RoguePlayerBase'};report['status']='PASS'
 finally:
  adb('shell','wm','size',override[1] if override else 'reset');time.sleep(.6)
  (a.output/'class-validation.log').write_text(logs(),encoding='utf-8');shot('restored-class')
  (a.output/'class-validation.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
 print(json.dumps(report))
if __name__=='__main__':main()
