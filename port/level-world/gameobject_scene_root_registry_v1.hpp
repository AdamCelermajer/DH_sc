#pragma once
#include "../engine-skinning/visual_skin_owner_v6.hpp"
#include "retained_gameobject_visual_v1.hpp"
#include "scene_manager_frame_fields_v67.hpp"
#include "light_set_name_owner_v3.hpp"
#include <vector>
#include <list>
namespace dh2::world {
class ScenePreloadCollectionV81;
class NativeLightSetV113;
struct NativeLightV113;
// SAME source manager state, separately typed from renderer draw-list owner.
// Full render registration is not implemented by root attachment.
struct GameObjectSceneManagerFieldsV1 {
 std::uint8_t hierarchy_dirty288{1},render_dirty289{1},force448{};
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
 // Source animator/absolute-cache traversal on this SAME registered root.
 // Static roots validate their actual empty animator domain; absence of a
 // provider is not treated as an empty source list.
 std::function<bool(std::uint32_t,std::string&)> scene_phase_v69;
 std::function<bool(std::vector<skinning::SourceModularSkinBorrowV114>&,std::string&)> modular_receivers_v114;
 //Typed native CLight subtype lookup. It reads the actual node NAME, not
 //the SLight library ID or GameObject NAME. Other native root families have
 //no CLight children unless their constructor provides this domain.
 std::function<bool(const std::string&,std::shared_ptr<NativeLightV113>&,std::string&)> light_by_name_v113;
 //Actual intrusive parent grab/drop, independent of host diagnostic leases.
 //Paired optional hooks: false means no reference-count mutation. A last
 //drop may preserve its real pending native D1 and be retried by that owner.
 std::function<bool(std::string&)> acquire_parent_reference_v110,release_parent_reference_v110;
};
struct SceneRegistrationTransportV69 {
 std::shared_ptr<void> provider;
 // Full source clear/register or cached-node virtual0 successor. Native GPU
 // adapter operates primitive keys, not emulated ARM node pointers.
 std::function<bool(std::string&)> clear_render_lists,register_nodes,refresh_cached_nodes;
};
struct SceneNodeUpdateBorrowV102 {
 std::shared_ptr<void> owner;
 std::uintptr_t identity{};
 std::function<bool(std::uint32_t,std::string&)> on_update18;
};
struct SceneUpdateTransportV102 {
 std::shared_ptr<void> provider;
 std::function<bool(std::uint32_t&,std::string&)> timer;
 // The optimized original branch needs all actual descendant nodes, not
 // merely one root callback per model. Ordinary root traversal is separate.
 std::function<bool(std::vector<SceneNodeUpdateBorrowV102>&,std::string&)> collect_nodes;
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
 struct Entry {GameObjectSceneRootBorrowV1 root;bool animators_removed{},parent_reference_owned_v110{};};
 struct ParentReferencePrefixV110 {GameObjectSceneRootBorrowV1 root;bool acquired{},released{},unproved{},busy{};std::string failure;};
 std::list<std::shared_ptr<ParentReferencePrefixV110>> parent_prefixes_v110_;
 bool release_parent_prefix_v110(const std::shared_ptr<ParentReferencePrefixV110>&,std::string&);
 bool drain_parent_prefixes_v110(std::uintptr_t only_identity,std::string&);
 bool unlink_parent_v110(std::uintptr_t,std::string&,bool notify_hierarchy=true);
 std::uintptr_t identity_;std::uint32_t root_flags_{0x60f};
 GameObjectSceneManagerFieldsV1 fields_;
 SceneManagerFrameFieldsV67 frame_v67_;
 std::shared_ptr<LightSetNameOwnerV3> source_light_names_v89_;
 std::shared_ptr<NativeLightSetV113> source_light_runtime_v113_;
 std::vector<Entry> children_;
 GameObjectSceneCameraBorrowV13 active_camera_e4_;
 bool ambient_set_v67_{};
 bool source_need_register_v69_{}; // same unique App Scene; original static BSS0
 std::vector<GameObjectSceneRootBorrowV1> collected_roots_v69_;
 float time254_v102_{}; // CSceneManager C1 zero at58db58.
 std::vector<SceneNodeUpdateBorrowV102> update_nodes_v102_;
 bool update_nodes_produced_v102_{};
 std::uint64_t scene_epoch_v69_{};
 bool scene_epoch_produced_v69_{},scene_busy_v69_{},scene_failed_v69_{};
 std::string scene_error_v69_;
 //Actual SceneManager preloaded-node collection44c, distinct from visible
 //root children/render registration. Native resources have independent pins.
 std::shared_ptr<ScenePreloadCollectionV81> preloaded_scenes_v81_;
public:
 GameObjectSceneRootRegistryV1():identity_(reinterpret_cast<std::uintptr_t>(this)){}
 explicit GameObjectSceneRootRegistryV1(std::uintptr_t source_root_identity):identity_(source_root_identity){}
 bool add_child(GameObjectSceneRootBorrowV1,std::string&);
 bool borrow_registered_root_v110(std::uintptr_t,GameObjectSceneRootBorrowV1&,std::string&)const;
 bool release_visual_root(std::uintptr_t,std::string&);
 bool retire_unpublished_root_aliases_v92(std::uintptr_t,std::string&);
 bool remove_root_parent_reference_v1(std::uintptr_t,std::string&);
 bool retire_source_cached_aliases_v106(std::string&);
 bool source_delivery_busy_v106()const noexcept{return scene_busy_v69_;}
 void force_register()noexcept{fields_.force448=1;fields_.render_dirty289=1;}
 void notify_hierarchy_changed()noexcept{fields_.hierarchy_dirty288=1;fields_.render_dirty289=1;update_nodes_produced_v102_=false;}
 // Whole CSceneManager::notifyVisibilityChanged5890a8 writes only byte289.
 void notify_visibility_changed_v3()noexcept{fields_.render_dirty289=1;}
 const GameObjectSceneManagerFieldsV1& fields()const noexcept{return fields_;}
 const SceneManagerFrameFieldsV67& frame_fields_v67()const noexcept{return frame_v67_;}
 SceneManagerFrameFieldsV67& source_frame_fields_v67()noexcept{return frame_v67_;}
 void source_set_ambient_v67(const std::array<float,4>& rgba)noexcept{
  frame_v67_.ambient104=rgba;ambient_set_v67_=true;++frame_v67_.parameter_revision;
 }
 void source_update_fog_v67(float start,float end)noexcept{
  if(frame_v67_.fog432){frame_v67_.fog_start_end={start,end};++frame_v67_.parameter_revision;}
 }
 void source_disable_fog_v67()noexcept{frame_v67_.fog432=0;++frame_v67_.parameter_revision;}
 // Whole SceneManager.EnableFog35233c scalar/material-parameter domain:
 // source432 first, then distance pair, then original RGB conversion/alpha0.
 void source_enable_fog_v68(float start,float end,const std::array<float,3>& color)noexcept{
  frame_v67_.fog432=1;frame_v67_.fog_start_end={start,end};
  frame_v67_.fog_color={source_fog_color_byte_v68(color[0]),source_fog_color_byte_v68(color[1]),source_fog_color_byte_v68(color[2]),0};
  ++frame_v67_.parameter_revision;
 }
 std::vector<std::uintptr_t> roots()const;
 bool source_find_modular_v114(skinning::SourceModularSkinBorrowV114&,std::string&)const;
 bool source_find_light_v113(const std::string&,std::shared_ptr<NativeLightV113>&,std::string&)const;
 const std::shared_ptr<LightSetNameOwnerV3>& source_light_names_v89(){
  if(!source_light_names_v89_)source_light_names_v89_=std::make_shared<LightSetNameOwnerV3>();
  return source_light_names_v89_;
 }
 const std::shared_ptr<NativeLightSetV113>& source_light_runtime_v113()const noexcept{return source_light_runtime_v113_;}
 bool publish_light_runtime_v113(std::shared_ptr<NativeLightSetV113> actual,std::string& e){
  if(!actual||source_light_runtime_v113_){e="Actual Scene LightSet field owner already produced or absent";return false;}
  source_light_runtime_v113_=std::move(actual);e.clear();return true;
 }
 std::uintptr_t identity()const noexcept{return identity_;}
 std::uintptr_t scene_manager_identity_v16()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 bool set_active_camera_v13(GameObjectSceneCameraBorrowV13,std::string&);
 const GameObjectSceneCameraBorrowV13& active_camera_v13()const noexcept{return active_camera_e4_;}
 bool source_scene_phase_v69(std::uint64_t actual_draw_epoch,std::uint32_t actual_timer,std::string&);
 bool source_update_v102(float delta,bool optimized,const SceneUpdateTransportV102&,std::string&);
 bool source_register_nodes_v69(bool native_context_force,std::uint8_t actual_script_byte30,
   const SceneRegistrationTransportV69&,std::string&);
 bool source_preload_collection_v81(std::shared_ptr<ScenePreloadCollectionV81>,std::string&);
 const std::shared_ptr<ScenePreloadCollectionV81>& preloaded_scenes_v81()const noexcept{return preloaded_scenes_v81_;}
 //Only SceneManager.clear354468 releases preloaded nodes; neither GL context
 //loss nor LevelD1's separate cachedCharOID-map clear implies this operation.
 void source_clear_preloaded_scenes_v81()noexcept;
 // CSceneManager::setAmbientLight receives LevelConfig1cc RGB and alpha1.
 void set_ambient_v67(const std::array<float,4>&value)noexcept{source_set_ambient_v67(value);}
 const std::array<float,4>* ambient_v67()const noexcept{return ambient_set_v67_?&frame_v67_.ambient104:nullptr;}
};
}
