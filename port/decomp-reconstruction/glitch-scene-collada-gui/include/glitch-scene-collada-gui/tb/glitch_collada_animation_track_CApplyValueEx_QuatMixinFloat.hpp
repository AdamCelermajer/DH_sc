// glitch::collada::animation_track::CApplyValueEx<glitch::core::quaternion, CSceneNodeQuaternionMixin<float>>,
// declared for Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CApplyValueEx_gli-0.
//
// The class is not declared as a full type anywhere in this category: animation_track_virtual_ex.hpp
// only forward-declares the primary template and names this instantiation in comments. So it is declared
// here, with only the explicit specialization defined (the primary template has no layout or body in the
// repository). The batch member is static: it takes the values, weights and count as plain arrays and the
// target and applicator info as opaque pointers.
#pragma once

#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CBlender_quaternion.hpp"
#include "glitch-shared/glitch_core.hpp"

namespace glitch::collada::animation_track {

template <typename Value, typename Mixin>
class CApplyValueEx;

template <typename T>
class CSceneNodeQuaternionMixin;

class CApplicatorInfo;

template <>
class CApplyValueEx<glitch::core::quaternion, CSceneNodeQuaternionMixin<float>> {
public:
    // 0x620968 applyBlendedValueEx(values, weights, count, target, info): static, no this.
    // Blends `count` quaternion keys with `weights` into a local identity (0, 0, 0, 1) through
    // CBlender<quaternion, 1, quaternion>::getBlendedValueEx, then calls the virtual slot at byte
    // offset +0x9c of target with (target, blended value) and returns its result. info is not read.
    // Partial: the slot at +0x9c is not mapped to a named member, so the final call cannot be
    // written. The return value is the result of that call; 0 is returned until it is mapped.
    static int applyBlendedValueEx(void* values, float* weights, int count, void* target, CApplicatorInfo* info);
};

}  // namespace glitch::collada::animation_track
