#pragma once
#include <cstdint>

namespace dh2::timeline {
enum class Error : std::uint32_t { ok, argument, range, nonfinite };
struct State {
    std::int32_t current_ms, start_ms, end_ms;
    std::uint32_t loop;
    float delta_magnitude, start_seconds, length_seconds, last_input_seconds;
    float current_seconds, scale;
    std::uint32_t finished, started;
};
}
// Range-only timing. No clip library, callback dispatch or triggered events.
// State is owned by the caller; errors leave it unchanged. Inputs are bounded
// to +/-1,000,000,000 ms, positive duration and finite scale of magnitude <=64.
extern "C" {
dh2::timeline::Error dh2_timeline_init(dh2::timeline::State *, std::int32_t start,
                                    std::int32_t end, float scale, bool loop);
dh2::timeline::Error dh2_timeline_jump(dh2::timeline::State *, std::int32_t milliseconds);
dh2::timeline::Error dh2_timeline_update(dh2::timeline::State *, std::int32_t absolute_time_ms);
}
