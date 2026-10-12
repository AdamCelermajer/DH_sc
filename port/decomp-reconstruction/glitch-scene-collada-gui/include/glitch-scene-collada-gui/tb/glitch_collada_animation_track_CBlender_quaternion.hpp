// glitch::collada::animation_track::CBlender<glitch::core::quaternion, 1, glitch::core::quaternion>,
// declared for Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CBlender_glitch_c-0.
//
// The class is declared nowhere else in this category, so it is declared here. Only the explicit
// specialization is defined: the primary template has no layout or body in the repository, and the
// two members are static blend helpers that take their keys and weights as plain arrays.
//
// Key layout: each key is one quaternion stored as four floats (x, y, z, w), 16 bytes per key.
// Weights are one float per key.
#pragma once

#include <cstdint>

#include "glitch-shared/glitch_core.hpp"

namespace glitch::collada::animation_track {

template <typename T, int Index, typename V>
class CBlender;

template <>
class CBlender<glitch::core::quaternion, 1, glitch::core::quaternion> {
public:
    // 0x6130d4 getBlendedValueEx(void*, float*, int, void*): static.
    // Blends `count` quaternion keys with `weights` into out (x, y, z, w). The result starts at the
    // first key with a nonzero weight and is slerped with each later nonzero key, using the running
    // weight total. Returns the Hex-Rays _BOOL4 word: 1 when the first nonzero weight is exactly 1.0
    // (the key is copied unchanged). Otherwise it returns the bit pattern of the blended x component,
    // which the original leaves in r0 at its tail; for count <= 0 or all-zero weights it returns 0
    // and writes the identity (0, 0, 0, 1).
    static std::uint32_t getBlendedValueEx(void* values, float* weights, int count, void* out);

    // 0x613378 getAddedValueEx(void*, float*, int, void*): static.
    // Accumulates a product over the keys: for each key with a positive weight the factor is
    // slerp(identity, key, w); for a negative weight it is slerp(identity, conjugate(key) with x, y, z
    // negated, -w). Zero weights are skipped. The factors multiply onto a running product that starts
    // at the identity. Writes the product (x, y, z, w) to out and returns out.
    static void* getAddedValueEx(void* values, float* weights, int count, void* out);
};

}  // namespace glitch::collada::animation_track
