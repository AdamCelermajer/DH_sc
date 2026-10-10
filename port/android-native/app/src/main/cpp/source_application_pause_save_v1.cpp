#include "source_application_pause_save_v1.hpp"
#include "model_renderer.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "source_campaign_conditions_v70.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_settings_update_job_v102.hpp"
#include "native_gslevel_runtime_v27.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "application_player_manager_bootstrap_v59.hpp"
#include "application_save_files_owner_v61.hpp"
#include "player_save_write_owner_v1.hpp"
#include "application_pause_save_v1.hpp"
#include <jni.h>
#include <ctime>
#include <memory>

namespace model_renderer { namespace {
bool save_player_zero(const SourceCampaignCandidateBorrowV55& candidate,
 const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>& level,std::string& error){
 std::shared_ptr<SourceWorldBorrowV61> world;
 if(!level||candidate.level!=level||!borrow_source_campaign_condition_world_v70(candidate,world,error)||
    !world->player_manager||!world->player_manager->manager()){
  if(error.empty())error="Required SAME source Level/PlayerManager for SG_SavePlayer(0,false)";
  return false;
 }
 auto* manager=world->player_manager->manager();std::int32_t count{};
 if(!manager->num_players(count,error))return false;
 // Level::SG_SavePlayer(int,bool) checks the source player count before its
 // GetPlayer(0,true) call. A genuinely empty roster is therefore a no-op.
 if(count<=0){error.clear();return true;}
 dh2::player::PlayerInfoFieldsV1* info{};
 if(!manager->get_player(0,true,info,error))return false;
 if(!info||!info->character660){error.clear();return true;}
 SourceCampaignCharacterBorrowV62 borrowed;
 if(!borrow_source_campaign_character_v62(world->owner,info->character660,borrowed,error)||!borrowed.character)return false;
 auto record=borrowed.character;bool is_player{};
 // SG_Save(Character*,false) begins with Character's vtable+0x28 IsPlayer.
 if(!record->is_player(is_player,error))return false;
 if(!is_player){error.clear();return true;}
 if(!record->save_fields||!record->save_fields->save_slot14e8()){
  error="Required same Character14e8 field for SG_SavePlayer";return false;
 }
 // Character::SG_Save has a guarded NULL-save return. Preserve it instead of
 // manufacturing a profile or another Save authority.
 if(!*record->save_fields->save_slot14e8()){error.clear();return true;}
 if(!record->save||!record->load||
    *record->save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(record->save.get())||
    &record->load->save()!=record->save.get()||!record->properties){
  error="SG_SavePlayer requires the SAME published Character14e8/Save/load/property owners";return false;
 }
 const bool was_blocked=record->load->save_disabled();
 struct RestoreBlock {
  std::shared_ptr<dh2::data::PlayerSaveLoadOwnerV1> load;bool value;
  ~RestoreBlock(){if(load)load->source_block_store_v88(value);}
 } restore{record->load,was_blocked};
 // Match SG_Save(Character*,false): current Character::GetLevel, save date,
 // Level+0x110 entry point with process difficulty, then Character::SG_Save.
 record->save->set_player_level(record->properties->resolved[19]>>8);
 record->save->source_save_date_store_v88(static_cast<std::uint32_t>(std::time(nullptr)));
 if(!source_campaign_character_save_entrypoint_v68(world->owner,info->character660,
     level->constructor_fields_v3().level110,-1,error))return false;
 dh2::data::PlayerSaveWriteServicesV1 services;
 if(record->load->profile().identity){
  auto writer=record->profile_bootstrap?record->profile_bootstrap->campaign_writer():nullptr;
  if(!writer||!writer->ready()){
   error="SG_SavePlayer requires the existing ready fifteen-section campaign writer";return false;
  }
  services=writer->write_services();
 }
 dh2::data::PlayerSaveWriteOwnerV1 writer(record->load,std::move(services));
 if(!writer.save(error))return false;
 error.clear();return true;
}
}}

bool model_renderer::source_application_pause_save_v1(std::string& error){
 using namespace dh2;
 loader::CanonicalCurrentLevelBorrowV1 current;
 if(!borrow_current_native_level_v27(current,error))return false;
 std::shared_ptr<loader::CanonicalLevelContextV1> level=current?current.level():nullptr;
 std::uint32_t phase{};std::uint8_t active{};
 if(level){const auto& fields=level->constructor_fields_v3();phase=fields.field130;active=fields.byte144;}
 const auto save_player=[&](const std::shared_ptr<void>& actual,std::int32_t index,bool block,std::string& e){
  if(index!=0||block){e="nativePause must dispatch SG_SavePlayer(0,false) on the same current Level";return false;}
  SourceCampaignCandidateBorrowV55 candidate;
  if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.level.get()!=actual.get()){
   if(e.empty())e="Required SAME live campaign candidate for focus-loss player save";return false;
  }
  return save_player_zero(candidate,level,e);
 };
 // The helper encodes Application::Pause's exact +0x130/+0x144 guard and
 // passes the raw original SG_SavePlayer arguments (player 0, false).
 if(!application::application_pause_save_player_v1(level,phase,active,save_player,error))return false;

 std::shared_ptr<application::ApplicationServicesOwnerV5> app;
 if(!borrow_actual_application_services_v5(app,error)||!app){
  if(error.empty())error="Required actual process Application for pause settings";return false;
 }
 // Application::Pause saves settings only when SavegameManager+0x28 is dirty.
 if(!save_process_settings_v102(app,error))return false;
 // Source Pause itself does not FlushJobs. This Android adapter suspends its
 // only frame pump immediately after this queued GL event, so drain the SAME
 // process queue after profile/settings serialization; a successful enqueue
 // alone is not durable. FlushJobs(nullptr) is the existing process-wide
 // barrier, matching Application::Quit's queue owner and lifetime.
 if(level&&phase==38&&active){
  auto files=app->source_save_files_v61();
  if(!files||!files->belongs_to_application(app)){
   error="Required SAME Application FileManager/jobs for focus-loss flush";return false;
  }
  if(!files->flush(nullptr,error))return false;
 }
 error.clear();return true;
}

extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_sourceApplicationPauseSaveV1(JNIEnv* env,jclass){
 std::string error;
 if(!model_renderer::source_application_pause_save_v1(error)){
  if(error.empty())error="source save failed";
  const auto report=std::string("nativePause/appPause source save failed: ")+error;
  return env->NewStringUTF(report.c_str());
 }
 return env->NewStringUTF("nativePause/appPause source save completed");
}
