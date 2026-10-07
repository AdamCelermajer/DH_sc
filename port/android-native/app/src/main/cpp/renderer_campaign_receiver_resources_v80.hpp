#pragma once
#include "renderer_campaign_runtime_borrow_v69.hpp"
#include "source_campaign_anchor_v75.hpp"
#include <camera_anchor_owner_v75.hpp>
#include <gameplay_camera_application_v23.hpp>
namespace model_renderer {
// Bind read-only existing native resource producers, retaining only independent
// authority and weak actual campaign/receiver closures. No new context/scene.
template<class Self>
bool bind_campaign_runtime_receivers_v80(const std::weak_ptr<Self>& weak,CampaignGenericRuntimeReceiverServicesV69& output,std::string& e){
 auto self=weak.lock();if(!self||!self->current(e))return false;
 if(!output.owner)output.owner=self->files;
 if(!output.target_position)output.target_position=[weak](auto& base,std::uintptr_t node,auto& pin,const float*& position,std::string& e){
  auto self=weak.lock();std::shared_ptr<void> receiver;dh2::world::CanonicalGameObjectBaseOwnerV1* actual{};
  if(!self||!self->borrow(base.identity(),receiver,actual,e)||actual!=&base)return false;
  auto target=base.pointer(0x180);auto visual_id=base.pointer(0x2d8);
  if(!target||*target!=node||!visual_id||!*visual_id){e="Required SAME published target180/Visual2d8";return false;}
  std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;const float* cache{};
  if(!self->actual_visual(*visual_id,visual,e)||!visual->borrow_node_position_v80(node,cache,e))return false;
  // No callback occurs between alias delivery and the actual target-phase copy.
  // The actual visual/node stays pinned during that read; no Map-child lease.
  pin=visual;position=cache;e.clear();return true;
 };
 if(!output.auxiliary)output.auxiliary=[weak](auto& base,std::uintptr_t id,CampaignAuxiliaryRuntimeBorrowV69& out,std::string& e){
  auto self=weak.lock();std::shared_ptr<void> receiver;dh2::world::CanonicalGameObjectBaseOwnerV1* actual{};
  if(!self||!self->borrow(base.identity(),receiver,actual,e)||actual!=&base)return false;
  const auto slot=base.pointer(0x2e0);auto world=self->scope.actual_world.lock();
  if(!slot||*slot!=id||!world){e="Required SAME published auxiliary2e0";return false;}
  std::shared_ptr<dh2::camera::CameraAnchorOwnerV75> anchor;
  if(!borrow_source_campaign_anchor_v80(world,id,anchor,e))return false;
  CampaignAuxiliaryRuntimeBorrowV69 result;const auto& fields=anchor->fields();result.receiver=anchor;result.identity=anchor->identity();result.type4=&fields.type4;result.mode3c=&fields.state3c;result.position_c=fields.position_c.data();
  result.update=[weak,id,actual=std::weak_ptr<dh2::camera::CameraAnchorOwnerV75>(anchor)](std::string& e){auto self=weak.lock();auto anchor=actual.lock();auto world=self?self->scope.actual_world.lock():nullptr;std::shared_ptr<dh2::camera::CameraAnchorOwnerV75> current;
   if(!self||!self->current(e)||!world||!anchor||!borrow_source_campaign_anchor_v80(world,id,current,e)||current.get()!=anchor.get()||current.owner_before(anchor)||anchor.owner_before(current)){if(e.empty())e="Released/replaced SAME actual auxiliary owner";return false;}return source_campaign_anchor_update_v75(world,id,e);};
  out=std::move(result);e.clear();return true;
 };
 if(!output.camera)output.camera=[weak](CampaignCameraRuntimeBorrowV69& out,std::string& e){
  auto self=weak.lock();SourceCampaignCandidateBorrowV55 actual;if(!self||!self->current(e)||!borrow_source_campaign_candidate_v55(actual,e)||!actual.camera_application){if(e.empty())e="Required SAME current camera Application owner";return false;}
  auto session=actual.camera_application->world();auto runtime=session&&session->camera?session->camera->level():nullptr;
  if(!runtime||!runtime->loaded()||!runtime->level()||!runtime->scene()){e="Required actual loaded Level128 CameraTarget/scene";return false;}
  CampaignCameraRuntimeBorrowV69 result;result.receiver=runtime;result.enabled25=&runtime->level()->target().fields().damping.enabled;
  result.position=[weak,expected=std::weak_ptr<dh2::camera::GameplayCameraRuntimeV11>(runtime)](float* xyz,std::string& e){auto self=weak.lock();auto camera=expected.lock();SourceCampaignCandidateBorrowV55 actual;
   if(!xyz||!self||!self->current(e)||!camera||!camera->loaded()||!borrow_source_campaign_candidate_v55(actual,e)||!actual.camera_application)return false;
   auto session=actual.camera_application->world();auto current=session&&session->camera?session->camera->level():nullptr;
   if(!current||current.get()!=camera.get()||current.owner_before(camera)||camera.owner_before(current)||!camera->scene()){e="Released/replaced SAME camera root position owner";return false;}return camera->scene()->root_position(xyz,e);};
  result.set_free=[weak,expected=std::weak_ptr<dh2::camera::GameplayCameraRuntimeV11>(runtime)](bool enabled,std::string& e){
   auto self=weak.lock();auto camera=expected.lock();SourceCampaignCandidateBorrowV55 actual;
   if(!self||!self->current(e)||!camera||!camera->loaded()||!borrow_source_campaign_candidate_v55(actual,e)||!actual.camera_application)return false;
   auto session=actual.camera_application->world();auto current=session&&session->camera?session->camera->level():nullptr;
   if(!current||current.get()!=camera.get()||current.owner_before(camera)||camera.owner_before(current)||!camera->level()){e="Released/replaced SAME CameraTarget damping owner";return false;}
   return camera->level()->target().enable_damping(enabled,e);
  };
  out=std::move(result);e.clear();return true;
 };
 e.clear();return true;
}
}
