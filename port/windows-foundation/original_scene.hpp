#pragma once

#include "renderer.hpp"
#include <filesystem>
#include <memory>
#include <string>

namespace dh::foundation {

// One entry per draw range. Keep original texture names for the asset boundary
// rather than assigning GPU texture handles in the decoder.
struct OriginalMaterial {
    std::string id, diffuse, alphaMap, effectFile, technique;
    std::array<float,16> textureMatrix{};
    float alphaReference = 0;
    bool additive = false;
    // B076: GL_Diffuse_L1 "Specular" sampler (image basename) when the GLES2 technique has the Sp slot; glossiness = exponent.
    std::string specularMap;
    float glossiness = 0;
};

struct OriginalScene {
    Mesh mesh;
    std::vector<OriginalMaterial> materials;
    Vec3 minimum{}, maximum{};
    // Map/RoomZone bounds: world AABB of the exported `_module_` helper geometry
    // (the module's own room box). Present only when the module exports one.
    bool hasModuleBounds = false;
    Vec3 moduleMinimum{}, moduleMaximum{};
    // B072: Map-page geometry (SceneManager::AddNodeToMap, IDA 0x3524a0, caller PFWorld 0x5206xx): per module the exported
    // `_minimap` nodes when the module has any, else its `_floor` navigation nodes. Position-only, world space, one DrawRange
    // (default material) per decoded module part; the Map page draws it flat. Never part of the visual draw ranges.
    Mesh mapMesh;
    std::size_t nodeCount = 0, instanceCount = 0, triangleCount = 0;
    std::string source;
    std::vector<std::string> notices;
    // P17 PROCGEN2: the LevelConfig skybox (data/3D/skybox/*.bdae), loaded with the level and drawn at the eye with
    // depth writes off. Null when the level declares none or it could not be loaded (then a notice says why).
    std::shared_ptr<OriginalScene> skybox;
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
