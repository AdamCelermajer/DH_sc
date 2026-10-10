#include "source_campaign_script_execution_v96.hpp"
#include "model_renderer.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "gameplay_camera_application_v23.hpp"
#include "application_spawn_random_owner_v4.hpp"
#include "canonical_object_manager_v1.hpp"
#include "application_player_manager_bootstrap_v59.hpp"
#include "source_campaign_fx_v77.hpp"
#include "source_campaign_script_world_v117.hpp"
#include "source_campaign_script_ai_v118.hpp"
#include "source_campaign_script_tutorial_v118.hpp"
#include "source_campaign_script_tutorial_control_v118.hpp"
#include "source_campaign_script_environment_v120.hpp"
#include "source_campaign_script_audio_v117.hpp"
#include "source_campaign_death_rewards_v84.hpp"
#include <array>
#include <cstring>
#include <vector>
namespace dh2::android_ui {
namespace {
bool required(const char* leaf,std::string& e){e=std::string("Required actual campaign script ")+leaf;return false;}
using ParsedCommandIdentityV96=std::array<std::uintptr_t,7>;
std::vector<ParsedCommandIdentityV96> parsed_command_identities_v96(
 const loader::ScriptManagerOwnerV52& manager){
 std::vector<ParsedCommandIdentityV96> out;
 for(const auto& script:manager.commands())if(script.storage8)for(const auto& slot:*script.storage8){
  const auto& command=slot.command;
  if(slot.storage_released)continue;
  out.push_back({command.identity,
   reinterpret_cast<std::uintptr_t>(command.actual_owner.get()),
   reinterpret_cast<std::uintptr_t>(command.canonical_receiver_v96.get()),
   command.retained_data&&*command.retained_data?
    reinterpret_cast<std::uintptr_t>(command.retained_data->get()):0,
   reinterpret_cast<std::uintptr_t>(command.skip4),
   reinterpret_cast<std::uintptr_t>(command.kind8),
   reinterpret_cast<std::uintptr_t>(command.data_c)});
 }
 return out;
}
struct Binding {
 std::weak_ptr<void> world;
 std::weak_ptr<application::ApplicationServicesOwnerV5> application;
 SourceScriptUiLeavesV96 ui;
 SourceScriptActorLeavesV96 actors;
 bool current(model_renderer::SourceCampaignCandidateBorrowV55& out,std::string& e)const{
  auto w=world.lock();auto app=application.lock();
  if(!w||!app||!model_renderer::borrow_source_campaign_candidate_runtime_v61(out,e))return false;
  if(out.actual_world!=w||out.application!=app)return required("current SAME World/Application",e);
  return true;
 }
 bool debug_load(std::string& e)const{
  model_renderer::SourceCampaignCandidateBorrowV55 c;if(!current(c,e))return false;
  std::shared_ptr<model_renderer::SourceWorldBorrowV61> source;
  if(!model_renderer::borrow_source_campaign_condition_world_v70(c,source,e)||!source->debug||!source->debug_files)
   return required("same Application Debug/file owner",e);
  if(dh2_character_debug_load(source->debug.get(),source->debug_files)!=1)return required("Debug.load source delivery",e);
  e.clear();return true;
 }
 bool debug_switch(const char* key,bool& value,std::string& e)const{
  model_renderer::SourceCampaignCandidateBorrowV55 c;if(!current(c,e))return false;
  std::shared_ptr<model_renderer::SourceWorldBorrowV61> source;
  if(!model_renderer::borrow_source_campaign_condition_world_v70(c,source,e)||!source->debug||!source->debug_files)
   return required("same Application Debug/file owner",e);
  std::uint32_t word{};
  if(dh2_character_debug_get(&word,source->debug.get(),key,source->debug_files)!=1)
   return required("Debug.GetSwitch source delivery",e);
  value=word!=0;return true;
 }
 bool object(model_renderer::SourceCampaignCandidateBorrowV55& c,const char* name,std::int32_t room,
  const dh2::world::CanonicalObjectBorrowV1*& out,std::string& e,const char* filter=nullptr,
  dh2::target_providers::Handle16* retained=nullptr)const{
  if(!c.objects||!name)return required("actual ObjectManager/name",e);
  dh2::target_providers::Handle16 handle{};
  if(!c.objects->by_name(name,room,false,filter,handle,e))return false;
  if(!c.objects->resolve_handle_v4(handle,false,out,{},e))return false;
  if(retained)*retained=handle;return true;
 }
 bool camera(model_renderer::SourceCampaignCandidateBorrowV55& c,
  std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11>& out,std::string& e)const{
  loader::CanonicalCurrentLevelBorrowV1 level;
  if(!model_renderer::borrow_current_native_level_v27(level,e))return false;
  if(!level){out.reset();return true;} //Original current-Level NULL early branch.
  if(!c.camera_application)return required("actual CameraApplication",e);
  auto session=c.camera_application->world();
  if(!session||session->bindings.world_lease!=c.actual_world||!session->camera)
   return required("SAME current Level camera owner",e);
  out=session->camera->level();return true; //Genuine NULL128 is accepted.
 }
 bool character(const model_renderer::SourceCampaignCandidateBorrowV55& c,
  const dh2::world::CanonicalObjectBorrowV1& object,model_renderer::SourceCampaignCharacterBorrowV62& out,std::string& e)const{
  std::uintptr_t identity{};if(!object.as_character||!object.as_character(object.context,identity,e))return false;
  out={};if(!identity)return true;
  return model_renderer::borrow_source_campaign_character_v62(c.actual_world,identity,out,e);
 }
 bool point(const model_renderer::SourceCampaignCandidateBorrowV55& c,
  const dh2::world::CanonicalObjectBorrowV1& object,std::shared_ptr<void>& pin,const float*& out,std::string& e)const{
  model_renderer::SourceCampaignCharacterBorrowV62 actor;if(!character(c,object,actor,e))return false;
  if(actor.character){pin=actor.character;out=actor.character->actor->source_position160_v7();return out!=nullptr;}
  dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
  if(!model_renderer::borrow_source_campaign_object_base_v77(c,object.identity,pin,base,e))return false;
  if(!base||base->identity()!=object.identity||!pin)return required("SAME generic object base/Position160",e);
  out=base->vector3(0x160);return out?true:required("produced generic Position160",e);
 }
 bool position(const model_renderer::SourceCampaignCandidateBorrowV55& c,
  const dh2::world::CanonicalObjectBorrowV1& object,const float* xyz,std::string& e)const{
  model_renderer::SourceCampaignCharacterBorrowV62 actor;if(!character(c,object,actor,e))return false;
  if(actor.character){if(!actor.character->position||!actor.character->position->set_position(xyz,true,e))return false;}
  else if(!actors.owner||!actors.generic_set_position||!actors.generic_set_position(object.identity,xyz,true,e))
   return required("actual generic GameObject.SetPosition",e);
  return actors.owner&&actors.force_update_position?actors.force_update_position(object.identity,e):required("actual ForceUpdatePosition/root dirty208",e);
 }
 bool look(const model_renderer::SourceCampaignCandidateBorrowV55& c,std::uintptr_t character,std::uintptr_t target,std::string& e)const{
  model_renderer::SourceCampaignCharacterBorrowV62 actor;
  if(!model_renderer::borrow_source_campaign_character_v62(c.actual_world,character,actor,e))return false;
  auto& record=*actor.character;dh2::character::ControllerCommandState32 state;
  if(!record.services.controller||!record.services.controller(state,record,e)||!record.actor->controller)
   return required("actual forced controller LookAt owner",e);
  auto* actual=record.actor->controller->command_state(state.global_blocked);if(!actual)return required("source controller bytes",e);
  actual->forced=1;
  if(!actors.owner||!actors.controller_look_at||!actors.controller_look_at(character,target,e))return required("actual v2Controller.Cmd_LookAt",e);
  actual->forced=0;return true; //Failure retains original reached force-store prefix.
 }
};
bool word(const loader::CheckedCommandBorrowV59& c,unsigned o,std::uint32_t& out,std::string& e){
 auto* f=c.actual_data->scalar(o);if(!f||f->width!=4)return required("SAME command Data word",e);out=f->bits;return true;
}
std::int32_t signed_word(std::uint32_t value){std::int32_t out;std::memcpy(&out,&value,4);return out;}
}
bool bind_source_campaign_script_execution_v96(const std::shared_ptr<void>& world,
 SourceScriptUiLeavesV96 ui,std::string& e,SourceScriptActorLeavesV96 actors,
 SourceScriptBehaviorDecoratorV96 decorate){
 model_renderer::SourceCampaignCandidateBorrowV55 c;
 if(!world||!ui.owner||!model_renderer::borrow_source_campaign_candidate_runtime_v61(c,e)||c.actual_world!=world||!c.application)
  return required("actual current World and independent UI provider",e);
 if(ui.owner.get()==c.application.get()||(!ui.owner.owner_before(c.application)&&!c.application.owner_before(ui.owner)))
  return required("UI provider without ScriptManager/Application ownership cycle",e);
 auto manager=c.application->source_script_manager_v52();if(!manager)return required("existing Application ScriptManager",e);
 if(actors.owner&&((!actors.owner.owner_before(c.application)&&!c.application.owner_before(actors.owner))||
     (!actors.owner.owner_before(world)&&!world.owner_before(actors.owner))))
  return required("actor service provider without App/World ownership cycle",e);
 auto binding=std::make_shared<Binding>();binding->world=world;binding->application=c.application;binding->ui=std::move(ui);binding->actors=std::move(actors);
 loader::ScriptSchedulerServicesV96 scheduler;scheduler.owner=binding;
 scheduler.all_players_dead=[binding](bool& value,auto& error){auto w=binding->world.lock();return w&&model_renderer::source_campaign_all_players_dead_v70(w,value,error);};
 scheduler.player_count714=[binding](auto& value,auto& error){auto w=binding->world.lock();return w&&model_renderer::source_campaign_player_manager_integer714_v70(w,value,error);};
 scheduler.online=[binding](bool& value,auto& error){model_renderer::SourceCampaignCandidateBorrowV55 current;
  if(!binding->current(current,error))return false;value=current.application->get_online_loading_v55()->byte5()!=0;return true;};
 scheduler.debug_load=[binding](auto& error){return binding->debug_load(error);};
 scheduler.debug_switch=[binding](const char* key,bool& value,auto& error){return binding->debug_switch(key,value,error);};
 scheduler.constant=[binding](const char* group,const char* key,auto& value,auto& error){
  model_renderer::SourceCampaignCandidateBorrowV55 current;if(!binding->current(current,error))return false;
  std::shared_ptr<model_renderer::SourceWorldBorrowV61> source;
  if(!model_renderer::borrow_source_campaign_condition_world_v70(current,source,error)||!source->design)return required("actual Application GameDesign constants",error);
  auto borrowed=source->design->borrow();const auto* design=borrowed.design();
  return design&&design->lookup&&design->lookup(design->context,0,group,key,&value)==0;
 };
 scheduler.flush_dialogs=[binding](auto& error){return binding->ui.flush_dialogs?binding->ui.flush_dialogs(error):required("actual dialog queue flush",error);};
 scheduler.send_script_message=[binding](bool skip,auto id,auto module,auto& error){return binding->ui.send_script_message?binding->ui.send_script_message(skip,id,module,error):required("actual script network message",error);};
 scheduler.publish_current_name=[binding](const char* name,auto& error){return binding->ui.publish_current_name?binding->ui.publish_current_name(name,error):required("actual current-script debug name field",error);};
 loader::ScriptExecutionControlServicesV96 control;control.application=c.application;control.manager=manager;
 control.debug_load=scheduler.debug_load;control.debug_switch=scheduler.debug_switch;
 control.application_dt=[binding](auto& dt,auto& error){model_renderer::SourceCampaignCandidateBorrowV55 current;
  if(!binding->current(current,error))return false;
  const auto& fields=current.application->source_loading_v55();
  if(!fields.native_dt_produced_v93)return required("produced actual Application dt8c",error);
  dt=fields.dt8c;return true;};
 control.random=[binding](auto*& random,auto& lease,auto& error){model_renderer::SourceCampaignCandidateBorrowV55 current;
  if(!binding->current(current,error))return false;
  const auto source=current.application->source_random_v62();if(!source)return required("actual process Random",error);
  random=&source->channel(0);lease=source;return true;};
 loader::ScriptCommandBehaviorV59 effects;effects.actual_owner=c.application;
 effects.execute=[binding,get_constant=scheduler.constant](const auto& command,bool skip,std::int32_t module,auto& error){
  const auto kind=*command.kind8;model_renderer::SourceCampaignCandidateBorrowV55 current;
  if(!binding->current(current,error))return false;
  if(kind==4){error.clear();return true;} //Original Script_SetCamera455678 is literal BX LR.
  if(kind==1||kind==2){
   auto app=current.application;auto pm=app->source_player_manager_v59();
   if(!pm||!pm->manager())return required("actual cutscene PlayerManager",error);
   if(kind==1){
    loader::CanonicalCurrentLevelBorrowV1 level;
    if(!model_renderer::borrow_current_native_level_v27(level,error))return false;
    if(level){
     //Source repeats Application.GetCurrentLevel before byte198 read.
     if(!model_renderer::borrow_current_native_level_v27(level,error)||!level)
      return required("nonnull second original current-Level lookup",error);
     bool show_ui=level.level()->constructor_fields_v3().byte198==0;
     if(!show_ui){bool forced{};
      if(!binding->ui.force_cutscene_ui||!binding->ui.force_cutscene_ui(forced,error))return required("actual MenuBase.s_igmOpened process byte",error);
      show_ui=forced;
     }
     if(show_ui){
      if(!binding->ui.menu_f4_virtual38||!binding->ui.menu_f4_virtual38(true,error))return required("actual MenuManager.f4 virtual38",error);
      player::PlayerInfoFieldsV1* local{};
      if(!pm->get_local_player(0,true,local,error)||!local)return required("actual cutscene local PlayerInfo",error);
      if(local->character660){
       if(!pm->get_local_player(0,true,local,error)||!local||!local->character660)return required("nonnull repeated cutscene local Character",error);
       if(!binding->actors.owner||!binding->actors.reload_skills||!binding->actors.reload_skills(local->character660,error))return required("actual Character.ReloadSkills",error);
      }
      //Application.ShowStatubBar31f668 is literal BX LR in original Android.
     }
    }
    const bool online=app->get_online_loading_v55()->byte5()!=0;
    if(online){player::PlayerInfoFieldsV1* local{};
     if(!pm->get_local_player(0,true,local,error)||!local)return required("actual online cutscene local player",error);
     if(local->character660){
      if(!pm->get_local_player(0,true,local,error)||!local||!local->character660)return required("repeated online cutscene Character",error);
      model_renderer::SourceCampaignCharacterBorrowV62 actor;
      if(!model_renderer::borrow_source_campaign_character_v62(current.actual_world,local->character660,actor,error))return false;
      if(!actor.character->life)return required("actual Character.IsDead storage",error);
      if(actor.character->life->dead){
       if(!pm->get_local_player(0,false,local,error)||!local)return required("actual dead local-player reset receiver",error);
       if(!binding->ui.reset_dead_local_player||!binding->ui.reset_dead_local_player(*local,error))return required("actual online dead-player NetStruct reset",error);
      }
     }
    }
   }
   //Both original bodies invoke actual HUD callback BEFORE source stores.
   if(!binding->ui.hud_callback_noargs||!binding->ui.hud_callback_noargs(kind==1?"StartCutscene":"StopCutscene",error))
    return required("actual zero-argument HUD cutscene AS callback",error);
   auto scripts=app->source_script_manager_v52();if(!scripts)return required("same cutscene ScriptManager byte30",error);
   scripts->fields().byte30=kind==1?1:0;
   if(!binding->ui.store_display_hud||!binding->ui.store_display_hud(kind==1?0:1,error))return required("actual AnimController.s_scalingEnabled process byte",error);
   if(app->get_online_loading_v55()->byte5()==0){error.clear();return true;}
   if(kind==2&&(!binding->ui.send_script_message||!binding->ui.send_script_message(true,-2,-1,error)))
    return required("actual online StopCutscene message",error);
   std::int32_t index=0;
   for(;;){
    const auto* count=pm->count_field();if(!count)return required("actual cutscene PM count6c4",error);
    if(index>=*count)break;
    player::PlayerInfoFieldsV1* info{};if(!pm->manager()->get_player(index,false,info,error)||!info)return required("actual cutscene player iteration",error);
    bool local{};if(!pm->network()||!pm->network()->is_local(*info,local,error))return required("actual PlayerInfo virtual50 IsLocal",error);
    if(local){
     if(!binding->ui.player_set_in_cutscene||!binding->ui.player_set_in_cutscene(*info,kind==1,error))return required("actual PlayerInfo.SetInCutscene NetStruct",error);
    }else if(info->character660){
     if(!binding->actors.owner||!binding->actors.render_visible||!binding->actors.render_visible(info->character660,kind==2,error))return required("actual Character RenderVisible virtual40",error);
    }
    ++index; //Original count/PM are reread after each actual effect.
   }
   error.clear();return true;
  }
  if(kind==79){
   if(!binding->ui.flush_dialogs||!binding->ui.flush_achievements||!binding->ui.flush_status||!binding->ui.flush_online)
    return required("actual four source message queues",error);
   return binding->ui.flush_dialogs(error)&&binding->ui.flush_achievements(error)&&
    binding->ui.flush_dialogs(error)&&binding->ui.flush_status(error)&&binding->ui.flush_online(error);
  }
  bool environment_handled{};if(!model_renderer::execute_source_campaign_script_environment_v120(current,command,skip,module,environment_handled,error))return false;if(environment_handled)return true;
  bool world_handled{};if(!model_renderer::execute_source_campaign_script_world_v117(current,command,skip,module,world_handled,error))return false;if(world_handled)return true;
  bool audio_handled{};if(!model_renderer::execute_source_campaign_script_audio_v117(current,command,skip,module,audio_handled,error))return false;if(audio_handled)return true;
  bool ai_handled{};if(!model_renderer::execute_source_campaign_script_ai_v118(current,command,skip,module,ai_handled,error))return false;if(ai_handled)return true;
  bool tutorial_handled{};if(!model_renderer::execute_source_campaign_script_tutorial_v118(current,command,skip,module,tutorial_handled,error))return false;if(tutorial_handled)return true;
  bool tutorial_control_handled{};if(!model_renderer::execute_source_campaign_script_tutorial_control_v118(current,command,skip,module,tutorial_control_handled,error))return false;if(tutorial_control_handled)return true;
  if(kind!=24&&kind!=25&&kind!=8&&kind!=5&&kind!=6&&kind!=7&&kind!=12&&kind!=10&&kind!=46&&kind!=41&&kind!=40&&kind!=45&&kind!=42&&kind!=43&&kind!=31&&kind!=32&&kind!=39&&kind!=44)
   return required("remaining reached native command effects",error);
  if((kind==24||kind==10||kind==45)&&skip){error.clear();return true;} //Original skip early exits.
  if(!binding->debug_load(error))return false;bool ignored{};
  if(!binding->debug_switch("isTracingScriptCmd",ignored,error))return false;
  if(kind==42||kind==43){
   //ShowActor has two consecutive genuine Debug prefixes; HideActor has one.
   if(kind==42&&(!binding->debug_load(error)||!binding->debug_switch("isTracingScriptCmd",ignored,error)))return false;
   const auto* name=command.actual_data->cstring(12);const dh2::world::CanonicalObjectBorrowV1* object{};
   if(!binding->object(current,name,module,object,error))return false;if(!object)return true;
   if(!binding->actors.owner)return required("actual selected visibility receiver",error);
   if(kind==42&&(!binding->actors.disable_zoning||!binding->actors.disable_zoning(object->identity,error)))return required("actual GameObject.DisableZoning",error);
   return binding->actors.render_visible?binding->actors.render_visible(object->identity,kind==42,error):required("actual GameObject.SetVisible virtual40",error);
  }
  if(kind==6){
    //45c670 reads near Data+12 before far Data+8. Positive bounds go straight
    //to the SAME current camera; -2 selects the actual LevelConfig values.
    //Other nonpositive values leave that bound unchanged. The command ignores
    //skip, and its IsBlocking body is the literal false at 455684.
    std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11> camera;
    if(!binding->camera(current,camera,error))return false;
    if(!camera){error.clear();return true;} //Original NULL current-Level camera guard.
    if(!camera->level()){error.clear();return true;} //Original NULL CameraLevel guard.
    std::uint32_t near_bits{};if(!word(command,12,near_bits,error))return false;
    const auto near_plane=signed_word(near_bits);
    if(near_plane>0){if(!camera->set_clip_start(static_cast<float>(near_plane),error))return false;}
    else if(near_plane==-2){
     std::int32_t default_near{},default_far{};
     if(!camera->source_clip_defaults_v120(default_near,default_far,error)||
        !camera->set_clip_start(static_cast<float>(default_near),error))return false;
    }
    std::uint32_t far_bits{};if(!word(command,8,far_bits,error))return false;
    const auto far_plane=signed_word(far_bits);
    if(far_plane>0){if(!camera->set_clip_end(static_cast<float>(far_plane),error))return false;}
    else if(far_plane==-2){
     std::int32_t default_near{},default_far{};
     if(!camera->source_clip_defaults_v120(default_near,default_far,error)||
        !camera->set_clip_end(static_cast<float>(default_far),error))return false;
    }
    error.clear();return true;
  }
  if(kind==45){
   const auto* name=command.actual_data->cstring(24);const dh2::world::CanonicalObjectBorrowV1* object{};dh2::target_providers::Handle16 handle{};
   if(!binding->object(current,name,module,object,error,nullptr,&handle))return false;
   //Native-width cached receiver remains a typed pointer; source key/frame
   //words stay words. Reconstruct this SAME handle for IsBlocking.
   if(!command.actual_receiver->write_operand_word(16,4,std::uint32_t(handle.key),error)||
      !command.actual_receiver->write_operand_pointer(20,handle.cached,object?object->lease:std::shared_ptr<void>{},error)||
      !command.actual_receiver->write_operand_word(24,4,handle.frame,error))return false;
   if(!object)return true;model_renderer::SourceCampaignCharacterBorrowV62 actor;if(!binding->character(current,*object,actor,error))return false;if(!actor.character)return true;
   std::uint32_t first{},second{},animation{};if(!word(command,8,first,error)||!word(command,12,second,error)||!word(command,16,animation,error))return false;
   return binding->actors.owner&&binding->actors.play_animation?binding->actors.play_animation(object->identity,signed_word(first),signed_word(second),signed_word(animation),error):required("actual shared authored animation/FSM receiver",error);
  }
  if(kind==40){
   const dh2::world::CanonicalObjectBorrowV1* source{};const dh2::world::CanonicalObjectBorrowV1* target{};
   if(!binding->object(current,command.actual_data->cstring(24),module,source,error))return false;
   model_renderer::SourceCampaignCharacterBorrowV62 actor;if(source&&!binding->character(current,*source,actor,error))return false;
   if(!binding->object(current,command.actual_data->cstring(12),module,target,error))return false;
   const auto* wait=command.actual_data->scalar(28);const auto* collision=command.actual_data->scalar(16);if(!wait||wait->width!=1||!collision||collision->width!=1)return required("actual MoveActor wait/collision bytes",error);
   std::uint8_t original_static{};if(actor.character){const auto* cell=actor.character->actor->source_bool_field(0x84);if(!cell)return required("actual MoveActor captured byte84",error);original_static=*cell;}
   if(!command.actual_receiver->write_operand_word(16,1,wait->bits,error)||!command.actual_receiver->write_operand_pointer(20,actor.character?source->identity:0,actor.character?source->lease:std::shared_ptr<void>{},error)||!command.actual_receiver->write_operand_word(24,1,original_static,error))return false;
   if(!actor.character||!target)return command.actual_receiver->write_operand_word(16,1,0,error);
   bool retained_wait=wait->bits!=0;
   if(!binding->actors.owner||!binding->actors.move_actor||!binding->actors.move_actor(source->identity,target->identity,skip,collision->bits!=0,wait->bits!=0,original_static,retained_wait,error))return required("actual MoveActor source controller/physical/path body",error);
   return command.actual_receiver->write_operand_word(16,1,retained_wait,error);
  }
  if(kind==31||kind==32||kind==39||kind==44){
   const char* name=command.actual_data->cstring(kind==32?16:12);if(!name)return required("actual Character-state command name",error);
   auto apply=[&](const dh2::world::CanonicalObjectBorrowV1* object){if(!object)return true;model_renderer::SourceCampaignCharacterBorrowV62 actor;if(!binding->character(current,*object,actor,error))return false;if(!actor.character)return true;
    if(kind==31)return binding->actors.put_idle?binding->actors.put_idle(object->identity,!skip,error):required("actual PutCharacterInIdle source callback",error);
    if(kind==32){const auto* value=command.actual_data->scalar(8);if(!value||value->width!=1)return required("actual scripted byte",error);return binding->actors.mark_scripted?binding->actors.mark_scripted(object->identity,value->bits!=0,error):required("actual source Character1480 setter",error);}
    if(kind==39)return binding->actors.stop_actor?binding->actors.stop_actor(object->identity,error):required("actual forced Cmd_Stop",error);
    auto& r=*actor.character;dh2::character::ControllerCommandState32 snapshot;if(!r.services.controller||!r.services.controller(snapshot,r,error)||!r.actor->controller)return required("actual KillActor controller",error);auto* control=r.actor->controller->command_state(snapshot.global_blocked);if(!control)return false;control->forced=1;
    if(!model_renderer::source_campaign_cmd_kill_v84(current.actual_world,r.actor->controller->identity(),object->identity,0,0,nullptr,error))return false;control->forced=0;return true;
   };
   const bool all=std::strlen(name)==3&&(name[0]=='A'||name[0]=='a')&&(name[1]=='L'||name[1]=='l')&&(name[2]=='L'||name[2]=='l');
   if(all&&kind!=44){
    if(kind==39){std::int32_t key{};const dh2::world::CanonicalObjectBorrowV1* object{};bool more=current.objects->source_ordered_begin_v38(key,object);while(more){if(!apply(object))return false;more=current.objects->source_ordered_next_v38(key,key,object);}return true;}
    for(auto id:current.objects->source_active2c_v102()){std::int32_t key{};const dh2::world::CanonicalObjectBorrowV1* object{};bool more=current.objects->source_ordered_begin_v38(key,object);while(more&&(!object||object->identity!=id))more=current.objects->source_ordered_next_v38(key,key,object);if(!apply(more?object:nullptr))return false;}return true;
   }
   const dh2::world::CanonicalObjectBorrowV1* object{};return binding->object(current,name,module,object,error)&&apply(object);
  }
  if(kind==46){
   const char* recipient_name=command.actual_data->cstring(20);
   const char* source_name=command.actual_data->cstring(12);
   const dh2::world::CanonicalObjectBorrowV1* recipient{};const dh2::world::CanonicalObjectBorrowV1* source{};
   //Original resolves recipient first, then source; both actual handles mayNULL.
   if(!binding->object(current,recipient_name,module,recipient,error)||!binding->object(current,source_name,module,source,error))return false;
   if(!recipient||!source){error.clear();return true;}
   model_renderer::SourceCampaignCharacterBorrowV62 actor;if(!binding->character(current,*recipient,actor,error))return false;
   if(actor.character){
    if(!binding->actors.owner||!binding->actors.set_idle||!binding->actors.set_idle(recipient->identity,false,error))
     return required("actual SM_SetIdleState(false)",error);
    bool player{};if(!actor.character->is_player(player,error))return false;
    if(player){
     auto manager=current.application->source_player_manager_v59();
     for(std::int32_t index=1;index<4;++index){player::PlayerInfoFieldsV1* peer{};
      if(!manager||!manager->manager()||!manager->manager()->get_player(index,false,peer,error)||!peer)
       return required("actual SetActorPosition PM peer lookup",error);
      if(!peer->character660)continue;
      model_renderer::SourceCampaignCharacterBorrowV62 peer_actor;
      if(!model_renderer::borrow_source_campaign_character_v62(current.actual_world,peer->character660,peer_actor,error))return false;
      std::shared_ptr<void> pin;const float* location{};if(!binding->point(current,*source,pin,location,error))return false;
      //Original fadds preserve signed zero on unaffected axes.
      const float x=location[0]+(index==1?-200.f:index==2?200.f:0.f);
      const float y=location[1]+(index==3?-200.f:0.f),z=location[2]+0.f;
      const float placement[]{x,y,z};auto peer_object=peer_actor.character->actor->canonical(peer_actor.character);
      if(!binding->position(current,peer_object,placement,error))return false;
     }
    }
   }
   std::shared_ptr<void> pin;const float* location{};
   return binding->point(current,*source,pin,location,error)&&binding->position(current,*recipient,location,error);
  }
  if(kind==41){
   const char* recipient_name=command.actual_data->cstring(20);
   const char* target_name=command.actual_data->cstring(12);
   if(!recipient_name||!target_name)return required("LookActor source names",error);
   const dh2::world::CanonicalObjectBorrowV1* recipient{};
   if(!binding->object(current,recipient_name,module,recipient,error))return false;
   model_renderer::SourceCampaignCharacterBorrowV62 actor;
   if(recipient&&!binding->character(current,*recipient,actor,error))return false;
   const dh2::world::CanonicalObjectBorrowV1* target{};
   const char* filter=std::strcmp(target_name,"HighestThreatPlayer")==0?recipient_name:nullptr;
   dh2::target_providers::Handle16 target_handle{};
   if(!binding->object(current,target_name,module,target,error,filter,&target_handle))return false;
   if(actor.character&&target){
    bool player{};if(!actor.character->is_player(player,error))return false;
    if(player){loader::CanonicalCurrentLevelBorrowV1 level;
     if(!model_renderer::borrow_current_native_level_v27(level,error))return false;
     if(level){auto pm=current.application->source_player_manager_v59();
      for(std::int32_t index=1;index<4;++index){player::PlayerInfoFieldsV1* peer{};
       if(!pm||!pm->manager()||!pm->manager()->get_player(index,false,peer,error)||!peer)
        return required("actual LookActor PM peer lookup",error);
       if(peer->character660&&!binding->look(current,peer->character660,target->identity,error))return false;
      }
     }
    }
    if(!binding->look(current,recipient->identity,target->identity,error))return false;
   }
   //Original final GetObject(false) reread keeps NULL-result source behavior.
   const dh2::world::CanonicalObjectBorrowV1* final_target{};
   return current.objects->resolve_handle_v4(target_handle,false,final_target,{},error);
  }
  if(kind==10){
   std::uint32_t style{};if(!word(command,12,style,error))return false;
   std::int32_t enter_location{};
   if(!get_constant("DialogStyles","EnterLocationDialog",enter_location,error))return false;
   if(signed_word(style)==enter_location){
    bool active{};
    if(!binding->ui.dialog_active||!binding->ui.dialog_active(active,error))return required("actual EnterLocation dialog queue query",error);
    if(active)return binding->ui.flush_dialogs?binding->ui.flush_dialogs(error):required("actual EnterLocation dialog queue flush",error);
   }
   std::uint32_t text{},speaker{};
   if(!word(command,16,text,error)||!word(command,8,speaker,error))return false;
   if(!binding->ui.enqueue_dialog)return required("actual DialogMsg constructor/queue/AS start",error);
   return binding->ui.enqueue_dialog(0,signed_word(text),signed_word(style),signed_word(speaker),0,true,error);
  }
  if(kind==24||kind==25){
   const char* name=command.actual_data->cstring(12);if(!name)return required("original Lock/Unlock name",error);
   if(!std::strcmp(name,"All"))return binding->ui.store_controller_global?
    binding->ui.store_controller_global(kind==24?1:0,error):required("actual global controller-block byte",error);
   const dh2::world::CanonicalObjectBorrowV1* object{};if(!binding->object(current,name,module,object,error))return false;
   if(!object){error.clear();return true;}std::uintptr_t character{};
   if(!object->as_character||!object->as_character(object->context,character,error))return false;
   if(!character){error.clear();return true;}
   model_renderer::SourceCampaignCharacterBorrowV62 actor;
   if(!model_renderer::borrow_source_campaign_character_v62(current.actual_world,character,actor,error))return false;
   auto& record=*actor.character;dh2::character::ControllerCommandState32 state;
   if(!record.services.controller||!record.services.controller(state,record,error)||!record.actor->controller)
    return required("SAME Character controller378/locked8",error);
   auto* actual=record.actor->controller->command_state(state.global_blocked);
   if(!actual)return required("produced actual controller global-block state",error);
   actual->locked=kind==24?1:0;return true;
  }
  if(kind==8){
   std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11> camera;if(!binding->camera(current,camera,error))return false;
   if(!camera){error.clear();return true;}
   const char* name=command.actual_data->cstring(16);if(!name)return required("SetCameraTarget source name",error);
   std::uint32_t duration{};if(!word(command,8,duration,error))return false;
   std::uintptr_t identity{};
   if(*name){const dh2::world::CanonicalObjectBorrowV1* object{};
    if(!binding->object(current,name,-1,object,error))return false;
    if(!object){error.clear();return true;}identity=object->identity;
   }else{auto pm=current.application->source_player_manager_v59();player::PlayerInfoFieldsV1* p{};
    if(!pm||!pm->get_local_player(0,true,p,error)||!p)return required("actual SetCameraTarget local player0",error);
    identity=p->character660;
   }
   return camera->set_target(identity,skip?0:signed_word(duration),error);
  }
  if(kind==5){std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11> camera;if(!binding->camera(current,camera,error))return false;
   if(!camera){error.clear();return true;}if(skip)return camera->play_idle(error);
   std::uint32_t animation{};if(!word(command,8,animation,error))return false;
   if(!camera->level())return required("actual CameraLevel controller",error);
   return camera->level()->play_animation(signed_word(animation),0,false,error);
  }
  if(kind==12||kind==7)return true; //WaitDialog45c4d8/WaitCamera45c5e8 contain just the actual Debug body above.
  return required("remaining reached native command effects",error);
 };
 effects.blocking=[binding](const auto& command,bool& out,auto& error){
  if(*command.kind8==6){out=false;error.clear();return true;} //455684 literal false.
  if(*command.kind8==12)return binding->ui.dialog_active?binding->ui.dialog_active(out,error):required("SAME dialog queue active query",error);
  model_renderer::SourceCampaignCandidateBorrowV55 current;if(!binding->current(current,error))return false;
  bool environment_handled{};
  if(!model_renderer::blocking_source_campaign_script_environment_v120(current,command,environment_handled,out,error))return false;
  if(environment_handled)return true;
  const auto kind=*command.kind8;
  if(kind==45){const auto* wait=command.actual_data->scalar(28);if(!wait||wait->width!=1)return required("actual animation wait byte",error);if(!wait->bits){out=false;return true;}std::uint32_t key{},frame{};std::uintptr_t cached{};if(!command.actual_receiver->read_operand_word(16,4,key,error)||!command.actual_receiver->read_operand_pointer(20,cached,error)||!command.actual_receiver->read_operand_word(24,4,frame,error))return false;dh2::target_providers::Handle16 handle{signed_word(key),frame,cached};const dh2::world::CanonicalObjectBorrowV1* object{};auto persist=[&]{std::uintptr_t previous{};if(!command.actual_receiver->read_operand_pointer(20,previous,error))return false;if(handle.cached!=previous&&!command.actual_receiver->write_operand_pointer(20,handle.cached,object?object->lease:std::shared_ptr<void>{},error))return false;return command.actual_receiver->write_operand_word(24,4,handle.frame,error);};if(!current.objects->resolve_handle_v4(handle,false,object,{},error)||!persist())return false;if(!object){out=false;return true;}if(!current.objects->resolve_handle_v4(handle,false,object,{},error)||!persist())return false;if(!object){out=false;return true;}std::uint32_t animation{};if(!word(command,16,animation,error))return false;return binding->actors.animation_blocking?binding->actors.animation_blocking(object->identity,signed_word(animation),out,error):required("actual animation wait receiver",error);}
  if(kind==40){std::uint32_t wait{},original_static{};std::uintptr_t actor{};if(!command.actual_receiver->read_operand_word(16,1,wait,error))return false;if(!wait){out=false;return true;}if(!command.actual_receiver->read_operand_pointer(20,actor,error))return false;if(!actor){out=false;return true;}if(!command.actual_receiver->read_operand_word(24,1,original_static,error))return false;return binding->actors.move_blocking?binding->actors.move_blocking(actor,static_cast<std::uint8_t>(original_static),out,error):required("actual MoveActor path/Stop/Revive wait",error);}
  if(kind==8||kind==5){const auto* wait=command.actual_data->scalar(kind==8?20:12);
   if(!wait||wait->width!=1)return required("actual camera command wait byte",error);
   if(!wait->bits){out=false;return true;}
  }
  if(kind==5||kind==7||kind==8){std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11> camera;
   if(!binding->camera(current,camera,error))return false;
   if(!camera){out=false;return true;}if(!camera->level())return required("actual CameraLevel source fields",error);
   out=kind==8?camera->level()->target().fields().transition_remaining>0:camera->level()->fields().shake84;return true;
  }
  return required("remaining reached dynamic command blocking leaf",error);
 };
 auto behavior=loader::script_execution_control_v96(std::move(effects),std::move(control));
 if(decorate){
  const auto manager_identity=manager->identity();
  const auto scheduler_owner=scheduler.owner;
  const auto parsed_before=parsed_command_identities_v96(*manager);
  if(!decorate(manager,scheduler,behavior,e))return false;
  auto app_manager=c.application->source_script_manager_v52();
  if(!app_manager||app_manager!=manager||manager->identity()!=manager_identity)
   return required("same Application ScriptManager after command behavior decoration",e);
  if(!scheduler.owner||scheduler.owner!=scheduler_owner||scheduler.owner!=binding)
   return required("same source scheduler owner after command behavior decoration",e);
  if(behavior.actual_owner.lock()!=c.application||!behavior.execute||!behavior.blocking)
   return required("same Application command behavior and native Execute/IsBlocking bodies after decoration",e);
  if(parsed_command_identities_v96(*manager)!=parsed_before)
   return required("same parsed receiver/data identity order after command behavior decoration",e);
 }
 if(!loader::bind_parsed_script_execution_v96(*manager,std::move(behavior),e))return false;
 return manager->bind_scheduler_v96(std::move(scheduler),e);
}
}
