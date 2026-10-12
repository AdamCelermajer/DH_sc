#pragma once
#include "navigation_motion.hpp"
namespace dh2::navigation {
// The recovered motion/obstacle fields of PFObject. Path storage is separately
// owned by PathObject; this is not the original ARM32 binary layout.
struct NavigationObject {
 MotionObject motion;std::uint64_t user;
 float radius,obstacle_weight,obstacle_extent;std::uint32_t reserved;
};
struct ObstacleEntry {std::uint32_t floor,reserved;std::uint64_t object;};
// Empty map keys persist as in original map::operator[]. Entry order within a
// floor is insertion order. UINT_MAX is the original null-floor map key.
struct ObstacleRegistry {
 ObstacleEntry* entries;std::uint32_t count,capacity;
 std::uint32_t* floors;std::uint32_t floor_count,floor_capacity;
};
struct ObjectInitRequest {
 const CollisionWorld* geometry;NavigationObject* object;std::uint64_t user;
 float position[3],radius;std::uint32_t flying,reserved;
};
struct ObstacleInitRequest {
 const CollisionWorld* geometry;ObstacleRegistry* registry;NavigationObject* object;
 std::uint64_t key;float weight,extent;std::uint32_t enabled,reserved;
};
struct ObjectPositionRequest {
 const CollisionWorld* geometry;ObstacleRegistry* registry;NavigationObject* object;
 std::uint64_t key;float* position;const MotionPolicy* policy;
};
static_assert(sizeof(NavigationObject)==64&&sizeof(ObstacleEntry)==16&&sizeof(ObstacleRegistry)==32);
static_assert(sizeof(ObjectInitRequest)==48&&sizeof(ObstacleInitRequest)==48&&sizeof(ObjectPositionRequest)==48);
}
extern "C" {
// Default recovered motion/obstacle fields; path/debug containers are outside
// this view. Original default normal is Vec3f_K, capability mask is 2.
int dh2_nav_object_defaults(dh2::navigation::NavigationObject*);
int dh2_nav_motion_policy_defaults(dh2::navigation::MotionPolicy*);
int dh2_nav_object_set_flying(dh2::navigation::NavigationObject*,std::uint32_t);
int dh2_nav_object_set_swimming(dh2::navigation::NavigationObject*,std::uint32_t);
int dh2_nav_object_is_flying(const dh2::navigation::NavigationObject*);
int dh2_nav_object_is_swimming(const dh2::navigation::NavigationObject*);
// 0 completed, 1 malformed caller, 2 insufficient bounded registry storage.
// Invalid/insufficient requests leave all caller state unchanged. The original
// InitObject bool is object flag 1; SetFlying changes capability mask bit 1.
int dh2_nav_init_object(const dh2::navigation::ObjectInitRequest*);
int dh2_nav_init_obstacle(const dh2::navigation::ObstacleInitRequest*);
// Recovered parent-list backend only; does not change object.motion.floor.
int dh2_nav_change_obstacle_parent(dh2::navigation::ObstacleRegistry*,const dh2::navigation::NavigationObject*,std::uint64_t key,std::uint32_t target_floor);
// Position validation with the recovered parent-list service attached. It is
// still not a speed/root-motion or dynamic-obstacle character controller.
int dh2_nav_validate_object_position(dh2::navigation::PositionResult*,const dh2::navigation::ObjectPositionRequest*);
}
