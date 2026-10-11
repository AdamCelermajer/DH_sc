#pragma once

// B076: the level's dynamic point light. The original static environment materials use the shader family
// GL_Diffuse_L1_* (shaders.pak GL_Diffuse_L1_iPhone_VS.glsl): vertex colour = (Light0.ambient + Light0.diffuse * max(0,N.L)
// * attenuation) * Color.rgb, with Light0 = the level LightSet's PlayerLight (a point light attached to the Player at
// attachedOffset). The LightSet file is named by the level's LevelConfig (`light_set`, ES1.1 fallback `fixed_light_set`).
// Data-driven: nothing here names a level; the light constants come from that file (units after LightBase::InitPost:
// colours /255, attenuation y/1000 and z/1000000).
#include "../../asset_catalog.hpp"

#include <array>
#include <string>

namespace dh::foundation::level_lighting {

struct LevelPlayerLightV1 {
    bool present = false;
    std::array<float, 3> attachedOffset{};
    std::array<float, 3> attenuation{1, 0, 0};
    std::array<float, 3> ambient{}, diffuse{}, specular{};
    std::string lightSetUri;
    // LevelConfig clear_color (floats 0..1), the framebuffer clear of the level (the Swamp declares black).
    bool hasClearColor = false;
    std::array<float, 3> clearColor{};
};

// Reads the LevelConfig of `levelUri`, loads its light set and returns the light attached to "Player".
// present=false (and true returned) when the level names no light set or it holds no Player light.
bool read_level_player_light_v1(const AssetCatalog& assets, const std::string& levelUri, bool fixedFunctionSet,
                                LevelPlayerLightV1& out, std::string& error);

} // namespace dh::foundation::level_lighting
