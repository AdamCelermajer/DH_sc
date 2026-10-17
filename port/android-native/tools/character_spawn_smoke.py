"""Test the bounded first-spawn Crypt adapter on an explicit 16 KiB emulator.

The authored hallway trigger, autonomous AI and full-game playability are not
claimed. APK installation is left to the caller so its identity stays explicit.
"""
import argparse,hashlib,json,re,subprocess,time,zipfile
from pathlib import Path
from emulator_smoke import launch_fresh,inspect

PACKAGE='com.example.dh2'
NAMES=('_prim_Monster_SURPRISE_01','_prim_Monster_SURPRISE_02')
BAD=re.compile(r'(?:FATAL EXCEPTION|Fatal signal|Native frame failed|World load failed|SpawnCharacter failed|Model draw GL error)')

def main():
 p=argparse.ArgumentParser(description=__doc__)
 p.add_argument('--adb',required=True);p.add_argument('--serial',required=True)
 p.add_argument('--apk',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
 a=p.parse_args();assert a.serial.startswith('emulator-'),'explicit emulator required'
 a.output.mkdir(parents=True,exist_ok=True)
 report={'validation':'FAIL','apk_sha256':hashlib.sha256(a.apk.read_bytes()).hexdigest(),
  'serial':a.serial,'full_game_playable':False,'authored_trigger_executed':False,
  'autonomous_gated_actor_ai_verified':False,'physical_arm64_phone_tested':False}
 commands=[];last_logs='';prior=[];deadline=time.monotonic()+150
 def adb(*args):
  if time.monotonic()>deadline:raise RuntimeError('spawn smoke deadline exceeded')
  run=subprocess.run([a.adb,'-s',a.serial,*args],capture_output=True,text=True,timeout=30)
  commands.append({'args':list(args),'returncode':run.returncode,'stderr':run.stderr,
   'stdout':run.stdout if args[0]!='logcat' else '[stored in spawn.log]'})
  if run.returncode:raise RuntimeError('ADB failed: '+str(args)+' '+run.stderr)
  return run.stdout.strip()
 def logs():
  nonlocal last_logs
  pid=adb('shell','pidof',PACKAGE);assert pid,'app exited'
  assert not report.get('pid') or report['pid']==pid,'process changed'
  report['pid']=pid;last_logs=adb('logcat','-d','--pid='+pid,'-v','brief')
  assert not BAD.search(last_logs),'native failure: '+str(BAD.findall(last_logs))
  return last_logs
 def wait(predicate,label):
  until=min(deadline,time.monotonic()+35)
  while time.monotonic()<until:
   text=logs()
   if predicate(text):return text
   time.sleep(.15)
  raise AssertionError(label+' not observed')
 def capture(name):
  path=a.output/(name+'.png')
  run=subprocess.run([a.adb,'-s',a.serial,'exec-out','screencap','-p'],capture_output=True,timeout=30)
  assert run.returncode==0 and run.stdout.startswith(b'\x89PNG'),'screenshot failed'
  path.write_bytes(run.stdout);return path.name
 def request(name):
  return adb('shell','am','broadcast','-a',PACKAGE+'.DEBUG_SPAWN_CHARACTER','-p',PACKAGE,
   '--es','character_name',name)
 try:
  report['native_libraries']=inspect(a.apk)
  assert adb('shell','getconf','PAGE_SIZE')=='16384','16 KiB runtime required'
  report['page_size']=16384;report['api_level']=int(adb('shell','getprop','ro.build.version.sdk'))
  assert report['api_level']>=37,'modern Android target required'
  prior=adb('shell','cmd','window','user-rotation').split()
  adb('shell','cmd','window','user-rotation','lock','0')
  with zipfile.ZipFile(a.apk) as archive:
   actors=json.loads(archive.read('assets/actor-provenance.json'))
   gates={r['name']:i for i,r in enumerate(actors['records']) if r.get('gated_spawn')}
   assert set(gates)==set(NAMES),'bounded authored gate fixture changed'
  launch_fresh(adb,'--es','world','crypt01.dwld','--ei','object_index',str(gates[NAMES[0]]),'--ez','enemy_ai','false')
  text=wait(lambda t:all('Gated character ready | '+n+' | source state 0 | presentation visible 0 | body 0' in t for n in NAMES) and 'Native actor frame' in t,'initial hidden/no-body state')
  report['screenshots']=[capture('limbus-hidden')]
  offset=len(text);request('_prim_monster_SURPRISE_01')
  text=wait(lambda t:'SpawnCharacter development request | _prim_monster_SURPRISE_01 | result lookup_miss' in t[offset:],'exact-name miss')
  assert 'Spawn source state |' not in text[offset:] and 'Spawn body ready |' not in text[offset:],'lookup miss mutated actors'
  rows=[]
  for name in NAMES:
   offset=len(text);request(name)
   text=wait(lambda t:'Spawn source event | '+name+' | event 0x22 | current 3 | body 1' in t[offset:],'finite source Spawn completion '+name)
   chunk=text[offset:]
   assert 'Spawn source state | '+name+' | previous 0 | current 1 | flags 241 | sequence 213 | clip 714 | body 0' in chunk,'authored Spawn selection differs'
   assert 'Spawn source state | '+name+' | previous 1 | current 3 | flags 2380 | sequence 210 | clip 706 | body 1' in chunk,'source Idle selection differs'
   assert chunk.count('Spawn body ready | '+name+' | creations 1 |')==1,'first Spawn did not create exactly one body'
   assert 'Spawn fade stub | '+name+' | raw argument 3000 | original bx lr' in chunk,'fade stub argument differs'
   other=NAMES[1] if name==NAMES[0] else NAMES[0]
   assert 'Spawn source state | '+other not in chunk,'another actor state was changed'
   rows.append({'name':name,'spawn_sequence':213,'spawn_clip':714,'idle_sequence':210,'idle_clip':706,'body_creation_on_completion':True,
    'interactive_clip_event_observed':('name is_interactive' in chunk)})
   report['screenshots'].append(capture(name.lower().strip('_')+'-idle'))
   offset=len(text)
   adb('shell','am','broadcast','-a',PACKAGE+'.DEBUG_PLAYER_ATTACK','-p',PACKAGE,
    '--ei','player_target_index',str(gates[name]))
   text=wait(lambda t:'Player command applied | Gated actor combat services are pending' in t[offset:],'unsupported combat boundary '+name)
   assert 'Player attack selected |' not in text[offset:] and 'Prince combat hit |' not in text[offset:],'unbound combat mutated a source-owned gated actor'
  offset=len(text);adb('shell','am','broadcast','-a',PACKAGE+'.DEBUG_RELOAD_WORLD','-p',PACKAGE)
  text=wait(lambda t:'World reload command applied |' in t[offset:] and all('Gated character ready | '+n+' | source state 3 | presentation visible 1 | body 1' in t[offset:] for n in NAMES),'direct native world replacement')
  for name in NAMES:
   assert text[offset:].count('Spawn body ready | '+name+' | creations 1 |')==1,'direct reload duplicated a body'
   assert 'Spawn source state | '+name+' | previous 0 | current 1' not in text[offset:],'direct reload replayed Spawn'
  report['direct_world_replacement_retains_states_and_rebuilds_bodies']=True
  offset=len(text);adb('shell','cmd','window','user-rotation','lock','1')
  text=wait(lambda t:all('Gated character ready | '+n+' | source state 3 | presentation visible 1 | body 1' in t[offset:] for n in NAMES) and 'Native actor frame' in t[offset:],'Activity recreation retained both native actor states')
  for name in NAMES:
   assert 'Spawn source state | '+name+' | previous 0 | current 1' not in text[offset:],'recreation replayed Spawn'
   assert text[offset:].count('Spawn body ready | '+name+' | creations 1 |')==1,'recreated world duplicated ghost body'
  time.sleep(1) # Let Android's rotation surface animation finish for the artifact.
  logs();report['screenshots'].append(capture('landscape-restored'))
  report.update(validation='PASS',actors=rows,exact_name_miss_atomic=True,independent_actor_states=True,
   unbound_gated_combat_rejected_without_legacy_scheduler=True,
   activity_recreation_preserves_idle_and_rebuilds_one_body=True,
   scope='Native bounded first-spawn adapter only; hide-until-Spawn is an explicit development presentation policy. No source hallway-trigger, autonomous AI or full-game parity claim.')
 except Exception as error:report['error']=str(error);raise
 finally:
  if prior:
   try:adb('shell','cmd','window','user-rotation',*prior)
   except Exception as error:report['rotation_cleanup_error']=str(error)
  report['commands']=commands;(a.output/'spawn.log').write_text(last_logs,encoding='utf-8')
  (a.output/'spawn-smoke.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
 print(json.dumps({k:v for k,v in report.items() if k not in ('commands','native_libraries')},indent=2))

if __name__=='__main__':main()
