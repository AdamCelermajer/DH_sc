// Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CInterpreter_glit-0 (entry 0x628850).
// The batch id is shared by many entries, so this file is named by the class instead of the id.
// Declarations are in glitch-scene-collada-gui/tb/glitch_collada_animation_track_CInterpreter_ScaleFloat3Lerp.hpp.

#include <cstddef>
#include <cstdint>

#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CInterpreter_ScaleFloat3Lerp.hpp"

namespace glitch::collada::animation_track {

// 0x628850 _ZN6glitch7collada15animation_track12CInterpreterINS1_20CSceneNodeScaleMixinIfEEfLi3ENS1_15SUseDefaultLerpIfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
// Hex-Rays: v14 = 1 - t; output = accessor.getOutput(0); base = output->data; key slot = base + 12 * key.
// Each component is (1 - t) * key[i] + t * key[i + 3] (x: slots 0 and 3, y: 1 and 4, z: 2 and 5).
// The sums keep the original "+ 0.0" terms, and the stores go to out[0], out[1], out[2]. Returns x.
// The third argument (a3) is in the ABI but never read by the body.
// Partial: control flow and side effects match Hex-Rays and the disassembly (the weight 1 - t is
// computed with the 1.0 constant, the data pointer is read from output +0x04, stride 12 bytes, key pair
// at element 3 * key and 3 * key + 3). SAnimationAccessor::getOutput (0x669e24) is declared in tb_ext but
// not defined in this batch, and the layouts/animation_track.hpp note describes its return value
// differently, so the return layout (data at +0x04) is taken from this pseudocode only. A null output is
// not checked, as in the original.
float CInterpreter<CSceneNodeScaleMixin<float>, float, 3, SUseDefaultLerp<float>>::getKeyBasedValueEx(
    const SAnimationAccessor& accessor, int key, int unusedIndex, float t, void* out) {
    (void)unusedIndex;

    SScaleAnimationOutput* output = tb_ext::scaleAccessorGetOutput(accessor, 0);
    const float v14 = 1.0f - t;

    // Scale keys are 12 bytes each (3 floats); the key index is scaled by that stride.
    auto* keys = static_cast<float*>(output->data);
    const float* k0 = keys + 3 * static_cast<std::ptrdiff_t>(key);
    const float* k1 = k0 + 3;

    // Hex-Rays: v9 = v14 * v8[1] (y of key 0); v11 = (v9 + 0.0) + a4 * v8[3] (y of key 1).
    const float y = (v14 * k0[1] + 0.0f) + t * k1[1];
    // Hex-Rays: v12 = a4 * v8[4] + ((1 - a4) * v8[1] + 0.0), z of key 0 and key 1.
    const float z = (t * k1[2]) + ((v14 * k0[2]) + 0.0f);
    // Hex-Rays: result = (v14 * v7 + 0.0) + a4 * v10[1], x of key 0 and key 1.
    const float x = (v14 * k0[0] + 0.0f) + t * k1[0];

    auto* o = static_cast<float*>(out);
    o[0] = x;
    o[1] = y;
    o[2] = z;
    return x;
}

}  // namespace glitch::collada::animation_track
