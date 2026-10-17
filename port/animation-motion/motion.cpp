#include "motion.hpp"
#include <cmath>
using namespace dh2::motion;
namespace {
bool finite(const dh2::math::Vector3f &v) {
    return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z);
}
Error arguments(const State *state, const dh2::math::Vector3f *position) {
    if (!state || !position) return Error::argument;
    const auto a = reinterpret_cast<std::uintptr_t>(state);
    const auto b = reinterpret_cast<std::uintptr_t>(position);
    if (a <= b ? b - a < sizeof(*state) : a - b < sizeof(*position)) return Error::argument;
    return finite(*position) ? Error::ok : Error::nonfinite;
}
}
extern "C" Error dh2_motion_reset(State *state, std::uint32_t timestamp,
                                  const dh2::math::Vector3f *position) {
    const auto error = arguments(state, position);
    if (error != Error::ok) return error;
    State result{timestamp, *position, {0, 0, 0}};
    *state = result; return Error::ok;
}
extern "C" Error dh2_motion_calculate(State *state, std::uint32_t timestamp,
                                      const dh2::math::Vector3f *position) {
    const auto error = arguments(state, position);
    if (error != Error::ok) return error;
    State result{timestamp, *position, {0, 0, 0}};
    if (state->timestamp != timestamp) {
        if (!finite(state->previous)) return Error::nonfinite;
        result.delta.y = position->y - state->previous.y;
        result.delta.z = position->z - state->previous.z;
        result.delta.x = position->x - state->previous.x;
        if (!finite(result.delta)) return Error::nonfinite;
    }
    *state = result; return Error::ok;
}
