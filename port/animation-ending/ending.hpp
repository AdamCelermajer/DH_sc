#pragma once
#include <cstdint>
namespace dh2::ending {
// Values from timeline::State, not an original ARM32 object layout.
struct Sample { std::int32_t current_ms; float delta_seconds, current_seconds; };
struct Notice { std::int32_t extra_ms; std::uint32_t pending; };
enum class Error : std::uint32_t { ok, argument, nonfinite, range };
}
extern "C" {
// Original CharAnimator::CalculateExtraTime field calculation. Null sample
// retains the prior value. Finite defined casts; output/sample disjoint.
dh2::ending::Error dh2_animation_extra_time(std::int32_t *, const dh2::ending::Sample *);
// Original AnimatorBlender::_HandleAnimEnding field projection. Active timeline
// lookup belongs to caller. A different sender leaves notice unchanged. A
// matching null handle marks pending, retaining extra time. A matching nonzero
// handle requires a sample, calculates extra time, then marks pending.
dh2::ending::Error dh2_animation_end_notice(dh2::ending::Notice *, std::uintptr_t active,
                                         std::uintptr_t sender, const dh2::ending::Sample *);
}
