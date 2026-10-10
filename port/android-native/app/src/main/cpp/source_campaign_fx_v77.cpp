#include "source_campaign_fx_v77.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_admission_v104.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "model_renderer.hpp"
#include "source_campaign_quests_v76.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <canonical_gameobject_graph_v68.hpp>
#include <character_world_runtime_v1.hpp>
#include <character_world_target_owner_v1.hpp>
#include <character_authored_resource_v6.hpp>
#include <character_authored_fx_forces_v4.hpp>
#include <character_fx_floor_sync_v29.hpp>
#include <visual_fx_preload.hpp>
#include <visual_fx_manager_libraries_v63.hpp>
#include <campaign_navigation_registry_v64.hpp>
#include <application_spawn_random_owner_v4.hpp>
#include <floors.hpp>
#include <algorithm>
#include <climits>
#include <exception>
namespace model_renderer {
class SourceCampaignFxRuntimeV77 {
public:
 std::weak_ptr<SourceWorldBorrowV61> world;
 std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> scene_visual;
 SourceCampaignFxRenderServicesV77 render;
 dh2::fx::CharacterAuthoredFxForceFactoryOwnerV4 forces;
 std::unique_ptr<dh2::fx::CharacterAuthoredResourceFactoryV6> resources;
 std::unique_ptr<dh2::fx::CharacterMeshFxOwnerV4> manager;
 bool scene_delivered{},busy{},failed{};std::uint64_t scene_epoch{};std::string failure;
 bool current(std::shared_ptr<SourceWorldBorrowV61>& out,std::string& e)const{
  out=world.lock();
  if(!out||!scene_visual||!scene_visual->ready()||!scene_visual->root_identity()){
   e="Required retained campaign FX Scene/cache lifetime";return false;
  }
  SourceCampaignCandidateBorrowV55 candidate;
  if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=out->owner){
   if(e.empty())e="FX instance addressed another current campaign";return false;
  }
  if(failed){e=failure;return false;}return true;
 }
 static bool asset(void* raw,const char* uri,std::vector<std::uint8_t>& bytes,std::string& e){
  auto& self=*static_cast<SourceCampaignFxRuntimeV77*>(raw);std::shared_ptr<SourceWorldBorrowV61> world;
  if(!uri||!self.current(world,e)||!world->read)return false;
  bool found{};if(!world->read(uri,found,bytes,e))return false;
  if(!found){e=std::string("Required authored campaign FX asset: ")+uri;return false;}return true;
 }
 static bool camera(void* raw,float* view,float* position,std::string& e){
  auto& self=*static_cast<SourceCampaignFxRuntimeV77*>(raw);std::shared_ptr<SourceWorldBorrowV61> world;
  return self.current(world,e)&&self.render.camera&&self.render.camera(view,position,e);
 }
 static bool driver(void*,std::uint32_t& out,std::string& e){
  std::int32_t actual{};if(!borrow_actual_device_driver_type_v55(actual,e)||actual<0)return false;
  out=static_cast<std::uint32_t>(actual);return true;
 }
 static bool service(void* raw,dh2::fx::MeshFxRequestV1& q,std::string& e){
  auto& self=*static_cast<SourceCampaignFxRuntimeV77*>(raw);std::shared_ptr<SourceWorldBorrowV61> world;
  if(!self.current(world,e))return false;
  if(q.live_scene!=&self.scene_visual->scene()){e="Campaign FX Scene identity changed";return false;}
  using O=dh2::fx::MeshFxOperationV1;
  if(q.operation==O::debug_load||q.operation==O::module_enabled||q.operation==O::set_switch||q.operation==O::instance_switch){
   if(!world->fx_debug){e="Required actual campaign FX Debug transport";return false;}
   const auto operation=q.operation==O::debug_load?dh2::fx::debug_load:
    q.operation==O::module_enabled?dh2::fx::debug_module:dh2::fx::debug_switch;
   return world->fx_debug(operation,q.text,q.result,e);
  }
  SourceCampaignCandidateBorrowV55 candidate;if(!borrow_source_campaign_candidate_v55(candidate,e))return false;
  SourceCampaignCharacterBorrowV62 character;
  const bool is_character=q.identity&&borrow_source_campaign_character_v62(world->owner,q.identity,character,e);
  std::shared_ptr<void> base_pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
  if(q.identity&&!is_character){e.clear();if(!borrow_source_campaign_object_base_v77(candidate,q.identity,base_pin,base,e))return false;}
  const auto record=character.character;const auto actor=record?record->actor:nullptr;
  if(q.operation==O::floor_normal){
   const float* anchor_normal=actor?actor->runtime.object.motion.normal:base?base->runtime().object.motion.normal:nullptr;
   float normal[3]{};if(!dh2::fx::character_fx_floor_sync_v29(&candidate.floors->collision_world,q.point,anchor_normal,normal,e))return false;
   std::copy_n(normal,3,q.point);return true;
  }
  if(!q.identity||(!actor&&!base)){e="Required current campaign FX anchor receiver";return false;}
  if(q.operation==O::anchor_dead){
   if(record){if(!record->life){e="Required actual Character FX IsDead storage";return false;}
    q.result=record->life->dead!=0;
   }else q.result=0; //Original GameObject::IsDead3400ac returns literal0.
   return true;
  }
  if(q.operation==O::anchor_disabled||q.operation==O::anchor_stationary){
   const auto offset=q.operation==O::anchor_disabled?0x81u:0x84u;
   const auto* value=actor?actor->source_bool_field(offset):base->byte(offset);
   if(!value){e="Required actual FX anchor byte";return false;}q.result=*value;return true;
  }
  if(q.operation==O::anchor_position){
   const float* position=nullptr;
   if(record){
    auto targets=record->services.world_targets;dh2::character::skills::WorldTargetActorBorrowV1 source{};
    if(!targets||targets->actor(q.identity,&source)||!source.position||!source.target_node||
       (*source.target_node&&!source.target_enabled)){
     e="Required SAME Character GetTargetPosition source fields";return false;
    }
    position=dh2::character::skills::dh2_world_target_position_v1(
     source.position,source.cached_target_position,*source.target_node,source.target_enabled?*source.target_enabled:0);
   }else{
    const auto* target_node=base->pointer(0x180);const auto* world_position=base->vector3(0x160);
    if(!target_node||!world_position){e="Required SAME GameObject GetTargetPosition source fields";return false;}
    if(*target_node){const auto* enabled=base->byte(0x80);
     if(!enabled){e="Required actual GameObject target enable byte80";return false;}
     if(*enabled){const auto* cached=base->vector3(0x184);
      if(!cached){e="Required actual enabled GameObject target position184";return false;}position=cached;
     }else position=world_position;
    }else position=world_position;
   }
   if(!position){e="Required actual GetTargetPosition result";return false;}
   std::copy_n(position,3,q.point);return true;
  }
  if(q.operation==O::anchor_scale){
   if(actor){std::array<float,3> value;if(!actor->source_scale(value,e))return false;std::copy(value.begin(),value.end(),q.point);return true;}
   const auto* value=base->vector3(0x120);if(!value){e="Required actual FX anchor scale120";return false;}std::copy_n(value,3,q.point);return true;
  }
  if(q.operation==O::anchor_rotation){
   std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;
   const auto* slot=base?base->pointer(0x2d8):nullptr;
   if(base&&!slot){e="Required actual generic FX anchor visual2d8 cell";return false;}
   const auto identity=actor?actor->source_visual():*slot;
   if(identity){
    if(actor)visual=record->visual?record->visual->visual():nullptr;
    else if(!world->gameobject_graph_v68||!world->gameobject_graph_v68->borrow_visual(q.identity,visual,e))return false;
    if(!visual||!visual->ready()){e="Required SAME actual FX anchor Visual2d8";return false;}
    return dh2::fx::character_fx_anchor_rotation_v28(q.point,visual->binding().root,e);
   }
   std::copy_n(actor?actor->runtime.rotation.rotation:base->runtime().rotation.rotation,3,q.point);return true;
  }
  e="Required original campaign FX operation "+std::to_string(static_cast<unsigned>(q.operation));return false;
 }
};
bool borrow_source_campaign_object_base_v77(const SourceCampaignCandidateBorrowV55& candidate,
 std::uintptr_t identity,std::shared_ptr<void>& pin,dh2::world::CanonicalGameObjectBaseOwnerV1*& base,std::string& e){
 pin.reset();base=nullptr;std::shared_ptr<SourceWorldBorrowV61> world;
 if(!borrow_source_campaign_condition_world_v70(candidate,world,e))return false;
 if(!world->gameobject_graph_v68){e="Required actual campaign canonical object graph";return false;}
 return world->gameobject_graph_v68->borrow_base_v77(identity,pin,base,e);
}
bool publish_source_campaign_fx_v77(const std::shared_ptr<void>& actual_world,std::uintptr_t identity,
 SourceCampaignFxRenderServicesV77 render,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> world;
 if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=actual_world||
    !borrow_source_campaign_condition_world_v70(candidate,world,e))return false;
 if(world->fx_runtime_v77){std::shared_ptr<SourceWorldBorrowV61> same;return world->fx_runtime_v77->current(same,e);}
 SourceCampaignCharacterBorrowV62 character;if(!borrow_source_campaign_character_v62(actual_world,identity,character,e))return false;
 // FX uses the retained animation Scene after dynamic mesh ownership moves
 // into the compiled root. This does not repopulate the source Visual2d8 slot.
 auto visual=character.character&&character.character->visual?character.character->visual->animation_visual_v112():nullptr;
 const auto libraries=candidate.application->source_fx_libraries_v63();
 if(!visual||!visual->ready()||!visual->root_identity()||!libraries||!render.camera){e="Required actual loaded Character Scene/App FX libraries/camera transport";return false;}
 try{
  auto runtime=std::make_shared<SourceCampaignFxRuntimeV77>();runtime->world=world;runtime->scene_visual=std::move(visual);runtime->render=std::move(render);
  runtime->resources=std::make_unique<dh2::fx::CharacterAuthoredResourceFactoryV6>(
   dh2::fx::CharacterBloodFxSceneServicesV2{runtime.get(),SourceCampaignFxRuntimeV77::camera,SourceCampaignFxRuntimeV77::driver},runtime->forces.factory());
  runtime->manager=std::make_unique<dh2::fx::CharacterMeshFxOwnerV4>(libraries,runtime->scene_visual->scene(),
   dh2::fx::MeshFxAssetsV1{runtime.get(),SourceCampaignFxRuntimeV77::asset},
   dh2::fx::MeshFxServicesV1{runtime.get(),SourceCampaignFxRuntimeV77::service},runtime->resources->factory());
  runtime->manager->bind_set_random_v118([weak=std::weak_ptr<SourceWorldBorrowV61>(world)](std::uint32_t bound,std::uint32_t& value,std::string& e){
   auto actual=weak.lock();SourceCampaignCandidateBorrowV55 current;
   if(!actual||!borrow_source_campaign_candidate_v55(current,e)||current.actual_world!=actual->owner){if(e.empty())e="Retired source FX Random callback World";return false;}
   auto random=actual->application?actual->application->source_random_v62():nullptr;
   if(!random||bound>INT32_MAX){e="Required SAME process source Random for FX step selection";return false;}
   std::int32_t selected{};if(dh2_loot_v2_random(&random->channel(0),static_cast<std::int32_t>(bound),&selected)!=0||selected<0){e="Original FX GetRandom delivery failed";return false;}
   value=static_cast<std::uint32_t>(selected);e.clear();return true;
  });
  world->fx_runtime_v77=runtime;
  SourceQuestMarkerDependenciesV76 markers;markers.provider=runtime;
  const auto weak=std::weak_ptr<SourceCampaignFxRuntimeV77>(runtime);
  markers.actual_fx=[weak](std::shared_ptr<void>& pin,dh2::fx::CharacterMeshFxOwnerV4*& manager,std::string& error){
   pin.reset();manager=nullptr;auto actual=weak.lock();std::shared_ptr<SourceWorldBorrowV61> world;
   if(!actual||!actual->current(world,error)||!actual->manager)return false;
   manager=actual->manager.get();pin=std::move(actual);return true;
  };
  markers.visual_owner204=[weak](auto& manager,auto identity,auto owner,auto& error){
   auto actual=weak.lock();if(!actual||actual->manager.get()!=&manager){error="Quest marker addressed another campaign FX backend";return false;}
   return manager.marker_visual_owner_v76(identity,owner,error);
  };
  markers.animator_loop=[weak](auto& manager,auto identity,bool loop,auto& error){
   auto actual=weak.lock();if(!actual||actual->manager.get()!=&manager){error="Quest marker loop addressed another campaign FX backend";return false;}
   return manager.marker_loop_v76(identity,loop,error);
  };
  if(!bind_source_campaign_quest_markers_v76(candidate,std::move(markers),e)){
   runtime->failed=true;runtime->failure=e;return false;
  }
  e.clear();return true;
 }catch(const std::exception& failure){e=failure.what();return false;}
}
bool borrow_source_campaign_fx_v77(const std::shared_ptr<void>& actual_world,std::shared_ptr<void>& pin,
 dh2::fx::CharacterMeshFxOwnerV4*& manager,std::string& e){
 pin.reset();manager=nullptr;SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> world;
 if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=actual_world||
    !borrow_source_campaign_condition_world_v70(candidate,world,e))return false;
 auto runtime=world->fx_runtime_v77;
 if(!runtime||!runtime->manager){e="Required fresh campaign FX instance backend";return false;}
 std::shared_ptr<SourceWorldBorrowV61> same;if(!runtime->current(same,e))return false;
 manager=runtime->manager.get();pin=std::move(runtime);e.clear();return true;
}
bool source_campaign_fx_scene_v77(const SourceCampaignCandidateBorrowV55& candidate,std::uint64_t epoch,std::uint32_t time,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(candidate,world,e))return false;
 auto runtime=world->fx_runtime_v77;if(!runtime||!runtime->manager){e="Required campaign FX backend before Scene phase";return false;}
 if(runtime->scene_delivered&&runtime->scene_epoch==epoch)return true;
 if(runtime->busy||runtime->failed){e=runtime->failed?runtime->failure:"Reentrant campaign FX Scene phase";return false;}
 runtime->scene_delivered=true;runtime->scene_epoch=epoch;runtime->busy=true;
 std::uint32_t dt{};bool delivered=false;
 try{delivered=borrow_application_dt_v93(dt,e)&&runtime->manager->scene_frame(static_cast<std::int32_t>(time),static_cast<std::int32_t>(dt),e);}
 catch(const std::exception& failure){e=failure.what();}
 runtime->busy=false;if(!delivered){runtime->failed=true;runtime->failure=e;}return delivered;
}
bool update_source_campaign_fx_v77(const SourceCampaignCandidateBorrowV55& candidate,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(candidate,world,e))return false;
 auto runtime=world->fx_runtime_v77;std::shared_ptr<SourceWorldBorrowV61> same;
 if(!runtime||!runtime->current(same,e)||!runtime->manager){if(e.empty())e="Required fresh campaign FX manager update";return false;}
 if(runtime->busy){e="Reentrant campaign FX manager update";return false;}
 runtime->busy=true;std::uint32_t dt{};bool delivered=false;
 try{delivered=borrow_application_dt_v93(dt,e)&&runtime->manager->manager_frame(static_cast<std::int32_t>(dt),e);}
 catch(const std::exception& failure){e=failure.what();}
 runtime->busy=false;if(!delivered){runtime->failed=true;runtime->failure=e;}return delivered;
}
bool flush_source_campaign_fx_libraries_v88(const SourceCampaignCandidateBorrowV55& c,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;
 if(!borrow_source_campaign_condition_world_v70(c,world,e)||!require_source_campaign_quiescence_v104(c.actual_world,e))return false;
 auto libraries=world->application->source_fx_libraries_v63();
 if(!libraries||!libraries->belongs_to(world->application)){e="Required SAME actual App FX libraries at Flush";return false;}
 auto runtime=world->fx_runtime_v77;
 if(runtime){
  if(runtime->busy||!runtime->manager||runtime->manager->source_libraries_v63()!=libraries){e="FX library Flush lost SAME quiescent instance backend";return false;}
  return runtime->manager->flush_libraries_v88(e);
 }
 //Actual pre-player C1/Build declarations can precede any instance backend.
 //There are no allocated FX instances until its real constructor publishes.
 //Positive pool members remain a strict error, never silently discarded.
 dh2::fx::VisualFxLibraryReleaseV88 release;release.owner=libraries;
 release.drop_finished=release.animated_d0=release.set_data_d1=[](std::shared_ptr<void>&,std::string& e){e="Positive FX pool member without its actual native instance backend";return false;};
 return libraries->flush_libraries_v88(release,e);
}
bool retire_source_campaign_fx_anchor_v117(const std::shared_ptr<void>& actual_world,std::uintptr_t object,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> world;
 if(!actual_world||!object||!borrow_source_campaign_candidate_runtime_v61(candidate,e)||
    candidate.actual_world!=actual_world||!borrow_source_campaign_condition_world_v70(candidate,world,e))return false;
 auto runtime=world->fx_runtime_v77;
 if(!runtime){e.clear();return true;} //No instance backend exists yet.
 if(runtime->busy||!runtime->manager){e="Cannot retire FX anchor during an active FX delivery";return false;}
 //Source Update clears dead/disabled anchors without resynchronizing. Doing
 //that at native receiver retirement prevents later lookups of deleted actors;
 //the authored effect keeps its current world transform and finishes normally.
 runtime->manager->detach_anchor_v117(object);e.clear();return true;
}
bool release_source_campaign_fx_v77(const SourceCampaignCandidateBorrowV55& candidate,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(candidate,world,e))return false;
 if(world->fx_runtime_v77&&world->fx_runtime_v77->busy){e="Campaign FX release requires quiescent Scene delivery";return false;}
 world->fx_runtime_v77.reset();e.clear();return true;
}
}
