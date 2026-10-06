#pragma once
#include "navigation_controller.hpp"
#include "body_transform.hpp"
namespace dh2::navigation {
struct ControllerPhysicalBinding {
 physical::TransformBody* body;
 physical::TransformWorld* world;
 const physical::TransformCallbacks* callbacks;
};
struct ControllerPhysicalResult {
 ControllerResult path;
 std::int32_t transform_result;
 std::uint32_t stop_applied;
};
static_assert(sizeof(ControllerPhysicalBinding)==24&&sizeof(ControllerPhysicalResult)==88);
}
extern "C" {
// Recovered UpdatePath plus its actual physical Stop sequence. Scene body
// presence must agree with the optional binding. A required body binding is
// validated before UpdatePath mutates caller state. All state/scratch/output
// allocations are distinct. Shape/proxy/broadphase callbacks remain caller
// services; this does not advance the physics world or UpdateSubObjects.
// 0 completed, 1 malformed caller/binding, 2 insufficient controller storage.
int dh2_nav_update_path_physical(dh2::navigation::ControllerPhysicalResult*,
 const dh2::navigation::ControllerRequest*,
 const dh2::navigation::ControllerPhysicalBinding*);
}
