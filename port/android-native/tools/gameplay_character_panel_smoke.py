"""Verify native character actions and potion health on the visible development emulator.

No gameplay state is injected: character changes use visible widgets, damage
uses the existing original enemy attack debug command, and healing uses touch.
Full campaign, authored SWF navigation and skill damage are outside this proof.
"""
from pathlib import Path
import argparse,hashlib,json,re,subprocess,time,xml.etree.ElementTree as ET
from character_combat_smoke import inspect

def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--adb',required=True);p.add_argument('--serial',required=True);p.add_argument('--apk',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--retain-session',action='store_true',help='Test the current visible world without restarting or changing actor positions.');a=p.parse_args()
 assert a.serial=='emulator-5554';a.output.mkdir(parents=True,exist_ok=True)
 report={'validation':'FAIL','scope':__doc__,'apk_sha256':hashlib.sha256(a.apk.read_bytes()).hexdigest(),'libraries':inspect(a.apk),'physical_arm64_tested':False,'full_game_playable':False}
 pid='';last='';steps=[]
 def adb(*args):return subprocess.run([a.adb,'-s',a.serial,*args],check=True,capture_output=True,text=True,encoding='utf8',timeout=45).stdout.strip()
 def logs():
  nonlocal last,pid
  process=subprocess.run([a.adb,'-s',a.serial,'shell','pidof','com.example.dh2'],capture_output=True,text=True,encoding='utf8',timeout=45)
  if process.returncode==1 and not process.stdout.strip():return ''
  process.check_returncode();pid=process.stdout.strip();assert pid and ' ' not in pid
  last=adb('logcat','-d','--pid='+pid,'-v','brief');assert not re.search(r'Fatal signal|FATAL EXCEPTION|Native frame failed|GL error',last),last[-4000:];return last
 def wait(predicate,timeout=20):
  start=time.monotonic()
  while time.monotonic()-start<timeout:
   value=logs()
   if predicate(value):return value
   time.sleep(.15)
  raise AssertionError('Timed out waiting for source observation')
 def states(text):return [json.loads(x) for x in re.findall(r'Character snapshot state \| (\{[^\n]+\})',text)]
 def view(name):
  adb('shell','uiautomator','dump','/sdcard/dh2-panel-smoke.xml');raw=adb('shell','cat','/sdcard/dh2-panel-smoke.xml');(a.output/(name+'.xml')).write_text(raw,encoding='utf8');return ET.fromstring(raw)
 def click(name,description=True):
  nodes=view('ui-'+str(len(steps)));node=next((n for n in nodes.iter('node') if n.get('content-desc' if description else 'text')==name and n.get('enabled')=='true'),None);assert node is not None,name
  x0,y0,x1,y1=map(int,re.findall(r'\d+',node.get('bounds')));assert x1>x0 and y1>y0;adb('shell','input','tap',str((x0+x1)//2),str((y0+y1)//2));steps.append(name)
 def screen(name):
  result=subprocess.run([a.adb,'-s',a.serial,'exec-out','screencap','-p'],check=True,capture_output=True,timeout=45);assert result.stdout.startswith(b'\x89PNG');(a.output/(name+'.png')).write_bytes(result.stdout)
 def change(name,check):
  old=len(states(logs()));click(name);text=wait(lambda t:len(states(t))>old);value=states(text)[-1];assert check(value),value;return value
 try:
  remote=adb('shell','pm','path','com.example.dh2').removeprefix('package:');assert '\n' not in remote
  report['installed_apk_sha256']=adb('shell','sha256sum',remote).split()[0];assert report['installed_apk_sha256']==report['apk_sha256']
  if not a.retain_session:
   adb('shell','am','force-stop','com.example.dh2');launch=adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity','--ez','enemy_ai','false','--es','world','crypt01.dwld');assert 'Status: ok' in launch
  report['retained_visible_session']=a.retain_session
  wait(lambda t:'World ready |' in t and 'Original initial skill slots | ready 1 | row0 level 1 | slot0 0 | unspent 0' in t)
  click('Open character stats inventory skills and faeries');text=wait(lambda t:bool(states(t)));initial=states(text)[-1]
  assert initial['ready'] and initial['stats']['Physical_Armor']==4 and initial['skillPoints']==0 and initial['skills'][0]['SkillLevel']==1 and initial['skills'][0]['EquippedSlot']==0
  screen('stats');click('Character Equipment tab');screen('equipment')
  unequipped=change('Unequip Ceremonial Garb',lambda s:s['items'][0]['EquippedSlot']==-1 and s['stats']['Physical_Armor']==2);screen('unequipped')
  restored=change('Equip Ceremonial Garb',lambda s:s['items'][0]['EquippedSlot']==0 and s['stats']['Physical_Armor']==4)
  click('Character Skills tab');nodes=view('skills');assert any(n.get('text','').startswith('Headsplitter') for n in nodes.iter('node'));assert any(n.get('content-desc')=='Train skill 0' and n.get('enabled')=='false' for n in nodes.iter('node'));screen('skills')
  old=len(states(logs()));click('Slot 2',False);text=wait(lambda t:len(states(t))>old);assert states(text)[-1]['skills'][0]['EquippedSlot']==1
  old=len(states(text));click('Slot 1',False);text=wait(lambda t:len(states(t))>old);assert states(text)[-1]['skills'][0]['EquippedSlot']==0
  click('Character Faeries tab');nodes=view('faeries');assert any('Celeste' in n.get('text','') for n in nodes.iter('node'));assert all(f['State']==0 for f in states(logs())[-1]['faeries']);screen('faeries')
  click('Close character panel');screen('gameplay')
  def hud_touch(control):
   nodes=view('hud');node=next(n for n in nodes.iter('node') if n.get('content-desc')=='Three equipped skills, faery spell, and health potion');x0,y0,x1,y1=map(int,re.findall(r'\d+',node.get('bounds')));adb('shell','input','tap',str(round(x0+(x1-x0)*(control+.5)/5)),str((y0+y1)//2));steps.append('HUD control '+str(control))
  start=len(logs());hud_touch(4);text=wait(lambda t:'Health and mana are full' in t[start:]);vectors=re.findall(r'Gameplay HUD snapshot \| (\[[^\n]+\])',text[start:]);assert vectors and json.loads(vectors[-1])[14]==5
  start=len(text);adb('shell','am','start','-W','--activity-single-top','-n','com.example.dh2/.MainActivity','--ei','object_index','0','--es','object_state','Attack','--ei','combat_target_index','-2','--ei','time_ms','-1')
  text=wait(lambda t:'Prince damage received |' in t[start:]);damage=re.findall(r'Prince damage received .*?HP (-?\d+) (-?\d+)',text[start:]);assert damage and int(damage[-1][1])<int(damage[-1][0])
  start=len(text);hud_touch(4);text=wait(lambda t:'Potion used: health and mana restored' in t[start:]);vectors=re.findall(r'Gameplay HUD snapshot \| (\[[^\n]+\])',text[start:]);assert vectors and json.loads(vectors[-1])[14]==4
  wait(lambda t:'HP 42265 42265' in t[start:])
  # The damage command focuses the enemy for inspection. Restore the ordinary
  # follow-player camera so the final visible emulator stays in gameplay.
  adb('shell','am','start','-W','--activity-single-top','-n','com.example.dh2/.MainActivity','--ei','object_index','-1','--ei','time_ms','-1')
  time.sleep(.2);screen('healed')
  report.update(validation='PASS',steps=steps,source_armor_before=4,source_armor_unequipped=2,source_armor_restored=4,source_skill_assignment_roundtrip=True,source_initial_skill_level=1,source_no_spendable_points_training_disabled=True,original_faery_names_visible=True,locked_faeries_preserved=True,potion_full_refused_count=5,potion_after_enemy_hit_count=4,enemy_hp_before=int(damage[-1][0]),enemy_hp_after=int(damage[-1][1]),potion_restored_raw_hp=42265)
 except Exception as error:report['error']=str(error);raise
 finally:
  (a.output/'current-process.log').write_text(last,encoding='utf8');(a.output/'gameplay-character-panel-smoke.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf8')
 print(json.dumps({k:v for k,v in report.items() if k not in ('scope','libraries')}))

if __name__=='__main__':main()
