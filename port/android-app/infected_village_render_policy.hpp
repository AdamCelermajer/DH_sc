#pragma once

#include <cstring>

namespace dh2::infectedpreview {

// Keep the imported ColorMaterial source commands in the scene. Their roots
// are 12-triangle, roughly 6,000 x 12,000 x 6,000 unit enclosing bounds that
// occlude the actual descendants when rendered as opaque fallback color.
// Equivalent ColorMaterial records are identified as TemplateDefs or `_colbox`
// in the checked BDAE draw inventory. Since the original pass/visibility state
// is unresolved, omit just these two source-identified root guides from this
// diagnostic renderer instead of asserting a fabricated opaque material.
inline bool omit_unresolved_root_guide(const char* node_id, const char* material_id) {
    if (!node_id || !material_id || std::strcmp(material_id, "ColorMaterial") != 0)
        return false;
    return std::strcmp(node_id, "_module_infectedvillage_01-node") == 0 ||
           std::strcmp(node_id, "_module_infectedvillage_02-node") == 0;
}

// The two imported module floor surfaces currently have no sampler references
// in the selected BDAE material (Standard_8). They are retained in the scene
// and nav-source records; this exact-node rule is an experiment for the static
// renderer's flat fallback tint, not a claim that the game hides these floors.
inline bool omit_unresolved_floor_fallback(const char* node_id, const char* material_id) {
    if (!node_id || !material_id || std::strcmp(material_id, "Standard_8") != 0)
        return false;
    return std::strcmp(node_id, "_floor_infectedvillage_01-node_PIVOT") == 0 ||
           std::strcmp(node_id, "_floor_infectedvillage_02-node_PIVOT") == 0;
}

inline bool omit_diagnostic_draw(const char* node_id, const char* material_id) {
    return omit_unresolved_root_guide(node_id, material_id) ||
           omit_unresolved_floor_fallback(node_id, material_id);
}

} // namespace dh2::infectedpreview
