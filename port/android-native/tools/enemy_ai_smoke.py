"""Reach a Crypt skeleton through touch input; it acquires/attacks unaided."""
import argparse,hashlib,json,re,struct,subprocess,time,xml.etree.ElementTree as ET
from pathlib import Path
from PIL import Image
from emulator_smoke import inspect,launch_fresh
from player_defender_smoke import HIT,checksum
from player_combat_smoke import POSITION

def main():
 p=argparse.ArgumentParser();p.add_argument('--adb',required=True);p.add_argument('--serial',required=True);p.add_argument('--apk',type=Path,required=True);p.add_argument('--oracle',type=Path,required=True);p.add_argument('--reference',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();assert a.serial.startswith('emulator-');a.output.mkdir(parents=True,exist_ok=True);inspect(a.apk);oracle=json.loads(a.oracle.read_text());raw=a.reference.read_bytes();assert oracle['player_defender'] and oracle['mismatches']==0 and hashlib.sha256(raw).hexdigest()==oracle['reference_sha256'];reference=oracle['live_sequences'][0];base=4+(oracle['synthetic_cases']+oracle['source_cases'])*14600
 def adb(*args):
  r=subprocess.run([a.adb,'-s',a.serial,*args],capture_output=True,text=True,timeout=45);assert r.returncode==0,r.stdout+r.stderr;return r.stdout.strip()
 def logs():
  pid=adb('shell','pidof','com.example.dh2');assert pid;s=adb('logcat','-d','--pid='+pid,'-v','brief');assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|completion failed|event dispatch failed|combat route failed|application failed|animation failed|Shader failed|Link failed',s),s[-6000:];return s
 def wait(predicate,seconds=45):
  end=time.monotonic()+seconds
  while True:
   s=logs()
   if predicate(s):return s
   assert time.monotonic()<end,s[-6000:];time.sleep(.1)
 def views():
  adb('shell','uiautomator','dump','/sdcard/dh2-ai.xml');root=ET.fromstring(adb('shell','cat','/sdcard/dh2-ai.xml'));return {n.get('content-desc'):n for n in root.iter('node') if n.get('content-desc')}
 def bounds(n):return tuple(map(int,re.findall(r'\d+',n.get('bounds'))))
 def capture(name):
  file=a.output/(name+'.png');v=views();adb('shell','screencap','-p','/sdcard/dh2-ai.png');adb('pull','/sdcard/dh2-ai.png',str(file));colors=Image.open(file).convert('RGB').crop(bounds(v['DH2 native texture viewport'])).getcolors(4096);assert colors is None or len(colors)>1024;return file.name
 def position(s):
  rows=re.findall(POSITION,s)
  return [float(v) for v in rows[-1][:3]] if rows else list(map(float,re.findall(r'World ready .*?position ([\d.-]+) ([\d.-]+) ([\d.-]+)',s)[-1]))
 movement=[]
 def travel(axis,destination,until_attack=False):
  for _ in range(25):
   s=logs();start=position(s);delta=destination-start[axis]
   if until_attack and 'Enemy melee selected | _prim_tmp_cultist07' in s:return
   if abs(delta)<50:return
   x0,y0,x1,y1=bounds(views()['Movement control']);cx,cy=(x0+x1)//2,(y0+y1)//2;sign=1 if delta>0 else -1;x=cx+round((x1-x0)*.4*sign) if axis==0 else cx;y=cy-round((y1-y0)*.4*sign) if axis==1 else cy;before=s.count('Player position ')
   adb('shell','input','touchscreen','motionevent','DOWN',str(x),str(y));time.sleep(min(1.2,max(.15,abs(delta)/420*.8)));adb('shell','input','touchscreen','motionevent','UP',str(x),str(y));s=wait(lambda s:s.count('Player position ')>before);after=position(s);movement.append({'start':start,'end':after,'axis':axis});assert abs(after[axis]-start[axis])>.5,(start,after)
  raise AssertionError('AI approach failed')
 def time_command(ms):
  before=logs().count('Actor command applied |');adb('shell','am','start','-W','--activity-single-top','-n','com.example.dh2/.MainActivity','--ei','time_ms',str(ms));return wait(lambda s:s.count('Actor command applied |')>before)
 sha=hashlib.sha256(a.apk.read_bytes()).hexdigest();rotations=[];pictures=[]
 try:
  adb('shell','input','keyevent','KEYCODE_HOME');assert 'Success' in adb('install','-r',str(a.apk));remote=adb('shell','pm','path','com.example.dh2').removeprefix('package:');assert adb('shell','sha256sum',remote).split()[0]==sha
  launch_fresh(adb,'--es','world','crypt01.dwld');s=wait(lambda s:'World ready |' in s and 'Model frame submitted at' in s);assert 'AI tables ready | configs 76 | factions 16 | automatic melee 1' in s;time.sleep(.3);assert not re.findall(HIT,logs())
  travel(1,-1000);travel(0,-1390);travel(1,-370,True);s=wait(lambda s:len(re.findall(HIT,s))>=1);approach=position(s);s=time_command(100);s=wait(lambda s:'Animation frame rendered at 100 ms' in s);count=len(re.findall(HIT,s));assert count<24;time.sleep(.3);assert len(re.findall(HIT,logs()))==count;pictures.append(capture('enemy-attacks-automatically'))
  for rotation in (1,0):
   before=logs().count('World ready |');adb('shell','cmd','window','user-rotation','lock',str(rotation));s=wait(lambda s:s.count('World ready |')>before and 'Animation frame rendered at 100 ms' in s);assert len(re.findall(HIT,s))==count;assert re.search(r'Combat actor restored \| _prim_tmp_cultist07 .*target -2 \| AI attack 1',s);rotations.append({'orientation':rotation,'damage_attempts':count})
  adb('shell','input','keyevent','KEYCODE_HOME');time.sleep(.3);assert len(re.findall(HIT,logs()))==count;adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity');s=wait(lambda s:'Animation frame rendered at 100 ms' in s);assert len(re.findall(HIT,s))==count
  s=time_command(-1);s=wait(lambda s:'Player death clip completed | completions 1 | active 0' in s,150);actual=re.findall(HIT,s);assert len(actual)==24
  verified=[]
  for i,(hit,expected) in enumerate(zip(actual,reference['attacks'])):
   source,attempt,result,before,after,dead,combo,seed,calls,statuses,armed,cue,hash_value=hit;words=expected['application_words'];signed=lambda v:v if v<0x80000000 else v-0x100000000
   assert source=='_prim_tmp_cultist07' and int(attempt)==i+1 and list(map(int,result.split()))==expected['result_after'];assert list(map(int,(before,after,dead,combo,armed,cue)))==[signed(words[1]),signed(words[2]),expected['state_after'][5],expected['state_after'][2],words[4],words[5]];assert [int(seed),int(calls)]==expected['random_after'] and int(statuses)==words[16] and hash_value==checksum(raw[base+i*14600+13520:base+i*14600+14416]);verified.append({'attempt':i+1,'hp_after':int(after),'checksum':hash_value,'rng':[int(seed),int(calls)]})
  assert 'Enemy target cleared | _prim_tmp_cultist07 | event 10 | Prince dead 1 | controller stopped' in s;time.sleep(.3);assert len(re.findall(HIT,logs()))==24;assert 'Defeated' in views()['Player health and mana'].get('text');pictures.append(capture('automatic-player-death'));s=logs();assert 'supplied development target' not in s and 'Actor state selected |' not in s;(a.output/'enemy-ai.log').write_text(s)
 finally:
  adb('shell','input','touchscreen','motionevent','CANCEL','0','0');adb('shell','cmd','window','user-rotation','lock','0');adb('shell','input','keyevent','KEYCODE_HOME')
 report={'apk_sha256':sha,'automatic_damage_attempts_verified':len(verified),'original_result_health_rng_owner_checksums_match':True,'attempts':verified,'approach_position':approach,'movement':movement,'rotations':rotations,'frozen_no_damage':True,'pause_no_damage':True,'death_clears_enemy_target_and_stops_controller':True,'screenshots':pictures,'explicit_development_targets_used':False,'automatic_targeting_default_enabled':True,'physical_arm64_tested':False,'full_game_playable':False,'scope':'One hostile player candidate adapter, original faction/3D melee/target event rules and authored attack events. Original spatial query, pursuit, full FSM/attack delay, status/audio and game-over remain pending.'};(a.output/'enemy-ai-smoke.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('movement','attempts')}),flush=True)
if __name__=='__main__':main()
