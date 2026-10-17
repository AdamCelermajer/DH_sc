#pragma once

#include "../../android-app/scene_buffers.hpp"
#include "../../scene-materials/technique_selector.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string_view>
#include <vector>

namespace dh2::irrlicht_swamp {

inline bool source_texture_path_is(const char* source_path,
                                   const char* expected_path) {
    if (!source_path || !expected_path) return false;
    constexpr char prefix[] = "q:/data/iphone/3d/";
    if (std::strncmp(source_path, prefix, sizeof(prefix) - 1) == 0)
        source_path += sizeof(prefix) - 1;
    return std::strcmp(source_path, expected_path) == 0;
}

inline bool is_swamp_diffuse_reference(
    const viewer::SceneTextureReference& reference) {
    return reference.image_index >= 0 &&
        (std::strcmp(reference.parameter_id, "Diffuse") == 0 ||
         std::strcmp(reference.parameter_id, "diffuse-sampler") == 0) &&
        source_texture_path_is(reference.source_path, "textures/env_swamp.tga");
}

inline bool is_swamp_alpha_map_reference(
    const viewer::SceneTextureReference& reference) {
    return reference.image_index >= 0 &&
        std::strcmp(reference.parameter_id, "AlphaMap") == 0 &&
        source_texture_path_is(reference.source_path,
                               "textures/pvr2_env_swamp_alpha.tga");
}

enum class AlphaMapMode : std::uint8_t {
    no_alpha_variant,
    source_al_fractional,
    source_at_cutout,
    unsupported,
};

struct AlphaMapSelection {
    AlphaMapMode mode = AlphaMapMode::unsupported;
    std::uint32_t profile_count = 0;
};

inline bool selector_component_is(std::string_view selector,
                                  std::size_t index,
                                  std::string_view expected) {
    std::size_t component_index = 0;
    std::size_t begin = 0;
    while (begin <= selector.size()) {
        const auto end = selector.find('_', begin);
        const auto component = selector.substr(begin,
            end == std::string_view::npos ? selector.size() - begin : end - begin);
        if (component_index == index) return component == expected;
        ++component_index;
        if (end == std::string_view::npos) break;
        begin = end + 1;
    }
    return false;
}

// CurrentTechnique's selector component 2 is the source effect's alpha
// variant. Classify only the checked L1_Vc selector family and exact Al/At
// tokens; do not infer a mode from material names or sampler presence.
inline AlphaMapMode alpha_map_mode_from_selector(std::string_view selector) {
    if (!selector_component_is(selector, 0, "L1") ||
        !selector_component_is(selector, 1, "Vc"))
        return AlphaMapMode::unsupported;
    if (selector_component_is(selector, 2, "Al"))
        return AlphaMapMode::source_al_fractional;
    if (selector_component_is(selector, 2, "At"))
        return AlphaMapMode::source_at_cutout;
    if (selector_component_is(selector, 2, "----"))
        return AlphaMapMode::no_alpha_variant;
    return AlphaMapMode::unsupported;
}

// Source/runtime profile and compiled renderer ordinal remain unknown.
// Accept an alpha mode only when every serialized known profile agrees.
inline AlphaMapSelection resolve_alpha_map_selection(
    const std::vector<dh2::scene_materials::CurrentTechnique>& selectors) {
    AlphaMapSelection result{};
    if (selectors.empty()) return result;
    bool saw_al = false;
    bool saw_at = false;
    bool saw_no_alpha = false;
    bool saw_gles = false;
    bool saw_gles2 = false;
    for (const auto& item : selectors) {
        if (item.profile == "GLES") {
            if (saw_gles) return result;
            saw_gles = true;
        } else if (item.profile == "GLES2") {
            if (saw_gles2) return result;
            saw_gles2 = true;
        } else {
            return result;
        }
        const auto mode = alpha_map_mode_from_selector(item.name);
        if (mode == AlphaMapMode::unsupported) return result;
        saw_al = saw_al || mode == AlphaMapMode::source_al_fractional;
        saw_at = saw_at || mode == AlphaMapMode::source_at_cutout;
        saw_no_alpha = saw_no_alpha || mode == AlphaMapMode::no_alpha_variant;
        ++result.profile_count;
    }
    if ((saw_al && (saw_at || saw_no_alpha)) ||
        (saw_at && saw_no_alpha)) return {};
    if (saw_al) result.mode = AlphaMapMode::source_al_fractional;
    else if (saw_at) result.mode = AlphaMapMode::source_at_cutout;
    else if (saw_no_alpha) result.mode = AlphaMapMode::no_alpha_variant;
    else return {};
    return result;
}

inline const char* alpha_map_mode_name(AlphaMapMode mode) {
    switch (mode) {
    case AlphaMapMode::no_alpha_variant: return "no-alpha-variant";
    case AlphaMapMode::source_al_fractional: return "source-AL-fractional";
    case AlphaMapMode::source_at_cutout: return "source-AT-cutout";
    case AlphaMapMode::unsupported: return "unsupported";
    }
    return "unsupported";
}

inline bool swamp_draw_has_alpha_map_pair(
    const viewer::SceneDrawDescriptor& draw,
    const viewer::SceneTextureReference* references,
    std::uint32_t reference_count) {
    (void)draw;
    if (!references || !reference_count)
        return false;
    bool diffuse = false;
    bool alpha_map = false;
    for (std::uint32_t i = 0; i < reference_count; ++i) {
        diffuse = diffuse || is_swamp_diffuse_reference(references[i]);
        alpha_map = alpha_map || is_swamp_alpha_map_reference(references[i]);
    }
    return diffuse && alpha_map;
}

inline bool swamp_draw_uses_alpha_map_mode(
    const viewer::SceneDrawDescriptor& draw,
    const viewer::SceneTextureReference* references,
    std::uint32_t reference_count, AlphaMapMode mode) {
    return swamp_draw_has_alpha_map_pair(draw, references, reference_count) &&
        (mode == AlphaMapMode::source_al_fractional ||
         mode == AlphaMapMode::source_at_cutout);
}

inline AlphaMapMode alpha_map_mode_for_material(
    std::int32_t material_index, const std::vector<AlphaMapMode>& modes) {
    return material_index >= 0 &&
            static_cast<std::size_t>(material_index) < modes.size()
        ? modes[static_cast<std::size_t>(material_index)]
        : AlphaMapMode::unsupported;
}

// The recovered source fragment reads the AlphaMap's blue channel and writes
// it to diffuse alpha. Preserve diffuse RGB and use that exact channel rather
// than the decoder's unrelated AlphaMap alpha byte.
inline bool apply_swamp_alpha_map(std::uint8_t* diffuse_rgba,
                                  const std::uint8_t* alpha_rgba,
                                  std::size_t pixel_count) {
    if (pixel_count && (!diffuse_rgba || !alpha_rgba)) return false;
    for (std::size_t pixel = 0; pixel < pixel_count; ++pixel)
        diffuse_rgba[pixel * 4 + 3] = alpha_rgba[pixel * 4 + 2];
    return true;
}

} // namespace dh2::irrlicht_swamp
