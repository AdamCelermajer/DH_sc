// glitch::collada::animation_track::CInterpreter<CSceneNodeScaleMixin<float>, float, 3, SUseDefaultLerp<float> >
//
// Declared for Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CInterpreter_glit-0
// (the batch id is shared by many entries; this header holds the 0x628850 entry only). The class is not
// declared anywhere else in glitch-scene-collada-gui, so this header holds only the explicit specialization
// this batch needs. The primary template is declared here with its four parameters; other specializations
// belong to their own batches.
//
// The class is stateless: the accessor is the first argument and there is no this pointer, so the method
// is static.
//
// Offsets are the original 32-bit ARM offsets, kept as comments only.
#pragma once

#include <cstdint>

namespace glitch::collada {

struct SAnimationAccessor;  // full layout in layouts/animation_track.hpp; not included here

namespace animation_track {

template <typename T>
class CSceneNodeScaleMixin;

template <typename T>
struct SUseDefaultLerp;

// Primary template: layout and other specializations are declared by their own batches.
template <typename Mixin, typename Key, int Dim, typename Default>
class CInterpreter;

template <>
class CInterpreter<CSceneNodeScaleMixin<float>, float, 3, SUseDefaultLerp<float>> {
public:
    // 0x628850 _ZN6glitch7collada15animation_track12CInterpreterINS1_20CSceneNodeScaleMixinIfEEfLi3ENS1_15SUseDefaultLerpIfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
    // Linear blend of the 3-float scale key at `key` toward the key at `key + 1` by weight t (see the .cpp).
    // Writes x, y, z to out[0..2] and returns the x component. The third argument is in the ABI but not read.
    static float getKeyBasedValueEx(const SAnimationAccessor& accessor, int key, int unusedIndex, float t,
                                    void* out);
};

}  // namespace animation_track

// Accessor output table entry for this interpreter (SAnimationAccessor::getOutput result). The key data
// pointer is at +0x04. Only the data pointer is read by this body. Named apart from the quaternion batch's
// SAnimationOutput so both headers can be included in one translation unit.
struct SScaleAnimationOutput {
    std::uint32_t reserved_0x00;  // +0x00 not read here
    void* data;                   // +0x04 key data, 12-byte scale keys (3 floats) for this track
};

namespace tb_ext {

// SAnimationAccessor::getOutput(int) const, 0x669e24. Declared only; defined outside this batch.
SScaleAnimationOutput* scaleAccessorGetOutput(const SAnimationAccessor& accessor, int index);

}  // namespace tb_ext

}  // namespace glitch::collada
