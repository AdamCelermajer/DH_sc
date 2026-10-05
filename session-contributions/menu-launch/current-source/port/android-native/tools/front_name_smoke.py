"""Verify actual authored name input on the isolated visible emulator.

The receipt covers the absent-save branch, typing, its eight-character limit,
Back, reopening and retained-process resize. Artwork and class selection remain
explicitly outside this receipt. No campaign save is written or removed.
"""
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
  end=time.monotonic()+50
  while time.monotonic()<end:
   s=logs()
   if 'Original menu input failed:' in s or 'Original UI display failed:' in s:raise RuntimeError(s[-6000:])
   if s.count(marker)>count:return
   time.sleep(.25)
  raise RuntimeError('Missing fresh '+marker)
 marker='Original name state inspected | path menu_EnterName.buttons.btn_character_name.text | member text | found 1 | text '
 def inspect(expected):
  before=logs().count(marker)
  adb('shell','am','broadcast','-a','com.example.dh2.DEBUG_MENU_SOUND','-p','com.example.dh2','--es','probe','inspect-name')
  wait(marker,before);s=logs();value=s[s.rfind(marker)+len(marker):].splitlines()[0]
  assert value==expected,(value,expected)
  return value
 push='Owned menu navigation | push menu_EnterName';pop='Owned menu navigation | pop menu_EnterName | current menu_MainMenu'
 def open_name():
  before=logs().count(push);tap(410,94);wait(push,before);time.sleep(.7);inspect('')
 def back():
  before=logs().count(pop);tap(18,18);wait(pop,before);time.sleep(.3)
 report={'status':'FAIL','apk_sha256':hashlib.sha256(a.apk.read_bytes()).hexdigest(),'scope':'Absent-save name input and Back; artwork and class selection incomplete','surfaces':[]}
 oldwm=adb('shell','wm','size');override=re.search(r'Override size: (\d+x\d+)',oldwm)
 try:
  adb('install','-r',str(a.apk));time.sleep(1)
  adb('shell','am','force-stop','com.example.dh2');adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity','--es','front_screen','main')
  wait('Original front/HUD screen submitted | screen main');time.sleep(.7)
  for size in ['1080x1920','1080x2400','1968x2184']:
   adb('shell','wm','size',size);time.sleep(1.2);shot('main-'+size);open_name()
   for x,y in [(40,219),(114,219),(40,219),(280,260)]:tap(x,y);time.sleep(.12)
   inspect('adam');shot('name-adam-'+size)
   for _ in range(5):tap(40,219);time.sleep(.12)
   inspect('adamaaaa');shot('name-limit-'+size)
   back();shot('back-main-'+size);open_name();shot('name-reopened-'+size);back()
   report['surfaces'].append({'override':size,'typed':'adam','limit_after_ninth_press':'adamaaaa','reopened_name':'','back':'PASS'})
  open_name();tap(40,219);inspect('a');pid=adb('shell','pidof','com.example.dh2').strip()
  adb('shell','wm','size','1080x2400');time.sleep(1.2);inspect('a');shot('live-resize-name')
  assert adb('shell','pidof','com.example.dh2').strip()==pid
  report['live_resize']={'pid':pid,'name_preserved':'a'};back();report['status']='PASS'
 finally:
  adb('shell','wm','size',override[1] if override else 'reset');time.sleep(.7)
  (a.output/'name-validation.log').write_text(logs(),encoding='utf-8');shot('restored-main')
  (a.output/'name-validation.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
 print(json.dumps(report))
if __name__=='__main__':main()
