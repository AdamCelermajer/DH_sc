#pragma once

#include <cmath>
#include <cstdint>
#include <limits>

namespace dh2::actor::application_clock_v1 {

// Typed projection of Application::ComputeDt fields +0x88/+0x8c/+0x90/+0x94
// and scales +0x9c/+0x98. The original updates the absolute clock first,
// computes GetDt from wrapping uint32 milliseconds and scaleA, then carries
// the fractional GetDtScaled remainder across calls.
struct State {
    std::uint32_t last_real_time_ms = 0;
    float dt_scale = 1.0f;          // Application +0x9c
    float scaled_dt_scale = 1.0f;   // Application +0x98
    float fractional_remainder = 0.0f; // Application +0x94
};

struct Result {
    std::uint32_t raw_elapsed_ms = 0; // wrapping Timer::getRealTime difference
    std::uint32_t dt_ms = 0;        // bit projection of Application +0x8c
    std::uint32_t scaled_dt_ms = 0; // bit projection of Application +0x90
    float fractional_remainder = 0.0f;
};

enum class Status : std::uint8_t { complete, invalid_argument, source_range_undefined };

inline bool source_f2iz(float value, std::int32_t* out) noexcept {
    if (!out || std::isnan(value)) return false;
    if (value >= 2147483648.0f) {
        *out = INT32_MAX;
        return true;
    }
    if (value <= -2147483648.0f) {
        *out = INT32_MIN;
        return true;
    }
    *out = static_cast<std::int32_t>(std::trunc(value));
    return true;
}

// Reconstructs the arithmetic after Timer::getRealTime. The timer source is a
// provider because Android uses its own monotonic clock; only elapsed uint32
// milliseconds enter this projection. No frame clamp or fixed-step splitting
// is applied here; Application's >2s early-return gate is an outer caller gate.
inline Status compute(State* state, std::uint32_t now_ms, Result* out) noexcept {
    if (!state || !out || !std::isfinite(state->dt_scale) ||
        !std::isfinite(state->scaled_dt_scale) ||
        !std::isfinite(state->fractional_remainder))
        return Status::invalid_argument;

    const std::uint32_t elapsed = now_ms - state->last_real_time_ms;
    volatile float elapsed_f = static_cast<float>(elapsed);
    volatile float dt_product = elapsed_f * state->dt_scale;
    std::int32_t signed_dt = 0;
    if (!source_f2iz(dt_product, &signed_dt))
        return Status::source_range_undefined;

    const std::uint32_t dt_bits = static_cast<std::uint32_t>(signed_dt);
    volatile float dt_f = static_cast<float>(dt_bits); // GetDt returns uint32
    volatile float scaled_product = dt_f * state->scaled_dt_scale;
    volatile float scaled_total = scaled_product + state->fractional_remainder;
    std::int32_t signed_scaled = 0;
    if (!source_f2iz(scaled_total, &signed_scaled))
        return Status::source_range_undefined;

    const std::uint32_t scaled_bits = static_cast<std::uint32_t>(signed_scaled);
    volatile float scaled_f = static_cast<float>(scaled_bits); // GetDtScaled returns uint32
    volatile float next_fraction = scaled_total - scaled_f;
    State next = *state;
    next.last_real_time_ms = now_ms;
    next.fractional_remainder = next_fraction;
    *state = next;
    *out = {elapsed, dt_bits, scaled_bits, next_fraction};
    return Status::complete;
}

} // namespace dh2::actor::application_clock_v1
