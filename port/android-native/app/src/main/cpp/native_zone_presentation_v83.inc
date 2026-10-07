// Included at native_app's global scope after native_menu_resources_v93.inc.
// Source GetHUDRoot42cb8c reads primary3/root140. No movie/menu C1, frame,
// resource reload or alternate UI art occurs in this binder.
#include "source_campaign_zones_v83.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_conditions_v70.hpp"
#include <cstring>
bool model_renderer::bind_native_zone_presentation_v83(const std::shared_ptr<void>& actual_world,
 dh2::world::ExitZoneRuntimeServicesV83& out,std::string& error){
 if(!actual_world){error="Required actual campaign World for FastTravel UI binding";return false;}
 std::weak_ptr<void> weak=actual_world;
 out.level_display_name=[weak](const char* name,std::string& title,std::string& e){
  auto world=weak.lock();if(!world||!name){e="Expired source FastTravel localization World/CString";return false;}
  model_renderer::SourceCampaignCandidateBorrowV55 candidate;
  if(!model_renderer::borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world||!candidate.application){e="FastTravel localization candidate changed";return false;}
  std::shared_ptr<model_renderer::SourceWorldBorrowV61> same;
  if(!model_renderer::borrow_source_campaign_condition_world_v70(candidate,same,e)||!same->design)return false;
  auto design=same->design->borrow();auto table=design.levels();if(!table){e="Required actual Arrays.LevelList";return false;}
  std::int32_t index=-1;for(std::size_t i=0;i<table->level_names.size();++i)if(table->level_names[i]==name){index=static_cast<std::int32_t>(i);break;}
  if(index<0||std::size_t(index)>=table->levels.size()){e="Original FastTravel missing LevelList name would dereference row−1";return false;}
  std::int32_t id{};const auto word=table->levels[std::size_t(index)].scalar.words[0x24/4];std::memcpy(&id,&word,4);
  return original_ui.source_integer_string_v83(id,title,e);
 };
 out.display_fasttravel=[weak](bool visible,const char* title,const char* level,std::int32_t entry,std::string& e){
  auto world=weak.lock();std::shared_ptr<void> menu;
  if(!world||!model_renderer::borrow_native_menu_manager_v58(world,menu,e)){if(e.empty())e="Released actual FastTravel HUD menu authority";return false;}
  return original_ui.display_fast_travel_v83(visible,title,level,entry,e);
 };
 error.clear();return true;
}
