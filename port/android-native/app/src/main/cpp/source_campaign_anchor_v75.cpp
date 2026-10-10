#include "source_campaign_anchor_v75.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_conditions_v70.hpp"
#include "model_renderer.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "camera_anchor_owner_v75.hpp"
#include "gameplay_camera_anchor_v6.hpp"
#include "application_services_owner_v5.hpp"
#include <map>
namespace model_renderer {
struct SourceCampaignAnchorDirectoryV75 {
 std::weak_ptr<SourceWorldBorrowV61> world;
 std::map<std::uintptr_t,std::shared_ptr<dh2::camera::CameraAnchorOwnerV75>> anchors;
 //Retain newly allocated failed C1 prefixes; never retry C1 or dereference
 //an uninitialized anchor merely because it has a native address.
 std::vector<std::shared_ptr<dh2::camera::CameraAnchorOwnerV75>> failed_constructors;
 dh2::camera::CameraAnchorServicesV75 services(const std::shared_ptr<SourceCampaignAnchorDirectoryV75>& self){
  dh2::camera::CameraAnchorServicesV75 s;
  //Independent callback lease carries a weak directory, avoiding the native
  //directory -> Anchor -> service-provider -> directory ownership cycle.
  s.provider=std::make_shared<std::weak_ptr<SourceCampaignAnchorDirectoryV75>>(self);
  const auto weak=world;
  s.actor=[weak](auto id,auto& out,auto& e){
   auto world=weak.lock();SourceCampaignCharacterBorrowV62 borrowed;
   if(!world||!borrow_source_campaign_character_v62(world->owner,id,borrowed,e)||!borrowed.character||!borrowed.character->actor)return false;
   const auto& actor=borrowed.character->actor;auto& heading=actor->runtime.controller.heading;
   if(!actor->position_fields_v7().constructed){e="Anchor actor has no actual GameObject position C1";return false;}
   out={actor,id,actor->source_position160_v7(),&heading.active,heading.direction};e.clear();return true;
  };
  s.handle_character=[weak](auto id,auto& value,auto& e){
   auto world=weak.lock();SourceCampaignCharacterBorrowV62 borrowed;
   if(!world||!borrow_source_campaign_character_v62(world->owner,id,borrowed,e)||!borrowed.character||!borrowed.character->actor)return false;
   auto& manager=world->canonical_world->manager;auto handle=borrowed.character->actor->shared_handle();
   const dh2::world::CanonicalObjectBorrowV1* object{};
   if(!manager.resolve_handle_v4(handle,false,object,{},e))return false;
   value=0;if(object){if(!object->as_character||!object->as_character(object->context,value,e))return false;}
   e.clear();return true;
  };
  const auto state=[weak](std::uintptr_t id,std::int32_t& value,std::string& e){
   auto world=weak.lock();SourceCampaignCharacterBorrowV62 borrowed;
   if(!world||!id||!borrow_source_campaign_character_v62(world->owner,id,borrowed,e)||!borrowed.character||!borrowed.character->actor||!borrowed.character->actor->machine){e="Required SAME anchor Character state machine";return false;}
   //SM_GetState3c01ac preserves actual constructor NULL -> -1. State.current
   //is a projection; the actual selected StateInfo slot remains authoritative.
   const auto& machine=borrowed.character->actor->machine->owner().machine();
   if(machine.current_index<0){value=-1;e.clear();return true;}
   if(!machine.states||static_cast<std::uint32_t>(machine.current_index)>=machine.state_count){e="Actual selected StateInfo outside source machine";return false;}
   value=machine.states[machine.current_index].id;e.clear();return true;
  };
  s.moving_false=[state](auto id,auto& value,auto& e){std::int32_t current{};if(!state(id,current,e))return false;value=current==4||current==19;e.clear();return true;};
  s.attacking=[state](auto id,auto& value,auto& e){std::int32_t current{};if(!state(id,current,e))return false;value=current==5;e.clear();return true;};
  s.look_at=[weak](auto id,auto& value,auto& e){auto world=weak.lock();if(!world){e="Anchor actual World expired";return false;}
   dh2::camera::PointV2 out;if(!source_campaign_character_look_at_v68(world->owner,id,out,e))return false;value=out;e.clear();return true;};
  s.application_dt=[weak](auto& value,auto& e){auto world=weak.lock();
   if(!world||!world->application||!world->application->source_loading_v55().native_dt_produced_v93){e="Required actual App8c anchor frame input";return false;}
   value=world->application->source_loading_v55().dt8c;e.clear();return true;};
  s.assertion=[weak](auto line,auto expression,auto& e){auto world=weak.lock();const auto dependency=world?world->condition_dependencies_v70:nullptr;
   if(!dependency||!dependency->assertion_owner||!dependency->assertion_mode){e="Required actual process Anchor assertion mode";return false;}
   if(*dependency->assertion_mode==2){e="Original Anchor mode2 NULL write refused";return false;}
   if(*dependency->assertion_mode==1){if(!dependency->assertion){e="Required process Anchor assertion logger";return false;}return dependency->assertion("AnchorForward.cpp",line,expression,e);}
   e.clear();return true;
  };
  return s;
 }
};
namespace {
bool borrow_world(const std::shared_ptr<void>& owner,std::shared_ptr<SourceWorldBorrowV61>& out,std::string& e){
 SourceCampaignCandidateBorrowV55 current;if(!borrow_source_campaign_candidate_v55(current,e)||current.actual_world.get()!=owner.get()||
    current.actual_world.owner_before(owner)||owner.owner_before(current.actual_world)){e="Required SAME actual current source campaign World";return false;}
 return borrow_source_campaign_condition_world_v70(current,out,e);
}
bool anchor(const std::shared_ptr<void>& world,std::uintptr_t id,std::shared_ptr<dh2::camera::CameraAnchorOwnerV75>& out,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> actual;if(!borrow_world(world,actual,e))return false;
 if(!actual->camera_anchors_v75){e="Actual campaign has no source Anchor producer";return false;}
 const auto found=actual->camera_anchors_v75->anchors.find(id);
 if(found==actual->camera_anchors_v75->anchors.end()||!found->second||!found->second->constructed()||found->second->identity()!=id){e="Required SAME actual retained Anchor receiver";return false;}
 out=found->second;e.clear();return true;
}
}
bool source_campaign_character_init_camera_v75(const std::shared_ptr<void>& owner,std::uintptr_t id,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> actual;if(!borrow_world(owner,actual,e))return false;
 SourceCampaignCharacterBorrowV62 borrowed;
 if(!borrow_source_campaign_character_v62(owner,id,borrowed,e)||!borrowed.character||!borrowed.character->actor)return false;
 const auto& actor=borrowed.character->actor;auto& pointers=actor->position_fields_v7();
 if(!pointers.constructed||!actual->debug||!actual->debug_files){e="InitCam requires SAME actual Character/Debug owners";return false;}
 //Original load/GetSwitch is executed on every source InitCam call.
 std::uint32_t use_static{};
 if(dh2_character_debug_load(actual->debug.get(),actual->debug_files)!=1||
    dh2_character_debug_get(&use_static,actual->debug.get(),"UseStaticCamera",actual->debug_files)!=1){e="Original InitCam Debug provider failed";return false;}
 float maximum{},speed{},threshold{};
 if(!use_static){
  auto data=actual->design->borrow();const auto* constants=data.design();
  std::int32_t m{},s{},t{};
  if(!constants||!constants->lookup||constants->lookup(constants->context,0,"CharacterDesign","ForwardCamera_Max_Distance",&m)||
     constants->lookup(constants->context,0,"CharacterDesign","ForwardCamera_Distance_PerSec",&s)||
     constants->lookup(constants->context,0,"CharacterDesign","ForwardCamera_Threshold",&t)){e="Required actual InitCam camera constants";return false;}
  maximum=static_cast<float>(m);speed=static_cast<float>(s);volatile float fraction=static_cast<float>(t)*.01f;threshold=fraction;
 }
 if(!actual->camera_anchors_v75){actual->camera_anchors_v75=std::make_shared<SourceCampaignAnchorDirectoryV75>();actual->camera_anchors_v75->world=actual;}
 const auto& directory=actual->camera_anchors_v75;
 auto fresh=std::make_shared<dh2::camera::CameraAnchorOwnerV75>(directory->services(directory));
 directory->failed_constructors.push_back(fresh);
 if(!fresh->construct(id,!use_static,maximum,speed,threshold,e))return false;
 directory->anchors.emplace(fresh->identity(),fresh);
 dh2::camera::CameraAnchorBorrowV6 target{actor,id,&pointers.attached2e0,actor->source_position160_v7(),{},
  [owner](auto old,auto& error){return source_campaign_anchor_delete_v75(owner,old,error);}};
 if(!dh2::camera::source_set_camera_anchor_v6(target,fresh->identity(),e))return false;
 directory->failed_constructors.pop_back();e.clear();return true;
}
bool borrow_source_campaign_anchor_v80(const std::shared_ptr<void>& world,std::uintptr_t id,std::shared_ptr<dh2::camera::CameraAnchorOwnerV75>& out,std::string& e){return anchor(world,id,out,e);}
bool source_campaign_anchor_position_v75(const std::shared_ptr<void>& world,std::uintptr_t id,const float*& out,std::string& e){
 std::shared_ptr<dh2::camera::CameraAnchorOwnerV75> actual;if(!anchor(world,id,actual,e))return false;
 out=actual->fields().position_c.data();e.clear();return true;
}
bool source_campaign_anchor_attached_position_v75(const std::shared_ptr<void>& world,std::uintptr_t id,float*& out,std::string& e){
 out=nullptr;std::shared_ptr<dh2::camera::CameraAnchorOwnerV75> actual;if(!anchor(world,id,actual,e))return false;
 out=actual->source_position_c();if(!out){e="Required constructed SAME Character camera Anchor +0x0c XYZ";return false;}
 e.clear();return true;
}
bool source_campaign_anchor_update_v75(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& e){
 std::shared_ptr<dh2::camera::CameraAnchorOwnerV75> actual;if(!anchor(world,id,actual,e))return false;return actual->update(e);
}
bool source_campaign_anchor_delete_v75(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> actual;if(!borrow_world(world,actual,e))return false;
 if(!actual->camera_anchors_v75){e="Required actual Anchor deleting destructor owner";return false;}
 const auto found=actual->camera_anchors_v75->anchors.find(id);
 if(found==actual->camera_anchors_v75->anchors.end()){e="Deleting destructor addressed another source Anchor";return false;}
 actual->camera_anchors_v75->anchors.erase(found);e.clear();return true;
}
}
