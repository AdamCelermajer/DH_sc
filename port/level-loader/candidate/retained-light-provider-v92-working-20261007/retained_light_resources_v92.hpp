#pragma once
#include "retained_visual_child_v91.hpp"
#include "gameobject_scene_root_registry_v1.hpp"
#include "canonical_light_point_v53.hpp"
#include <array>
#include <thread>
namespace dh2::world {
// Native CLight59fe08 storage. The original pooled intrusive ABI is replaced
// by one owned resource; its transformation still names the SAME node matrix.
struct RetainedLightFieldsV92 {
 std::array<float,4> ambient4{0,0,0,1},diffuse14{1,1,1,1},specular24{1,1,1,1};
 std::array<float,3> attenuation34{1,0,0};
 float radius40,falloff44{0},outer48{45},inner4c{2};
 std::uint16_t type58{};std::uint8_t enabled5a{1};
 std::shared_ptr<scene::NodeStorageV91> transformation;
 std::shared_ptr<std::array<float,16>> retired_transformation;
 RetainedLightFieldsV92();
 const float* absolute_transform()const noexcept;
};
// The light node is a genuine owned subclass/resource in the SAME node domain;
// CLight allocation is not a VisualObject or immutable BRES byte alias.
struct RetainedLightSceneNodeV92 final:RetainedVisualNodeV91 {
 std::shared_ptr<RetainedLightFieldsV92> light;
 std::array<float,6> bounds13c{-1,-1,-1,1,1,1};
 std::uint16_t cached_type138{};std::uint32_t culling118{};
 std::uintptr_t parentec{};
 bool actor_reference_live{},root_reference_live{};
 RetainedLightSceneNodeV92();
 ~RetainedLightSceneNodeV92();
 void recalculate_source_bounds()noexcept;
 const std::array<float,6>& source_bounds_v92()noexcept{if(cached_type138!=light->type58)recalculate_source_bounds();return bounds13c;}
 bool update_static_node(std::uint32_t,std::string&);
};
struct RetainedLightSceneV92 {
 scene::Scene scene;
 std::vector<std::shared_ptr<RetainedVisualNodeV91>> nodes;
 // Original CRootSceneNode188 raw list preserves construction order.
 std::vector<std::shared_ptr<RetainedLightSceneNodeV92>> lights188;
 static bool load(const resources::BresView&,std::shared_ptr<RetainedLightSceneV92>&,std::string&);
};
// ONE resource facet on the actual Scene's LightSetManager, borrowing its SAME
// previously constructed Names fields. No parallel name manager/table/cache.
struct RetainedLightScenePrefixV92 {std::shared_ptr<RetainedLightSceneV92> root;};
class SceneLightResourcesV92 {
public:
 struct Automatic {std::uintptr_t actor{};std::weak_ptr<RetainedLightSceneNodeV92> node;};
 struct Active {target_providers::Handle16 target;std::weak_ptr<RetainedLightSceneNodeV92> node;};
private:
 std::shared_ptr<LightSetNameOwnerV3> names_;
 std::array<std::array<std::shared_ptr<RetainedLightFieldsV92>,5>,4> lights78_{};
 std::array<std::array<std::uint8_t,5>,4> dirty64_{};
 std::vector<Automatic> automatic424_;
 std::vector<Active> active11c_;
 std::vector<std::weak_ptr<RetainedLightSceneNodeV92>> staticdc_;
 std::thread::id owning_thread_;
 std::uint64_t generation_{1}; // Native lifetime epoch, not game/script state.
public:
 explicit SceneLightResourcesV92(std::shared_ptr<LightSetNameOwnerV3>);
 const auto& names()const noexcept{return names_;}
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 std::uint64_t generation()const noexcept{return generation_;}
 bool owning_thread()const noexcept{return std::this_thread::get_id()==owning_thread_;}
 bool set_light(std::int32_t,std::int32_t,std::shared_ptr<RetainedLightFieldsV92>,std::string&);
 bool add_automatic(std::uintptr_t,std::shared_ptr<RetainedLightSceneNodeV92>,std::string&);
 bool add_active(target_providers::Handle16,std::shared_ptr<RetainedLightSceneNodeV92>,std::string&);
 const auto& automatic424()const noexcept{return automatic424_;}
 const auto& active11c()const noexcept{return active11c_;}
 const auto& source_lights78()const noexcept{return lights78_;}
 const auto& source_dirty64()const noexcept{return dirty64_;}
 // Invoke only at original SceneManager.clear's LightSet assignment prefix.
 // The separate automatic424 actor collection is NOT inventedly erased here.
 void source_reset_lightset_at_scene_clear()noexcept;
};
class RetainedLightActorReferenceV92 final:public LightActorNodeReferenceV67 {
 std::shared_ptr<RetainedLightSceneNodeV92> actor_pin_;
 std::weak_ptr<SceneLightResourcesV92> scene_;
 bool consumed_{};
public:
 RetainedLightActorReferenceV92(std::shared_ptr<RetainedLightSceneNodeV92>,std::shared_ptr<SceneLightResourcesV92>,
  std::shared_ptr<void>,LightRuntimeStampV67,std::uintptr_t);
 bool validate_exact_actor_reference(const LightQuiescenceLeaseV67&)const noexcept override;
 void drop_actual_actor_node_reference()noexcept override;
};
bool borrow_retained_light_parameters_v92(const LightNodeBorrowV53&,LightParameterFieldsV53&,std::string&);
bool attach_retained_light_root_v92(GameObjectSceneRootRegistryV1&,const LightNodeBorrowV53&,std::string&);
}
