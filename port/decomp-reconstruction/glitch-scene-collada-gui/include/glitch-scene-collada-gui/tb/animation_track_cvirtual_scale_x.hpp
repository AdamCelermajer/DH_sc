// glitch::collada::animation_track::CVirtualEx<
//     CApplyValueEx<glitch::core::vector3d<float>,
//                   CSceneNodeScaleComponentMixin<CSceneNodeScaleXEx<float>, 0, float> > >
//
// Declared for Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CVirtualEx_glitch-0.
//
// The generic CVirtualEx<Applicator> in animation_track_virtual_ex.hpp only has the empty
// destructor. This header gives the X-scale instantiation its own explicit specialization, so
// the shared template and the other instantiations stay unchanged. Include this header
// before any use of this instantiation.
//
// Offsets are the original 32-bit offsets, kept as comments only.
#pragma once

#include <cstddef>
#include <cstdint>

#include "glitch-scene-collada-gui/animation_track_virtual_ex.hpp"

namespace glitch::core {
template <typename T>
struct vector3d;
}  // namespace glitch::core

namespace glitch::collada::animation_track {

class CApplicatorInfo;

template <typename Value, typename Mixin>
class CApplyValueEx;

template <typename Component, int Index, typename Value>
class CSceneNodeScaleComponentMixin;

template <typename Value>
class CSceneNodeScaleXEx;

template <>
class CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>,
                               CSceneNodeScaleComponentMixin<CSceneNodeScaleXEx<float>, 0, float>>> {
public:
    // 0x60ed64 _ZN6glitch7collada15animation_track10CVirtualExI...EED1Ev (empty body)
    ~CVirtualEx() = default;

    // 0x60ef4c _ZNK...12getValueSizeEv: the size of one X-scale value (three floats).
    int getValueSize() const;

    // 0x610b44 _ZN...11getInstanceEv: function-local static slot. Partial: see the .cpp.
    static std::int32_t* getInstance();

    // 0x620738 _ZNK...12retrieveValueEPvS...: partial, see the .cpp.
    std::uint32_t* retrieveValue(void* source, void* out) const;

    // 0x6238f4 _ZNK...10applyValueEPvS...PNS1_15CApplicatorInfoE: partial, see the .cpp.
    int applyValue(void* value, void* object, CApplicatorInfo* applicator) const;

    // 0x62d0b4 _ZNK...17applyBlendedValueEPvPfiS...PNS1_15CApplicatorInfoE: forwarder, partial.
    int applyBlendedValue(void* self, float* weights, int count, void* target,
                          CApplicatorInfo* applicator) const;

    // 0x6278a8 _ZNK...15getBlendedValueEPvPfiS...: weighted sum of count three-float values.
    void getBlendedValue(void* values, float* weights, int count, void* out) const;
};

}  // namespace glitch::collada::animation_track
