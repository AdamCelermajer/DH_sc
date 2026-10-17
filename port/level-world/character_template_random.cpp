#include "character_template_random.hpp"

#include <cstddef>
#include <cstdint>

namespace dh2::character::template_random {
namespace {

bool overlaps(const void* left, std::size_t left_size,
              const void* right, std::size_t right_size) {
    const auto a = reinterpret_cast<std::uintptr_t>(left);
    const auto b = reinterpret_cast<std::uintptr_t>(right);
    if (a > UINTPTR_MAX - left_size || b > UINTPTR_MAX - right_size) return true;
    return left_size && right_size && a < b + right_size && b < a + left_size;
}

Selection invalid() { return {Status::invalid_argument, -1}; }

}  // namespace

Selection select_uncached_slot(dh2_random_state* streams,
                               std::int32_t slot_count,
                               Selection* output) {
    if (!streams || !output || overlaps(streams, sizeof(*streams), output, sizeof(*output)) ||
        slot_count < 0)
        return invalid();

    if (slot_count == 0) {
        const Selection result{Status::no_alternatives, -1};
        *output = result;
        return result;
    }

    // Character::SafeGetCharPropsId uses the ordinary global seed inline.
    // `dh2_random_next` is the independently source-differential-tested port
    // of the same LCG; sync=0 selects seeds[0]/counters[0].
    const auto index = dh2_random_next(streams, static_cast<std::uint32_t>(slot_count), 0);
    const Selection result{Status::selected, index};
    *output = result;
    return result;
}

const char* status_name(Status status) {
    switch (status) {
    case Status::selected: return "selected";
    case Status::no_alternatives: return "no_alternatives";
    case Status::invalid_argument: return "invalid_argument";
    }
    return "unknown";
}

}  // namespace dh2::character::template_random
