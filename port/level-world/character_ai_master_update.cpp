#include "character_ai_master_update.hpp"
#include <limits>

namespace dh2::character_ai_master_update {
namespace {
bool overlap(const void* a, std::uintptr_t an, const void* b, std::uintptr_t bn) {
    const auto x = reinterpret_cast<std::uintptr_t>(a), y = reinterpret_cast<std::uintptr_t>(b);
    return x <= y ? y - x < an : x - y < bn;
}
bool range_ok(const void* p, std::uintptr_t size, std::uintptr_t alignment) {
    const auto value = reinterpret_cast<std::uintptr_t>(p);
    return value && value % alignment == 0 &&
        size - 1 <= std::numeric_limits<std::uintptr_t>::max() - value;
}
}
Status update(State* state, const Services* services, Result* result) {
    if (!range_ok(state, sizeof(*state), alignof(State)) ||
        !range_ok(services, sizeof(*services), alignof(Services)) ||
        !range_ok(result, sizeof(*result), alignof(Result)) ||
        overlap(state, sizeof(*state), services, sizeof(*services)) ||
        overlap(state, sizeof(*state), result, sizeof(*result)) ||
        overlap(services, sizeof(*services), result, sizeof(*result)) ||
        !state->ai || !state->owner || state->reserved || !services->invoke)
        return Status::invalid_argument;
    *result = {};
    if (!state->master) return Status::complete;
    const auto ai = state->ai;
    auto call = [&](Operation operation, std::uintptr_t subject, std::uintptr_t peer,
                    std::uint32_t event, std::uint32_t& word) {
        const Request request{operation, event, subject, peer};
        Response response{};
        ++result->calls;
        try {
            if (services->invoke(services->context, state, &request, &response))
                return Status::service_failed;
        } catch (...) { return Status::service_failed; }
        word = response.word;
        result->last_word = word;
        return Status::complete;
    };
    auto emit = [&](std::uint32_t event) {
        std::uint32_t ignored = 0;
        ++result->events;
        result->last_event = event;
        return call(Operation::raise_event, state->owner, state->master, event, ignored);
    };
    std::uint32_t word = 0;
    auto status = call(Operation::char_ai_id, state->owner, 0, 0, word);
    if (status != Status::complete) return status;
    // Original dereferences the freshly reread master vtable here.
    if (!state->master) return Status::invalid_source_fact;
    status = call(Operation::master_is_dead, state->master, 0, 0, word);
    if (status != Status::complete) return status;
    const auto alive = static_cast<std::uint8_t>(word ^ 1u);
    if (state->alive_54 && !alive) status = emit(0x12);
    else if (!state->alive_54 && alive) status = emit(0x13);
    if (status != Status::complete) return status;
    state->alive_54 = alive;
    if (!state->master) return Status::complete;
    status = call(Operation::sight, ai, state->master, 0, word);
    if (status != Status::complete) return status;
    const auto sight = word;
    if (state->sight_55 && !sight) status = emit(0x14);
    else if (!state->sight_55 && sight) status = emit(0x15);
    if (status != Status::complete) return status;
    state->sight_55 = static_cast<std::uint8_t>(sight);
    if (!state->master || !state->alive_54 || !sight) return Status::complete;
    status = call(Operation::my_turn, ai, 0, 0, word);
    if (status != Status::complete || !word) return status;
    status = call(Operation::can_range_attack, state->owner, 0, 0, word);
    if (status != Status::complete) return status;
    std::uint32_t event = 0x16;
    if (word) {
        status = call(Operation::close_range, ai, state->master, 0, word);
        if (status != Status::complete) return status;
        if (word) event = 0x18;
        else {
            status = call(Operation::range, ai, state->master, 0, word);
            if (status != Status::complete) return status;
            if (word) event = 0x17;
        }
    } else {
        status = call(Operation::melee_range, ai, state->master, 0, word);
        if (status != Status::complete) return status;
        if (word) event = 0x19;
    }
    return emit(event);
}
}
