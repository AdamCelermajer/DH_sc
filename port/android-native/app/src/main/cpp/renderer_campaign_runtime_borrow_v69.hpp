#pragma once
#include <actor_runtime.hpp>
#include <floors.hpp>
#include <campaign_navigation_registry_v64.hpp>
#include <canonical_gameobject_base_owner_v1.hpp>
#include <functional>
#include "source_campaign_frame_environment_v76.hpp"
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
// SAME concrete Root environmental owner; no new motion/scratch/floor owners.
using CampaignGenericRuntimeEnvironmentV69=SourceCampaignFrameEnvironmentV76;struct CampaignAuxiliaryRuntimeBorrowV69 {
 std::shared_ptr<void> receiver;std::uintptr_t identity{};
 const std::int32_t* type4{};const std::int32_t* mode3c{};const float* position_c{};
 std::function<bool(std::string&)> update; // actual2e0 virtual0c, not GameObject frame
};
struct CampaignCameraRuntimeBorrowV69 {
 std::shared_ptr<void> receiver;const std::uint8_t* enabled25{};
 std::function<bool(float*,std::string&)> position; // SAME camera root absolute XYZ
 std::function<bool(bool,std::string&)> set_free; // native adapter name; actual EnableDamping41161c
};
struct CampaignGenericRuntimeReceiverServicesV69 {
 std::shared_ptr<void> owner; // independent service authority, weak actual records
 std::function<bool(dh2::world::CanonicalGameObjectBaseOwnerV1&,std::uintptr_t,std::shared_ptr<void>&,const float*&,std::string&)> target_position;
 std::function<bool(dh2::world::CanonicalGameObjectBaseOwnerV1&,std::uintptr_t,CampaignAuxiliaryRuntimeBorrowV69&,std::string&)> auxiliary;
 std::function<bool(CampaignCameraRuntimeBorrowV69&,std::string&)> camera;
};
struct CampaignGenericRuntimeResourcesV69 {
 CampaignGenericRuntimeEnvironmentV69 environment;
 CampaignGenericRuntimeReceiverServicesV69 receiver;
};
}
