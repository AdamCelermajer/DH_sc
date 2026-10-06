#pragma once
#include "navigation_world.hpp"
namespace dh2::navigation {
struct HeightHit {float height,normal[3];std::uint32_t room,floor,hit,reserved;};
// UINT_MAX represents a null cached room/floor. Original parent-list storage
// belongs to the caller; notifications below identify the required service.
struct MotionObject {std::uint32_t flags,object_flags,room,floor;float position[3],normal[3];};
struct MotionPolicy {float maximum_height_delta;std::uint32_t ignore_height_delta;};
struct PositionResult {std::uint32_t valid,kind,parent_service,parent_change,old_floor,new_floor;};
struct DirectionRequest {const CollisionWorld* world;const float* position;float radius;std::uint32_t flags,reserved;};
static_assert(sizeof(HeightHit)==32&&sizeof(MotionObject)==40&&sizeof(MotionPolicy)==8&&sizeof(PositionResult)==24&&sizeof(DirectionRequest)==32);
}
extern "C" {
// 1 hit, 0 miss, -1 invalid caller. Miss retains supplied height/normal,
// resets only hit/room/floor. Native selector/collision supplies the triangle.
int dh2_nav_floor_height(dh2::navigation::HeightHit*,const dh2::selector::Floor*,const float*);
int dh2_nav_room_height(dh2::navigation::HeightHit*,const dh2::navigation::CollisionWorld*,std::uint32_t room,const float*,std::uint32_t include_special);
int dh2_nav_world_height(dh2::navigation::HeightHit*,const dh2::navigation::CollisionWorld*,const float*,std::uint32_t include_special);
// 0 completed, 1 malformed caller, before state/output writes. Result.valid is
// the original return bool; kind 0 miss, 1 epsilon-equal, 2 accepted, 3 clamped.
// Original obstacle parent-list mutation is represented by a service request;
// this function reconstructs floor/normal/position state, not that backend.
int dh2_nav_validate_position(dh2::navigation::PositionResult*,const dh2::navigation::CollisionWorld*,dh2::navigation::MotionObject*,float*,const dh2::navigation::MotionPolicy*);
// Recovered floor-boundary lookahead/edge slide. Radius is accepted as in the
// original overload but its body does not use it. Does not collide with movable
// PF obstacles, apply speed, root motion or advance a character controller.
int dh2_nav_validate_direction(std::uint32_t*,float*,const dh2::navigation::DirectionRequest*);
// Original glitch::line2d finite-segment intersection (distinct from the
// Point2D classification used by portal smoothing). 1 hit,0 miss,-1 invalid.
int dh2_nav_segment_intersect(float*,const float* first_line,const float* second_line);
}
