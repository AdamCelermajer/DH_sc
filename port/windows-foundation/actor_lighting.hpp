#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace dh::foundation {

// Source LightBase properties before InitPost's unit conversion.
struct AuthoredPointLight {
    std::string name, attachedTo;
    std::array<float,3> position{}, attachedOffset{}, attenuation{};
    std::array<float,3> ambient{}, diffuse{}, specular{};
    float radius = 2.0f;
    bool automatic = false;
};

struct SourceLight {
    std::array<float,4> position{0,0,0,1};
    std::array<float,3> attenuation{};
    std::array<float,4> ambient{0,0,0,1}, diffuse{0,0,0,1}, specular{0,0,0,1};
    float radius = 2.0f;
};

// Bounded Module/LightPoint asset adapter; no automatic named-LightSet assignment.
bool parseAuthoredPointLights(std::string_view xml, std::vector<AuthoredPointLight>& output,
                             std::string& error);
SourceLight normalizeSourceLight(const AuthoredPointLight& authored);

// Matches the recovered four named sets with five retained slots. Their names
// do not imply a color or default light. Constructor slots are genuinely null.
enum class SourceLightSetId : std::uint32_t { Player=0, Scene=1, Camera=2, Monster=3 };
struct SourceLightSets {
    std::array<std::array<std::optional<SourceLight>,5>,4> sets;
    std::array<std::optional<SourceLight>,5> dummyOff;
};
struct SourceLightBinding {
    std::uint32_t shaderSlot = 0, sourceSlot = 0;
    bool dummy = false;
    std::optional<SourceLight> light;
};

// ApplySettings0x40c9a8 considers slots0..3, compresses available enabled lights
// into light0..3 parameters, then fills disabled slots from dummyOff. Enabled
// null lights are skipped; disabled null dummy parameters are still written.
bool planSourceLightBindings(const SourceLightSets&, SourceLightSetId,
                             const std::array<bool,5>& filter,
                             const std::array<bool,4>& shaderParameterPresent,
                             std::vector<SourceLightBinding>& output, std::string& error);

enum class SourceVertexLighting { CommonUnlit, CommonLit, DiffuseL1VertexColor, Unsupported };
// Must receive the ACTUAL selected shader/preamble, not an inferred model name.
SourceVertexLighting classifySourceVertexLighting(std::string_view filename,
                                                 std::string_view callerPreamble);

struct SourceCommonMaterialLighting {
    std::array<float,4> emission{}, ambient{}, diffuse{}, specular{}, sceneAmbient{};
    float shininess = 0;
};

// CPU reference for the actual ProfileCOMMON_emul_VS.glsl LIGHTING branch.
// Inputs are already in source world space; source transformed normals are NOT
// normalized again. This reference is not evidence that a particular actor
// selected that branch, nor GLES precision/framebuffer equivalence.
bool evaluateSourceCommonVertexLight(const SourceLight&, const SourceCommonMaterialLighting&,
                                    const std::array<float,3>& worldPosition,
                                    const std::array<float,3>& transformedNormal,
                                    std::array<float,4>& output, std::string& error);

} // namespace dh::foundation
