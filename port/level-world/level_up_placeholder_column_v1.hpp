#pragma once

// P16 LEVELUP4 PLACEHOLDER OVERRIDE. This is NOT original art.
//
// The original level-up light column (reference video v1.0.3, 769.03-769.53 s) is
// white and is cut hard about 0.43 s after the flash. The authored sheet
// `level_up.bdae` (`_7_-_Default1`) is gold and fades out at about 0.93 s, and no
// colour/bloom mechanism in the IDA or assets reproduces the white (see
// coordination/claude-preview16/LEVELUP4-report.md). Until the original is decoded,
// this record replaces the sheet look for FX set 135 (level_up) only:
//   - the sheet material colour is forced to `white` during [white_begin_ms, white_cut_ms)
//     measured from the FX play start (the flash moment);
//   - outside that window the gold sheet part is not drawn (suppressed), so the
//     gold colour track never shows while the override is active.
// The sheet geometry (camera-facing 200 x 1140 billboard from the feet upward) is
// kept as authored. The override is applied in CharacterMeshFxOwnerV4::draw_parts
// and matches the FX record by its asset uri. It is bound to set 135 in
// features/effects/runtime_level_up_presentation_v1.hpp.

#include "../engine-skinning/visual_skin_owner_v6.hpp"
#include "../scene-materials/scene.hpp"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

namespace dh2::fx {

struct LevelUpPlaceholderColumnV1 {
    // Asset uri of the FX set 135 record (matched case-insensitively, '\\' as '/').
    const char* fx_uri;
    // Authored gold sheet material id inside that asset.
    const char* sheet_material;
    // Window of the white phase, ms after the FX play start (reference 769.10 s .. 769.53 s).
    std::int32_t white_begin_ms;
    std::int32_t white_cut_ms;
    // Colour used for the white phase (RGBA, linear 0..1).
    float white[4];
    // Horizontal scale of the sheet (x about the sheet centre) during the white phase.
    // PLACEHOLDER: reference white row span ~150 px vs our authored 200-unit sheet ~60 px
    // at the same row (LEVELUP4 measurement, 769.33 s vs our f156), so 2.5.
    float width_scale;
};

// PLACEHOLDER values read from the reference video frames (see LEVELUP4 report, Placeholders).
inline constexpr LevelUpPlaceholderColumnV1 kLevelUpPlaceholderColumnV1{
    "data/3d/interface/level_up.bdae",
    "_7_-_Default1",
    100,
    430,
    {1.0f, 1.0f, 1.0f, 1.0f},
    2.5f,
};

namespace level_up_placeholder_detail_v1 {

inline bool same_asset_uri(const std::string& a, const char* b) {
    const std::size_t n = std::strlen(b);
    if (a.size() != n) return false;
    for (std::size_t i = 0; i < n; ++i) {
        char x = a[i] == '\\' ? '/' : a[i];
        char y = b[i] == '\\' ? '/' : b[i];
        if (std::tolower(static_cast<unsigned char>(x)) != std::tolower(static_cast<unsigned char>(y))) return false;
    }
    return true;
}

// Keeps the previous draw-part retention alive while the override owns a copied material table.
struct MaterialSnapshot {
    std::shared_ptr<const void> previous;
    std::vector<scene::Material> materials;
    std::shared_ptr<skinning::VisualGeometryV6> geometry;
};

} // namespace level_up_placeholder_detail_v1

// Applies the placeholder to ONE draw part of an FX record. `elapsed_ms` is the time
// since the FX play start. Returns false when the part must not be drawn (gold sheet
// outside the white window). A sheet part inside the window gets a snapshot material
// table with the sheet colour set to white; other parts are returned unchanged.
// No-op (true) for any FX whose uri is not the override uri.
inline bool level_up_placeholder_keep_part_v1(const std::string& uri, std::int32_t elapsed_ms,
                                              skinning::VisualDrawPartV6& part) {
    namespace detail = level_up_placeholder_detail_v1;
    const LevelUpPlaceholderColumnV1& o = kLevelUpPlaceholderColumnV1;
    if (!detail::same_asset_uri(uri, o.fx_uri)) return true;
    bool sheet = false;
    if (part.material_table && part.materials) {
        const auto& table = *part.material_table;
        for (auto index : *part.materials)
            if (index < table.size() && table[index].id == o.sheet_material) sheet = true;
    }
    if (!sheet) return true;
    if (!(elapsed_ms >= o.white_begin_ms && elapsed_ms < o.white_cut_ms)) return false; // suppressed
    auto snapshot = std::make_shared<detail::MaterialSnapshot>();
    snapshot->previous = part.retention;
    snapshot->materials = *part.material_table;
    for (auto& material : snapshot->materials)
        if (material.id == o.sheet_material) std::copy_n(o.white, 4, material.color);
    part.material_table = &snapshot->materials;
    if (o.width_scale != 1.0f && part.geometry) {
        snapshot->geometry = std::make_shared<skinning::VisualGeometryV6>(*part.geometry);
        for (auto& position : snapshot->geometry->positions) position[0] *= o.width_scale;
        for (auto& position : part.positions) position[0] *= o.width_scale;
        part.geometry = snapshot->geometry.get();
    }
    part.retention = snapshot;
    return true;
}

// Vector form used by CharacterMeshFxOwnerV4::draw_parts.
inline void apply_level_up_placeholder_v1(const std::string& uri, std::int32_t elapsed_ms,
                                          std::vector<skinning::VisualDrawPartV6>& parts) {
    std::vector<skinning::VisualDrawPartV6> kept;
    kept.reserve(parts.size());
    for (auto& part : parts)
        if (level_up_placeholder_keep_part_v1(uri, elapsed_ms, part)) kept.push_back(std::move(part));
    parts = std::move(kept);
}

} // namespace dh2::fx
