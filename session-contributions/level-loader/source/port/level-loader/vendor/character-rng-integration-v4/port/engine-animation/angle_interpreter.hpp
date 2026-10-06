#pragma once
#include "../engine-math/math.hpp"
#include <cstdint>
namespace dh2::animation {
struct AngleAccessor24 {
 const float* values;
 const float* default_value; // original variant data: axis XYZ then unused angle
 std::uint32_t count,reserved;
};
static_assert(sizeof(void*)==8&&sizeof(AngleAccessor24)==24);
}
// Source float QuaternionAngleMixin (types6..9), key and key-pair wrappers.
// Radians and unnormalized default axis are passed to the source math kernel.
// 0 completed, -1 malformed, -2 missing default (source scratch angle undefined).
extern "C" int dh2_animation_angle_key(dh2::math::Quaternion*,
 const dh2::animation::AngleAccessor24*,std::uint32_t key);
extern "C" int dh2_animation_angle_between(dh2::math::Quaternion*,
 const dh2::animation::AngleAccessor24*,std::uint32_t key,std::uint32_t next,float fraction);
