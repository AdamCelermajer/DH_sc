// Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CApplyValueEx_gli-0 (vector3d<float>
// position Y component entry, batch index 76). The batch id is shared with the quaternion entry (index 71, in
// tb_scene-collada-gui-glitch_collada_animation_track_CApplyValueEx_gli-0.cpp) and the position mixin entry
// (index 72, in ..._gli-0_vec3pos.cpp), so this entry uses its own file. Declarations are in
// glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_PositionYComponentVec3f.hpp.

#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_PositionYComponentVec3f.hpp"

namespace glitch::collada::animation_track {

namespace {

// Weighted sum of count vec3 values (stride 12 bytes), as the assembly at 0x6293a4 does it:
//   count == 1: out = values[0..2]; the weights are not read (loc_629478).
//   count == 0: out = (0, 0, 0) (loc_629450 with the accumulators still zero).
//   otherwise:  acc = 0; count times: acc.x += w * v.x, acc.y += w * v.y, acc.z += w * v.z (loc_6293E8).
// The count is compared as unsigned, as the assembly compares it.
void blendPositionYVec3f(const float* values, const float* weights, unsigned int count, float out[3]) {
    if (count == 1) {
        out[0] = values[0];
        out[1] = values[1];
        out[2] = values[2];
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

// 0x6293a4 glitch::collada::animation_track::CApplyValueEx<glitch::core::vector3d<float>,glitch::collada::animation_track::CSceneNodePositionComponentMixin<glitch::collada::animation_track::CSceneNodePositionYEx<float>,1,float>>::applyBlendedValueEx(void*, float*, int, void*, glitch::collada::animation_track::CApplicatorInfo*)
// Partial. Hex-Rays and disassembly agree on the blend. The Hex-Rays body drops the vector argument of the final call;
// the assembly shows it: the blended vector is stored on the stack, R0 = target (reloaded from the saved R3), R1 = &vector,
// and the call goes through vtable slot +0xA4 of target. Missing: the object type of target and the name of the virtual
// method at +0xA4 (slot 41), so the call is not made and the function returns 0 until the callee is mapped.
// The info argument (5th) is never read by the original. This is the same body as the position mixin entry at 0x6284a4.
int applyBlendedValueExPositionYVec3f(void* values, float* weights, int count, void* target, CApplicatorInfo* info) {
    (void)info;
    float blended[3] = {0.0f, 0.0f, 0.0f};
    blendPositionYVec3f(static_cast<const float*>(values), weights, static_cast<unsigned int>(count), blended);
    (void)target;
    (void)blended;
    return 0;
}

}  // namespace glitch::collada::animation_track
