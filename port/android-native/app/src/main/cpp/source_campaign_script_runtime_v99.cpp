#include "source_campaign_script_runtime_v99.hpp"
#include "native_source_script_ui_v98.hpp"
#include "model_renderer.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "application_player_manager_bootstrap_v59.hpp"
#include "visual_anim_controller_owner_v4.hpp"
namespace model_renderer {
bool prepare_source_campaign_script_start_v99(const SourceCampaignCandidateBorrowV55& c,std::string& e){
 dh2::android_ui::SourceScriptUiLeavesV96 ui;
 if(!build_native_source_script_ui_leaves_v99(c,ui,e)||!enrich_source_campaign_script_runtime_v99(c,ui,e))return false;
 // Parsing remains actual Stage8. This enrolls the same services over the
 // currently owned (initially empty) command directory. It executes no
 // command/C1/Start, and Stage26 borrows every surviving source context.
 return dh2::android_ui::bind_source_campaign_script_execution_v96(c.actual_world,std::move(ui),e);
}
bool enrich_source_campaign_script_runtime_v99(const SourceCampaignCandidateBorrowV55& c,
 dh2::android_ui::SourceScriptUiLeavesV96& ui,std::string& e){
 SourceCampaignCandidateBorrowV55 current;
 if(!c.actual_world||!c.application||!ui.owner||!borrow_source_campaign_candidate_runtime_v61(current,e)||
    current.actual_world!=c.actual_world||current.application!=c.application){
  if(e.empty())e="Required SAME actual campaign/UI provider for script runtime enrollment";return false;
 }
 const auto world=std::weak_ptr<void>(c.actual_world);
 const auto app=std::weak_ptr<dh2::application::ApplicationServicesOwnerV5>(c.application);
 auto actual=[world,app](SourceCampaignCandidateBorrowV55& out,std::string& error){
  auto w=world.lock();auto a=app.lock();
  if(!w||!a||!borrow_source_campaign_candidate_runtime_v61(out,error)||out.actual_world!=w||out.application!=a){
   if(error.empty())error="Expired/replaced source script campaign";return false;
  }return true;
 };
 ui.force_cutscene_ui=[actual](bool& result,std::string& error){
  SourceCampaignCandidateBorrowV55 c;if(!actual(c,error))return false;
  return source_native_igm_opened_v98(c.actual_world,result,error);
 };
 ui.menu_f4_virtual38=[actual](bool all,std::string& error){
  SourceCampaignCandidateBorrowV55 c;if(!actual(c,error))return false;
  // The actual f4 receiver is MultiMenuManager, whose selected virtual38
  // is whole PopMenu438c14. It is neither MenuBase nor movie visibility.
  return source_native_cutscene_popmenu_v99(c.actual_world,all,error);
 };
 ui.store_display_hud=[actual](std::uint8_t value,std::string& error){
  SourceCampaignCandidateBorrowV55 c;if(!actual(c,error))return false;
  dh2::world::source_store_animation_scaling_v99(value);error.clear();return true;
 };
 ui.player_set_in_cutscene=[actual](dh2::player::PlayerInfoFieldsV1& player,bool value,std::string& error){
  SourceCampaignCandidateBorrowV55 c;if(!actual(c,error))return false;
  auto pm=c.application->source_player_manager_v59();
  if(!pm||!pm->network()){error="Required SAME App PM network member owner";return false;}
  return pm->network()->source_set_in_cutscene_v99(player,value,error);
 };
 ui.reset_dead_local_player=[actual](dh2::player::PlayerInfoFieldsV1& player,std::string& error){
  SourceCampaignCandidateBorrowV55 c;if(!actual(c,error))return false;
  auto pm=c.application->source_player_manager_v59();
  if(!pm||!pm->network()){error="Required SAME App PM death member owner";return false;}
  return pm->network()->source_reset_dead_local_v99(player,error);
 };
 e.clear();return true;
}
}
