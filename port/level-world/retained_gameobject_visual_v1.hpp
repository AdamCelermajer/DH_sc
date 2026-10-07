#pragma once
#include "canonical_gameobject_base_owner_v1.hpp"
#include "gameobject_visual_field_borrow_v5.hpp"
#include "base_named_animation_controller_v1.hpp"
#include "generic_animation_callbacks_v21.hpp"
#include "decor_scene.hpp"
#include "retained_visual_child_v91.hpp"
#include "retained_scene_node_borrow_v109.hpp"
#include "retained_map_mesh_v93.hpp"
#include "gameobject_scene_binding_v1.hpp"
#include "../engine-skinning/skinning.hpp"
#include "../engine-skinning/visual_skin_owner_v6.hpp"
#include <functional>
#include <optional>
namespace dh2::world {
class NativeLightSetV113;
struct RetainedGameObjectVisualServicesV1 {
 std::shared_ptr<void> owner;
 // Source asset miss is delivered=true/found=false, distinct from I/O failure.
 std::function<bool(const std::string&,std::vector<std::uint8_t>&,bool&,std::string&)> read_asset;
 // Actual SceneManager registration/ForceRegister; no fabricated registration.
 std::function<bool(std::uintptr_t,std::string&)> register_root,force_register;
 // Whole source root virtual74/68/drop continuation for this actual registry.
 std::function<bool(std::uintptr_t,std::string&)> release_root;
 std::function<bool(bool&,std::string&)> parent_is_animated;
 std::function<bool(std::string&)> update_pf;
 std::function<bool(skinning::SourceModularSkinBorrowV114&,std::string&)> find_modular_v114;
 std::function<bool(std::string&)> notify_visibility_v114;
};
// Native semantic successor of VisualObjectC1 for the verified complete BRES
// static/skinned mesh + colbox domain. Owns bytes, graph, skin and animator.
// Unsupported scene kinds fail explicitly; no foreign ARM object is cast.
class RetainedGameObjectVisualV1 {
public:
 struct SkinnedMesh {
  std::uint32_t instance{};
  skinning::Skin skin;
  std::vector<skinning::Matrix> palette;
  std::shared_ptr<RetainedMeshDataV91> mesh_owner;
  std::vector<std::array<float,3>>& source_positions;
  std::vector<std::array<float,3>>& positions;
  explicit SkinnedMesh(std::shared_ptr<RetainedMeshDataV91> mesh):mesh_owner(std::move(mesh)),source_positions(mesh_owner->source_positions),positions(mesh_owner->positions){}
 };
private:
 struct Clip {std::string name;std::int32_t start{},end{};};
 using NodeHandle=RetainedVisualNodeV91;
 CanonicalGameObjectBaseOwnerV1* base_{};
 GameObjectVisualFieldBorrowV5 fields_;
 RetainedGameObjectVisualServicesV1 services_;
 // ONE immutable-after-read resource block; floor clones pin this independently.
 std::shared_ptr<std::vector<std::uint8_t>> bytes_;
 resources::BresView bres_{};
 scene::Scene scene_;
 scene::NodeFlagsViewV91 node_flags_;
 using Visibility=scene::VisibilityV91;
 Visibility root_visibility_;
 scene::NodeVisibilityViewV91 node_visibility_;
 scene::AuthoredVisibilityV76 authored_visibility_v76_;
 using MeshFieldsV76=scene::MeshFieldsV91;
 scene::MeshFieldsViewV91 mesh_fields_v76_;
 scene::NodeDetachViewV91 detached_nodes_v76_;
 scene::MeshDetachViewV91 detached_meshes_v76_;
 std::array<float,16> root_cached_v76_{};
 std::uint8_t root_force_position208_v96_{1}; //RootC1 35d8d4 actual store1
 std::uint8_t root_character_update200_v106_{1}; //RootC1 35d8f0, written by Character.CanUpdate.
 bool static_optimized_v76_{};
 std::uintptr_t root_parentec_{};
 std::string root_name24_v109_; // ISceneNode C1 empty name, independent of model URI.
 std::vector<std::shared_ptr<NodeHandle>> node_handles_;
 std::vector<std::shared_ptr<RetainedMeshNodeV91>> native_meshes_v93_;
 std::vector<std::shared_ptr<RetainedMeshDataV91>> mesh_resources_v91_;
 std::vector<SkinnedMesh> skinned_;
 std::optional<skinning::VisualSkinResourcesV6> modular_resources_v114_;
 std::shared_ptr<skinning::VisualSkinOwnerV6> own_modular_v114_;
 skinning::SourceModularSkinBorrowV114 selected_modular2c_v114_;
 visual::GameObjectSceneBindingV1 binding_;
 animation::Player animation_;
 std::vector<Clip> clips_;
 timeline::State timeline_{};
 timeline::Completion completion_{}; // actual AnimApplicator extra10/pending30
 GenericAnimatorCallbackFieldsV21 generic_callbacks_v69_; // SAME animator callback fields
 physical::DecorSceneMarker marker_{};
 std::array<float,6> mesh_box_{};
 std::uint32_t timestamp_{};
 std::uintptr_t root_game_object204_{}; // source Root ctor35d8f4 stores0
 std::int32_t light_set40_{}; // source Visual ctor472a98 stores0
 std::vector<bool> light_filter44_v113_; //Visual C1 empty vector; actual InitLightFilter writes five.
 std::uint8_t material_rim7e_v113_{},material_special_a9_v113_{}; //472abc/472ae8 C1 stores0.
 std::vector<std::uintptr_t> shadow_nodes8c_v113_,xray_nodes9c_v113_; //472ad0..472ae4 C1-empty.
 bool root_present_{},ready_{},animated_{};
 bool source_parent_detached_v112_{}; //only source SetParent(NULL) writes this
 bool displacement_enabled_{}; // source RootSceneNode+1ec constructor0
 bool root_animator_present_{};
 std::uint32_t batch_animation_references_v112_{};
 bool batch_visual_released_v112_{};
 std::function<bool(std::string&)> character_animator_remove_v6_;
 std::weak_ptr<void> character_scene_lifetime_v69_;
 std::function<bool(std::uint32_t,std::string&)> character_scene_phase_v69_;
 std::uint8_t root_visible_request209_{}; // RootSceneNode ctor0, deferred setVisible
 bool root_visibility_failed_v3_{}; // host required-provider diagnostic, no retry
 std::optional<std::uint32_t> root_automatic_culling118_v70_;
 bool missing(const char*,std::string&)const;
 bool clips(std::string&);
 bool sample(bool reset,std::string&);
 bool initialize_child_owners_v91(std::string&);
 bool load_skinned_meshes(std::string&);
 bool update_skinned_meshes(std::string&);
 void notify_node_visibility(std::size_t,bool);
 void propagate_node_visibility(std::size_t);
 void update_mesh_visibility_v76(std::size_t,bool);
 bool update_root_cache_v76(std::string&);
 void optimize_node_v76(unsigned);
 bool optimize_static_v76(std::string&);
public:
 RetainedGameObjectVisualV1(CanonicalGameObjectBaseOwnerV1&,RetainedGameObjectVisualServicesV1);
 // Additive receiver-borrow constructor. Class layout changed: rebuild every
 // owner/consumer coherently; do not interpose this into an older APK layout.
 RetainedGameObjectVisualV1(GameObjectVisualFieldBorrowV5,RetainedGameObjectVisualServicesV1);
 RetainedGameObjectVisualV1(const RetainedGameObjectVisualV1&)=delete;
 bool initialize(const char* model,const char* xref,std::string&);
 bool sync(std::string&);
 bool sync_position_v7(std::string&);
 bool sync_rotation_v86(std::string&); //same Root/parent16c, no position/scale store
 bool sync_scaling_v111(std::string&); //same Root/parent120, no pose/timeline store
 bool source_modular_receivers_v114(std::weak_ptr<void>,std::vector<skinning::SourceModularSkinBorrowV114>&,std::string&)const;
 bool source_light_by_name_v113(const std::string&,std::shared_ptr<NativeLightV113>&,std::string&)const;
 bool source_set_modular_skin_v114(std::int32_t,std::int32_t,std::string&);
 bool modular_draw_views_v114(const std::vector<skinning::VisualDrawViewV32>*&,std::string&)const;
 std::optional<std::uint32_t> source_modular_node_v114()const;
 //Whole VisualObject.ForceUpdatePosition470bd0: mark the existing Root208
 //only. This performs no scene/animation/cache/physics update.
 void source_force_update_position_v96()noexcept{if(root_present_)root_force_position208_v96_=1;}
 std::uint8_t source_force_position208_v96()const noexcept{return root_force_position208_v96_;}
 bool calc_mesh_box(std::string&);
 bool apply_mesh_box(std::string&);
 bool play(const char*,bool loop,bool& accepted,std::string&);
 bool update(std::uint32_t absolute_ms,std::string&);
 // Additive source callback continuation; no layout/timeline/pose duplication.
 bool update_callbacks_v21(std::uint32_t,GenericAnimatorCallbackFieldsV21&,std::string&);
 GenericAnimatorCallbackFieldsV21& generic_callbacks_v69()noexcept{return generic_callbacks_v69_;}
 bool source_scene_phase_v69(std::uint32_t,const std::function<bool(std::string&)>& notify_visibility,std::string&);
 bool bind_character_scene_phase_v69(std::weak_ptr<void>,std::function<bool(std::uint32_t,std::string&)>,std::string&);
 bool retain_batch_animation_v112(const std::shared_ptr<RetainedGameObjectVisualV1>&,BatchAnimationBorrowV112&,std::string&);
 bool batch_animation_retained_v112()const noexcept{return batch_animation_references_v112_!=0;}
 bool batch_animation_phase_v112(std::uint32_t,std::string&);
 bool sync_batch_parent_pose_v113(std::string&);
 bool borrow_batch_root_matrix_v113(const std::array<float,16>*&,std::string&)const;
 void drop_batch_animation_v112()noexcept;
 bool generic_animator_identity_v21(std::uintptr_t&,bool&,std::string&)const;
 bool release(std::string&);
 bool set_root_game_object(std::uintptr_t identity,std::string&);
 std::uintptr_t root_game_object()const noexcept{return root_game_object204_;}
 void store_light_set(std::int32_t id)noexcept{light_set40_=id;}
 std::int32_t light_set()const noexcept{return light_set40_;}
 std::vector<bool>& source_light_filter44_v113()noexcept{return light_filter44_v113_;}
 bool source_apply_light_set_v113(NativeLightSetV113&,std::string&);
 bool source_apply_material_tail_v113(std::string&);
 bool node_from_name(const char*,std::uintptr_t&,std::string&)const;
 bool source_set_visible_recur_v91(bool,std::string&);
 bool source_mesh_name_v93(unsigned,const std::shared_ptr<SceneManagerMapOwnerV2>&,std::string&,unsigned&,std::string&)const;
 bool lend_map_mesh_v93(unsigned,const std::shared_ptr<SceneManagerMapOwnerV2>&,SceneMapNodeBorrowV2&,std::string&)const;
 bool grab_node_v91(std::uintptr_t,std::shared_ptr<RetainedVisualNodeV91>&,std::string&)const;
 bool borrow_mesh_source_v111(unsigned,std::shared_ptr<RetainedMeshNodeV91>&,std::string&)const;
 bool borrow_scene_node_v109(const std::shared_ptr<RetainedGameObjectVisualV1>&,
   std::uintptr_t,RetainedSceneNodeBorrowV109&,std::string&);
 bool node_position(std::uintptr_t,float* out3,std::string&)const;
 bool source_node_set_position_v112(std::uintptr_t,const float*,std::string&);
 bool source_root_set_position_v112(const float*,std::string&);
 void source_set_parent_null_v112()noexcept{source_parent_detached_v112_=true;}
 bool source_current_clip_duration_v112(std::uint32_t group,std::int32_t&,std::string&)const;
 bool borrow_node_position_v80(std::uintptr_t,const float*&,std::string&)const;
 // SAME stable graph indices, including authored hidden children. Detach never erases/reindexes.
 bool source_mesh_pose_v76(unsigned,std::array<float,4>&,std::array<float,3>&,std::string&)const;
 bool mesh_attached_v76(unsigned)const noexcept;
 bool node_attached_v76(unsigned)const noexcept;
 bool mesh_visible_v76(unsigned)const noexcept;
 bool remove_floor_mesh_v76(unsigned,std::string&);
 bool remove_node_v76(unsigned,std::string&);
 // Pins SAME actual read bytes, with no Visual/World/manager retention or copy.
 std::shared_ptr<void> resource_owner_v76()const noexcept{return bytes_;}
 bool notify_root_visibility(bool,std::string&);
 bool set_root_local_visibility_v3(bool,std::string&);
 bool commit_root_visibility_v3(const std::function<bool(std::string&)>& notify_visibility_changed,std::string&);
 bool set_node_local_visibility(std::uintptr_t,bool,std::string&);
 bool remove_root_animators(std::string&);
 // Source root owns one replacement Character animator delivery endpoint.
 // Replaces generic animation tracks through real removeAnimators first.
 bool attach_character_animator_v6(std::function<bool(std::string&)>,std::string&);
 bool character_pose_changed_v6(std::string&);
 bool root_animator_present()const noexcept{return root_animator_present_;}
 unsigned animation_track_count()const noexcept{return animation_.track_count();}
 std::uintptr_t& root_parent_identity()noexcept{return root_parentec_;}
 bool ready()const noexcept{return ready_;}
 // Whole ISceneNode.setAutomaticCulling59719c is one raw enum word store.
 // No constructor value is inferred before the actual setter is reached.
 void source_set_automatic_culling_v70(std::uint32_t value)noexcept{root_automatic_culling118_v70_=value;}
 const std::optional<std::uint32_t>& source_automatic_culling_v70()const noexcept{return root_automatic_culling118_v70_;}
 std::uint8_t& source_character_update200_v106()noexcept{return root_character_update200_v106_;}
 std::uintptr_t root_identity()noexcept{return root_present_?reinterpret_cast<std::uintptr_t>(&binding_.root):0;}
 const resources::BresView& bres()const noexcept{return bres_;}
 scene::Scene& scene()noexcept{return scene_;}
 visual::GameObjectSceneBindingV1& binding()noexcept{return binding_;}
 timeline::State& timeline()noexcept{return timeline_;}
 std::uint32_t& scene_flags()noexcept{return binding_.root.flags;}
 const scene::NodeFlagsViewV91& node_flags()const noexcept{return node_flags_;}
 const std::vector<SkinnedMesh>& skinned_meshes()const noexcept{return skinned_;}
 const physical::DecorSceneMarker& marker()const noexcept{return marker_;}
 const std::array<float,6>& mesh_box()const noexcept{return mesh_box_;}
 BaseNamedAnimationBorrowV1 named_animation(std::shared_ptr<void> real_visual_lease);
};
}
