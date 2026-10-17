#pragma once

#include "../engine-resources/resources.hpp"
#include "../animation-pose/pose.hpp"
#include "../animation-layers/layers.hpp"

#include <cstdint>

// A bounded, owner-independent projection of checked static BRES scene draws.
// The output owns its buffers and remains valid after the BRES input is freed.
namespace dh2::viewer {
enum class SceneMeshError : std::uint32_t {
    ok, argument, allocation, unsupported, limit, scene_walk, no_draw
};
// Owned metadata for one flattened triangle draw. Offsets address SceneMesh's
// combined buffers; strings and sampler paths are copied from the BRES, so a
// renderer may release the source image after assembly.
struct SceneTextureReference {
    std::int32_t image_index;
    char parameter_id[96];
    char image_id[128];
    char image_name[128];
    char source_path[256];
};
struct SceneDrawDescriptor {
    std::uint32_t first_vertex, vertex_count;
    std::uint32_t first_index, index_count;
    std::uint32_t visual_index, node_record, visible;
    std::int32_t geometry_index, primitive_index, material_index;
    std::uint32_t first_texture, texture_count;
    char node_id[160];
    char geometry_id[160];
    char material_id[160];
    char material_name[160];
    char external_effect_file[256];
    char effect_url[160];
};
struct SceneMesh {
    float* vertices;          // x, y, z, u, v per vertex; XYZ normalized after walk.
    std::uint16_t* indices;   // Combined triangle list, offset per draw.
    std::uint32_t vertex_count, index_count, draw_commands;
    std::uint32_t skin_joints; // Nonzero for the first-controller pose fallback.
    char first_diffuse_texture[96];
    // Capacity is tracked so large original modules can grow to their actual
    // payload size without reserving the same maximum for every animation frame.
    std::uint32_t vertex_capacity, index_capacity;
    SceneDrawDescriptor* draws;
    SceneTextureReference* texture_references;
    std::uint32_t draw_capacity, texture_reference_count, texture_reference_capacity;
};
}

extern "C" dh2::viewer::SceneMeshError dh2_viewer_scene_mesh(
    dh2::viewer::SceneMesh* output, const dh2::resources::BresView* image);
extern "C" void dh2_viewer_scene_mesh_free(dh2::viewer::SceneMesh* output);
extern "C" dh2::viewer::SceneMeshError dh2_viewer_scene_mesh_at(
    dh2::viewer::SceneMesh*, const dh2::resources::BresView*,
    const dh2::pose::Clip*, std::int32_t milliseconds);
extern "C" dh2::viewer::SceneMeshError dh2_viewer_scene_mesh_layers(
    dh2::viewer::SceneMesh*, const dh2::resources::BresView*,
    const dh2::layers::Layers*);

// World renderer projections keep the checked original coordinates. An
// optional node prefix selects one module from a BRES catalogue. The viewer's
// existing normalized APIs above remain unchanged.
extern "C" dh2::viewer::SceneMeshError dh2_world_scene_mesh(
    dh2::viewer::SceneMesh*, const dh2::resources::BresView*, const char* node_prefix);
extern "C" dh2::viewer::SceneMeshError dh2_world_scene_mesh_at(
    dh2::viewer::SceneMesh*, const dh2::resources::BresView*,
    const dh2::pose::Clip*, std::int32_t milliseconds);

// Assemble only the selected source skin controller's mesh using its bind
// palette or the supplied source clip. Modular actors also expose unrelated
// static scene records, so diagnostics must explicitly select their controller.
extern "C" dh2::viewer::SceneMeshError dh2_world_scene_skin_mesh_at(
    dh2::viewer::SceneMesh*, const dh2::resources::BresView*,
    const dh2::pose::Clip*, std::int32_t milliseconds, const char* controller_id);

// Assemble one selected original module from its complete node-record list.
// A source-derived correction matrix replaces the catalogue placement with
// the level placement while retaining every descendant transform.
extern "C" dh2::viewer::SceneMeshError dh2_world_scene_mesh_nodes(
    dh2::viewer::SceneMesh*, const dh2::resources::BresView*,
    const std::uint32_t* node_records, std::uint32_t node_count,
    const dh2::math::Matrix4f* placement_correction);
