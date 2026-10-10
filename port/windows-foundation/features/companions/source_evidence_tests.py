"""Validate executable fixture provenance against the supplied cache and IDA."""
from pathlib import Path
import hashlib,json,struct,zipfile
import re

root=Path(__file__).resolve().parents[4]
here=Path(__file__).resolve().parent
cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
expected={
 'follower.luac':'2d281c3bc784bbc12356e37cff9e7bdcd44a13e1d45da1fcb21069b4fef48d9d',
 'attacking_follower.luac':'6a9c560b8a7904a19518db0fcecba593c63061fdabe34abedc8f2c88f0a3438c',
 'rene.luac':'631e7137c6ba49fca45e6cf0b37d1e45e4f42af4ca120106274e7e28d5f56fc7'}
with zipfile.ZipFile(cache) as z:
 for name,digest in expected.items():
  raw=z.read('com.gameloft.android.GAND.GloftD2SS/files/data/scripts/ai/'+name)
  assert hashlib.sha256(raw).hexdigest()==digest
  assert raw==(here/'reference'/name).read_bytes()
 ai=z.read('com.gameloft.android.GAND.GloftD2SS/files/data/pydata/ai_pyarray.bin')
 count=struct.unpack_from('<I',ai)[0];at=4;scripts=[]
 for _ in range(count):
  at+=33;n=struct.unpack_from('<I',ai,at)[0];at+=4
  scripts.append(ai[at:at+n].decode());at+=n+20
 assert at==len(ai) and scripts[50]=='rene'
profile=json.loads((root/'.local-inputs/windows-shared-assets/annotated-profiles.json').read_text())
priest=next(r for r in profile['configurations'] if r['character']=='WanderingPriest')
assert priest['property_row']==419 and priest['properties']['AI']==50
# The Swamp's actual authored Character placement is the priest template. Keep
# this evidence distinct from the unverified user-facing name "Castor".
placement_path=root/'port/windows-foundation/assets/original-cache/data/3d/modules/swamp/mgp/obj_4of4_brdwalk_sw_00.mgp'
placement_bytes=placement_path.read_bytes()
placement=placement_bytes.decode()
assert hashlib.sha256(placement_bytes).hexdigest()==\
 '651eac91518c0a22362c22cb3ffe15972649e259a6af59d04ba028a90a7baf31'
priest_placement=re.search(r'<GameObject\b(?=[^>]*\bname="_prim_NPC_PriestGood")[^>]*\/>',placement,re.S)
assert priest_placement
for authored in ('gametype="Character"','_templateName="NPC"','charpropsname="WanderingPriest"','activate_cond="RENE_FOLLOW"'):
 assert authored in priest_placement.group(0)
swamp_pydata=(root/'port/windows-foundation/assets/original-cache/data/pydata/scripts/001_swamp_pyscripts.bin').read_bytes()
assert hashlib.sha256(swamp_pydata).hexdigest()==\
 '144447bc17c9b5ac530e308cce86490c04191779b66f4b88914f2a6c8adde836'
moving_rene=swamp_pydata.index(b'Moving Rene')
rene_move_block=swamp_pydata[moving_rene:moving_rene+512]
assert b'_prim_NPC_PriestGood' in rene_move_block
assert b'Rene done got moved!!!!!!!!!!!' in rene_move_block
assert b'castor' not in placement.lower().encode() and b'castor' not in swamp_pydata.lower()
# The v1.0.3 reference frame is at a manifest-recorded timestamp. Its dialogue
# speaker label identifies the caged priest as Rene; it does not establish a
# second name or an alias to Castor.
video_manifest=json.loads((root/'.local-inputs/referenceframes/dh2-act1/manifest.json').read_text())
assert video_manifest['source_version']=='1.0.3' and video_manifest['recovered_version']=='1.0.2'
assert video_manifest['source_url']=='https://www.youtube.com/watch?v=z_Zky7qQdYs'
frame=next(f for f in video_manifest['frames'] if f['timestamp_seconds']==120)
assert Path(frame['path']).resolve()==(root/'.local-inputs/referenceframes/dh2-act1/at-0120s.png').resolve()
assert hashlib.sha256(Path(frame['path']).read_bytes()).hexdigest()==\
 '8d2417a8ee0b7ed754bb1bc1de1d375b2d961d9f581c0af17bfd5d89fb3a5251'
video=root/'.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4'
assert hashlib.sha256(video.read_bytes()).hexdigest()==\
 '98087ee80f594bcc317144b5dd15ee4b470830b8eb8f5b13badc24bd9831e502'
rene=(here/'reference/rene.luac').read_text()
assert 'StartTimerCB(200, false, CaughtUp)' in rene
assert 'local OID_HASTE' in rene and 'Buff_Rene_Haste' in rene
assert 'MoveTo(g_master);' in rene and 'if(not HasPath()) then' in rene and 'WarpTo(g_master);' in rene
assert 'g_is_catching_up = false;' in rene and 'EnableCollisions(true);' in rene
assert 'function rene_OnEnemySpotted(enemy)' in rene
assert 'if(not g_is_far)then' in rene and 'GrabTarget(enemy);' in rene
assert 'function rene_OnTargetInRangedRange()' in rene and 'Attack();' in rene
assert 'function rene_OnTargetInCloseRange()' in rene and 'Attack();' in rene
for callback in ('OnEnemySpotted','OnTargetDied','OnTargetOutOfSight','OnTargetOutOfRange',
                 'OnTargetInRangedRange','OnTargetInCloseRange','OnMasterOutOfSight',
                 'OnMasterInSight','OnMasterInRangedRange','OnMasterInCloseRange'):
 assert f'AddToVFTable("{callback}"' in rene
# Bind the live adapter's callback IDs against the independent native source
# producers: target update raises Character 0xa..0x11 and master update raises
# 18..25 only after its same-CharAI field/range checks.
live=(here/'live_companion_scripts.cpp').read_text()
for event,callback in ((18,'OnMasterDied'),(19,'OnMasterRevived'),(20,'OnMasterOutOfSight'),
                       (21,'OnMasterInSight'),(22,'OnMasterOutOfRange'),
                       (23,'OnMasterInRangedRange'),(24,'OnMasterInCloseRange'),
                       (25,'OnMasterInMeleeRange')):
 assert f'{{{event},"{callback}"' in live
master_kernel=(root/'port/level-world/character_update_master_v108.hpp').read_text()
assert 'event=24' in master_kernel and 'value?23:22' in master_kernel and 'value?25:22' in master_kernel
target_kernel=(root/'port/level-world/character_target_update.cpp').read_text()
assert 'event(s,services,0xa,s->target)' in target_kernel and 'value?0x11:0xe' in target_kernel
record_events=(here/'source_companion_events.cpp').read_text()
assert 'borrow_source_campaign_character_path_v105(record, path, error)' in record_events
assert 'path.path != &actor.runtime.path' in record_events
assert 'path.machine != &actor.machine->native_fsm()' in record_events
assert 'session.dispatch_target(callback.object_event' in record_events
assert 'subject != fields->master50' in record_events
assert 'event == 9 ? subject : 0' in record_events
follower=(here/'reference/follower.luac').read_text()
assert 'MoveTo(g_master);' in follower and 'WarpBehind(g_master);' in follower
assert follower.index('MoveTo(g_master);')<follower.index('ClearTarget();')
attacking=(here/'reference/attacking_follower.luac').read_text()
assert 'local g_can_attack = true;' in attacking
assert 'if(g_can_attack and not HasTarget()) then' in attacking
assert 'Flee(g_target);' in attacking and 'Attack(g_target);' in attacking
ida=root/'.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so'
vt=next(r for r in json.loads((ida/'vtables-and-rtti.json').read_text()) if r['name']=='_ZTV8AISFaery')
assert struct.unpack_from('<I',bytes.fromhex(vt['raw_bytes_hex']),8+124)[0]==0x3de318
melee=(ida/'pseudocode/003d/003de318.c').read_text()
assert '200.0' in melee and 'Cmd_HeadTo' in melee and melee.count('SM_IsIdle')==2
warp=(ida/'pseudocode/003d/003de4c8.c').read_text()
assert 'Cmd_WarpTo' in warp and 'GetTargetPosition' in warp
update=(ida/'pseudocode/003d/003de544.c').read_text()
assert update.index('AISDefault::OnUpdate')<update.index('SetModularSkin')
assert update.count('SG_GetCurrentFaerieId')==2
print('companion cache identity/authored branches/native vtable service evidence passed')
