// Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CVirtualEx_glitch-0.
// glitch::collada::animation_track::CVirtualEx<CApplyValueEx<float, CMixin<float, 1, ...>>> (FloatApplyValueEx).
// Declarations are in glitch-scene-collada-gui/include/glitch-scene-collada-gui/animation_track_virtual_ex.hpp.

#include <cstdint>

#include "glitch-scene-collada-gui/animation_track_virtual_ex.hpp"

namespace glitch::collada::animation_track {

namespace tb_standin {

// 0x62008c glitch::collada::animation_track::CApplyValueEx<float, CMixin<float, 1, ...>>::applyBlendedValueEx(void*, float*, int, void*, CApplicatorInfo*)
// Static member: the caller passes no this pointer. The body is queued in this category but not
// reconstructed yet, so applyBlendedValue below is partial.
int applyBlendedValueExFloat(void* values, float* weights, int count, void* target, CApplicatorInfo* info);

// IMaterialParameters<CMaterial, ISharedMemoryBlockHeader<CMaterial>>::setParameterCvt<float>(ushort, uint, float const&)
// on the parameter object passed as target. No category header maps this class, so applyValue is partial.
// The return value is taken from the tail call, as the pseudocode shows; the callee return type is not mapped.
int setParameterCvtFloat(void* target, std::uint16_t index, unsigned flags, const float* value);

}  // namespace tb_standin

// 0x60ee94 glitch::collada::animation_track::CVirtualEx<...float...>::getValueSize() const
template <>
int CVirtualEx<FloatApplyValueEx>::getValueSize() const {
    return 4;  // one 32-bit float word
}

// 0x618fe4 glitch::collada::animation_track::CVirtualEx<...float...>::applyValue(void*, void*, CApplicatorInfo*) const
// Pseudocode: return setParameterCvt<float>(target, 0, 0, value). Register map from the assembly:
// r1 (value) moves to r3, r2 (target) moves to r0. The this pointer and the info pointer are unused.
// The index is a u16 that the assembly loads with LDRH from [R12 + 8] while R12 is 0; Hex-Rays folds
// that load to 0, so the index stays 0 here. The row is partial until that load is confirmed.
template <>
int CVirtualEx<FloatApplyValueEx>::applyValue(void* value, void* target, CApplicatorInfo* info) const {
    (void)info;
    return tb_standin::setParameterCvtFloat(target, 0, 0, static_cast<const float*>(value));
}

// 0x620118 glitch::collada::animation_track::CVirtualEx<...float...>::applyBlendedValue(void*, float*, int, void*, CApplicatorInfo*) const
// Thunk: forwards the five arguments to the static CApplyValueEx<float>::applyBlendedValueEx without this.
// The register and stack moves match the pseudocode; the target body is not reconstructed yet.
template <>
int CVirtualEx<FloatApplyValueEx>::applyBlendedValue(void* values, float* weights, int count, void* target,
                                                     CApplicatorInfo* info) const {
    return tb_standin::applyBlendedValueExFloat(values, weights, count, target, info);
}

// 0x620134 glitch::collada::animation_track::CVirtualEx<...float...>::getBlendedValue(void*, float*, int, void*) const
// Count 1 copies values[0]. Otherwise the result is the dot product of values and weights over count
// terms, and 0 when count is 0. The original's do-while loop is written as a for loop; the two agree
// for every count >= 0, and the caller never passes a negative count.
template <>
void CVirtualEx<FloatApplyValueEx>::getBlendedValue(void* values, float* weights, int count, void* out) const {
    const float* value = static_cast<const float*>(values);
    float* result = static_cast<float*>(out);
    if (count == 1) {
        *result = value[0];
        return;
    }
    float sum = 0.0f;
    for (int i = 0; i < count; ++i) {
        sum = sum + value[i] * weights[i];
    }
    *result = sum;
}

}  // namespace glitch::collada::animation_track
