#pragma once
#include "canonical_level_module_bindings_v2.hpp"
#include "retained_module_visual_v3.hpp"
#include "condition_data_init_v3.hpp"
#include "game_object_visual_asset_owner_v1.hpp"
#include "game_object_set_position_v2.hpp"
#include "module_pf_room_v3.hpp"
namespace dh2::world {
struct CanonicalModuleGraphServicesV3 {
 std::shared_ptr<void> candidate;
 std::shared_ptr<ModulePFRoomsV3> rooms; // SAME floor world/map owner
 RetainedModuleVisualServicesV3 visual;
 ConditionDataInitServicesV3 conditions;
 GameObjectSetPositionServicesV2 position;
 std::function<bool(const std::string&,std::shared_ptr<const std::vector<std::uint8_t>>&,bool&,std::string&)> read_asset;
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::string&)> update_pf;
 // Whole SAME canonical Spawn owner, never a separate Zone registration.
 std::function<bool(const std::string&,target_providers::Handle16&,std::uint32_t&,std::string&)> spawn_zone;
 std::function<bool(target_providers::Handle16&,const std::array<float,6>&,std::uintptr_t,std::string&)> zone_init;
};
// Production composition of actual selected Module visual/mesh/PFRoom graph.
// Platform/source spawn, device, enable, PF InitObject and LightSet queries in
// incoming GameObject services are retained and required when reached. This
// builder supplies concrete graph continuations instead of empty callbacks.
class CanonicalModuleGraphV3 : public std::enable_shared_from_this<CanonicalModuleGraphV3> {
 struct Entry {
  std::weak_ptr<CanonicalModuleRecordV2> record;
  std::map<std::uintptr_t,std::shared_ptr<RetainedModuleVisualV3>> visuals;
  std::unique_ptr<GameObjectVisualAssetOwnerV1> assets;
 };
 CanonicalModuleGraphServicesV3 services_;
 std::map<const CanonicalModuleRecordV2*,std::shared_ptr<Entry>> entries_;
 bool missing(const char*,std::string&)const;
public:
 explicit CanonicalModuleGraphV3(CanonicalModuleGraphServicesV3 s):services_(std::move(s)){}
 bool bind(const std::shared_ptr<CanonicalModuleRecordV2>&,
  GameObjectInitializationServicesV1& actual_platform,ModuleInitServicesV1&,std::string&);
 // Explicit actual visual destruction before canonical receiver/world release.
 bool release(const CanonicalModuleRecordV2*,std::string&);
 std::shared_ptr<RetainedModuleVisualV3> visual(std::uintptr_t)const;
};
}
