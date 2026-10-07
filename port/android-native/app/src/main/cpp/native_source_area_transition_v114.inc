// Process-owned sequencing; the existing retirement/load owners do the work.
#include "source_campaign_transition_restore_v114.hpp"
#include "native_source_main_menu_v114.hpp"
#include "renderer_area_transition_projection_v114.hpp"
#include <canonical_character_candidate_v60.hpp>
namespace {
std::unique_ptr<dh2::loader::AreaTransitionSequenceV114> native_area_transition_v114;
bool drain_native_area_transition_frame_v114(){
 using Sequence=dh2::loader::AreaTransitionSequenceV114;
 using Step=dh2::loader::AreaTransitionStepV114;
 if(!native_area_transition_v114)return false;
 std::string error;
 const auto before=native_area_transition_v114->phase();
 const auto result=native_area_transition_v114->drain(error);
 if(result==Step::failed){
  const auto failure="Source area transition: "+error;
  if(original_ui_error!=failure)__android_log_print(ANDROID_LOG_ERROR,tag,"%s",failure.c_str());
  original_ui_error=failure;return true;
 }
 if(result==Step::complete){
  native_area_transition_v114.reset();return false;
 }
 const auto after=native_area_transition_v114->phase();
 if(after==Sequence::Phase::await_native_completion){
  // Startup was attempted once. The ordinary native frame must now advance
  // its real loading/restore pipeline; completion polling cannot drive it.
  if(before!=after)original_ui_error.clear();
  return false;
 }
 return true; // Old retirement still owns the frame, without a second drain.
}
}
bool model_renderer::enqueue_confirmed_source_area_transition_v114(
 std::shared_ptr<const dh2::loader::AreaTransitionRequestV114> request,
 dh2::loader::AreaTransitionServicesV114 services,std::string& error){
 using Sequence=dh2::loader::AreaTransitionSequenceV114;
 if(source_main_menu_transition_pending_v114()){
  error="An accepted main-menu return owns the native transition";return false;
 }
 if(native_area_transition_v114&&native_area_transition_v114->phase()!=Sequence::Phase::complete){
  error="An accepted area transition still owns its native continuation";return false;
 }
 auto next=std::make_unique<Sequence>();
 const bool accepted=next->accept(std::move(request),std::move(services),error);
 // Failed admission may have produced a real retirement prefix. Retain it
 // for the same failure continuation instead of discarding/replaying it.
 if(accepted||next->phase()!=Sequence::Phase::idle)native_area_transition_v114=std::move(next);
 return accepted;
}
bool model_renderer::resume_source_area_transition_retirement_v114(std::string& error){
 if(!native_area_transition_v114){error="No retained area retirement to resume";return false;}
 return native_area_transition_v114->resume_retirement(error);
}
bool model_renderer::request_confirmed_source_area_load_v114(
 const dh2::loader::ApplicationLoadLevelArgumentsV114& arguments,std::string& error){
 using namespace dh2::loader;
 if(source_main_menu_transition_pending_v114()){
  error="Cannot save or load another area during accepted main-menu return";return false;
 }
 SourceCampaignCandidateBorrowV55 current;PlayerGameplayBinding player;
 if(!gameplay_assets||!borrow_source_campaign_candidate_v55(current,error)||
    !borrow_source_campaign_player_gameplay_v67(current.actual_world,player,error)||
    !player.active||!player.save||arguments.profile_slot!=player.save->slot()){
  if(error.empty())error="Confirmed LoadLevel slot differs from current selected source profile";return false;
 }
 AreaTransitionRestoreReceiptV114 restore;
 if(!capture_source_campaign_transition_restore_v114(current.actual_world,restore,error))return false;
 std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50> profile;
 if(!restore.file){error="Confirmed transition has no serialized selected profile";return false;}
 if(!front_ui.project_selected_profile_file_v114(current.application,restore.profile_slot,
    player.difficulty,*restore.file,profile,error))return false;
 if(!profile||profile->private_directory!=restore.private_directory){
  error="Confirmed transition profile does not belong to the current save directory";return false;
 }
 AreaTransitionConfirmationV114 confirmed;
 confirmed.destination=arguments.filename;confirmed.destination_domain=AreaTransitionDestinationDomainV114::raw_filename;
 confirmed.profile_slot=arguments.profile_slot;confirmed.entry=arguments.entry;
 confirmed.flag_f1=arguments.flag_f1;confirmed.use_spawn_point=arguments.use_spawn_point;
 confirmed.mode=arguments.mode;confirmed.bypass_saved_seed=arguments.bypass_saved_seed;
 confirmed.explicit_seed24=arguments.seed24;confirmed.explicit_seed28=arguments.seed28;
 const auto old=std::weak_ptr<void>(current.actual_world);
 AreaTransitionProjectionServicesV114 leaves;
 leaves.current=[old](auto& e){auto world=old.lock();SourceCampaignCandidateBorrowV55 actual;
  if(!world||source_campaign_retirement_requested_v88()||!borrow_source_campaign_candidate_v55(actual,e)||actual.actual_world!=world){if(e.empty())e="Confirmed transition campaign changed";return false;}return true;};
 leaves.debug_load=[](auto& e){std::int32_t ignored{};if(!authored_character_menu){e="Required actual process transition Debug owner";return false;}return authored_character_menu->debug(nullptr,ignored,e);};
 leaves.debug_switch=[](const char* key,bool& value,auto& e){std::int32_t result{};if(!authored_character_menu||!authored_character_menu->debug(key,result,e))return false;value=result!=0;return true;};
 leaves.real_time=[](auto& value,auto& e){value=std::uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());e.clear();return true;};
 std::shared_ptr<const AreaTransitionRequestV114> request;
 if(!project_source_campaign_area_transition_v114(current,confirmed,std::move(restore),std::move(leaves),request,error))return false;
 struct Delivery {
  std::weak_ptr<void> old;
  std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> app;
  std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50> profile;
  std::shared_ptr<const AreaTransitionRequestV114> request;
  AAssetManager* assets{};
 };
 auto delivery=std::make_shared<Delivery>();delivery->old=old;delivery->app=current.application;
 delivery->profile=std::move(profile);delivery->request=request;delivery->assets=gameplay_assets;
 AreaTransitionServicesV114 services;services.owner=delivery;
 services.begin_retirement=[delivery](const auto& requested,auto& e){auto world=delivery->old.lock();
  if(requested!=delivery->request||!world){e="Accepted transition lost its actual old campaign";return false;}
  return request_native_source_campaign_retirement_v104(world,e);};
 services.drain_retirement=[](auto& e){const auto result=drain_source_campaign_retirement_v88(e);
  return result==SourceCampaignRetirementResultV88::complete?AreaTransitionStepV114::complete:
   result==SourceCampaignRetirementResultV88::pending?AreaTransitionStepV114::pending:AreaTransitionStepV114::failed;};
 services.retry_retirement=[](auto& e){return retry_source_campaign_retirement_v114(e);};
 services.start_native=[delivery](const auto& requested,auto& e){
  if(requested!=delivery->request||!delivery->app.lock()){e="Actual transition process authority expired";return false;}
  return start_source_campaign_v55(delivery->assets,delivery->profile,e,requested.get());};
 services.native_completion=[delivery](const auto& requested,auto& e){
  std::shared_ptr<void> world;bool complete{};
  if(requested!=delivery->request||!source_campaign_profile_loading_outcome_v114(delivery->profile,world,complete,e))return AreaTransitionStepV114::failed;
  if(!complete)return AreaTransitionStepV114::pending;
  SourceCampaignCandidateBorrowV55 actual;PlayerGameplayBinding restored;SourceCampaignCharacterBorrowV62 character;
  if(!world||!borrow_source_campaign_candidate_v55(actual,e)||actual.application!=delivery->app.lock()||
     actual.selected_profile!=delivery->profile||!borrow_source_campaign_player_gameplay_v67(world,restored,e)||
     !restored.active||!restored.save||restored.save->slot()!=requested->restore.profile_slot||
     restored.difficulty!=requested->source_difficulty||!borrow_source_campaign_character_v62(world,restored.character,character,e)||
     !character.character->profile_bootstrap||!character.character->profile_bootstrap->finished()){
   if(e.empty())e="Actual destination load has not restored the same logical selected profile";return AreaTransitionStepV114::failed;
  }
  e.clear();return AreaTransitionStepV114::complete;
 };
 return enqueue_confirmed_source_area_transition_v114(std::move(request),std::move(services),error);
}
