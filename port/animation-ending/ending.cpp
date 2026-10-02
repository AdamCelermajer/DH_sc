#include "ending.hpp"
#include <cmath>
#include <cstring>
using namespace dh2::ending;
namespace {
bool overlaps(const void *p, std::size_t n, const Sample *sample) {
    const auto a = reinterpret_cast<std::uintptr_t>(p), b = reinterpret_cast<std::uintptr_t>(sample);
    return a <= b ? b - a < n : a - b < sizeof(*sample);
}
Error calculate(const Sample &sample, std::int32_t &out) {
    const float delta = sample.delta_seconds * 1000.0f;
    const float current = sample.current_seconds * 1000.0f;
    if (!std::isfinite(delta) || !std::isfinite(current)) return Error::nonfinite;
    if (delta < -2147483648.0f || delta >= 2147483648.0f ||
        current < -2147483648.0f || current >= 2147483648.0f) return Error::range;
    const auto elapsed = static_cast<std::int32_t>(delta);
    const auto milliseconds = static_cast<std::int32_t>(current);
    const std::uint32_t bits = std::uint32_t(milliseconds) - std::uint32_t(sample.current_ms);
    std::int32_t difference; std::memcpy(&difference, &bits, sizeof(difference));
    out = difference >= 0 && difference < elapsed ? elapsed - difference : 0;
    return Error::ok;
}
}
extern "C" Error dh2_animation_extra_time(std::int32_t *out, const Sample *sample) {
    if (!out) return Error::argument;
    if (!sample) return Error::ok;
    if (overlaps(out, sizeof(*out), sample)) return Error::argument;
    std::int32_t result{};
    const auto error = calculate(*sample, result);
    if (error == Error::ok) *out = result;
    return error;
}
extern "C" Error dh2_animation_end_notice(Notice *out, std::uintptr_t active,
                                          std::uintptr_t sender, const Sample *sample) {
    if (!out) return Error::argument;
    if (active != sender) return Error::ok;
    auto result = *out;
    if (active) {
        if (!sample || overlaps(out, sizeof(*out), sample)) return Error::argument;
        const auto error = calculate(*sample, result.extra_ms);
        if (error != Error::ok) return error;
    }
    result.pending = 1;
    *out = result; return Error::ok;
}
