#pragma once
#include "../engine-math/math.hpp"
#include <cstdint>
namespace dh2::motion {
enum class Error : std::uint32_t { ok, argument, nonfinite };
struct State { std::uint32_t timestamp; math::Vector3f previous, delta; };
}
extern "C" {
// Source projection of AnimApplicator's explicit-position Reset/CalculateDelta.
// Finite input, disjoint state/position. Rejection leaves state unchanged.
dh2::motion::Error dh2_motion_reset(dh2::motion::State *, std::uint32_t,
                                  const dh2::math::Vector3f *);
// Same timestamp gives zero delta, and STILL replaces previous position.
dh2::motion::Error dh2_motion_calculate(dh2::motion::State *, std::uint32_t,
                                      const dh2::math::Vector3f *);
}
