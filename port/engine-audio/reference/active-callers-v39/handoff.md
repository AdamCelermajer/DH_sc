# Active audio callers V39

Source snapshot: 2026-10-06T11:52:21.080717+00:00. Read-only current CMake declarations, recursive renderer .inc inclusion and actual callback registration. This is not a new compile, APK or device validation; cached compile commands are metadata only.

Root wired named sound into the ACTIVE player SkillState value3 and NPC authored operation during this audit. This captured snapshot includes those changes. Player attack/cast and NPC development scheduler paths are separate. The audio engine and AAudio output code are compiled declarations with no instantiated gameplay mixer/bank/bridge/output runtime owner in native_app/model_renderer; MainActivity still owns FrontAudio.

## Build and inclusion authority

App CMake declares native_app.cpp, model_renderer.cpp and audio_output_v34.cpp in dh2_native, adds/links dh2_engine_audio, and compiles VoxAudioBridgeV34 in dh2_level_world. Engine-audio CMake contains V34 audio units and V38 bindings/listener/source fields. Generic target V38 CMake declaration=True, active callee=True. The old target source also remains a library source; the active callsite evidence determines which entry is used.

The renderer has 47 recursive .inc edges. These present handoffs are absent from that include graph:

- `port/android-native/app/src/main/cpp/renderer_player_melee_event_v1.inc`: inactive; no include/registered owner found.
- `port/android-native/app/src/main/cpp/renderer_npc_melee_event_v1.inc`: inactive; no include/registered owner found.
- `port/android-native/app/src/main/cpp/renderer_item_graph_v4.inc`: inactive; no include/registered owner found.
- `port/android-native/app/src/main/cpp/renderer_character_loot_gameplay_v23.inc`: inactive; no include/registered owner found.
- `port/android-native/app/src/main/cpp/renderer_character_kill_live_v21.inc`: inactive; no include/registered owner found.
- `port/android-native/app/src/main/cpp/renderer_character_loot_live_v22.inc`: inactive; no include/registered owner found.

Header inclusion or library compilation does not instantiate an owner/register callbacks. The ACTIVE loot GPU sync is visual output; Item145/drop/pickup handoffs are not installed. Canonical container owners are library sources with no active renderer interaction/audio service found.

## Family ledger

| Family | Snapshot status |
| --- | --- |
| player animation-step and sword swoosh | active producer; positive Vox endpoints missing |
| player named sfx_ outside attack/cast | active leaf wired in current snapshot |
| player attack-state named sfx_ and combat impact | active development melee path; named/impact tails absent |
| player cast-state named sfx_ | active cast branch; non-do_spell events return |
| retained NPC authored sfx_ | active leaf wired in current snapshot |
| NPC development attack/monster combat sound | active damage route; source audio tails absent |
| skill result sound | active common result tail; Vox suffix missing |
| target-in-sight sound | active generic V38 caller |
| projectile/impact audio | no active projectile impact owner |
| item drop/pickup audio | gameplay handoffs inactive; GPU sync function active |
| chests/containers audio | library owners available; no active renderer container interaction/audio service |

### player animation-step and sword swoosh

**Active chain:** PlayerSkillsRuntime.bind_combat_fx -> prince_locomotion.selection_fx_v2={&t,player_animation_step_fx_v2} -> player_animation_step_fx_v2 ordinary step / source swoosh fallback -> source_animation_sound_v4 -> source_animation_request_v38 -> VoxPlay3DOwnerV2.

**Remaining:** Only disabled/current_level operations bound; reached online/platform/UID/bank/emit/native branches required; source_level_load_phase defaults0; no writer found in active source snapshot; Ordinary helper reads Vox identity inside request construction AFTER heading_position; original3c94f8/3ca898 captures manager BEFORE GetTargetPosition3935dc. Named helper already preserves this order..

**Root integration:** Bind one real audio bridge to source_animation_request_v38, preserving actual gates. Capture ordinary helper manager before the position callback as original3c94f8/3ca898 does. The player selection callback is registered. Do not force phase38.

**Source evidence:**
- [port/android-native/app/src/main/cpp/renderer_combat_fx_runtime_v4.inc:91](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/renderer_combat_fx_runtime_v4.inc:91): `prince_locomotion.selection_fx_v2={&t,player_animation_step_fx_v2};`
- [port/android-native/app/src/main/cpp/renderer_player_animation_step_fx_v2.inc:41](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/renderer_player_animation_step_fx_v2.inc:41): `if(!step.swoosh&&source_animation_sound_v4(t,step.sound)){error=t.error;return false;}`
- [port/android-native/app/src/main/cpp/renderer_animation_sound_v4.inc:4](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/renderer_animation_sound_v4.inc:4): `static int source_animation_request_v38(PlayerSkillsRuntime& t,const dh2::character::CombatSoundPlayV1& play){`

### player named sfx_ outside attack/cast

**Active chain:** character_playback_event event0x28 and state!=5/7 -> dh2_character_skill_state_v4 skill_state_ai_event_v4 -> PlayerSkillsRuntime.state_service skill_state_named_prefix_v4 value3 -> source_named_animation_sound_v38 -> genuine AudioSourceBindingsV38 names -> source_animation_request_v38 -> Vox prefix.

**Remaining:** Gameplay bank/driver suffix remains unbound; state5/7 bypass this prefix path.

**Root integration:** Current value3 branch is the correct active SkillState integration point. Root added it during this audit; do not edit inactive melee handoff.

**Source evidence:**
- [port/android-native/app/src/main/cpp/model_renderer.cpp:2109](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/model_renderer.cpp:2109): `case skill_state_named_prefix_v4:{`
- [port/android-native/app/src/main/cpp/model_renderer.cpp:2126](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/model_renderer.cpp:2126): `if(q->value==3)return source_named_animation_sound_v38(t,t.fsm.character,reinterpret_cast<const char*>(q->payload));`
- [port/android-native/app/src/main/cpp/renderer_animation_sound_v4.inc:28](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/renderer_animation_sound_v4.inc:28): `const int status=dh2::audio::audio_named_animation_sound_v38(suffix,t.audio_source_bindings_v38,services,result);`
- [port/android-native/app/src/main/cpp/renderer_combat_fx_runtime_v4.inc:76](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/renderer_combat_fx_runtime_v4.inc:76): `if(!t.audio_source_bindings_v38.load(records.data(),records.size(),names.data(),names.size(),t.error))`

### player attack-state named sfx_ and combat impact

**Active chain:** character_playback_event state5 event0x28 -> player_authored_event -> dh2_combat_event_route -> only CombatEventKind::melee accepted -> dh2_combat_melee + dh2_combat_apply_player_to_monster -> text/threat/HP/pending_death.

**Remaining:** sfx_ routes to no melee action and returns; This direct damage path does not call combat_sound_application_v2 or common skill_apply_sound_v6; renderer_player_melee_event_v1.inc is not included/registered.

**Root integration:** Replace active player_authored_event call path with whole source melee-event ownership when its genuine actor/AI/range/attack providers exist; or source-proved named prefix dispatch at this active entry. Do not claim inactive authored_melee_event is running.

**Source evidence:**
- [port/android-native/app/src/main/cpp/model_renderer.cpp:1559](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/model_renderer.cpp:1559): `void player_authored_event(const dh2::animation::TriggeredEvent& event,int clip){`
- [port/android-native/app/src/main/cpp/model_renderer.cpp:1566](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/model_renderer.cpp:1566): `if(action.kind!=dh2::data::CombatEventKind::melee)return;`
- [port/android-native/app/src/main/cpp/model_renderer.cpp:1775](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/model_renderer.cpp:1775): `const dh2::animation::TriggeredEvent trigger{event.handoff.lag_ms,event.handoff.payload};player_authored_event(trigger,event.clip);`

### player cast-state named sfx_

**Active chain:** character_playback_event state7 event0x28 -> FaeryCastOwnerV2.animation_event -> only do_spell sends faery_spell_callback_v1.

**Remaining:** sfx_ event is ignored by current cast animation_event; whole source CharAI prefix routing not installed for this state.

**Root integration:** Integrate at the active state7 authored branch before/through the source cast owner using recovered prefix ordering, not the unused melee include.

**Source evidence:**
- [port/android-native/app/src/main/cpp/model_renderer.cpp:1767](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/model_renderer.cpp:1767): `if(player_skills_runtime->cast->animation_event(player_gameplay_binding(),event.handoff.payload))throw std::runtime_error("Original cast animation event: "+player_skills_runtime->cast->error());return;`
- [port/android-native/app/src/main/cpp/faery_cast_owner_v2.cpp:83](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/faery_cast_owner_v2.cpp:83): `if(!same(p)||!name)return -1;if(p.state->current!=7||std::strcmp(name,"do_spell"))return 0;`

### retained NPC authored sfx_

**Active chain:** NPC retained BlendedPlayback observer -> MonsterScriptHandle.observe -> dh2_character_ai_event -> MonsterScriptHandle.animation_service helper3d4434 -> dh2_character_melee_animation_event_v1 step_index/step_count -> melee_event_sound_fx -> source_named_animation_sound_v38 actual NPC identity.

**Remaining:** Other authored melee/range/skill/spell/interaction branches still return-1 in active service; Current retained render branch restricted to living Idle or source state11; Attack/Walk/Dead development scheduler has another route; no registered NPC selection_fx_v2 callback in active snapshot.

**Root integration:** Use renderer_npc_animation_v1.inc authored service at helper3d4434; root added named case here during audit. For NPC ordinary step sound, register a same-NPC selection callback on its retained playback; preserve actual NPC manager/position/inventory.

**Source evidence:**
- [port/android-native/app/src/main/cpp/renderer_npc_animation_v1.inc:85](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/renderer_npc_animation_v1.inc:85): `if(q->operation==0x3d4434){`
- [port/android-native/app/src/main/cpp/renderer_npc_animation_v1.inc:92](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/renderer_npc_animation_v1.inc:92): `if(request->operation==melee_event_sound_fx){`
- [port/android-native/app/src/main/cpp/renderer_npc_animation_v1.inc:151](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/renderer_npc_animation_v1.inc:151): `m.animation->playback().observer={&m,MonsterScriptHandle::observe};`
- [port/android-native/app/src/main/cpp/model_renderer.cpp:2865](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/model_renderer.cpp:2865): `if(actor.script&&!actor.combat_state().dead&&(actor.state=="Idle"||actor.script->machine->state().current==11)){`

### NPC development attack/monster combat sound

**Active chain:** draw/update development NPC scheduler -> dh2_events_update -> dh2_combat_event_route -> apply_actor_attack -> apply_actor_to_player or dh2_combat_apply_monster_to_monster.

**Remaining:** No F_ApplyCombatSound/common skill_apply_sound_v6 invocation in these direct functions; only melee kind accepted; range/projectile actions ignored; low-health cue logs audio pending.

**Root integration:** Integrate actual source NPC melee/combat ownership at apply_actor_attack/event dispatch, retaining common result tail services; do not append guessed sounds to the development damage routine.

**Source evidence:**
- [port/android-native/app/src/main/cpp/model_renderer.cpp:1341](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/model_renderer.cpp:1341): `void apply_actor_attack(ObjectActor& attacker,const dh2::data::CombatEventAction& action){`
- [port/android-native/app/src/main/cpp/model_renderer.cpp:2910](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/model_renderer.cpp:2910): `apply_actor_attack(actor,action);`
- [port/android-native/app/src/main/cpp/model_renderer.cpp:1337](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/model_renderer.cpp:1337): `if(applied.health.low_health_cue)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Player low health request | HP %d | maximum %d | audio pending",applied.health.after,prince_combat.properties().resolved[38]);`

### skill result sound

**Active chain:** player_skill_animation_event_v6 -> native_skill_animation_event -> CharacterWorldSkillExecutionV6 -> WorldSkillCombatBackendsV6.application skill_apply_sound_v6 -> combat_sound_application_v2 -> CharacterCombatSoundV1 -> actual cached rows/shared Random/target position -> VoxPlay3DOwnerV2.

**Remaining:** Audio transport only disabled/current_level; bank/output continuations required; This common sound tail is not used by direct development player/NPC melee routines.

**Root integration:** Bind the positive Vox services at renderer_combat_sound_v2.inc s.play to same runtime bridge; retain genuine result/actor rows and RNG.

**Source evidence:**
- [port/android-native/app/src/main/cpp/model_renderer.cpp:2304](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/model_renderer.cpp:2304): `if(q->service==skill_apply_sound_v6){if(!result)return -1;const auto handled=combat_sound_application_v2(t,*q,*result);return handled==1?0:-1;}`
- [port/android-native/app/src/main/cpp/renderer_combat_sound_v2.inc:5](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/renderer_combat_sound_v2.inc:5): `int combat_sound_application_v2(PlayerSkillsRuntime& t,`
- [port/android-native/app/src/main/cpp/renderer_combat_sound_v2.inc:34](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/renderer_combat_sound_v2.inc:34): `runtime.error="Required original Vox Play3D reached operation "+std::to_string(unsigned(request.operation));return -1;`

### target-in-sight sound

**Active chain:** PlayerSkillsRuntime.event_service ai_event_virtual0xa..0x11 -> target_ai_event -> TargetEventServices40 -> generic target-event coordinator -> player_target_event_service_v2 target_event_play_sound -> Vox prefix.

**Remaining:** positive Vox suffix required.

**Root integration:** V38 generic successor is now declared in CMake and called by active target_ai_event; preserve generic request fields and bind actual positive Vox services.

**Source evidence:**
- [port/android-native/app/src/main/cpp/renderer_player_cast_v2.inc:50](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/renderer_player_cast_v2.inc:50): `if(q->event>=0xa&&q->event<=0x11)return t.target_ai_event(q->event,q->callee);`
- [port/android-native/app/src/main/cpp/renderer_player_target_frame_v2.inc:158](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/renderer_player_target_frame_v2.inc:158): `const int result=dh2_audio_target_event_v38(&state,event,&services);`
- [port/android-native/app/src/main/cpp/renderer_player_target_frame_v2.inc:129](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/renderer_player_target_frame_v2.inc:129): `case target_event_play_sound:{`

### projectile/impact audio

**Active chain:** active attack_backend attack_range_redirect returns required -> development combat dispatch accepts only melee action.

**Remaining:** No active HandleImpactFX/source Projectile instance/impact owner found; 41-row source impact producer proof is independent evidence, not installed runtime.

**Root integration:** Restore actual range/projectile actor/update/collision and impact callback authority before attaching the source-proved impact sound producer.

**Source evidence:**
- [port/android-native/app/src/main/cpp/renderer_player_attack_v1.inc:54](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/renderer_player_attack_v1.inc:54): `case attack_range_redirect:`
- [port/android-native/app/src/main/cpp/renderer_player_attack_v1.inc:55](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/renderer_player_attack_v1.inc:55): `t.error="Required original AI_DoRangeAttack controller/sequence/projectile continuation";return -1;`
- [port/android-native/app/src/main/cpp/model_renderer.cpp:1342](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/model_renderer.cpp:1342): `if(action.kind!=dh2::data::CombatEventKind::melee||attacker.combat_state().dead)return;`

### item drop/pickup audio

**Active chain:** renderer_loot_gpu_v27.inc supplies sync_loot_visual_draws_v27 -> canonical world currently adopts player/NPC -> item/world modules exist in library source.

**Remaining:** renderer_item_graph_v4.inc and renderer_character_loot_gameplay_v23.inc not included; No active RendererCharacterLootGameplayV23 construction/frame/pickup call found; world_loot_gameplay_v23.cpp/world_loot_pickup_v23.cpp absent current level-world CMake.

**Root integration:** Install sole actual Item145 graph/lifecycle and source drop/pickup authority through the root gameplay owner; supply real Vox to its item services. GPU geometry sync alone is not drop/pickup gameplay.

**Source evidence:**
- [port/android-native/app/src/main/cpp/model_renderer.cpp:1116](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/model_renderer.cpp:1116): `#include "renderer_loot_gpu_v27.inc"`
- [port/android-native/app/src/main/cpp/renderer_canonical_world_v4.inc:83](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/renderer_canonical_world_v4.inc:83): `if(!canonical.adopt(actor.script,error))throw std::runtime_error(error);`
- [port/android-native/app/src/main/cpp/renderer_character_loot_gameplay_v23.inc:57](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/android-native/app/src/main/cpp/renderer_character_loot_gameplay_v23.inc:57): `owner_=std::make_unique<dh2::character::WorldLootGameplayV23>(*t_.targets,`

### chests/containers audio

**Active chain:** canonical/openable/destructible source owners listed in dh2_level_world CMake -> development renderer adopts retained player/NPC graph.

**Remaining:** No active container services constructing/loading/interacting with chest33 sound found; inactive loot gameplay would still need actual container event and item drop transports; exact ChestOpen sample absent in V38 asset ledger.

**Root integration:** Bind canonical source container interaction/animation services on actual Level module objects and same audio bridge. Do not infer a chest from visual mesh alone.

**Source evidence:**
- [port/level-world/CMakeLists.txt:27](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-audio/reference/active-callers-v39/source-snapshot/port/level-world/CMakeLists.txt:27): `container_animation_connection_v21.cpp canonical_openable_graph_v21.cpp)`

## Common positive sound boundary

Active animation/named/combat/target transports create VoxPlay3DOwnerV2 services handling only disabled/current_level. Once the real initialized Level reaches subsequent gates, online/platform/source UID/event/bank/trace/emit/native operations remain explicit required failures. The captured current-Level reply uses renderer &level and WorldScriptContext.source_level_load_phase, initialized0 with no active assignment found. Root must bind genuine GS/current-Level/phase authority, rather than setting that field38.

Install one real audio runtime with genuine generated bindings/selected XML, bank/sample lifetime, source/listener fields, event timestamps, and bridge/output control thread. Connect lifecycle/focus via MainActivity/NativeBridge without competing with existing FrontAudio. AudioLifecycleV34.java and audio_output_v34.cpp availability is not application wiring. Exact authored assets/missing clips remain a separate V38 ledger; no suffix/substitute fallback is authorized by this audit.

## Verification boundary

All 61 inspected files are copied and hashed under source-snapshot. Changes detected during capture: []. Evidence links point at those immutable review snapshots because root may edit live files afterward. Only this V39 reference directory was written. V38/frozen/shared renderer/CMake were not edited; no emulator/ADB/APK was used.
