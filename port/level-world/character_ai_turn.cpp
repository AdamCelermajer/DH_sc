#include "character_ai_turn.hpp"

#include <cstddef>
#include <limits>

namespace dh2::character_ai_turn {
namespace {
struct Range { std::uintptr_t begin, end; };
bool range(const void* pointer, std::size_t bytes, Range& out) {
    const auto first = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || first > std::numeric_limits<std::uintptr_t>::max() - bytes)
        return false;
    out = {first, first + bytes};
    return true;
}
bool overlap(Range a, Range b) { return a.begin < b.end && b.begin < a.end; }
bool aligned(const void* pointer, std::uintptr_t alignment) {
    return pointer && reinterpret_cast<std::uintptr_t>(pointer) % alignment == 0;
}
Status query(State* state, const Services& services, Query kind,
             std::uint32_t& value, std::uint32_t& count) {
    if (!services.invoke) return Status::service_unavailable;
    const auto owner = state->owner;
    if (!owner) return Status::invalid_argument;
    ++count;
    try {
        if (services.invoke(services.context, state, kind, owner, &value) != 0)
            return Status::service_failed;
    } catch (...) {
        return Status::service_failed;
    }
    return Status::complete;
}
}  // namespace

Status evaluate(State* state, const Globals* globals,
                const Services* services, Result* result) {
    Range ranges[4];
    if (!aligned(state, alignof(State)) || !aligned(globals, alignof(Globals)) ||
        !aligned(services, alignof(Services)) || !aligned(result, alignof(Result)) ||
        !range(state, sizeof(*state), ranges[0]) ||
        !range(globals, sizeof(*globals), ranges[1]) ||
        !range(services, sizeof(*services), ranges[2]) ||
        !range(result, sizeof(*result), ranges[3])) return Status::invalid_argument;
    for (unsigned i = 0; i < 4; ++i)
        for (unsigned j = i + 1; j < 4; ++j)
            if (overlap(ranges[i], ranges[j])) return Status::invalid_argument;
    if (!state->ai) return Status::invalid_argument;
    *result = {};
    if (!globals->queue_length) return Status::empty_queue_unsupported;
    if (globals->update_timer <= 0) {
        result->queue_front_read = 1;
        if (globals->queue_front == state->ai) {
            result->value = 1;
            return Status::complete;
        }
    }
    const Services captured = *services;
    std::uint32_t value = 0;
    auto status = query(state, captured, Query::is_follower,
                        value, result->follower_queries);
    if (status != Status::complete) return status;
    if (value) { result->value = 1; return Status::complete; }
    status = query(state, captured, Query::is_faerie,
                   value, result->faerie_queries);
    if (status != Status::complete) return status;
    if (value) { result->value = 1; return Status::complete; }
    status = query(state, captured, Query::virtual_is_player,
                   value, result->player_queries);
    if (status == Status::complete) result->value = value;
    return status;
}

}  // namespace dh2::character_ai_turn
