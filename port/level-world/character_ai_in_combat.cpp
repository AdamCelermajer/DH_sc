#include "character_ai_in_combat.hpp"
#include <cstddef>
#include <limits>

namespace dh2::character_ai_in_combat {
namespace {
struct Range { std::uintptr_t first, end; };
bool range(const void* pointer, std::size_t size, std::size_t alignment, Range& out) {
    const auto first = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || first % alignment || first > std::numeric_limits<std::uintptr_t>::max() - size)
        return false;
    out = {first, first + size};
    return true;
}
bool overlap(Range a, Range b) { return a.first < b.end && b.first < a.end; }
}

Status evaluate(State* state, const Services* services, Result* result) {
    Range ranges[3];
    if (!range(state, sizeof(*state), alignof(State), ranges[0]) ||
        !range(services, sizeof(*services), alignof(Services), ranges[1]) ||
        !range(result, sizeof(*result), alignof(Result), ranges[2]) ||
        overlap(ranges[0], ranges[1]) || overlap(ranges[0], ranges[2]) ||
        overlap(ranges[1], ranges[2])) return Status::invalid_argument;
    const auto ai = state->ai;
    if (!ai) return Status::invalid_argument;
    const Services captured = *services;
    *result = {};
    for (std::uint32_t index = 0; index < 5; ++index) {
        const auto query = static_cast<Query>(index);
        const auto subject = index < 2 ? ai : state->owner;
        if (!subject) return Status::invalid_argument;
        if (!captured.invoke) return Status::service_unavailable;
        std::uint32_t word = 0;
        ++result->service_calls;
        result->last_query = index;
        try {
            if (captured.invoke(captured.context, state, query, subject, &word))
                return Status::service_failed;
        } catch (...) { return Status::service_failed; }
        if (index == 4) { result->value = word; return Status::complete; }
        if (word) { result->value = 1; return Status::complete; }
    }
    return Status::complete;
}

}  // namespace dh2::character_ai_in_combat
