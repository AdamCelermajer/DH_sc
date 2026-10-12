// glitch::collada::animation_track::CApplyValueEx<glitch::core::vector3d<float>, CSceneNodePositionMixin<float>>,
// declared for Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CApplyValueEx_gli-0.
//
// The primary template is only forward-declared in animation_track_virtual_ex.hpp, so only this explicit
// specialization is declared. Its applyBlendedValueEx is a static member (no this pointer), as in the
// quaternion header. blendPositionVec3f is the shared weighted-sum helper of the vec3 applicators in this batch.
#pragma once

#include "glitch-shared/glitch_core.hpp"

namespace glitch::collada::animation_track {

template <typename Value, typename Mixin>
class CApplyValueEx;

template <typename T>
class CSceneNodePositionMixin;

class CApplicatorInfo;

// Weighted sum of count vec3 values (stride 12 bytes). count 1 copies values[0..2] without weighting them;
// count 0 gives zero; otherwise sums count terms of values[i] * weights[i] per component, from 0.
// Matches the assembly at 0x6284a4 (the Hex-Rays body does not show the count 1 path).
void blendPositionVec3f(const float* values, const float* weights, unsigned int count, float out[3]);

template <>
class CApplyValueEx<::glitch::core::vector3d<float>, CSceneNodePositionMixin<float>> {
public:
    // 0x6284a4 applyBlendedValueEx(values, weights, count, target, info): static, no this.
    // Blends the vec3 values into a local vector, then calls the virtual slot at byte offset +0xa4 of target
    // with (target, &vector). Partial: the target's type and the slot's method are not mapped, so the final call
    // is not written and the function returns 0. info is not read.
    static int applyBlendedValueEx(void* values, float* weights, int count, void* target, CApplicatorInfo* info);
};

}  // namespace glitch::collada::animation_track
