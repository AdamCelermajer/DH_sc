#include "transition.hpp"
#include "../animation-mixing/mixing.hpp"
#include <cmath>
#include <cstring>

using namespace dh2::transition;
namespace {
Error valid(const State *s) {
    if (!s) return Error::argument;
    if (!s->count || s->count > 8 || s->current >= s->count || s->previous >= s->count)
        return Error::limit;
    if (!std::isfinite(s->inverse_duration)) return Error::nonfinite;
    for (std::uint32_t i = 0; i < s->count; ++i)
        if (!std::isfinite(s->weights[i])) return Error::nonfinite;
    return Error::ok;
}
std::int32_t signed_bits(std::uint32_t value) {
    std::int32_t result; std::memcpy(&result, &value, sizeof(result)); return result;
}
}
extern "C" Error dh2_transition_init(State *out, std::uint32_t count) {
    if (!out) return Error::argument;
    if (!count || count > 8) return Error::limit;
    State s{}; s.count = count; s.weights[0] = 1;
    *out = s; return Error::ok;
}
extern "C" Error dh2_transition_begin(State *state, std::int32_t duration) {
    const auto error = valid(state);
    if (error != Error::ok) return error;
    if (state->count != 2) return Error::limit;
    auto s = *state;
    s.previous = s.current;
    s.current = (s.current + 1) % s.count;
    s.remaining = s.requested_duration;
    if (s.requested_duration > 0) s.inverse_duration = 1.0f / float(s.requested_duration);
    s.requested_duration = duration < 0 ? 0 : duration;
    *state = s; return Error::ok;
}
extern "C" Error dh2_transition_update(State *state, std::uint32_t timestamp,
                                      std::uint32_t *active_bits) {
    const auto error = valid(state);
    if (error != Error::ok) return error;
    if (!active_bits) return Error::argument;
    const auto a = reinterpret_cast<std::uintptr_t>(state);
    const auto b = reinterpret_cast<std::uintptr_t>(active_bits);
    if (a <= b ? b - a < sizeof(*state) : a - b < sizeof(*active_bits)) return Error::argument;
    auto s = *state;
    if (s.remaining >= 0) {
        s.remaining = signed_bits(std::uint32_t(s.remaining) - (timestamp - s.last_timestamp));
        if (s.remaining > 0) {
            const float previous_weight = float(s.remaining) * s.inverse_duration;
            s.weights[s.previous] = previous_weight;
            s.weights[s.current] = 1.0f - previous_weight;
        } else {
            s.weights[s.previous] = 0;
            s.weights[s.current] = 1;
        }
    }
    std::uint32_t active = 0;
    for (std::uint32_t i = 0; i < s.count; ++i)
        if (s.weights[i] != 0.0f) active |= 1u << i;
    if (dh2_animation_weights_normalize(s.weights, s.count) != dh2::mixing::Error::ok)
        return Error::nonfinite;
    s.last_timestamp = timestamp;
    *state = s; *active_bits = active; return Error::ok;
}
