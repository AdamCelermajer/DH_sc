#include "destructible_container_visual_provider_v1.hpp"

#include "character_scene.hpp"
#include "decor_body_config.hpp"
#include "../engine-resources/resources.hpp"
#include "../scene-materials/scene.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <vector>

namespace dh2::destructible_container_visual_provider_v1 {
namespace {

std::string normalized_path(std::string_view input) {
    std::string result;
    result.reserve(input.size());
    for (const unsigned char ch : input) {
        if (ch == '/') result.push_back('/');
        else if (ch == '\\') result.push_back('/');
        else result.push_back(static_cast<char>(std::tolower(ch)));
    }
    while (result.compare(0, 2, "./") == 0) result.erase(0, 2);
    const auto data_root = result.find("data/");
    if (data_root != std::string::npos && data_root != 0)
        result.erase(0, data_root);
    return result;
}

std::uint32_t word(const std::uint8_t* bytes) noexcept {
    return std::uint32_t(bytes[0]) |
        (std::uint32_t(bytes[1]) << 8) |
        (std::uint32_t(bytes[2]) << 16) |
        (std::uint32_t(bytes[3]) << 24);
}

bool has_timeline_name(const resources::BresView& view,
                       const char* wanted) noexcept {
    const auto count = dh2_bres_library_count(
        &view, resources::Library::animation_clip);
    for (std::uint32_t index = 0; index < count; ++index) {
        const auto* row = dh2_bres_library_item(
            &view, resources::Library::animation_clip,
            static_cast<std::int32_t>(index));
        if (!row || view.size < 12) return false;
        const auto offset = word(row);
        if (offset >= view.size) return false;
        const auto* end = static_cast<const std::uint8_t*>(
            std::memchr(view.bytes + offset, 0, view.size - offset));
        if (!end || end == view.bytes + offset) return false;
        const std::string_view name(
            reinterpret_cast<const char*>(view.bytes + offset),
            static_cast<std::size_t>(end - (view.bytes + offset)));
        if (name == wanted) return true;
    }
    return false;
}

std::int32_t initial_container_state(const resources::BresView& view) noexcept {
    // Container::InitPost uses this exact priority, not clip order.
    if (has_timeline_name(view, "prespawn")) return 0;
    if (has_timeline_name(view, "spawn")) return 1;
    if (has_timeline_name(view, "idle")) return 2;
    return -1;
}

bool valid_source(const world::SourceObjectCensusV1& source) noexcept {
    if (source.gametype != "DestructibleContainer" ||
        source.template_name != "DestructibleContainer" ||
        source.data_desc_pydata != "DestructibleContainers" ||
        source.data_desc.empty() || source.name.empty() ||
        source.source_handle < 0 || source.duplicate_name) return false;
    for (unsigned axis = 0; axis < 3; ++axis) {
        if (!std::isfinite(source.world.position[axis]) ||
            !std::isfinite(source.world.rotation_degrees[axis]) ||
            !std::isfinite(source.world.scale[axis])) return false;
    }
    return true;
}

bool absolute_box(const resources::BresView& view,
                  const world::SourceObjectCensusV1& source,
                  destructible_container_source_owner_v1::AbsoluteAabbXY& out,
                  std::string& error) {
    scene::Scene loaded_scene;
    if (!scene::load(view, loaded_scene, error)) return false;

    std::vector<physical::CharacterMeshEntry> entries;
    if (!physical::character_scene_entries(view, loaded_scene, entries, error))
        return false;

    physical::DecorSceneInput placement{};
    std::copy(source.world.position.begin(), source.world.position.end(),
              placement.position);
    std::copy(source.world.rotation_degrees.begin(),
              source.world.rotation_degrees.end(), placement.rotation_degrees);
    std::copy(source.world.scale.begin(), source.world.scale.end(),
              placement.scale);
    const physical::CharacterMeshBoxInput mesh_input{
        entries.data(), static_cast<std::uint32_t>(entries.size()), 0,
        placement};
    physical::DecorSceneOutput mesh_output{};
    if (dh2_character_mesh_box(&mesh_output, &mesh_input) != 0) {
        error = "DestructibleContainer CalcMeshBox rejected the BDAE scene";
        return false;
    }

    // GameObject::SetRelativeAABB applies strict <10 unit XY padding, then
    // adds the owner position. This is the same shared source kernel used by
    // the already reconstructed decor and character paths.
    std::uint8_t owner_token = 0;
    physical::DecorBodyInput body_input{};
    body_input.owner = &owner_token;
    body_input.visual_present = 1;
    std::copy(mesh_output.mesh_box, mesh_output.mesh_box + 6,
              body_input.mesh_box);
    std::copy(source.world.position.begin(), source.world.position.end(),
              body_input.position);
    physical::DecorBodyConfig body_output{};
    if (dh2_decor_body_config(&body_output, &body_input) != 0) {
        error = "DestructibleContainer SetRelativeAABB rejected the mesh box";
        return false;
    }

    const auto* bounds = body_output.absolute_box;
    for (unsigned axis = 0; axis < 2; ++axis) {
        if (!std::isfinite(bounds[axis]) ||
            !std::isfinite(bounds[axis + 3]) ||
            bounds[axis + 3] < bounds[axis]) {
            error = "DestructibleContainer absolute AABB is invalid";
            return false;
        }
    }
    out = {bounds[0], bounds[1], bounds[3], bounds[4], 1};
    return true;
}

} // namespace

Status build_init_post_facts(
    const world::SourceObjectCensusV1& source,
    const destructible_container_table_v1::Tables& tables,
    std::string_view visual_source_path,
    const std::uint8_t* bdae, std::size_t bdae_size,
    std::uint8_t condition_enabled, std::uint8_t deleted_81,
    Facts& out, std::string& error) noexcept {
    out = {};
    error.clear();
    if (condition_enabled > 1 || deleted_81 > 1 || visual_source_path.empty()) {
        error = "DestructibleContainer visual provider arguments are invalid";
        return Status::invalid_argument;
    }
    if (!valid_source(source)) {
        error = "MGP row does not satisfy the DestructibleContainer source contract";
        return Status::invalid_source_contract;
    }

    std::int32_t data_id = -1;
    const auto* row = tables.find(source.data_desc, &data_id);
    const auto* resolved_name = tables.data_desc(data_id);
    if (!row || !resolved_name || *resolved_name != source.data_desc ||
        row->visual < 0) {
        error = "DestructibleContainer source table row or visual ID is unavailable";
        return Status::table_row_unavailable;
    }
    const auto* expected_path = tables.visual_path(row->visual);
    if (!expected_path || normalized_path(*expected_path) !=
                              normalized_path(visual_source_path)) {
        error = "Loaded BDAE path differs from the Visual ID selected by the source table row";
        return Status::visual_path_mismatch;
    }

    if (!bdae || !bdae_size) {
        error = "DestructibleContainer selected visual BDAE is missing";
        return Status::invalid_argument;
    }
    resources::BresView view{};
    if (dh2_bres_open(&view, bdae, bdae_size) != resources::BresError::ok) {
        error = "DestructibleContainer selected visual BDAE is malformed";
        return Status::bdae_invalid;
    }

    const auto state = condition_enabled
        ? initial_container_state(view)
        : std::int32_t{4};
    if (state < 0) {
        error = "BDAE timeline has no source prespawn, spawn, or idle clip";
        return Status::timeline_state_unresolved;
    }

    Facts facts{};
    facts.container_state_394 = state;
    facts.deleted_81 = deleted_81;
    facts.condition_enabled = condition_enabled;
    if (condition_enabled &&
        !absolute_box(view, source, facts.aabb, error)) {
        if (error.empty()) error = "DestructibleContainer ApplyMeshBox could not be reconstructed";
        return Status::mesh_box_unavailable;
    }
    out = facts;
    error.clear();
    return Status::complete;
}

} // namespace dh2::destructible_container_visual_provider_v1
