#pragma once
#include "gameplay_camera_animator_v10.hpp"
#include "gameobject_scene_root_registry_v1.hpp"
namespace dh2::camera {
// Native source CameraSceneNode binding, distinct from immutable SCamera raw
// metadata. Pose/target remain the SAME V3 graph; no duplicate transform array.
class CameraNodeLifetimeV15 {
 std::shared_ptr<GameplayCameraSceneV3> scene_;
 std::uint32_t index_{},references_{1};bool alive_{true};
 std::uint32_t flags11c_{0x60f};std::uint8_t local120_{1},parent121_{1};
 std::uintptr_t manager110_{};
public:
 CameraNodeLifetimeV15(std::shared_ptr<GameplayCameraSceneV3> s,std::uint32_t i):scene_(std::move(s)),index_(i){}
 bool grab(std::string&);
 bool drop(std::string&);
 bool camera_index(std::uint32_t&,std::string&)const;
 void notify_parent_visibility(bool)noexcept;
 std::uint32_t flags()const noexcept{return flags11c_;}
 std::uintptr_t& source_manager110()noexcept{return manager110_;}
 bool alive()const noexcept{return alive_;}
 std::uint32_t source_references()const noexcept{return references_;}
};
class GameplayCameraSceneMembershipV15:public std::enable_shared_from_this<GameplayCameraSceneMembershipV15> {
 struct Visibility {std::uint32_t flags11c{0x60f};std::uint8_t local120{1},parent121{1};};
 std::weak_ptr<world::GameObjectSceneRootRegistryV1> manager_;
 std::shared_ptr<GameplayCameraSceneV3> scene_;
 Visibility root_;
 std::uintptr_t parentec_{};
 std::uintptr_t manager110_{};
 std::function<bool(std::uint32_t,std::uintptr_t,std::string&)> camera_manager_changed_;
 std::vector<Visibility> nodes_;
 std::vector<std::shared_ptr<CameraNodeLifetimeV15>> cameras_;
 std::vector<std::shared_ptr<GameplayCameraAnimatorV10>> animators_;
 bool add_attempted_{},registered_{},parents_released_{};
 void notify_node(std::size_t,bool);
 bool changed_manager(std::uintptr_t,std::string&);
public:
 GameplayCameraSceneMembershipV15(std::shared_ptr<GameplayCameraSceneV3>,std::shared_ptr<world::GameObjectSceneRootRegistryV1>,std::function<bool(std::uint32_t,std::uintptr_t,std::string&)>);
 bool attach_graph(std::string&);
 bool attach_animator(std::shared_ptr<GameplayCameraAnimatorV10>,std::string&);
 bool notify_visibility(bool,std::string&);
 bool remove_animators(std::string&);
 bool source_scene_phase_v69(std::uint32_t,std::string&);
 bool detach_visual(std::string&);
 // Original root destruction releases its actual camera-child references;
 // invoke after Base root reference drops, before Base camera reference drop.
 bool release_parent_camera_references(std::string&);
 bool retain_camera(std::uint32_t,std::shared_ptr<CameraNodeLifetimeV15>&,std::string&);
 world::GameObjectSceneCameraBorrowV13 scene_camera_borrow(std::shared_ptr<CameraNodeLifetimeV15>);
 std::uintptr_t root_identity()const noexcept{return reinterpret_cast<std::uintptr_t>(&root_);}
 std::uintptr_t parent_identity()const noexcept{return parentec_;}
 const std::shared_ptr<GameplayCameraSceneV3>& scene()const noexcept{return scene_;}
 std::size_t animator_count()const noexcept{return animators_.size();}
 const std::vector<std::uint32_t> visibility_flags()const;
};
}
