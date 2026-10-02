#pragma once
#include <cstdint>

namespace dh2::transition {
enum class Error : std::uint32_t { ok, argument, limit, nonfinite };
struct State {
    float weights[8];
    std::uint32_t count, current, previous;
    std::int32_t requested_duration, remaining;
    float inverse_duration;
    std::uint32_t last_timestamp;
};
}
extern "C" {
// Bounded state projection of AnimatorBlender, without object dispatch/events.
dh2::transition::Error dh2_transition_init(dh2::transition::State *, std::uint32_t count);
// Original Blend's checked path requires exactly two children. The fade uses
// the PREVIOUS requested duration; this call stores the duration for next time.
dh2::transition::Error dh2_transition_begin(dh2::transition::State *, std::int32_t duration);
// Active bits record nonzero children BEFORE updateTime's normalization.
// No child timelines or synchronization objects are called by this API.
// Rejection leaves both outputs unchanged. active_bits must not overlap state.
dh2::transition::Error dh2_transition_update(dh2::transition::State *, std::uint32_t timestamp,
                                          std::uint32_t *active_bits);
}
