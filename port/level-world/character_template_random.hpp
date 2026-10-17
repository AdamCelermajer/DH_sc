#pragma once

#include <cstdint>

extern "C" {
#include "../random/random.h"
}

namespace dh2::character::template_random {

enum class Status : std::uint32_t {
    selected,
    no_alternatives,
    invalid_argument,
};

struct Selection {
    Status status = Status::invalid_argument;
    std::int32_t slot_index = -1;
};

// Models only the uncached, non-player Character template-selection branch.
// `streams` is owned and initialized by the caller. The source path uses the
// ordinary Random::s_seed stream, never the synchronized stream. A zero slot
// count takes the source's no-draw path and does not increment a counter.
// Negative counts, null pointers, and output/state overlap are rejected before
// mutation. Callers must pass the result to the template resolver to map the
// selected slot to its authored CharacterTable ID.
Selection select_uncached_slot(dh2_random_state* streams,
                               std::int32_t slot_count,
                               Selection* output);

const char* status_name(Status);

}  // namespace dh2::character::template_random
