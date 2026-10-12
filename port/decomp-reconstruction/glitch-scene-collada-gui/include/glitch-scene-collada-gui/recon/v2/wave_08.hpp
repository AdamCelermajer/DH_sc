// Wave 08 reconstruction: glitch::collada::animation_track light-colour
// interpreter helpers (slice_09 rows). Layouts come from
// glitch-scene-collada-gui/layouts/animation_track.hpp only.
//
// Guest model: the layout structs hold 32-bit guest addresses. The helpers below
// turn a guest word into a host pointer with guest_ptr(). This is valid only when
// the guest memory sits below 4 GiB on the host (the tests use MAP_32BIT).
#pragma once

#include <cstddef>
#include <cstdint>

#include "glitch-scene-collada-gui/layouts/animation_track.hpp"

namespace glitch::collada::recon_v2_08 {

// Keyframe entry, stride 28. Not in the layout headers; declared from the note on
// SAnimationAccessorTrack::keyframes (the uint32 at entry+0x18 is the output index).
struct SAnimationKeyframe {
    std::uint8_t pad_0x00[0x18];   // +0x00..+0x17 not read by these bodies
    std::uint32_t outputIndex;     // +0x18 index into the output table (getOutput)
};
static_assert(sizeof(SAnimationKeyframe) == 28, "keyframe stride is 28 bytes");
static_assert(offsetof(SAnimationKeyframe, outputIndex) == 0x18, "keyframe outputIndex");

// Output table entry, stride 8. getOutput returns the guest address
// outputs + 8 * outputIndex + 4, which is the address of the slot's first word
// here. The pseudocode then reads the word 4 bytes after that address (`dataWord`),
// which is the guest pointer to the byte rows.
struct SAnimationOutputSlot {
    std::uint32_t atReturned;      // +0x00 from the returned address
    std::uint32_t dataWord;        // +0x04 from the returned address; guest pointer to the rows
};
static_assert(sizeof(SAnimationOutputSlot) == 8, "output slot stride is 8 bytes");

// One light-colour row: three bytes (RGB). Stride 3.
struct SLightColorRow {
    std::uint8_t c[3];
};
static_assert(sizeof(SLightColorRow) == 3, "light colour row stride is 3 bytes");

// Guest address to host pointer. Caller must guarantee the address is valid.
template <typename T>
inline const T* guest_ptr(std::uint32_t addr) {
    return reinterpret_cast<const T*>(static_cast<std::uintptr_t>(addr));
}

// SAnimationAccessor::getOutput(int) at 0x669e24. Returns a guest pointer
// (outputs + 8 * keyframes[i].outputIndex + 4) as the layout note states.
const SAnimationOutputSlot* getOutput(const glitch::collada::SAnimationAccessor& a, int i);

// 0x6126c8 glitch::collada::animation_track::CInterpreter<CMixin<uchar,3,SLightColor,-1,uchar>,uchar,3,SUseDefaultLerp<uchar>>::getKeyBasedValueEx(SAnimationAccessor const&, int, int, float, void*)
std::uint32_t CInterpreter_SLightColor_getKeyBasedValueEx(const glitch::collada::SAnimationAccessor& a1, int a2, int a3, float a4, void* a5);

// 0x612820 glitch::collada::animation_track::CVirtualEx<CApplyValueEx<vector3d<float>,CLightColorMixin<uchar>>>::getKeyBasedValue(SAnimationAccessor const&, int, int, void*) const
std::uint32_t CVirtualEx_CLightColor_getKeyBasedValue_delta(const glitch::collada::SAnimationAccessor& a2, int a3, int a4, void* a5);

// 0x612874 glitch::collada::animation_track::CInterpreter<CMixin<uchar,3,SLightColor,-1,uchar>,uchar,3,SUseDefaultLerp<uchar>>::getKeyBasedValueEx(SAnimationAccessor const&, int, int, int, float, void*)
std::uint32_t CInterpreter_SLightColor_getKeyBasedValueEx_offset(const glitch::collada::SAnimationAccessor& a1, int a2, int a3, int a4, float a5, void* a6);

}  // namespace glitch::collada::recon_v2_08
