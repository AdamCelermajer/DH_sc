// glitch::collada::animation_track::CApplyValueEx<glitch::core::vector3d<float>, CSceneNodeScaleComponentMixin<CSceneNodeScaleYEx<float>, 1, float>>,
// declared for Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CApplyValueEx_gli-0
// (the scale Y entry, applyBlendedValueEx at 0x62c944).
//
// No header in this category declares this instantiation as a full type: animation_track_virtual_ex.hpp only
// forward-declares the templates, and CVirtualEx_ScaleYVec3f.hpp declares a free function for the same body.
// So only the explicit specialization is defined here. The member is static: it takes the values, weights and
// count as plain arrays and the target and applicator info as opaque pointers.
#pragma once

#include "glitch-shared/glitch_core.hpp"

namespace glitch::collada::animation_track {

template <typename Value, typename Mixin>
class CApplyValueEx;

template <typename Target, int Index, typename Scalar>
class CSceneNodeScaleComponentMixin;

template <typename T>
class CSceneNodeScaleYEx;

class CApplicatorInfo;

template <>
class CApplyValueEx<::glitch::core::vector3d<float>, CSceneNodeScaleComponentMixin<CSceneNodeScaleYEx<float>, 1, float>> {
public:
    // 0x62c944 applyBlendedValueEx(values, weights, count, target, info): static, no this.
    // Blends `count` vec3 keys (stride 12 bytes) with `weights` into a local (x, y, z), then calls the
    // virtual slot at byte offset +0x94 of target with (target, &blended) and returns its result. info is
    // not read. Partial: the slot at +0x94 is not mapped to a named member, so the final call cannot be
    // written. The return value is 0 until it is mapped.
    static int applyBlendedValueEx(void* values, float* weights, int count, void* target, CApplicatorInfo* info);
};

}  // namespace glitch::collada::animation_track
