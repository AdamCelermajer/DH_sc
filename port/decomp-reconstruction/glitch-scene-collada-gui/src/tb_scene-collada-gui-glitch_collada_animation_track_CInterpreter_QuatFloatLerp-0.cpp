// Track B fixup-1: definition of
// glitch::collada::animation_track::CInterpreter<CSceneNodeQuaternionMixin<float>, float, 4, SUseDefaultLerp<float>>::getKeyBasedValueEx,
// declared in glitch-scene-collada-gui/tb/glitch_collada_animation_track_CInterpreter_QuatFloatLerp.hpp.
//
// The body follows the Hex-Rays listing (py -3 tools/decomp_extract.py --address 0x613294): take key-data output 0 of the accessor,
// blend the quaternion keys at key and key + 1 with weights (1 - t, t) through CBlender<quaternion, 1, quaternion>, and return the
// blender's result. The third argument is in the ABI but not read.

#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CInterpreter_QuatFloatLerp.hpp"

namespace glitch::collada::animation_track {

// 0x613294 _ZN6glitch7collada15animation_track12CInterpreterINS1_25CSceneNodeQuaternionMixinIfEEfLi4ENS1_15SUseDefaultLerpIfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
bool CInterpreter<CSceneNodeQuaternionMixin<float>, float, 4, SUseDefaultLerp<float>>::getKeyBasedValueEx(
    const SAnimationAccessor& accessor, int key, int unusedIndex, float t, void* out) {
    (void)unusedIndex;
    const SAnimationOutput* output = tb_ext::accessorGetOutput(accessor, 0);
    // Quaternion keys are 16 bytes each (+0x04 of the output entry holds the key data pointer).
    const char* keyData = static_cast<const char*>(output->data);
    const float weights[2] = {1.0f - t, t};
    return tb_ext::quatBlenderGetBlendedValueEx(keyData + 16 * key, weights, 2, out);
}

}  // namespace glitch::collada::animation_track
