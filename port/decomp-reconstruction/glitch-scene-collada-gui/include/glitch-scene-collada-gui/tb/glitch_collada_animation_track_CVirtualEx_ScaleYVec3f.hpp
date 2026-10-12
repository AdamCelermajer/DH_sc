// glitch::collada::animation_track::CVirtualEx<CApplyValueEx<vector3d<float>,
//     CSceneNodeScaleComponentMixin<CSceneNodeScaleYEx<float>, 1, float>>>, declared for Track B batch
// e1-glitch-scene-collada-gui-glitch_collada_animation_track_CVirtualEx_glitch-0 (the ScaleY float entry).
//
// The primary template is in animation_track_virtual_ex.hpp and only has the empty destructor. The
// members below belong to this one instantiation, so they are declared as an explicit specialization
// instead of being added to the primary template. Every translation unit that uses this instantiation
// must include this header before any implicit instantiation of it.
//
// The applicator layout is not mapped, so no member is declared at an offset. The vtable dispatch in
// retrieveValue, applyValue and applyBlendedValue is declared as an unmapped stand-in below.
#pragma once

#include <cstddef>
#include <cstdint>

#include "glitch-scene-collada-gui/animation_track_virtual_ex.hpp"

namespace glitch::core {
template <typename T>
struct vector3d;
}

namespace glitch::collada::animation_track {

class CApplicatorInfo;

template <typename T>
struct CSceneNodeScaleYEx;

template <typename T, int Index, typename V>
struct CSceneNodeScaleComponentMixin;

template <typename V, typename Mixin>
struct CApplyValueEx;

// Stand-ins for unmapped guest vtable calls. The object layouts they dispatch on are not in the
// repository. slotByteOffset is the vtable offset the pseudocode reads (for example 144 for +0x90).
// Declared only; defined once the owning layout is mapped.
const std::uint32_t* invokeVirtualSlotWords(const void* self, std::size_t slotByteOffset);
int invokeVirtualSlotInt(const void* self, std::size_t slotByteOffset);

// CApplyValueEx<vector3d<float>, CSceneNodeScaleComponentMixin<CSceneNodeScaleYEx<float>, 1, float>>::applyBlendedValueEx.
// Not in this category's queue and not reconstructed. Declared only.
int applyBlendedValueExScaleYVec3f(const float* values, const float* weights, unsigned int count, void* target);

template <>
class CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>,
                               CSceneNodeScaleComponentMixin<CSceneNodeScaleYEx<float>, 1, float>>> {
public:
    // Empty D1 body, as the primary template.
    ~CVirtualEx() = default;

    // 0x60ef34 getValueSize() const: constant 12 (three floats).
    int getValueSize() const noexcept;

    // 0x610d00 getInstance(): function-local static of this instantiation.
    static CVirtualEx* getInstance();

    // 0x62069c retrieveValue(void*, void*) const: copies three words from the source's vtable
    // result (slot +0x90) into out and returns that result pointer.
    const std::uint32_t* retrieveValue(const void* source, std::uint32_t* out) const;

    // 0x622c40 applyValue(void*, void*, CApplicatorInfo*) const: calls vtable slot +0x94 on the target.
    // The value argument and the applicator info are not read by the pseudocode.
    int applyValue(const void* value, void* target, const CApplicatorInfo* info) const;

    // 0x62a034 getBlendedValue(void*, float*, int, void*) const: weighted sum of count vec3 values
    // (a count of 1 copies the first value without weighting it). The symbol spells the values and out
    // pointers as void*; they are typed here as the pseudocode reads them.
    void getBlendedValue(const float* values, const float* weights, int count, float* out) const;

    // 0x62ca3c applyBlendedValue(void*, float*, int, void*, CApplicatorInfo*) const: forwards to
    // CApplyValueEx::applyBlendedValueEx. The applicator info argument is not read by the pseudocode.
    int applyBlendedValue(const float* values, const float* weights, int count, void* target,
                          const CApplicatorInfo* info) const;
};

}  // namespace glitch::collada::animation_track
