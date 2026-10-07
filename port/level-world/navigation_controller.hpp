#pragma once
#include "navigation_path.hpp"
#include "navigation_avoidance.hpp"
#include "navigation_heading.hpp"
namespace dh2::navigation {
struct PathController {float position[3],destination[3];HeadingState heading;std::uint32_t path_requested,validate_boundary;};
struct ControllerPolicy {std::uint32_t update_path,avoid_obstacles,debug_skip_boundary,update_physics;};
// Caller scratch is distinct from all input and output storage. The scratch
// may change on failure, but controller, path, object, registry and result do not.
struct ControllerWorkspace {
 PathSegment* segments;std::uint32_t segment_capacity,reserved0;
 AvoidanceActor* actors;std::uint32_t actor_capacity,reserved1;
 std::uint32_t* floors;std::uint32_t floor_capacity,reserved2;
};
// Source Debug is queried only when boundary validation is reached.
struct ControllerSourceServicesV69 {void* context{};int(*skip_boundary)(void*,std::uint32_t*){};};
struct ControllerRequest {
 PathController* controller;PathObject* path;NavigationObject* object;
 const CollisionWorld* geometry;const Graph* graph;const AvoidanceScene* scene;
 const ControllerPolicy* policy;ControllerWorkspace* workspace;std::uint64_t key;
};
struct ControllerResult {
 MoveResult move;AvoidanceResult avoidance;
 std::uint32_t at_destination,stopped,boundary_checked,direction_valid,physical_stop_requested,reserved;
};
static_assert(sizeof(PathController)==56&&sizeof(ControllerPolicy)==16&&sizeof(ControllerWorkspace)==48&&sizeof(ControllerRequest)==72&&sizeof(ControllerResult)==80);
}
extern "C" {
// Recovered IsAtDestination: final path target if nonempty, otherwise the
// GameObject destination. Strict squared XY distance < 6400; ignores Z.
// 1 at destination, 0 not there, -1 malformed caller.
int dh2_nav_is_at_destination(const dh2::navigation::PathController*,const dh2::navigation::PathObject*);
// Recovered UpdatePath coordinator. Returns 0 completed, 1 malformed caller,
// 2 insufficient scratch/map storage. Virtual policy values are caller facts.
// Stop's physical backend is an explicit request; applying velocity/body
// resets and UpdateSubObjects/root motion are separate, pending work.
int dh2_nav_update_path(dh2::navigation::ControllerResult*,const dh2::navigation::ControllerRequest*);
int dh2_nav_update_path_source_v69(dh2::navigation::ControllerResult*,const dh2::navigation::ControllerRequest*,const dh2::navigation::ControllerSourceServicesV69*);
}
