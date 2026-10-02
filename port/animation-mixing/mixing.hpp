#pragma once
#include "../engine-math/math.hpp"
#include <cstdint>

namespace dh2::mixing {
enum class Error : std::uint32_t { ok, argument, nonfinite, limit };
}
// Bounded original blending calculations. Caller arrays contain count values;
// count is at most 256. Errors leave output/input weights unchanged. Mixing
// outputs must not overlap either input array. These do not schedule animations.
extern "C" {
dh2::mixing::Error dh2_animation_weights_normalize(float *weights, std::uint32_t count);
dh2::mixing::Error dh2_animation_vector_mix(const dh2::math::Vector3f *values,
    const float *weights, std::uint32_t count, dh2::math::Vector3f *out);
dh2::mixing::Error dh2_animation_scalar_mix(const float *values, const float *weights,
    std::uint32_t count, float *out);
dh2::mixing::Error dh2_animation_quaternion_add(const dh2::math::Quaternion *values,
    const float *weights, std::uint32_t count, dh2::math::Quaternion *out);
}
