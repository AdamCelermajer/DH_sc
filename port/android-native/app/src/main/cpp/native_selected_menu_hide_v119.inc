//Include in native_app after the process menu directory/singleton and main
//menu bindings. Qualified MenuBase.Hide is supplied by the startup owner.
#include "source_process_zoom_v119.hpp"
#include "gameplay_camera_application_v23.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "player_save_write_owner_v1.hpp"
#include "character_profile_bootstrap_v59.hpp"
#include "actual_device_android_v54.hpp"
#include "native_menu_preview_v121.hpp"
namespace {
struct NativeMenuPaneCameraV119 {
 //These are the genuine process BSS camera pointer slots. No source Create
 //has published either pane in this backend. A future positive producer must
 //publish its actual reference owner and remove/drop/restore operations here.
 std::uintptr_t identity{};
 std::shared_ptr<void> owner;
 std::function<bool(std::string&)> remove,drop,restore;
 bool destroy(std::string& e){
  if(!identity){e.clear();return true;}
  if(!owner||!remove||!drop||!restore){e="Required SAME positive inventory-pane camera source owner";return false;}
  if(!remove(e)||!drop(e))return false;
  identity=0;owner.reset();
  return restore(e);
 }
};
NativeMenuPaneCameraV119 merchant_pane_camera_v119,inventory_pane_camera_v119;
std::uintptr_t source_leader_v119{}; //MenuLeaderboard::leader original BSS.
bool selected_hide_confirm_v119(NativeCharacterMenuV4& native,NativeMenuSingletonV67& receiver,std::string& e){
 //GetMenuBelow42d19c reads the SAME MenuManager legacy64 vector. Modern
 //MultiMenu Push/Pop uses its separate owner; never infer this old vector
 //from visible movies or flatten current RenderFX states into new storage.
 auto& source=native.fields.source_menu_stack64_v119();
 std::uintptr_t below{};
 if(source.size()>1){
  bool found{};
  for(std::size_t i=0;i+1<source.size();++i){
   if(!source[i]||!source[i+1]||!source[i+1]->name){e="Invalid actual legacy MenuManager.GetMenuBelow entry";return false;}
   if(receiver.fields.name==source[i+1]->name){below=source[i]->identity;found=true;break;}
  }
  //Shipping source would read the one-past entry if no matching successor;
  //retain the exact invalid-input boundary rather than invent a menu below.
  if(!found){e="Original GetMenuBelow reads beyond actual legacy source vector";return false;}
 }
 return native.fields.register_listener(below,e)&&model_renderer::native_process_menu_base_hide_v119(receiver.fields.identity,e);
}
bool selected_hide_local_v119(std::uintptr_t& character,std::string& e){
 auto app=application_services_v5;auto pm=app?app->source_player_manager_v59():nullptr;
 dh2::player::PlayerInfoFieldsV1* local{};
 if(!pm||!pm->belongs_to_application(app)||!pm->get_local_player(0,true,local,e)||!local){
  if(e.empty())e="Required actual GetLocalPlayer0(true) receiver in selected Hide";return false;
 }
 //The source constructor dummy is a genuine receiver with660NULL. Do not
 //force an AddPlayer/local row/Character merely to execute startup Hide.
 character=local->character660;e.clear();return true;
}
bool selected_hide_current_camera_v119(dh2::loader::CanonicalCurrentLevelBorrowV1& current,
 std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11>& camera,std::string& e){
 camera.reset();if(!model_renderer::borrow_current_native_level_v27(current,e))return false;
 if(!current||!current.level()->constructor_fields_v3().field128){e.clear();return true;}
 model_renderer::SourceCampaignCandidateBorrowV55 candidate;
 if(!model_renderer::borrow_source_campaign_candidate_v55(candidate,e)||candidate.level!=current.level()||!candidate.camera_application){
  if(e.empty())e="Required SAME current Level128 in menu Hide";return false;
 }
 auto session=candidate.camera_application->world();
 camera=session&&session->camera?session->camera->level():nullptr;
 if(!camera||!camera->level()||reinterpret_cast<std::uintptr_t>(camera.get())!=current.level()->constructor_fields_v3().field128){
  e="Required actual current CameraLevel128 receiver";return false;
 }
 e.clear();return true;
}
bool selected_hide_restore_camera_v119(const std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11>& camera,
 std::uintptr_t character,bool source_requires_camera,std::string& e){
 if(character){
  if(!camera){if(source_requires_camera){e="Original Map.Hide requires nonNULL128 for positive Character660";return false;}}
  else if(!camera->set_target(character,0,e)||!camera->activate(e)||!camera->play_idle(e))return false;
 }
 std::shared_ptr<dh2::camera::GameplayCameraZoomV17> zoom;
 return model_renderer::borrow_source_process_zoom_v119(application_services_v5,zoom,e)&&zoom->set_camera(camera,e);
}
bool selected_hide_save_character_v119(std::uintptr_t id,std::string& e){
 model_renderer::SourceCampaignCandidateBorrowV55 candidate;
 model_renderer::SourceCampaignCharacterBorrowV62 actor;
 if(!model_renderer::borrow_source_campaign_candidate_v55(candidate,e)||
    !model_renderer::borrow_source_campaign_character_v62(candidate.actual_world,id,actor,e)||!actor.character){
  if(e.empty())e="Required SAME canonical local Character for Merchant.SG_Save";return false;
 }
 const auto& record=*actor.character;
 //SG_Save's whole writer, over its real retained Save/cache/section providers.
 const auto writer=record.profile_bootstrap?record.profile_bootstrap->campaign_writer():nullptr;
 if(!writer||!writer->ready()||!record.load){e="Required actual initialized SG_Save writer in Merchant.Hide";return false;}
 dh2::data::PlayerSaveWriteOwnerV1 source(record.load,writer->write_services());
 return source.save(e);
}
bool selected_hide_optional_gui_v119(std::string& e){
 std::int32_t driver{};
 if(!model_renderer::borrow_actual_device_driver_type_v55(driver,e))return false;
 //Explicit native backend adaptation: this GLES renderer uses GameSWF and
 //does not construct an Irrlicht IGUIEnvironment. Its optional GUI service is
 //NULL; no original SceneManager/GUI object or virtual48 callback is claimed.
 e.clear();return true;
}
}
bool model_renderer::native_selected_menu_hide_v119(std::uintptr_t id,std::string& e){
 auto native=authored_character_menu;auto app=application_services_v5;
 if(!id||!app||!native||native->process_application_v104.lock()!=app||!native->process_directory_ready_v104){
  e="Required SAME process App/MenuManager for selected virtual10 Hide";return false;
 }
 std::shared_ptr<NativeMenuSingletonV67> selected;
 for(const auto& owner:source_singleton_v67)if(owner&&owner->fields.identity==id){selected=owner;break;}
 if(selected){
  if(!selected->constructed){e=selected->failure.empty()?"Incomplete selected Menu C1":selected->failure;return false;}
  switch(selected->kind){
  case MenuSingletonKindV67::main:
  case MenuSingletonKindV67::select_class:
   //Both actual overrides test virtual3c IsValidMenu before ANY base/tail.
   if(!selected->fields.valid7c){e.clear();return true;}
   if(!native_process_menu_base_hide_v119(id,e))return false;
   if(selected->kind==MenuSingletonKindV67::main){
    //42c128 hides menu_bg before DestroyScene, preserving the actual source
    //positive missing receiver boundary instead of ignoring that lookup.
    auto* background=native->shared_stack_v27->menu("menu_bg");
    if(!background){e="Original Main.Hide dereferences missing menu_bg";return false;}
    auto front=native_main_menu_front_v114;
    if(!front||!front->hide(*background,e))return false;
    return native_menu_preview_main_hide_v121(app,e);
   }
   if(!native_menu_preview_select_hide_v121(app,selected->select160,0,e))return false;
   return front_ui.process_class_select_hide_v87(selected->fields.render,e);
  case MenuSingletonKindV67::merchant:{
   if(!merchant_pane_camera_v119.destroy(e)||!native_process_menu_base_hide_v119(id,e))return false;
   std::uintptr_t character{};if(!selected_hide_local_v119(character,e))return false;
   if(!character){e.clear();return true;}
   //4332b0 reloads the local record for its positive660 branch.
   if(!selected_hide_local_v119(character,e)||!character){if(e.empty())e="Merchant.Hide second Character660 became NULL";return false;}
   return selected_hide_save_character_v119(character,e);
  }
  case MenuSingletonKindV67::world_map:{
   if(!app->events14()){e="Required actual process EventManager for WorldMap.Detach";return false;}
   const auto receiver=reinterpret_cast<std::uintptr_t>(&selected->worldmap_event_c8_v119);
   bool removed{};
   if(!app->events14()->detach(4,receiver,removed,e)||!app->events14()->detach(5,receiver,removed,e))return false;
   dh2::loader::CanonicalCurrentLevelBorrowV1 current;
   std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11> camera;
   if(!selected_hide_current_camera_v119(current,camera,e))return false;
   std::uintptr_t character{};if(!selected_hide_local_v119(character,e))return false;
   //NULL Level still calls GetLocalPlayer, but ignores its returned660.
   if(!selected_hide_restore_camera_v119(camera,current?character:0,false,e))return false;
   return native_process_menu_base_hide_v119(id,e);
  }
  case MenuSingletonKindV67::inventory:
  case MenuSingletonKindV67::inventory_details:
   return inventory_pane_camera_v119.destroy(e)&&native_process_menu_base_hide_v119(id,e);
  case MenuSingletonKindV67::enter_name:
   if(!selected->fields.valid7c){e.clear();return true;}
   return native_process_menu_base_hide_v119(id,e);
  case MenuSingletonKindV67::debug:
  case MenuSingletonKindV67::lobby:
   return native_process_menu_base_hide_v119(id,e);
  case MenuSingletonKindV67::confirm:
  case MenuSingletonKindV67::hud_confirm:
   return selected_hide_confirm_v119(*native,*selected,e);
  case MenuSingletonKindV67::leaderboard:
   source_leader_v119=0;
   return native_process_menu_base_hide_v119(id,e);
  default:e=std::string("Required selected derived Hide override for ")+selected->fields.name;return false;
  }
 }
 if(source_map_menu_v62&&source_map_menu_v62->fields().identity==id){
  //4537c0 captures CurrentLevel/128 BEFORE its local-player query.
  dh2::loader::CanonicalCurrentLevelBorrowV1 current;
  std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11> camera;
  if(!selected_hide_current_camera_v119(current,camera,e))return false;
  std::uintptr_t character{};if(!selected_hide_local_v119(character,e)||
     !selected_hide_restore_camera_v119(camera,character,true,e))return false;
  //The retained Map C1's actual1e8 is NULL until CreateMapCamera publishes it.
  if(source_map_menu_v62->source_map_camera1e8_v119()){
   e="Required SAME positive Map camera1e8 deleting owner";return false;
  }
  if(!native_process_menu_base_hide_v119(id,e))return false;
  return selected_hide_optional_gui_v119(e);
 }
 if(source_menu_process_v62&&source_menu_process_v62->existing_debug_hud()&&
    source_menu_process_v62->existing_debug_hud()->fields().identity==id)
  return native_process_menu_base_hide_v119(id,e);
 e="Selected Hide receiver not present in actual process constructor directory";return false;
}
