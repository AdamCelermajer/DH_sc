#pragma once
#include "game_object_initialization_owner_v1.hpp"
#include "game_object_set_position_v2.hpp"
#include "retained_scene_visual_connection_v3.hpp"
#include <map>
namespace dh2::world {
// A scoped borrow of an already constructed canonical class. The callback
// must pin that exact receiver; this graph constructs no GameObject or World.
using CanonicalBaseBorrowV68=std::function<bool(
 std::shared_ptr<void>&,CanonicalGameObjectBaseOwnerV1*&,std::string&)>;
struct CanonicalGameObjectGraphServicesV68 {
 std::shared_ptr<void> owner; // independent native cache/service lifetime
 std::shared_ptr<GameObjectSceneRootRegistryV1> roots;
 RetainedGameObjectVisualServicesV1 visual;
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,bool&,std::string&)> is_animated;
 std::function<bool(std::string&)> current;
};
// Shared resource implementation for scenery, animated scenery, triggers and
// destructible objects. All field writes stay on the actual class base. The
// caller supplies original condition/spawn/PF/visibility/light-name services;
// only actual visual construction and scene/position bridges are added here.
class CanonicalGameObjectGraphV68 final:
 public std::enable_shared_from_this<CanonicalGameObjectGraphV68> {
 struct Entry {
  std::uintptr_t identity{};
  CanonicalBaseBorrowV68 borrow;
  GameObjectInitializationServicesV1 platform;
  GameObjectSetPositionServicesV2 position;
  std::shared_ptr<RetainedSceneVisualConnectionV3> visual;
  std::function<bool(std::shared_ptr<RetainedGameObjectVisualV1>&,std::string&)> external_visual;
  std::unique_ptr<GameObjectVisualAssetOwnerV1> assets;
  bool releasing{},base_only_v78{};
 };
 CanonicalGameObjectGraphServicesV68 services_;
 std::map<std::uintptr_t,std::shared_ptr<Entry>> entries_;
 bool borrow(const std::shared_ptr<Entry>&,std::shared_ptr<void>&,
  CanonicalGameObjectBaseOwnerV1*&,std::string&)const;
 bool visual(const std::shared_ptr<Entry>&,std::uintptr_t,
  std::shared_ptr<RetainedGameObjectVisualV1>&,std::string&)const;
public:
 explicit CanonicalGameObjectGraphV68(CanonicalGameObjectGraphServicesV68);
 // Admit SAME constructed base before script Init can borrow waypoints;
 // no visual C1, initialization body or property/default execution occurs.
 bool observe_base_v78(std::uintptr_t,CanonicalBaseBorrowV68,std::string&);
 bool bind(std::uintptr_t,CanonicalBaseBorrowV68,
  GameObjectInitializationServicesV1&,GameObjectSetPositionServicesV2&,std::string&);
 // Containers already own their complete typed visual/animation/body graph.
 // Observe that exact graph for scene submission, without another Visual C1.
 bool observe_visual(std::uintptr_t,CanonicalBaseBorrowV68,
  std::function<bool(std::shared_ptr<RetainedGameObjectVisualV1>&,std::string&)>,std::string&);
 bool retire_observed_visual(std::uintptr_t,std::string&);
 bool capture_visuals(std::vector<std::shared_ptr<RetainedGameObjectVisualV1>>&,std::string&)const;
 bool borrow_visual(std::uintptr_t,std::shared_ptr<RetainedGameObjectVisualV1>&,std::string&)const;
 bool borrow_base_v77(std::uintptr_t,std::shared_ptr<void>&,CanonicalGameObjectBaseOwnerV1*&,std::string&)const;
 // Qualified resource teardown before actual class/base destruction. Does not
 // claim conditions/scripts/network/base D0 or manager unpublication occurred.
 bool release_resources(std::uintptr_t,std::string&);
};
}
