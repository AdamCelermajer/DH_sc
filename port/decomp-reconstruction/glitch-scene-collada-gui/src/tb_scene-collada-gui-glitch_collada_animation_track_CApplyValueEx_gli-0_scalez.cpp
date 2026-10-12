// Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CApplyValueEx_gli-0, scale Z entry.
// The declaration is in glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_ScaleZVec3f.hpp.
// The quaternion entry of the same batch id is in tb_scene-collada-gui-glitch_collada_animation_track_CApplyValueEx_gli-0.cpp.

#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_ScaleZVec3f.hpp"

namespace glitch::collada::animation_track {

namespace {

// Blend of count vec3 keys (stride 12 bytes) with one float weight per key. The branches follow the
// assembly at 0x62c2cc, which is the source of truth here (the Hex-Rays body drops the count 1 path):
//   count == 1: out = values[0] copied unweighted; the weights are not read.
//   count == 0: out = (0, 0, 0).
//   otherwise:  each component accumulates weight * component from 0, in key order, do-while on count.
// out[0] is x, out[1] is y, out[2] is z: the original stores them at sp+0x0c, +0x10, +0x14.
void blendScaleZVec3f(const float* values, const float* weights, unsigned int count, float out[3]) {
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
    unsigned int remaining = count;
    unsigned int index = 0;
    do {
        const float w = weights[index];
        const float* key = values + index * 3u;
        x = x + w * key[0];
        y = y + w * key[1];
        z = z + w * key[2];
        ++index;
        --remaining;
    } while (remaining != 0);
    out[0] = x;
    out[1] = y;
    out[2] = z;
}

}  // namespace

// 0x62c2cc glitch::collada::animation_track::CApplyValueEx<vector3d<float>, CSceneNodeScaleComponentMixin<CSceneNodeScaleZEx<float>, 2, float>>::applyBlendedValueEx(void*, float*, int, void*, CApplicatorInfo*)
// Partial. Hex-Rays and disassembly agree on the blend and on the call shape:
//   value[3] = blend(values, weights, count)          (stored at sp+0x0c..+0x14)
//   return vtable(target)[+0x94](target, value)       (LDR R3,[R0]; LDR PC,[R3,#0x94]; R0 = target, R1 = &value)
// The Hex-Rays body drops the value argument of that call and returns the blended x, y, z as dead
// register sums. Missing: the virtual slot at +0x94 of target is not mapped to a named member (its
// signature is inferred as (target, float* value) from the call shape only), so the final call is not written
// and the function returns 0. The 5th argument (info) is never read by the original.
int CApplyValueEx<::glitch::core::vector3d<float>, CSceneNodeScaleComponentMixin<CSceneNodeScaleZEx<float>, 2, float>>::
    applyBlendedValueEx(void* values, float* weights, int count, void* target, CApplicatorInfo* info) {
    (void)info;

    float value[3] = {0.0f, 0.0f, 0.0f};
    blendScaleZVec3f(static_cast<const float*>(values), weights, static_cast<unsigned int>(count), value);

    // Missing: the virtual call at +0x94 of target with (target, value), whose result is the return value.
    (void)target;
    (void)value;
    return 0;
}

}  // namespace glitch::collada::animation_track
