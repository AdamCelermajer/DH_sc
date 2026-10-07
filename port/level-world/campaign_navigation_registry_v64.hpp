#pragma once
#include "navigation_objects.hpp"
#include <vector>
#include <stdexcept>
namespace dh2::navigation {
// Real fresh native caller storage for the existing source map/obstacle
// kernels. Entries/floor keys are produced only by those kernels; logical
// counts start at zero as an actual fresh map. Capacities are an explicit
// native engineering admission domain, never source actor/map counts.
class CampaignNavigationRegistryV64 final {
 std::vector<ObstacleEntry> entries_;
 std::vector<std::uint32_t> floors_;
 ObstacleRegistry registry_{};
 static std::uint32_t admitted(std::uint32_t count){
  if(!count||count>65536)throw std::invalid_argument("Native campaign obstacle registry capacity outside explicit domain");
  return count;
 }
public:
 explicit CampaignNavigationRegistryV64(std::uint32_t entries=4096,std::uint32_t floors=4096):entries_(admitted(entries)),floors_(admitted(floors)){
  registry_={entries_.data(),0,entries, floors_.data(),0,floors};
 }
 CampaignNavigationRegistryV64(const CampaignNavigationRegistryV64&)=delete;
 CampaignNavigationRegistryV64& operator=(const CampaignNavigationRegistryV64&)=delete;
 ObstacleRegistry& registry()noexcept{return registry_;}
 bool erase_floor_objects_v106(std::string& e){
  if(registry_.entries!=entries_.data()||registry_.floors!=floors_.data()||registry_.count>entries_.size()||registry_.floor_count>floors_.size()){e="Malformed actual PF floor-object registry during clear";return false;}
  registry_.count=0;registry_.floor_count=0;e.clear();return true;
 }
 const ObstacleRegistry& registry()const noexcept{return registry_;}
};
}
