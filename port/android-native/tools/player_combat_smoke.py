"""Walk through the real touch control and attack a nearby Crypt skeleton."""
import argparse,hashlib,json,math,re,struct,subprocess,time,xml.etree.ElementTree as ET
from pathlib import Path
from PIL import Image
from emulator_smoke import inspect,launch_fresh
HIT=r'Prince combat hit \| target (\S+) \| attempt (\d+) \| result ((?:-?\d+ ){9}-?\d+) \| HP (-?\d+) (-?\d+) \| dead (\d+) \| combo (\d+) \| RNG (\d+) (\d+) \| statuses (\d+)'
POSITION=r'Player position ([\d.-]+) ([\d.-]+) ([\d.-]+) \| moved (\d+) \| blocked (\d+)'
def main():
 p=argparse.ArgumentParser();p.add_argument('--adb',required=True);p.add_argument('--serial',required=True);p.add_argument('--apk',type=Path,required=True);p.add_argument('--oracle',type=Path,required=True);p.add_argument('--spawn-reference',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();assert a.serial.startswith('emulator-');a.output.mkdir(parents=True,exist_ok=True);libraries=inspect(a.apk);oracle=json.loads(a.oracle.read_text());assert oracle['player_attacker'] and oracle['mismatches']==0;reference=next(r for r in oracle['live_sequences'] if r['defender']=='Crypt_Skeleton')
 def adb(*args,missing=False):
  r=subprocess.run([a.adb,'-s',a.serial,*args],capture_output=True,text=True,timeout=45)
  if missing and r.returncode==1 and not (r.stdout+r.stderr).strip():return ''
  if r.returncode:raise RuntimeError(r.stdout+r.stderr)
  return r.stdout.strip()
 def logs():
  pid=adb('shell','pidof','com.example.dh2',missing=True);assert pid,'App exited';s=adb('logcat','-d','--pid='+pid,'-v','brief');assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|completion failed|event dispatch failed|combat route failed|application failed|death animation failed|Shader failed|Link failed',s),s;return s
 def wait(predicate,seconds=45):
  end=time.monotonic()+seconds
  while True:
   s=logs()
   if predicate(s):return s
   assert time.monotonic()<end,s[-7000:];time.sleep(.1)
 def views():
  adb('shell','uiautomator','dump','/sdcard/dh2-player-window.xml');root=ET.fromstring(adb('shell','cat','/sdcard/dh2-player-window.xml'));return {n.get('content-desc'):tuple(map(int,re.findall(r'\d+',n.get('bounds')))) for n in root.iter('node') if n.get('content-desc')}
 def attack():
  x0,y0,x1,y1=views()['Attack nearby enemy'];before=logs().count('Player input |');adb('shell','input','tap',str((x0+x1)//2),str((y0+y1)//2));return wait(lambda s:s.count('Player input |')>before)
 def capture(name):
  file=a.output/(name+'.png');end=time.monotonic()+8
  while True:
   bounds=views()['DH2 native texture viewport'];adb('shell','screencap','-p','/sdcard/dh2-player-test.png');adb('pull','/sdcard/dh2-player-test.png',str(file));colors=Image.open(file).convert('RGB').crop(bounds).getcolors(4096)
   if colors is None or len(colors)>1024:return file.name
   assert time.monotonic()<end,'Scene not presented';time.sleep(.1)
 def position(s):
  matches=re.findall(POSITION,s)
  if matches:return [float(v) for v in matches[-1][:3]],int(matches[-1][4])
  return [float(v) for v in re.findall(r'World ready .*?position ([\d.-]+) ([\d.-]+) ([\d.-]+)',s)[-1]],0
 movement=[];rotations=[];pause=None;rows=[]
 def travel(axis,destination):
  for _ in range(25):
   start,blocked=position(logs());delta=destination-start[axis]
   if abs(delta)<50:return
   x0,y0,x1,y1=views()['Movement control'];cx,cy=(x0+x1)//2,(y0+y1)//2;sign=1 if delta>0 else -1;x=cx+round((x1-x0)*.4*sign) if axis==0 else cx;y=cy-round((y1-y0)*.4*sign) if axis==1 else cy;before=logs().count('Player position ')
   adb('shell','input','touchscreen','motionevent','DOWN',str(x),str(y));time.sleep(min(1.5,max(.15,abs(delta)/420*.8)));adb('shell','input','touchscreen','motionevent','UP',str(x),str(y));s=wait(lambda s:s.count('Player position ')>before);after,endblocked=position(s);movement.append({'axis':axis,'start':start,'end':after,'blocked':endblocked-blocked});assert abs(after[axis]-start[axis])>.5,('Movement blocked before enemy',start,after)
  raise AssertionError('Waypoint not reached')
 try:
  adb('shell','input','touchscreen','motionevent','CANCEL','0','0');adb('shell','input','keyevent','KEYCODE_HOME');assert 'Success' in adb('install','-r',str(a.apk));remote=adb('shell','pm','path','com.example.dh2').removeprefix('package:');installed=adb('shell','sha256sum',remote).split()[0];sha=hashlib.sha256(a.apk.read_bytes()).hexdigest();assert sha==installed
  launch_fresh(adb,'--ez','enemy_ai','false','--es','world','crypt01.dwld');s=wait(lambda s:'World ready |' in s and 'Player properties |' in s and 'Model frame submitted at' in s)
  assert len(re.findall(r'Player attack track ready \|',s))==9
  spawn=a.spawn_reference.read_bytes();assert hashlib.sha256(spawn).hexdigest()==oracle['spawn_reference_sha256'];sheet=spawn[263*3584+2688:264*3584];h=14695981039346656037
  for value in sheet:h=((h^value)*1099511628211)&0xffffffffffffffff
  assert f'Player properties | KnightPlayerBase | HP 42265 | MP 6976 | checksum {h:016x} | attempts 0 | attacking 0' in s
  s=attack();assert 'Player input | Walk closer to an enemy' in s and not re.findall(HIT,s);capture('out-of-reach')
  travel(1,-1000);travel(0,-1390);travel(1,-370);nearby=position(logs())[0];assert abs(nearby[2]-198.437)<=96 and ((nearby[0]+1234.43)**2+(nearby[1]+369.638)**2)**.5<=220
  # Schedule while frozen: the authored event must not change HP or RNG.
  s=adb('shell','am','start','-W','--activity-single-top','-n','com.example.dh2/.MainActivity','--ez','player_attack','true','--ei','player_target_index','4','--ei','time_ms','100');assert 'Status: ok' in s;s=wait(lambda s:'Player command applied | Attacking' in s and 'Animation frame rendered at 100 ms' in s);time.sleep(.2);assert not re.findall(HIT,logs());capture('frozen-attack-no-hit')
  # UI hierarchy lookup may outlast a live attack. Exercise the real touch
  # busy guard while the sequence is frozen, then resume that same sequence.
  selected=logs().count('Player attack selected |');s=attack();busy_verified='Player input | Attack is already in progress' in s
  assert busy_verified and s.count('Player attack selected |')==selected and not re.findall(HIT,s)
  adb('shell','am','start','-W','--activity-single-top','-n','com.example.dh2/.MainActivity','--ei','time_ms','-1');s=wait(lambda s:len(re.findall(HIT,s))>=1);first_target=re.findall(HIT,s)[0][0];assert first_target=='_prim_tmp_cultist07'
  s=wait(lambda s:'Player clip completed | completions 9 | active 0' in s);assert len(re.findall(HIT,s))==3
  for orientation in (1,0):
   count=s.count('World ready |');adb('shell','cmd','window','user-rotation','lock',str(orientation));s=wait(lambda s:s.count('World ready |')>count and 'Player properties | KnightPlayerBase | HP 42265 | MP 6976' in s);time.sleep(.2);assert len(re.findall(HIT,logs()))==3;rotations.append({'orientation':orientation,'attempts':3})
  for cycle in range(1,50):
   if len(re.findall(HIT,s))==len(reference['attacks']):break
   selected=s.count('Player attack selected |');done=s.count('Player clip completed | completions 9 | active 0');s=attack();s=wait(lambda s:s.count('Player attack selected |')>selected)
   if cycle==1:
    adb('shell','input','keyevent','KEYCODE_HOME');before=len(re.findall(HIT,logs()));adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity');pause={'attempts_at_pause':before}
   s=wait(lambda s:s.count('Player clip completed | completions 9 | active 0')>done)
   if cycle%5==0:print(json.dumps({'cycles':cycle+1,'attempts':len(re.findall(HIT,s))}),flush=True)
  attempts=re.findall(HIT,s);assert len(attempts)==len(reference['attacks'])==112
  for number,(actual,expected) in enumerate(zip(attempts,reference['attacks']),1):
   target,attempt,result,before,after,dead,combo,seed,calls,statuses=actual;words=expected['application_words'];signed=lambda v:v if v<0x80000000 else v-0x100000000
   assert target==first_target and int(attempt)==number and list(map(int,result.split()))==expected['result_after'] and list(map(int,(before,after,dead,combo)))==[signed(words[1]),signed(words[2]),expected['state_after'][5],expected['state_after'][2]] and [int(seed),int(calls)]==expected['random_after'] and int(statuses)==words[16],(number,actual,expected)
   rows.append({'attempt':number,'result':list(map(int,result.split())),'hp_before':int(before),'hp_after':int(after),'dead':int(dead),'combo':int(combo),'random_after':[int(seed),int(calls)],'status_requests':int(statuses)})
  s=wait(lambda s:f'Combat death animation selected | {first_target} | clip 1185 | dead 1' in s and re.search(r'Actor clip completed \| '+re.escape(first_target)+r' \| state Died \| completions 1 \| active 0',s));capture('enemy-defeated');s=attack();assert 'Player input | Walk closer to an enemy' in s and len(re.findall(HIT,s))==112
  aggro=re.findall(r'Combat aggression \| owner (\d+) \| target (\d+) \| amount bits ([0-9a-f]{8}) \| delta bits ([0-9a-f]{8}) \| outgoing (\d+) \| incoming (\d+) \| requests (\d+)',s)
  positive=sum(row['result'][0]>0 for row in rows);assert len(aggro)==positive and positive>0
  assert all((row[0],row[1],row[4],row[5])==('4294967302','4294967297','1','1') for row in aggro)
  previous=0.;index=0
  for expected in reference['attacks']:
   if expected['result_after'][0]<=0:continue
   amount=expected['application_words'][14];amount_float=struct.unpack('<f',struct.pack('<I',amount))[0]
   current=struct.unpack('<f',struct.pack('<f',previous+amount_float))[0];delta=struct.unpack('<I',struct.pack('<f',current-previous))[0]
   assert (aggro[index][2],aggro[index][3])==(f'{amount:08x}',f'{delta:08x}'),(index,aggro[index],amount,delta)
   previous=current;index+=1
  assert [int(row[6]) for row in aggro]==[1]+[0]*(positive-1)
  assert re.search(r'Combat actor restored \| '+re.escape(first_target)+r' .*aggro 1 0',s),'Aggression not preserved on GL recreation'
  text=logs();facing_rows=re.findall(r'Player attack facing \| direction ([\d.eE+-]+) ([\d.eE+-]+) \| angle ([\d.eE+-]+) \| native updates (\d+)',text)
  if facing_rows:
   assert len(facing_rows)==text.count('Player attack selected |'),(len(facing_rows),text.count('Player attack selected |'))
   for x,y,angle,count in facing_rows:
    x,y,angle=float(x),float(y),float(angle);assert int(count)>0 and (x or y)
    expected=(math.pi/2 if x>0 else 3*math.pi/2) if y==0 else math.atan(x/-y)+(math.pi if y>0 and x>0 else -math.pi if y>0 else 0.)
    assert abs(angle-expected)<2e-6,(x,y,angle,expected)
  if facing_rows:(a.output/'player-heading.json').write_text(json.dumps({'apk_sha256':sha,'native_heading_used_by_player_melee':True,'attack_heading_checks':len(facing_rows),'angle_tolerance_radians':2e-6,'directions_and_angles':[[float(x),float(y),float(angle),int(count)] for x,y,angle,count in facing_rows]},indent=2)+'\n')
  (a.output/'player-combat.log').write_text(text);assert busy_verified,'Busy-input guard was not exercised'
 finally:
  adb('shell','input','touchscreen','motionevent','CANCEL','0','0');adb('shell','cmd','window','user-rotation','lock','0');adb('shell','input','keyevent','KEYCODE_HOME')
 report={'apk_sha256':sha,'installed_apk_sha256':installed,'original_player_application_report_sha256':hashlib.sha256(a.oracle.read_bytes()).hexdigest(),'native_player_attempts_verified':len(rows),'attempts':rows,'automatic_original_death_clip':1185,'movement':movement,'approach_position':nearby,'out_of_reach_rejected':True,'frozen_attack_no_hit':True,'busy_attack_does_not_restart':busy_verified,'rotation_does_not_replay_hits':rotations,'inflight_pause_resume':pause,'original_player_sheet_checksum_verified':True,'player_attack_clips':9,'physical_arm64_tested':False,'full_ai_or_equipment_implemented':False,'full_game_playable':False,'automatic_enemy_ai_disabled_for_isolated_regression':True,'native_aggression_updates_verified':positive,'reciprocal_relation_and_rotation_retention_verified':True,'input_policy':'New nearest living target selection with original strict full-3D sum-of-melee-radii range. Single tap plays stationary three-combo sequence; original input/FSM/root motion still pending.'};(a.output/'player-combat-smoke.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('attempts','movement')}),flush=True)
if __name__=='__main__':main()
