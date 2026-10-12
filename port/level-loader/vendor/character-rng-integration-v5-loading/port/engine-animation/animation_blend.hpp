#pragma once
#include <cstdint>

namespace dh2::animation {
inline constexpr std::int32_t maximum_blend_slots = 65536;
static_assert(sizeof(void*) == 8, "native blend API requires 64-bit pointers");
}

extern "C" {
// Immutable contiguous slots: scalar stride1, vector3 stride3, quaternion
// stride4 (XYZW). Native contract: 0 success, 1 malformed before any write.
// count is0..65536. Output must be float-aligned and disjoint from every read
// input span. Inputs may alias each other. For count0 inputs may be null.
// Scalar/vector count1 copy exact bits and ignore weights (which may be null).
// Other nonempty calls need aligned values and weights. All IEEE values and
// weights, including NaN/infinity/negative/zero, are accepted source inputs;
// arithmetic NaNs have classification parity rather than a payload contract.
// No normalization, positive-weight filtering or caller weight normalization.
int dh2_animation_blend_scalar(float* output, const float* values,
                               const float* weights, std::int32_t count);
// Both original node Position626b4c and Scale6275fc use this identical kernel.
int dh2_animation_blend_vector3(float* output, const float* values,
                                const float* weights, std::int32_t count);
// Original6130d4: allzero -> identity; first nonzero weight1 -> immediate
// exact copy ignoring remaining slots; otherwise ordered incremental
// weight/(accumulated+weight) through the recovered612d00 quaternion slerp.
int dh2_animation_blend_quaternion(float* output, const float* values,
                                   const float* weights, std::int32_t count);
}
