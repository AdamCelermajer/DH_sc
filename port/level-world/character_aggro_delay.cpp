#include "character_aggro_delay.hpp"

#include <cstddef>
#include <cstring>

namespace dh2::character_aggro_delay {
namespace {
std::int32_t signed_word(std::uint32_t value) {
    std::int32_t result;
    std::memcpy(&result, &value, sizeof(result));
    return result;
}
bool overlap(const void* a, std::size_t an, const void* b, std::size_t bn) {
    const auto x = reinterpret_cast<std::uintptr_t>(a);
    const auto y = reinterpret_cast<std::uintptr_t>(b);
    if (x > UINTPTR_MAX - an || y > UINTPTR_MAX - bn) return true;
    return x < y + bn && y < x + an;
}
}  // namespace

Status update(State* state, dh2_random_state* random, const Services* services,
              Result* result) {
    if (!state || !random || !services || !result ||
        reinterpret_cast<std::uintptr_t>(state) % alignof(State) ||
        reinterpret_cast<std::uintptr_t>(random) % alignof(dh2_random_state) ||
        reinterpret_cast<std::uintptr_t>(services) % alignof(Services) ||
        reinterpret_cast<std::uintptr_t>(result) % alignof(Result))
        return Status::invalid_argument;
    const void* objects[] = {state, random, services, result};
    const std::size_t sizes[] = {sizeof(*state), sizeof(*random), sizeof(*services), sizeof(*result)};
    for (unsigned i = 0; i < 4; ++i)
        for (unsigned j = 0; j < i; ++j)
            if (overlap(objects[i], sizes[i], objects[j], sizes[j]))
                return Status::invalid_argument;
    // Portable service-table ownership; services/context may otherwise be
    // borrowed from actor objects. Fields/state are still read at source points.
    const Services bound = *services;
    *result = {};
    auto query = [&](auto callback, std::uint32_t& count, std::uint32_t& value) {
        if (!callback) return Status::service_unavailable;
        ++count;
        value = 0;
        try {
            return callback(bound.context, state, &value) ? Status::service_failed : Status::complete;
        } catch (...) {
            return Status::service_failed;
        }
    };
    auto done = [&](Decision decision) {
        result->decision = decision;
        return Status::complete;
    };

    std::uint32_t value = 0;
    Status status = query(bound.is_my_turn, result->turn_queries, value);
    if (status != Status::complete) return status;
    if (!value && signed_word(state->elapsed_not_turn_ms) < 500) {
        // r8 captures +8 BEFORE the first GetDt. r7 captures +0xc AFTER
        // that completed store and BEFORE the separately fresh second GetDt.
        const auto countdown = state->countdown_ms;
        status = query(bound.frame_delta, result->delta_reads, value);
        if (status != Status::complete) return status;
        state->countdown_ms = countdown - value;
        const auto elapsed = state->elapsed_not_turn_ms;
        status = query(bound.frame_delta, result->delta_reads, value);
        if (status != Status::complete) return status;
        state->elapsed_not_turn_ms = elapsed + value;
        return done(Decision::waiting_turn);
    }

    state->elapsed_not_turn_ms = 0;
    status = query(bound.disable_optimization, result->debug_queries, value);
    if (status != Status::complete) return status;
    if (value) return done(Decision::proceed_to_acquisition);

    const auto countdown = state->countdown_ms;
    if (signed_word(countdown) > 0) {
        status = query(bound.frame_delta, result->delta_reads, value);
        if (status != Status::complete) return status;
        state->countdown_ms = countdown - value;
        if (signed_word(state->countdown_ms) > 0) return done(Decision::waiting_delay);
    }
    // Same ordinary seed/counter as original inline Random draw. Sync stream
    // remains untouched; uint32 multiply/add and counter wrap match ARM words.
    state->countdown_ms = 100u + std::uint32_t(dh2_random_next(random, 200, 0));
    result->ordinary_random_draws = 1;
    return done(Decision::proceed_to_acquisition);
}

}  // namespace dh2::character_aggro_delay
