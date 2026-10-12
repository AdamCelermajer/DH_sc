#pragma once
#include "floor_source.hpp"
#include "navigation.hpp"
#include "navigation_search.hpp"
#include "navigation_world.hpp"
#include "navigation_path.hpp"
#include "navigation_motion.hpp"
#include "../scene-materials/scene.hpp"
#include <memory>
namespace dh2::floors {
// Own all collision, octree, query and graph storage. Records have stable
// addresses; neither APK assets nor original ABI service pointers are retained.
struct Record {
 std::string name;unsigned room=0,geometry=0;
 selector::Matrix clone{};floor_source::Flags flags{};octree::Box local{},world{},bounds{};
 std::vector<collision::Triangle> triangles,retained,selected;
 std::vector<octree::Node> octants;std::vector<unsigned> indices,scratch,selected_ids;
 octree::Tree tree{};selector::Workspace workspace{};
 Record()=default;Record(const Record&)=delete;Record& operator=(const Record&)=delete;
};
struct World {
 std::vector<std::unique_ptr<Record>> records;std::vector<selector::Floor> selectors;
 std::vector<navigation::Node> nodes;std::vector<navigation::Edge> edges;
 std::vector<navigation::InvalidNode> invalid;std::vector<unsigned> validation;
 std::vector<navigation::FloorGraph> floor_graphs;std::vector<unsigned> validation_floors;
 std::vector<navigation::BoundaryNode> first_boundary,second_boundary;
 std::vector<navigation::FloorLink> links;navigation::LinkWorkspace sewing{};bool sewn=false;
 std::vector<unsigned> search_edges,search_offsets;
 std::vector<navigation::SearchNode> search_nodes;
 std::vector<navigation::SearchEntry> search_heap;
 std::vector<std::vector<unsigned>> collision_room_floors;
 std::vector<navigation::CollisionRoom> collision_rooms;
 std::vector<navigation::CollisionFloor> collision_floors;
 std::vector<navigation::FloorTraits> route_traits;
 std::vector<navigation::FailedRoute> failed_routes;std::vector<unsigned> route_internal_path;
 navigation::CollisionWorld collision_world{};navigation::SearchGraph route_graph{};
 navigation::RouteWorld route_world{};navigation::SearchWorkspace route_search{};navigation::RouteWorkspace route_workspace{};
 navigation::Graph graph{};
 World()=default;World(const World&)=delete;World& operator=(const World&)=delete;
};
// Supports authored floor nodes with no floortypes override. Full
// CStrProps decoding remains pending and explicitly rejects such overrides.
bool append(const resources::BresView&,const scene::Scene&,const scene::Instance&,unsigned room,World&,std::string&);
bool build_graph(World&,std::string&);
bool post_load(World&,std::string&);
// Graph-node search only. The original world endpoint/radius/obstacle wrappers
// and movement controller are not supplied by this adapter. World scratch is
// reused sequentially; caller predicates and result/path storage remain owned.
int search_nodes(World&,unsigned start,unsigned limit,const navigation::SearchTest&,navigation::SearchResult&);
int route(World&,const float* source,const float* target,unsigned limit,navigation::RouteObject*,navigation::RouteResult&);
int find_path(World&,navigation::PathObject&,const float* target,unsigned limit,navigation::RouteResult&);
// Original failed-pair cache invalidation/lifecycle still needs reconstruction.
// Explicitly clear when a caller starts a new validation session or changes
// graph/capability state; gameplay pursuit has not been attached to this API.
void clear_route_cache(World&);
bool height(const World&,const float* point,float&);
}
