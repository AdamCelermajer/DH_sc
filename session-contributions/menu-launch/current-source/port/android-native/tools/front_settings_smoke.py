"""Authored Options controls, real file persistence, volume delivery and resize.

Uses only private ADB 5038/emulator5580. Restores the app's prior settings bytes.
Does not certify gameplay effects of HUD/control selectors or all languages.
"""
import argparse,hashlib,json,re,subprocess,time,struct
from pathlib import Path
from PIL import Image

def main():
 p=argparse.ArgumentParser();p.add_argument('--apk',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
 a=p.parse_args();a.output.mkdir(parents=True,exist_ok=True)
 cmd=[r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe','-P','5038','-s','emulator-5580']
 def adb(*args):return subprocess.check_output(cmd+list(args),text=True,encoding='utf-8',timeout=45)
 def binary(*args):return subprocess.check_output(cmd+list(args),timeout=45)
 def logs():return adb('logcat','-d','--pid='+adb('shell','pidof','com.example.dh2').strip(),'-v','brief')
 def wait(marker,count=1):
  end=time.monotonic()+60
  while time.monotonic()<end:
   text=logs()
   for error in ['Original menu input failed:','Original UI display failed:','FATAL EXCEPTION','Menu effect failed','Title music error']:
    assert error not in text,text[-6000:]
   if text.count(marker)>=count:return text
   time.sleep(.2)
  raise AssertionError('Missing '+marker+'\n'+logs()[-6000:])
 def launch():
  adb('shell','am','force-stop','com.example.dh2');adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity','--es','front_screen','main')
  wait('Original front/HUD screen submitted | screen main');time.sleep(1)
 surface=[2400,1080]
 def shot(name):
  f=a.output/(name+'.png');f.write_bytes(binary('exec-out','screencap','-p'))
  with Image.open(f) as im:surface[:]=im.size
 def point(x,y):
  w,h=surface;cw=min(w,h*3//2);ch=min(h,w*2//3)
  return str(round((w-cw)/2+x*cw/480)),str(round((h-ch)/2+y*ch/320))
 def action(x,y,marker):
  count=logs().count(marker);adb('shell','input','tap',*point(x,y));wait(marker,count+1);time.sleep(.3)
 def drag(y,current,target,key):
  before=logs().count('Front option changed | name '+key)
  adb('shell','input','swipe',*point(179+current*2.32,y),*point(179+target*2.32,y),'1200')
  wait('Front option changed | name '+key,before+1);time.sleep(.4)
  vals=re.findall('Front option changed \\| name '+key+' \\| value (-?\\d+)',logs());value=int(vals[-1]);assert abs(value-target)<=6,(key,value,target)
  return value
 def settings_bytes():return binary('exec-out','run-as','com.example.dh2','cat','files/dh2_settings.savegame')
 def decode(b):
  n=struct.unpack_from('<I',b)[0];at=4;values={}
  for _ in range(n):
   length=struct.unpack_from('<I',b,at)[0];at+=4;name=b[at:at+length].decode();at+=length
   values[name]=struct.unpack_from('<i',b,at)[0];at+=4
  assert len(b)-at==14;assert list(values)==sorted(values);return values
 previous_size=adb('shell','wm','size');override=re.search(r'Override size: (\d+x\d+)',previous_size)
 exists=adb('shell','run-as','com.example.dh2','ls','files').splitlines()
 backup=settings_bytes() if 'dh2_settings.savegame' in exists else None
 if backup is not None:(a.output/'prior-settings.bin').write_bytes(backup)
 report={'apk_sha256':hashlib.sha256(a.apk.read_bytes()).hexdigest(),'complete_menu':False,'scope':'Options file/controls, English and cached pack 1 (Ukrainian) labels, audio volume delivery, resize/lifecycle; gameplay selector effects and other languages pending','scenarios':[]}
 try:
  assert 'Success' in adb('install','-r',str(a.apk));time.sleep(1)
  for size in ['1080x1920','1080x2400','1968x2184']:
   # Test defaults without inventing a settings file; restore prior bytes below.
   adb('shell','run-as','com.example.dh2','rm','-f','files/dh2_settings.savegame')
   adb('shell','wm','size',size);launch();shot('main-'+size)
   action(410,160,'Owned menu navigation | push menu_Options | depth 2');shot('options-default-'+size)
   fx=drag(92,100,65,'VolumeFX');music=drag(128,100,40,'VolumeMusic');shot('volumes-'+size)
   wait('Menu audio volume applied | music='+str(music)+' | fx='+str(fx))
   for x,y,key,value in [(185,160,'DPad',0),(460,193,'HUDStyle',1),(460,230,'AutoTransmute',1),(185,293,'AutoOrientation',0)]:
    action(x,y,'Front option changed | name '+key+' | value '+str(value))
   localized='symbol MENU_OPTIONS | found 1 | text Налаштування'
   localized_before=logs().count(localized)
   action(460,263,'Front option changed | name Language | value 1');shot('options-pack1-ukrainian-'+size)
   wait(localized,localized_before+1)
   values=decode(settings_bytes());assert values['Language']==1
   action(185,263,'Front option changed | name Language | value 0');shot('options-english-'+size)
   action(18,18,'Owned menu navigation | pop menu_Options | current menu_MainMenu | depth 1');shot('main-return-'+size)
   data=settings_bytes();(a.output/('settings-'+size+'.bin')).write_bytes(data);values=decode(data)
   expected={'VolumeMusic':music,'VolumeFX':fx,'DPad':0,'HUDStyle':1,'AutoTransmute':1,'AutoOrientation':0,'Language':0}
   assert all(values[k]==v for k,v in expected.items()),values
   launch();shot('main-relaunch-'+size)
   wait('Front settings loaded | found 1 | options 16 | language 0 | music '+str(music)+' | fx '+str(fx))
   action(410,160,'Owned menu navigation | push menu_Options | depth 2');shot('options-persisted-'+size)
   assert decode(settings_bytes())==values
   (a.output/('settings-'+size+'.log')).write_text(logs(),encoding='utf-8')
   report['scenarios'].append({'size':size,'surface':surface.copy(),'values':expected,'file_bytes':len(data),'restart_persistence':True,'english_cached_pack1_and_return':True})
  pid=adb('shell','pidof','com.example.dh2').strip()
  adb('shell','wm','size','1080x2400');time.sleep(1);shot('options-live-resize')
  adb('shell','input','keyevent','KEYCODE_HOME');time.sleep(.5)
  adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity','-f','0x10200000');time.sleep(1);shot('options-after-resume')
  assert adb('shell','pidof','com.example.dh2').strip()==pid
  action(18,18,'Owned menu navigation | pop menu_Options | current menu_MainMenu | depth 1')
  report['live_resize_resume_back']=True;report['status']='PASS'
 except Exception as e:
  report['status']='FAIL';report['error']=str(e);(a.output/'failure.log').write_text(logs(),encoding='utf-8');shot('failure');raise
 finally:
  adb('shell','am','force-stop','com.example.dh2')
  if backup is None:adb('shell','run-as','com.example.dh2','rm','-f','files/dh2_settings.savegame')
  else:
   remote='/data/local/tmp/dh2-front-settings-test-restore.bin';adb('push',str(a.output/'prior-settings.bin'),remote)
   adb('shell','run-as','com.example.dh2','cp',remote,'files/dh2_settings.savegame');adb('shell','rm',remote)
   assert settings_bytes()==backup
  adb('shell','wm','size',override[1] if override else 'reset');launch();shot('restored-main')
  (a.output/'settings-validation.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps(report,indent=2))
if __name__=='__main__':main()
