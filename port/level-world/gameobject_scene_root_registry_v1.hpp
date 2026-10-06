#pragma once
#include "retained_gameobject_visual_v1.hpp"
#include <vector>
namespace dh2::world {
// SAME source manager state, separately typed from renderer draw-list owner.
// Full render registration is not implemented by root attachment.
struct GameObjectSceneManagerFieldsV1 {
 std::uint8_t hierarchy_dirty288{},render_dirty289{},force448{};
 std::uint32_t counter440{},cadence444{4};
};
struct GameObjectSceneRootBorrowV1 {
 std::shared_ptr<void> owner;
 std::uintptr_t identity{};
 std::uint32_t* flags11c{};
 std::uintptr_t* parentec{};
 // The real node's complete visibility propagation and animator remove list.
 std::function<bool(bool,std::string&)> notify_visibility;
 std::function<bool(std::string&)> remove_animators;
 // Additive specialized source setSceneManager110/onChangedSceneManager.
 // Ordinary historical node domains retain their existing empty leaf path.
 std::function<bool(std::uintptr_t,std::string&)> scene_manager_changed;
};
// Actual CSceneManager active-camera e4. Camera lease and reference methods
// belong to the same retained node, including its source deleting tail.
struct GameObjectSceneCameraBorrowV13 {
 std::shared_ptr<void> owner;
 std::uintptr_t identity{};
 std::function<bool(std::string&)> grab,drop;
};
// Native owner of actual SceneManager-root child membership. Entries borrow
// same visual-root fields and lease; no pose/graph/animation copy is created.
class GameObjectSceneRootRegistryV1 {
 struct Entry {GameObjectSceneRootBorrowV1 root;bool animators_removed{};};
 std::uintptr_t identity_;std::uint32_t root_flags_{0x60f};
 GameObjectSceneManagerFieldsV1 fields_;
 std::vector<Entry> children_;
 GameObjectSceneCameraBorrowV13 active_camera_e4_;
public:
 GameObjectSceneRootRegistryV1():identity_(reinterpret_cast<std::uintptr_t>(this)){}
 explicit GameObjectSceneRootRegistryV1(std::uintptr_t source_root_identity):identity_(source_root_identity){}
 bool add_child(GameObjectSceneRootBorrowV1,std::string&);
 bool release_visual_root(std::uintptr_t,std::string&);
 void force_register()noexcept{fields_.force448=1;fields_.render_dirty289=1;}
 void notify_hierarchy_changed()noexcept{fields_.hierarchy_dirty288=1;fields_.render_dirty289=1;}
 // Whole CSceneManager::notifyVisibilityChanged5890a8 writes only byte289.
 void notify_visibility_changed_v3()noexcept{fields_.render_dirty289=1;}
 const GameObjectSceneManagerFieldsV1& fields()const noexcept{return fields_;}
 std::vector<std::uintptr_t> roots()const;
 std::uintptr_t identity()const noexcept{return identity_;}
 std::uintptr_t scene_manager_identity_v16()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 bool set_active_camera_v13(GameObjectSceneCameraBorrowV13,std::string&);
 const GameObjectSceneCameraBorrowV13& active_camera_v13()const noexcept{return active_camera_e4_;}
};
}
