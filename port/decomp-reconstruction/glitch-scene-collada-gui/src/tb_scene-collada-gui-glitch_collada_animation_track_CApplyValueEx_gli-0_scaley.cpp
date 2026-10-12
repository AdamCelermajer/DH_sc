// Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CApplyValueEx_gli-0 (scale Y vec3 entry,
// applyBlendedValueEx at 0x62c944). The batch id is shared with the quaternion and other vec3 entries, whose bodies
// live in their own files, so this entry uses its own file.
// Declarations are in glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_ScaleYVec3f.hpp.

#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_ScaleYVec3f.hpp"

namespace glitch::collada::animation_track {

namespace {

// Weighted sum of count vec3 values (stride 12 bytes), as the assembly at 0x62c944 does it:
//   count == 1: out = values[0..2]; the weights are not read (loc_62CA18).
//   count == 0: out = (0, 0, 0) (the accumulators are still zero, loc_62C9F0).
//   otherwise:  acc = 0; count times: acc.x += w * v.x, acc.y += w * v.y, acc.z += w * v.z (do-while, loc_62C988).
// The Hex-Rays body only shows the loop (it tests count > 1 and drops the count == 1 copy), so the copy path
// comes from the assembly. The count is compared as unsigned, as the assembly compares it.
void blendScaleYVec3f(const float* values, const float* weights, unsigned int count, float out[3]) {
    if (count == 1) {
        out[0] = values[0];  // +0x00
        out[1] = values[1];  // +0x04
        out[2] = values[2];  // +0x08
        return;
    }
    if (count == 0) {
        out[0] = 0.0f;
        out[1] = 0.0f;
        out[2] = 0.0f;
        return;
    }
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    for (unsigned int i = 0; i < count; ++i) {
        const float w = weights[i];
        const float* v = values + 3 * i;  // element stride 12 bytes (three floats)
        x = x + w * v[0];                 // +0x00
        y = y + w * v[1];                 // +0x04
        z = z + w * v[2];                 // +0x08
    }
    out[0] = x;
    out[1] = y;
    out[2] = z;
}

}  // namespace

// 0x62c944 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
// Partial. The blend matches the assembly. The assembly then stores the blended vector on the stack and calls
// vtable slot +0x94 of target (R0 = target, R1 = &vector, so the slot takes the vector as its one argument).
// Missing: the object type of target and the name of the virtual method at +0x94. The Hex-Rays body drops the
// vector argument and the call is shown with one argument only, so the argument comes from the assembly. The
// info argument (5th) is never read. Returns 0 until the callee is mapped; the blended vector is computed and
// then not used.
int CApplyValueEx<::glitch::core::vector3d<float>, CSceneNodeScaleComponentMixin<CSceneNodeScaleYEx<float>, 1, float>>::
    applyBlendedValueEx(void* values, float* weights, int count, void* target, CApplicatorInfo* info) {
    (void)info;
    float blended[3] = {0.0f, 0.0f, 0.0f};
    blendScaleYVec3f(static_cast<const float*>(values), weights, static_cast<unsigned int>(count), blended);
    (void)target;
    (void)blended;
    return 0;  // placeholder for the unmapped vtable slot +0x94 call on target
}

}  // namespace glitch::collada::animation_track
