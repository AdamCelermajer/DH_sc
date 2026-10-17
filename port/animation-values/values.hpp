#pragma once
#include "../asset-payloads/payloads.hpp"
#include "../engine-math/math.hpp"

namespace dh2::animation {
enum class Error : std::uint32_t { ok, argument, range, unsupported, nonfinite, limit };
}
// Checked float scene-track calculations. Inputs must be valid borrowed views;
// outputs must be disjoint from their backing BRES. These do not apply a pose
// to a scene or implement animator state, transitions or allocation.
extern "C" {
dh2::animation::Error dh2_animation_float_key(const dh2::assets::Animation *, std::uint32_t key,
                                              float *value4);
dh2::animation::Error dh2_animation_float_interpolate(const dh2::assets::Animation *,
                                                      std::uint32_t first, std::uint32_t second,
                                                      float fraction, float *value4);
dh2::animation::Error dh2_animation_float_delta(const dh2::assets::Animation *,
                                                std::uint32_t reference, std::uint32_t first,
                                                std::uint32_t second, float fraction,
                                                bool interpolate, float *value4);
dh2::animation::Error dh2_animation_quaternion_blend(const dh2::math::Quaternion *,
                                                     const float *weights, std::uint32_t count,
                                                     dh2::math::Quaternion *);
}
