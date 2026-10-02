#pragma once
#include "../engine-resources/resources.hpp"
#include "../engine-math/math.hpp"

// Immutable views of serialized Collada scene records. Input bytes must outlive
// every view. These interfaces are not the original engine's ARM32 object ABI.
namespace dh2::scene {
using dh2::resources::BresView;
enum class Error : std::uint32_t {
    ok, argument, range, string, nonfinite, unsupported_reference,
    nonaffine, walk_limit, walk_cycle, walk_stopped
};
struct Scene {
    BresView image;
    std::uint32_t references, reference_offset;
    std::uint32_t visuals, visual_offset;
};
struct Reference {
    std::uint32_t type;
    const char* url;
};
struct Visual {
    BresView image;
    const char* id;
    const char* name;
    std::uint32_t roots, root_offset;
};
struct Node {
    BresView image;
    const char* id;
    const char* name;
    std::uint32_t record, children, child_offset, instances, instance_offset;
    std::uint32_t visible, extension_offset;
    float position[3], rotation[4], scale[3];
};
struct Instance {
    std::uint32_t type, payload_offset;
    // Only the type-3 geometry URL layout is exposed; other instance kinds
    // remain typed opaque records until their layouts are reconstructed.
    const char* geometry_url;
};
using WalkCallback = bool (*)(const Node*, const dh2::math::Matrix4f*,
                              std::uint32_t depth, void* user);
}

extern "C" {
dh2::scene::Error dh2_scene_open(dh2::scene::Scene*, const dh2::resources::BresView*);
dh2::scene::Error dh2_scene_reference(const dh2::scene::Scene*, std::int32_t,
                                      dh2::scene::Reference*);
dh2::scene::Error dh2_scene_visual(const dh2::scene::Scene*, std::int32_t,
                                   dh2::scene::Visual*);
dh2::scene::Error dh2_scene_root_node(const dh2::scene::Visual*, std::int32_t,
                                      dh2::scene::Node*);
dh2::scene::Error dh2_scene_child_node(const dh2::scene::Node*, std::int32_t,
                                       dh2::scene::Node*);
dh2::scene::Error dh2_scene_instance(const dh2::scene::Node*, std::int32_t,
                                     dh2::scene::Instance*);
// Resolve local #ID references; -1 means absent, external or unresolved.
std::int32_t dh2_scene_visual_index(const dh2::scene::Scene*, const char* url);
std::int32_t dh2_scene_geometry_index(const dh2::scene::Scene*, const dh2::scene::Instance*);
// Node-local rotation uses the original quaternion getMatrix_transposed
// convention, followed by column-wise postScale and translation.
dh2::scene::Error dh2_scene_local_matrix(const dh2::scene::Node*,
                                         dh2::math::Matrix4f*);
// A null parent is identity. Output may alias parent.
dh2::scene::Error dh2_scene_world_matrix(const dh2::scene::Node*,
                                         const dh2::math::Matrix4f* parent,
                                         dh2::math::Matrix4f*);
// Preorder walk with bounded depth (64) and caller-supplied node limit.
dh2::scene::Error dh2_scene_walk_visual(const dh2::scene::Visual*,
                                        dh2::scene::WalkCallback, void* user,
                                        std::uint32_t max_nodes);
}
