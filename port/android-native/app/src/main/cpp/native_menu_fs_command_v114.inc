//Included after native_menu_postmovie_v62.inc's actual provider definition.
#include "native_source_main_menu_v114.hpp"
#include "native_source_area_transition_v114.hpp"
#include <menu_fs_command_v114.hpp>
#include <menu_fx_pop_all_v114.hpp>
#include <script_manager_owner_v52.hpp>
#include <character_game_design.hpp>
namespace {
struct NativeMenuFsOwnerV114 {
 std::weak_ptr<NativeMenuPostMovieV62> provider;
 std::uintptr_t manager{};
 bool current(NativeCharacterMenuV4*& native,std::string& e)const{
  auto p=provider.lock();if(!p||!p->directory||!p->directory->resolve(native,e)||
     !native->shared_stack_v27||!native->shared_roster_v27||
     reinterpret_cast<std::uintptr_t>(native->shared_stack_v27.get())!=manager){
   if(e.empty())e="Retired actual native MenuManager FS receiver";return false;
  }
  return true;
 }
 bool remaining(std::uintptr_t id,const char* command,const char* args,bool& handled,std::string& e){
  handled=false;NativeCharacterMenuV4* native{};if(!current(native,e))return false;
  const auto callback=source_menu_process_v62->fs_commands().callback(command?command:"");
  if(!callback){e.clear();return true;} //Actual native-function-table miss.
  auto stack=native->shared_stack_v27;auto* roster=native->shared_roster_v27.get();
  auto* menu=stack->registered_menu_v27(id);if(!menu){e="Required captured source FS MenuBase";return false;}
  std::shared_ptr<void> receiver_pin,render_pin;
  if(!stack->capture_storage_v101(menu,receiver_pin,render_pin,e))return false;
  const std::string name=menu->name?menu->name:"";
  const auto services=roster->stack_services();
  const auto stack_result=[&](int status){if(status){e=roster->error().empty()?"Actual FS menu-stack operation failed":roster->error();return false;}handled=true;e.clear();return true;};
  //Exact original FS_PushMenu421334 and FS_PopMenu421410. Empty Pop args
  //uses the calling receiver's own name8, never the currently focused menu.
  if(callback==0x421334){if(!args){e="FS_PushMenu NULL args outside source pointer domain";return false;}return stack_result(stack->push(args,services));}
  if(callback==0x421410){if(!args){e="FS_PopMenu NULL args outside source pointer domain";return false;}return stack_result(stack->pop(*args?args:name.c_str(),services));}
  //Source f4.virtual38 belongs to this actual shared-stack projection.
  if(callback==0x4203e4)return stack_result(stack->pop_current(true,services));
  if(callback==0x4212f8){if(!args){e="FS_SwitchMenu NULL args outside source pointer domain";return false;}
   if(!stack_result(stack->pop_current(false,services)))return false;
   NativeCharacterMenuV4* after{};if(!current(after,e)||after->shared_stack_v27!=stack){e="FS_SwitchMenu manager changed after actual Pop";return false;}
   return stack_result(stack->push(args,services));
  }
  //Captured source leaf bodies: Twitter returns1; Pause/Resume return0.
  //They perform no platform launch or gameplay pause mutation in this build.
  if(callback==0x41f404){handled=true;e.clear();return true;}
  if(callback==0x41f444||callback==0x41f44c){e.clear();return true;}
  if(callback==0x420b48)return front_ui.finish_loading_fs_v114(command,args,handled,e);
  if(callback==0x420c3c){
   if(!model_renderer::request_source_main_menu_v114(0,e))return false;handled=true;return true;
  }
  if(callback==0x420c68){auto p=provider.lock();auto app=p?p->directory->application.lock():nullptr;
   auto scripts=app?app->source_script_manager_v52():nullptr;
   if(!scripts){e="Required SAME ScriptManager FS_SkipScript";return false;}
   if(!scripts->skip_script_v96(-2,false,e))return false;handled=true;return true;
  }
  //Recognized source names never become fake unhandled/success when their
  //positive native endpoint has not yet been reconstructed.
  e=std::string("Required registered original MenuBase FS endpoint: ")+(command?command:"<NULL>")+" @ "+std::to_string(callback);return false;
 }
};
bool bind_native_hud_fs_v114(const std::shared_ptr<NativeMenuPostMovieV62>& provider,std::string& e){
 NativeCharacterMenuV4* native{};
 if(!provider||!provider->directory||!provider->directory->resolve(native,e)||!native->hud||
    !native->shared_stack_v27||!native->shared_roster_v27){if(e.empty())e="Required actual HUD/manager/roster FS producer";return false;}
 auto owner=std::make_shared<NativeMenuFsOwnerV114>();owner->provider=provider;
 owner->manager=reinterpret_cast<std::uintptr_t>(native->shared_stack_v27.get());
 dh2::ui::MenuFSCommandServicesV114 services;services.owner=owner;
 services.current=[owner](auto& e){NativeCharacterMenuV4* native{};return owner->current(native,e);};
 services.source_trace=[owner](bool manager,const char*,const char*,auto& e){
  NativeCharacterMenuV4* native{};if(!owner->current(native,e))return false;
  std::int32_t ignored{};return native->debug(manager?"isTracingMenuManager":"isTracingMenuBase",ignored,e);
 };
 services.pop_all_render=[owner](auto& render,auto& e){
  NativeCharacterMenuV4* native{};if(!owner->current(native,e))return false;
  auto stack=native->shared_stack_v27;auto* roster=native->shared_roster_v27.get();
  if(stack->render(render.identity)!=&render){e="FS PopAll render is not the actual calling MenuFX";return false;}
  struct Captured {std::shared_ptr<void> render;std::shared_ptr<dh2::ui::MenuStackOwnerV1> stack;};
  auto captured=std::make_shared<Captured>();captured->stack=stack;
  if(!stack->capture_render_storage_v114(&render,captured->render,e))return false;
  dh2::ui::MenuFXPopAllServicesV114 actual;actual.owner=captured;
  actual.current=[owner,stack,id=render.identity](auto& e){NativeCharacterMenuV4* native{};
   if(!owner->current(native,e)||native->shared_stack_v27!=stack||!stack->render(id)){if(e.empty())e="Retired actual captured PopAll render";return false;}return true;
  };
  actual.hide_virtual10=[owner,stack,roster](auto& menu,auto& e){
   NativeCharacterMenuV4* native{};if(!owner->current(native,e)||native->shared_stack_v27!=stack||native->shared_roster_v27.get()!=roster)return false;
   std::shared_ptr<void> menu_pin,render_pin;if(!stack->capture_storage_v101(&menu,menu_pin,render_pin,e))return false;
   dh2::ui::MenuStackRequestV1 request{};request.operation=dh2::ui::MenuStackOperationV1::menu_hide;request.menu=&menu;request.render=menu.render;
   //SAME selected stack virtual dispatch: genuine roster MenuBase is handled
   //there; derived receivers use their existing actual platform continuation.
   //An absent derived Hide cannot be replaced with generic MenuBase.Hide.
   const auto source=roster->stack_services();
   if(!source.invoke||source.invoke(source.context,stack->view(),&request)){e=roster->error().empty()?"Required selected source receiver virtual10 Hide":roster->error();return false;}
   return true;
  };
  actual.resize_states=[stack](auto& render,auto capacity,auto& e){return stack->resize_render_states_v114(render,capacity,e);};
  return dh2::ui::menu_fx_pop_all_v114(render,actual,e);
 };
 services.application_load_level=[](const auto& arguments,auto& e){return model_renderer::request_confirmed_source_area_load_v114(arguments,e);};
 services.level_tables=[owner](auto& pin,const auto*& tables,auto& e){
  NativeCharacterMenuV4* native{};auto p=owner->provider.lock();if(!owner->current(native,e)||!p)return false;
  model_renderer::SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<model_renderer::SourceWorldBorrowV61> world;
  if(!model_renderer::borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=p->directory->current_world()||
     !model_renderer::borrow_source_campaign_condition_world_v70(candidate,world,e)||!world->design)return false;
  auto design=std::make_shared<dh2::character::CharacterGameDesign::Borrow>(world->design->borrow());
  tables=design->levels();if(!tables){e="Required SAME source LevelList snapshot for LoadLevel2";return false;}pin=design;return true;
 };
 services.remaining_my_fs=[owner](auto id,const char* command,const char* args,bool& handled,auto& e){return owner->remaining(id,command,args,handled,e);};
 if(!native->shared_roster_v27->bind_fs_command_services_v114(std::move(services),e))return false;
 const auto weak=std::weak_ptr<NativeMenuFsOwnerV114>(owner);
 //This ONLY producer is documented MenuManager.fc: shared_stack identity.
 //Other source handler kinds must install their own exact receiver callback.
 return native->hud->source_bind_fs_command_v114([weak](std::uintptr_t captured_fc,const char* command,const char* args,bool& handled,auto& e){
  handled=false;auto owner=weak.lock();NativeCharacterMenuV4* native{};
  if(!owner||captured_fc!=owner->manager||!owner->current(native,e)){if(e.empty())e="FS listenerfc differs from actual HUD MenuManager producer";return false;}
  return native->shared_roster_v27->manager_fs_command_v114(command,args,e);
 },e);
}
}
