#include "source_campaign_animation_step_v100.hpp"
#include "model_renderer.hpp"
#include "source_campaign_fx_v77.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "source_campaign_animation_random_scope_v101.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "character_candidate_cache_v62.hpp"
#include "character_combat_sound_v1.hpp"
#include "character_animation_step_fx_v2.hpp"
#include "audio_animation_swoosh_v38.hpp"
#include "audio_world_producer_v38.hpp"
#include "vox_play3d_owner_v2.hpp"
#include "canonical_point3d_globals_v1.hpp"
#include <algorithm>
namespace model_renderer {namespace {
bool submit_menu_preview_animation_sound_v122(
 const dh2::audio::AudioApplicationBorrowV42& captured,
 dh2::world::CanonicalCharacterCandidateRecordV60& record,
 const dh2::character::CombatSoundPlayV1& play,std::string& error){
 struct Prefix {const dh2::audio::AudioApplicationBorrowV42& audio;
  dh2::world::CanonicalCharacterCandidateRecordV60& record;std::string& error;};
 if(!captured.manager||!captured.manager->runtime_on_producer()||!record.services.debug||!record.services.debug_files){
  error="Required same process preview Vox/Debug owners";return false;
 }
 Prefix prefix{captured,record,error};
 // Keep the request context alive for the synchronous owner call.
 dh2::sound::VoxPlay3DOwnerV2 owner({&prefix,[](void* raw,const dh2::sound::VoxPlay3DRequestV2& q,
   dh2::sound::VoxPlay3DResponseV2& out){
   auto& p=*static_cast<Prefix*>(raw);using O=dh2::sound::VoxPlay3DOperationV2;
   if(!q.play||q.play->manager!=p.audio.identity())return -1;
   if(q.operation==O::disabled){std::uint32_t disabled{};
    const int loaded=dh2_character_debug_load(p.record.services.debug,p.record.services.debug_files);
    if(loaded!=1){p.error="Preview Vox Play3D Debug.Load(IsDisablingSounds) status "+std::to_string(loaded);return -1;}
    const int queried=dh2_character_debug_get(&disabled,p.record.services.debug,"IsDisablingSounds",p.record.services.debug_files);
    if(queried!=1){p.error="Preview Vox Play3D Debug.GetSwitch(IsDisablingSounds) status "+std::to_string(queried);return -1;}
    out.value=std::int32_t(disabled);return 0;
   }
   if(q.operation==O::current_level){dh2::loader::CanonicalCurrentLevelBorrowV1 current;
    if(!borrow_current_native_level_v27(current,p.error))return -1;out.identity=current.identity();out.value=0;
    if(current){dh2::loader::CanonicalLevelContextV1::LoadingFieldsV26 fields;
     if(!current.level()->loading_fields_v26(fields,p.error)||!fields.state130)return -1;out.value=std::int32_t(*fields.state130);}
    return 0;
   }
   p.error="Required actual process preview Vox Play3D continuation "+std::to_string(unsigned(q.operation));return -1;
  }});
 const int result=owner.play(play);if(result){if(error.empty())error=owner.error();return false;}error.clear();return true;
}
struct CombatDeliveryV112 {
 std::shared_ptr<void> world;
 std::shared_ptr<const dh2::character::CharacterCandidateCacheV62> cache;
 SourceCampaignCharacterBorrowV62 target;
 dh2::audio::AudioApplicationBorrowV42 manager;
 std::string& error;
};
struct StepDeliveryV100 {
 dh2::world::CanonicalCharacterCandidateRecordV60& record;
 std::string& error;
 bool position(float* out){
  auto& r=record;dh2::character::skills::WorldTargetActorBorrowV1 b;
  if(!r.services.world_targets||r.services.world_targets->actor(r.actor->object->identity,&b)||!b.position||!b.target_node){error="Required SAME canonical animation GetTargetPosition";return false;}
  if(*b.target_node&&(!b.target_enabled||(*b.target_enabled&&!b.cached_target_position))){error="Required actual animation target-node position producer";return false;}
  const auto* p=dh2::character::skills::dh2_world_target_position_v1(b.position,b.cached_target_position,*b.target_node,b.target_enabled?*b.target_enabled:0);
  if(!p)return false;std::copy_n(p,3,out);return true;
 }
 bool rotation(float* out){
  dh2::world::GameObjectInitializationFieldsV62 fields;
  if(!record.actor->inherited_initialization_fields_v62(record.shared_from_this(),fields,error))return false;
  auto* p=fields.vector3?fields.vector3(0x16c):nullptr;
  if(!p){error="Required SAME raw Character rotation16c";return false;}std::copy_n(p,3,out);return true;
 }
 bool sound(int id){
  dh2::audio::AudioApplicationBorrowV42 captured;
  if(!borrow_actual_application_audio_v42(captured,error))return false;
  std::array<float,3> p;if(!position(p.data()))return false;
  bool player{};if(!record.is_player(player,error))return false;
  // Capture the source SoundManager before the position callback. Null and
  // negative IDs are delivered to the real source Play3D prefix, not skipped.
  const auto play=dh2::audio::audio_world_request_v38(captured.identity(),0,id,p);
  SourceCampaignCandidateBorrowV55 campaign;std::string ignored;
  if(borrow_source_campaign_candidate_v55(campaign,ignored)&&campaign.actual_world==record.services.world)
   return submit_campaign_audio_v46(player?dh2::audio::AudioCategoryV46::attack:dh2::audio::AudioCategoryV46::monster,
    captured,record.services.world,play,error);
  // Menu preview records belong to NativeMenuRendererDomainV121, not a
  // WorldScriptContext. Their real process Debug receiver is carried on the
  // record services; never static-cast that distinct owner to World.
  return submit_menu_preview_animation_sound_v122(captured,record,play,error);
 }
 bool play(int set,const float* p,const float* rotation,std::uintptr_t anchor){
  std::shared_ptr<void> lease;dh2::fx::CharacterMeshFxOwnerV4* manager{};
  if(!borrow_source_campaign_fx_v77(record.services.world,lease,manager,error)||!manager)return false;
  return manager->play_set(set,p,rotation,anchor,nullptr,error);
 }
};
}
bool source_campaign_combat_sound_v112(const std::shared_ptr<void>& world,std::uintptr_t attacker,
 std::uintptr_t target,bool character_attacker,const dh2::data::CombatResult& result,std::string& error){
 using namespace dh2::character;using namespace dh2::character::skills;
 CombatDeliveryV112 d{world,{},{},{},error};
 if(!borrow_source_campaign_character_cache_v81(world,d.cache,error)||
    !borrow_source_campaign_character_v62(world,target,d.target,error))return false;
 auto& record=*d.target.character;CampaignAnimationRandomScopeV101 random_scope(record);
 CombatSoundServicesV1 s;s.context=&d;
 s.row=[](void* raw,std::uintptr_t id,const CombatSoundRowV1** out){auto& d=*static_cast<CombatDeliveryV112*>(raw);SourceCampaignCharacterBorrowV62 actual;
  if(!out||!borrow_source_campaign_character_v62(d.world,id,actual,d.error)||!actual.character->properties)return -1;
  *out=d.cache->sounds_v70().get(actual.character->properties->resolved[9]);return *out?0:-1;};
 s.minimal_randoms=[](void* raw,std::uint32_t* out){auto& r=*static_cast<CombatDeliveryV112*>(raw)->target.character;
  return out&&r.services.debug&&r.services.debug_files&&dh2_character_debug_load(r.services.debug,r.services.debug_files)==1&&
   dh2_character_debug_get(out,r.services.debug,"MP_MinimalRandoms",r.services.debug_files)==1?0:-1;};
 s.dead=[](void* raw,std::uintptr_t id,bool* out){auto& d=*static_cast<CombatDeliveryV112*>(raw);auto& r=*d.target.character;WorldTargetActorBorrowV1 actual;
  if(!out||!r.services.world_targets||r.services.world_targets->actor(id,&actual)||!actual.life)return -1;*out=actual.life->dead!=0;return 0;};
 s.manager=[](void* raw,std::uintptr_t* out){auto& d=*static_cast<CombatDeliveryV112*>(raw);if(!out||!borrow_actual_application_audio_v42(d.manager,d.error))return -1;*out=d.manager.identity();return 0;};
 s.random=[](void* raw,std::uint32_t count,std::uint32_t* out){auto& d=*static_cast<CombatDeliveryV112*>(raw);auto* random=d.target.character->services.random;
  if(!out||!random){d.error="Required SAME App Random(false) combat sound channel";return -1;}*out=dh2_animation_random(&random->seed,&random->calls,count);return 0;};
 s.position=[](void* raw,std::uintptr_t id,std::array<float,3>* out){auto& d=*static_cast<CombatDeliveryV112*>(raw);auto& r=*d.target.character;WorldTargetActorBorrowV1 a;
  if(!out||!r.services.world_targets||r.services.world_targets->actor(id,&a)||!a.position||!a.target_node)return -1;
  if(*a.target_node&&(!a.target_enabled||(*a.target_enabled&&!a.cached_target_position)))return -1;
  const auto* p=dh2_world_target_position_v1(a.position,a.cached_target_position,*a.target_node,a.target_enabled?*a.target_enabled:0);if(!p)return -1;std::copy_n(p,3,out->begin());return 0;};
 s.play=[](void* raw,const CombatSoundPlayV1* request){auto& d=*static_cast<CombatDeliveryV112*>(raw);bool player{};
  if(!request||!d.target.character->is_player(player,d.error))return -1;
  return submit_campaign_audio_v46(player?dh2::audio::AudioCategoryV46::attack:dh2::audio::AudioCategoryV46::monster,d.manager,d.world,*request,d.error)?0:-1;};
 CombatSoundOutputV1 out;const auto status=character_combat_sound_v1(&out,&result,attacker,target,character_attacker,&s);
 if(status){error="Required canonical combat sound phase "+std::to_string(out.phase)+(error.empty()?"":"; "+error);return false;}
 error.clear();return true;
}
bool source_campaign_animation_step_v100(void* raw,dh2::actor::BlendedPlayback& playback,
 const dh2::data::AnimationStep& step,std::string& error){
 auto* r=static_cast<dh2::world::CanonicalCharacterCandidateRecordV60*>(raw);
 if(!r||!r->actor||!r->actor->object||!r->visual||!r->visual->animator()||&r->visual->animator()->playback()!=&playback){error="Required SAME canonical CharAnimator step receiver";return false;}
 CampaignAnimationRandomScopeV101 random_scope(*r);
 StepDeliveryV100 delivery{*r,error};dh2::character::AnimationStepFxServicesV2 services;services.context=&delivery;
 services.owner=[](void* p,std::uintptr_t& id){id=static_cast<StepDeliveryV100*>(p)->record.actor->object->identity;return id?0:-1;};
 services.target_position=[](void* p,std::uintptr_t id,float* out){auto& d=*static_cast<StepDeliveryV100*>(p);return id==d.record.actor->object->identity&&d.position(out)?0:-1;};
 services.rotation=[](void* p,std::uintptr_t id,float* out){auto& d=*static_cast<StepDeliveryV100*>(p);return id==d.record.actor->object->identity&&d.rotation(out)?0:-1;};
 services.play=[](void* p,int set,const float* position,const float* rotation,std::uintptr_t anchor){return static_cast<StepDeliveryV100*>(p)->play(set,position,rotation,anchor)?0:-1;};
 services.swoosh_fx_gate=[](void* p,const auto& step,bool& gate){
  auto& d=*static_cast<StepDeliveryV100*>(p);dh2::character::AnimationSwooshServicesV4 sw;sw.context=&d;
  sw.equipped=[](void* raw,int kind,std::uintptr_t& id){auto& d=*static_cast<StepDeliveryV100*>(raw);auto* inv=d.record.inventory37c;
   if(!inv||inv->character()!=d.record.actor->object->identity||kind<0||kind>=9){d.error="Required SAME Character constructor/Gear inventory37c";return -1;}
   auto* slot=inv->equipment().at(inv->current_equipment()).at(kind);id=slot&&slot->item?reinterpret_cast<std::uintptr_t>(slot->item.get()):0;return 0;};
  sw.effects=[](void* raw,std::uintptr_t id,int& sound,int& fx){auto& d=*static_cast<StepDeliveryV100*>(raw);auto* inv=d.record.inventory37c;if(!inv)return -1;
   const auto it=std::find_if(inv->items().begin(),inv->items().end(),[id](const auto& slot){return slot&&reinterpret_cast<std::uintptr_t>(slot->item.get())==id;});
   if(it==inv->items().end())return -1;auto* item=dh2::data::item(inv->table(),(*it)->item->id);if(!item)return -1;sound=item->record.words[5];fx=item->record.words[6];return 0;};
  sw.play_sound=[](void* raw,int id){return static_cast<StepDeliveryV100*>(raw)->sound(id)?0:-1;};
  sw.play_fx=[](void* raw,int id,bool anchored){auto& d=*static_cast<StepDeliveryV100*>(raw);if(anchored)return d.play(id,dh2::world::canonical_vec3_origin_v1().data(),nullptr,d.record.actor->object->identity)?0:-1;
   float p[3],rotation[3];return d.position(p)&&d.rotation(rotation)&&d.play(id,p,rotation,0)?0:-1;};
  bool fallback{};if(dh2::audio::audio_animation_swoosh_v38(step.anchor_fx,sw,fallback,gate,d.error))return -1;
  return !fallback||d.sound(step.sound)?0:-1;
 };
 if(!step.swoosh&&!delivery.sound(step.sound))return false;
 std::string detail;if(dh2::character::character_animation_step_fx_v2(step,services,detail)){if(error.empty())error=detail;else error=detail+"; "+error;return false;}return true;
}
}
