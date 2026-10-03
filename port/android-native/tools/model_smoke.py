"""Exercise original static scenes, GPU draws, orbit and context recreation."""
import argparse,hashlib,json,re,subprocess,time
from pathlib import Path
import xml.etree.ElementTree as ET
from PIL import Image,ImageChops
from emulator_smoke import inspect,launch_fresh
MODELS={'candle_flame.bdae':(2,4),'main_menu_charactere_swamp.bdae':(4,2146),'prince_modular.bdae':(4,586)}

def main():
 p=argparse.ArgumentParser();p.add_argument('--adb',required=True);p.add_argument('--serial',required=True)
 p.add_argument('--apk',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 assert a.serial.startswith('emulator-');a.output.mkdir(parents=True,exist_ok=True)
 def adb(*args,missing=False):
  r=subprocess.run([a.adb,'-s',a.serial,*args],capture_output=True,text=True,timeout=45)
  if missing and r.returncode==1 and not (r.stdout+r.stderr).strip():return ''
  if r.returncode:raise RuntimeError(r.stdout+r.stderr)
  return r.stdout.strip()
 libraries=inspect(a.apk);prior=adb('shell','cmd','window','user-rotation').split();rows=[]
 def logs(pid):
  text=adb('logcat','-d','--pid='+pid,'-v','brief')
  assert not re.search(r'FATAL EXCEPTION|GL error|Shader failed|Link failed|Model load failed|Animation sample failed|Skin sample failed',text),text
  return text
 def capture(stem):
  adb('shell','uiautomator','dump','/sdcard/dh2-native-window.xml')
  root=ET.fromstring(adb('shell','cat','/sdcard/dh2-native-window.xml'))
  n=next(n for n in root.iter('node') if n.get('content-desc')=='DH2 native texture viewport')
  bounds=tuple(map(int,re.findall(r'\d+',n.get('bounds'))))
  adb('shell','screencap','-p','/sdcard/dh2-model-test.png')
  path=a.output/(stem+'.png');adb('pull','/sdcard/dh2-model-test.png',str(path))
  crop=Image.open(path).convert('RGB').crop(bounds)
  mask=ImageChops.difference(crop,Image.new('RGB',crop.size,(20,23,28))).convert('L').point(lambda v:255 if v>8 else 0)
  count=mask.histogram()[255];assert count>200,(stem,'Blank model surface',count)
  return crop,{'screenshot':path.name,'viewport':crop.size,'rendered_pixels':count,'rendered_bounds':mask.getbbox()}
 try:
  for model,(draws,triangles) in MODELS.items():
   adb('shell','cmd','window','user-rotation','lock','0')
   launch=launch_fresh(adb,'--es','model',model,'--ei','time_ms','0')
   deadline=time.monotonic()+30;pid=''
   while not pid:
    pid=adb('shell','pidof','com.example.dh2',missing=True)
    assert time.monotonic()<deadline;time.sleep(.1)
   expected=f'3D upload OK | {draws} draws | {triangles} triangles'
   while True:
    text=logs(pid)
    if 'models/'+model+': '+expected in text:break
    assert time.monotonic()<deadline,text;time.sleep(.1)
   before,record=capture(Path(model).stem+'-portrait');record.update(model=model,rotation=0,upload_ok=True);rows.append(record)
   # Rotate on the actual GLSurfaceView, rather than injecting native calls.
   adb('shell','input','swipe','400','1100','620','1200','400')
   after,record=capture(Path(model).stem+'-orbit')
   assert ImageChops.difference(before,after).getbbox(),'Orbit did not change rendered pixels'
   record.update(model=model,orbit_changed_pixels=True);rows.append(record)
   adb('shell','cmd','window','user-rotation','lock','1')
   deadline=time.monotonic()+30
   while True:
    text=logs(pid);frames=re.findall(r'Model frame submitted at (\d+) x (\d+)',text)
    if frames and int(frames[-1][0])>int(frames[-1][1]):break
    assert time.monotonic()<deadline,text;time.sleep(.1)
   _,record=capture(Path(model).stem+'-landscape');record.update(model=model,rotation=1);rows.append(record)
   # Android recreates the GL context on rotation; it must reload the scene.
   text=logs(pid);assert text.count('Renderer: ')>=2,text
   (a.output/(Path(model).stem+'.log')).write_text('\n'.join(x for x in text.splitlines() if 'DH2Native' in x or 'AndroidRuntime' in x)+'\n')
 finally:
  if prior and prior[0] in ('free','lock'):adb('shell','cmd','window','user-rotation',*prior)
 report={'apk_sha256':hashlib.sha256(a.apk.read_bytes()).hexdigest(),'serial':a.serial,'api':adb('shell','getprop','ro.build.version.sdk'),
         'abi':adb('shell','getprop','ro.product.cpu.abi'),'page_size':adb('shell','getconf','PAGESIZE'),
         'libraries':libraries,'cases':rows,'physical_arm64_tested':False,'original_visual_parity_verified':False}
 (a.output/'model-smoke.json').write_text(json.dumps(report,indent=2)+'\n')
 print(f'Passed {len(rows)} scene screenshots, including orbit and rotation/context recreation')
if __name__=='__main__':main()
