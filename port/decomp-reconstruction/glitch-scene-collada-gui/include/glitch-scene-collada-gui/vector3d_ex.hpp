// glitch::collada::animation_track::CVector3dEx: vector-valued animation
// track. Value is three 32-bit floats (12 bytes). Only the value-type helpers
// that do not need SAnimationAccessor are reconstructed here.
#pragma once

#include <cstddef>
#include <cstring>

namespace glitch::collada::animation_track {

// 0x6e42b4 _ZNK6glitch7collada15animation_track11CVector3dEx12getValueSizeEv
inline std::size_t CVector3dEx_getValueSize() { return 3 * sizeof(float); }

struct CVector3dEx {
    // Track base state is not yet mapped; the value helpers below do not use it.
    virtual ~CVector3dEx() = default;
    virtual std::size_t getValueSize() const { return CVector3dEx_getValueSize(); }  // vtable +8
};

// 0x6e42bc _ZNK6glitch7collada15animation_track11CVector3dEx16getIdentityValueEPv
void CVector3dEx_getIdentityValue(float* out);

// 0x6e4420 _ZNK6glitch7collada15animation_track11CVector3dEx10applyValueEPvS3_PNS1_15CApplicatorInfoE
void CVector3dEx_applyValue(const CVector3dEx* self, const void* value, void* target);

// 0x6e3fb0 _ZN6glitch7collada15animation_track11CVector3dEx17getBlendedValueExEPvPfiS3_
// Partial: the original reads the key block (12-byte keys) from the object pointer. The
// CVector3dEx layout maps only the vptr, so the key block is passed as `keys` (3 floats per key).
// Return value for count not 2 and not >= 3 is the original's this-pointer bits; here 0.0f.
float CVector3dEx_getBlendedValueEx(const float* keys, const float* weights, int count, float* out);

// 0x6e41ec _ZN6glitch7collada15animation_track11CVector3dEx17getBlendedValueExEPvPfiS3_f
// Partial: same key-block layout gap as the 4-argument overload.
float CVector3dEx_getBlendedValueEx(const float* keys, const float* weights, int count, float* out, float factor);

}  // namespace glitch::collada::animation_track
