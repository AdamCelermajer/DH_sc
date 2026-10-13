// Wave 08 reconstruction: glitch::collada::animation_track light-colour
// interpreter bodies (slice_09 rows 0x6126c8, 0x612820, 0x612874).
// Reads the rows through the canonical SAnimationAccessor. Bodies follow the Hex-Rays pseudocode.

#include "glitch-scene-collada-gui/recon/v2/wave_08.hpp"

namespace glitch::collada::recon_v2_08 {

// 0x6126c8 glitch::collada::animation_track::CInterpreter<...SLightColor...>::getKeyBasedValueEx(SAnimationAccessor const&, int, int, float, void*)
// Blends rows a2 and a2 + 1 with weights (1 - a4) and a4. The a3 argument is not read.
std::uint32_t CInterpreter_SLightColor_getKeyBasedValueEx(const SAnimationAccessor& a1, int a2, int /*a3*/, float a4, void* a5) {
    const auto* rows = static_cast<const SLightColorRow*>(a1.getOutput(0)->data);
    const SLightColorRow* src[2] = {rows + a2, rows + a2 + 1};
    const float w[2] = {1.0f - a4, a4};
    float acc[3] = {0.0f, 0.0f, 0.0f};
    for (int k = 0; k < 2; ++k) {
        for (int j = 0; j < 3; ++j) {
            acc[j] = static_cast<float>(src[k]->c[j]) * w[k] + acc[j];
        }
    }
    auto* out = static_cast<std::uint8_t*>(a5);
    for (int j = 0; j < 3; ++j) {
        out[j] = static_cast<std::uint8_t>(static_cast<std::uint32_t>(acc[j]));
    }
    return static_cast<std::uint32_t>(acc[2]);
}

// 0x612820 glitch::collada::animation_track::CVirtualEx<...>::getKeyBasedValue(SAnimationAccessor const&, int, int, void*) const
// Writes row a4 minus row a3 into a5 and returns the output slot address.
const SAnimationOutput* CVirtualEx_CLightColor_getKeyBasedValue_delta(const SAnimationAccessor& a2, int a3, int a4, void* a5) {
    const SAnimationOutput* slot = a2.getOutput(0);
    const auto* rows = static_cast<const SLightColorRow*>(slot->data);
    auto* out = static_cast<std::uint8_t*>(a5);
    for (int i = 0; i < 3; ++i) {
        out[i] = static_cast<std::uint8_t>(rows[a4].c[i] - rows[a3].c[i]);
    }
    return slot;
}

// 0x612874 glitch::collada::animation_track::CInterpreter<...SLightColor...>::getKeyBasedValueEx(SAnimationAccessor const&, int, int, int, float, void*)
// Lerps row a3 toward row a4 by a5, then stores (lerp - row a2) per component.
// Returns the last component before the subtraction, as the pseudocode does.
std::uint32_t CInterpreter_SLightColor_getKeyBasedValueEx_offset(const SAnimationAccessor& a1, int a2, int a3, int a4, float a5, void* a6) {
    const auto* rows = static_cast<const SLightColorRow*>(a1.getOutput(0)->data);
    const SLightColorRow& delta = rows[a2];
    const SLightColorRow& from = rows[a3];
    const SLightColorRow& to = rows[a4];
    auto* out = static_cast<std::uint8_t*>(a6);
    std::uint32_t result = 0;
    for (int i = 0; i < 3; ++i) {
        const float blended = static_cast<float>(from.c[i]) +
                              a5 * static_cast<float>(static_cast<int>(to.c[i]) - static_cast<int>(from.c[i]));
        result = static_cast<std::uint32_t>(blended);
        out[i] = static_cast<std::uint8_t>(result - delta.c[i]);
    }
    return result;
}

}  // namespace glitch::collada::recon_v2_08
