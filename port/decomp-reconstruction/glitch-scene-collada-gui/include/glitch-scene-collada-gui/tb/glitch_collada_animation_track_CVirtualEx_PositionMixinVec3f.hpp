// glitch::collada::animation_track::CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>, CSceneNodePositionMixin<float>>>,
// declared for Track B batch e2-glitch-scene-collada-gui-pack2-52827d-0.
//
// The primary template (animation_track_virtual_ex.hpp) only has the empty D1 destructor and the identity
// writer for this instantiation. Its members are not declared in any header of this category, so this header
// declares them as a full explicit specialization. The applicator it wraps is declared in
// glitch_collada_animation_track_CApplyValueEx_PositionMixinVec3f.hpp.
#pragma once

#include "glitch-scene-collada-gui/animation_track_virtual_ex.hpp"
#include "glitch-scene-collada-gui/layouts/animation_track.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_PositionMixinVec3f.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CInterpreter_CMixinPosFloat3.hpp"

namespace glitch::collada::animation_track {

class CApplicatorInfo;

template <>
class CVirtualEx<CApplyValueEx<::glitch::core::vector3d<float>, CSceneNodePositionMixin<float>>> {
public:
    // D1 body is empty, as in the primary template.
    ~CVirtualEx() = default;

    // 0x60eff8 getValueSize() const: constant 12 (three floats).
    int getValueSize() const noexcept;

    // 0x623dd8 applyValue(void*, void*, CApplicatorInfo*) const. Partial: tail-calls vtable slot +0xa4 of the
    // applicator info; that vtable is not mapped, so the function returns 0.
    int applyValue(void* value, void* target, CApplicatorInfo* info) const;

    // 0x626b4c getBlendedValue(void*, float*, int, void*) const: weighted sum of vec3 values, count 1 copies.
    void getBlendedValue(void* values, float* weights, int count, void* out) const;

    // 0x62859c applyBlendedValue(void*, float*, int, void*, CApplicatorInfo*) const: forwards values, weights,
    // count, target and info to CApplyValueEx::applyBlendedValueEx as its fifth argument.
    int applyBlendedValue(void* values, float* weights, int count, void* target, CApplicatorInfo* info) const;

    // 0x628834 getKeyBasedValue(SAnimationAccessor const&, int, int, float, void*) const: forwards to
    // CInterpreter<CMixin<float,3,SSceneNodePosition,-1,float>, float, 3, SUseDefaultLerp<float>>::getKeyBasedValueEx.
    float getKeyBasedValue(const glitch::collada::SAnimationAccessor& accessor, int from, int to, float t,
                           float* out) const;
};

}  // namespace glitch::collada::animation_track
