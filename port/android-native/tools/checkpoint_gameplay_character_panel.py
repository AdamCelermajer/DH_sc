"""Preserve scoped menu/potion checkpoint, its APK and actual compiler inputs."""
from pathlib import Path
import hashlib,io,json,subprocess,zipfile,argparse
from character_combat_smoke import inspect
REPO=Path(__file__).resolve().parents[3]
def sha(raw):return hashlib.sha256(raw).hexdigest()
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--smoke',type=Path,required=True);p.add_argument('--ninja',type=Path,required=True);p.add_argument('--skill-smoke',type=Path);p.add_argument('--recurring-smoke',type=Path);p.add_argument('--animation-smoke',type=Path);p.add_argument('--attack-smoke',type=Path);p.add_argument('--known-failure',type=Path,action='append',default=[]);p.add_argument('--host-evidence',type=Path,action='append',default=[]);a=p.parse_args()
 project=REPO/'port/android-native';apk=project/'app/build/outputs/apk/debug/app-debug.apk';raw=apk.read_bytes();digest=sha(raw)
 smoke=json.loads(a.smoke.read_text());assert smoke['validation']=='PASS' and smoke['apk_sha256']==smoke['installed_apk_sha256']==digest
 skill=None
 if a.skill_smoke:
  skill=json.loads(a.skill_smoke.read_text());assert skill['validation']=='PASS' and skill['apk_sha256']==skill['installed_apk_sha256']==digest
 prefix='native-skill-world' if skill else 'native-character-gameplay'
 runtime=[]
 for label,path in (('recurring_smoke',a.recurring_smoke),('animation_smoke',a.animation_smoke)):
  if path:
   evidence=json.loads(path.read_text());assert evidence['validation']=='PASS' and evidence['apk_sha256']==evidence['installed_apk_sha256']==digest
   runtime.append((label,path,evidence))
 if runtime:
  assert skill
  prefix='native-gameplay-runtime'
 attack=None
 if a.attack_smoke:
  attack=json.loads(a.attack_smoke.read_text());assert attack['validation']=='PASS' and attack['apk_sha256']==attack['installed_apk_sha256']==digest
  assert attack['source_target_injected'] is False and attack['existing_melee_application_verified']
  prefix='native-targeted-combat'
 source={};compiler={};snap={}
 for abi in ('arm64-v8a','x86_64'):
  dbs=list((project/'app/.cxx/Debug').glob('*/'+abi+'/compile_commands.json'));assert len(dbs)==1;db=dbs[0];commands=json.loads(db.read_bytes())
  deps=subprocess.run([str(a.ninja),'-C',str(db.parent),'-t','deps'],check=True,capture_output=True,text=True).stdout
  names=set(row['file'] for row in commands)|{line.strip() for line in deps.splitlines() if line.startswith('    ')};used={}
  for name in sorted(names):
   path=Path(name).resolve()
   if not path.is_relative_to(REPO):continue
   payload=path.read_bytes();key=path.relative_to(REPO).as_posix();used[key]=sha(payload);source[key]=sha(payload);snap['source/'+key]=payload
  compiler[abi]={'compile_commands_sha256':sha(db.read_bytes()),'dependencies_sha256':sha(deps.encode()),'inputs':used}
  snap['compiler/'+abi+'-commands.json']=db.read_bytes();snap['compiler/'+abi+'-dependencies.txt']=deps.encode()
 for path in (project/'app/src/main').rglob('*'):
  if path.is_file() and 'assets' not in path.parts:
   key=path.relative_to(REPO).as_posix();payload=path.read_bytes();source[key]=sha(payload);snap['source/'+key]=payload
 for relative in ('port/android-native/app/build.gradle.kts','port/android-native/app/src/main/cpp/CMakeLists.txt','port/level-world/CMakeLists.txt','port/engine-ui/CMakeLists.txt'):
  payload=(REPO/relative).read_bytes();source[relative]=sha(payload);snap['source/'+relative]=payload
 assets={};cache=None
 with zipfile.ZipFile(apk) as archive:
  for info in archive.infolist():
   if not info.filename.startswith('assets/') or info.is_dir():continue
   name=info.filename.removeprefix('assets/');payload=archive.read(info)
   if name=='dh2-original-cache.zip':
    assert info.compress_type==zipfile.ZIP_STORED and sha(payload)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
    with zipfile.ZipFile(io.BytesIO(payload)) as bundled:cache={'sha256':sha(payload),'files':sum(not item.is_dir() for item in bundled.infolist()),'bytes':len(payload)}
   else:assert payload==(project/'app/src/main/assets'/name).read_bytes(),name
   assets[name]={'bytes':len(payload),'sha256':sha(payload),'compression':info.compress_type}
 assert cache and cache['files']==6833
 libraries=inspect(apk);out=project/'build/checkpoints';out.mkdir(parents=True,exist_ok=True);checkpoint=out/('dh2-'+prefix+'-'+digest[:8]+'.apk')
 if checkpoint.exists():assert checkpoint.read_bytes()==raw,'Existing checkpoint differs'
 record={'validation':'PASS','scope':'Native character view/action and potion milestone; full authored SWF lifecycle, enemy AI, skill effects, campaign saves and physical devices are incomplete.','checkpoint':{'path':str(checkpoint),'sha256':digest,'bytes':len(raw)},'smoke':{'path':str(a.smoke.resolve()),'sha256':sha(a.smoke.read_bytes())},'compiler':compiler,'source_sha256':source,'libraries':libraries,'assets_verified':assets,'cache':cache,'physical_arm64_tested':False,'full_game_playable':False}
 snap['validation/'+a.smoke.name]=a.smoke.read_bytes()
 if skill:
  record['skill_smoke']={'path':str(a.skill_smoke.resolve()),'sha256':sha(a.skill_smoke.read_bytes()),'evidence':skill}
  snap['validation/'+a.skill_smoke.name]=a.skill_smoke.read_bytes()
  record['scope']='Genuine eleven-NPC Crypt state initialization and target flags, 69-row native trophy owner, same-world skill combat binding, no-target Headsplitter source animation cycle, character actions and potions verified. Enemy skill damage, full AI/physical movement, faery casting, skill FX/audio, original animated menu lifecycle, campaign saves and physical devices remain incomplete.'
 for label,path,evidence in runtime:
  record[label]={'path':str(path.resolve()),'sha256':sha(path.read_bytes()),'evidence':evidence}
  snap['validation/'+path.name]=path.read_bytes()
 if runtime:
  record['scope']+=' Same-player mana regeneration and repeated no-target skill availability are checked separately in the included live runtime receipts. Positive enemy target bars and unlocked faery casting remain host-only or untested as specified in those receipts.'
 if a.animation_smoke:
  record['scope']+=' Physical joystick touches after actual equipment-triggered level reload verify source Move/Idle and left/right Lua animation-event delivery; the retained floor borrow is rebound to the replacement level. Accepted Crypt footprint sets are -1, so this does not establish positive visual FX.'
 if attack:
  record['attack_smoke']={'path':str(a.attack_smoke.resolve()),'sha256':sha(a.attack_smoke.read_bytes()),'evidence':attack}
  snap['validation/'+a.attack_smoke.name]=a.attack_smoke.read_bytes()
  record['scope']='Source player attack command searches actual registered enemies and retains the same AI target; original attack step begin/end and native melee HP changes passed after real equipment reload. Character tabs, skill-slot changes and potion healing passed in the retained visible world. This proves existing melee HP application, not the complete original hit pipeline. Enemy skill damage, full enemy AI/physics, skill FX/audio, unlocked faery casting, authored menu lifecycle, campaign saves and physical devices remain incomplete. Precision waypoint movement did not pass; retained failure evidence describes the limitation.'
 record['known_failures']=[]
 for path in a.known_failure:
  payload=path.read_bytes();evidence=json.loads(payload);assert evidence.get('validation')=='FAIL' and evidence['apk_sha256']==digest,path
  record['known_failures'].append({'path':str(path.resolve()),'sha256':sha(payload),'evidence':evidence})
  snap['validation/failure-'+path.name]=payload
 record['host_evidence']=[]
 for path in a.host_evidence:
  payload=path.read_bytes();evidence=json.loads(payload);assert evidence.get('validation')=='PASS',path
  record['host_evidence'].append({'path':str(path.resolve()),'sha256':sha(payload),'evidence':evidence})
  snap['validation/host-'+path.name]=payload
 if not checkpoint.exists():checkpoint.write_bytes(raw)
 report=project/'reports'/(prefix+'-'+digest[:8]+'-checkpoint-validation.json');report.write_text(json.dumps(record,indent=2)+'\n')
 snap['checkpoint-build-provenance.json']=json.dumps(record,indent=2).encode();snapshot=out/('dh2-'+prefix+'-'+digest[:8]+'-source.zip')
 assert not snapshot.exists()
 with zipfile.ZipFile(snapshot,'w',zipfile.ZIP_DEFLATED) as archive:
  for key,payload in sorted(snap.items()):archive.writestr(key,payload)
 print(json.dumps({'checkpoint':str(checkpoint),'sha256':digest,'sources':len(source),'libraries':len(libraries),'assets':len(assets),'cache_files':cache['files'],'report':str(report),'source_snapshot':str(snapshot)}))
if __name__=='__main__':main()
