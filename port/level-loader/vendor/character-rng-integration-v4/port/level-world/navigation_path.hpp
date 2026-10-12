#pragma once
#include "navigation_world.hpp"
namespace dh2::navigation {
// Original Point2D line classification. Parallel branches leave point and
// parameters untouched; finite arithmetic uses individually rounded floats.
struct LineIntersection {float point[2],first,second;std::uint32_t kind,reserved;};
// Graph edges retain stable IDs. Zero denotes PFObject's embedded direct edge;
// UINT_MAX denotes the single temporary direct edge owned by SmoothPath.
// Graph coordinates are materialized by the ownership adapter: the graph must
// remain immutable throughout this path's lifetime.
struct PathSegment {
 std::uint32_t edge,from,to;float weight,distance,clearance,source[3],target[3];
};
struct PathObject {
 RouteObject route;float position[3],target[3],waypoint[3];
 PathSegment* segments;std::uint32_t count,capacity,owned,reserved;
};
struct FindRequest {
 RouteWorld* world;const CollisionWorld* geometry;PathObject* object;
 RouteResult* result;RouteWorkspace* workspace;
 float target[3];std::uint32_t limit,pathfinding_enabled,reserved;
};
struct FindSourceServicesV1 {
 void* context{};
 // Source Debug/profile/module delivery AFTER DropPath and target store.
 // 0 delivered with actual module word0/1; nonzero reached required failure.
 int(*search_enabled)(void*,std::uint32_t*){};
};
struct MoveResult {std::uint32_t active,past;float target[3];std::uint32_t reserved;};
static_assert(sizeof(LineIntersection)==24&&sizeof(PathSegment)==48&&sizeof(PathObject)==96&&sizeof(FindRequest)==64&&sizeof(MoveResult)==24);
}
extern "C" {
int dh2_nav_line_intersection(dh2::navigation::LineIntersection*,const float*,const float*,const float*,const float*);
// Returns 0 completed, 1 malformed caller data. Empty paths are rejected by
// smooth/calc/past as required by the original assertions. Drop/length/move
// support empty paths. Owned storage/destruction are bounded native adapters.
int dh2_nav_smooth_path(dh2::navigation::PathObject*,const dh2::navigation::Graph*);
int dh2_nav_calc_waypoint(dh2::navigation::PathObject*);
int dh2_nav_past_waypoint(std::uint32_t*,const dh2::navigation::PathObject*);
int dh2_nav_drop_path(dh2::navigation::PathObject*);
int dh2_nav_path_length(float*,const dh2::navigation::PathObject*);
int dh2_nav_move_path(dh2::navigation::MoveResult*,dh2::navigation::PathObject*,const dh2::navigation::Graph*);
// Recovered FindPath with profiling/timing disabled. Uses caller route scratch,
// requires capacity for node_count+1 segments/edge IDs before dropping old path.
// Returns 0 completed (result.found is original bool), 1 malformed, 2 capacity.
int dh2_nav_find_path(const dh2::navigation::FindRequest*);
// Additive source-live policy boundary; historical FindRequest behavior stays.
// Returns3 on a reached policy failure, preserving DropPath/target prefix.
int dh2_nav_find_path_source_v1(const dh2::navigation::FindRequest*,const dh2::navigation::FindSourceServicesV1*);
}
