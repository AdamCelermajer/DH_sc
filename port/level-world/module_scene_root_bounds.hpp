#pragma once

#include "octree.hpp"
#include "../world-data/world.hpp"
#include "../world-data/world_scene.hpp"
#include "../scene-payloads/scene.hpp"
#include "../engine-resources/resources.hpp"

namespace dh2::module_scene_root_bounds {

enum class Status : std::uint32_t {
    complete,
    invalid_argument,
    unsupported_transform,
    scene_failure,
    unsupported_instance,
    geometry_failure,
    transform_failure,
    empty_scene
};

struct Result {
    dh2::octree::Box bounds;
    std::uint32_t scene_nodes;
    std::uint32_t geometry_instances;
    std::uint32_t draw_buffers;
    std::uint32_t ignored_non_geometry_instances;
};

// Reproduce the data path used by Module::InitPost for static mesh scene roots.
// owner_world_position is the live VisualObject owner position (GameObject
// +0x160), not an inferred DACT room/module index. Module transforms and the
// selected serialized root must satisfy world_scene's translation-only gates.
// The output is the union of each mesh's serialized local AABB transformed by
// its selected-subtree scene matrix and the owner's source position.
Status build(const dh2::world::Module* module,
             const dh2::scene_payload::Scene* scene,
             const dh2::resources::BresView* catalogue,
             const float owner_world_position[3], Result* output);

} // namespace dh2::module_scene_root_bounds
