#include "character_enemy_spotted.hpp"

#include <cstddef>
#include <cstring>
#include <limits>

namespace dh2::character_enemy_spotted {
namespace {
static_assert(sizeof(float) == 4 && std::numeric_limits<float>::is_iec559);
float floating(std::uint32_t bits) {
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}
bool overlap(const void* a, std::size_t an, const void* b, std::size_t bn) {
    const auto x = reinterpret_cast<std::uintptr_t>(a);
    const auto y = reinterpret_cast<std::uintptr_t>(b);
    if (x > UINTPTR_MAX - an || y > UINTPTR_MAX - bn) return true;
    return x < y + bn && y < x + an;
}
struct EntryHold {
    State* state;
    ~EntryHold() { state->in_progress = 0; }
};
struct ActiveHold {
    const Services& services;
    ActiveAIS active;
    void* hold;
    ~ActiveHold() {
        if (hold) services.release_active(services.context, &active, hold);
    }
};
}  // namespace

Status on_enemy_spotted(State* state, std::uintptr_t enemy,
                       const Services* services, Result* result) {
    if (!state || !services || !result || !enemy) return Status::invalid_argument;
    const void* objects[] = {state, services, result};
    const std::size_t sizes[] = {sizeof(*state), sizeof(*services), sizeof(*result)};
    for (unsigned i = 0; i < 3; ++i)
        for (unsigned j = 0; j < i; ++j)
            if (overlap(objects[i], sizes[i], objects[j], sizes[j]))
                return Status::invalid_argument;
    if (!state->ai_identity) return Status::invalid_argument;
    if (state->in_progress) return Status::reentrant_call;
    const auto ai = state->ai_identity;  // Original r7 is fixed, not reread.
    const Services bound = *services;
    *result = {};
    state->in_progress = 1;
    EntryHold entry{state};
    auto done = [&](Decision decision) {
        result->decision = decision;
        return Status::complete;
    };
    auto query = [&](auto callback, std::uint32_t& count,
                     std::uintptr_t identity, std::uint32_t& value) {
        if (!identity) return Status::invalid_live_projection;
        if (!callback) return Status::service_unavailable;
        ++count;
        value = 0;
        return callback(bound.context, state, identity, &value)
            ? Status::service_failed : Status::complete;
    };
    auto debug = [&](DebugPoint point) {
        if (!bound.debug_switch) return Status::complete;
        ++result->debug_queries;
        return bound.debug_switch(bound.context, state, point)
            ? Status::service_failed : Status::complete;
    };
    Status status = debug(DebugPoint::entry);
    if (status != Status::complete) return status;
    const auto group = state->group_identity;
    if (group) {
        const auto owner = state->owner_identity;
        if (!owner) return Status::invalid_live_projection;
        if (!bound.group_enemy_spotted) return Status::service_unavailable;
        ++result->group_calls;
        if (bound.group_enemy_spotted(bound.context, state, group, owner, enemy))
            return Status::service_failed;
    }

    std::uint32_t value = 0;
    status = query(bound.is_awaiting_to_spawn, result->awaiting_spawn_queries, enemy, value);
    if (status != Status::complete) return status;
    if (value) return done(Decision::enemy_awaiting_spawn);
    status = query(bound.is_awaiting_to_spawn, result->awaiting_spawn_queries, state->owner_identity, value);
    if (status != Status::complete) return status;
    if (value) return done(Decision::owner_awaiting_spawn);
    status = query(bound.is_in_limbus, result->limbus_queries, enemy, value);
    if (status != Status::complete) return status;
    if (value) return done(Decision::enemy_in_limbus);
    status = query(bound.is_in_limbus, result->limbus_queries, state->owner_identity, value);
    if (status != Status::complete) return status;
    if (value) return done(Decision::owner_in_limbus);

    status = query(bound.is_in_combat, result->combat_queries, ai, value);
    if (status != Status::complete) return status;
    bool inspect_aggro = true;
    if (value) {
        status = query(bound.is_player, result->player_queries, enemy, value);
        if (status != Status::complete) return status;
        inspect_aggro = value != 0;
    }
    if (inspect_aggro) {
        if (!bound.get_aggro) return Status::service_unavailable;
        ++result->aggro_queries;
        if (bound.get_aggro(bound.context, state, ai, enemy, &result->aggro_bits))
            return Status::service_failed;
        // Original __aeabi_fcmpeq(value, +0.0f); NaN is not equal, -0 is.
        if (floating(result->aggro_bits) == 0.0f) {
            const auto owner = state->owner_identity;
            if (!owner) return Status::invalid_live_projection;
            if (!bound.initial_aggro) return Status::service_unavailable;
            ++result->initial_aggro_reads;
            std::uint32_t amount = 0;
            if (bound.initial_aggro(bound.context, state, &amount))
                return Status::service_failed;
            if (!bound.add_aggro) return Status::service_unavailable;
            ++result->aggro_adds;
            if (bound.add_aggro(bound.context, state, owner, enemy, amount,
                                &result->added_delta_bits))
                return Status::service_failed;
            // Original __aeabi_fcmpgt(delta, +0.0f). It changes diagnostic
            // calls only; neither branch vetoes the selected AIS callback.
            if (floating(result->added_delta_bits) > 0.0f) {
                status = debug(DebugPoint::positive_aggro_added);
                if (status != Status::complete) return status;
            }
        }
    }

    // Original active AIS load at 0x3d1618 occurs after all previous services.
    const ActiveAIS active = state->active;
    if (!active.identity) return done(Decision::no_active_ais);
    if (!active.callee) return Status::invalid_live_projection;
    if (!bound.retain_active || !bound.dispatch_active || !bound.release_active)
        return Status::service_unavailable;
    ActiveHold selected{bound, active, nullptr};
    ++result->active_retains;
    const auto retained = bound.retain_active(bound.context, state, &selected.active,
                                             &selected.hold);
    if (retained || !selected.hold) return Status::active_lifetime_failed;
    result->dispatched_active = selected.active;
    ++result->active_dispatches;
    if (bound.dispatch_active(bound.context, state, &selected.active,
                              selected.hold, enemy)) return Status::service_failed;
    return done(Decision::dispatched);
}

}  // namespace dh2::character_enemy_spotted
