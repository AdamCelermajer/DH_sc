// Included in native_app.cpp after native_source_area_transition_v114.inc.
#include "native_source_main_menu_v114.hpp"
#include <application_go_to_main_menu_v114.hpp>
#include "source_main_menu_leaves_v114.hpp"
#include "source_main_menu_process_v114.hpp"
#include "source_android_notify_trophy_v114.hpp"
namespace {
using MainMenuStepV114=dh2::loader::AreaTransitionStepV114;
struct NativeSourceMainMenuV114 {
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> app;
 std::weak_ptr<NativeCharacterMenuV4> native;
 std::weak_ptr<dh2::ui::MenuStackOwnerV1> manager;
 std::weak_ptr<void> old_world;
 bool retirement_begun{},retirement_complete{},entry_complete{};

 bool current(std::string& e)const{
  auto a=app.lock();auto n=native.lock();auto m=manager.lock();
  if(!a||a!=application_services_v5||!n||n!=authored_character_menu||!m||
     n->process_application_v104.lock()!=a||n->shared_stack_v27!=m||!n->process_directory_ready_v104){
   e="Main-menu return lost its same process Application/MenuManager";return false;
  }
  e.clear();return true;
 }
 MainMenuStepV114 switch_state(const dh2::loader::MainMenuApplicationBorrowV114& a,
  const dh2::loader::MainMenuStateBorrowV114& s,bool previous,std::string& e){
  if(!current(e)||previous||a.identity!=app.lock()->identity()){
   if(e.empty())e="Main-menu return addressed a different actual state-switch receiver";return MainMenuStepV114::failed;
  }
  dh2::loader::MainMenuStateBorrowV114 actual;
  if(!front_ui.borrow_main_menu_state_v114(actual,e)||actual.identity!=s.identity||
     actual.menu_c!=s.menu_c||actual.source29!=s.source29){
   if(e.empty())e="Actual GSFlashMenu state replaced before SwitchState";return MainMenuStepV114::failed;
  }
  if(!retirement_begun){
   auto w=old_world.lock();
   if(w){if(!model_renderer::request_native_source_campaign_retirement_v104(w,e))return MainMenuStepV114::failed;}
   else if(model_renderer::source_campaign_active_v55()){
    e="Main-menu return lost the accepted old campaign before retirement";return MainMenuStepV114::failed;
   }else retirement_complete=true; // Real process currentLevel/World absent.
   retirement_begun=true;
  }
  if(!retirement_complete){
   const auto r=model_renderer::drain_source_campaign_retirement_v88(e);
   if(r==model_renderer::SourceCampaignRetirementResultV88::failed)return MainMenuStepV114::failed;
   if(r==model_renderer::SourceCampaignRetirementResultV88::pending)return MainMenuStepV114::pending;
   retirement_complete=true; // Cache the real completion before further leaves.
  }
  if(!entry_complete){
   if(!current(e)||!front_ui.enter_main_menu_state_v114(s,e))return MainMenuStepV114::failed;
   entry_complete=true;
  }
  e.clear();return MainMenuStepV114::complete;
 }
};
// Leaves borrow genuine process owners. Positive platform work remains a
// reached failure until its real native producer is enrolled, never a no-op.
dh2::application::MainMenuProcessLeavesV114 main_menu_process_leaves_v114(const std::shared_ptr<NativeSourceMainMenuV114>& d){
 struct ProcessLoan {std::weak_ptr<NativeSourceMainMenuV114> delivery;std::uint32_t trophy_count{};};
 auto loan=std::make_shared<ProcessLoan>();loan->delivery=d;
 dh2::application::MainMenuProcessLeavesV114 s;s.provider=loan;
 s.notify_trophy=[loan](std::int32_t id,std::string& e){
  auto d=loan->delivery.lock();if(!d||!d->current(e))return false;
  return dh2::android_ui::source_android_notify_trophy_v114(front_directory,id,e);
 };
 s.current=[loan](auto& e){auto d=loan->delivery.lock();if(!d){e="Expired original main-menu process delivery";return false;}return d->current(e);};
 s.properties=[loan](auto id,auto& pin,auto*& out,auto& e){auto d=loan->delivery.lock();
  if(!d||!d->current(e))return false;
  std::shared_ptr<void> world;model_renderer::SourceCampaignCharacterBorrowV62 actor;
  if(!model_renderer::borrow_source_campaign_world_owner_v104(world,e)||
     !model_renderer::borrow_source_campaign_character_v62(world,id,actor,e)||!actor.character){
   if(e.empty())e="Positive PM score Character has no actual retained source receiver";return false;
  }
  pin=actor.character;out=&actor.character->view;return true;
 };
 s.trophies=[loan](auto& pin,auto*& manager,auto*& count,auto& e){auto d=loan->delivery.lock();
  if(!d||!d->current(e))return false;
  dh2::android_ui::ProcessTrophyBorrowV100 actual;
  if(!dh2::android_ui::borrow_source_process_trophies_v100(d->app.lock(),actual,e)||!actual.owner||!actual.manager)return false;
  loan->trophy_count=static_cast<std::uint32_t>(actual.manager->catalog().rows().size());
  pin=actual.owner;manager=actual.manager;count=&loan->trophy_count;return true;
 };
 return s;
}
std::shared_ptr<NativeSourceMainMenuV114> native_source_main_menu_v114;
std::unique_ptr<dh2::loader::ApplicationGoToMainMenuV114> native_source_main_menu_command_v114;
bool drain_native_source_main_menu_frame_v114(){
 if(!native_source_main_menu_v114)return false;
 auto accepted=native_source_main_menu_v114;std::string e;
 const auto result=native_source_main_menu_command_v114->drain(e);
 if(result==MainMenuStepV114::complete){
  native_source_main_menu_command_v114.reset();native_source_main_menu_v114.reset();original_ui_error.clear();
  return false; // The existing Front/native frame now draws the actual menu.
 }
 if(result==MainMenuStepV114::failed){
  const auto failure="Source main-menu return: "+e;
  if(original_ui_error!=failure)__android_log_print(ANDROID_LOG_ERROR,tag,"%s",failure.c_str());
  original_ui_error=failure;
 }
 return true; // SAME accepted command exclusively owns pending/failed frame.
}
}
bool model_renderer::source_main_menu_transition_pending_v114()noexcept{
 return bool(native_source_main_menu_v114);
}
bool model_renderer::request_source_main_menu_v114(std::int32_t event,std::string& e){
 using namespace dh2::loader;
 if(native_source_main_menu_v114||native_area_transition_v114||source_campaign_retirement_requested_v88()){
  e="An accepted native transition already owns its continuation";return false;
 }
 if(!application_services_v5||!authored_character_menu||!authored_character_menu->shared_stack_v27||
    !authored_character_menu->process_directory_ready_v104||
    authored_character_menu->process_application_v104.lock()!=application_services_v5){
  e="Required actual retained process App/MenuManager for main-menu return";return false;
 }
 auto d=std::make_shared<NativeSourceMainMenuV114>();d->app=application_services_v5;
 d->native=authored_character_menu;d->manager=authored_character_menu->shared_stack_v27;
 if(source_campaign_active_v55()){
  std::shared_ptr<void> w;if(!borrow_source_campaign_world_owner_v104(w,e))return false;d->old_world=w;
 }
 ApplicationMainMenuServicesV114 s;s.owner=d;
 s.submit_online_scores=false; // Explicit offline Act 1 policy; networking is deferred.
 s.application=[d](auto& out,auto& e){if(!d->current(e))return false;auto a=d->app.lock();auto& f=a->source_loading_v55();
  out={a,a->identity(),&f.byte_ab,&f.event_b0};return true;};
 s.current_application=[d](const auto& out,auto& e){if(!d->current(e))return false;auto a=d->app.lock();auto& f=a->source_loading_v55();
  if(out.owner!=a||out.identity!=a->identity()||out.byte_ab!=&f.byte_ab||out.event_b0!=&f.event_b0){e="GoToMainMenu App cell loan was replaced";return false;}return true;};
 s.cleared_process_flags=[d](auto*& igm,auto*& mode,auto& pin,auto& e){if(!d->current(e))return false;
  auto n=d->native.lock();auto m=d->manager.lock();auto* view=m->view();
  if(!view||!view->globals){e="Required existing source multiplayer BSS projections";return false;}
  // Android ARM/ARM64 are little endian. These are the SAME canonical global
  // cells, whose source boolean values are produced by existing menu kernels.
  igm=reinterpret_cast<std::uint8_t*>(&view->globals->multiplayer_igm);
  mode=reinterpret_cast<std::uint8_t*>(&view->globals->multiplayer);pin=n;return true;};
 s.current_level=[d](auto& out,auto& e){return d->current(e)&&source_main_menu_level_v114(out,e);};
 s.map_object_virtual38_true=[d](auto& e){if(!d->current(e))return false;auto n=d->native.lock();auto m=d->manager.lock();
  if(!n->shared_roster_v27){e="Required actual App54/f4 menu virtual38 receiver";return false;}
  const auto r=m->pop_current(true,n->shared_roster_v27->stack_services());
  if(r){e=n->shared_roster_v27->error().empty()?"Actual map-menu virtual38(true) failed":n->shared_roster_v27->error();return false;}
  return d->current(e);};
 s.quick_save=[d](const auto& l,bool b,auto& e){return d->current(e)&&source_main_menu_quick_save_v114(l,b,e);};
 s.save_local_player=[d](const auto& l,bool b,auto& e){return d->current(e)&&source_main_menu_save_local_v114(l,b,e);};
 s.menu_manager=[d](auto& pin,auto& id,auto& e){if(!d->current(e))return false;auto m=d->manager.lock();pin=m;id=reinterpret_cast<std::uintptr_t>(m.get());return true;};
 s.reset_menu_manager=[d](auto id,auto& e){return d->current(e)&&reset_source_main_menu_manager_v114(id,e);};
 s.debug=[d](unsigned call,auto& e){if(!d->current(e))return false;
  // Genuine original DEBUG_OUT call, not Debug.Load/GetSwitch.
  __android_log_print(ANDROID_LOG_INFO,tag,"Application.GoToMainMenu source DEBUG_OUT @%08x",call);return true;};
 s.get_menu=[d](auto id,const char* name,auto& out,auto& e){return d->current(e)&&get_source_main_menu_v114(id,name,out,e);};
 s.menu_state=[d](auto& out,auto& e){return d->current(e)&&front_ui.borrow_main_menu_state_v114(out,e);};
 s.switch_state=[d](const auto& a,const auto& state,bool prev,auto& e){return d->switch_state(a,state,prev,e);};
 s.send_gl_high_score=[d](const auto&,auto& e){if(!d->current(e))return false;auto leaves=main_menu_process_leaves_v114(d);return dh2::application::source_send_score_v114(*d->app.lock(),leaves,e);};
 s.unlock_trophies_gl_live=[d](auto& e){if(!d->current(e))return false;auto leaves=main_menu_process_leaves_v114(d);return dh2::application::source_unlock_trophies_v114(*d->app.lock(),leaves,e);};
 s.remove_all_players=[d](auto& e){return d->current(e)&&source_main_menu_remove_players_v114(d->app.lock(),e);};
 s.online_byte5=[d](auto& out,auto& e){if(!d->current(e))return false;out=d->app.lock()->get_online_loading_v55()->byte5();return true;};
 // Positive online teardown requires its genuine platform/controller owners.
 // Offline is admitted only by the actual COnline C1/SetIsOnlineGame byte5.
 s.show_status_bar=[d](const auto&,bool,auto& e){return d->current(e);}; //31f668 is bx lr.
 auto command=std::make_unique<ApplicationGoToMainMenuV114>();
 if(!command->begin(event,std::move(s),e))return false;
 native_source_main_menu_command_v114=std::move(command);
 native_source_main_menu_v114=std::move(d);e.clear();return true;
}
bool model_renderer::resume_source_main_menu_v114(std::string& e){
 auto d=native_source_main_menu_v114;if(!d){e="No retained main-menu return command";return false;}
 if(!d->current(e))return false;
 if(!arm_source_main_menu_front_resume_v114(e))return false;
 if(d->retirement_begun&&!d->retirement_complete&&!retry_source_campaign_retirement_v114(e))return false;
 return native_source_main_menu_command_v114->resume(e);
}

bool model_renderer::source_main_menu_transition_owned_v114()noexcept{return bool(native_source_main_menu_v114);}

bool model_renderer::source_main_menu_refresh_hud_v114(const std::shared_ptr<void>& world,std::string& e){
 NativeMenuResourcesV93 first;first.owner=authored_character_menu;first.world=world;
 dh2::ui::MenuMovieBorrowV58 present;
 if(!first.movie_slot(3,present,e))return false;
 if(!present.identity){e.clear();return true;} // original genuine GetHUDRoot NULL
 // Original fresh MenuManager.GetInstance/GetHUDRoot after positive first call.
 NativeMenuResourcesV93 fresh;fresh.owner=authored_character_menu;fresh.world=world;
 dh2::ui::MenuMovieBorrowV58 actual;
 if(!fresh.movie_slot(3,actual,e))return false;
 if(!actual.identity){e="HUD disappeared between original GetHUDRoot calls";return false;}
 std::uint32_t slot{};dh2::ui::SwfMovie* movie{};std::shared_ptr<void> pin;
 if(!fresh.typed_movie(actual,slot,movie,pin,e)||slot!=3||!movie)return false;
 return movie->menu_action_script(nullptr,[](void*,dh2::ui::SwfAsGraph& graph,std::string& error){
  return graph.invoke_renderfx("menu_HUD_0","completeRefresh",{},error);
 },e);
}
