#pragma once
#include "selector.hpp"
#include "../asset-payloads/payloads.hpp"
namespace dh2::floor_source {
// Borrowed checked mesh streams. Original selector indices are unsigned 16-bit.
// draw_count is the triangle-list draw count, including nonindexed streams.
struct Part {assets::Attribute position;const void* indices;std::uint32_t draw_count,primitive_type;};
struct Flags {std::uint32_t floor,object;};
}
extern "C" {
// _LoadNavMesh sets source position from its cached absolute position before
// CopyMeshSceneNode reads local position/rotation/scale into a parentless node.
int dh2_floor_clone_matrix(dh2::selector::Matrix*,const float* absolute_position3,const float* local_quaternion4,const float* local_scale3);
// _LoadNavMesh raises its separate retained triangle array after graph build.
// Selector collision geometry stays unraised. Exact in-place use is supported.
int dh2_floor_raise_triangles(dh2::collision::Triangle*,const dh2::collision::Triangle*,std::uint32_t count);
// Reconstruct mesh-buffer order, supported position formats, reversed winding
// and optional constructor baking. Does not filter degenerate/vertical faces.
// 0 success, 1 invalid input, 2 insufficient storage. Rejects without writes.
int dh2_floor_mesh_triangles(dh2::collision::Triangle*,std::uint32_t capacity,std::uint32_t* count,const dh2::floor_source::Part*,std::uint32_t parts,const dh2::selector::Matrix* node,std::uint32_t bake);
// Original _LoadNavMesh substring flags and original node world bounds +/-1000
// Z expansion. Caller supplies the node's world box and initial flag words.
int dh2_floor_source_flags(dh2::floor_source::Flags*,const char* tags,std::uint32_t length);
int dh2_floor_source_bounds(dh2::octree::Box*,const dh2::octree::Box* world);
// Original dirty world-box producer (transformBoxEx), including its float
// association. The explicit matrix identity hint does not skip this operation.
int dh2_floor_transform_bounds(dh2::octree::Box*,const dh2::octree::Box* local,const dh2::selector::Matrix*);
}
