"""Check animation changes GPU pixels and deterministic frozen poses remain stable."""
import argparse,hashlib,json,re,subprocess,time
from pathlib import Path
import xml.etree.ElementTree as ET
from PIL import Image,ImageChops
from emulator_smoke import inspect,launch_fresh
def main():
 p=argparse.ArgumentParser();p.add_argument('--adb',required=True);p.add_argument('--serial',required=True)
 p.add_argument('--apk',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
 p.add_argument('--model',default='candle_flame.bdae');p.add_argument('--second-ms',type=int,default=500);a=p.parse_args()
 expected_tracks=29 if a.model=='prince_modular.bdae' else 2
 assert a.serial.startswith('emulator-');a.output.mkdir(parents=True,exist_ok=True)
 def adb(*args,missing=False):
  r=subprocess.run([a.adb,'-s',a.serial,*args],capture_output=True,text=True,timeout=45)
  if missing and r.returncode==1 and not (r.stdout+r.stderr).strip():return ''
  if r.returncode:raise RuntimeError(r.stdout+r.stderr)
  return r.stdout.strip()
 libraries=inspect(a.apk);prior=adb('shell','cmd','window','user-rotation').split();rows=[]
 def launch(cursor):
  args=['--es','model',a.model]
  if cursor is not None:args+=['--ei','time_ms',str(cursor)]
  response=launch_fresh(adb,*args)
  deadline=time.monotonic()+30;pid=''
  while True:
   pid=adb('shell','pidof','com.example.dh2',missing=True)
   logs=adb('logcat','-d','--pid='+pid,'-v','brief') if pid else ''
   assert not re.search(r'FATAL EXCEPTION|GL error|Model load failed|Animation sample failed|Skin sample failed',logs),logs
   marker=f'Animation frame rendered at {cursor} ms' if cursor is not None else f'{expected_tracks} animation tracks | 0 skipped'
   if marker in logs:break
   assert time.monotonic()<deadline,logs;time.sleep(.1)
  return pid
 def screenshot(stem):
  adb('shell','uiautomator','dump','/sdcard/dh2-native-window.xml')
  root=ET.fromstring(adb('shell','cat','/sdcard/dh2-native-window.xml'))
  view=next(n for n in root.iter('node') if n.get('content-desc')=='DH2 native texture viewport')
  bounds=tuple(map(int,re.findall(r'\d+',view.get('bounds'))))
  adb('shell','screencap','-p','/sdcard/dh2-animation-test.png');path=a.output/(stem+'.png');adb('pull','/sdcard/dh2-animation-test.png',str(path))
  crop=Image.open(path).convert('RGB').crop(bounds)
  mask=ImageChops.difference(crop,Image.new('RGB',crop.size,(20,23,28))).convert('L').point(lambda x:255 if x>8 else 0)
  assert mask.histogram()[255]>1000,'Blank animated model surface'
  return crop,{'screenshot':path.name,'viewport':crop.size,'rendered_bounds':mask.getbbox()}
 try:
  adb('shell','cmd','window','user-rotation','lock','0')
  pid=launch(100);first,row=screenshot('frozen-100');row['time_ms']=100;rows.append(row)
  same,row=screenshot('frozen-100-repeat');assert first.size==same.size and ImageChops.difference(first,same).getbbox() is None,'Frozen pose moved'
  row['frozen_pose_stable']=True;rows.append(row)
  pid=launch(a.second_ms);second,row=screenshot('frozen-'+str(a.second_ms));row['time_ms']=a.second_ms;rows.append(row)
  difference=ImageChops.difference(first,second);assert difference.getbbox(),'Different keys produced identical GPU frames'
  changed=difference.convert('L').point(lambda x:255 if x>2 else 0).histogram()[255];assert changed>50,changed
  pid=launch(None);live1,row=screenshot('live-first');rows.append(row);live2,row=screenshot('live-second');rows.append(row)
  assert ImageChops.difference(live1,live2).getbbox(),'Live animation did not change GPU pixels'
  logs=adb('logcat','-d','--pid='+pid,'-v','brief');assert not re.search(r'FATAL EXCEPTION|GL error|Animation sample failed|Skin sample failed',logs),logs
  (a.output/'animation.log').write_text('\n'.join(x for x in logs.splitlines() if 'DH2Native' in x or 'AndroidRuntime' in x)+'\n')
 finally:
  if prior and prior[0] in ('free','lock'):adb('shell','cmd','window','user-rotation',*prior)
 report={'apk_sha256':hashlib.sha256(a.apk.read_bytes()).hexdigest(),'serial':a.serial,'api':adb('shell','getprop','ro.build.version.sdk'),
  'abi':adb('shell','getprop','ro.product.cpu.abi'),'model':a.model,'libraries':libraries,'cases':rows,'changed_pixels_between_poses':changed,
  'pose_times_ms':[100,a.second_ms],'live_animation_changed_pixels':True,'frozen_pose_stable':True,'physical_arm64_tested':False,'skinning_implemented':a.model=='prince_modular.bdae'}
 (a.output/'animation-smoke.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('libraries','cases')}))
if __name__=='__main__':main()
