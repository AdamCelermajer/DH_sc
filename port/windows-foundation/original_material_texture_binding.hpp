#pragma once
#include "original_scene.hpp"
#include <functional>

namespace dh::foundation {
using OriginalTextureLookup = std::function<std::uint32_t(const std::string&, const std::string&, bool)>;

// Transport the same decoded material through either geometry owner. The
// original L1 fragment shader samples AlphaSampler.z for Al and At variants;
// specular eligibility/map/glossiness are resolved by the scene decoder.
inline void bind_original_material_textures(const OriginalMaterial& source,
                                           Material& target,
                                           const OriginalTextureLookup& lookup) {
    const bool blueAlpha = !source.alphaMap.empty() && source.effectFile.rfind("GL_Diffuse_L1", 0)==0;
    const auto diffuse = lookup(source.diffuse, source.alphaMap, blueAlpha);
    const auto specular = source.specularMap.empty()?0:lookup(source.specularMap, {}, false);
    // A required mask/lookup exception must not publish a half-bound material.
    target.texture = diffuse;
    target.specularTexture = specular;
    target.glossiness = source.glossiness;
}
}
