#pragma once
#include "asset_catalog.hpp"
#include "source_module_trace.hpp"
#include "../level-world/canonical_level_module_bindings_v2.hpp"
#include "../level-world/module_selected_scene_v2.hpp"
#include "../level-world/floors.hpp"
namespace dh::foundation {
enum class SourceModulePosePhase { unavailable, constructor_degrees, initialized_radians };
struct SourceModuleFloorRequest {
 SourceModuleTraceEntry trace;
 std::shared_ptr<dh2::world::CanonicalModuleRecordV2> record;
 SourceModulePosePhase pose_phase=SourceModulePosePhase::unavailable;
 // Explicit upstream admission; this component does not invent campaign gates.
 bool admitted=false;
};
struct SourceModuleFloorMesh {
 unsigned instance{},floor{};
 std::string source_name;
 std::array<float,4> mesh_local_quaternion{0,0,0,1};
 std::array<float,3> mesh_local_scale{1,1,1};
 std::array<float,16> source_cached_world{};
};
struct SourceModuleFloorRoom {
 unsigned id{};std::int32_t source_room1c=-1,module_id=-1;
 std::string occurrence,name;
 std::shared_ptr<dh2::world::CanonicalModuleRecordV2> module;
 std::vector<std::uint8_t> bytes;
 dh2::resources::BresView bres{};
 dh2::world::ModuleSelectedSceneV2 factory;
 std::vector<SourceModuleFloorMesh> meshes;
 std::vector<unsigned> floors;
 bool completed=false;std::string failure;
};
// Actual in-house PFRoom constructor registry. IDs are assigned by this real
// registry's publication, independently of Module40c and GameObject.room64.
// Owns selected original factory/mesh cache and exact source floor records.
class SourceModuleFloors {
public:
 explicit SourceModuleFloors(std::shared_ptr<dh2::floors::World>);
 bool load(const AssetCatalog&,const SourceModuleFloorRequest&,
           std::shared_ptr<SourceModuleFloorRoom>&,std::string&);
 bool post_load(std::string&);
 const std::vector<std::shared_ptr<SourceModuleFloorRoom>>& rooms()const{return rooms_;}
 std::shared_ptr<dh2::floors::World> world()const{return world_;}
private:
 std::shared_ptr<dh2::floors::World> world_;
 std::vector<std::shared_ptr<SourceModuleFloorRoom>> rooms_;
 bool failed_=false;
};
}
