#pragma once
#include "navigation_objects.hpp"
namespace dh2::navigation {
struct ContactFilter {std::int16_t group;std::uint16_t category,mask,present;};
// Recovered PhysicalObject fields used by canCollide/onCollisionTest. Primary
// shape takes precedence over secondary. GameObject ownership is a caller fact.
struct PhysicalContact {std::uint32_t present,disabled,owner_present,owner_enabled;ContactFilter primary,secondary;};
struct AvoidanceActor {
 NavigationObject object;float target[3];std::uint32_t has_path;
 float path_target[3];std::uint32_t reserved;PhysicalContact physical;
};
struct AvoidanceScene {
 ObstacleRegistry* registry;const AvoidanceActor* actors;const std::uint64_t* keys;std::uint32_t count,reserved;
 //Optional whole selected PhysicalObject.canCollide receiver. Native keys
 //resolve actual retained owners, never default filters for Character vptrs.
 void* collision_context{};
 //0 delivered (allowed is the actual bool), nonzero required-provider failure.
 int(*can_collide_v108)(void*,std::uint64_t,std::uint64_t,std::int32_t*){};
};
struct ObstacleForce {float direction[3],coefficient;std::uint64_t object;};
struct ForceBuffer {ObstacleForce* entries;std::uint32_t count,capacity;};
struct ForceResult {float direction[3];std::uint32_t count;};
struct AvoidanceResult {ForceResult force;std::uint32_t evaluated,adjusted,turn_limited,reserved;};
struct AvoidanceRequest {const AvoidanceScene* scene;std::uint64_t object;ForceBuffer* records;};
static_assert(sizeof(ContactFilter)==8&&sizeof(PhysicalContact)==32&&sizeof(AvoidanceActor)==128);
static_assert(sizeof(AvoidanceScene)==48);
static_assert(sizeof(ObstacleForce)==24&&sizeof(ForceBuffer)==16&&sizeof(ForceResult)==16&&sizeof(AvoidanceResult)==32&&sizeof(AvoidanceRequest)==24);
}
extern "C" {
// Original PhysicalObject::canCollide using its default onCollisionTest virtual
// implementation. 1 allowed, 0 denied, -1 invalid fields. Constructors/world
// body ownership and any custom virtual override are outside this view.
int dh2_nav_can_collide(const dh2::navigation::PhysicalContact*,const dh2::navigation::PhysicalContact*);
// 0 completed, 1 malformed caller, 2 insufficient bounded output/map storage.
// Recovered force sum and complete ordered contributions, optionally appended
// to an existing buffer. Original map::operator[] retains an empty floor key.
int dh2_nav_obstacle_force(dh2::navigation::ForceResult*,const dh2::navigation::AvoidanceRequest*);
// Recovered original PFWorld::AvoidObstacles. Does not validate floor bounds,
// apply speed/root motion or move actor state. Early flag/null-floor gates do
// not touch map keys. Malformed/capacity failures preserve caller state.
int dh2_nav_avoid_obstacles(dh2::navigation::AvoidanceResult*,float* direction,const dh2::navigation::AvoidanceRequest*);
}
