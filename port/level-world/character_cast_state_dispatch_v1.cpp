#include "character_cast_state_dispatch_v1.hpp"

#include <cstddef>

namespace dh2::character_cast_state_dispatch_v1 {
namespace {
struct Range { std::uintptr_t begin, end; };

bool range(const void* pointer, std::size_t size, std::size_t alignment,
           Range& output) {
    const auto begin = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || begin % alignment || begin > UINTPTR_MAX - size)
        return false;
    output = {begin, begin + size};
    return true;
}

bool overlaps(Range left, Range right) {
    return left.begin < right.end && right.begin < left.end;
}
} // namespace

Status event(const character::State* state, std::uint32_t event_id,
             Result* output) {
    Range state_range{}, output_range{};
    if (!range(state, sizeof(*state), alignof(character::State), state_range) ||
        !range(output, sizeof(*output), alignof(Result), output_range) ||
        overlaps(state_range, output_range))
        return Status::invalid_argument;

    Result result{};
    const auto current = state->current;
    if ((current == 3 || current == 4 || current == 5) &&
        event_id == 50006u) {
        result.next = 7;
        result.registered = 1;
    } else if (current == 7 && event_id == 34u) {
        result.next = 3;
        result.registered = 1;
    } else if (current == 7 && event_id == 50008u) {
        result.next = 12;
        result.registered = 1;
    }

    *output = result;
    return Status::complete;
}

} // namespace dh2::character_cast_state_dispatch_v1
