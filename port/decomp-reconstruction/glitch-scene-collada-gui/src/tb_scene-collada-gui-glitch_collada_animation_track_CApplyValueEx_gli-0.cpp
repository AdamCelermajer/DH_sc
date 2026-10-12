// Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CApplyValueEx_gli-0.
// The batch holds nine applyBlendedValueEx bodies from three applicator families. Declarations are in:
//   glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_QuatMixinFloat.hpp
//   glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_PositionMixinVec3f.hpp
//   glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_Vec3fComponents.hpp
//
// Common shape of all nine (Hex-Rays and assembly agree unless noted):
//   argument 1 values, argument 2 weights, argument 3 count, argument 4 target; argument 5 (info) is never read.
//   Vec3 applicators: count 1 copies values[0..2] unweighted (the Hex-Rays body does not show this path);
//   count 0 gives zero; otherwise the weighted sum of the vec3 values is stored in three accumulators and
//   stored as a 12-byte vector. The original then calls a virtual slot of target with (target, &vector) and
//   returns its result. The Hex-Rays body drops the vector argument, so it is taken from the assembly.
//   Position applicators call slot +0xa4 (41); scale applicators call slot +0x94 (37).
//   Quaternion applicator: identity seed (0, 0, 0, 1), CBlender blend, then slot +0x9c (39) of target.
//
// Every body is partial. The type of target and the name of each virtual slot are not mapped, so the final
// call is not written and the function returns 0. The blend step is written and matches the original.

#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_QuatMixinFloat.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_PositionMixinVec3f.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_Vec3fComponents.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CBlender_quaternion.hpp"

namespace glitch::collada::animation_track {

// Shared weighted sum of the vec3 applicators (see the header). Matches the assembly at 0x6284a4:
//   count == 1: out = values[0..2]; the weights are not read (loc_628578).
//   count == 0: out = (0, 0, 0) (loc_628550 with the accumulators still zero).
//   otherwise:  acc = 0; count times: acc.x += w * v.x, acc.y += w * v.y, acc.z += w * v.z (do-while, loc_6284E8).
// The count is compared as unsigned, as the assembly compares it.
void blendPositionVec3f(const float* values, const float* weights, unsigned int count, float out[3]) {
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

// 0x620968 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEE19applyBlendedValueExEPvPfiS8_PNS1_15CApplicatorInfoE
// Partial. Identity seed (0, 0, 0, 1), then CBlender<quaternion,1,quaternion>::getBlendedValueEx(values, weights, count, value).
// The original then calls the virtual slot at +0x9c (LDR PC, [R3,#0x9C]) of target with (target, value).
// Missing: the target type and the name of the slot at +0x9c (slot 39). Returns 0 until it is mapped.
int CApplyValueEx<glitch::core::quaternion, CSceneNodeQuaternionMixin<float>>::applyBlendedValueEx(
    void* values, float* weights, int count, void* target, CApplicatorInfo* info) {
    (void)info;

    // Identity quaternion seed: v6[0..2] = 0, v6[3] = 1.0.
    float value[4] = {0.0f, 0.0f, 0.0f, 1.0f};

    // The Hex-Rays _BOOL4 return of getBlendedValueEx is not used by the caller.
    (void)CBlender<glitch::core::quaternion, 1, glitch::core::quaternion>::getBlendedValueEx(values, weights,
                                                                                            count, value);

    // Missing: the virtual call at +0x9c of target with (target, value), whose result is the return value.
    (void)target;
    (void)value;
    return 0;
}

// 0x6284a4 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIfEEE19applyBlendedValueExEPvPfiS9_PNS1_15CApplicatorInfoE
// Partial. Blend as in the header comment, then the virtual slot at +0xa4 of target with (target, &vector).
// Missing: the target type and the name of the slot at +0xa4 (slot 41). Returns 0 until it is mapped.
int CApplyValueEx<::glitch::core::vector3d<float>, CSceneNodePositionMixin<float>>::applyBlendedValueEx(
    void* values, float* weights, int count, void* target, CApplicatorInfo* info) {
    (void)info;
    float blended[3] = {0.0f, 0.0f, 0.0f};
    blendPositionVec3f(static_cast<const float*>(values), weights, static_cast<unsigned int>(count), blended);
    (void)target;
    (void)blended;
    return 0;
}

// 0x628d2c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
// Partial. Same blend and slot +0xa4 as the position mixin. Missing: the target type and the method at slot 41.
int CApplyValueEx<::glitch::core::vector3d<float>,
                  CSceneNodePositionComponentMixin<CSceneNodePositionZEx<float>, 2, float>>::applyBlendedValueEx(
    void* values, float* weights, int count, void* target, CApplicatorInfo* info) {
    (void)info;
    float blended[3] = {0.0f, 0.0f, 0.0f};
    blendPositionVec3f(static_cast<const float*>(values), weights, static_cast<unsigned int>(count), blended);
    (void)target;
    (void)blended;
    return 0;
}

// 0x6293a4 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
// Partial. Same blend and slot +0xa4 as the position mixin. Missing: the target type and the method at slot 41.
int CApplyValueEx<::glitch::core::vector3d<float>,
                  CSceneNodePositionComponentMixin<CSceneNodePositionYEx<float>, 1, float>>::applyBlendedValueEx(
    void* values, float* weights, int count, void* target, CApplicatorInfo* info) {
    (void)info;
    float blended[3] = {0.0f, 0.0f, 0.0f};
    blendPositionVec3f(static_cast<const float*>(values), weights, static_cast<unsigned int>(count), blended);
    (void)target;
    (void)blended;
    return 0;
}

// 0x629a1c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
// Partial. Same blend and slot +0xa4 as the position mixin. Missing: the target type and the method at slot 41.
int CApplyValueEx<::glitch::core::vector3d<float>,
                  CSceneNodePositionComponentMixin<CSceneNodePositionXEx<float>, 0, float>>::applyBlendedValueEx(
    void* values, float* weights, int count, void* target, CApplicatorInfo* info) {
    (void)info;
    float blended[3] = {0.0f, 0.0f, 0.0f};
    blendPositionVec3f(static_cast<const float*>(values), weights, static_cast<unsigned int>(count), blended);
    (void)target;
    (void)blended;
    return 0;
}

// 0x62d634 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIfEEE19applyBlendedValueExEPvPfiS9_PNS1_15CApplicatorInfoE
// Partial. Same blend as the position family, then the virtual slot at +0x94 of target (slot 37).
// Missing: the target type and the method at slot 37. Returns 0 until it is mapped.
int CApplyValueEx<::glitch::core::vector3d<float>, CSceneNodeScaleMixin<float>>::applyBlendedValueEx(
    void* values, float* weights, int count, void* target, CApplicatorInfo* info) {
    (void)info;
    float blended[3] = {0.0f, 0.0f, 0.0f};
    blendPositionVec3f(static_cast<const float*>(values), weights, static_cast<unsigned int>(count), blended);
    (void)target;
    (void)blended;
    return 0;
}

// 0x62c2cc _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
// Partial. Same blend and slot +0x94 as the scale mixin. Missing: the target type and the method at slot 37.
int CApplyValueEx<::glitch::core::vector3d<float>,
                  CSceneNodeScaleComponentMixin<CSceneNodeScaleZEx<float>, 2, float>>::applyBlendedValueEx(
    void* values, float* weights, int count, void* target, CApplicatorInfo* info) {
    (void)info;
    float blended[3] = {0.0f, 0.0f, 0.0f};
    blendPositionVec3f(static_cast<const float*>(values), weights, static_cast<unsigned int>(count), blended);
    (void)target;
    (void)blended;
    return 0;
}

// 0x62c944 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
// Partial. Same blend and slot +0x94 as the scale mixin. Missing: the target type and the method at slot 37.
int CApplyValueEx<::glitch::core::vector3d<float>,
                  CSceneNodeScaleComponentMixin<CSceneNodeScaleYEx<float>, 1, float>>::applyBlendedValueEx(
    void* values, float* weights, int count, void* target, CApplicatorInfo* info) {
    (void)info;
    float blended[3] = {0.0f, 0.0f, 0.0f};
    blendPositionVec3f(static_cast<const float*>(values), weights, static_cast<unsigned int>(count), blended);
    (void)target;
    (void)blended;
    return 0;
}

// 0x62cfbc _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIfEELi0EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
// Partial. Same blend and slot +0x94 as the scale mixin. Missing: the target type and the method at slot 37.
int CApplyValueEx<::glitch::core::vector3d<float>,
                  CSceneNodeScaleComponentMixin<CSceneNodeScaleXEx<float>, 0, float>>::applyBlendedValueEx(
    void* values, float* weights, int count, void* target, CApplicatorInfo* info) {
    (void)info;
    float blended[3] = {0.0f, 0.0f, 0.0f};
    blendPositionVec3f(static_cast<const float*>(values), weights, static_cast<unsigned int>(count), blended);
    (void)target;
    (void)blended;
    return 0;
}

}  // namespace glitch::collada::animation_track
