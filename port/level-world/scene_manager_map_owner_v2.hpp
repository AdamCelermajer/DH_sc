#pragma once
#include "pf_flush_services_v1.hpp"
#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>
namespace dh2::world {
class GameObjectSceneRootRegistryV1;
struct RetainedMeshNodeV91;
struct SceneMapNodeBorrowV2 {
 std::shared_ptr<void> owner;
 std::shared_ptr<RetainedMeshNodeV91> source_mesh_v93; // SAME authored mesh, not map/root/Visual alias.
 std::uintptr_t identity{};
 std::uintptr_t* parentec{};
 std::uint32_t* flags11c{};
 // Source methods on this SAME live node. Position is read from cached
 // absolute24, not freshly recomputed relative pose.
 std::function<bool(std::array<float,3>&,std::string&)> cached_position;
 std::function<bool(const std::array<float,3>&,std::string&)> set_position;
 std::function<bool(std::string&)> detach,optimize_static;
 std::function<bool(bool,std::string&)> notify_parent_visibility;
 std::function<bool(std::uint32_t,std::string&)> scene_phase_v69;
};
// Native retained SceneManager+28c group. Constructor is source CSceneNode(-1)
// with default TRS, post-update flags721 and empty child/animator lists. AddNodeToMap is
// source3524a0, with source grab/detach/optimize/attach/drop order.
class SceneManagerMapOwnerV2 : public std::enable_shared_from_this<SceneManagerMapOwnerV2> {
 std::weak_ptr<GameObjectSceneRootRegistryV1> scene_manager_;
 std::uintptr_t parent_{};
 std::string name24_v93_; // Actual lazy CSceneNode group name: original C1-empty CString.
 // ISNodeC2 stores60f then calls updateAbsolutePosition(false), producing721.
 std::uint32_t flags_{0x721};
 std::array<float,16> cached_matrix_{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
 std::array<float,6> relative_box_{-1,-1,-1,1,1,1}; // CSceneNode5839d8
 bool local_visible_{true},parent_visible_{true},constructed_{};
 std::vector<SceneMapNodeBorrowV2> children_;
 bool native_d1_attempted_v1_{},native_d1_complete_v1_{};std::string native_d1_failure_v1_;
 bool visibility(bool,std::string&);
 bool source_scene_phase_v69(std::uint32_t,std::string&);
public:
 explicit SceneManagerMapOwnerV2(std::shared_ptr<GameObjectSceneRootRegistryV1>);
 bool add(SceneMapNodeBorrowV2,std::string&);
 bool remove(std::uintptr_t,std::string&);
 bool source_child_detached_v93(std::string&);
 bool release(std::string&);
 bool release_scene_source_v1(const SceneMapDestructionV1&,std::string&);
 bool native_d1_prefix_v106(std::string&);
 bool native_d1_tail_v106(std::string&);
 bool native_destroyed_v1()const noexcept{return native_d1_complete_v1_;}
 std::uintptr_t scene_manager_identity_v1()const noexcept;
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 std::vector<std::uintptr_t> children()const;
 const std::vector<SceneMapNodeBorrowV2>& source_children_v93()const noexcept{return children_;}
 const std::string& source_name_v93()const noexcept{return name24_v93_;}
 std::uint32_t flags()const noexcept{return flags_;}
 const std::array<float,6>& constructor_box()const noexcept{return relative_box_;}
 // Cache-only source virtual38. The parent assignment marks40 but does not
 // recompute this field; child OptimizeStatic observes this retained cache.
 const std::array<float,16>& cached_matrix()const noexcept{return cached_matrix_;}
 bool visible()const noexcept{return local_visible_&&parent_visible_;}
};
}
