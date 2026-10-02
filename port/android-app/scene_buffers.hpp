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
struct SceneMesh {
    float* vertices;          // x, y, z, u, v per vertex; XYZ normalized after walk.
    std::uint16_t* indices;   // Combined triangle list, offset per draw.
    std::uint32_t vertex_count, index_count, draw_commands;
    std::uint32_t skin_joints; // Nonzero for the first-controller pose fallback.
    char first_diffuse_texture[96];
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
