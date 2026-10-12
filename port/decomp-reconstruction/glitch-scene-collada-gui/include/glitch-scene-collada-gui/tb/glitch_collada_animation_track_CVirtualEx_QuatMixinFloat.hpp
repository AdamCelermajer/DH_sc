// glitch::collada::animation_track::CVirtualEx<CApplyValueEx<glitch::core::quaternion, CSceneNodeQuaternionMixin<float>>>,
// declared for Track B batch e2-glitch-scene-collada-gui-pack2-52827d-0.
//
// The primary template (animation_track_virtual_ex.hpp) has only the D1 destructor and the identity writer for
// this instantiation. Its members are declared here as a full explicit specialization.
#pragma once

#include "glitch-scene-collada-gui/animation_track_virtual_ex.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_QuatMixinFloat.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CBlender_quaternion.hpp"

namespace glitch::collada::animation_track {

class CApplicatorInfo;

template <>
class CVirtualEx<CApplyValueEx<glitch::core::quaternion, CSceneNodeQuaternionMixin<float>>> {
public:
    // D1 body is empty, as in the primary template.
    ~CVirtualEx() = default;

    // 0x60f0e8 getValueSize() const: constant 16 (four floats).
    int getValueSize() const noexcept;

    // 0x60ff20 getInstance(): function-local static of this instantiation. Partial, see the source file.
    static int* getInstance();

    // 0x613300 getBlendedValue(void*, float*, int, void*) const: forwards to
    // CBlender<quaternion, 1, quaternion>::getBlendedValueEx and returns its low byte as a bool.
    bool getBlendedValue(void* values, float* weights, int count, void* out) const;

    // 0x62084c applyValue(void*, void*, CApplicatorInfo*) const. Partial: tail-calls vtable slot +0x9c of the
    // applicator info; that vtable is not mapped, so the function returns 0.
    int applyValue(void* value, void* target, CApplicatorInfo* info) const;

    // 0x6209b4 applyBlendedValue(void*, float*, int, void*, CApplicatorInfo*) const: forwards values, weights,
    // count, target and info to CApplyValueEx::applyBlendedValueEx as its fifth argument.
    int applyBlendedValue(void* values, float* weights, int count, void* target, CApplicatorInfo* info) const;
};

}  // namespace glitch::collada::animation_track
