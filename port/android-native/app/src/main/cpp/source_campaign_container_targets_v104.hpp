#pragma once
#include <canonical_container_target_v104.hpp>
#include <canonical_openable_graph_v21.hpp>
#include <canonical_destructible_container_v16.hpp>
namespace model_renderer {
bool bind_source_openable_interaction_v116(const std::shared_ptr<void>&,
 std::function<bool(std::shared_ptr<void>&,dh2::world::CanonicalGameObjectBaseOwnerV1*&,std::string&)>,
 dh2::world::OpenableContainerInteractionServicesV2&,std::string&);
bool bind_source_container_drop_v104(const std::shared_ptr<void>& actual_world,std::uintptr_t container,
 std::function<bool(std::int32_t,std::uintptr_t,std::int32_t,bool,std::string&)>& actual_leaf,std::string&);
struct SourceCampaignCandidateBorrowV55;
class SourceCampaignContainerTargetsV104;
//Called by the genuine source Container InitPost suffix. Services weakly
//borrow its already constructed class/graph; no C1, Add or source defaults.
bool enroll_source_campaign_container_target_v104(const SourceCampaignCandidateBorrowV55&,
 dh2::world::CanonicalContainerTargetServicesV104,std::string&);
//Called by actual class journal/factory only AFTER canonical erase.
bool retire_source_campaign_container_target_v104(const std::shared_ptr<void>&,
 std::uintptr_t actual_container,std::string&);
bool release_source_campaign_container_targets_v104(const SourceCampaignCandidateBorrowV55&,std::string&);
bool source_campaign_item_container_interact_v114(const std::shared_ptr<void>& actual_world,
 std::uintptr_t target,std::uintptr_t character,bool& handled,std::string&);
bool bind_source_openable_target_suffix_v104(const std::shared_ptr<void>& actual_world,
 std::shared_ptr<void> provider,const std::shared_ptr<dh2::world::CanonicalOpenableGraphV21>&,
 dh2::world::CanonicalClassReceiverV1&,std::string&);
bool bind_source_destructible_target_suffix_v104(const std::shared_ptr<void>& actual_world,
 std::shared_ptr<void> provider,const std::shared_ptr<dh2::world::CanonicalDestructibleContainerV16>&,
 dh2::world::CanonicalClassReceiverV1&,std::string&);
}
