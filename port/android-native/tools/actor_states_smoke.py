"""Exercise original actor state clips and completion on the development emulator."""
import argparse,hashlib,json,re,struct,subprocess,time,xml.etree.ElementTree as ET
from pathlib import Path
from PIL import Image,ImageChops
from emulator_smoke import inspect,launch_fresh
def main():
 p=argparse.ArgumentParser();p.add_argument('--adb',required=True);p.add_argument('--serial',required=True);p.add_argument('--apk',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--class-report',type=Path);p.add_argument('--property-report',type=Path);p.add_argument('--vitals-report',type=Path);p.add_argument('--event-report',type=Path);p.add_argument('--combat-event-report',type=Path);p.add_argument('--combat-result-report',type=Path);p.add_argument('--health-report',type=Path);a=p.parse_args();assert a.serial.startswith('emulator-');a.output.mkdir(parents=True,exist_ok=True);libraries=inspect(a.apk)
 with __import__('zipfile').ZipFile(a.apk) as z:records=json.loads(z.read('assets/actor-provenance.json'))['records']
 expected_events={}
 if a.event_report:
  oracle=json.loads(a.event_report.read_text());assert oracle['mismatches']==0
  authored={r['file']:r for r in oracle['authored_tracks']}
  with __import__('zipfile').ZipFile(a.apk) as z:
   for clip in json.loads(z.read('assets/actor-state-provenance.json'))['inputs']:
    name=Path(clip['asset']).name
    if name in authored:
     row=authored[name];assert hashlib.sha256(z.read('assets/'+clip['asset'])).hexdigest()==row['sha256'];expected_events[clip['clip_id']]=row
  assert len(expected_events)==5
 targets={model:next((i,r['name']) for i,r in enumerate(records) if r['model']==model) for model in ('skeleton.bdae','slime_green_v2.bdae','ghost.bdae')};cases=[];attacks=[];deaths=[];changes={};loaded=False
 expected_classes={r['character']:r for r in json.loads(a.class_report.read_text())['crypt_base_snapshots']} if a.class_report else {}
 expected_properties={r['character']:r for r in json.loads(a.property_report.read_text())['crypt_resolved_snapshots']} if a.property_report else {}
 expected_vitals={r['character']:r for r in json.loads(a.vitals_report.read_text())['crypt_spawn_snapshots']} if a.vitals_report else {}
 route_oracle=json.loads(a.combat_event_report.read_text()) if a.combat_event_report else None
 if route_oracle:
  assert route_oracle['mismatches']==0
  fixtures={(r['sequence_step'],r['can_range'],r['name']) for r in route_oracle['runtime_cases'] if r['state']==5 and r['clip_step']==1}
  assert {(sequence,capability,event) for sequence in (0,1) for capability in (0,1) for event in ('attack_mainhand','attack_offhand','attack_ranged','do_skill')}<=fixtures
 result_oracle=json.loads(a.combat_result_report.read_text()) if a.combat_result_report else None
 if result_oracle:assert result_oracle['mismatches']==0 and result_oracle['all_ten_result_words_and_rng_state_compared']
 health_oracle=json.loads(a.health_report.read_text()) if a.health_report else None
 if health_oracle:assert health_oracle['mismatches']==0 and health_oracle['all_four_property_sheets_and_health_requests_compared']
 def health_probes(s):
  if not health_oracle:return s
  expected={(r['character'],r['damage_raw']):r['change'] for r in health_oracle['source_snapshots']};marker='Property rules ready | defaults row 0 | types row 1 | properties 224 | supplied sheets only'
  rows=re.findall(r'Health probe \| (\S+) \| character (\S+) \| damage (\d+) \| add (-?\d+) \| before (-?\d+) \| after (-?\d+) \| kill (\d+) \| armed (\d+) \| cue (\d+) \| lifecycle (-?\d+) \| dead skip (\d+) \| validation only',s[s.rfind(marker):]);assert len(rows)==22,len(rows)
  authored={r['name']:r for r in records if r['kind']==1};assert {(r[0],int(r[2])) for r in rows}=={(name,damage) for name in authored for damage in (256,16384)}
  for name,character,damage,*values in rows:assert authored[name]['character']==character and list(map(int,values))==expected[character,int(damage)],(name,values)
  return s
 def combat_probes(s):
  if not result_oracle:return health_probes(s)
  expected={r['character']:r for r in result_oracle['crypt_unarmed_melee_snapshots']};marker='Property rules ready | defaults row 0 | types row 1 | properties 224 | supplied sheets only'
  rows=re.findall(r'Combat result probe \| (\S+) \| character (\S+) \| amount (-?\d+) \| dot element (-?\d+) \| dot duration (-?\d+) \| dot amount (-?\d+) \| HP leech (-?\d+) \| MP leech (-?\d+) \| outcomes (\d+) \| mask (\d+) \| category (-?\d+) \| element (-?\d+) \| seed (\d+) \| calls (\d+) \| validation only',s[s.rfind(marker):]);assert len(rows)==11,len(rows)
  authored={r['name']:r for r in records if r['kind']==1};assert {r[0] for r in rows}==set(authored)
  for name,character,*values in rows:
   reference=expected[character];assert authored[name]['character']==character and list(map(int,values))==reference['result']+reference['random_after'],(name,values,reference)
  return health_probes(s)
 def vitals(s):
  if not a.vitals_report:return combat_probes(s)
  marker='Property rules ready | defaults row 0 | types row 1 | properties 224 | supplied sheets only';assert marker in s
  rows=re.findall(r'Spawn vitals \| (\S+) \| character (\S+) \| HP raw (-?\d+) \| Max_HP raw (-?\d+) \| MP raw (-?\d+) \| Max_MP raw (-?\d+) \| first HP add raw (-?\d+) \| first MP add raw (-?\d+) \| second HP add raw (-?\d+) \| second MP add raw (-?\d+) \| checksum ([0-9a-f]{16}) \| passes 2',s[s.rfind(marker):]);assert len(rows)==11,len(rows)
  authored={r['name']:r for r in records if r['kind']==1};assert {r[0] for r in rows}==set(authored)
  for name,character,*values in rows:
   assert authored[name]['character']==character;expected=expected_vitals[character]
   assert tuple(int(v) for v in values[:8])==tuple(expected[k] for k in ('hp_raw','max_hp_raw','mp_raw','max_mp_raw'))+tuple(c[0] for c in expected['changes']) and values[8]==expected['resolved_sheet_fnv1a64'],(name,values,expected)
  return combat_probes(s)
 def properties(s):
  if not a.property_report:return vitals(s)
  marker='Property rules ready | defaults row 0 | types row 1 | properties 224 | supplied sheets only';assert marker in s
  rows=re.findall(r'Resolved properties \| (\S+) \| character (\S+) \| HP raw (-?\d+) \| Max_HP raw (-?\d+) \| MP raw (-?\d+) \| Max_MP raw (-?\d+) \| checksum ([0-9a-f]{16}) \| supplied sheets only',s[s.rfind(marker):]);assert len(rows)==11,len(rows)
  authored={r['name']:r for r in records if r['kind']==1};assert {r[0] for r in rows}==set(authored)
  for name,character,hp,maxhp,mp,maxmp,checksum in rows:
   assert authored[name]['character']==character;expected=expected_properties[character]
   assert (int(hp),int(maxhp),int(mp),int(maxmp),checksum)==tuple(expected[k] for k in ('hp_raw','max_hp_raw','mp_raw','max_mp_raw','resolved_sheet_fnv1a64')),(name,expected)
  return vitals(s)
 def classes(s):
  if not a.class_report:return properties(s)
  marker='Class tables ready | classes 260 | bytes 34224 | cached base snapshots';assert marker in s
  rows=re.findall(r'Base class snapshot \| (\S+) \| character (\S+) \| class (-?\d+) \| level raw (-?\d+) \| Max_HP raw (-?\d+) \| Max_MP raw (-?\d+) \| checksum ([0-9a-f]{16}) \| cached only',s[s.rfind(marker):]);assert len(rows)==11,len(rows)
  authored={r['name']:r for r in records if r['kind']==1};assert {r[0] for r in rows}==set(authored)
  for name,character,id,level,hp,mp,checksum in rows:
   assert authored[name]['character']==character;expected=expected_classes[character]
   assert (int(id),int(level),int(hp),int(mp),checksum)==tuple(expected[k] for k in ('class_id','level_raw','max_hp_raw','max_mp_raw','base_sheet_fnv1a64')),(name,expected)
  return properties(s)
 def adb(*args,missing=False):
  r=subprocess.run([a.adb,'-s',a.serial,*args],capture_output=True,text=True,timeout=45)
  if missing and r.returncode==1 and not (r.stdout+r.stderr).strip():return ''
  if r.returncode:raise RuntimeError(r.stdout+r.stderr)
  return r.stdout.strip()
 def logs():
  pid=adb('shell','pidof','com.example.dh2',missing=True);assert pid,'App process exited';s=adb('logcat','-d','--pid='+pid,'-v','brief');assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|completion failed|event dispatch failed|combat route failed|Shader failed|Link failed',s),s;return s
 def wait(predicate,timeout=60):
  end=time.monotonic()+timeout
  while True:
   s=logs()
   if predicate(s):return s
   assert time.monotonic()<end,s;time.sleep(.15)
 def launch(index,state,ms=None):
  nonlocal loaded
  adb('shell','input','touchscreen','motionevent','CANCEL','0','0');extras=['--es','world','crypt01.dwld','--ei','object_index',str(index),'--es','object_state',state]
  if ms is not None:extras+=['--ei','time_ms',str(ms)]
  if not loaded:
   launch_fresh(adb,'--ez','enemy_ai','false',*extras);loaded=True
   return classes(wait(lambda s:f'Actor state selected | index {index} |' in s and 'state '+state+' |' in s and 'Model frame submitted at' in s and (ms is None or f'Animation frame rendered at {ms} ms' in s)))
  before=logs().count('Actor command applied |');result=adb('shell','am','start','-W','--activity-single-top','-n','com.example.dh2/.MainActivity',*extras);assert 'Status: ok' in result,result
  return classes(wait(lambda s:s.count('Actor command applied |')>before and f'Actor command applied | index {index} | state {state} | time '+str(-1 if ms is None else ms) in s and (ms is None or s.rfind(f'Animation frame rendered at {ms} ms')>s.rfind('Actor command applied |'))))
 def capture(stem,index,state):
  adb('shell','uiautomator','dump','/sdcard/dh2-actor-ui.xml');tree=ET.fromstring(adb('shell','cat','/sdcard/dh2-actor-ui.xml'));node=next(n for n in tree.iter('node') if n.get('content-desc')=='DH2 native texture viewport');bounds=tuple(map(int,re.findall(r'\d+',node.get('bounds'))));file=a.output/(stem+'.png');adb('shell','screencap','-p','/sdcard/dh2-actor.png');adb('pull','/sdcard/dh2-actor.png',str(file));pic=Image.open(file).convert('RGB').crop(bounds);cases.append({'screenshot':file.name,'index':index,'state':state,'bounds':bounds});return pic
 def events(s,name,state):return [(int(c),int(active),int(clip),int(layers)) for c,active,clip,layers in re.findall(r'Actor clip completed \| '+re.escape(name)+r' \| state '+state+r' \| completions (\d+) \| active (\d+) \| next clip (\d+) \| layers (\d+)',s)]
 def animation_events(s,name,state):
  rows=[(int(clip),event,int(lag),int(ms),int(count)) for clip,event,lag,ms,count in re.findall(r'Actor animation event \| '+re.escape(name)+r' \| state '+state+r' \| clip (\d+) \| name (\S+) \| lag ms (-?\d+) \| time ms (-?\d+) \| count (\d+)',s[s.rfind('Actor command applied |'):])]
  if a.event_report:
   assert len(rows)==(1 if state=='Attack' else 0),(name,state,rows)
   for clip,event,lag,ms,count in rows:
    row=expected_events[clip];assert row['names']==[['attack_mainhand']] and event=='attack_mainhand' and count==1
    f32=lambda x:struct.unpack('<f',struct.pack('<f',x))[0]
    stamp=row['keys'][0];expected=int(f32(f32(ms)-f32(stamp))) if row['type']==4 else int(f32(f32(ms)+f32(f32(stamp)*f32(-33.333332061767578125))))
    assert lag==expected and 0<=lag<=100,(name,rows,expected)
  return rows
 def combat_actions(s,name,state):
  rows=[(int(clip),event,*map(int,(kind,sequence,step,offhand,capability,projectile))) for clip,event,kind,sequence,step,offhand,capability,projectile in re.findall(r'Actor combat action \| '+re.escape(name)+r' \| state '+state+r' \| clip (\d+) \| name (\S+) \| kind (\d+) \| sequence (-?\d+) \| attack step (-?\d+) \| offhand (\d+) \| capability (\d+) \| projectile (-?\d+) \| execution pending',s[s.rfind('Actor command applied |'):])]
  if route_oracle:
   assert len(rows)==(1 if state=='Attack' else 0),(name,state,rows)
   for clip,event,kind,sequence,step,offhand,capability,projectile in rows:
    assert capability==0 and projectile==-1 and event=='attack_mainhand'
    original=next(r for r in route_oracle['runtime_cases'] if r['state']==5 and r['name']==event and r['sequence_step']==sequence and r['clip_step']==1 and r['can_range']==capability)
    assert [kind,sequence,step,offhand]==original['action'],(rows,original)
  return rows
 prior=adb('shell','cmd','window','user-rotation').split()
 try:
  assert 'Success' in adb('install','-r',str(a.apk));adb('shell','cmd','window','user-rotation','lock','0')
  for model,(index,name) in targets.items():
   for state in ('Walk','Attack','Died'):
    pictures=[]
    for ms in (100,700):
     frozen_log=launch(index,state,ms)
     if route_oracle:
      latest=frozen_log[frozen_log.rfind('Actor command applied |'):];assert not re.search(r'Actor (animation event|combat action) \| '+re.escape(name)+r' \|',latest)
     pictures.append(capture(model.removesuffix('.bdae')+'-'+state+'-'+str(ms),index,state))
    diff=ImageChops.difference(*pictures).convert('L').point(lambda v:255 if v>3 else 0).histogram()[255];assert diff>100,(model,state,diff);changes[model+':'+state]=diff
   launch(index,'Attack');s=wait(lambda s:events(s[s.rfind('Actor command applied |'):],name,'Attack') and events(s[s.rfind('Actor command applied |'):],name,'Attack')[-1][1]==0);played=events(s[s.rfind('Actor command applied |'):],name,'Attack');assert [e[0] for e in played]==[1,2,3] and played[-1][3]==0,(model,played)
   assert 'state Idle |' in s,'Other actor idle progression absent';capture(model.removesuffix('.bdae')+'-Attack-finished',index,'Attack');attacks.append({'model':model,'object_index':index,'events':played,'animation_events':animation_events(s,name,'Attack'),'combat_actions':combat_actions(s,name,'Attack')});(a.output/(model.removesuffix('.bdae')+'-attack.log')).write_text(s+'\n')
   launch(index,'Died');s=wait(lambda s:events(s[s.rfind('Actor command applied |'):],name,'Died') and events(s[s.rfind('Actor command applied |'):],name,'Died')[-1][1]==0);played=events(s[s.rfind('Actor command applied |'):],name,'Died');assert len(played)==1 and played[0][0]==1 and played[0][3]==0;(a.output/(model.removesuffix('.bdae')+'-death.log')).write_text(s+'\n');capture(model.removesuffix('.bdae')+'-Died-finished',index,'Died');deaths.append({'model':model,'object_index':index,'events':played,'animation_events':animation_events(s,name,'Died'),'combat_actions':combat_actions(s,name,'Died')})
  index,name=targets['slime_green_v2.bdae'];launch(index,'Idle');s=wait(lambda s:len(events(s[s.rfind('Actor command applied |'):],name,'Idle'))>=6);played=events(s[s.rfind('Actor command applied |'):],name,'Idle');assert all(e[1]==1 for e in played);(a.output/'idle-independent.log').write_text(s+'\n');assert len(set(re.findall(r'Actor clip completed \| (\S+) \| state Idle',s)))>=5
 finally:
  adb('shell','input','touchscreen','motionevent','CANCEL','0','0')
  if prior and prior[0] in ('free','lock'):adb('shell','cmd','window','user-rotation',*prior)
 report={'apk_sha256':hashlib.sha256(a.apk.read_bytes()).hexdigest(),'serial':a.serial,'api':adb('shell','getprop','ro.build.version.sdk'),'abi':adb('shell','getprop','ro.product.cpu.abi'),'libraries':libraries,'cases':cases,'pose_changed_pixels':changes,'three_stage_attacks':attacks,'one_stage_deaths':deaths,'independent_idle_instances_verified':True,'physical_arm64_tested':False,'combat_or_ai_implemented':False,'blending_or_root_motion_dispatch_implemented':False}
 report.update({'authored_animation_events_verified':bool(a.event_report),'original_event_report_sha256':hashlib.sha256(a.event_report.read_bytes()).hexdigest() if a.event_report else None,'actor_animation_event_tracks':len(expected_events)})
 report.update({'state5_attack_event_decisions_verified':bool(route_oracle),'frozen_attack_events_absent_verified':bool(route_oracle),'original_combat_event_report_sha256':hashlib.sha256(a.combat_event_report.read_bytes()).hexdigest() if route_oracle else None,'combat_execution_implemented':True,'combat_scope':'Supplied development targets, offline nonplayer health/core kill application. Full AI/status services pending.'})
 report.update({'packaged_combat_result_probes_verified':bool(result_oracle),'combat_result_probe_count':11 if result_oracle else 0,'original_combat_result_report_sha256':hashlib.sha256(a.combat_result_report.read_bytes()).hexdigest() if result_oracle else None})
 report.update({'packaged_health_probes_verified':bool(health_oracle),'health_probe_count':22 if health_oracle else 0,'original_health_report_sha256':hashlib.sha256(a.health_report.read_bytes()).hexdigest() if health_oracle else None,'live_combat_health_application_implemented':True,'full_combat_or_ai_implemented':False})
 report.update({'base_class_snapshots_verified':bool(a.class_report),'base_class_instance_count':11 if a.class_report else 0,'original_class_report_sha256':hashlib.sha256(a.class_report.read_bytes()).hexdigest() if a.class_report else None,'resolved_property_snapshots_verified':bool(a.property_report),'resolved_property_instance_count':11 if a.property_report else 0,'original_property_report_sha256':hashlib.sha256(a.property_report.read_bytes()).hexdigest() if a.property_report else None,'spawn_vitals_verified':bool(a.vitals_report),'spawn_vitals_instance_count':11 if a.vitals_report else 0,'original_vitals_report_sha256':hashlib.sha256(a.vitals_report.read_bytes()).hexdigest() if a.vitals_report else None});(a.output/'actor-states-smoke.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('cases','libraries')}))
if __name__=='__main__':main()
