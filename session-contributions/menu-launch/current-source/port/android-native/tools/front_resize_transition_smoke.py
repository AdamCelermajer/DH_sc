"""Reproduce rapid Back/resize and require visible pixels plus retained activity/context."""
from pathlib import Path
import subprocess,time,json,hashlib,argparse
from PIL import Image
parser=argparse.ArgumentParser();parser.add_argument('--apk',type=Path,required=True);parser.add_argument('--output',type=Path,required=True)
args=parser.parse_args();D=args.output;D.mkdir(parents=True,exist_ok=True)
cmd=[r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe','-P','5038','-s','emulator-5580']
def adb(*args):return subprocess.check_output(cmd+list(args),text=True,timeout=45)
def shot(name):
 p=D/(name+'.png');p.write_bytes(subprocess.check_output(cmd+['exec-out','screencap','-p'],timeout=30))
 with Image.open(p) as im:return im.size
def tap(x,y):
 w,h=shot('point');cw=min(w,h*3//2);ch=min(h,w*2//3)
 adb('shell','input','tap',str(round((w-cw)/2+x*cw/480)),str(round((h-ch)/2+y*ch/320)))
def probe(name):
 pid=adb('shell','pidof','com.example.dh2').strip();s=adb('logcat','-d','--pid='+pid,'-v','brief');before=s.count('Front state inspected')
 adb('shell','am','broadcast','-a','com.example.dh2.DEBUG_MENU_SOUND','-p','com.example.dh2','--es','probe','inspect-front')
 end=time.monotonic()+20
 while time.monotonic()<end:
  s=adb('logcat','-d','--pid='+pid,'-v','brief')
  if s.count('Front state inspected')>before:break
  time.sleep(.2)
 else:raise RuntimeError('Missing fresh front inspection')
 (D/(name+'.log')).write_text(s,encoding='utf-8')
 tail=s[s.rfind('Front state inspected'):]
 print(name,'\n'+'\n'.join(x for x in tail.splitlines() if 'Front state inspected' in x or ('path menu_MainMenu | member' in x and ('_visible' in x or '_currentframe' in x))))
apk=args.apk
adb('install','-r',str(apk));adb('shell','wm','size','reset');adb('shell','am','force-stop','com.example.dh2')
adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity','--es','front_screen','main');time.sleep(6)
shot('main-before');probe('main-before');tap(410,94);time.sleep(1);tap(40,219);time.sleep(.2);shot('name-before');probe('name-before')
assert 'Owned menu navigation | push menu_EnterName' in (D/'name-before.log').read_text()
tap(18,18);time.sleep(.1);adb('shell','wm','size','1968x2184');time.sleep(2)
shot('rapid-resize');probe('rapid-resize');time.sleep(3);shot('settled-resize');probe('settled-resize')
adb('shell','wm','size','reset');time.sleep(2);shot('restored');probe('restored')
counts={}
for name in ['main-before','rapid-resize','settled-resize','restored']:
 with Image.open(D/(name+'.png')) as im:
  w,h=im.size;cw=min(w,h*3//2);ch=min(h,w*2//3);left=(w-cw)//2;top=(h-ch)//2
  crop=im.crop((left+cw//10,top+ch//10,left+cw*9//10,top+ch*9//10)).resize((64,64))
  counts[name]=len(set(crop.get_flattened_data()))
s=(D/'restored.log').read_text();retained='Front surface configuration retained' in s
r=dict(status='PASS' if min(counts.values())>500 and retained else 'FAIL',apk_sha256=hashlib.sha256(apk.read_bytes()).hexdigest(),distinct_center_colors=counts,configuration_retained=retained,gl_initializations=s.count('Authored UI shader GPU contract PASS'))
(D/'receipt.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(r));assert r['status']=='PASS'
