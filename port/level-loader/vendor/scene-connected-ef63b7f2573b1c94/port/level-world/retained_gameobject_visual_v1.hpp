#pragma once
#include "canonical_gameobject_base_owner_v1.hpp"
#include "base_named_animation_controller_v1.hpp"
#include "decor_scene.hpp"
#include "gameobject_scene_binding_v1.hpp"
#include "../engine-skinning/skinning.hpp"
#include <functional>
namespace dh2::world {
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
  std::vector<std::array<float,3>> source_positions,positions;
 };
private:
 struct Clip {std::string name;std::int32_t start{},end{};};
 struct NodeHandle {std::uint32_t index;};
 CanonicalGameObjectBaseOwnerV1& base_;
 RetainedGameObjectVisualServicesV1 services_;
 std::vector<std::uint8_t> bytes_;
 resources::BresView bres_{};
 scene::Scene scene_;
 std::vector<std::uint32_t> node_flags_;
 struct Visibility {std::uint8_t local120{1},parent121{1};};
 Visibility root_visibility_;
 std::vector<Visibility> node_visibility_;
 std::uintptr_t root_parentec_{};
 std::vector<std::unique_ptr<NodeHandle>> node_handles_;
 std::vector<SkinnedMesh> skinned_;
 visual::GameObjectSceneBindingV1 binding_;
 animation::Player animation_;
 std::vector<Clip> clips_;
 timeline::State timeline_{};
 timeline::Completion completion_{}; // actual AnimApplicator extra10/pending30
 physical::DecorSceneMarker marker_{};
 std::array<float,6> mesh_box_{};
 std::uint32_t timestamp_{};
 std::uintptr_t root_game_object204_{}; // source Root ctor35d8f4 stores0
 std::int32_t light_set40_{}; // source Visual ctor472a98 stores0
 bool root_present_{},ready_{},animated_{};
 bool displacement_enabled_{}; // source RootSceneNode+1ec constructor0
 bool root_animator_present_{};
 bool missing(const char*,std::string&)const;
 bool clips(std::string&);
 bool sample(bool reset,std::string&);
 bool load_skinned_meshes(std::string&);
 bool update_skinned_meshes(std::string&);
 void notify_node_visibility(std::size_t,bool);
 void propagate_node_visibility(std::size_t);
public:
 RetainedGameObjectVisualV1(CanonicalGameObjectBaseOwnerV1&,RetainedGameObjectVisualServicesV1);
 RetainedGameObjectVisualV1(const RetainedGameObjectVisualV1&)=delete;
 bool initialize(const char* model,const char* xref,std::string&);
 bool sync(std::string&);
 bool calc_mesh_box(std::string&);
 bool apply_mesh_box(std::string&);
 bool play(const char*,bool loop,bool& accepted,std::string&);
 bool update(std::uint32_t absolute_ms,std::string&);
 bool release(std::string&);
 bool set_root_game_object(std::uintptr_t identity,std::string&);
 std::uintptr_t root_game_object()const noexcept{return root_game_object204_;}
 void store_light_set(std::int32_t id)noexcept{light_set40_=id;}
 std::int32_t light_set()const noexcept{return light_set40_;}
 bool node_from_name(const char*,std::uintptr_t&,std::string&)const;
 bool node_position(std::uintptr_t,float* out3,std::string&)const;
 bool notify_root_visibility(bool,std::string&);
 bool set_node_local_visibility(std::uintptr_t,bool,std::string&);
 bool remove_root_animators(std::string&);
 bool root_animator_present()const noexcept{return root_animator_present_;}
 unsigned animation_track_count()const noexcept{return animation_.track_count();}
 std::uintptr_t& root_parent_identity()noexcept{return root_parentec_;}
 bool ready()const noexcept{return ready_;}
 std::uintptr_t root_identity()noexcept{return root_present_?reinterpret_cast<std::uintptr_t>(&binding_.root):0;}
 const resources::BresView& bres()const noexcept{return bres_;}
 scene::Scene& scene()noexcept{return scene_;}
 visual::GameObjectSceneBindingV1& binding()noexcept{return binding_;}
 timeline::State& timeline()noexcept{return timeline_;}
 std::uint32_t& scene_flags()noexcept{return binding_.root.flags;}
 const std::vector<std::uint32_t>& node_flags()const noexcept{return node_flags_;}
 const std::vector<SkinnedMesh>& skinned_meshes()const noexcept{return skinned_;}
 const physical::DecorSceneMarker& marker()const noexcept{return marker_;}
 const std::array<float,6>& mesh_box()const noexcept{return mesh_box_;}
 BaseNamedAnimationBorrowV1 named_animation(std::shared_ptr<void> real_visual_lease);
};
}
