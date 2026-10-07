#include "source_campaign_character_gameobject_v111.hpp"
#include "source_campaign_fx_v77.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "source_campaign_frame_environment_v76.hpp"
#include "source_campaign_physical_filter_v105.hpp"
#include "source_campaign_avoidance_collision_v108.hpp"
#include "source_campaign_object_update_actor_v104.hpp"
#include "source_campaign_anchor_v75.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "model_renderer.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <actor_runtime.hpp>
#include <gameobject_update_prefix_v23.hpp>
#include <camera_anchor_owner_v75.hpp>
#include <gameplay_camera_application_v23.hpp>
#include <gameplay_camera_picking_v20.hpp>
#include <application_player_manager_bootstrap_v59.hpp>
#include <level_can_move_towards_v108.hpp>
#include <campaign_navigation_registry_v64.hpp>
#include <floors.hpp>
#include <canonical_point3d_globals_v1.hpp>
#include <algorithm>
#include <cstring>
#include <cmath>
namespace model_renderer {namespace {
struct CharacterGoFrameV111 {
 SourceCampaignCandidateBorrowV55 scope;
 std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> record;
 dh2::world::GameObjectInitializationFieldsV62 fields;
 SourceCampaignFrameEnvironmentV76 environment;
 std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;
 std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> target_visual;
 std::shared_ptr<dh2::camera::CameraAnchorOwnerV75> anchor;
 std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11> camera;
 dh2::actor::RuntimePolicy policy{};
 dh2::subobjects::Services subobjects{this,invoke};
 dh2::subobjects::AuxiliaryBranchServicesV69 auxiliary{this,borrow_auxiliary};
 dh2::navigation::ControllerSourceServicesV69 path{this,skip_boundary};
 dh2::actor::GenericTargetPositionServicesV80 target{this,borrow_target};
 std::vector<dh2::navigation::AvoidanceActor> actors;
 std::vector<std::uint64_t> keys;
 std::vector<std::shared_ptr<void>> physical_pins;
 dh2::navigation::AvoidanceScene avoidance{};
 std::unique_ptr<SourceAvoidanceCollisionV108> collision;
 bool auxiliary_updated{};std::string error;
 bool current(){SourceCampaignCandidateBorrowV55 actual;
  if(!borrow_source_campaign_candidate_v55(actual,error)||actual.actual_world!=scope.actual_world){error="Retired SAME Character GameObject frame World";return false;}return true;}
 bool camera_borrow(){dh2::loader::CanonicalCurrentLevelBorrowV1 level;
  if(!borrow_current_native_level_v27(level,error)||!level){error="Required actual Level128 camera after auxiliary Update";return false;}
  auto application=scope.camera_application;auto session=application?application->world():nullptr;
  camera=session&&session->camera?session->camera->level():nullptr;
  if(!camera||!camera->level()||level.level()->constructor_fields_v3().field128!=reinterpret_cast<std::uintptr_t>(camera.get())){error="Required SAME loaded Level128 camera receiver";return false;}return true;}
 bool can_move(bool& allowed){
  using namespace dh2::world;LevelCanMoveServicesV108 s;s.owner=record;
  s.count6c4=[&](std::int32_t& out,std::string& e){auto pm=scope.application->source_player_manager_v59();if(!pm||!pm->manager()){e="Required SAME PM6c4";return false;}out=*pm->manager()->character_count_field();return true;};
  s.online5=[&](std::uint8_t& out,std::string& e){auto online=scope.application->get_online_loading_v55();if(!online){e="Required GetOnline5";return false;}out=online->byte5();return true;};
  s.camera_parent4=[&](bool& present,std::array<float,3>& out,std::string&){if(!camera_borrow())return false;present=bool(camera->scene());if(!present)return true;dh2::camera::PointV2 eye;return camera->source_positions_v67(eye,out,error);};
  s.limits=[&](std::array<float,3>& out,std::string& e){std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(scope,world,e)||!world->settings)return false;
   auto settings=world->settings->borrow();const unsigned indices[]{6,7,5};for(unsigned i=0;i<3;++i){auto word=settings.word(0,indices[i]);if(!word){e="Required actual DesignSettings screen limits";return false;}std::memcpy(&out[i],word,4);}return true;};
  s.assertion=[](std::uint32_t line,const char*,std::string& e){e="Original Level.CanMoveTowards assertion line "+std::to_string(line);return false;};
  s.player=[&](unsigned index,LevelCanMovePartyV108& out,std::string& e){auto pm=scope.application->source_player_manager_v59();dh2::player::PlayerInfoFieldsV1* info{};
   if(!pm||!pm->manager()||!pm->manager()->get_player(index,false,info,e)||!info)return false;out.character=info->character660;if(!out.character)return true;
   SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(scope.actual_world,out.character,actual,e)||!actual.character->life)return false;
   out.receiver=actual.character;out.dead=actual.character->life->dead!=0;out.position160=actual.character->actor->source_position160_v7();return true;};
  s.screen=[&](const float* position,std::array<float,2>& out,std::string&){if(!camera_borrow())return false;dh2::camera::CameraViewV11 view;if(!camera->view(view,error))return false;
   dh2_camera_screen_coord_v20(out.data(),position,view.view.values,view.projection.values);return true;};
  return level_can_move_towards_v108(record->actor->runtime.subobjects.position,record->actor->runtime.subobjects.heading,s,allowed,error);
 }
 bool idle_sound(std::int16_t sound){
  dh2::audio::AudioApplicationBorrowV42 captured;if(!borrow_actual_application_audio_v42(captured,error))return false;
  if(!captured.manager)return true;
  auto disabled=fields.byte(0x373),playing=fields.byte(0x372);
  if(!disabled||!playing){error="Required actual GameObject idle sound372/373 cells";return false;}
  if(*disabled){if(!*playing)return true;*playing=0;return stop_campaign_sound_v106(scope.actual_world,sound,1000,error);}
  dh2::loader::CanonicalCurrentLevelBorrowV1 current;
  if(!borrow_current_native_level_v27(current,error))return false;if(!current)return true;
  auto pm=scope.application->source_player_manager_v59();dh2::player::PlayerInfoFieldsV1* info{};
  if(!pm||!pm->get_local_player(0,true,info,error)||!info)return false;if(!info->character660)return true;
  if(!borrow_current_native_level_v27(current,error)||!current){error="Current Level removed during UpdateIdleSound source reload";return false;}
  if(current.level()->constructor_fields_v3().field130!=38)return true;
  std::int32_t raw_radius{};auto design=record->design.design();
  if(!design||!design->lookup||design->lookup(design->context,0,"AudioConstants","CameraMaxDistance",&raw_radius)){error="Required actual AudioConstants.CameraMaxDistance";return false;}
  const float radius=static_cast<float>(raw_radius);
  dh2::camera::PointV2 listener=dh2::world::canonical_vec3_origin_v1();
  if(!pm->get_local_player(0,true,info,error)||!info)return false;
  if(info->character660){if(!pm->get_local_player(0,true,info,error)||!info||!info->character660){error="Local idle listener removed during source reload";return false;}
   SourceCampaignCharacterBorrowV62 player;if(!borrow_source_campaign_character_v62(scope.actual_world,info->character660,player,error))return false;
   std::copy_n(player.character->actor->source_position160_v7(),3,listener.data());}
  const auto position=record->actor->source_position160_v7();
  volatile float x=position[0]-listener[0],y=position[1]-listener[1],z=position[2]-listener[2];
  volatile float xx=x*x,yy=y*y,zz=z*z,sum=xx+yy,total=sum+zz;
  const float distance=static_cast<float>(std::sqrt(static_cast<double>(total)));
  if(*playing&&radius<=distance&&radius>0.f){*playing=0;return stop_campaign_sound_3d_v112(scope.actual_world,sound,1000,listener.data(),radius,error);}
  if(*playing||!(radius>distance&&radius>0.f))return true;
  *playing=1; //source store precedes synchronous Play3D
  dh2::character::CombatSoundPlayV1 play;play.manager=captured.identity();play.sound_id=sound;
  std::copy_n(position,3,play.position.data());play.source_bool=true;play.source_integer=1;play.source_float0=play.source_float1=-1.f;
  return submit_campaign_audio_v46(dh2::audio::AudioCategoryV46::ambience,captured,scope.actual_world,play,error);
 }
 static int borrow_auxiliary(void* p,dh2::subobjects::AuxiliaryBranchV69* out){auto& self=*static_cast<CharacterGoFrameV111*>(p);const auto slot=self.fields.pointer(0x2e0);
  if(!out||!self.current()||!slot||!*slot||!borrow_source_campaign_anchor_v80(self.scope.actual_world,*slot,self.anchor,self.error))return 1;
  const auto& f=self.anchor->fields();out->type=f.type4;out->mode=f.state3c;out->position_c=f.position_c.data();return 0;}
 static int borrow_target(void* p,const float** out){auto& self=*static_cast<CharacterGoFrameV111*>(p);if(!out||!self.current())return 1;
  auto slot=self.fields.pointer(0x180);if(!slot)return 1;if(!*slot){*out=nullptr;return 0;}
  if(!self.target_visual){self.error="Missing SAME retained target180 node graph";return 1;}
  return self.target_visual->borrow_node_position_v80(*slot,*out,self.error)?0:1;}
 static int skip_boundary(void* p,std::uint32_t* out){auto& self=*static_cast<CharacterGoFrameV111*>(p);const auto& s=self.record->services;
  return out&&s.debug&&s.debug_files&&dh2_character_debug_load(s.debug,s.debug_files)==1&&dh2_character_debug_get(out,s.debug,"DisableWallAvoidance",s.debug_files)==1&&self.current()?0:1;}
 static std::uint32_t invoke(void* p,std::uint32_t event,float* values){auto& self=*static_cast<CharacterGoFrameV111*>(p);using namespace dh2::subobjects;
  if(!self.current())return UINT32_MAX;
  switch(event){
   case visual_update:case visual_apply_rotation:return 1; //470cf0/470ccc literal BX LR
   case get_speed:if(values)*values=4.f;return 1; //3400e4 literal4
   case visual_sync_scaling:return !self.visual||self.visual->sync_scaling_v111(self.error)?1:UINT32_MAX;
   case auxiliary_update:{auto slot=self.fields.pointer(0x2e0);if(!slot||!*slot||!source_campaign_anchor_update_v75(self.scope.actual_world,*slot,self.error))return UINT32_MAX;self.auxiliary_updated=true;return 1;}
   case camera_get:{if(!values)return UINT32_MAX;
    if(!self.auxiliary_updated){dh2::loader::CanonicalCurrentLevelBorrowV1 level;if(!borrow_current_native_level_v27(level,self.error))return UINT32_MAX;values[0]=0.f;return bool(level);}
    if(!self.camera_borrow())return UINT32_MAX;values[0]=self.camera->level()->target().fields().offset_enabled?1.f:0.f;return 1;}
   case camera_can_move:{bool allowed;if(!self.can_move(allowed))return UINT32_MAX;return allowed;}
   case camera_position:{if(!values||!self.camera_borrow())return UINT32_MAX;dh2::camera::PointV2 eye,parent;if(!self.camera->source_positions_v67(eye,parent,self.error))return UINT32_MAX;std::copy_n(eye.data(),3,values);return 1;}
   case camera_set_free:return self.camera_borrow()&&self.camera->level()->target().enable_damping(true,self.error)?1:UINT32_MAX;
   default:self.error="Unexpected Character GameObject subobject event "+std::to_string(event);return UINT32_MAX;
  }
 }
 bool physical(std::uint64_t id,SourceAvoidanceCollisionV108::Loan& out,std::string& e){
  SourceCampaignCharacterBorrowV62 actor;std::string ignored;
  if(borrow_source_campaign_character_v62(scope.actual_world,id,actor,ignored)){
   auto r=actor.character;auto slot=r->actor->position_fields_v7().physical2dc;
   if(!slot){out.owner=r;out.physical_present=false;return true;}
   dh2::world::GameObjectInitializationFieldsV62 fields;if(!r->actor->inherited_initialization_fields_v62(r->actor,fields,e)||!fields.byte(0x80)||!r->physical_owner_v62)return false;
   dh2::physical::NativePhysicalFilterBorrowV1 loan;if(!r->physical_owner_v62->source_filter_borrow_v111(loan,e))return false;
   return SourceAvoidanceCollisionV108::from_physical_filter(r,loan,r->physical_owner_v62->world_object(),*fields.byte(0x80),out,e);
  }
  std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
  if(!borrow_source_campaign_object_base_v77(scope,id,pin,base,e)||!base||!base->pointer(0x2dc))return false;
  if(!*base->pointer(0x2dc)){out.owner=pin;out.physical_present=false;return true;}
  dh2::physical::NativePhysicalFilterBorrowV1 loan;if(!borrow_source_campaign_physical_filter_v105(scope,id,pin,loan,e))return false;
  if(loan.world!=scope.physical_world.get()){e="Avoidance peer belongs to another actual PhysicalWorld";return false;}
  auto shape=loan.primary&&*loan.primary?*loan.primary:loan.secondary?*loan.secondary:nullptr;
  if(!shape||!shape->GetUserData()||!base->byte(0x80)){e="Required actual native avoidance peer shape receiver";return false;}
  //The typed V69 loan above certifies this native shape association. NativeWorld
  //owns its WorldObject userdata; the legacy ARM vtable/tag is never consulted.
  auto* receiver=static_cast<dh2::physical::WorldObject*>(shape->GetUserData());
  return SourceAvoidanceCollisionV108::from_physical_filter(pin,loan,*receiver,*base->byte(0x80),out,e);
 }
};
}
bool source_campaign_character_gameobject_v111(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& e){
 CharacterGoFrameV111 frame;SourceCampaignCharacterBorrowV62 actual;
 if(!borrow_source_campaign_candidate_v55(frame.scope,e)||frame.scope.actual_world!=world||!borrow_source_campaign_character_v62(world,id,actual,e))return false;
 frame.record=actual.character;auto r=frame.record;auto a=r?r->actor:nullptr;
 if(!a||!a->machine||!a->inherited_initialization_fields_v62(a,frame.fields,e))return false;
 std::copy_n(a->source_position160_v7(),3,a->runtime.subobjects.position);
 dh2::world::GameObjectUpdatePrefixServicesV23 prefix;prefix.provider_lease=r;prefix.debug=r->services.debug;if(r->services.debug_files)prefix.files=*r->services.debug_files;
 prefix.context=&frame;prefix.manager38=[](void* raw,dh2::world::CanonicalObjectManagerV1*& out,std::string&){auto& f=*static_cast<CharacterGoFrameV111*>(raw);if(!f.current())return false;out=f.scope.objects.get();return out!=nullptr;};
 if(!dh2::world::gameobject_update_begin_v23(prefix,e))return false;
 auto collision=frame.fields.pointer(0x2e4);if(!collision){e="Required Character collision2e4 C1 field";return false;}
 if(*collision){e="Required selected Character.Interact98 after physical collision";return false;}
 auto visual_slot=frame.fields.pointer(0x2d8);if(!visual_slot){e="Required actual Character visual2d8 field";return false;}
 if(*visual_slot){frame.visual=r->visual?r->visual->visual():nullptr;if(!frame.visual||reinterpret_cast<std::uintptr_t>(frame.visual.get())!=*visual_slot){e="Character visual2d8 differs from retained visual receiver";return false;}}
 frame.target_visual=r->visual?r->visual->animation_visual_v112():nullptr;
 const auto count=frame.scope.navigation_registry->registry().count;
 if(!borrow_source_campaign_frame_environment_v76(frame.scope,a->runtime.path.count,count+1,frame.environment,e))return false;
 std::int32_t state;if(dh2_character_native_fsm_get_integer(&state,&a->machine->native_fsm(),0)!=1)return false;
 bool player;if(!r->is_player(player,e))return false;
 const auto physical=a->position_fields_v7().physical2dc;
 frame.policy.path={1,static_cast<std::uint32_t>(state==4||state==19),0,static_cast<std::uint32_t>(bool(physical))};frame.policy.validating_camera=player;frame.policy.virtual_speed=4.f;
 auto auxiliary=frame.fields.pointer(0x2e0);if(!auxiliary)return false;frame.policy.has_auxiliary=*auxiliary!=0;
 frame.keys.reserve(count+1);frame.actors.reserve(count+1);
 auto add=[&](std::uint64_t key){if(std::find(frame.keys.begin(),frame.keys.end(),key)!=frame.keys.end())return true;
  SourceAvoidanceCollisionV108::Loan loan;if(!frame.physical(key,loan,e))return false;
  dh2::navigation::AvoidanceActor actor{};SourceCampaignCharacterBorrowV62 character;std::string ignored;
  if(borrow_source_campaign_character_v62(world,key,character,ignored)){auto& runtime=character.character->actor->runtime;actor.object=runtime.object;std::copy_n(runtime.subobjects.destination,3,actor.target);actor.has_path=runtime.path.count!=0;std::copy_n(runtime.path.target,3,actor.path_target);}
  else{std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};if(!borrow_source_campaign_object_base_v77(frame.scope,key,pin,base,e))return false;auto& runtime=base->runtime();actor.object=runtime.object;std::copy_n(runtime.controller.destination,3,actor.target);actor.has_path=runtime.path.count!=0;std::copy_n(runtime.path.target,3,actor.path_target);}
  actor.physical=loan.fields;actor.physical.present=loan.physical_present;frame.physical_pins.push_back(std::move(loan.owner));frame.keys.push_back(key);frame.actors.push_back(actor);return true;};
 if(!add(id))return false;
 if(frame.policy.path.avoid_obstacles)for(unsigned i=0;i<count;++i)if(!add(frame.environment.registry->entries[i].object))return false;
 frame.avoidance={frame.environment.registry,frame.actors.data(),frame.keys.data(),static_cast<unsigned>(frame.keys.size()),0};
 frame.collision=std::make_unique<SourceAvoidanceCollisionV108>([&](auto own,auto& loan,auto& error){return frame.physical(own,loan,error);});frame.collision->bind(frame.avoidance);
 dh2::actor::GenericRuntimeRequestV4 request;auto& q=request.actor;
 q.state=&a->runtime;q.key=id;q.geometry=frame.environment.geometry;q.graph=frame.environment.graph;q.registry=frame.environment.registry;q.motion_policy=frame.environment.motion_policy;q.workspace=frame.environment.workspace;
 q.policy=&frame.policy;q.services=&frame.subobjects;q.avoidance=&frame.avoidance;q.dt_ms=frame.environment.dt_ms;
 if(physical){if(!r->physical_owner_v62||physical!=reinterpret_cast<std::uintptr_t>(r->physical_owner_v62.get())){e="Required SAME Character2dc body owner";return false;}q.native_body=&r->physical_owner_v62->native();}
 const auto flags=a->machine->state().flags;
 if(dh2_move_policy(&request.actual_virtual_policy,&flags)||dh2_move_rotation_speed(&request.actual_rotation_speed,&flags,r->view.resolved))return false;
 request.source_path=&frame.path;request.source_auxiliary=&frame.auxiliary;request.source_target=&frame.target;
 if(frame.visual){request.visual={&frame,&frame.visual->binding().root,
  [](void* raw,std::string& e){auto& f=*static_cast<CharacterGoFrameV111*>(raw);return f.current()&&f.visual->binding().update_world(f.visual->scene(),e);},
  [](void* raw,const float* angles,std::string& e){auto& f=*static_cast<CharacterGoFrameV111*>(raw);return f.current()&&f.visual->binding().set_rotation(angles)&&f.visual->binding().update_world(f.visual->scene(),e);}};}
 dh2::actor::RuntimeResult result;if(dh2::actor::update_gameobject_v4(result,request,e)){if(!frame.error.empty())e=frame.error;else if(!frame.collision->error().empty())e=frame.collision->error();return false;}
 std::copy_n(a->runtime.subobjects.position,3,a->object->position.data());
 frame.fields.update_absolute_aabb();
 //RequireOnlineUpdate's actual offline return happens before NetStruct100.
 auto online=frame.scope.application->get_online_loading_v55();if(!online)return false;
 if(online->byte5()){e="Required Character RequireOnlineUpdate hosting/ownership source continuation";return false;}
 auto idle=frame.fields.integer(0x370);if(!idle){e="Required SAME Character idle sound370";return false;}
 if(static_cast<std::int16_t>(*idle)>=0&&!frame.idle_sound(static_cast<std::int16_t>(*idle))){e=frame.error;return false;}
 return dh2::world::gameobject_update_end_v23(e);
}
}
