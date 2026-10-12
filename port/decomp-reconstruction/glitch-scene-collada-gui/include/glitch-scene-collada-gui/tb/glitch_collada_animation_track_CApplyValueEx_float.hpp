// glitch::collada::animation_track::CApplyValueEx<float, CMixin<float, 1, SMaterialSetParam<SAnimationTypes<float, float>>, -1, float>>,
// declared for Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CApplyValueEx_flo-0.
//
// The primary template is only forward-declared in animation_track_virtual_ex.hpp, so this
// explicit specialization is the first full declaration of the float applicator. Its applicator
// layout is not mapped, so the class holds no data members.
#pragma once

#include <cstdint>

#include "glitch-scene-collada-gui/animation_track_virtual_ex.hpp"

namespace glitch::collada::animation_track {

template <>
class CApplyValueEx<float, CMixin<float, 1, SMaterialSetParam<SAnimationTypes<float, float>>, -1, float>> {
public:
    // 0x62008c applyBlendedValueEx(void*, float*, int, void*, CApplicatorInfo*). Static, no this argument.
    static int applyBlendedValueEx(void* values, float* weights, int count, void* target, CApplicatorInfo* info);
};

}  // namespace glitch::collada::animation_track
