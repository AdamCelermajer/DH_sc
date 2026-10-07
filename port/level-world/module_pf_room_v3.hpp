#pragma once
#include "module_static_scene_v2.hpp"
#include "module_floor_clone_v3.hpp"
#include "module_floor_graph_v3.hpp"
#include "pf_native_storage_lifecycle_v106.hpp"
namespace dh2::world {
class RetainedGameObjectVisualV1;
struct ModulePFDebugV3 {
 std::shared_ptr<void> owner;
 // Whole source Debug Load/CString/query/destroy for this exact key.
 std::function<bool(const char*,bool&,std::string&)> query;
 std::function<bool(std::uint32_t&,std::string&)> clock_ms;
};
struct ModulePFRoomV3 {
 std::string name;unsigned id{};
 std::uint32_t source_room1c{}; // Exact ctor input; UINT_MAX is valid.
 std::uint32_t flags24{}; // PFRoom ctor52159c; Decor solid updates mask 1.
 std::vector<unsigned> floors;
 std::vector<std::shared_ptr<ModuleFloorCloneV3>> clones;
 SceneMapNodeBorrowV2 minimap_prefix_v93,exit_prefix_v93; // Retain real native child on a reached transfer failure.
 octree::Box bounds{}; // Source ctor521540 zeroes all six values.
};
struct ModulePFExitV3 {unsigned direction{};std::array<float,3> position{};};
// Ordered PFWorld::LoadRoom / PFRoom::_LoadFloor continuation. The SAME
// World owns collision and graph storage. No copied triangle/PF authority.
// Calls are sequential; failures retain published room/floor/map prefixes.
class ModulePFRoomsV3 {
 std::shared_ptr<floors::World> world_;
 std::shared_ptr<PFNativeStorageLifecycleV106> native_storage_v106_;
 std::shared_ptr<SceneManagerMapOwnerV2> map_;
 ModulePFDebugV3 debug_;
 std::vector<std::shared_ptr<ModulePFRoomV3>> rooms_;
 std::vector<ModulePFExitV3> exits_;
 octree::Box bounds_{};
 bool flush_attempted_v1_{},flush_complete_v1_{};std::string flush_failure_v1_;
 std::weak_ptr<void> flush_receiver_v1_;std::uintptr_t flush_receiver_identity_v1_{};
 PFWorldFlushPhaseV1 flush_phase_v1_{PFWorldFlushPhaseV1::idle},flush_failed_at_v1_{PFWorldFlushPhaseV1::idle};
 // One source LoadRoom algorithm over the caller's SAME retained graph.
 // These scoped adapters lend actual mesh local fields and native membership;
 // no alternative Scene/C1/floor world or retained callback graph is created.
 bool load_graph_v77(const resources::BresView&,std::shared_ptr<void>,const scene::Scene&,
  const std::function<bool(unsigned)>&,
  const std::function<bool(unsigned,std::array<float,4>&,std::array<float,3>&,std::string&)>&,
  const std::function<bool(unsigned,std::string&)>&,
  const std::function<bool(unsigned,std::string&)>&,
  const std::function<bool(unsigned,SceneMapNodeBorrowV2&,std::string&)>&,
  const std::function<bool(unsigned,std::string&,unsigned&,std::string&)>&,unsigned,const std::string&,
  std::shared_ptr<ModulePFRoomV3>&,std::string&);
public:
 ModulePFRoomsV3(std::shared_ptr<floors::World>,std::shared_ptr<SceneManagerMapOwnerV2>,ModulePFDebugV3);
 bool load(const resources::BresView&,std::shared_ptr<void> actual_resource,
  ModuleStaticSceneV2&,unsigned room,const std::string& name,
  std::shared_ptr<ModulePFRoomV3>& out,std::string&);
 // Existing real generic/scenery visual, same bytes and native graph. Floor
 // clones pin its independent resource block, never its enclosing actor/C1.
 bool load(RetainedGameObjectVisualV1&,unsigned,const std::string&,
  std::shared_ptr<ModulePFRoomV3>&,std::string&);
 bool extend_owner_bounds(const std::shared_ptr<ModulePFRoomV3>&,
  const float* owner_aabb6,bool solid,std::string&);
 // Actual ExtendBoundingBox only; Decor already wrote SAME flags24.
 bool extend_source_bounds_v77(const std::shared_ptr<ModulePFRoomV3>&,const float*,std::string&);
 // After the SAME floor graph's proven post_load, publish retained PFRoom
 // boxes (including Decor extensions) to the collision query projection.
 bool publish_collision_bounds(std::string&);
 bool flush_source_v1(const PFWorldFlushServicesV1&,const std::shared_ptr<void>&,std::uintptr_t,std::string&);
 auto flush_phase_v1()const noexcept{return flush_phase_v1_;}
 bool source_flushing_v1()const noexcept{return flush_attempted_v1_;}
 const auto& native_storage_v106()const noexcept{return native_storage_v106_;}
 const auto& rooms()const noexcept{return rooms_;}
 const auto& exits()const noexcept{return exits_;}
 const octree::Box& bounds()const noexcept{return bounds_;}
 const std::shared_ptr<floors::World>& world()const noexcept{return world_;}
 const std::shared_ptr<SceneManagerMapOwnerV2>& map_owner_v69()const noexcept{return map_;}
};
}
