#pragma once
#include "gameobject_scene_root_registry_v1.hpp"
namespace dh2::world {
// Reuses SAME actual SceneManager root membership owner for Item resources.
// Lookup borrows the constructing candidate from its one visual registry.
RetainedGameObjectVisualServicesV1 world_item_scene_services_v3(RetainedGameObjectVisualServicesV1,std::shared_ptr<GameObjectSceneRootRegistryV1>,std::function<std::shared_ptr<RetainedGameObjectVisualV1>(std::uintptr_t)> lookup_root);
}
