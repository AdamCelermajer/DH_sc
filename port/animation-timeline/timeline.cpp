#include "timeline.hpp"
#include <cmath>

using namespace dh2::timeline;
namespace {
bool bounded(std::int32_t value) { return value >= -1000000000 && value <= 1000000000; }
bool valid(const State &s) {
    return bounded(s.start_ms) && bounded(s.end_ms) && s.end_ms > s.start_ms &&
           s.loop <= 1 && s.started <= 1 && s.finished <= 1 &&
           std::isfinite(s.delta_magnitude) && std::isfinite(s.start_seconds) &&
           std::isfinite(s.length_seconds) && s.length_seconds > 0 &&
           std::isfinite(s.last_input_seconds) && std::isfinite(s.current_seconds) &&
           std::isfinite(s.scale) && std::fabs(s.scale) <= 64;
}
float seconds(std::int32_t ms) { return static_cast<float>(ms) / 1000.0f; }
}
extern "C" Error dh2_timeline_init(State *state, std::int32_t start, std::int32_t end,
                                  float scale, bool loop) {
    if (!state) return Error::argument;
    if (!bounded(start) || !bounded(end) || end <= start) return Error::range;
    if (!std::isfinite(scale)) return Error::nonfinite;
    if (std::fabs(scale) > 64) return Error::range;
    State s{};
    s.start_ms = start; s.end_ms = end; s.current_ms = start;
    s.start_seconds = seconds(start);
    s.length_seconds = seconds(end - start);
    s.current_seconds = s.start_seconds;
    s.scale = scale; s.loop = loop;
    *state = s;
    return Error::ok;
}
extern "C" Error dh2_timeline_jump(State *state, std::int32_t ms) {
    if (!state) return Error::argument;
    if (!valid(*state) || !bounded(ms)) return Error::range;
    auto s = *state;
    s.current_ms = ms; s.current_seconds = seconds(ms);
    s.finished = 0; s.started = 0;
    *state = s;
    return Error::ok;
}
extern "C" Error dh2_timeline_update(State *state, std::int32_t absolute_time_ms) {
    if (!state) return Error::argument;
    if (!valid(*state) || !bounded(absolute_time_ms)) return Error::range;
    auto s = *state;
    const float now = seconds(absolute_time_ms);
    float delta = 0;
    if (!s.started) {
        s.started = 1;
        s.current_seconds = s.current_seconds + 0.0f;
    } else {
        delta = (now - s.last_input_seconds) * s.scale;
        s.current_seconds = delta + s.current_seconds;
    }
    s.last_input_seconds = now;
    s.delta_magnitude = delta < 0.0f ? -delta : delta;
    // The original uses the integer end converted separately on forward
    // playback, and start_seconds + length_seconds on reverse playback.
    const bool reverse = delta < 0.0f;
    const float boundary = reverse ? seconds(s.start_ms) : seconds(s.end_ms);
    const float wrap_target = reverse ? s.start_seconds + s.length_seconds : s.start_seconds;
    const bool crossed = reverse ? s.current_seconds < boundary : s.current_seconds > boundary;
    if (crossed) {
        if (s.loop)
            s.current_seconds = wrap_target + std::fmod(s.current_seconds - boundary, s.length_seconds);
        else {
            s.current_seconds = boundary;
            s.finished = 1;
        }
    }
    const float ms = s.current_seconds * 1000.0f;
    if (!std::isfinite(ms) || ms <= -2147483648.0f || ms >= 2147483648.0f)
        return Error::nonfinite;
    s.current_ms = static_cast<std::int32_t>(ms);
    *state = s;
    return Error::ok;
}
