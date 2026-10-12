// Body for CInterpreter<CMixin<float, 3, SSceneNodePosition, -1, float>, float, 3, SUseDefaultLerp<float>>
// (batch owning 0x6286cc). Declaration is in glitch-scene-collada-gui/include/glitch-scene-collada-gui/tb/
// glitch_collada_animation_track_CInterpreter_CMixinPosFloat3.hpp.
//
// partial: the Hex-Rays body reads the key data through SAnimationAccessor::getOutput(0) and the word at
// output +0x04. That accessor helper is not reconstructed in this tree, so it is declared in tb_ext and
// left undefined here. The data pointer is read as a host pointer, which is not verified against the
// guest 32-bit word in the original.

#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CInterpreter_CMixinPosFloat3.hpp"

namespace glitch::collada::animation_track {

// 0x6286cc glitch::collada::animation_track::CInterpreter<glitch::collada::animation_track::CMixin<float, 3, glitch::collada::animation_track::SSceneNodePosition, -1, float>, float, 3, glitch::collada::animation_track::SUseDefaultLerp<float> >::getKeyBasedValueEx(glitch::collada::SAnimationAccessor const&, int, int, float, void*)
// partial: the key data comes from tb_ext::accessorOutputKeys (SAnimationAccessor::getOutput(0) at +0x04).
// The blend weights are (1 - t) for key a2 and t for key a2 + 1. a3 is not read.
// Key a2 starts at float index 3 * a2 (12 bytes per key), matching the original v5 = 12 * a2.
float CInterpreter<CMixin<float, 3, SSceneNodePosition, -1, float>, float, 3, SUseDefaultLerp<float>>::getKeyBasedValueEx(
    const glitch::collada::SAnimationAccessor& a1, int a2, int a3, float a4, float* a5) {
    (void)a3;  // not read by the original body
    const float oneMinusT = 1.0f - a4;
    const float* keys = glitch::collada::tb_ext::accessorOutputKeys(a1);
    const float* key = keys + 3 * a2;  // key a2: x, y, z
    const float* next = key + 3;       // key a2 + 1: x, y, z

    // Hex-Rays: result = ((v14 * v7) + 0.0) + (a4 * v10[1]), with v10[1] the x of the next key.
    const float x = ((oneMinusT * key[0]) + 0.0f) + (a4 * next[0]);
    // Hex-Rays: v11 = (v9 + 0.0) + (a4 * v8[3]), v9 = (1 - a4) * key y, v8[3] the y of the next key.
    const float y = ((oneMinusT * key[1]) + 0.0f) + (a4 * next[1]);
    // Hex-Rays: v12 = (a4 * v8[4]) + ((1 - a4) * v8[1] + 0.0), v8[4] the z of the next key.
    const float z = (a4 * next[2]) + ((oneMinusT * key[2]) + 0.0f);

    a5[0] = x;
    a5[1] = y;
    a5[2] = z;
    return x;
}

}  // namespace glitch::collada::animation_track
