#include "character_aggro_acquisition_prefix.hpp"

#include <cstddef>
#include <cstring>
#include <limits>

namespace dh2::character_aggro_acquisition_prefix {
namespace {
struct Range { std::uintptr_t start, end; };
bool range(const void* pointer, std::size_t size, std::size_t alignment, Range& out) {
    const auto at = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || at % alignment || at > UINTPTR_MAX - size) return false;
    out = {at, at + size}; return true;
}
bool overlap(Range a, Range b) { return a.start < b.end && b.start < a.end; }
bool positive_float(std::uint32_t word) {
    float value;
    static_assert(sizeof(value) == sizeof(word) && std::numeric_limits<float>::is_iec559);
    std::memcpy(&value, &word, sizeof(value));
    return value > 0.0f;  // Original __aeabi_fcmpgt: NaN and both zeros false.
}
struct TimingBridge {
    State* state;
    const Services* services;
    Result* result;
    std::uintptr_t ai;
    static std::int32_t call(void* context, Query query, std::uint32_t* value) {
        auto& bridge = *static_cast<TimingBridge*>(context);
        // This bridge is installed only when the main invoke provider exists.
        ++bridge.result->service_calls;
        return bridge.services->invoke(bridge.services->context, bridge.state,
                                       query, query == Query::is_my_turn ? bridge.ai : 0, value);
    }
    static std::int32_t turn(void* c, character_aggro_delay::State*, std::uint32_t* v) {
        return call(c, Query::is_my_turn, v);
    }
    static std::int32_t debug(void* c, character_aggro_delay::State*, std::uint32_t* v) {
        return call(c, Query::disable_optimization, v);
    }
    static std::int32_t delta(void* c, character_aggro_delay::State*, std::uint32_t* v) {
        return call(c, Query::frame_delta, v);
    }
};
}  // namespace

Status prepare(State* state, dh2_random_state* random, const Services* services, Result* result) {
    Range ranges[4];
    if (!range(state, sizeof(*state), alignof(State), ranges[0]) ||
        !range(services, sizeof(*services), alignof(Services), ranges[1]) ||
        !range(result, sizeof(*result), alignof(Result), ranges[2]) ||
        (random && !range(random, sizeof(*random), alignof(dh2_random_state), ranges[3])))
        return Status::invalid_argument;
    const unsigned range_count = random ? 4 : 3;
    for (unsigned i = 0; i < range_count; ++i)
        for (unsigned j = 0; j < i; ++j)
            if (overlap(ranges[i], ranges[j])) return Status::invalid_argument;
    const auto ai = state->ai;
    if (!ai) return Status::invalid_argument;
    const Services bound = *services;
    *result = {};
    auto done = [&](Decision decision) { result->decision = decision; return Status::complete; };
    auto query = [&](Query operation, std::uintptr_t subject, std::uint32_t& value) {
        if (!subject && operation != Query::disable_optimization && operation != Query::frame_delta)
            return Status::invalid_source_fact;
        if (!bound.invoke) return Status::service_unavailable;
        ++result->service_calls;
        value = 0;
        try {
            return bound.invoke(bound.context, state, operation, subject, &value) ?
                   Status::service_failed : Status::complete;
        } catch (...) { return Status::service_failed; }
    };
    std::uint32_t value = 0;
    auto status = query(Query::is_player, state->owner, value);
    if (status != Status::complete) return status;
    if (value) return done(Decision::skip_player);
    status = query(Query::is_player, state->owner, value);
    if (status != Status::complete) return status;
    if (!value) {
        status = query(Query::is_faerie, state->owner, value);
        if (status != Status::complete) return status;
        if (!value) {
            if (!random) return Status::invalid_argument;
            TimingBridge bridge{state, &bound, result, ai};
            const character_aggro_delay::Services timing_services{
                &bridge, TimingBridge::turn, TimingBridge::debug, TimingBridge::delta};
            const auto timing_status = character_aggro_delay::update(
                &state->timing, random, &timing_services, &result->timing);
            if (timing_status != character_aggro_delay::Status::complete)
                return static_cast<Status>(timing_status);
            if (result->timing.decision == character_aggro_delay::Decision::waiting_turn)
                return done(Decision::waiting_turn);
            if (result->timing.decision == character_aggro_delay::Decision::waiting_delay)
                return done(Decision::waiting_delay);
        }
    }
    status = query(Query::is_npc, state->owner, value);
    if (status != Status::complete) return status;
    if (value) return done(Decision::skip_npc);
    status = query(Query::is_monster, state->owner, value);
    if (status != Status::complete) return status;
    std::uintptr_t type_owner = 0;
    if (value) {
        status = query(Query::is_remotely_updated, state->owner, value);
        if (status != Status::complete) return status;
        if (!value) {
            type_owner = state->owner;
            status = query(Query::target_408_present, type_owner, value);
            if (status != Status::complete) return status;
            if (value) {
                result->decision = Decision::monster_target_branch;
                return Status::unsupported_branch;
            }
        }
    }
    // A local Monster without target preserves the owner already captured for
    // the +0x408 field read. Other paths reload at0x3cf59c before IsFaerie.
    if (!type_owner) type_owner = state->owner;
    status = query(Query::is_faerie, type_owner, value);
    if (status != Status::complete) return status;
    if (value) {
        type_owner = state->owner;
        status = query(Query::target_418_present, type_owner, value);
        if (status != Status::complete) return status;
        if (value) return done(Decision::skip_faerie_target);
    } else {
        type_owner = state->owner;  // conditional source reload at0x3cf5a8
    }
    if (!bound.capture_ai_props) return Status::service_unavailable;
    const AiPropsTable* table = nullptr;
    ++result->table_captures;
    try {
        if (bound.capture_ai_props(bound.context, state, &table)) return Status::service_failed;
    } catch (...) { return Status::service_failed; }
    Range table_range{};
    if (!range(table, sizeof(*table), alignof(AiPropsTable), table_range)) return Status::invalid_source_fact;
    for (unsigned i = 0; i < range_count; ++i)
        if (overlap(ranges[i], table_range)) return Status::invalid_source_fact;
    const auto* rows = table->rows;
    const auto capacity = table->capacity;
    Range rows_range{};
    if (!capacity || capacity > 4096 || !range(rows, sizeof(*rows) * capacity, alignof(AiPropsRow), rows_range))
        return Status::invalid_source_fact;
    for (unsigned i = 0; i < range_count; ++i)
        if (overlap(ranges[i], rows_range)) return Status::invalid_source_fact;
    status = query(Query::get_char_ai_id, type_owner, value);
    if (status != Status::complete) return status;
    if (value >= capacity) return Status::invalid_source_fact;
    const auto* row = rows + value;
    // Original loads fresh owner and row+0x3c BEFORE the Awaiting callback.
    const auto awaiting_owner = state->owner;
    std::uint32_t radius = row->aggro_radius_3c;
    status = query(Query::state_awaiting_to_spawn, awaiting_owner, value);
    if (status != Status::complete) return status;
    if (value) {
        const auto spawn_owner = state->owner;
        std::uint32_t spawn_radius = 0;
        status = query(Query::spawn_radius_143c, spawn_owner, spawn_radius);
        if (status != Status::complete) return status;
        if (positive_float(spawn_radius)) {
            result->list_owner = spawn_owner;
            result->radius_word = spawn_radius;
            return done(Decision::ready_normal_acquisition);
        }
    }
    status = query(Query::has_aggro, ai, value);
    if (status != Status::complete) return status;
    if (!value) radius = row->view_radius_40;  // later row read after HasAggro
    result->list_owner = state->owner;
    if (!result->list_owner) return Status::invalid_source_fact;
    result->radius_word = radius;
    return done(Decision::ready_normal_acquisition);
}

}  // namespace dh2::character_aggro_acquisition_prefix
