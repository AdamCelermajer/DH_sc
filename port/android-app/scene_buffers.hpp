#pragma once

#include "../engine-resources/resources.hpp"

#include <cstdint>

// A bounded, owner-independent projection of checked static BRES scene draws.
// The output owns its buffers and remains valid after the BRES input is freed.
namespace dh2::viewer {
enum class SceneMeshError : std::uint32_t {
    ok, argument, allocation, unsupported, limit, scene_walk, no_draw
};
struct SceneMesh {
    float* vertices;          // x, z, u, v per vertex; x/z normalized after walk.
    std::uint16_t* indices;   // Combined triangle list, offset per draw.
    std::uint32_t vertex_count, index_count, draw_commands;
    char first_diffuse_texture[96];
};
}

extern "C" dh2::viewer::SceneMeshError dh2_viewer_scene_mesh(
    dh2::viewer::SceneMesh* output, const dh2::resources::BresView* image);
extern "C" void dh2_viewer_scene_mesh_free(dh2::viewer::SceneMesh* output);
