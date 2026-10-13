// Wave 08 reconstruction: glitch::collada::animation_track light-colour
// interpreter helpers (slice_09 rows). The rows are the data of output record 0 of the canonical SAnimationAccessor
// (collada/SAnimationAccessor.hpp).
#pragma once

#include <cstddef>
#include <cstdint>

#include "glitch-scene-collada-gui/layouts/animation_track.hpp"

namespace glitch::collada::recon_v2_08 {

// One light-colour row: three bytes (RGB). Stride 3.
struct SLightColorRow {
    std::uint8_t c[3];
};
static_assert(sizeof(SLightColorRow) == 3, "light colour row stride is 3 bytes");

// 0x6126c8 glitch::collada::animation_track::CInterpreter<CMixin<uchar,3,SLightColor,-1,uchar>,uchar,3,SUseDefaultLerp<uchar>>::getKeyBasedValueEx(SAnimationAccessor const&, int, int, float, void*)
std::uint32_t CInterpreter_SLightColor_getKeyBasedValueEx(const glitch::collada::SAnimationAccessor& a1, int a2, int a3, float a4, void* a5);

// 0x612820 glitch::collada::animation_track::CVirtualEx<CApplyValueEx<vector3d<float>,CLightColorMixin<uchar>>>::getKeyBasedValue(SAnimationAccessor const&, int, int, void*) const
// Returns the output record, as the original returns its address.
const glitch::collada::SAnimationOutput* CVirtualEx_CLightColor_getKeyBasedValue_delta(const glitch::collada::SAnimationAccessor& a2, int a3, int a4, void* a5);

// 0x612874 glitch::collada::animation_track::CInterpreter<CMixin<uchar,3,SLightColor,-1,uchar>,uchar,3,SUseDefaultLerp<uchar>>::getKeyBasedValueEx(SAnimationAccessor const&, int, int, int, float, void*)
std::uint32_t CInterpreter_SLightColor_getKeyBasedValueEx_offset(const glitch::collada::SAnimationAccessor& a1, int a2, int a3, int a4, float a5, void* a6);

}  // namespace glitch::collada::recon_v2_08
