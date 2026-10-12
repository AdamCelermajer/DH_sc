// Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CApplyValueEx_gli-0 (shared batch id).
// This file holds the X-position applicator only (0x629a1c). Declaration: glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_PosXVec3.hpp.

#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_PosXVec3.hpp"

namespace glitch::collada::animation_track {

// 0x629a1c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
//
// Arguments, from the assembly: R0 = values (3-float keys, stride 0xC), R1 = weights (one float per key),
// R2 = count, R3 = target, info on the stack (never read).
//
// Accumulation (assembly loc_629A60): for each key i, with w = weights[i] and k = values + 3*i,
//   x += w * k[+0x0];  y += w * k[+0x4];  z += w * k[+0x8];
// Where the original sums the keys, the sums start at 0.
//
// Count cases in the assembly:
//   count == 0: the three sums stay 0 (loc_629AC8 with zeroed registers).
//   count == 1: BEQ loc_629AF0 copies k[0..2] without weighting. Hex-Rays shows no path for this
//               case (it leaves its sum at 0.0), but the assembly is clear, so the copy is kept.
//   count >= 2: the weighted loop runs count times.
//
// Then the vector {x, y, z} is stored at SP+0x0C (x), SP+0x10 (y), SP+0x14 (z), and the
// function calls the virtual at vtable slot +0xA4 of target with (target, &vector). Its return
// value is the function's return value (LDR PC, [R3,#0xA4]; R0 = target; R1 = &vector).
//
// Partial: the vtable slot +0xA4 of target is not mapped to a named virtual on the target's type,
// so the final call is not written and the function returns 0 until it is. The blended vector
// and the count cases match the assembly.
int CApplyValueEx<glitch::core::vector3d<float>,
                  CSceneNodePositionComponentMixin<CSceneNodePositionXEx<float>, 0, float>>::applyBlendedValueEx(
    void* values, float* weights, int count, void* target, CApplicatorInfo* info) {
    (void)info;  // the 5th argument is never read by the original

    const float* keys = static_cast<const float*>(values);
    const unsigned int n = static_cast<unsigned int>(count);

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    if (n == 1) {
        // Unweighted copy of the first key (assembly loc_629AF0).
        x = keys[0];  // +0x0
        y = keys[1];  // +0x4
        z = keys[2];  // +0x8
    } else if (n > 1) {
        const float* k = keys;
        for (unsigned int i = 0; i < n; ++i) {
            const float w = weights[i];  // +0x0 of the weight stream, stride 4
            x = x + (w * k[0]);          // +0x0 of the key, stride 0xC
            y = y + (w * k[1]);          // +0x4
            z = z + (w * k[2]);          // +0x8
            k += 3;
        }
    }
    // count == 0 leaves x, y, z at 0.

    // Missing: the virtual call at vtable slot +0xA4 of target with (target, &{x, y, z}), whose
    // result is the return value. The target's type and that slot's virtual name are not mapped.
    (void)target;
    (void)x;
    (void)y;
    (void)z;
    return 0;
}

}  // namespace glitch::collada::animation_track
