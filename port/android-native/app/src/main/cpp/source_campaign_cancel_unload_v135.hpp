#pragma once
#include "source_campaign_admission_v104.hpp"
#include <lifecycle_v36.hpp>
#include <utility>
namespace model_renderer {
template<class World>
bool source_campaign_cancel_admission_v135(const std::shared_ptr<World>& world,bool& pending,std::string& e){
 if(!world||!world->admission_v104){e="Required SAME retained campaign admission for cancellation";return false;}
 if(!world->admission_v104->close(pending,e))return false;
 return pending||world->admission_v104->require_closed_quiescent(e);
}
// The source loader owns this callback, so its containing World/Level must
// remain weak. Close actual delivery admission before invoking the existing
// unload journal; pending scopes and failed journals retain all loader pins.
template<class World,class Level,class Unload>
std::function<dh2::loader::LifecycleStepV36(std::string&)> source_campaign_cancel_unload_v135(
 std::weak_ptr<World> world,std::weak_ptr<Level> level,Unload unload){
 return [world=std::move(world),level=std::move(level),unload=std::move(unload)](std::string& e){
  using Step=dh2::loader::LifecycleStepV36;
  auto actual_world=world.lock();auto actual_level=level.lock();
  if(!actual_world||!actual_level){
   e="Required SAME retained campaign/Level admission for cancellation";return Step::failed;
  }
  bool pending{};
  if(!source_campaign_cancel_admission_v135(actual_world,pending,e))return Step::failed;
  if(pending){e.clear();return Step::pending;}
  if(!unload(actual_world,actual_level,e))return Step::failed;
  e.clear();return Step::complete;
 };
}
}
