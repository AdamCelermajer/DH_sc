#pragma once
#include "octree.hpp"
namespace dh2::selector {
// Column-major original matrix arithmetic, with its explicit identity hint.
// The hint controls shortcuts; it must agree with the authored matrix state.
struct Matrix {float values[16];std::uint32_t identity;};
struct Selector {const octree::Tree* tree;const Matrix* node_transform;std::uint32_t geometry_transformed,reserved;};
struct Workspace {std::uint32_t* indices;collision::Triangle* triangles;std::uint32_t capacity,reserved;};
struct Floor {Selector selector;octree::Box bounds;Workspace* workspace;};
struct FloorSet {const Floor* floors;std::uint32_t count,reserved;};
static_assert(sizeof(Matrix)==68);
}
extern "C" {
// 1 inverted, 0 singular (unchanged), -1 invalid caller arguments (unchanged).
int dh2_selector_inverse(dh2::selector::Matrix*);
// Output is in Workspace::triangles, in original selector order. Caller owns
// storage with capacity >= tree triangle_count. UINT_MAX rejects arguments.
// Node transforms are ignored when geometry_transformed is set, as originally.
std::uint32_t dh2_selector_triangles(dh2::selector::Workspace*,const dh2::selector::Selector*,const dh2::octree::Box*,const dh2::selector::Matrix* extra);
// Compose original selector -> octree -> collision manager -> triangle tests.
// Result index is the selected output-list index, not the raw mesh index.
int dh2_selector_raycast(dh2::collision::Result*,const dh2::selector::Selector*,dh2::selector::Workspace*,const dh2::collision::Ray*);
int dh2_selector_floor(dh2::collision::Result*,const dh2::selector::Floor*,const float* point);
std::uint32_t dh2_selector_floor_query(void*,std::uint32_t floor,const float* point);
}
