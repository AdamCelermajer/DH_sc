#pragma once
#include "module_static_scene_bounds_v3.hpp"
#include "module_visual_mesh_box_v2.hpp"
#include "game_object_relative_box_v3.hpp"
#include "gameobject_scene_root_registry_v1.hpp"
namespace dh2::world {
struct RetainedModuleVisualServicesV3 {
 std::shared_ptr<void> owner;
 std::shared_ptr<GameObjectSceneRootRegistryV1> roots;
 std::function<bool(std::int32_t&,std::string&)> driver_type;
 // _FindModularSkinnedMeshNode4718f0 searches the SAME SceneManager, not
 // merely this asset. Deliver source SearchByType order; last receiver wins.
 std::function<bool(std::vector<std::uintptr_t>&,std::string&)> modular_meshes;
 std::function<bool(std::string&)> update_pf;
 //Native exact-root GPU alias retirement at actual source D0, not Scene clear.
 std::function<bool(std::uintptr_t,std::string&)> retire_root_gpu_v107;
 // SAME original RootSceneNode::updateAbsolutePosition global diagnostic
 // counter increment at35c290. Not SceneManager's render cadence counter.
 std::uint32_t* root_update_counter{};
};
// Bounded whole VisualObjectC1 static/no-colbox successor for the actual SWAMP
// Module BRES. Unsupported resources reject before claiming this domain.
// The caller owns this receiver by shared_ptr and explicitly releases root
// membership before candidate teardown; root registry borrows that same lease.
class RetainedModuleVisualV3 : public std::enable_shared_from_this<RetainedModuleVisualV3> {
 CanonicalGameObjectBaseOwnerV1& base_;
 RetainedModuleVisualServicesV3 services_;
 std::shared_ptr<const std::vector<std::uint8_t>> bytes_;
 std::shared_ptr<void> resource_lease_;
 resources::BresView bres_{};
 // Visual root8 owns its initial factory lease. SceneManager membership and
 // controller38 acquire leases of this SAME root allocation, never the visual.
 std::shared_ptr<void> root_;
 std::unique_ptr<VisualAnimControllerOwnerV4> controller38_;
 ModuleStaticSceneV2& root_scene()const{return *static_cast<ModuleStaticSceneV2*>(root_.get());}
 std::array<float,6> mesh_box_{};
 std::uintptr_t modular_mesh_{};
 std::uint8_t modular_found_{};
 std::uint8_t colbox28_{}; // VisualObjectC1 ctor472ac0; source no-colbox keeps0
 std::int32_t light_set40_{};
 bool root_present_{},ready_{};
 bool missing(const char*,std::string&)const;
 bool calc_mesh_box(std::string&);
public:
 RetainedModuleVisualV3(CanonicalGameObjectBaseOwnerV1&,RetainedModuleVisualServicesV3);
 bool initialize(std::shared_ptr<const std::vector<std::uint8_t>>,const char* xref,bool& found,std::string&);
 bool source_asset_miss(std::string&);
 bool apply_mesh_box(std::string&);
 bool sync(std::string&);
 bool set_root_local_visibility_v94(bool,std::string&);
 bool sync_visibility_v94(std::string&);
 bool release(std::string&);
 bool root_bounds(std::array<float,6>&,std::string&);
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 bool physical()const noexcept{return colbox28_!=0;}
 bool set_root_game_object(std::uintptr_t,std::string&);
 void set_light_set(std::int32_t id)noexcept{light_set40_=id;}
 bool node_from_name(const char*,std::uintptr_t&,std::string&)const;
 bool borrow_scene_node_v109(std::uintptr_t,RetainedSceneNodeBorrowV109&,std::string&);
 std::uintptr_t root_identity()const noexcept{return root_present_?reinterpret_cast<std::uintptr_t>(root_.get()):0;}
 std::uintptr_t controller_identity()const noexcept{return reinterpret_cast<std::uintptr_t>(controller38_.get());}
 std::uintptr_t controller_root_identity()const noexcept{return controller38_?controller38_->root_identity():0;}
 bool ready()const noexcept{return ready_;}
 ModuleStaticSceneV2& scene()noexcept{return root_scene();}
 const resources::BresView& bres()const noexcept{return bres_;}
 std::shared_ptr<void> resource_lease()const{return resource_lease_;}
 const std::array<float,6>& mesh_box()const noexcept{return mesh_box_;}
};
}
