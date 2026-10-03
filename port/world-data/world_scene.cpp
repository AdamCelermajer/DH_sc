#include "world_scene.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>

namespace {
using namespace dh2::world;
Error fail(Diagnostic* d, Error e, const char* message) {
    if (d) { *d = {}; d->error = e; std::snprintf(d->message, sizeof d->message, "%s", message); }
    return e;
}
struct Collect {
    std::uint32_t* records;
    std::uint32_t capacity, count, selected, depth;
    bool active, found, overflow;
};
bool collect(const dh2::scene::Node* node, const dh2::math::Matrix4f*,
             std::uint32_t depth, void* user) {
    auto& state = *static_cast<Collect*>(user);
    if (state.active && depth <= state.depth) state.active = false;
    if (node->record == state.selected) {
        if (state.found) return false;
        state.active = state.found = true; state.depth = depth;
    }
    if (state.active) {
        if (state.count == state.capacity) { state.overflow = true; return false; }
        state.records[state.count++] = node->record;
    }
    return true;
}
}
extern "C" Error dh2_world_bind_module(ModuleBinding* output, const Module* module,
    const dh2::scene::Scene* scene, Diagnostic* d) {
    if (!output || !module || !scene || !module->catalogue_node_id)
        return fail(d, Error::argument, "Missing module/scene binding input");
    const auto& transform = module->record.local;
    for (unsigned i = 0; i < 3; ++i)
        if (!std::isfinite(transform.position[i]) || transform.rotation_degrees[i] != 0 || transform.scale[i] != 1)
            return fail(d, Error::unsupported_transform, "Module placement must be finite translation only");
    ModuleBinding candidate{}; bool found = false;
    for (std::uint32_t v = 0; v < scene->visuals; ++v) {
        dh2::scene::Visual visual{};
        if (dh2_scene_visual(scene, static_cast<std::int32_t>(v), &visual) != dh2::scene::Error::ok)
            return fail(d, Error::scene, "Invalid visual scene");
        for (std::uint32_t r = 0; r < visual.roots; ++r) {
            dh2::scene::Node node{};
            if (dh2_scene_root_node(&visual, static_cast<std::int32_t>(r), &node) != dh2::scene::Error::ok)
                return fail(d, Error::scene, "Invalid catalogue root");
            if (!node.id || std::strcmp(node.id, module->catalogue_node_id)) continue;
            if (found) return fail(d, Error::ambiguous_node, "Catalogue root ID is not unique");
            if (node.rotation[0] != 0 || node.rotation[1] != 0 || node.rotation[2] != 0 ||
                std::fabs(node.rotation[3]) != 1)
                return fail(d, Error::unsupported_transform, "Catalogue root rotation is not identity");
            for (unsigned i = 0; i < 3; ++i) {
                if (node.scale[i] != 1 || !std::isfinite(node.position[i]))
                    return fail(d, Error::unsupported_transform, "Catalogue root is not finite/unit scale");
                candidate.catalogue_origin[i] = node.position[i];
                candidate.placement_delta[i] = transform.position[i] - node.position[i];
                if (!std::isfinite(candidate.placement_delta[i]))
                    return fail(d, Error::unsupported_transform, "Module placement delta overflow");
            }
            candidate.visual_index = v; candidate.node_record = node.record; found = true;
        }
    }
    if (!found) return fail(d, Error::missing_node, "Module root is absent from catalogue roots");
    *output = candidate; if (d) *d = {}; return Error::ok;
}
extern "C" Error dh2_world_module_records(std::uint32_t* records,
    std::uint32_t capacity, std::uint32_t* count, const ModuleBinding* binding,
    const dh2::scene::Scene* scene, Diagnostic* d) {
    if (count) *count = 0;
    if (!records || !count || !binding || !scene || !capacity || capacity > 65536 ||
        binding->visual_index >= scene->visuals)
        return fail(d, Error::argument, "Invalid module record collection input");
    dh2::scene::Visual visual{};
    if (dh2_scene_visual(scene, static_cast<std::int32_t>(binding->visual_index), &visual) != dh2::scene::Error::ok)
        return fail(d, Error::scene, "Invalid module visual scene");
    Collect state{records, capacity, 0, binding->node_record, 0, false, false, false};
    const auto result = dh2_scene_walk_visual(&visual, collect, &state, 65536);
    if (state.overflow) return fail(d, Error::limit, "Module record output capacity exceeded");
    if (result != dh2::scene::Error::ok) return fail(d, Error::scene, "Invalid module subtree walk");
    if (!state.found) return fail(d, Error::missing_node, "Module binding record absent from scene");
    *count = state.count; if (d) *d = {}; return Error::ok;
}
extern "C" Error dh2_world_placement_matrix(dh2::math::Matrix4f* output,
    const ModuleBinding* binding, Diagnostic* d) {
    if (!output || !binding) return fail(d, Error::argument, "Missing placement correction input");
    dh2::math::Matrix4f candidate{};
    candidate.m[0] = candidate.m[5] = candidate.m[10] = candidate.m[15] = 1;
    for (unsigned i = 0; i < 3; ++i) {
        if (!std::isfinite(binding->placement_delta[i]))
            return fail(d, Error::unsupported_transform, "Nonfinite placement correction");
        candidate.m[12 + i] = binding->placement_delta[i];
    }
    candidate.identity_hint = 0; *output = candidate;
    if (d) *d = {};
    return Error::ok;
}
extern "C" Error dh2_world_place_matrix(dh2::math::Matrix4f* output,
    const ModuleBinding* binding, const dh2::math::Matrix4f* original, Diagnostic* d) {
    if (!output || !binding || !original) return fail(d, Error::argument, "Missing placement matrix input");
    auto placed = *original;
    for (float value : placed.m)
        if (!std::isfinite(value)) return fail(d, Error::unsupported_transform, "Nonfinite scene matrix");
    if (placed.m[3] != 0 || placed.m[7] != 0 || placed.m[11] != 0 || placed.m[15] != 1)
        return fail(d, Error::unsupported_transform, "Scene matrix is not affine");
    for (unsigned i = 0; i < 3; ++i) {
        placed.m[12 + i] += binding->placement_delta[i];
        if (!std::isfinite(placed.m[12 + i])) return fail(d, Error::unsupported_transform, "Placed scene matrix overflow");
    }
    placed.identity_hint = 0; *output = placed; if (d) *d = {}; return Error::ok;
}
