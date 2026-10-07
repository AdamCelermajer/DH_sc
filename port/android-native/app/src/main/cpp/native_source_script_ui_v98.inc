// Included after NativeMenuResourcesV93: one retained movie-slot directory.
namespace {
bool invoke_source_menu_v98(const std::shared_ptr<void>& world,std::uint32_t slot,
 const char* path,const char* method,const std::vector<dh2::ui::SwfAsValue>& arguments,std::string& e){
 NativeMenuResourcesV93 resources;resources.owner=authored_character_menu;resources.world=world;
 dh2::ui::MenuMovieBorrowV58 borrow;
 if(!resources.movie_slot(slot,borrow,e))return false;
 if(!borrow.identity){e.clear();return true;} // Actual NULL primary movie.
 std::uint32_t actual_slot{};dh2::ui::SwfMovie* movie{};std::shared_ptr<void> pin;
 if(!resources.typed_movie(borrow,actual_slot,movie,pin,e))return false;
 if(actual_slot!=slot||!movie){e="Source callback movie changed primary slot";return false;}
 struct Call {const char* path;const char* method;const std::vector<dh2::ui::SwfAsValue>& arguments;} call{path,method,arguments};
 return movie->menu_action_script(&call,[](void* raw,dh2::ui::SwfAsGraph& graph,std::string& error){
  auto& q=*static_cast<Call*>(raw);return graph.invoke_renderfx(q.path,q.method,q.arguments,error);
 },e);
}
}
bool model_renderer::source_native_character_spec_time_v96(const std::shared_ptr<void>& world,bool value,std::string& e){
 return invoke_source_menu_v98(world,1,"_root.menu_CharacterMenu","IsSpecTime",{dh2::ui::SwfAsValue::boolean(value)},e);
}
bool model_renderer::source_native_igm_opened_v98(const std::shared_ptr<void>& world,bool& value,std::string& e){
 NativeMenuResourcesV93 resources;resources.owner=authored_character_menu;resources.world=world;
 auto native=resources.resolve(e);if(!native)return false;
 value=native->fields.igm_opened()!=0;e.clear();return true;
}
bool model_renderer::source_native_cutscene_popmenu_v99(const std::shared_ptr<void>& world,bool all,std::string& e){
 NativeMenuResourcesV93 resources;resources.owner=authored_character_menu;resources.world=world;
 auto native=resources.resolve(e);if(!native)return false;
 if(!native->shared_roster_v27){e="Missing actual cutscene MultiMenu lifecycle receiver";return false;}
 const auto status=native->shared_stack_v27->pop_current(all,native->shared_roster_v27->stack_services());
 if(status){e=native->shared_roster_v27->error();if(e.empty())e="Original cutscene PopMenu failed: "+std::to_string(status);return false;}
 e.clear();return true;
}
bool model_renderer::borrow_native_last_open_menu_id_v118(std::int32_t& value,std::string& e){
 if(!authored_character_menu||!authored_character_menu->shared_stack_v27||
    !authored_character_menu->shared_stack_v27->view()||!authored_character_menu->shared_stack_v27->view()->globals){
  e="Required same process MenuGlobals.last_open_menu";return false;
 }
 value=authored_character_menu->shared_stack_v27->view()->globals->last_open_menu;e.clear();return true;
}
bool model_renderer::enqueue_source_tutorial_v118(const std::shared_ptr<void>& world,
 std::int32_t text,std::int32_t duration,std::string& e){
 return original_ui.enqueue_tutorial_v118(world,text,duration,e);
}
bool model_renderer::skip_source_tutorial_v118(const std::shared_ptr<void>& world,bool all,std::string& e){
 return original_ui.skip_tutorial_v118(world,all,e);
}
bool model_renderer::character_tutorial_operation_v118(const std::shared_ptr<void>& world,std::int32_t kind,
 std::int32_t id,const char* frame,const char* menu,std::string& e){
 SourceCampaignCandidateBorrowV55 current;
 if(!borrow_source_campaign_candidate_runtime_v61(current,e)||current.actual_world!=world)return false;
 if(kind==74)return character_tutorial_queue.enqueue_script_command(id,frame,menu,e);
 if(kind==76){character_tutorial_queue.flush();e.clear();return true;}
 if(kind!=75){e="Unknown source character tutorial operation";return false;}
 return character_tutorial_queue.skip([](const char* function,std::string& e){
  if(!character_panel){e="Required actual CharacterMenu tutorial callback graph";return false;}
  struct Call {const char* function;static bool run(void* raw,dh2::ui::SwfAsGraph& graph,std::string& e){
   auto& call=*static_cast<Call*>(raw);dh2::ui::SwfAsValue root,result;bool callable{};
   if(!graph.root_value(root,e)||!graph.invoke(root,root,call.function,{},result,callable,e))return false;
   if(!callable){e="Required original character tutorial skip callback";return false;}return true;
  }} call{function};
  return character_panel->authored_graph_scope_v4(&call,Call::run,e);
 },e);
}
bool model_renderer::bind_native_source_script_ui_v98(const SourceCampaignCandidateBorrowV55& candidate,std::string& e){
 dh2::android_ui::SourceScriptUiLeavesV96 ui;
 dh2::android_ui::SourceScriptActorLeavesV96 actors;
 if(!build_native_source_script_ui_leaves_v99(candidate,ui,e)||
    !build_source_campaign_script_actor_leaves_v96(candidate,actors,e)||
    !enrich_source_campaign_script_runtime_v99(candidate,ui,e))return false;
 std::weak_ptr<void> world=candidate.actual_world;
 ui.hud_callback_noargs=[world](const char* method,std::string& error){
  auto actual=world.lock();if(!actual){error="Expired actual script HUD World";return false;}
  return invoke_source_menu_v98(actual,3,"_root",method,{},error);
 };
 return dh2::android_ui::bind_source_campaign_script_execution_v96(candidate.actual_world,std::move(ui),e,std::move(actors));
}
bool model_renderer::build_native_source_script_ui_leaves_v99(const SourceCampaignCandidateBorrowV55& candidate,
 dh2::android_ui::SourceScriptUiLeavesV96& out,std::string& e){
 return original_ui.source_script_ui_leaves_v97(candidate,out,e);
}

bool model_renderer::source_native_quest_completed_dialog_v108(const std::shared_ptr<void>& world,std::int32_t title,std::int32_t style,const std::vector<dh2::data::QuestRewardDefinitionV51>& rewards,std::string& e){
 SourceCampaignCandidateBorrowV55 c;
 if(!borrow_source_campaign_candidate_runtime_v61(c,e)||c.actual_world!=world){if(e.empty())e="Quest dialog belongs to retired campaign";return false;}
 return original_ui.quest_completed_dialog_v108(title,style,rewards,e);
}

#include "source_campaign_character_interaction_v114.hpp"
bool model_renderer::bind_native_character_interaction_ui_v115(const std::shared_ptr<void>& world,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;dh2::ui::SwfMovie* hud{};std::shared_ptr<void> ui_owner;
 if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world||
    !original_ui.hud_movie_borrow_v4(hud,ui_owner,e))return false;
 const auto expected_hud=reinterpret_cast<std::uintptr_t>(hud);const auto weak=std::weak_ptr<void>(world);
 return bind_source_campaign_character_interaction_ui_v114(world,ui_owner,[weak,expected_hud](const SourceCharacterInteractionMenuRequestV114& request,std::string& e){
  auto world=weak.lock();if(!world){e="Expired actual Character interaction menu World";return false;}
  NativeMenuResourcesV93 resources;resources.owner=authored_character_menu;resources.world=world;
  const bool information=request.operation==SourceCharacterInteractionMenuV114::merchant_information;
  const std::uint32_t slot=information?1:3;dh2::ui::MenuMovieBorrowV58 source;
  if(!resources.movie_slot(slot,source,e))return false;
  if(!source.identity||(!information&&source.identity!=expected_hud)){
   e="Required original positive interaction HUD/CharacterMenu movie generation";return false;
  }
  std::uint32_t actual_slot{};dh2::ui::SwfMovie* movie{};std::shared_ptr<void> movie_owner;
  if(!resources.typed_movie(source,actual_slot,movie,movie_owner,e)||actual_slot!=slot||!movie){
   if(e.empty())e="Interaction source renderer differs from original primary slot";return false;
  }
  const char* method{};std::vector<dh2::ui::SwfAsValue> arguments;
  switch(request.operation){
  case SourceCharacterInteractionMenuV114::open_merchant:method="openMerchantMenu";arguments.push_back(dh2::ui::SwfAsValue::number(request.friendly678));break;
  case SourceCharacterInteractionMenuV114::open_cleaner:method="openCleanerMenu";arguments.push_back(dh2::ui::SwfAsValue::number(request.friendly678));break;
  case SourceCharacterInteractionMenuV114::merchant_information:
   if(!request.name44){e="Required actual Merchant name44 string";return false;}
   method="AdditionnalInfosForMerchant";arguments.push_back(dh2::ui::SwfAsValue::text(request.name44));arguments.push_back(dh2::ui::SwfAsValue::number(request.oid64));break;
  }
  if(!method){e="Unknown source Character interaction UI operation";return false;}
  struct Call {const char* method;const std::vector<dh2::ui::SwfAsValue>& arguments;} call{method,arguments};
  //GetHUDRoot/_root and separate CharacterMenu/_root are the actual original
  //receivers. No new menu/stock/player selector or inventory is created here.
  return movie->menu_action_script(&call,[](void* raw,dh2::ui::SwfAsGraph& graph,std::string& e){
   auto& call=*static_cast<Call*>(raw);return graph.invoke_renderfx("_root",call.method,call.arguments,e);
  },e);
 },e);
}
