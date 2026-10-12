// Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CApplyValueEx_gli-0, scale X entry
// (batch index 80, owner CSceneNodeScaleComponentMixin<CSceneNodeScaleXEx<float>, 0, float>).
// The batch id is shared with the quaternion, position and scale Z/Y entries, so this entry has its own file.
// The declaration is in glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_Vec3fComponents.hpp.

#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_Vec3fComponents.hpp"

namespace glitch::collada::animation_track {

namespace {

// Blend of count vec3 keys (stride 12 bytes) with one float weight per key. The branches follow the
// assembly at 0x62cfbc, which is the source of truth here (the Hex-Rays body drops the count 1 path):
//   count == 1: out = values[0..2] copied unweighted; the weights are not read (loc_62D090).
//   count == 0: out = (0, 0, 0) (loc_62D068 with the accumulators still zero).
//   otherwise:  each component accumulates weight * component from 0, in key order, do-while on count
//               (loc_62D000). The count is compared as unsigned, as the assembly compares it.
// out[0] is x, out[1] is y, out[2] is z: the original stores them at sp+0x0c, +0x10, +0x14.
void blendScaleXVec3f(const float* values, const float* weights, unsigned int count, float out[3]) {
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
        const float* key = values + index * 3u;  // element stride 12 bytes (three floats)
        x = x + w * key[0];                      // +0x00
        y = y + w * key[1];                      // +0x04
        z = z + w * key[2];                      // +0x08
        ++index;
        --remaining;
    } while (remaining != 0);
    out[0] = x;
    out[1] = y;
    out[2] = z;
}

}  // namespace

// 0x62cfbc _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIfEELi0EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
// Partial. Hex-Rays and disassembly agree on the blend. The assembly shows the call shape that the
// Hex-Rays body drops:
//   value[3] = blend(values, weights, count)          (stored at sp+0x0c..+0x14)
//   return vtable(target)[+0x94](target, value)       (LDR R3,[R0]; MOV LR,PC; LDR PC,[R3,#0x94];
//                                                      R0 = target, R1 = &value)
// Missing: the virtual slot at +0x94 of target is not mapped to a named member. ISceneNode::setScale is
// slot +0xa0 (scene_node.hpp), so +0x94 is a different method and its signature is inferred only from the
// call shape (target, float* value). The final call is not written and the function returns 0 until it is
// mapped. The 5th argument (info) is never read by the original.
int CApplyValueEx<::glitch::core::vector3d<float>, CSceneNodeScaleComponentMixin<CSceneNodeScaleXEx<float>, 0, float>>::
    applyBlendedValueEx(void* values, float* weights, int count, void* target, CApplicatorInfo* info) {
    (void)info;

    float value[3] = {0.0f, 0.0f, 0.0f};
    blendScaleXVec3f(static_cast<const float*>(values), weights, static_cast<unsigned int>(count), value);

    // Missing: the virtual call at +0x94 of target with (target, value), whose result is the return value.
    (void)target;
    (void)value;
    return 0;
}

}  // namespace glitch::collada::animation_track
