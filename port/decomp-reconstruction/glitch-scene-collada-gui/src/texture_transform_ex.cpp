#include "glitch-scene-collada-gui/texture_transform_ex.hpp"

#include <cstring>

namespace glitch::collada::animation_track {

// 0x6e33f4 _ZNK6glitch7collada15animation_track19CTextureTransformEx13retrieveValueEPvS3_
void CTextureTransformEx_retrieveValue(const void* /*self*/, void* /*in*/, void* /*out*/) {}

// 0x6e3400 _ZNK6glitch7collada15animation_track19CTextureTransformEx16getIdentityValueEPv
void CTextureTransformEx_getIdentityValue(std::uint32_t* out) {
    const float one = 1.0f;
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
    std::memcpy(&out[3], &one, sizeof(float));
    std::memcpy(&out[4], &one, sizeof(float));
}

// 0x6e36f8 _ZNK6glitch7collada15animation_track19CTextureTransformEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
void CTextureTransformEx_getKeyBasedValue_iifPv(const SAnimationAccessor&, int, int, float, void*) {}

// 0x6e36fc _ZNK6glitch7collada15animation_track19CTextureTransformEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
void CTextureTransformEx_getKeyBasedValue_iPv(const SAnimationAccessor&, int, void*) {}

// 0x6e3700 _ZNK6glitch7collada15animation_track19CTextureTransformEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
void CTextureTransformEx_getKeyBasedValue_iiifPv(const SAnimationAccessor&, int, int, int, float, void*) {}

// 0x6e3704 _ZNK6glitch7collada15animation_track19CTextureTransformEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
void CTextureTransformEx_getKeyBasedValue_iiPv(const SAnimationAccessor&, int, int, void*) {}

}  // namespace glitch::collada::animation_track
