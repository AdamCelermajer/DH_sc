#include "source_campaign_script_actor_v96.hpp"
#include "source_campaign_script_execution_v96.hpp"
#include "source_campaign_fx_v77.hpp"
#include <campaign_navigation_registry_v64.hpp>
#include "source_campaign_runtime_v61.hpp"
#include "model_renderer.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "canonical_gameobject_graph_v68.hpp"
#include "character_path_commands.hpp"
#include "character_controller_commands.hpp"
#include "character_buffs.hpp"
#include "source_campaign_character_fsm_v101.hpp"
#include "source_campaign_character_zoning_v108.hpp"
#include "source_campaign_frame_environment_v76.hpp"
#include "character_candidate_cache_v62.hpp"
#include "canonical_character_pf_v62.hpp"
#include "source_campaign_noncharacter_virtual_v105.hpp"
#include <physical_world.hpp>
#include <algorithm>
namespace model_renderer {namespace {
bool required(const char* leaf,std::string& e){if(e.empty())e=std::string("Required actual campaign script actor ")+leaf;return false;}
struct ActorLeavesV96 {
 std::weak_ptr<SourceWorldBorrowV61> world;
 bool current(SourceCampaignCandidateBorrowV55& candidate,std::shared_ptr<SourceWorldBorrowV61>& actual,std::string& e)const{
  actual=world.lock();if(!actual||!actual->owner||!actual->application||!borrow_source_campaign_candidate_runtime_v61(candidate,e))return false;
  return candidate.actual_world==actual->owner&&candidate.application==actual->application?true:required("SAME current World/Application",e);
 }
 bool character(std::uintptr_t id,SourceCampaignCharacterBorrowV62& actor,std::string& e)const{
  SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> actual;
  return current(candidate,actual,e)&&borrow_source_campaign_character_v62(actual->owner,id,actor,e)&&actor.character&&actor.character->actor;
 }
 bool visual(const std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>& record,
  std::shared_ptr<dh2::world::RetainedGameObjectVisualV1>& out,std::string& e)const{
  if(!record||!record->actor)return required("SAME Character visual field",e);
  const auto id=record->actor->source_visual();out.reset();if(!id){e.clear();return true;}
  out=record->visual?record->visual->visual():nullptr;
  if(!out||reinterpret_cast<std::uintptr_t>(out.get())!=id)return required("SAME retained source VisualObject2d8",e);
  e.clear();return true;
 }
 bool target_point(std::uintptr_t id,std::shared_ptr<void>& pin,const float*& point,std::string& e)const{
  SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> actual;
  if(!current(candidate,actual,e)||!candidate.objects)return false;
  const dh2::world::CanonicalObjectBorrowV1* object{};std::int32_t key{};
  bool next=candidate.objects->source_ordered_begin_v38(key,object);
  while(next&&(!object||object->identity!=id))next=candidate.objects->source_ordered_next_v38(key,key,object);
  if(!next||!object||!object->as_character)return required("actual canonical LookAt target",e);
  std::uintptr_t character{};if(!object->as_character(object->context,character,e))return false;
  if(character){SourceCampaignCharacterBorrowV62 actor;if(!borrow_source_campaign_character_v62(actual->owner,character,actor,e))return false;
   auto& r=*actor.character;dh2::world::GameObjectInitializationFieldsV62 fields;
   if(!r.actor->inherited_initialization_fields_v62(r.actor,fields,e))return false;
   const auto* node=fields.pointer(0x180);if(!node)return required("source target180",e);
   point=r.actor->source_position160_v7();
   if(*node){const auto* enabled=r.actor->source_bool_field(0x80);if(!enabled)return required("source target enabled80",e);if(*enabled)point=r.actor->runtime.target_position;}
   pin=r.actor;e.clear();return point!=nullptr;
  }
  dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
  if(!actual->gameobject_graph_v68||!actual->gameobject_graph_v68->borrow_base_v77(id,pin,base,e))return false;
  const auto* node=base->pointer(0x180);if(!node)return required("actual generic target180",e);
  point=base->vector3(0x160);
  if(*node){const auto* enabled=base->byte(0x80);if(!enabled)return required("actual generic target enabled80",e);if(*enabled)point=base->runtime().target_position;}
  e.clear();return point!=nullptr;
 }
 bool look(std::uintptr_t id,std::uintptr_t target,std::string& e){
  SourceCampaignCharacterBorrowV62 actor;if(!character(id,actor,e))return false;
  auto& r=*actor.character;dh2::character::ControllerCommandState32 source;
  if(!r.actor->controller||!r.services.controller||!r.services.controller(source,r,e))return required("SAME active controller",e);
  auto* active=r.actor->controller->command_state(source.global_blocked);
  if(!active||active->owner!=id||active->controller!=r.actor->controller->identity())return required("source controller378/controllable receiver",e);
  struct Dispatch {ActorLeavesV96* self;std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> actor;std::string* error;};Dispatch dispatch{this,actor.character,&e};
  const dh2::character::CharacterControlServices16 services{&dispatch,[](void* raw,const auto* q,auto* out){
   auto& d=*static_cast<Dispatch*>(raw);if(!q||!out||q->reserved)return -1;
   if(q->service==dh2::character::control_target_position){std::shared_ptr<void> pin;const float* point{};if(!d.self->target_point(q->subject,pin,point,*d.error))return -1;for(unsigned i=0;i<3;++i)out->position[i]=point[i];return 1;}
   if(q->service==dh2::character::control_look_at_point){auto& a=*d.actor->actor;if(!a.object||q->subject!=a.object->identity)return -1;const auto* point=a.source_position160_v7();if(!point)return -1;
    dh2::character::LookAtState16 state{{point[0],point[1],point[2]},a.runtime.rotation.heading_angle};
    if(dh2_character_look_at_point(&state,q->position))return -1;
    a.runtime.rotation.heading_angle=state.heading_angle;return 1;
   }
   required("unexpected Character controllable LookAt operation",*d.error);return -1;
  }};
  return dh2_character_controller_character(active,dh2::character::controller_look_object,target,&services)==1?true:required("whole Cmd_LookAt/Character controllable",e);
 }
 bool fields(const SourceCampaignCharacterBorrowV62& actor,dh2::world::GameObjectInitializationFieldsV62& out,std::string& e)const{
  return actor.character&&actor.character->actor&&actor.character->actor->inherited_initialization_fields_v62(actor.character,out,e);
 }
 bool controller(const SourceCampaignCharacterBorrowV62& actor,dh2::character::ControllerCommandState32*& out,std::string& e)const{
  out=nullptr;auto& r=*actor.character;dh2::character::ControllerCommandState32 source;
  if(!r.actor->controller||!r.services.controller||!r.services.controller(source,r,e))return required("SAME scripted active controller",e);
  out=r.actor->controller->command_state(source.global_blocked);
  return out&&out->owner==r.actor->object->identity&&out->controller==r.actor->controller->identity()?true:required("actual scripted controller378",e);
 }
 bool force(const SourceCampaignCharacterBorrowV62& actor,bool value,std::string& e)const{
  dh2::character::ControllerCommandState32* actual{};if(!controller(actor,actual,e))return false;actual->forced=value?1:0;return true;
 }
 bool play(std::uintptr_t id,std::int32_t first,std::int32_t second,std::int32_t base,std::string& e){
  SourceCampaignCharacterBorrowV62 actor;SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!character(id,actor,e)||!current(c,w,e)||!w->character_cache_v116||!w->character_cache_v116->ready()||
     actor.character->services.animation_tables!=&w->character_cache_v116->animation_tables())return required("SAME mutable authored AnimTable",e);
  if(!w->character_cache_v116->source_script_animation_steps_v116(base,first,second,e))return false;
  const auto animation=static_cast<std::int64_t>(base)+2;
  return source_campaign_character_set_anim_state_v116(w->owner,id,static_cast<std::int32_t>(animation),false,true,e);
 }
 bool animation_blocking(std::uintptr_t id,std::int32_t base,bool& blocking,std::string& e){
  blocking=false;SourceCampaignCharacterBorrowV62 actor;dh2::world::GameObjectInitializationFieldsV62 f;
  if(!character(id,actor,e)||!fields(actor,f,e)||!actor.character->actor->machine)return false;
  const auto* stopped=f.integer?f.integer(0x194):nullptr;if(!stopped)return required("actual GameObject194 script-animation gate",e);
  if(*stopped){e.clear();return true;}
  blocking=static_cast<std::int64_t>(base)+2==actor.character->actor->machine->state().current_animation;e.clear();return true;
 }
 bool disable_collisions(const SourceCampaignCharacterBorrowV62& actor,std::string& e){
  auto& r=*actor.character;SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!current(c,w,e)||!c.floors||!c.navigation_registry)return required("SAME PF obstacle owner",e);
  const auto physical=r.actor->position_fields_v7().physical2dc;
  if(physical){if(!r.physical_owner_v62||physical!=reinterpret_cast<std::uintptr_t>(r.physical_owner_v62.get()))return required("actual scripted physical2dc filter",e);
   if(!r.physical_owner_v62->disable_filter()){e=r.physical_owner_v62->error();return false;}}
  //Character's real virtualb4/b8/bc are true/50/20. DisableCollisions
  //removes its obstacle independently of the still-present physical slot.
  const dh2::navigation::ObstacleInitRequest q{&c.floors->collision_world,&c.navigation_registry->registry(),
   &r.actor->runtime.object,r.actor->object->identity,50.f,20.f,0,0};
  if(dh2_nav_init_obstacle(&q))return required("whole PF InitObstacle(false)",e);e.clear();return true;
 }
 bool physical_null(const SourceCampaignCharacterBorrowV62& actor,std::string& e){
  auto& r=*actor.character;if(!r.services.debug||!r.services.debug_files)return required("SetPhysicalObject Debug owner",e);
  std::uint32_t disabled{};
  if(dh2_character_debug_load(r.services.debug,r.services.debug_files)!=1||
     dh2_character_debug_get(&disabled,r.services.debug,"MP_NoPhysics",r.services.debug_files)!=1)return required("actual MP_NoPhysics query",e);
  if(disabled){e.clear();return true;} //Original NULL incoming body branch.
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!current(c,w,e)||!c.floors||!c.navigation_registry||!c.physical_world||!c.physical_world->cleanup_delivery_idle_v106())return required("actual idle physical replacement",e);
  const auto captured=r.actor->position_fields_v7().physical2dc;
  if(captured){auto* body=r.physical_owner_v62.get();if(!body||captured!=reinterpret_cast<std::uintptr_t>(body))return required("SAME SetPhysicalObject old body",e);
   if(!body->release()){e=body->error();return false;}
   if(r.physical_owner_v62.get()!=body||r.actor->position_fields_v7().physical2dc)return required("actual physical D1/detachment receipt",e);
   r.physical_owner_v62.reset();
  }
  return dh2::world::canonical_character_update_pf_v62(r,c.floors->collision_world,c.navigation_registry->registry(),e);
 }
 bool warp(const SourceCampaignCharacterBorrowV62& actor,std::uintptr_t target,std::string& e){
  dh2::character::ControllerCommandState32* control{};if(!controller(actor,control,e))return false;
  if(!control->forced&&(control->global_blocked||control->locked)){e.clear();return true;}
  auto& r=*actor.character;const auto* remote=r.actor->source_bool_field(0x118);
  if(!remote||!r.actor->machine)return required("actual Ctrl_WarpTo remote receiver",e);
  if(r.actor->machine->combat_fields().network_id!=-1||*remote){e.clear();return true;}
  std::shared_ptr<void> target_pin;const float* target_position{};
  if(!target_point(target,target_pin,target_position,e))return false;
  std::array<float,3> point{target_position[0],target_position[1],target_position[2]};
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignFrameEnvironmentV76 environment;
  if(!current(c,w,e)||!borrow_source_campaign_frame_environment_v76(c,r.actor->runtime.path.count,1,environment,e))return false;
  const dh2::navigation::ObjectPositionRequest q{environment.geometry,environment.registry,&r.actor->runtime.object,id_of(actor),point.data(),environment.motion_policy};
  dh2::navigation::PositionResult result{};if(dh2_nav_validate_object_position(&result,&q))return required("whole PF ValidatePosition at Ctrl_WarpTo",e);
  if(!result.valid){e.clear();return true;}
  if(!r.set_position(point,true,e))return false;
  //Original1e0 is the SAME embedded PFObject position, not another pose.
  std::copy(point.begin(),point.end(),r.actor->runtime.object.motion.position);
  return source_campaign_character_control_stop_v116(w->owner,id_of(actor),e)&&source_campaign_character_raise_event_v114(w->owner,id_of(actor),1,0,e);
 }
 static std::uintptr_t id_of(const SourceCampaignCharacterBorrowV62& actor){return actor.character->actor->object->identity;}
 bool move(std::uintptr_t id,std::uintptr_t target,bool skip,bool disabled,bool wait,std::uint8_t& original_static,bool& retained_wait,std::string& e){
  retained_wait=wait;SourceCampaignCharacterBorrowV62 actor;dh2::world::GameObjectInitializationFieldsV62 f;
  if(!character(id,actor,e)||!fields(actor,f,e))return false;auto* stat=f.byte?f.byte(0x84):nullptr;
  if(!stat)return required("actual MoveActor captured byte84",e);original_static=*stat;
  if(disabled&&!disable_collisions(actor,e))return false;if(!force(actor,true,e))return false;
  if(skip){if(!warp(actor,target,e))return false;return force(actor,false,e);}
  if(original_static){
   if(!wait)return force(actor,false,e);
   if(!physical_null(actor,e))return false;*stat=0;
  }
  if(!source_campaign_character_command_move_v116(actor.actual_world,id,target,e))return false;
  SourceCharacterPathBorrowV105 path;if(!borrow_source_campaign_character_path_v105(actor.actual_world,id,path,e)||!path.path)return false;
  if(path.path->count)return force(actor,false,e);
  retained_wait=false;if(!warp(actor,target,e))return false;return force(actor,false,e);
 }
 bool move_blocking(std::uintptr_t id,std::uint8_t original_static,bool& blocking,std::string& e){
  blocking=false;SourceCampaignCharacterBorrowV62 actor;if(!character(id,actor,e))return false;
  SourceCharacterPathBorrowV105 path;if(!borrow_source_campaign_character_path_v105(actor.actual_world,id,path,e)||!path.path)return false;
  if(path.path->count){blocking=true;e.clear();return true;}
  if(!source_campaign_character_command_stop_v101(actor.actual_world,id,e))return false;
  if(original_static){dh2::world::GameObjectInitializationFieldsV62 f;if(!fields(actor,f,e))return false;
   auto* stat=f.byte?f.byte(0x84):nullptr;if(!stat)return required("MoveActor restore captured byte84",e);*stat=1;
   if(!actor.character->revive_v70(0,1,e))return false;
  }
  e.clear();return true;
 }
 bool scripted(std::uintptr_t id,bool value,std::string& e){
  SourceCampaignCharacterBorrowV62 actor;dh2::world::GameObjectInitializationFieldsV62 f;
  if(!character(id,actor,e)||!fields(actor,f,e))return false;auto* cell=f.byte?f.byte(0x1480):nullptr;
  if(!cell)return required("actual Character scripted1480",e);*cell=value?1:0;e.clear();return true;
 }
 bool put_idle(std::uintptr_t id,bool wait,std::string& e){
  SourceCampaignCharacterBorrowV62 actor;if(!character(id,actor,e)||!actor.character->life||!actor.character->actor->machine)return false;
  if(actor.character->life->dead){e.clear();return true;}
  auto& machine=*actor.character->actor->machine;std::int32_t state{};
  if(dh2_character_native_fsm_get_integer(&state,&machine.native_fsm(),0)!=1)return false;
  if(state==0){e.clear();return true;}
  if(dh2_character_native_fsm_get_integer(&state,&machine.native_fsm(),0)!=1)return false;
  if(state==17){e.clear();return true;}
  machine.state().idle_suppressed=wait?1:0;
  if(machine.transition(3,-1,0)<0){e=machine.error();return false;}e.clear();return true;
 }
 bool selected_character(std::uintptr_t id,std::uintptr_t& character_id,SourceCampaignCandidateBorrowV55& c,std::shared_ptr<SourceWorldBorrowV61>& w,std::string& e)const{
  character_id=0;if(!current(c,w,e)||!c.objects)return false;
  const dh2::world::CanonicalObjectBorrowV1* object{};std::int32_t key{};
  bool next=c.objects->source_ordered_begin_v38(key,object);
  while(next&&(!object||object->identity!=id))next=c.objects->source_ordered_next_v38(key,key,object);
  if(!next||!object||!object->lease||!object->as_character)return required("actual scripted GameObject receiver",e);
  const auto captured=*object;return captured.as_character(captured.context,character_id,e);
 }
 bool set_position(std::uintptr_t id,const float* point,bool destination,std::string& e){
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;std::uintptr_t character_id{};
  if(!point||!selected_character(id,character_id,c,w,e))return false;
  if(!character_id)return w->gameobject_graph_v68&&w->gameobject_graph_v68->source_set_position_v96(id,point,destination,e);
  SourceCampaignCharacterBorrowV62 actor;if(!character(character_id,actor,e))return false;
  std::array<float,3> p{point[0],point[1],point[2]};return actor.character->set_position(p,destination,e);
 }
 bool visible(std::uintptr_t id,bool value,std::string& e){
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;std::uintptr_t character_id{};
  if(!selected_character(id,character_id,c,w,e))return false;
  if(character_id)return source_campaign_character_set_visible_v96(w->owner,character_id,value,e);
  std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
  if(!borrow_source_campaign_object_base_v77(c,id,pin,base,e)||!base)return false;
  auto* stored=base->byte(0x80);const auto* enabled=base->byte(0x8a);
  if(!stored||!enabled)return required("actual generic visible80/enabled8a",e);
  *stored=value?*enabled:0;
  return source_campaign_noncharacter_sync_visibility_v105(c,id,e);
 }
 bool reload(std::uintptr_t id,std::string& e){
  SourceCampaignCharacterBorrowV62 actor;if(!character(id,actor,e))return false;auto& r=*actor.character;
  auto* skills=r.player_script_owner_v62.get();
  if(!skills||!skills->ready()||!skills->native_buffs())return required("SAME player AI/buff owners for ReloadSkills",e);
  dh2::character::BuffResult24 removed{};
  if(dh2_character_buffs_remove_all(&removed,skills->native_buffs())!=1)return required("whole PROPS_RemoveAllBuffs",e);
  dh2::character::NpcInitPostRequestV1 q;q.source_entry=0x3bc4d0;q.subject=id;q.argument0=32;dh2::character::NpcInitPostResponseV1 response;
  if(!dh2::world::CanonicalCharacterCandidateRecordV60::init_service(&r,q,response,e))return false;
  if(!r.save_fields||!r.save||!r.load||*r.save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(r.save.get())||&r.load->save()!=r.save.get())return required("SAME positive Character14e8 Save for retained player AI reload",e);
  struct Reload {dh2::world::CanonicalCharacterCandidateRecordV60* r;std::string* e;};Reload reload{&r,&e};
  const dh2::character::skills::SkillSaveReloadServicesV6 services{&reload,
   [](void* raw,auto* save,std::uintptr_t character,const auto** rows){auto& d=*static_cast<Reload*>(raw);auto& r=*d.r;
    if(!rows||!r.actor||character!=r.actor->object->identity||save!=r.save.get()||!r.services.skills)return -1;
    auto index=r.properties->resolved[28];const auto& lists=r.services.skills.lists();if(index<0||std::size_t(index)>=lists.size())index=3;
    if(index<0||std::size_t(index)>=lists.size()){required("GetSkillsList fallback3",*d.e);return -1;}*rows=&lists[static_cast<std::size_t>(index)];return 0;
   },
   [](void* raw,auto* save,std::uint32_t mask){auto& d=*static_cast<Reload*>(raw);if(save!=d.r->save.get()||!d.r->load||&d.r->load->save()!=save)return -1;return d.r->load->load(static_cast<std::int32_t>(mask),*d.e)?0:-1;}
  };
  if(skills->native_reload_skills(&services)!=1||skills->update()!=1){if(e.empty())e=skills->error();return required("whole AI_ReloadSkills/UpdateAllSkills",e);}
  q.source_entry=0x3e0810;q.argument0=1;if(!dh2::world::CanonicalCharacterCandidateRecordV60::init_service(&r,q,response,e))return false;
  if(!r.equipment||r.equipment.get()!=r.prepared_equipment_v60||!r.equipment->check_item_requirements_v1(e))return required("SAME INV_CheckItemsRequirements",e);
  return verify_specialization(id,e);
 }
 // Character.VerifySpecialization3bd000 is also called directly by PM at
 // source stage26; it must not replay ReloadSkills/buffs/equipment prefixes.
 bool verify_specialization(std::uintptr_t id,std::string& e){
  SourceCampaignCharacterBorrowV62 actor;if(!character(id,actor,e))return false;auto& r=*actor.character;
  const auto get_save=[&](const dh2::data::PlayerSavegameV1*& save){
   save=nullptr;if(!r.save_fields)return required("actual SG getter14e8 slot",e);
   const auto source=*r.save_fields->save_slot14e8();if(!source)return true;
   if(!r.save||source!=reinterpret_cast<std::uintptr_t>(r.save.get()))return required("actual SG getter Save receiver",e);
   save=r.save.get();return true;
  };
  const dh2::data::PlayerSavegameV1* save{};if(!get_save(save))return false;
  const auto level=save?save->level():-1;bool specialization=false;
  if(level>11){
   //Source repeats the guarded SG_GetPlayerClass call at each failed match.
   if(!get_save(save))return false;
   if((save?save->class_id():-1)==263)specialization=true;
   else{if(!get_save(save))return false;if((save?save->class_id():-1)==325)specialization=true;
    else{if(!get_save(save))return false;if((save?save->class_id():-1)==290)specialization=true;}}
  }
  return source_native_character_spec_time_v96(actor.actual_world,specialization,e);
 }
};
bool actor_context_v116(const std::shared_ptr<void>& world,ActorLeavesV96& services,std::string& e){
 SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> source;
 if(!world||!borrow_source_campaign_candidate_runtime_v61(c,e)||c.actual_world!=world||
    c.actual_world.owner_before(world)||world.owner_before(c.actual_world)||
    !borrow_source_campaign_condition_world_v70(c,source,e))return false;
 services.world=source;return true;
}
}
bool source_campaign_character_verify_specialization_v134(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& e){
 ActorLeavesV96 services;return actor_context_v116(world,services,e)&&services.verify_specialization(id,e);
}
bool source_campaign_character_play_animation_v116(const std::shared_ptr<void>& world,std::uintptr_t id,std::int32_t first,std::int32_t second,std::int32_t base,std::string& e){
 ActorLeavesV96 services;return actor_context_v116(world,services,e)&&services.play(id,first,second,base,e);
}
bool source_campaign_character_animation_blocking_v116(const std::shared_ptr<void>& world,std::uintptr_t id,std::int32_t base,bool& blocking,std::string& e){
 ActorLeavesV96 services;return actor_context_v116(world,services,e)&&services.animation_blocking(id,base,blocking,e);
}
bool source_campaign_character_move_actor_v116(const std::shared_ptr<void>& world,std::uintptr_t id,std::uintptr_t target,bool skip,bool disabled,bool wait,std::uint8_t& original_static,bool& retained_wait,std::string& e){
 ActorLeavesV96 services;return actor_context_v116(world,services,e)&&services.move(id,target,skip,disabled,wait,original_static,retained_wait,e);
}
bool source_campaign_character_move_blocking_v116(const std::shared_ptr<void>& world,std::uintptr_t id,std::uint8_t original_static,bool& blocking,std::string& e){
 ActorLeavesV96 services;return actor_context_v116(world,services,e)&&services.move_blocking(id,original_static,blocking,e);
}
bool source_campaign_object_set_visible_v116(const std::shared_ptr<void>& world,std::uintptr_t id,bool value,std::string& e){
 ActorLeavesV96 services;return actor_context_v116(world,services,e)&&services.visible(id,value,e);
}
bool source_campaign_character_command_look_v114(const std::shared_ptr<void>& world,std::uintptr_t id,std::uintptr_t target,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> source;
 if(!borrow_source_campaign_candidate_runtime_v61(candidate,e)||candidate.actual_world!=world||
    !borrow_source_campaign_condition_world_v70(candidate,source,e))return false;
 ActorLeavesV96 services;services.world=source;return services.look(id,target,e);
}
bool source_campaign_character_set_visible_v96(const std::shared_ptr<void>& world,std::uintptr_t id,bool visible,std::string& e){
 SourceCampaignCharacterBorrowV62 actor;if(!borrow_source_campaign_character_v62(world,id,actor,e))return false;
 std::uint8_t stored{};if(!actor.character->actor->source_set_visible80_v96(visible,stored,e))return false;
 return source_campaign_character_sync_visibility_v86(world,id,e);
}
bool source_campaign_character_sync_visibility_v86(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> actual;
 if(!world||!borrow_source_campaign_candidate_runtime_v61(candidate,e)||candidate.actual_world!=world||!borrow_source_campaign_condition_world_v70(candidate,actual,e))return false;
 ActorLeavesV96 services;services.world=actual;SourceCampaignCharacterBorrowV62 actor;
 if(!services.character(id,actor,e))return false;auto& r=*actor.character;
 const auto* stored=r.actor->source_bool_field(0x80);if(!stored)return required("actual inherited visible80",e);
 std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> v;if(!services.visual(actor.character,v,e))return false;
 if(!v){e.clear();return true;}bool local=*stored!=0;
 if(local){bool player{};if(!r.is_player(player,e))return false;
  if(!player){std::int32_t ai{};const auto* rows=r.design.ai();if(!rows||dh2_character_target_ai_id(&ai,r.properties->resolved.data(),static_cast<std::uint32_t>(rows->rows.size()))||ai<0||std::size_t(ai)>=rows->rows.size())return required("actual IsFaerie/IsZonable AI row",e);
   if(rows->rows[static_cast<std::size_t>(ai)].type!=3){const auto* zoned=r.actor->source_bool_field(0x2ee);if(!zoned)return required("source zoned-visibility2ee",e);if(*zoned){const auto* in_zone=r.actor->source_bool_field(0x2f0);if(!in_zone)return required("source zone-membership2f0",e);if(!*in_zone)local=false;}}
  }
 }
 return v->set_root_local_visibility_v3(local,e);
}
bool build_source_campaign_script_actor_leaves_v96(const SourceCampaignCandidateBorrowV55& candidate,dh2::android_ui::SourceScriptActorLeavesV96& out,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(candidate,world,e)||!world->owner||!world->application||!world->gameobject_graph_v68)return required("existing candidate/graph actor authorities",e);
 auto services=std::make_shared<ActorLeavesV96>();services->world=world;dh2::android_ui::SourceScriptActorLeavesV96 result;result.owner=services;
 result.set_idle=[services](std::uintptr_t id,bool mode,auto& e){SourceCampaignCharacterBorrowV62 r;if(!services->character(id,r,e)||!r.character->actor->machine)return required("SAME source StateMachine",e);auto& machine=*r.character->actor->machine;machine.state().idle_suppressed=mode?1:0;const auto code=machine.transition(3,-1,0);if(code<0){e=machine.error();return false;}e.clear();return true;};
 result.controller_look_at=[services](auto id,auto target,auto& e){return services->look(id,target,e);};
 result.generic_set_position=[services](auto id,const float* point,bool destination,auto& e){return services->set_position(id,point,destination,e);};
 result.force_update_position=[services](auto id,auto& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!services->current(c,w,e)||!c.objects)return false;
  const dh2::world::CanonicalObjectBorrowV1* object{};std::int32_t key{};bool next=c.objects->source_ordered_begin_v38(key,object);while(next&&(!object||object->identity!=id))next=c.objects->source_ordered_next_v38(key,key,object);
  if(!next||!object||!object->as_character)return required("actual ForceUpdatePosition receiver",e);std::uintptr_t character{};if(!object->as_character(object->context,character,e))return false;
  if(!character)return w->gameobject_graph_v68&&w->gameobject_graph_v68->source_force_update_position_v96(id,e);
  SourceCampaignCharacterBorrowV62 r;std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> v;if(!services->character(character,r,e)||!services->visual(r.character,v,e))return false;if(v)v->source_force_update_position_v96();e.clear();return true;
 };
 result.render_visible=[services](auto id,bool value,auto& e){return services->visible(id,value,e);};
 result.reload_skills=[services](auto id,auto& e){return services->reload(id,e);};
 result.play_animation=[services](auto id,auto first,auto second,auto base,auto& e){return services->play(id,first,second,base,e);};
 result.animation_blocking=[services](auto id,auto base,bool& blocking,auto& e){return services->animation_blocking(id,base,blocking,e);};
 result.move_actor=[services](auto id,auto target,bool skip,bool disabled,bool wait,std::uint8_t& original_static,bool& retained_wait,auto& e){return services->move(id,target,skip,disabled,wait,original_static,retained_wait,e);};
 result.move_blocking=[services](auto id,std::uint8_t original_static,bool& blocking,auto& e){return services->move_blocking(id,original_static,blocking,e);};
 result.disable_zoning=[services](auto id,auto& e){SourceCampaignCharacterBorrowV62 actor;return services->character(id,actor,e)&&source_campaign_character_zoning_v108(actor.actual_world,id,false,e);};
 result.stop_actor=[services](auto id,auto& e){SourceCampaignCharacterBorrowV62 actor;return services->character(id,actor,e)&&source_campaign_character_command_stop_v101(actor.actual_world,id,e);};
 result.mark_scripted=[services](auto id,bool value,auto& e){return services->scripted(id,value,e);};
 result.put_idle=[services](auto id,bool wait,auto& e){return services->put_idle(id,wait,e);};
 out=std::move(result);e.clear();return true;
}
}
