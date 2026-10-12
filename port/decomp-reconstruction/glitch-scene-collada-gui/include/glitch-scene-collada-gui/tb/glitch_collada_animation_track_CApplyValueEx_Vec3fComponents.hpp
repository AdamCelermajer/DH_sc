// CApplyValueEx<glitch::core::vector3d<float>, ...> for the seven remaining vec3 applicators of Track B batch
// e1-glitch-scene-collada-gui-glitch_collada_animation_track_CApplyValueEx_gli-0:
//   position components X/Y/Z (CSceneNodePositionComponentMixin<CSceneNodePositionXEx|YEx|ZEx<float>, 0|1|2, float>)
//   scale mixin (CSceneNodeScaleMixin<float>)
//   scale components X/Y/Z (CSceneNodeScaleComponentMixin<CSceneNodeScaleXEx|YEx|ZEx<float>, 0|1|2, float>)
//
// Explicit specializations with a static applyBlendedValueEx, as in the quaternion and position mixin headers.
// The primary template is only forward-declared. The position mixin itself is in
// glitch_collada_animation_track_CApplyValueEx_PositionMixinVec3f.hpp.
#pragma once

#include "glitch-shared/glitch_core.hpp"

namespace glitch::collada::animation_track {

template <typename Value, typename Mixin>
class CApplyValueEx;

template <typename Node, int Index, typename Value>
struct CSceneNodePositionComponentMixin;
template <class T>
struct CSceneNodePositionXEx;
template <class T>
struct CSceneNodePositionYEx;
template <class T>
struct CSceneNodePositionZEx;

template <typename T>
class CSceneNodeScaleMixin;
template <typename Target, int Index, typename Scalar>
class CSceneNodeScaleComponentMixin;
template <typename T>
class CSceneNodeScaleXEx;
template <typename T>
class CSceneNodeScaleYEx;
template <typename T>
class CSceneNodeScaleZEx;

class CApplicatorInfo;

// 0x628d2c position Z component, slot +0xa4.
template <>
class CApplyValueEx<::glitch::core::vector3d<float>, CSceneNodePositionComponentMixin<CSceneNodePositionZEx<float>, 2, float>> {
public:
    // Static, no this. Calls the virtual slot at +0xa4 of target with (target, &blended); partial.
    static int applyBlendedValueEx(void* values, float* weights, int count, void* target, CApplicatorInfo* info);
};

// 0x6293a4 position Y component, slot +0xa4.
template <>
class CApplyValueEx<::glitch::core::vector3d<float>, CSceneNodePositionComponentMixin<CSceneNodePositionYEx<float>, 1, float>> {
public:
    static int applyBlendedValueEx(void* values, float* weights, int count, void* target, CApplicatorInfo* info);
};

// 0x629a1c position X component, slot +0xa4.
template <>
class CApplyValueEx<::glitch::core::vector3d<float>, CSceneNodePositionComponentMixin<CSceneNodePositionXEx<float>, 0, float>> {
public:
    static int applyBlendedValueEx(void* values, float* weights, int count, void* target, CApplicatorInfo* info);
};

// 0x62d634 scale mixin, slot +0x94.
template <>
class CApplyValueEx<::glitch::core::vector3d<float>, CSceneNodeScaleMixin<float>> {
public:
    static int applyBlendedValueEx(void* values, float* weights, int count, void* target, CApplicatorInfo* info);
};

// 0x62c2cc scale Z component, slot +0x94.
template <>
class CApplyValueEx<::glitch::core::vector3d<float>, CSceneNodeScaleComponentMixin<CSceneNodeScaleZEx<float>, 2, float>> {
public:
    static int applyBlendedValueEx(void* values, float* weights, int count, void* target, CApplicatorInfo* info);
};

// 0x62c944 scale Y component, slot +0x94.
template <>
class CApplyValueEx<::glitch::core::vector3d<float>, CSceneNodeScaleComponentMixin<CSceneNodeScaleYEx<float>, 1, float>> {
public:
    static int applyBlendedValueEx(void* values, float* weights, int count, void* target, CApplicatorInfo* info);
};

// 0x62cfbc scale X component, slot +0x94.
template <>
class CApplyValueEx<::glitch::core::vector3d<float>, CSceneNodeScaleComponentMixin<CSceneNodeScaleXEx<float>, 0, float>> {
public:
    static int applyBlendedValueEx(void* values, float* weights, int count, void* target, CApplicatorInfo* info);
};

}  // namespace glitch::collada::animation_track
