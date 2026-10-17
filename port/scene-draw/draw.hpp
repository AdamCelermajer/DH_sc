#pragma once

#include "../scene-payloads/scene.hpp"
#include "../asset-payloads/payloads.hpp"
#include "../material-bindings/bindings.hpp"

// A new, renderer-neutral bridge from checked scene records to static triangle
// draw descriptors. It does not recreate the original engine's draw ABI.
namespace dh2::draw {
enum class Error : std::uint32_t {
    ok, argument, scene, visual, instance, mesh, primitive, material,
    node_limit, draw_limit, callback_stopped
};

struct Command {
    dh2::math::Matrix4f world; // Column-major static local-to-world matrix.
    const char* node_id;       // Borrowed from the input BRES.
    const char* geometry_id;   // Borrowed from the input BRES.
    const char* material_id;   // Borrowed; may have no matching material record.
    std::uint32_t visual_index, node_record, visible, vertex_count;
    std::uint32_t index_count, index_width;
    std::int32_t geometry_index, primitive_index, material_index;
};

struct Stats {
    std::uint32_t visual_references, visual_scenes, nodes, instances;
    std::uint32_t geometry_instances, resolved_geometry, skipped_nonvisual_references;
    std::uint32_t skipped_unresolved_visuals, skipped_unresolved_geometry;
    std::uint32_t skipped_unsupported_geometry, skipped_nontriangle_primitives;
    std::uint32_t unresolved_materials, draw_commands;
    std::uint64_t triangles;
};

// The callback receives a stack-local Command. Copy any fields needed later;
// borrowed strings remain valid only while the BRES input remains alive.
using Callback = bool (*)(const Command*, void*);
}

extern "C" dh2::draw::Error dh2_static_scene_draws(
    dh2::draw::Stats* stats, const dh2::resources::BresView* image,
    dh2::draw::Callback callback, void* user,
    std::uint32_t max_nodes, std::uint32_t max_draws);
