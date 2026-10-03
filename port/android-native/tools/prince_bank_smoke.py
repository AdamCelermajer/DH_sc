"""Bounded full-Prince-bank emulator smoke; caller installs the named APK first.

Uses actual touch waypoints, source playback logs, whole-actor inspection pause,
and Activity recreation. No original full-AI/FSM, pose/GPU parity or phone claim.
"""
import argparse,hashlib,json,math,re,struct,subprocess,time,xml.etree.ElementTree as ET,zipfile
from pathlib import Path
from PIL import Image,ImageChops
from emulator_smoke import launch_fresh,inspect
from live_actor_smoke import PACKAGE,FLOAT,READY,WORLD,POSITION,BAD,digest,require,position,body_matches
from character_combat_smoke import TRANSITION

BANK=re.compile(r'Prince blended bank ready \| resources (\d+) \| occurrences (\d+) \| targets (\d+) \| template (-?\d+) \| engine (-?\d+) \| game clip (-?\d+)')
FRAME=re.compile(r'Native actor frame \| scene (\d+) \| Step (\d+) \| actor (\d+) \| source phase (\d+) \| clip (-?\d+) \| ms (-?\d+) \| replays (\d+) \| body ('+FLOAT+r') ('+FLOAT+r') \| contacts (\d+) (\d+) \| state (-?\d+) \| body present (\d+)(?: \| timeline scale ('+FLOAT+r'))?')
EVENT=re.compile(r'Blended character event \| event 0x([0-9a-fA-F]+) \| clip (-?\d+) \| slot (\d+) \| phase (\d+) \| lag (-?\d+)')
INSPECT=re.compile(r'Prince blended inspection \| frozen (\d+) \| scene (\d+) \| Step (\d+) \| actor (\d+) \| clip (-?\d+) \| ms (-?\d+) \| body ('+FLOAT+r') ('+FLOAT+r') \| pose ([0-9a-fA-F]{16})')
RESTART=re.compile(r'Player blended sequence restarted \| state (-?\d+) \| sequence (-?\d+) \| clip (-?\d+) \| engine (-?\d+) \| development recreation')
SELECTION=re.compile(r'Character animation selected \| state (-?\d+) \| flags ([0-9a-fA-F]+) \| sequence (-?\d+) \| clip (-?\d+) \| (scene callback|actor state service)')
MANIFEST_SHA='76633bb4f0ab5f645f4516e407e1f926df61641ea66faa805f68da57f043eb81'
BINARY_SHA='c0cf8bfbb804256050e0d1ac326b8388b472c9b3034c8366c992f2056f477b0b'

def f32(value):return struct.unpack('<f',struct.pack('<f',value))[0]
def f32_ulp(value):
 value=abs(f32(value));bits=struct.unpack('<I',struct.pack('<f',value))[0]
 return struct.unpack('<f',struct.pack('<I',bits+1))[0]-value
def printed_error(value,significant):
 # %.Ng emits nonzero finite floats in fixed/exponent form; zero is exact.
 return .5*10**(math.floor(math.log10(abs(value)))-significant+1) if value else 0.
def source_radius(extent):
 # character_body_config: (max XY extent * .01f) * .5f; READY prints *100.f.
 scaled=f32(f32(extent)*f32(.01));physical=f32(scaled*.5)
 return f32(physical*100)
def radius_facts(start,baseline):
 radius=float(start[3]);bounds=list(map(float,start.group(4,5,6,7)))
 require(math.isfinite(radius) and radius>0 and all(map(math.isfinite,bounds)) and bounds[0]<bounds[2] and bounds[1]<bounds[3],'Nonfinite/degenerate source radius bounds')
 errors=[printed_error(v,6) for v in bounds];extents=[bounds[i+2]-bounds[i] for i in range(2)];extent_errors=[errors[i+2]+errors[i] for i in range(2)]
 low=source_radius(max(extents[i]-extent_errors[i] for i in range(2)));high=source_radius(max(extents[i]+extent_errors[i] for i in range(2)));radius_error=printed_error(radius,9)
 require(low-radius_error<=radius<=high+radius_error,'Radius differs from source half-max XY extent within READY precision')
 base_radius=float(baseline[3]);base_bounds=list(map(float,baseline.group(4,5,6,7)));base_errors=[printed_error(v,6) for v in base_bounds]
 per_axis=[]
 for axis in range(2):
  rounding=0.;scaled_rounding=0.
  for coordinates,precision in ((bounds,errors),(base_bounds,base_errors)):
   # One translated float32 endpoint addition, then float32 hi-lo. Use the
   # larger neighboring ULP across the log's decimal interval at power edges.
   for j in (axis,axis+2):rounding+=.5*max(f32_ulp(coordinates[j]-precision[j]),f32_ulp(coordinates[j]+precision[j]))
   extent=coordinates[axis+2]-coordinates[axis];extent_hi=extent+precision[axis+2]+precision[axis]
   rounding+=.5*f32_ulp(extent_hi)
   scaled_rounding+=.5*f32_ulp(f32(extent_hi)*f32(.01))*.5*100
  per_axis.append(rounding*f32(.01)*.5*100+scaled_rounding)
 # Multiplication by .5 is exact for these normal floats. Final *100 and
 # decimal radius printing contribute their own half-ULP/half-decimal bounds.
 envelope=max(per_axis)+.5*f32_ulp(radius)+.5*f32_ulp(base_radius)+radius_error+printed_error(base_radius,9)
 require(abs(radius-base_radius)<=envelope,'Radius changed beyond float32 translation/subtraction envelope')
 return {'radius':radius,'source_bounds':bounds,'bounds_radius_interval':[low-radius_error,high+radius_error],'baseline_radius':base_radius,'baseline_delta':abs(radius-base_radius),'translation_rounding_envelope':envelope,'bounds_significant_digits':6,'radius_significant_digits':9}

def frame_rows(text):
 out=[]
 for m in FRAME.finditer(text):
  values=list(map(int,m.group(1,2,3,4,5,6,7)));r=dict(zip(('scene_ms','physics_steps','actor_frames','phase','clip','clip_ms','replays'),values));r.update(body_xy=list(map(float,m.group(8,9))),contacts=list(map(int,m.group(10,11))),state=int(m[12]),body_present=int(m[13]));require(r['phase']==5 and r['actor_frames']==r['physics_steps'] and r['actor_frames']>0,'Incomplete actor phase');require(all(map(math.isfinite,r['body_xy'])),'Nonfinite body')
  if m[14] is not None:r['timeline_scale']=float(m[14]);require(math.isfinite(r['timeline_scale']),'Nonfinite applied timeline scale')
  out.append(r)
 return out
def contexts(text):
 starts=list(READY.finditer(text));out=[]
 for i,start in enumerate(starts):
  require((int(start[1]),int(start[2]))==(83,82),'Native body ownership differs')
  if i==0:require(abs(float(start[3])-113.699707)<.00002,'Initial source spawn radius differs')
  radius=radius_facts(start,starts[0])
  chunk=text[start.end():starts[i+1].start() if i+1<len(starts) else len(text)];rows=frame_rows(chunk)
  if rows:require(rows[0]['actor_frames']==1,'Context counters did not reset')
  for previous,current in zip(rows,rows[1:]):require(current['actor_frames']>previous['actor_frames'] and current['scene_ms']>=previous['scene_ms'] and current['replays']>=previous['replays'],'Context clocks regressed')
  out.append({'index':i+1,'frames':rows,'radius_validation':radius})
 return out

def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--adb',required=True);p.add_argument('--serial',required=True);p.add_argument('--apk',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();require(a.serial.startswith('emulator-'),'Named development emulator required');a.output.mkdir(parents=True,exist_ok=True)
 report={'validation':'FAIL','apk_sha256':digest(a.apk),'serial':a.serial,'install_requested':False,'physical_arm64_phone_tested':False,'original_full_ai_verified':False,'original_full_frame_parity_verified':False,'full_game_playable':False};commands=[];last_logs='';prior=[];operated=False;held=False;frozen=False;deadline=time.monotonic()+240
 def adb(*args,missing=False,cleanup=False):
  remaining=deadline-time.monotonic();require(cleanup or remaining>0,'Bank smoke exceeded240-second budget');timeout=8 if cleanup else min(45,max(.1,remaining))
  try:r=subprocess.run([a.adb,'-s',a.serial,*args],capture_output=True,text=True,timeout=timeout)
  except subprocess.TimeoutExpired:commands.append({'args':list(args),'timeout_seconds':timeout});raise
  row={'args':list(args),'returncode':r.returncode,'stderr':r.stderr}
  if args and args[0]=='logcat':row.update(stdout_bytes=len(r.stdout.encode()),stdout_sha256=hashlib.sha256(r.stdout.encode()).hexdigest())
  else:row['stdout']=r.stdout
  commands.append(row)
  if missing and r.returncode==1 and not (r.stdout+r.stderr).strip():return ''
  require(r.returncode==0,'ADB command failed: '+str(args)+'\n'+r.stdout+r.stderr);return r.stdout.strip()
 def logs():
  nonlocal last_logs
  pid=adb('shell','pidof',PACKAGE,missing=True);require(pid,'Native app exited');require(not report.get('pid') or report['pid']==pid,'Native process changed');report['pid']=pid;last_logs=adb('logcat','-d','--pid='+pid,'-v','brief');require(not BAD.search(last_logs),'Native failure in logs');contexts(last_logs);return last_logs
 def latest(text):
  rows=contexts(text);require(rows and rows[-1]['frames'],'Native context/frame absent');return rows[-1]
 def wait(predicate,label,timeout=35):
  until=min(deadline,time.monotonic()+timeout)
  while True:
   text=logs()
   if predicate(text):return text
   require(time.monotonic()<until,'Timed out waiting for '+label);time.sleep(.15)
 def hierarchy():
  adb('shell','uiautomator','dump','/sdcard/dh2-prince-bank-window.xml');xml=adb('shell','cat','/sdcard/dh2-prince-bank-window.xml');root=ET.fromstring(xml);views={n.get('content-desc'):tuple(map(int,re.findall(r'-?\d+',n.get('bounds','')))) for n in root.iter('node') if n.get('content-desc')};require('Movement control' in views and 'DH2 native texture viewport' in views,'Native controls missing');v=views['DH2 native texture viewport'];pad=views['Movement control'];require(v[0]<=pad[0]<pad[2]<=v[2] and v[1]<=pad[1]<pad[3]<=v[3],'Movement pad outside viewport');return views,xml
 def capture(stem):
  views,xml=hierarchy();viewport=views['DH2 native texture viewport'];file=a.output/(stem+'.png');(a.output/(stem+'.xml')).write_text(xml,encoding='utf-8');adb('shell','screencap','-p','/sdcard/dh2-prince-bank.png');adb('pull','/sdcard/dh2-prince-bank.png',str(file))
  with Image.open(file) as image:crop=image.convert('RGB').crop(viewport)
  delta=ImageChops.difference(crop,Image.new('RGB',crop.size,(20,23,28))).convert('L').point(lambda v:255 if v>8 else 0);pixels=delta.histogram()[255];require(pixels>crop.width*crop.height*.2,'Blank native world viewport');report.setdefault('screenshots',[]).append({'path':file.name,'sha256':digest(file),'viewport':list(viewport),'rendered_pixels':pixels});return crop
 def axis(bounds,x,y,action='DOWN'):
  nonlocal held
  left,top,right,bottom=bounds;adb('shell','input','touchscreen','motionevent',action,str(round((left+right)/2+x*(right-left)*.44)),str(round((top+bottom)/2-y*(right-left)*.44)));held=action!='UP'
 def zero(bounds):
  n=len(POSITION.findall(logs()));axis(bounds,0,0);axis(bounds,0,0,'UP');return position(wait(lambda t:len(POSITION.findall(t))>n,'authoritative position'))
 def idle_after(step):return wait(lambda t:latest(t)['frames'][-1]['actor_frames']>step and latest(t)['frames'][-1]['state']==3 and latest(t)['frames'][-1]['clip'] in (1040,1041),'authored Idle')
 def inspect_time(ms):
  nonlocal frozen
  n=len(INSPECT.findall(logs()));before=latest(last_logs)['index'];result=adb('shell','am','broadcast','-a',PACKAGE+'.DEBUG_ANIMATION_TIME','-p',PACKAGE,'--ei','time_ms',str(ms));require('Broadcast completed: result=0' in result,'Debug inspection broadcast failed');text=wait(lambda t:len(INSPECT.findall(t))>n,'inspection snapshot');require(latest(text)['index']==before,'Debug inspection unexpectedly recreated native context');row=INSPECT.findall(text)[-1];frozen=ms>=0;require(int(row[0])==int(frozen),'Inspection pause state differs');return list(row)
 def travel(index,destination,bounds):
  speed=None
  for _ in range(40):
   origin=zero(bounds);delta=destination-origin['game_xyz'][index]
   if abs(delta)<35:return origin
   duration=.2 if speed is None else min(.6,max(.035,abs(delta)/speed*.65));axes=[0.,0.];axes[index]=.9 if delta>0 else -.9;n=len(POSITION.findall(logs()));axis(bounds,*axes);time.sleep(duration);axis(bounds,0,0,'UP');text=wait(lambda t:len(POSITION.findall(t))>n,'waypoint release');step=latest(text)['frames'][-1]['actor_frames'];idle_after(step);point=zero(bounds);moved=abs(point['game_xyz'][index]-origin['game_xyz'][index]);require(moved>.1,'Touch waypoint blocked');speed=moved/duration;report.setdefault('waypoint_movement',[]).append({'axis':index,'origin':origin,'end':point,'held_seconds':duration})
  raise AssertionError('Actual touch waypoint not reached')
 try:
  report['libraries']=inspect(a.apk)
  with zipfile.ZipFile(a.apk) as z:
   raw=z.read('assets/data/prince-animation-bank.json');binary=z.read('assets/data/prince-animation-bank.bin');require(hashlib.sha256(raw).hexdigest()==MANIFEST_SHA and hashlib.sha256(binary).hexdigest()==BINARY_SHA,'Packaged bank metadata differs');m=json.loads(raw);require(len(m['resources'])==116 and len(m['registration_requests'])==158 and m['template_clip_id']==1111,'Bank counts differ')
   for r in m['resources']:data=z.read('assets/'+r['asset']);require(len(data)==r['bytes'] and hashlib.sha256(data).hexdigest()==r['sha256'],'Packaged fullbank resource differs: '+r['asset'])
  report.update(bank_manifest_sha256=MANIFEST_SHA,bank_binary_sha256=BINARY_SHA,packaged_resource_count=116,packaged_registration_count=158)
  paths=adb('shell','pm','path',PACKAGE).splitlines();require(len(paths)==1 and paths[0].startswith('package:'),'Single installed APK required');report['installed_apk_sha256']=adb('shell','sha256sum',paths[0][8:]).split()[0];require(report['installed_apk_sha256']==report['apk_sha256'],'Installed APK differs; install supplied artifact before smoke')
  prior=adb('shell','cmd','window','user-rotation').split();require(prior and prior[0] in ('lock','free'),'Cannot preserve rotation policy');report['original_rotation_policy']=prior;operated=True;adb('shell','input','touchscreen','motionevent','CANCEL','0','0');adb('shell','cmd','window','user-rotation','lock','0');launch_fresh(adb,'--es','world','crypt01.dwld','--ez','enemy_ai','false')
  text=wait(lambda t:BANK.search(t) and WORLD.search(t) and contexts(t) and contexts(t)[-1]['frames'],'full bank and first actor frame');require(all(tuple(map(int,row.groups()))==(116,158,83,1111,0,1111) for row in BANK.finditer(text)),'Source library0/default binding differs');report['bank_ready']=dict(zip(('resources','occurrences','targets','template','engine','game_clip'),map(int,BANK.search(text).groups())));require(re.search(r'AI tables ready .*automatic melee 0',text),'Independent enemy AI was not disabled');initial=latest(text)['frames'][-1];require(initial['state']==3 and initial['clip'] in (1040,1041),'Initial authored Idle absent');report['initial_frame']=initial;capture('idle-full-bank');bounds=hierarchy()[0]['Movement control'];report['movement']=[]
  for name,length,clip in (('Walk',.5,1126),('Run',.95,1114)):
   origin=zero(bounds);before=latest(logs())['frames'][-1];n=len(POSITION.findall(last_logs));axis(bounds,0,-length)
   text=wait(lambda t:any(f['clip']==clip and f['actor_frames']>before['actor_frames'] and math.dist(before['body_xy'],f['body_xy'])>.0005 for f in latest(t)['frames']),name+' measured body displacement')
   moving=[f for f in latest(text)['frames'] if f['clip']==clip and f['actor_frames']>before['actor_frames'] and math.dist(before['body_xy'],f['body_xy'])>.0005][-1]
   requested=280 if name=='Walk' else 271
   require(any(int(r[0])==4 and int(r[2])==requested for r in SELECTION.findall(text)),'Source '+name+' sequence request absent')
   require(abs(moving.get('timeline_scale',float('inf'))-1.3)<.000001,'Applied '+name+' timeline scale missing or wrong')
   axis(bounds,0,0,'UP');wait(lambda t:len(POSITION.findall(t))>n,'movement release');idle_text=idle_after(moving['actor_frames']);idle=latest(idle_text)['frames'][-1];settled=zero(bounds);body_matches(idle,settled['game_xyz'])
   require(math.dist(origin['game_xyz'][:2],settled['game_xyz'][:2])>.05 and settled['moved_frames']>origin['moved_frames'],name+' did not move')
   report['movement'].append({'state':name,'requested_sequence':requested,'clip':clip,'timeline_speed':moving['timeline_scale'],'speed_observation':'applied actor frame','origin':origin,'settled':settled,'moving_frame':moving,'idle_frame':idle});capture(name.lower()+'-released')
  event_rows=list(EVENT.finditer(logs()));require({int(e[3]) for e in event_rows}=={0,1},'Both playback slots were not observed during selection transitions');require(all(int(e[2]) in m['first_unique_resource_order'] and int(e[4]) in (1,2,3,4,5) for e in event_rows),'Invalid blended event identity/phase');report['observed_slots']=[0,1]
  first=inspect_time(0);frozen_image=capture('inspection-frozen');time.sleep(.3);second=inspect_time(0);repeat_image=capture('inspection-frozen-repeat');require(first==second,'Frozen actor clocks/body/pose changed');require(frozen_image.size==repeat_image.size and ImageChops.difference(frozen_image,repeat_image).getbbox() is None,'Repeated frozen viewport changed');report['freeze_snapshots']=[first,second];resumed=inspect_time(-1);require(first[1:]==resumed[1:],'Resume advanced the actor before snapshot');step=int(resumed[3]);text=wait(lambda t:latest(t)['frames'][-1]['actor_frames']>step and latest(t)['frames'][-1]['scene_ms']>int(resumed[1]),'resumed full actor clocks');report['resume_frame']=latest(text)['frames'][-1]
  # Existing reviewed Crypt test setup: these are development touch waypoints,
  # never an injected position or a claim of original target/path selection.
  for index,destination in ((1,-1000),(0,-1390),(1,-370)):travel(index,destination,bounds)
  offset=len(logs());adb('shell','am','broadcast','-a','com.example.dh2.DEBUG_PLAYER_ATTACK','-p',PACKAGE,'--ei','player_target_index','4');text=wait(lambda t:'Player attack selected |' in t[offset:] and any(int(r[2])==5 for r in TRANSITION.finditer(t[offset:])),'actual source Attack selection');capture('attack-started');text=wait(lambda t:any(int(r[1])==5 and int(r[2])==3 and int(r[3],0)==0x22 for r in TRANSITION.finditer(t[offset:])),'finite Attack closure',45);attack_events=[e.groups() for e in EVENT.finditer(text[offset:])];require(any(int(e[0],16)==0x28 for e in attack_events) and any(int(e[0],16)==0x22 for e in attack_events),'Authored trigger/finite closure absent');report['attack_events']=[list(e) for e in attack_events];capture('attack-finite-idle')
  before=latest(logs());saved=zero(bounds);selections=[r for r in SELECTION.findall(last_logs) if int(r[0])==3];require(selections,'Saved Idle sequence marker absent');saved_sequence=int(selections[-1][2]);offset=len(last_logs);adb('shell','cmd','window','user-rotation','lock','1');text=wait(lambda t:contexts(t) and contexts(t)[-1]['index']>before['index'] and contexts(t)[-1]['frames'] and any(int(r[0])==3 for r in SELECTION.findall(t[offset:])),'Activity recreation and source Idle focus');recreated=latest(text);row=[r for r in SELECTION.findall(text[offset:]) if int(r[0])==3][-1];require(int(row[2])==saved_sequence and int(row[3]) in (1040,1041) and int(row[1],16)==0x2380 and row[4]=='actor state service' and recreated['frames'][0]['actor_frames']==1,'Recreation did not reselect saved Idle sequence');capture('landscape-recreated');bounds=hierarchy()[0]['Movement control'];restored=zero(bounds);body_matches(recreated['frames'][-1],restored['game_xyz']);require(math.dist(saved['game_xyz'],restored['game_xyz'])<.05,'Recreation changed player position');report['recreation']={'restart':[int(row[0]),int(row[2]),int(row[3])],'restart_source':'Character animation selected: source Idle focus, not a second explicit restart','flags':row[1],'service':row[4],'saved_sequence':saved_sequence,'previous_context':before['index'],'new_context':recreated['index'],'first_frame':recreated['frames'][0],'saved':saved,'restored':restored}
  text=logs();require(all(tuple(map(int,row.groups()))==(116,158,83,1111,0,1111) for row in BANK.finditer(text)),'Recreated bank binding differs');report.update(validation='PASS',bank_ready_markers=len(BANK.findall(text)),blended_event_markers=len(EVENT.findall(text)),context_count=len(contexts(text)),context_radius_checks=[{'context':r['index'],**r['radius_validation']} for r in contexts(text)],actor_phase=5,freeze_body_pose_clocks_preserved=True,resume_advances=True,touch_movement_verified=True,finite_attack_closure_observed=True,scope='Fullbank native renderer runtime/log smoke only. Touch/waypoints/camera/target and Activity recreation are development adapters; separate source audits establish instruction behavior. No full AI, original complete frame/blended-pose/GPU or physical ARM64 phone parity claim.')
 except Exception as e:report['error']=str(e);raise
 finally:
  if operated:
   try:adb('shell','input','touchscreen','motionevent','CANCEL','0','0',cleanup=True)
   except Exception as e:report['touch_cleanup_error']=str(e)
  if frozen:
   try:adb('shell','am','broadcast','-a',PACKAGE+'.DEBUG_ANIMATION_TIME','-p',PACKAGE,'--ei','time_ms','-1',cleanup=True)
   except Exception as e:report['freeze_cleanup_error']=str(e)
  if prior:
   try:adb('shell','cmd','window','user-rotation',*prior,cleanup=True)
   except Exception as e:report['rotation_cleanup_error']=str(e)
  report['cleanup_complete']=not any(k.endswith('cleanup_error') for k in report)
  if not report['cleanup_complete']:report['validation']='FAIL'
  (a.output/'prince-bank.log').write_text(last_logs+'\n',encoding='utf-8');(a.output/'adb-transcript.json').write_text(json.dumps(commands,indent=2)+'\n',encoding='utf-8');(a.output/'prince-bank-smoke.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
 require(report['validation']=='PASS','Smoke cleanup failed');print(json.dumps({k:v for k,v in report.items() if k not in ('libraries','screenshots','attack_events','waypoint_movement')},indent=2))
if __name__=='__main__':main()
