#pragma once
#include <sound_emitter_source_v92.hpp>
#include <native_audio_application_v42.hpp>
#include <audio_campaign_bridge_v46.hpp>
#include "renderer_character_campaign_v62.hpp"
#include "model_renderer.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "renderer_native_gslevel_v27.hpp"
#include <application_player_manager_bootstrap_v59.hpp>
namespace model_renderer {
// Exact qualified base tail implemented by CampaignNonCharacterReleaseV92.
// This helper is instantiated only with that existing typed source owner; it
// captures neither containing World nor a new destruction/record registry.
template<class Release,class Record>
void bind_campaign_sound_emitter_base_v92(std::weak_ptr<Release> release,
 std::weak_ptr<Record> record,dh2::world::SoundEmitterServicesV32& out){
 if(out.destroy_base_v92)return;
 out.destroy_base_v92=[release,record](dh2::world::CanonicalSoundEmitterV32& actual,std::string& e){
  auto owner=release.lock();auto same=record.lock();
  if(!owner||!same||!same->owner||same->owner.get()!=&actual){e="Required SAME live qualified SoundEmitter GameObjectD2 owner";return false;}
  // Audio390 Stop and derived CString378 are ALREADY complete. This cannot
  // replay SoundEmitterD1 or a dispatch-to-derived deleting destructor.
  return owner->destroy_gameobject(actual.base(),e);
 };
}
// Provider is the existing CampaignNativeLendersV89 authority; record is the
// SAME admitted CanonicalSoundEmitterRecordV32. No Audio/World/Class allocation.
template<class Provider,class Record>
bool bind_campaign_sound_emitter_v92(const std::shared_ptr<Provider>& provider,
 std::weak_ptr<Record> record,dh2::world::SoundEmitterServicesV32& out,std::string& e){
 if(!provider){e="Required independent SoundEmitter source authority";return false;}
 const auto live=[provider,record](std::shared_ptr<Record>& r,
  std::shared_ptr<SourceWorldBorrowV61>& source,SourceCampaignCandidateBorrowV55& actual,std::string& e){
  r=record.lock();source=provider->source.lock();
  if(!r||!r->owner||!source||!source->application||!provider->current(actual,e)||source->application!=actual.application){if(e.empty())e="Required SAME live SoundEmitter record/App source delivery";return false;}return true;
 };
 dh2::world::SoundEmitterSourceServicesV92 source;source.owner=provider;
 source.current_level=[live](auto& out,std::string& e){
  std::shared_ptr<Record> record;std::shared_ptr<SourceWorldBorrowV61> source;SourceCampaignCandidateBorrowV55 actual;
  if(!live(record,source,actual,e))return false;
  dh2::loader::CanonicalCurrentLevelBorrowV1 current;if(!borrow_current_native_level_v27(current,e))return false;
  out={};auto level=current.level();if(!level){e.clear();return true;}
  auto fields=level->constructor_borrow_v3();if(!fields.owner||!fields.fields){e="Required actual SoundEmitter current Level130 cells";return false;}
  out={fields.owner,&fields.fields->field130};e.clear();return true;
 };
 source.local_player=[live](std::int32_t index,bool create,auto& out,std::string& e){
  std::shared_ptr<Record> record;std::shared_ptr<SourceWorldBorrowV61> source;SourceCampaignCandidateBorrowV55 actual;
  if(!live(record,source,actual,e))return false;auto pm=source->application->source_player_manager_v59();
  if(!pm||!pm->belongs_to_application(source->application)||!pm->manager()){e="Required SAME App40 SoundEmitter PlayerManager";return false;}
  dh2::player::PlayerInfoFieldsV1* player{};if(!pm->get_local_player(index,create,player,e))return false;
  if(!player){e="Original SoundEmitter NULL PlayerInfo before660";return false;}
  out={std::shared_ptr<void>(pm,player),&player->character660};e.clear();return true;
 };
 source.character_position=[live](std::uintptr_t id,auto& out,std::string& e){
  std::shared_ptr<Record> record;std::shared_ptr<SourceWorldBorrowV61> source;SourceCampaignCandidateBorrowV55 actual;
  if(!live(record,source,actual,e))return false;SourceCampaignCameraActorBorrowV67 character;
  if(!borrow_source_campaign_camera_actor_v67(source->owner,id,character,e)||!character.receiver||character.identity!=id||!character.position160){if(e.empty())e="Required SAME actual local Character160 source cells";return false;}
  out={character.receiver,character.position160};e.clear();return true;
 };
 const auto platform=out.platform_stop_v92;
 source.sound_manager=[live,platform](dh2::world::SoundEmitterAudioV92& out,std::string& e){
  out={};std::shared_ptr<Record> record;std::shared_ptr<SourceWorldBorrowV61> source;SourceCampaignCandidateBorrowV55 actual;
  if(!live(record,source,actual,e))return false;
  dh2::audio::AudioApplicationBorrowV42 captured;
  if(!dh2::android_audio::borrow_application_audio_v42(captured,e))return false;
  if(!captured.manager){e.clear();return true;} // genuine nullable singleton ONLY
  out.receiver=captured.manager;out.identity=captured.identity();
  const std::weak_ptr<dh2::audio::AudioApplicationManagerV42> manager=captured.manager;
  out.play3d=[live,manager](std::int32_t uid,const auto& position,bool loop,std::int32_t arg,float first,float second,std::string& e){
   std::shared_ptr<Record> record;std::shared_ptr<SourceWorldBorrowV61> source;SourceCampaignCandidateBorrowV55 actual;
   if(!live(record,source,actual,e))return false;auto owner=manager.lock();if(!owner){e="Captured SAME SoundEmitter manager retired";return false;}
   dh2::character::CombatSoundPlayV1 play;play.manager=owner->identity();play.target=record->owner->base().identity();play.sound_id=uid;
   play.position=position;play.source_bool=loop;play.source_integer=arg;play.source_float0=first;play.source_float1=second;
   dh2::audio::AudioApplicationBorrowV42 receipt;receipt.manager=owner;
   // Existing actual bridge owns all original disabled/negative/online/GS,
   // listener/settings/RNG/native emission gates and authored frame time.
   return submit_campaign_audio_v46(dh2::audio::AudioCategoryV46::ambience,receipt,source->owner,play,e);
  };
  out.stop3d=[live,manager,platform](std::int32_t uid,std::int32_t ms,const float* point,float radius,std::string& e){
   std::shared_ptr<Record> record;std::shared_ptr<SourceWorldBorrowV61> source;SourceCampaignCandidateBorrowV55 actual;
   if(!live(record,source,actual,e))return false;auto owner=manager.lock();if(!owner){e="Captured SAME Stop3D manager retired";return false;}
   return owner->stop_source_v92(uid,ms,point,radius,platform,e);
  };
  out.stop=[live,manager,platform](std::int32_t uid,std::int32_t ms,std::string& e){
   std::shared_ptr<Record> record;std::shared_ptr<SourceWorldBorrowV61> source;SourceCampaignCandidateBorrowV55 actual;
   if(!live(record,source,actual,e))return false;auto owner=manager.lock();if(!owner){e="Captured SAME Stop manager retired";return false;}
   return owner->stop_source_v92(uid,ms,nullptr,0.0f,platform,e);
  };
  e.clear();return true;
 };
 // Native class owns complete D1 ordering. Main supplies only its existing
 // inherited GameObjectD2 leaf/journal; graph cleanup alone cannot claim it.
 source.destroy_gameobject_base=out.destroy_base_v92;
 if(!out.whole_update)out.whole_update=[record,source](auto& actual,std::string& e){auto r=record.lock();if(!r||!r->owner||r->owner.get()!=&actual){e="Foreign SAME SoundEmitter Update receiver";return false;}return dh2::world::sound_emitter_update_v92(actual,source,e);};
 if(!out.whole_destroy)out.whole_destroy=[record,source](auto& actual,std::string& e){auto r=record.lock();if(!r||!r->owner||r->owner.get()!=&actual){e="Foreign SAME SoundEmitter D1 receiver";return false;}return dh2::world::sound_emitter_destroy_v92(actual,source,e);};
 e.clear();return true;
}
}
