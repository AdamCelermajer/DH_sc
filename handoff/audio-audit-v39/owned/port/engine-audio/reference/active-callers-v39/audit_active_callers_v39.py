"""Read-only caller/build census; writes only this independent V39 report."""
from pathlib import Path
import re,json,hashlib,datetime
R=Path(__file__).resolve().parents[4];O=Path(__file__).resolve().parent
CPP=R/'port/android-native/app/src/main/cpp';MODEL=CPP/'model_renderer.cpp'
sources={};edges=[]
def read(p):
 p=p.resolve()
 if p not in sources:sources[p]=p.read_bytes()
 return sources[p].decode('utf-8-sig')
def walk(p):
 s=read(p)
 for no,line in enumerate(s.splitlines(),1):
  m=re.match(r'\s*#include\s+"([^"]+\.inc)"',line)
  if not m:continue
  q=p.parent/m[1];edges.append(dict(parent=p.relative_to(R).as_posix(),line=no,include=m[1],path=q.relative_to(R).as_posix()))
  if q.exists() and q.resolve()not in sources:walk(q)
walk(MODEL)
active={p.name for p in sources}
cmakes=[CPP/'CMakeLists.txt',R/'port/level-world/CMakeLists.txt',R/'port/engine-audio/CMakeLists.txt']
for p in cmakes:read(p)
for name in ['native_app.cpp','faery_cast_owner_v2.cpp','renderer_player_melee_event_v1.inc','renderer_npc_melee_event_v1.inc','renderer_item_graph_v4.inc','renderer_character_loot_gameplay_v23.inc','renderer_character_kill_live_v21.inc','renderer_character_loot_live_v22.inc']:
 p=CPP/name
 if p.exists():read(p)
for name in ['MainActivity.java','FrontAudio.java','AudioLifecycleV34.java']:
 read(R/'port/android-native/app/src/main/java/com/example/dh2'/name)
def hits(filename,needle):
 return [dict(path=p.relative_to(R).as_posix(),line=i,text=line.strip())for p in sources if p.name==filename for i,line in enumerate(read(p).splitlines(),1)if needle in line]
def one(filename,needle):
 h=hits(filename,needle);return h[0]if h else dict(path=filename,missing=needle)
families=[]
def family(name,status,chain,gaps,integration,anchors):
 families.append(dict(family=name,status=status,active_chain=chain,remaining=gaps,root_integration=integration,evidence=[one(f,k)for f,k in anchors]))
model=read(MODEL);npc=read(CPP/'renderer_npc_animation_v1.inc');sound=read(CPP/'renderer_animation_sound_v4.inc')
target_v38_callee='dh2_audio_target_event_v38(&state' in read(CPP/'renderer_player_target_frame_v2.inc')
target_v38_cmake=any('audio_target_events_v38.cpp' in read(p) for p in cmakes)
family('player animation-step and sword swoosh','active producer; positive Vox endpoints missing',
 ['PlayerSkillsRuntime.bind_combat_fx','prince_locomotion.selection_fx_v2={&t,player_animation_step_fx_v2}','player_animation_step_fx_v2 ordinary step / source swoosh fallback','source_animation_sound_v4','source_animation_request_v38 -> VoxPlay3DOwnerV2'],
 ['Only disabled/current_level operations bound; reached online/platform/UID/bank/emit/native branches required','source_level_load_phase defaults0; no writer found in active source snapshot','Ordinary helper reads Vox identity inside request construction AFTER heading_position; original3c94f8/3ca898 captures manager BEFORE GetTargetPosition3935dc. Named helper already preserves this order.'],
 'Bind one real audio bridge to source_animation_request_v38, preserving actual gates. Capture ordinary helper manager before the position callback as original3c94f8/3ca898 does. The player selection callback is registered. Do not force phase38.',
 [('renderer_combat_fx_runtime_v4.inc','prince_locomotion.selection_fx_v2={&t'),('renderer_player_animation_step_fx_v2.inc','source_animation_sound_v4(t,step.sound)'),('renderer_animation_sound_v4.inc','source_animation_request_v38')])
family('player named sfx_ outside attack/cast','active leaf wired in current snapshot'if 'q->value==3)return source_named_animation_sound_v38' in model else 'active prefix case missing',
 ['character_playback_event event0x28 and state!=5/7','dh2_character_skill_state_v4 skill_state_ai_event_v4','PlayerSkillsRuntime.state_service skill_state_named_prefix_v4 value3','source_named_animation_sound_v38 -> genuine AudioSourceBindingsV38 names','source_animation_request_v38 -> Vox prefix'],
 ['Gameplay bank/driver suffix remains unbound','state5/7 bypass this prefix path'],
 'Current value3 branch is the correct active SkillState integration point. Root added it during this audit; do not edit inactive melee handoff.',
 [('model_renderer.cpp','skill_state_named_prefix_v4'),('model_renderer.cpp','q->value==3)return source_named_animation_sound_v38'),('renderer_animation_sound_v4.inc','audio_named_animation_sound_v38(suffix'),('renderer_combat_fx_runtime_v4.inc','audio_source_bindings_v38.load')])
family('player attack-state named sfx_ and combat impact','active development melee path; named/impact tails absent',
 ['character_playback_event state5 event0x28','player_authored_event','dh2_combat_event_route','only CombatEventKind::melee accepted','dh2_combat_melee + dh2_combat_apply_player_to_monster','text/threat/HP/pending_death'],
 ['sfx_ routes to no melee action and returns','This direct damage path does not call combat_sound_application_v2 or common skill_apply_sound_v6','renderer_player_melee_event_v1.inc is not included/registered'],
 'Replace active player_authored_event call path with whole source melee-event ownership when its genuine actor/AI/range/attack providers exist; or source-proved named prefix dispatch at this active entry. Do not claim inactive authored_melee_event is running.',
 [('model_renderer.cpp','void player_authored_event'),('model_renderer.cpp','if(action.kind!=dh2::data::CombatEventKind::melee)return;'),('model_renderer.cpp','player_authored_event(trigger,event.clip)')])
family('player cast-state named sfx_','active cast branch; non-do_spell events return',
 ['character_playback_event state7 event0x28','FaeryCastOwnerV2.animation_event','only do_spell sends faery_spell_callback_v1'],
 ['sfx_ event is ignored by current cast animation_event','whole source CharAI prefix routing not installed for this state'],
 'Integrate at the active state7 authored branch before/through the source cast owner using recovered prefix ordering, not the unused melee include.',
 [('model_renderer.cpp','cast->animation_event(player_gameplay_binding()'),('faery_cast_owner_v2.cpp','p.state->current!=7||std::strcmp(name,"do_spell")')])
family('retained NPC authored sfx_','active leaf wired in current snapshot'if 'request->operation==melee_event_sound_fx' in npc else 'active leaf missing',
 ['NPC retained BlendedPlayback observer','MonsterScriptHandle.observe -> dh2_character_ai_event','MonsterScriptHandle.animation_service helper3d4434','dh2_character_melee_animation_event_v1 step_index/step_count','melee_event_sound_fx -> source_named_animation_sound_v38 actual NPC identity'],
 ['Other authored melee/range/skill/spell/interaction branches still return-1 in active service','Current retained render branch restricted to living Idle or source state11; Attack/Walk/Dead development scheduler has another route','no registered NPC selection_fx_v2 callback in active snapshot'],
 'Use renderer_npc_animation_v1.inc authored service at helper3d4434; root added named case here during audit. For NPC ordinary step sound, register a same-NPC selection callback on its retained playback; preserve actual NPC manager/position/inventory.',
 [('renderer_npc_animation_v1.inc','if(q->operation==0x3d4434)'),('renderer_npc_animation_v1.inc','request->operation==melee_event_sound_fx'),('renderer_npc_animation_v1.inc','playback().observer='),('model_renderer.cpp','actor.script&&!actor.combat_state().dead')])
family('NPC development attack/monster combat sound','active damage route; source audio tails absent',
 ['draw/update development NPC scheduler','dh2_events_update -> dh2_combat_event_route','apply_actor_attack','apply_actor_to_player or dh2_combat_apply_monster_to_monster'],
 ['No F_ApplyCombatSound/common skill_apply_sound_v6 invocation in these direct functions','only melee kind accepted; range/projectile actions ignored','low-health cue logs audio pending'],
 'Integrate actual source NPC melee/combat ownership at apply_actor_attack/event dispatch, retaining common result tail services; do not append guessed sounds to the development damage routine.',
 [('model_renderer.cpp','void apply_actor_attack'),('model_renderer.cpp','apply_actor_attack(actor,action)'),('model_renderer.cpp','audio pending')])
family('skill result sound','active common result tail; Vox suffix missing',
 ['player_skill_animation_event_v6','native_skill_animation_event -> CharacterWorldSkillExecutionV6','WorldSkillCombatBackendsV6.application skill_apply_sound_v6','combat_sound_application_v2 -> CharacterCombatSoundV1','actual cached rows/shared Random/target position -> VoxPlay3DOwnerV2'],
 ['Audio transport only disabled/current_level; bank/output continuations required','This common sound tail is not used by direct development player/NPC melee routines'],
 'Bind the positive Vox services at renderer_combat_sound_v2.inc s.play to same runtime bridge; retain genuine result/actor rows and RNG.',
 [('model_renderer.cpp','q->service==skill_apply_sound_v6'),('renderer_combat_sound_v2.inc','int combat_sound_application_v2'),('renderer_combat_sound_v2.inc','Required original Vox Play3D reached operation')])
family('target-in-sight sound','active generic V38 caller'if target_v38_callee else 'active generic caller still old entry',
 ['PlayerSkillsRuntime.event_service ai_event_virtual0xa..0x11','target_ai_event -> TargetEventServices40','generic target-event coordinator','player_target_event_service_v2 target_event_play_sound -> Vox prefix'],
 ([]if target_v38_callee and target_v38_cmake else ['V38 generic successor not yet both CMake/callee in this snapshot'])+['positive Vox suffix required'],
 'V38 generic successor is now declared in CMake and called by active target_ai_event; preserve generic request fields and bind actual positive Vox services.'if target_v38_callee and target_v38_cmake else 'Compile audio_target_events_v38.cpp and change active target_ai_event callee to dh2_audio_target_event_v38; preserve generic request fields.',
 [('renderer_player_cast_v2.inc','t.target_ai_event'),('renderer_player_target_frame_v2.inc','const int result=dh2_'),('renderer_player_target_frame_v2.inc','case target_event_play_sound')])
family('projectile/impact audio','no active projectile impact owner',
 ['active attack_backend attack_range_redirect returns required','development combat dispatch accepts only melee action'],
 ['No active HandleImpactFX/source Projectile instance/impact owner found','41-row source impact producer proof is independent evidence, not installed runtime'],
 'Restore actual range/projectile actor/update/collision and impact callback authority before attaching the source-proved impact sound producer.',
 [('renderer_player_attack_v1.inc','case attack_range_redirect:'),('renderer_player_attack_v1.inc','Required original AI_DoRangeAttack'),('model_renderer.cpp','action.kind!=dh2::data::CombatEventKind::melee')])
family('item drop/pickup audio','gameplay handoffs inactive; GPU sync function active',
 ['renderer_loot_gpu_v27.inc supplies sync_loot_visual_draws_v27','canonical world currently adopts player/NPC','item/world modules exist in library source'],
 ['renderer_item_graph_v4.inc and renderer_character_loot_gameplay_v23.inc not included','No active RendererCharacterLootGameplayV23 construction/frame/pickup call found','world_loot_gameplay_v23.cpp/world_loot_pickup_v23.cpp absent current level-world CMake'],
 'Install sole actual Item145 graph/lifecycle and source drop/pickup authority through the root gameplay owner; supply real Vox to its item services. GPU geometry sync alone is not drop/pickup gameplay.',
 [('model_renderer.cpp','#include "renderer_loot_gpu_v27.inc"'),('renderer_canonical_world_v4.inc','canonical.adopt(actor.script'),('renderer_character_loot_gameplay_v23.inc','owner_=std::make_unique')])
family('chests/containers audio','library owners available; no active renderer container interaction/audio service',
 ['canonical/openable/destructible source owners listed in dh2_level_world CMake','development renderer adopts retained player/NPC graph'],
 ['No active container services constructing/loading/interacting with chest33 sound found','inactive loot gameplay would still need actual container event and item drop transports','exact ChestOpen sample absent in V38 asset ledger'],
 'Bind canonical source container interaction/animation services on actual Level module objects and same audio bridge. Do not infer a chest from visual mesh alone.',
 [('CMakeLists.txt','canonical_openable_graph_v21.cpp')])
inactive=[dict(file=p.relative_to(R).as_posix(),active_include=p.name in active)for p in sources if p.name in ['renderer_player_melee_event_v1.inc','renderer_npc_melee_event_v1.inc','renderer_item_graph_v4.inc','renderer_character_loot_gameplay_v23.inc','renderer_character_kill_live_v21.inc','renderer_character_loot_live_v22.inc']]
# Preserve exact source snapshot, so root edits made concurrently do not
# silently alter the report's line references or its active/inactive finding.
snapshot=[]
for p,b in sorted(sources.items()):
 relative=p.relative_to(R);q=O/'source-snapshot'/relative;q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(b)
 snapshot.append(dict(path=relative.as_posix(),bytes=len(b),sha256=hashlib.sha256(b).hexdigest(),snapshot=q.relative_to(R).as_posix()))
dbs=[]
for p in (R/'port/android-native/app/.cxx').glob('**/compile_commands.json'):
 try:
  rows=json.loads(p.read_text());selected=[row for row in rows if Path(row['file']).name=='model_renderer.cpp']
  if selected:dbs.append(dict(path=p.relative_to(R).as_posix(),sha256=hashlib.sha256(p.read_bytes()).hexdigest(),modified_utc=datetime.datetime.fromtimestamp(p.stat().st_mtime,datetime.timezone.utc).isoformat(),model_renderer_entries=len(selected),scope='cached configure command, not proof latest source was built'))
 except (ValueError,OSError):pass
drift=[p.relative_to(R).as_posix()for p,b in sources.items()if p.read_bytes()!=b]
report=dict(snapshot_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),scope='Current build declarations + recursive local .inc source inclusion and actual service registrations. Read-only: not a new build or APK/device proof. Root changed named caller integration during early audit; this snapshot reflects latest captured source.',include_edges=edges,inactive_handoffs=inactive,families=families,build_target_v38=dict(callee=target_v38_callee,cmake=target_v38_cmake),common_output_gap=dict(audio_engine_linked=True,audio_output_cpp_in_native_target=True,audio_runtime_owner_instantiated_in_active_sources=False,source_bindings_loaded_in_combat_fx_bind=True,focus_owner='MainActivity FrontAudio only; AudioLifecycleV34 not instantiated',gate='World source_level_load_phase constructor0; no active assignment found; do not force38'),source_snapshot=snapshot,cached_compile_databases=dbs,changed_after_snapshot=drift)
(O/'active-callers.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(dict(snapshot=report['snapshot_utc'],active_inc_edges=len(edges),snapshot_files=len(snapshot),inactive=inactive,changes_after_snapshot=drift)))
