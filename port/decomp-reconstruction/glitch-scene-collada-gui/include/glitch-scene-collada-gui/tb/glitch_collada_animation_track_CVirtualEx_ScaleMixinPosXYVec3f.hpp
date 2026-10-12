// glitch::collada::animation_track::CVirtualEx<...> for three vec3<float> applicators, declared for Track B
// batch e2-glitch-scene-collada-gui-pack1-686a65-0:
//   CVirtualEx<CApplyValueEx<vector3d<float>, CSceneNodeScaleMixin<float>>>
//   CVirtualEx<CApplyValueEx<vector3d<float>, CSceneNodePositionComponentMixin<CSceneNodePositionYEx<float>, 1, float>>>
//   CVirtualEx<CApplyValueEx<vector3d<float>, CSceneNodePositionComponentMixin<CSceneNodePositionXEx<float>, 0, float>>>
//
// The generic CVirtualEx<Applicator> in animation_track_virtual_ex.hpp only has the empty destructor. Each of
// these three is an explicit specialization here, so the shared template and the other instantiations stay
// unchanged. The applicator types come from glitch_collada_animation_track_CApplyValueEx_Vec3fComponents.hpp.
// Offsets are the original 32-bit offsets, kept as comments only.
#pragma once

#include <cstddef>
#include <cstdint>

#include "glitch-scene-collada-gui/animation_track_virtual_ex.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_Vec3fComponents.hpp"

namespace glitch::collada::animation_track {

// Scale mixin (CSceneNodeScaleMixin<float>): the whole vec3 scale, slot +0x94 (applyValue) on the object.
template <>
class CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>, CSceneNodeScaleMixin<float>>> {
public:
    // 0x60ef64 getValueSize() const: constant 12 (three floats).
    int getValueSize() const;

    // 0x610988 getInstance(): function-local word s_Instance. Partial, see the .cpp.
    static std::int32_t* getInstance();

    // 0x62393c applyValue(void*, void*, CApplicatorInfo*) const: partial, see the .cpp.
    int applyValue(void* value, void* object, CApplicatorInfo* applicator) const;

    // 0x6275fc getBlendedValue(void*, float*, int, void*) const: weighted sum of count vec3 values.
    void getBlendedValue(void* values, float* weights, int count, void* out) const;

    // 0x62d72c applyBlendedValue(void*, float*, int, void*, CApplicatorInfo*) const: forwarder, partial.
    int applyBlendedValue(void* values, float* weights, int count, void* target,
                          CApplicatorInfo* applicator) const;
};

// Position Y component (CSceneNodePositionComponentMixin<CSceneNodePositionYEx<float>, 1, float>).
template <>
class CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>,
                               CSceneNodePositionComponentMixin<CSceneNodePositionYEx<float>, 1, float>>> {
public:
    // 0x60efc8 getValueSize() const: constant 12 (three floats).
    int getValueSize() const;

    // 0x610610 getInstance(): function-local word s_Instance. Partial, see the .cpp.
    static std::int32_t* getInstance();

    // 0x620460 retrieveValue(void*, void*) const: partial, see the .cpp.
    std::uint32_t* retrieveValue(void* source, void* out) const;

    // 0x623d48 applyValue(void*, void*, CApplicatorInfo*) const: partial, see the .cpp.
    int applyValue(void* value, void* object, CApplicatorInfo* applicator) const;

    // 0x6270a4 getBlendedValue(void*, float*, int, void*) const: weighted sum of count vec3 values.
    void getBlendedValue(void* values, float* weights, int count, void* out) const;

    // 0x62949c applyBlendedValue(void*, float*, int, void*, CApplicatorInfo*) const: forwarder, partial.
    int applyBlendedValue(void* values, float* weights, int count, void* target,
                          CApplicatorInfo* applicator) const;
};

// Position X component (CSceneNodePositionComponentMixin<CSceneNodePositionXEx<float>, 0, float>).
template <>
class CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>,
                               CSceneNodePositionComponentMixin<CSceneNodePositionXEx<float>, 0, float>>> {
public:
    // 0x60efe0 getValueSize() const: constant 12 (three floats).
    int getValueSize() const;

    // 0x610454 getInstance(): function-local word s_Instance. Partial, see the .cpp.
    static std::int32_t* getInstance();

    // 0x6204fc retrieveValue(void*, void*) const: partial, see the .cpp.
    std::uint32_t* retrieveValue(void* source, void* out) const;

    // 0x623d90 applyValue(void*, void*, CApplicatorInfo*) const: partial, see the .cpp.
    int applyValue(void* value, void* object, CApplicatorInfo* applicator) const;

    // 0x626df8 getBlendedValue(void*, float*, int, void*) const: weighted sum of count vec3 values.
    void getBlendedValue(void* values, float* weights, int count, void* out) const;

    // 0x629b14 applyBlendedValue(void*, float*, int, void*, CApplicatorInfo*) const: forwarder, partial.
    int applyBlendedValue(void* values, float* weights, int count, void* target,
                          CApplicatorInfo* applicator) const;
};

}  // namespace glitch::collada::animation_track
