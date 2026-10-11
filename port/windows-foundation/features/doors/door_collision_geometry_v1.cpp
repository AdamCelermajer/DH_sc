#include "door_collision_geometry_v1.hpp"

#include "../../../level-world/character_scene.hpp"
#include "../../../level-world/decor_scene.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <stdexcept>

namespace dh::foundation::doors {

bool validate_source_door_apply_mesh_box_v1(const float absolute6[6], std::string& error) {
    error.clear();
    if (!absolute6) {
        error = "Source door ApplyMeshBox has no output bounds";
        return false;
    }
    for (const float value : std::array<float, 6>{absolute6[0], absolute6[1], absolute6[2],
                                                  absolute6[3], absolute6[4], absolute6[5]}) {
        if (!std::isfinite(value)) {
            error = "Source door ApplyMeshBox has a nonfinite extent";
            return false;
        }
    }
    // Source door PhysicalObject is a 2D polygon. ApplyMeshBox preserves Z,
    // but neither body construction nor collision admission requires positive Z.
    if (!(absolute6[0] < absolute6[3] && absolute6[1] < absolute6[4] && absolute6[2] <= absolute6[5])) {
        error = "Source door ApplyMeshBox has invalid XY or ordered-Z bounds";
        return false;
    }
    return true;
}

bool source_door_apply_mesh_box_v1(const AssetCatalog& assets,
                                   const CharacterVisual& visual,
                                   const float position[3],
                                   const float rotation_degrees[3],
                                   const float scale[3],
                                   float output_absolute6[6],
                                   std::string& error) {
    error.clear();
    try {
        const auto* scene = visual.retained_scene_borrow();
        const auto* config = visual.configuration();
        if (!visual.loaded() || !scene || !config || config->model_path.empty() ||
            !position || !rotation_degrees || !scale || !output_absolute6)
            throw std::runtime_error("Source door ApplyMeshBox requires its loaded visual, placement, and output");
        for (unsigned i = 0; i < 3; ++i) {
            if (!std::isfinite(position[i]) || !std::isfinite(rotation_degrees[i]) || !std::isfinite(scale[i]))
                throw std::runtime_error("Source door placement contains a nonfinite component");
        }

        const auto bytes = assets.read(config->model_path);
        dh2::resources::BresView view{};
        if (dh2_bres_open(&view, bytes.data(), bytes.size()) != dh2::resources::BresError::ok)
            throw std::runtime_error("Source door BRES rejected while projecting ApplyMeshBox");

        dh2::physical::DecorSceneInput placement{};
        std::copy_n(position, 3, placement.position);
        std::copy_n(rotation_degrees, 3, placement.rotation_degrees);
        std::copy_n(scale, 3, placement.scale);

        dh2::physical::DecorSceneMarker marker{};
        if (!dh2::physical::decor_scene_marker(view, *scene, marker, error))
            throw std::runtime_error("Source door _colbox_ marker: " + error);

        dh2::physical::DecorSceneOutput mesh_box{};
        if (marker.found) {
            std::copy_n(marker.bounds, 6, placement.marker_bounds);
            std::copy_n(marker.parent_scale, 3, placement.marker_parent_scale);
            if (dh2_decor_scene(&mesh_box, &placement))
                throw std::runtime_error("Source door marker ApplyMeshBox transform rejected");
        } else {
            std::vector<dh2::physical::CharacterMeshEntry> entries;
            if (!dh2::physical::character_scene_entries(view, *scene, entries, error))
                throw std::runtime_error("Source door CalcMeshBox providers: " + error);
            if (entries.empty())
                throw std::runtime_error("Source door CalcMeshBox has no mesh providers");
            dh2::physical::CharacterMeshBoxInput input{};
            input.entries = entries.data();
            input.count = static_cast<std::uint32_t>(entries.size());
            input.placement = placement;
            if (dh2_character_mesh_box(&mesh_box, &input))
                throw std::runtime_error("Source door fallback ApplyMeshBox calculation rejected");
        }

        // Source ApplyMeshBox publishes GameObject's padded relative and
        // absolute AABB (5 units of padding on each XY side when an extent is
        // below 10). It also constructs a physical config when _colbox_
        // exists. The typed kernel is pure; local non-null tokens satisfy its
        // input contract without issuing physical-world services.
        std::uint8_t owner_token = 0, physical_token = 0;
        dh2::physical::DecorBodyInput apply{};
        apply.owner = &owner_token;
        apply.new_physical = marker.found ? &physical_token : nullptr;
        apply.visual_present = 1;
        apply.colbox_found = marker.found;
        std::copy_n(mesh_box.mesh_box, 6, apply.mesh_box);
        std::copy_n(position, 3, apply.position);
        dh2::physical::DecorBodyConfig applied{};
        if (dh2_decor_body_config(&applied, &apply))
            throw std::runtime_error("Source door GameObject::ApplyMeshBox admission rejected");
        float absolute[6]{};
        std::copy_n(applied.absolute_box, 6, absolute);
        if (!validate_source_door_apply_mesh_box_v1(absolute, error))
            throw std::runtime_error(error);
        std::copy_n(absolute, 6, output_absolute6);
        error.clear();
        return true;
    } catch (const std::exception& e) {
        error = e.what();
        return false;
    }
}

} // namespace dh::foundation::doors
