"""Drive original enemy melee events through Prince health, warning and death."""
import argparse,hashlib,json,re,struct,subprocess,time,xml.etree.ElementTree as ET
from pathlib import Path
from decimal import Decimal,ROUND_HALF_UP
from PIL import Image
from emulator_smoke import inspect,launch_fresh
HIT=r'Prince damage received \| attacker (\S+) \| attempt (\d+) \| result ((?:-?\d+ ){9}-?\d+) \| HP (-?\d+) (-?\d+) \| dead (\d+) \| combo (\d+) \| RNG (\d+) (\d+) \| statuses (\d+) \| low health armed (\d+) \| cue (\d+) \| checksum ([0-9a-f]{16})'
def checksum(raw):
 h=14695981039346656037
 for v in raw:h=((h^v)*1099511628211)&0xffffffffffffffff
 return f'{h:016x}'
def main():
 p=argparse.ArgumentParser();p.add_argument('--adb',required=True);p.add_argument('--serial',required=True);p.add_argument('--apk',type=Path,required=True);p.add_argument('--oracle',type=Path,required=True);p.add_argument('--reference',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();assert a.serial.startswith('emulator-');a.output.mkdir(parents=True,exist_ok=True);inspect(a.apk);oracle=json.loads(a.oracle.read_text());raw=a.reference.read_bytes();assert oracle['player_defender'] and oracle['mismatches']==0 and hashlib.sha256(raw).hexdigest()==oracle['reference_sha256'];assert struct.unpack_from('<I',raw)[0]==oracle['comparisons'];at=4+(oracle['synthetic_cases']+oracle['source_cases'])*14600
 def adb(*args,missing=False):
  r=subprocess.run([a.adb,'-s',a.serial,*args],capture_output=True,text=True,timeout=45)
  if missing and r.returncode==1 and not (r.stdout+r.stderr).strip():return ''
  assert r.returncode==0,r.stdout+r.stderr;return r.stdout.strip()
 def logs():
  pid=adb('shell','pidof','com.example.dh2',missing=True);assert pid,'App exited';s=adb('logcat','-d','--pid='+pid,'-v','brief');assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|completion failed|event dispatch failed|combat route failed|application failed|death animation failed|Shader failed|Link failed',s),s;return s
 def wait(predicate,seconds=45):
  end=time.monotonic()+seconds
  while True:
   s=logs()
   if predicate(s):return s
   assert time.monotonic()<end,s[-6000:];time.sleep(.1)
 def views():
  adb('shell','uiautomator','dump','/sdcard/dh2-defender-window.xml');tree=ET.fromstring(adb('shell','cat','/sdcard/dh2-defender-window.xml'));return {n.get('content-desc'):n for n in tree.iter('node') if n.get('content-desc')}
 def bounds(node):return tuple(map(int,re.findall(r'\d+',node.get('bounds'))))
 def command(index,state='Attack',target=-2):
  before=logs().count('Actor command applied |');adb('shell','am','start','-W','--activity-single-top','-n','com.example.dh2/.MainActivity','--ei','object_index',str(index),'--es','object_state',state,'--ei','combat_target_index',str(target),'--ei','time_ms','-1');return wait(lambda s:s.count('Actor command applied |')>before)
 def capture(name):
  file=a.output/(name+'.png');node=views()['DH2 native texture viewport'];adb('shell','screencap','-p','/sdcard/dh2-defender.png');adb('pull','/sdcard/dh2-defender.png',str(file));colors=Image.open(file).convert('RGB').crop(bounds(node)).getcolors(4096);assert colors is None or len(colors)>1024;return file.name
 def hud(hp,dead):
  end=time.monotonic()+10
  while True:
   value=views()['Player health and mana'].get('text');shown=(Decimal(hp)/256).quantize(Decimal('.1'),rounding=ROUND_HALF_UP);expected=f'HP {shown} / 165.1   MP 27.3 / 27.3'+('   Defeated' if dead else '')
   if value==expected:return value
   assert time.monotonic()<end,(value,expected)
 pairs=[];rotations=[];pause=False;movement_blocked=False;attack_blocked=False
 try:
  adb('shell','input','touchscreen','motionevent','CANCEL','0','0');adb('shell','input','keyevent','KEYCODE_HOME');assert 'Success' in adb('install','-r',str(a.apk));remote=adb('shell','pm','path','com.example.dh2').removeprefix('package:');installed=adb('shell','sha256sum',remote).split()[0];sha=hashlib.sha256(a.apk.read_bytes()).hexdigest();assert sha==installed
  for scenario,(index,reference) in enumerate(zip((0,2,76),oracle['live_sequences'])):
   launch_fresh(adb,'--ez','enemy_ai','false','--es','world','crypt01.dwld','--ei','object_index',str(index),'--es','object_state','Attack','--ei','combat_target_index','-2','--ei','time_ms','100');s=wait(lambda s:'Animation frame rendered at 100 ms' in s and 'Player death track ready | clip 1023 | tracks 27 | unbound 0 | attack_mainhand ms -1' in s);assert not re.findall(HIT,s);hud(42265,False);capture(f'pair-{scenario}-frozen');rows=[]
   for number,expected in enumerate(reference['attacks'],1):
    s=command(index);s=wait(lambda s:len(re.findall(HIT,s))==number);actual=re.findall(HIT,s)[-1];source,attempt,result,before,after,dead,combo,seed,calls,statuses,armed,cue,hash_value=actual;words=expected['application_words'];signed=lambda v:v if v<0x80000000 else v-0x100000000;sheet=raw[at+13520:at+14416];at+=14600
    assert int(attempt)==number and list(map(int,result.split()))==expected['result_after'] and list(map(int,(before,after,dead,combo,armed,cue)))==[signed(words[1]),signed(words[2]),expected['state_after'][5],expected['state_after'][2],words[4],words[5]] and [int(seed),int(calls)]==expected['random_after'] and int(statuses)==words[16] and hash_value==checksum(sheet),(scenario,number,actual,expected)
    rows.append({'attempt':number,'result':list(map(int,result.split())),'hp_before':int(before),'hp_after':int(after),'dead':int(dead),'combo':int(combo),'random_after':[int(seed),int(calls)],'status_requests':int(statuses),'low_health_armed':int(armed),'low_health_cue':int(cue),'resolved_checksum':hash_value})
    if scenario==0 and number==2:
     adb('shell','input','keyevent','KEYCODE_HOME');assert 'Status: ok' in adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity');pause=True
    s=wait(lambda s:re.search(r'Actor clip completed \| '+re.escape(source)+r' \| state Attack \| completions 3 \| active 0',s[s.rfind('Actor state selected |'):]))
    if scenario==0 and number==1:
     for orientation in (1,0):
      count=s.count('World ready |');adb('shell','cmd','window','user-rotation','lock',str(orientation));s=wait(lambda s:s.count('World ready |')>count);assert len(re.findall(HIT,s))==1;hud(int(after),False);rotations.append({'orientation':orientation,'attempts':1,'hp':int(after)})
    if int(cue):hud(int(after),False);capture(f'pair-{scenario}-low-health')
   s=wait(lambda s:'Player death animation selected | clip 1023 | dead 1 | lifecycle 3' in s and 'Player death clip completed | completions 1 | active 0' in s);assert len(re.findall(HIT,s))==24 and sum(r['low_health_cue'] for r in rows)==1;hud(0,True)
   if scenario==0:
    for orientation in (1,0):
     count=s.count('World ready |');adb('shell','cmd','window','user-rotation','lock',str(orientation));s=wait(lambda s:s.count('World ready |')>count);assert len(re.findall(HIT,s))==24 and s.count('Player death animation selected |')==1;hud(0,True);rotations.append({'orientation':orientation,'attempts':24,'hp':0})
    nodes=views();x0,y0,x1,y1=bounds(nodes['Attack nearby enemy']);adb('shell','input','tap',str((x0+x1)//2),str((y0+y1)//2));s=wait(lambda s:'Player input | Player is unavailable' in s);attack_blocked=True
    x0,y0,x1,y1=bounds(nodes['Movement control']);x,y=round(x1-(x1-x0)*.1),(y0+y1)//2;count=s.count('Player position ');adb('shell','input','touchscreen','motionevent','DOWN',str(x),str(y));time.sleep(.3);adb('shell','input','touchscreen','motionevent','UP',str(x),str(y));s=wait(lambda s:s.count('Player position ')>count);position=re.findall(r'Player position ([\d.-]+) ([\d.-]+) ([\d.-]+)',s)[-1];assert abs(float(position[0])+2227.77)<.01 and abs(float(position[1])-1220.9301)<.01;movement_blocked=True
   s=command(index);assert 'Combat target is dead' in s[s.rfind('Actor command applied |'):];assert len(re.findall(HIT,s))==24
   # Restore the camera to the Prince before recording his held death pose.
   before=s.count('Actor command applied |');adb('shell','am','start','-W','--activity-single-top','-n','com.example.dh2/.MainActivity','--ei','object_index','-1','--ei','time_ms','-1');s=wait(lambda s:s.count('Actor command applied |')>before);picture=capture(f'pair-{scenario}-player-defeated');(a.output/f'pair-{scenario}.log').write_text(logs());pairs.append({'attacker_index':index,'attacker':reference['attacker'],'defender':reference['defender'],'attacks':rows,'death_clip':1023,'death_screenshot':picture,'frozen_no_hit':True,'dead_player_target_rejected':True});print(json.dumps({'scenario':scenario,'attempts':len(rows),'dead':True}),flush=True)
  # A held touch must not suppress the original reaction for supplied state 13.
  launch_fresh(adb,'--ez','enemy_ai','false','--es','world','crypt01.dwld','--ei','object_index','0','--es','object_state','Attack','--ei','combat_target_index','-2','--ei','time_ms','100');s=wait(lambda s:'Animation frame rendered at 100 ms' in s);node=views()['Movement control'];x0,y0,x1,y1=bounds(node);x,y=(x0+x1)//2,round(y1-(y1-y0)*.1);s=command(0);adb('shell','input','touchscreen','motionevent','DOWN',str(x),str(y));s=wait(lambda s:len(re.findall(HIT,s))==1);actual=re.findall(HIT,s)[0];prefix=s[:s.index('Prince damage received |')];assert re.findall(r'Locomotion clip: (\w+)',prefix)[-1]=='walk';adb('shell','input','touchscreen','motionevent','UP',str(x),str(y));expected=oracle['walking_live_case'];words=expected['application_words'];source,attempt,result,before,after,dead,combo,seed,calls,statuses,armed,cue,hash_value=actual
  assert list(map(int,result.split()))==expected['result_after'] and [int(seed),int(calls)]==expected['random_after'] and list(map(int,(before,after,dead,combo,armed,cue)))==[words[1],words[2],expected['state_after'][5],expected['state_after'][2],words[4],words[5]] and int(statuses)==words[16] and hash_value==checksum(raw[at+13520:at+14416]);at+=14600;assert at==len(raw);walking_case={'result':list(map(int,result.split())),'hp_before':int(before),'hp_after':int(after),'dead':int(dead),'combo':int(combo),'random_after':[int(seed),int(calls)],'status_requests':int(statuses),'low_health_armed':int(armed),'low_health_cue':int(cue),'resolved_checksum':hash_value,'defender_fsm_state':13,'held_touch_and_walk_clip_verified':True};(a.output/'walking-damage.log').write_text(logs())
 finally:
  adb('shell','input','touchscreen','motionevent','CANCEL','0','0');adb('shell','cmd','window','user-rotation','lock','0');adb('shell','input','keyevent','KEYCODE_HOME')
 report={'apk_sha256':sha,'installed_apk_sha256':installed,'original_report_sha256':hashlib.sha256(a.oracle.read_bytes()).hexdigest(),'original_reference_sha256':hashlib.sha256(raw).hexdigest(),'pairs':pairs,'native_player_damage_attempts_verified':sum(len(r['attacks']) for r in pairs)+1,'walking_live_case':walking_case,'walking_damage_verified':True,'low_health_cues_verified':3,'automatic_player_deaths_verified':3,'death_clip':1023,'hud_health_mana_and_defeated_verified':True,'rotation_preserves_health_and_death':rotations,'inflight_pause_resume_verified':pause,'dead_player_attack_rejected':attack_blocked,'dead_player_movement_blocked':movement_blocked,'target_scope':'Explicit development target -2 means Prince; automatic enemy AI/acquisition and actual status/audio services pending.','physical_arm64_tested':False,'full_game_playable':False};(a.output/'player-defender-smoke.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='pairs'}),flush=True)

if __name__=='__main__':main()
