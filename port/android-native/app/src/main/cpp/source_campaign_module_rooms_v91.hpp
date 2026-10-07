#pragma once
#include "model_renderer.hpp"
#include <canonical_module_graph_v3.hpp>
namespace model_renderer {
bool bind_source_campaign_module_room_zones_v91(const SourceCampaignCandidateBorrowV55&,
 decltype(dh2::world::CanonicalModuleGraphServicesV3::spawn_zone),
 decltype(dh2::world::CanonicalModuleGraphServicesV3::zone_init),std::string&);
bool borrow_source_campaign_module_v91(const std::shared_ptr<void>& actual_world,std::uintptr_t,
 std::shared_ptr<void>&,dh2::world::CanonicalModuleV1*&,std::string&);
bool borrow_source_campaign_module_visited_v91(const std::shared_ptr<void>& actual_world,std::uintptr_t,
 std::shared_ptr<void>&,std::uint8_t*&,std::string&);
bool source_campaign_module_sync_visibility_v94(const std::shared_ptr<void>& actual_world,
 std::uintptr_t actual_module_identity,std::string&);

}
