#pragma once
#include "navigation_controller.hpp"
#include <atomic>
#include <memory>
#include <mutex>
#include <vector>
#include <string>
namespace dh2::navigation {
// Native scratch for the existing UpdatePath kernels. No actor, path, floor
// registry or clock is stored here. A live lease prevents a nested delivery
// from reallocating the scratch still borrowed by its caller.
class CampaignFrameScratchV76 {
 struct Entry {
  std::atomic<bool> busy{false};
  std::vector<PathSegment> segments;
  std::vector<AvoidanceActor> actors;
  std::vector<std::uint32_t> floors;
  ControllerWorkspace workspace{};
 };
 struct Lease {
  std::shared_ptr<Entry> entry;
  explicit Lease(std::shared_ptr<Entry> same):entry(std::move(same)){}
  ~Lease(){entry->busy.store(false,std::memory_order_release);}
 };
 std::mutex mutex_;
 std::vector<std::shared_ptr<Entry>> entries_;
public:
 CampaignFrameScratchV76()=default;
 bool borrow(std::uint32_t path_segments,std::uint32_t avoidance_actors,
  std::uint32_t registry_floor_capacity,std::shared_ptr<void>&,
  ControllerWorkspace*&,std::string&);
};
}
