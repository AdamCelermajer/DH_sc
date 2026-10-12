// glitch::collada::animation_track::CTextureTransformEx: texture-transform
// animation track. The value is five 32-bit words (20 bytes): a 2-D translation
// (words 0-1), a rotation word (word 2) and scale words (words 3-4). Only the
// value-type helpers and the no-op key-based overloads are reconstructed here.
#pragma once

#include <cstddef>
#include <cstdint>

namespace glitch::collada {
struct SAnimationAccessor;  // declared by the animation accessor module
}

namespace glitch::collada::animation_track {

// Track object. Only its empty destructor is mapped; the track base is not.
class CTextureTransformEx {
public:
    // 0x6e3708 _ZN6glitch7collada15animation_track19CTextureTransformExD1Ev (empty)
    ~CTextureTransformEx() = default;
};

// 0x6e33f4 _ZNK6glitch7collada15animation_track19CTextureTransformEx13retrieveValueEPvS3_
// Original body is empty.
void CTextureTransformEx_retrieveValue(const void* self, void* in, void* out);

// 0x6e33f8 _ZNK6glitch7collada15animation_track19CTextureTransformEx12getValueSizeEv
inline std::size_t CTextureTransformEx_getValueSize() { return 0x14; }

// 0x6e3400 _ZNK6glitch7collada15animation_track19CTextureTransformEx16getIdentityValueEPv
// Writes translation (0, 0), rotation 0, scale (1, 1).
void CTextureTransformEx_getIdentityValue(std::uint32_t* out);

// 0x6e36f8 .. 0x6e3704: key-based overloads with empty bodies in the original.
void CTextureTransformEx_getKeyBasedValue_iifPv(const SAnimationAccessor& accessor, int a, int b, float c, void* out);  // 0x6e36f8
void CTextureTransformEx_getKeyBasedValue_iPv(const SAnimationAccessor& accessor, int a, void* out);                  // 0x6e36fc
void CTextureTransformEx_getKeyBasedValue_iiifPv(const SAnimationAccessor& accessor, int a, int b, int c, float d, void* out);  // 0x6e3700
void CTextureTransformEx_getKeyBasedValue_iiPv(const SAnimationAccessor& accessor, int a, int b, void* out);          // 0x6e3704

}  // namespace glitch::collada::animation_track
