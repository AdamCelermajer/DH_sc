"""Actual authored melee events, persistent HP and queued original death clips."""
import argparse,hashlib,json,math,re,struct,subprocess,time,zipfile
import xml.etree.ElementTree as ET
from pathlib import Path
from PIL import Image
from emulator_smoke import inspect,launch_fresh
HIT=r'Native combat hit \| (\S+) \| target (\S+) \| hit (\d+) \| result ((?:-?\d+ ){9}-?\d+) \| HP (-?\d+) (-?\d+) \| dead (\d+) \| combo (\d+) \| RNG (\d+) (\d+) \| statuses (\d+) \| threat (\S+)'
def main():
 p=argparse.ArgumentParser();p.add_argument('--adb',required=True);p.add_argument('--serial',required=True);p.add_argument('--apk',type=Path,required=True);p.add_argument('--oracle',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();assert a.serial.startswith('emulator-');a.output.mkdir(parents=True,exist_ok=True);libraries=inspect(a.apk);oracle=json.loads(a.oracle.read_text());assert oracle['mismatches']==0 and len(oracle['live_sequences'])==3
 with zipfile.ZipFile(a.apk) as z:records=json.loads(z.read('assets/actor-provenance.json'))['records']
 def adb(*args,missing=False):
  r=subprocess.run([a.adb,'-s',a.serial,*args],capture_output=True,text=True,timeout=45)
  if missing and r.returncode==1 and not (r.stdout+r.stderr).strip():return ''
  if r.returncode:raise RuntimeError(r.stdout+r.stderr)
  return r.stdout.strip()
 def logs():
  pid=adb('shell','pidof','com.example.dh2',missing=True);assert pid,'App process exited';s=adb('logcat','-d','--pid='+pid,'-v','brief');assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|completion failed|event dispatch failed|combat route failed|application failed|death animation failed|Shader failed|Link failed',s),s;return s
 def wait(predicate,timeout=45):
  deadline=time.monotonic()+timeout
  while True:
   s=logs()
   if predicate(s):return s
   assert time.monotonic()<deadline,s[-10000:];time.sleep(.15)
 def command(index,state=None,target=None,ms=None):
  extra=['--ei','object_index',str(index)]
  if state is not None:extra+=['--es','object_state',state]
  if target is not None:extra+=['--ei','combat_target_index',str(target)]
  if ms is not None:extra+=['--ei','time_ms',str(ms)]
  before=logs().count('Actor command applied |');r=adb('shell','am','start','-W','--activity-single-top','-n','com.example.dh2/.MainActivity',*extra);assert 'Status: ok' in r,r;return wait(lambda s:s.count('Actor command applied |')>before)
 def capture(name):
  file=a.output/(name+'.png');deadline=time.monotonic()+8
  while True:
   adb('shell','uiautomator','dump','/sdcard/dh2-combat-ui.xml');tree=ET.fromstring(adb('shell','cat','/sdcard/dh2-combat-ui.xml'));node=next(n for n in tree.iter('node') if n.get('content-desc')=='DH2 native texture viewport');bounds=tuple(map(int,re.findall(r'\d+',node.get('bounds'))));adb('shell','screencap','-p','/sdcard/dh2-combat-test.png');adb('pull','/sdcard/dh2-combat-test.png',str(file));colors=Image.open(file).convert('RGB').crop(bounds).getcolors(4096)
   if colors is None or len(colors)>1024:return file.name
   assert time.monotonic()<deadline,'Native scene was not presented in the viewport';time.sleep(.1)
 def actual_hits(s):return re.findall(HIT,s)
 suites=[];rotations=[];backgrounds=[]
 try:
  adb('shell','input','touchscreen','motionevent','CANCEL','0','0');adb('shell','input','keyevent','KEYCODE_HOME');assert 'Success' in adb('install','-r',str(a.apk))
  remote=adb('shell','pm','path','com.example.dh2').removeprefix('package:');installed=adb('shell','sha256sum',remote).split()[0];sha=hashlib.sha256(a.apk.read_bytes()).hexdigest();assert installed==sha
  for scenario,reference in enumerate(oracle['live_sequences']):
   attacker=next((i,r) for i,r in enumerate(records) if r['character']==reference['attacker']);defender=next((i,r) for i,r in enumerate(records) if r['character']==reference['defender']);ai,ar=attacker;di,dr=defender;an,dn=ar['name'],dr['name'];adb('shell','input','touchscreen','motionevent','CANCEL','0','0')
   launch_fresh(adb,'--ez','enemy_ai','false','--es','world','crypt01.dwld','--ei','object_index',str(ai),'--es','object_state','Attack','--ei','combat_target_index',str(di),'--ei','time_ms','100');s=wait(lambda s:'Animation frame rendered at 100 ms' in s and f'Actor state selected | index {ai} |' in s);assert not actual_hits(s),s;capture(f'pair-{scenario}-frozen-no-hit')
   rows=[]
   for attack,expected in enumerate(reference['attacks']):
    command(ai,'Attack',di,-1);s=wait(lambda s:len(actual_hits(s))==attack+1);row=actual_hits(s)[-1];source,target,number,result,before,after,dead,combo,seed,calls,statuses,threat=row;assert source==an and target==dn and int(number)==attack+1;assert list(map(int,result.split()))==expected['result_after'],(scenario,attack,'result',row,expected)
    words=expected['application_words'];state=expected['state_after'];signed=lambda n:n if n<0x80000000 else n-0x100000000
    assert list(map(int,(before,after,dead,combo)))==[signed(words[1]),signed(words[2]),state[5],state[2]] and [int(seed),int(calls)]==expected['random_after'] and int(statuses)==words[16] and struct.unpack('<I',struct.pack('<f',float(threat)))[0]==words[14],(scenario,attack,'application',row,expected)
    rows.append({'result':list(map(int,result.split())),'hp_before':int(before),'hp_after':int(after),'dead':int(dead),'combo':int(combo),'random_after':[int(seed),int(calls)],'status_requests':int(statuses)})
    if scenario==0 and attack==1:
     adb('shell','input','keyevent','KEYCODE_HOME');assert 'Status: ok' in adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity');s=wait(lambda s:len(actual_hits(s))==2 and re.search(r'Actor clip completed \| '+re.escape(an)+r' \| state Attack \| completions 3 \| active 0',s[s.rfind('Actor state selected |'):]) is not None);time.sleep(.2);assert len(actual_hits(logs()))==2;backgrounds.append({'case':'inflight_attack_pause_resume','hits':2})
    else:s=wait(lambda s:re.search(r'Actor clip completed \| '+re.escape(an)+r' \| state Attack \| completions 3 \| active 0',s[s.rfind('Actor state selected |'):]) is not None)
    if scenario==0 and attack==0:
     for orientation in (1,0):
      before_ready=logs().count('World ready |');adb('shell','cmd','window','user-rotation','lock',str(orientation));s=wait(lambda s:s.count('World ready |')>before_ready and f'Combat actor restored | {dn} | HP {after} | dead 0 |' in s and 'Native combat resumed' in s);time.sleep(.2);assert len(actual_hits(logs()))==1,'Renderer recreation replayed a hit';rotations.append({'orientation':orientation,'target_hp':int(after),'hits':1})
   s=wait(lambda s:f'Combat death animation selected | {dn} |' in s and re.search(r'Actor clip completed \| '+re.escape(dn)+r' \| state Died \| completions 1 \| active 0',s) is not None);death_clip=int(re.findall(r'Combat death animation selected \| '+re.escape(dn)+r' \| clip (\d+) \| dead 1',s)[-1]);assert death_clip=={'CryptSlime':1251,'Crypt_Ghost':700,'Crypt_Skeleton':1185}[reference['defender']]
   rejected=command(ai,'Attack',di,-1);assert 'Combat target is dead' in rejected[rejected.rfind('Actor command applied |'):];time.sleep(.2);assert len(actual_hits(logs()))==len(reference['attacks']),'Dead target accepted another attack'
   s=command(di);assert re.search(r'Combat actor state \| '+re.escape(dn)+r' \| HP 0 \| MP 5632 \| dead 1 \| combo 0 \| state Died',s);picture=capture(f'pair-{scenario}-dead-target');(a.output/f'pair-{scenario}.log').write_text(logs());suites.append({'attacker_index':ai,'defender_index':di,'attacker':an,'defender':dn,'attacks':rows,'death_clip':death_clip,'death_screenshot':picture,'frozen_no_hit':True,'dead_target_rejected':True})
 finally:
  adb('shell','input','touchscreen','motionevent','CANCEL','0','0');adb('shell','cmd','window','user-rotation','lock','0');adb('shell','input','keyevent','KEYCODE_HOME')
 report={'apk_sha256':sha,'installed_apk_sha256':installed,'original_application_report_sha256':hashlib.sha256(a.oracle.read_bytes()).hexdigest(),'serial':a.serial,'api':adb('shell','getprop','ro.build.version.sdk'),'abi':adb('shell','getprop','ro.product.cpu.abi'),'libraries':libraries,'pairs':suites,'native_hits_verified':sum(len(s['attacks']) for s in suites),'frozen_attacks_do_not_hit':True,'dead_target_rejection_verified':True,'automatic_original_death_clips_verified':True,'rotation_preserves_hp_and_does_not_replay_hits':rotations,'inflight_pause_resume_does_not_replay_hits':backgrounds,'target_acquisition_scope':'Explicit development target indices; automatic AI/player targeting remains pending.','status_services_implemented':False,'full_kill_rewards_or_ai_fsm_implemented':False,'physical_arm64_tested':False,'full_game_playable':False}
 (a.output/'combat-application-smoke.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('pairs','libraries')}),flush=True)
if __name__=='__main__':main()
