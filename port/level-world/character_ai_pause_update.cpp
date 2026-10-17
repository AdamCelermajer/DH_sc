#include "character_ai_pause_update.hpp"
namespace dh2::character_ai_pause_update {
Status pause(State* state, std::uint32_t duration, const Services* services, Result* out) {
    if (!state || !state->ai || !state->owner || !state->owner->owner || !out)
        return Status::invalid_argument;
    const auto owner = state->owner->owner;
    state->paused = 1;
    *out = {};
    if (!services || !services->start) return Status::service_unavailable;
    const Request request{state->ai, owner, 0, duration, 0, 0x31, 0x3b4};
    out->called = 1;
    return services->start(services->context, state, &request, &out->timer_id) ?
        Status::service_failed : Status::complete;
}
}
