#pragma once
#include "canonical_level_config_module_v1.hpp"
#include "gameobject_scene_root_registry_v1.hpp"
namespace dh2::world {
struct LevelFogServicesV103 {
 std::shared_ptr<void> provider;
 std::function<bool(std::shared_ptr<CanonicalLevelConfigV1>&,std::string&)> current_config;
 std::function<bool(std::string&)> trace_level;
 std::function<bool(std::shared_ptr<GameObjectSceneRootRegistryV1>&,std::string&)> scene;
};
// UpdateFog3f2304 reborrows config after Debug before EnableFog3f1430.
// The SceneManager Enable/Disable methods ignore the passed node: their
// actual effects are the same global fog flag and material parameters.
bool source_level_update_fog_v103(const LevelFogServicesV103&,std::string&);
// Whole original methods3ef234 and3ef280 are bx lr, including all arguments.
inline void source_level_update_material_v103(bool,bool)noexcept{}
inline void source_level_update_light_set_v103(bool,std::int32_t)noexcept{}
}
