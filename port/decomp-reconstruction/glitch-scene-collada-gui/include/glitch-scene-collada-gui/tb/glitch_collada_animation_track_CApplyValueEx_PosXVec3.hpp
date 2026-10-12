// glitch::collada::animation_track::CApplyValueEx<
//     glitch::core::vector3d<float>,
//     CSceneNodePositionComponentMixin<CSceneNodePositionXEx<float>, 0, float> >
//
// Declared for Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CApplyValueEx_gli-0
// (function 0x629a1c, applyBlendedValueEx). No declaration of this instantiation existed in the category.
// The primary CApplyValueEx and the mixin are only forward-declared here, as in
// animation_track_virtual_ex.hpp. Only the member this function needs is declared.
#pragma once

namespace glitch::core {
template <typename T>
struct vector3d;
}  // namespace glitch::core

namespace glitch::collada::animation_track {

class CApplicatorInfo;

template <typename Value, typename Mixin>
class CApplyValueEx;

template <typename Node, int Index, typename Value>
struct CSceneNodePositionComponentMixin;

template <class T>
struct CSceneNodePositionXEx;

template <>
class CApplyValueEx<glitch::core::vector3d<float>,
                    CSceneNodePositionComponentMixin<CSceneNodePositionXEx<float>, 0, float>> {
public:
    // 0x629a1c applyBlendedValueEx(values, weights, count, target, info). The implicit this and the
    // info argument are not read by the original. Partial: see the .cpp.
    int applyBlendedValueEx(void* values, float* weights, int count, void* target, CApplicatorInfo* info);
};

}  // namespace glitch::collada::animation_track
