#pragma once
#include "world.hpp"
#include "../scene-payloads/scene.hpp"

namespace dh2::world {
// Copyable binding; no borrowed scene strings or buffer ownership. All nine
// SWAMP placements use translation-only roots. Unsupported transforms fail.
struct ModuleBinding {
    std::uint32_t visual_index, node_record;
    float catalogue_origin[3], placement_delta[3];
};
}
extern "C" {
dh2::world::Error dh2_world_bind_module(dh2::world::ModuleBinding*,
    const dh2::world::Module*, const dh2::scene::Scene*, dh2::world::Diagnostic*);
// Preorder IDs of the root and every descendant, without geometry. On failure
// *count is zero; output may contain an incomplete prefix. Capacity is bounded
// to 65536 and the entire visual walk uses the same checked node limit.
dh2::world::Error dh2_world_module_records(std::uint32_t* records,
    std::uint32_t capacity, std::uint32_t* count,
    const dh2::world::ModuleBinding*, const dh2::scene::Scene*,
    dh2::world::Diagnostic*);
// Left-multiply this translation correction by a catalogue descendant matrix.
dh2::world::Error dh2_world_placement_matrix(dh2::math::Matrix4f*,
    const dh2::world::ModuleBinding*, dh2::world::Diagnostic*);
// Input is the existing scene walk/draw matrix of a descendant of this module.
// Caller must select that subtree using binding.node_record. This helper does
// not verify descendant membership or create/flatten any geometry.
dh2::world::Error dh2_world_place_matrix(dh2::math::Matrix4f*,
    const dh2::world::ModuleBinding*, const dh2::math::Matrix4f*,
    dh2::world::Diagnostic*);
}
