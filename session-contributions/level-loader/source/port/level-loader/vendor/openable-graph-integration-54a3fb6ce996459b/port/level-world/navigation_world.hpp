#pragma once
#include "navigation_search.hpp"
#include "selector.hpp"
namespace dh2::navigation {
struct FloorTraits {std::uint32_t type,object;};
struct CollisionRoom {octree::Box bounds;const std::uint32_t* floors;std::uint32_t count,reserved;};
struct CollisionFloor {const selector::Floor* selector;FloorTraits traits;};
struct CollisionWorld {const CollisionRoom* rooms;const CollisionFloor* floors;std::uint32_t room_count,floor_count;octree::Box bounds;};
struct WorldHit {std::uint32_t hit,room,floor,reserved;collision::Result collision;};
struct FailedRoute {std::uint32_t source,target,limit;};
struct RouteWorld {
 const SearchGraph* graph;const FloorGraph* floors;const FloorTraits* traits;
 std::uint32_t floor_count,reserved;FailedRoute* failed;std::uint32_t failed_count,failed_capacity;
};
// Original PFObject capability mask/radius and its owned direct-edge endpoints.
struct RouteObject {std::uint32_t flags;float radius,direct_source[3],direct_target[3];};
struct RouteRequest {
 RouteWorld* world;const CollisionWorld* geometry;RouteObject* object;
 float source[3],target[3];std::uint32_t limit,pathfinding_enabled,output_requested,reserved;
};
struct RouteResult {std::uint32_t found,source,target,kind;SearchResult search;};
struct RouteWorkspace {SearchWorkspace* search;std::uint32_t* internal_path;std::uint32_t internal_capacity,reserved;};
static_assert(sizeof(CollisionRoom)==40&&sizeof(CollisionFloor)==16&&sizeof(CollisionWorld)==48);
static_assert(sizeof(WorldHit)==72&&sizeof(RouteWorld)==48&&sizeof(RouteRequest)==64&&sizeof(RouteResult)==56&&sizeof(RouteWorkspace)==24);
}
extern "C" {
// Recovered PFWorld/PFRoom first-hit floor traversal. The selector/collision
// chain executes natively; scene/room grouping and ownership belong to caller.
// 1 hit, 0 miss, -1 invalid caller data. Miss preserves point/triangle bytes.
int dh2_nav_world_collision(dh2::navigation::WorldHit*,const dh2::navigation::CollisionWorld*,const float*,std::uint32_t include_special);
// Recovered PFWorld::_SearchGraph position-to-position wrapper. 0 completed,
// 1 invalid caller contract, 2 insufficient bounded storage. Graphs/trees must
// be well formed. Edge ID zero represents the PFObject-owned direct edge.
// Result.kind: 0 failed, 1 direct, 2 graph. Search stats are diagnostic output.
// This does not implement FindPath smoothing, obstacle avoidance or MovePath.
int dh2_nav_route(dh2::navigation::RouteResult*,const dh2::navigation::RouteRequest*,dh2::navigation::RouteWorkspace*);
}
