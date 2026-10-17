#pragma once

#include <cstring>

namespace dh2::viewer {

// Material__11598's source SRenderState pass enables blending, selects
// GL_ONE/GL_ONE with GL_FUNC_ADD, and disables depth writes. Alpha-reference
// cutouts discard transparent texels before depth is written, so their
// visible texels must keep depth writes enabled. This is a narrow preview
// projection for SWAMP module zero, not a general material decoder.
inline bool swamp_material_uses_additive_one_one(const char* material_id) {
    return material_id && std::strcmp(material_id, "Material__11598") == 0;
}

inline bool swamp_draw_writes_depth(const char* material_id, bool alpha_cutout) {
    return alpha_cutout || !swamp_material_uses_additive_one_one(material_id);
}

// Keep the unresolved bridge-root diagnostic omission keyed to both source
// identities so similarly named nodes or materials elsewhere remain visible.
inline bool omit_unresolved_swamp_draw(const char* node_id, const char* material_id) {
    return node_id && material_id &&
        std::strcmp(node_id, "_module_obj_4of4_brdwalk_sw_00-node") == 0 &&
        std::strcmp(material_id, "ColorMaterial") == 0;
}

} // namespace dh2::viewer
