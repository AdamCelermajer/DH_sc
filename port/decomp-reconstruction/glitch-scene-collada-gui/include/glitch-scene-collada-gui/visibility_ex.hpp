// glitch::collada::animation_track::CVisibilityEx: the visibility animation
// track. Only the value accessors are reconstructed so far. The object's
// visible flag lives at original offset +0x11c (low bit).
#pragma once

#include <cstdint>

namespace glitch::collada::animation_track {

// Layout stand-in for the parts of CVisibilityEx used by the accessors below.
struct CVisibilityEx {
    // 0x6e4a24 _ZN6glitch7collada15animation_track13CVisibilityExD1Ev (empty)
    ~CVisibilityEx() = default;

    std::uint8_t pad_0000[0x11c];  // +0x000 (base track state, not yet mapped)
    std::uint32_t visible_flags;   // +0x11c
};

}  // namespace glitch::collada::animation_track

namespace glitch::collada::animation_track::visibility_ex {

// 0x6e493c _ZNK6glitch7collada15animation_track13CVisibilityEx13retrieveValueEPvS3_
void retrieveValue(const CVisibilityEx* self, void* /*unused*/, std::uint8_t* out);

// 0x6e494c _ZNK6glitch7collada15animation_track13CVisibilityEx12getValueSizeEv
int getValueSize();

// 0x6e4954 _ZNK6glitch7collada15animation_track13CVisibilityEx16getIdentityValueEPv
void getIdentityValue(const CVisibilityEx* self, std::uint32_t* out);

}  // namespace glitch::collada::animation_track::visibility_ex
