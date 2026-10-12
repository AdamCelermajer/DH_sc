#pragma once
#include "collision.hpp"
namespace dh2::octree {
struct Box {float minimum[3],maximum[3];};
struct Node {std::uint32_t first,count,children[8];Box box;};
struct Tree {
 Node* nodes;std::uint32_t* indices;std::uint32_t* scratch;const collision::Triangle* triangles;
 std::uint32_t triangle_count,node_count,index_count,node_capacity,index_capacity,scratch_capacity,leaf_limit,root,reserved;
};
static_assert(sizeof(Node)==64&&sizeof(Box)==24);
}
extern "C" {
// Caller owns bounded storage. 0 succeeds, 1 rejects inputs, 2 exhausts storage.
// Storage exhaustion exposes no tree (root/counts become zero); invalid inputs
// leave the previous tree unchanged. Finite geometry and
// a nonnegative leaf threshold mirror the authored floor-selector inputs.
int dh2_octree_build(dh2::octree::Tree*,const dh2::collision::Triangle*,std::uint32_t count,std::uint32_t leaf_limit);
// Returns selected count or UINT_MAX for invalid caller storage. Indices refer
// to input triangles; output follows parent-first/octant order and capacity.
// Storage and input geometry must remain alive and unchanged after building.
// Use capacity >= triangle_count for original full-output query behavior.
// Smaller capacities truncate safely; the legacy sibling overrun is not copied.
std::uint32_t dh2_octree_box(const dh2::octree::Tree*,const dh2::octree::Box*,std::uint32_t*,std::uint32_t capacity);
}
