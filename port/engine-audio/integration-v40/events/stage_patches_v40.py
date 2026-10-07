"""Write hash-locked prospective patches only; never edit live source."""
from pathlib import Path
import json,hashlib,difflib,datetime
O=Path(__file__).resolve().parent;R=O.parents[3]
CPP='port/android-native/app/src/main/cpp/'
old={};new={};groups={};raw_base={}
def source(path):
 if path not in old:raw_base[path]=(R/path).read_bytes();old[path]=(R/path).read_text(encoding='utf-8');new[path]=old[path]
 return new[path]
def replace(path,a,b,group):
 text=source(path);assert text.count(a)==1,(path,a[:80],text.count(a));new[path]=text.replace(a,b);groups.setdefault(group,set()).add(path)
def span(path,start,end,replacement,group):
 text=source(path);assert text.count(start)==text.count(end)==1
 a=text.index(start);b=text.index(end,a);new[path]=text[:a]+replacement+text[b:];groups.setdefault(group,set()).add(path)
model=CPP+'model_renderer.cpp';animation=CPP+'renderer_animation_sound_v4.inc';combat=CPP+'renderer_combat_sound_v2.inc';target=CPP+'renderer_player_target_frame_v2.inc'
replace(model,'#include "audio_named_animation_sound_v38.hpp"', '#include "audio_named_animation_sound_v38.hpp"\n#include "integration-v40/events/audio_producer_transport_v40.hpp"\n#include "integration-v40/events/audio_authored_event_services_v40.hpp"','01-active-transport')
replace(model,'  dh2::audio::AudioSourceBindingsV38 audio_source_bindings_v38;','  dh2::audio::AudioSourceBindingsV38 audio_source_bindings_v38;\n  dh2::audio::AudioProducerTransportV40 audio_producers_v40; // bound to ONE root runtime + actual event clock','01-active-transport')
start='static int source_animation_request_v38(';end='\nstatic int source_animation_sound_v4('
span(animation,start,end,'static int source_animation_request_v38(PlayerSkillsRuntime& t,const dh2::character::CombatSoundPlayV1& play){\n if(!t.vox_music||!t.world){t.error="Required same animation Vox/World owner";return -1;}\n return dh2::audio::audio_submit_actual_producer_v40(t.audio_producers_v40,play,t.error)?0:-1;\n}\n','01-active-transport')
replace(animation,' const float* position=nullptr;if(!PlayerSkillsRuntime::heading_position(&t,t.world->player_object->identity,position,t.error))return -1;\n return source_animation_request_v38(t,dh2::audio::audio_world_request_v38(t.vox_music->identity(),0,id,{position[0],position[1],position[2]}));',' const auto captured_manager=t.vox_music->identity(); // source capture BEFORE position callback\n const float* position=nullptr;if(!PlayerSkillsRuntime::heading_position(&t,t.world->player_object->identity,position,t.error))return -1;\n return source_animation_request_v38(t,dh2::audio::audio_world_request_v38(captured_manager,0,id,{position[0],position[1],position[2]}));','01-active-transport')
span(combat,' s.play=[]','\n CombatSoundOutputV1 out{};',' s.play=[](void* p,const CombatSoundPlayV1* play){auto& r=*static_cast<PlayerSkillsRuntime*>(p);if(!play)return -1;\n  return dh2::audio::audio_submit_actual_producer_v40(r.audio_producers_v40,*play,r.error)?0:-1;};','01-active-transport')
text=source(target);a=text.index('  dh2::sound::VoxPlay3DOwnerV2 owner(',text.index('case target_event_play_sound:'));b=text.index('\n }\n case target_event_active_dispatch:',a)
oldpart=text[a:b];replace(target,oldpart,'  return dh2::audio::audio_submit_actual_producer_v40(t.audio_producers_v40,play,t.error)?0:-1;','01-active-transport')
# Stage named prefix paths separately; this is NOT full melee/range/cast
# replacement and does not insert guessed impact sounds after prototype hits.
base_after_core=dict(new)
replace(model,'static int source_named_animation_sound_v38(PlayerSkillsRuntime&,std::uintptr_t,const char*);','static int source_named_animation_sound_v38(PlayerSkillsRuntime&,std::uintptr_t,const char*);\nstatic int source_player_named_prefix_v40(PlayerSkillsRuntime&,const char*,bool&);','02-player-named-prefix')
replace(model,'#include "renderer_animation_sound_v4.inc"','#include "renderer_animation_sound_v4.inc"\n#include "integration-v40/events/renderer_player_named_prefix_v40.inc"','02-player-named-prefix')
replace(model,'void player_authored_event(const dh2::animation::TriggeredEvent& event,int clip){','void player_authored_event(const dh2::animation::TriggeredEvent& event,int clip){\n if(event.name&&!std::strncmp(event.name,"sfx_",4)){\n  if(!player_skills_runtime)throw std::runtime_error("Required actual player named-event owner");\n  bool handled=false;const int status=source_player_named_prefix_v40(*player_skills_runtime,event.name,handled);\n  if(!handled||status)throw std::runtime_error("Required source attack named sound: "+player_skills_runtime->error);return;\n }','02-player-named-prefix')
replace(model,'   if(prince_state.current==7&&player_skills_runtime&&player_skills_runtime->cast){\n    if(player_skills_runtime->cast->animation_event','   if(prince_state.current==7&&player_skills_runtime&&player_skills_runtime->cast){\n    bool handled=false;const int named_status=source_player_named_prefix_v40(*player_skills_runtime,static_cast<const char*>(event.handoff.payload),handled);\n    if(handled){if(named_status)throw std::runtime_error("Required source cast named sound: "+player_skills_runtime->error);return;}\n    if(player_skills_runtime->cast->animation_event','02-player-named-prefix')
# Prospective item services, held until genuine145/WorldLoot owners exist.
graph_h='port/level-world/world_item_graph_v3.hpp';pickup_h='port/level-world/world_loot_pickup_v23.hpp'
for path in (graph_h,pickup_h):
 replace(path,'#pragma once','#pragma once\n#include "../engine-audio/integration-v40/events/audio_source_emission_v40.hpp"','03-prerequisite-item-sites')
 marker=' bool(*before_item_destroy)(void*,data::ItemInstanceV1&,std::string&){};' if path==graph_h else ' bool(*remaining)(void*,const LootInteractRequestV8&,LootInteractResponseV8&,std::string&){};'
 replace(path,marker,marker+'\n const audio::AudioProducerTransportV40* audio_runtime_v40{};\n void* audio_manager_context_v40{};\n bool(*actual_audio_manager_v40)(void*,std::uintptr_t&,std::string&){};','03-prerequisite-item-sites')
graph='port/level-world/world_item_graph_v3.cpp';pickup='port/level-world/world_loot_pickup_v23.cpp'
replace(graph,' case WorldItemOperationV1::drop_sound:\n  if(!services_.vox){e="Required same world Vox Play3D owner";return false;}\n  return world_item_drop_sound_v2(*services_.vox,services_.vox_identity,q.object,static_cast<std::int16_t>(q.integer),q.position,e);',' case WorldItemOperationV1::drop_sound:{\n  if(!services_.audio_runtime_v40||!services_.actual_audio_manager_v40){e="Required ONE root audio runtime for actual Item drop";return false;}\n  std::uintptr_t manager{};if(!services_.actual_audio_manager_v40(services_.audio_manager_context_v40,manager,e))return false;\n  if(!q.position){e="Required actual cached Item position1a8";return false;}std::array<float,3> position{};std::memcpy(position.data(),q.position,12);\n  return audio::audio_emit_actual_item_v40(*services_.audio_runtime_v40,manager,q.object,static_cast<std::int16_t>(q.integer),position,e);}','03-prerequisite-item-sites')
replace(pickup,' case O::pickup_sound:{if(!services_.vox){e="Required actual pickup Vox owner";return false;}const float* p=item->base().vector3(0x160);return world_item_drop_sound_v2(*services_.vox,services_.vox_identity,q.object,static_cast<std::int16_t>(q.argument),p,e);}',' case O::pickup_sound:{\n  if(!services_.audio_runtime_v40||!services_.actual_audio_manager_v40){e="Required ONE root audio runtime for actual Item pickup";return false;}\n  std::uintptr_t manager{};if(!services_.actual_audio_manager_v40(services_.audio_manager_context_v40,manager,e))return false;\n  const float* p=item->base().vector3(0x160);if(!p){e="Required actual raw Item position160";return false;}std::array<float,3> position{};std::memcpy(position.data(),p,12);\n  return audio::audio_emit_actual_item_v40(*services_.audio_runtime_v40,manager,q.object,static_cast<std::int16_t>(q.argument),position,e);}','03-prerequisite-item-sites')
# Complete container emission entry, held until genuine object factory services
# provide manager -> actual getter -> raw160 -> common runtime transport.
ch='port/level-world/openable_container_owner_v1.hpp';cc='port/level-world/openable_container_owner_v1.cpp';dc='port/level-world/canonical_destructible_container_v16.cpp'
replace(ch,'    std::function<bool(const char*,std::uintptr_t,const char*,std::string&)> script_call;','    std::function<bool(const char*,std::uintptr_t,const char*,std::string&)> script_call;\n    std::function<bool(std::string&)> emit_complete_source_sound_v40; // source manager -> sound getter -> raw160','04-prerequisite-container-site')
replace('port/level-world/canonical_destructible_container_v16.hpp',' std::function<bool(const char*,std::uintptr_t,const char*,std::string&)> script_call;',' std::function<bool(const char*,std::uintptr_t,const char*,std::string&)> script_call;\n std::function<bool(std::string&)> emit_complete_source_sound_v40; // complete manager -> getter -> raw160','04-prerequisite-container-site')
replace(cc,'    if(!call(s_.play_sound_3d,"VoxSoundManager::Play3D",e,r.sound))return false;','    if(!call(s_.emit_complete_source_sound_v40,"complete source container sound emission",e))return false;','04-prerequisite-container-site')
replace(dc,' row=current_row();if(!required(services_.common.play_sound_3d,"actual VoxSoundManager Play3D",e,row?row->sound():-1))return false;',' if(!required(services_.common.emit_complete_source_sound_v40,"complete source container sound emission",e))return false;','04-prerequisite-container-site')
replace(dc,'auto* row=current_row();return required(services_.common.play_sound_3d,"actual stage VoxSoundManager Play3D",e,row?row->sound():-1);','return required(services_.common.emit_complete_source_sound_v40,"complete source staged container sound emission",e);','04-prerequisite-container-site')
patches=O/'patches';patches.mkdir(exist_ok=True);prospective=O/'prospective';baseline=O/'source-snapshot'
def encoded(path,text):return text.replace('\n','\r\n').encode() if b'\r\n' in raw_base[path] else text.encode()
def diff(path,a,b):
 lines=list(difflib.unified_diff(a.splitlines(True),b.splitlines(True),fromfile='a/'+path,tofile='b/'+path))
 if b'\r\n' in raw_base[path]:lines=[line.replace('\n','\r\n')if line.startswith((' ','+','-')) and not line.startswith(('+++','---')) else line for line in lines]
 return ''.join(lines)
patch_bases={};patch_normalized_bases={}
for group,paths in groups.items():
 chunks=[]
 for path in sorted(paths):
  before=base_after_core.get(path,old[path]) if group=='02-player-named-prefix' else old[path]
  after=base_after_core.get(path,new[path]) if group=='01-active-transport' else new[path]
  chunks.append(diff(path,before,after))
  patch_bases.setdefault(group,{})[path]=hashlib.sha256(encoded(path,before)).hexdigest()
  patch_normalized_bases.setdefault(group,{})[path]=hashlib.sha256(before.encode()).hexdigest()
 (patches/(group+'.patch')).write_bytes(''.join(chunks).encode())
for path in old:
 for directory,text in ((baseline,old[path]),(prospective,new[path])):
  p=directory/path;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw_base[path]if directory==baseline else encoded(path,text))
entries=[dict(path=p,base_sha256=hashlib.sha256(raw_base[p]).hexdigest(),base_normalized_sha256=hashlib.sha256(old[p].encode()).hexdigest(),staged_sha256=hashlib.sha256(encoded(p,new[p])).hexdigest())for p in sorted(old)]
drift=[p for p in old if (R/p).read_bytes()!=raw_base[p]]
(O/'staged-contract.json').write_text(json.dumps(dict(captured_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),scope='Prospective diffs only; no shared/live/frozen source edited. Apply01 then02;03/04 held for genuine gameplay prerequisites.',staged_files=entries,patches={k:sorted(v)for k,v in groups.items()},patch_base_sha256=patch_bases,patch_base_normalized_sha256=patch_normalized_bases,changed_during_capture=drift,requires=dict(runtime='ONE root AudioGameplayRuntimeV40 and source gate/command/listener/RNG authorities',event_clock='actual positive source-event monotonic timestamp; scheduler lag resolved in actual producer scope, no helper now()',root_publication='bind PlayerSkillsRuntime.audio_producers_v40 callbacks before reached play; contexts outlive synchronous delivery',item='genuine Item145/WorldLoot lifecycle/inventory/physical/quest/tooltip/trophy/FX owners and source preloader',container='genuine canonical factory/interact/animation/quest/loot owners; complete sound manager->getter->position leaf; proven XML UID preload, no event RNG precache')),indent=2)+'\n')
print(json.dumps(dict(staged_files=len(entries),patches=list(groups),changed_during_capture=drift)))
