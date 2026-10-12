#pragma once
#include <cstdint>

namespace dh2::navigation {
// Reconstructed PFFloor graph construction. Storage and floor collision queries
// belong to the caller; no original pointers, vtables or machine code are used.
struct Node {
 std::uint32_t id, floor;
 float position[3], direction[3], width, length;
 std::uint32_t parent, left, right, red;
};
struct Edge {std::uint32_t from, to;float distance, clearance, weight;};
struct InvalidNode {float position[3], source[3], destination[3];std::uint32_t neighbours[2];float normal[3];};
struct Triangle {float points[3][3];};
using FloorQuery=std::uint32_t (*)(void*,std::uint32_t,const float*);
struct Graph {
 Node* nodes;Edge* edges;InvalidNode* invalid;std::uint32_t* validation;
 std::uint32_t node_count, edge_count, invalid_count, validation_count;
 std::uint32_t node_capacity, edge_capacity, invalid_capacity, validation_capacity;
 std::uint32_t root, floor, floor_start, reserved;
 FloorQuery query;void* user;
};
// Each original PFFloor retains its own midpoint tree and invalid-node range.
// Node/edge identities remain shared. Bounds are the loader's expanded bounds.
struct FloorGraph {
 std::uint32_t floor, flags, root, start, invalid_first, invalid_count;
 float minimum[3], maximum[3];
};
struct BoundaryNode {std::uint32_t invalid, node;};
struct FloorLink {std::uint32_t from, to;};
struct LinkWorkspace {
 BoundaryNode* first;BoundaryNode* second;std::uint32_t capacity, reserved;
 std::uint32_t* validation_floors;FloorLink* links;
 std::uint32_t link_count, link_capacity;
};
static_assert(sizeof(Node)==56&&sizeof(Edge)==20&&sizeof(InvalidNode)==56&&sizeof(Triangle)==36);
static_assert(sizeof(FloorGraph)==48&&sizeof(BoundaryNode)==8&&sizeof(FloorLink)==8);
}
extern "C" {
// Returns 0 on success. 1 rejects caller storage/request, 2 rejects insufficient
// storage before any writes. Start each distinct PFFloor with begin_floor.
int dh2_nav_begin_floor(dh2::navigation::Graph*,std::uint32_t floor);
int dh2_nav_triangle(dh2::navigation::Graph*,const dh2::navigation::Triangle*,std::uint32_t flags);
// Reconstructed PFFloor::_Link. Original midpoint trees, link order and flag
// ownership are preserved. Caller bounds/storage/ID layouts are modern adapters.
// Returns 1 for malformed input, 2 for insufficient storage before mutations.
int dh2_nav_link(dh2::navigation::Graph*,dh2::navigation::FloorGraph*,dh2::navigation::FloorGraph*,dh2::navigation::LinkWorkspace*);
// Original PostLoad/room-link overlap uses inclusive comparisons and float
// addition/subtraction in this order. Room pairs use margin 50, same-room 0.
int dh2_nav_bounds_overlap(const float* minimum_a,const float* maximum_a,const float* minimum_b,const float* maximum_b,float margin);
}
