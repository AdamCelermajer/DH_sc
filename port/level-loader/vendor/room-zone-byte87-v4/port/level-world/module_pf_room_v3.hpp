#pragma once
#include "module_static_scene_v2.hpp"
#include "module_floor_clone_v3.hpp"
#include "module_floor_graph_v3.hpp"
namespace dh2::world {
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
 octree::Box bounds{}; // Source ctor521540 zeroes all six values.
};
struct ModulePFExitV3 {unsigned direction{};std::array<float,3> position{};};
// Ordered PFWorld::LoadRoom / PFRoom::_LoadFloor continuation. The SAME
// World owns collision and graph storage. No copied triangle/PF authority.
// Calls are sequential; failures retain published room/floor/map prefixes.
class ModulePFRoomsV3 {
 std::shared_ptr<floors::World> world_;
 std::shared_ptr<SceneManagerMapOwnerV2> map_;
 ModulePFDebugV3 debug_;
 std::vector<std::shared_ptr<ModulePFRoomV3>> rooms_;
 std::vector<ModulePFExitV3> exits_;
 octree::Box bounds_{};
public:
 ModulePFRoomsV3(std::shared_ptr<floors::World>,std::shared_ptr<SceneManagerMapOwnerV2>,ModulePFDebugV3);
 bool load(const resources::BresView&,std::shared_ptr<void> actual_resource,
  ModuleStaticSceneV2&,unsigned room,const std::string& name,
  std::shared_ptr<ModulePFRoomV3>& out,std::string&);
 bool extend_owner_bounds(const std::shared_ptr<ModulePFRoomV3>&,
  const float* owner_aabb6,bool solid,std::string&);
 // After the SAME floor graph's proven post_load, publish retained PFRoom
 // boxes (including Decor extensions) to the collision query projection.
 bool publish_collision_bounds(std::string&);
 const auto& rooms()const noexcept{return rooms_;}
 const auto& exits()const noexcept{return exits_;}
 const octree::Box& bounds()const noexcept{return bounds_;}
 const std::shared_ptr<floors::World>& world()const noexcept{return world_;}
};
}
