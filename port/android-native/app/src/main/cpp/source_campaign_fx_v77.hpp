#pragma once
#include <character_mesh_fx_owner_v4.hpp>
#include <functional>
#include <memory>
namespace dh2::world {class CanonicalGameObjectBaseOwnerV1;}
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
class SourceCampaignFxRuntimeV77;
struct SourceCampaignFxRenderServicesV77 {
 // Native render camera transport. It samples the actual submitted camera;
 // it must not advance camera state, substitute an identity or tick a scene.
 std::function<bool(float*,float*,std::string&)> camera;
};
bool publish_source_campaign_fx_v77(const std::shared_ptr<void>& actual_world,
 std::uintptr_t actual_character,SourceCampaignFxRenderServicesV77,std::string&);
bool borrow_source_campaign_fx_v77(const std::shared_ptr<void>& actual_world,
 std::shared_ptr<void>&,dh2::fx::CharacterMeshFxOwnerV4*&,std::string&);
bool source_campaign_fx_scene_v77(const SourceCampaignCandidateBorrowV55&,
 std::uint64_t actual_epoch,std::uint32_t actual_time,std::string&);
bool update_source_campaign_fx_v77(const SourceCampaignCandidateBorrowV55&,std::string&);
bool flush_source_campaign_fx_libraries_v88(const SourceCampaignCandidateBorrowV55&,std::string&);
bool release_source_campaign_fx_v77(const SourceCampaignCandidateBorrowV55&,std::string&);
 bool retire_source_campaign_fx_anchor_v117(const std::shared_ptr<void>& actual_world,
  std::uintptr_t actual_object,std::string&);
bool borrow_source_campaign_object_base_v77(const SourceCampaignCandidateBorrowV55&,
 std::uintptr_t,std::shared_ptr<void>&,dh2::world::CanonicalGameObjectBaseOwnerV1*&,std::string&);
bool borrow_source_campaign_fx_camera_v77(const std::shared_ptr<void>& actual_world,
 float view16[16],float position3[3],std::string&);
}
