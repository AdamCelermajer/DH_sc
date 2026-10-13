// glitch::collada::animation_track::CInterpreter<CSceneNodeQuaternionMixin<float>, float, 4, SUseDefaultLerp<float> >
//
// Declared for Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CInterpreter_glit-0.
// The class is not declared anywhere else in glitch-scene-collada-gui, so this header holds only the
// explicit specialization that the batch needs. The primary template is declared here with its four
// parameters; its other specializations belong to their own batches.
//
// Offsets are the original 32-bit ARM offsets, kept as comments only.
#pragma once

#include <cstdint>

#include "glitch-scene-collada-gui/collada/SAnimation.hpp"

namespace glitch::core {
class quaternion;
}  // namespace glitch::core

namespace glitch::collada {

struct SAnimationAccessor;  // full layout in layouts/animation_track.hpp; not included here

namespace animation_track {

template <typename T>
class CSceneNodeQuaternionMixin;

template <typename T>
struct SUseDefaultLerp;

// Primary template: layout and other specializations are declared by their own batches.
template <typename Mixin, typename Key, int Dim, typename Default>
class CInterpreter;

template <>
class CInterpreter<CSceneNodeQuaternionMixin<float>, float, 4, SUseDefaultLerp<float>> {
public:
    // 0x613294 _ZN6glitch7collada15animation_track12CInterpreterINS1_25CSceneNodeQuaternionMixinIfEEfLi4ENS1_15SUseDefaultLerpIfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
    // Blends the two quaternion keys at `key` and `key + 1` by weight t (see the .cpp).
    // The third argument is passed in the ABI but not read by the body.
    bool getKeyBasedValueEx(const SAnimationAccessor& accessor, int key, int unusedIndex, float t,
                            void* out);
};

}  // namespace animation_track

// SAnimationOutput (the SAnimationAccessor::getOutput result; the key data pointer is at +0x04): canonical in collada/SAnimation.hpp.

namespace tb_ext {

// SAnimationAccessor::getOutput(int) const, 0x669e24. Defined outside this batch.
const SAnimationOutput* accessorGetOutput(const SAnimationAccessor& accessor, int index);

// CBlender<glitch::core::quaternion, 1, glitch::core::quaternion>::getBlendedValueEx, a static member
// with the parameter list (void*, float*, int, void*). Defined outside this batch. The return value
// is passed through by the interpreter unchanged.
bool quatBlenderGetBlendedValueEx(const void* values, const float* weights, int count, void* out);

}  // namespace tb_ext

}  // namespace glitch::collada
