"""Inspect native objects at authored positions without teleporting the player."""
import argparse,hashlib,json,re,subprocess,time,xml.etree.ElementTree as ET
from pathlib import Path
from PIL import Image,ImageChops
from emulator_smoke import inspect,launch_fresh
def main():
 p=argparse.ArgumentParser();p.add_argument('--adb',required=True);p.add_argument('--serial',required=True);p.add_argument('--apk',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 assert a.serial.startswith('emulator-');a.output.mkdir(parents=True,exist_ok=True);libraries=inspect(a.apk)
 with __import__('zipfile').ZipFile(a.apk) as archive:provenance=json.loads(archive.read('assets/actor-provenance.json'))
 selected={}
 for i,row in enumerate(provenance['records']):selected.setdefault(row['model'],i)
 rows=[]
 def adb(*args,missing=False):
  r=subprocess.run([a.adb,'-s',a.serial,*args],capture_output=True,text=True,timeout=45)
  if missing and r.returncode==1 and not (r.stdout+r.stderr).strip():return ''
  if r.returncode:raise RuntimeError(r.stdout+r.stderr)
  return r.stdout.strip()
 def logs():
  pid=adb('shell','pidof','com.example.dh2',missing=True);assert pid,'App process exited'
  text=adb('logcat','-d','--pid='+pid,'-v','brief');assert not re.search(r'FATAL EXCEPTION|GL error|Shader failed|Link failed|load failed|sample failed',text),text
  return text
 def wait(predicate):
  end=time.monotonic()+30
  while True:
   text=logs()
   if predicate(text):return text
   assert time.monotonic()<end,text;time.sleep(.1)
 def launch(index,ms):
  adb('shell','input','touchscreen','motionevent','CANCEL','0','0');args=['--es','world','crypt01.dwld','--ei','object_index',str(index)]
  if ms is not None:args+=['--ei','time_ms',str(ms)]
  launch_fresh(adb,*args)
  return wait(lambda t:'Objects ready | monsters 11 | decors 84 | resources 12 | instance draws 291 | triangles 6103 | character records 448 | model entries 116' in t and f'Inspect object {index} |' in t and 'Model frame submitted at' in t and (ms is None or f'Animation frame rendered at {ms} ms' in t))
 def capture(stem,index):
  adb('shell','uiautomator','dump','/sdcard/dh2-object-ui.xml');root=ET.fromstring(adb('shell','cat','/sdcard/dh2-object-ui.xml'))
  node=next(n for n in root.iter('node') if n.get('content-desc')=='DH2 native texture viewport');bounds=tuple(map(int,re.findall(r'\d+',node.get('bounds'))))
  path=a.output/(stem+'.png');adb('shell','screencap','-p','/sdcard/dh2-object-test.png');adb('pull','/sdcard/dh2-object-test.png',str(path))
  picture=Image.open(path).convert('RGB').crop(bounds);mask=ImageChops.difference(picture,Image.new('RGB',picture.size,(20,23,28))).convert('L').point(lambda v:255 if v>8 else 0);pixels=mask.histogram()[255]
  assert pixels>picture.width*picture.height*.15,(stem,pixels)
  rows.append({'screenshot':path.name,'object_index':index,'model':provenance['records'][index]['model'],'room':provenance['records'][index]['room'],'position':provenance['records'][index]['position'],'viewport':picture.size,'bounds':bounds,'rendered_pixels':pixels});return picture
 prior=adb('shell','cmd','window','user-rotation').split();differences={}
 try:
  assert 'Success' in adb('install','-r',str(a.apk));adb('shell','cmd','window','user-rotation','lock','0')
  originals={}
  for name,index in selected.items():launch(index,100);originals[name]=capture(name.removesuffix('.bdae')+'-100',index)
  for name,cursor in (('skeleton.bdae',700),('slime_green_v2.bdae',700),('ghost.bdae',1000),('crypt_candle_big.bdae',700)):
   index=selected[name];launch(index,cursor);second=capture(name.removesuffix('.bdae')+'-'+str(cursor),index);diff=ImageChops.difference(originals[name],second).convert('L').point(lambda v:255 if v>3 else 0).histogram()[255];assert diff>100,(name,diff);differences[name]=diff
  index=selected['skeleton.bdae'];launch(index,100);first=capture('skeleton-frozen-a',index);second=capture('skeleton-frozen-b',index);assert ImageChops.difference(first,second).getbbox() is None
  launch(index,None);first=capture('skeleton-live-a',index);second=capture('skeleton-live-b',index);assert ImageChops.difference(first,second).getbbox()
  text=logs();ready=text.count('Objects ready |');adb('shell','cmd','window','user-rotation','lock','1')
  text=wait(lambda t:t.count('Objects ready |')>ready and t.rfind('Model frame submitted at')>t.rfind(f'Inspect object {index} |') and bool(re.findall(r'Model frame submitted at (\d+) x (\d+)',t)) and int(re.findall(r'Model frame submitted at (\d+) x (\d+)',t)[-1][0])>int(re.findall(r'Model frame submitted at (\d+) x (\d+)',t)[-1][1]));landscape=capture('skeleton-landscape',index);assert landscape.width>landscape.height
  ready=text.count('Objects ready |');adb('shell','input','keyevent','KEYCODE_HOME');adb('shell','input','touchscreen','motionevent','CANCEL','0','0');adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity')
  wait(lambda t:t.count('Objects ready |')>ready and t.rfind('Model frame submitted at')>t.rfind(f'Inspect object {index} |'));capture('skeleton-resumed',index)
  text=logs();resources=re.findall(r'Object resource (\S+) \| primitives (\d+) \| tracks (\d+) \| unbound (\d+) \| unsupported (\d+) \| removed helpers (\d+)',text)
  assert len({r[0] for r in resources})==12;assert any(r[0]=='skeleton.bdae' and r[3]=='3' for r in resources);assert any(r[0]=='swamp_caveentrance_effect.bdae' and r[4]=='3' for r in resources)
  (a.output/'objects-lifecycle.log').write_text('\n'.join(line for line in text.splitlines() if 'DH2Native' in line or 'AndroidRuntime' in line)+'\n')
 finally:
  adb('shell','input','touchscreen','motionevent','CANCEL','0','0')
  if prior and prior[0] in ('free','lock'):adb('shell','cmd','window','user-rotation',*prior)
 report={'apk_sha256':hashlib.sha256(a.apk.read_bytes()).hexdigest(),'serial':a.serial,'api':adb('shell','getprop','ro.build.version.sdk'),'abi':adb('shell','getprop','ro.product.cpu.abi'),'libraries':libraries,'objects':95,'monsters':11,'decors':84,'resource_count':12,'cases':rows,'changed_pixels_by_resource':differences,'frozen_stable':True,'live_animation_changes':True,'rotation_reloads_objects':True,'resume_reloads_objects':True,'player_teleported_for_tests':False,'physical_arm64_tested':False,'combat_or_ai_implemented':False,'original_gpu_parity_verified':False}
 (a.output/'objects-smoke.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('cases','libraries')}))
if __name__=='__main__':main()
