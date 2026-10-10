#pragma once

#include "renderer.hpp"
#include <filesystem>
#include <string>

namespace dh::foundation {

// One entry per draw range. Keep original texture names for the asset boundary
// rather than assigning GPU texture handles in the decoder.
struct OriginalMaterial {
    std::string id, diffuse, alphaMap, effectFile, technique;
    std::array<float,16> textureMatrix{};
    float alphaReference = 0;
    bool additive = false;
};

struct OriginalScene {
    Mesh mesh;
    std::vector<OriginalMaterial> materials;
    Vec3 minimum{}, maximum{};
    std::size_t nodeCount = 0, instanceCount = 0, triangleCount = 0;
    std::string source;
    std::vector<std::string> notices;
};

// Decodes visible static visual geometry with the original scene transforms baked
// into its vertices. Rejects skinned instances instead of showing a wrong pose.
// Exported module bounds, navigation, exit, minimap and collision helpers are
// separate roles and are excluded from visual draw ranges, with notices.
// This loads one authored BDAE scene, not procedural level/module placement.
bool decode_original_scene(const std::vector<std::uint8_t>& bytes,
                           OriginalScene& output, std::string& error);
bool decode_original_scene_module(const std::vector<std::uint8_t>& bytes,
                                  const std::string& authoredNode,
                                  const Mat4& placement,
                                  OriginalScene& output, std::string& error);
bool load_original_scene(const std::filesystem::path& path,
                         OriginalScene& output, std::string& error);

} // namespace dh::foundation
