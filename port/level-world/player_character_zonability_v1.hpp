#pragma once

#include "character_zonability.hpp"

namespace dh2::player_character_zonability_v1 {

// Character::IsZonable returns false at its first virtual query for the
// retained Player Character. This typed adapter feeds that source predicate
// into the shared Character::IsZonable kernel; it owns no zone state.
struct Owner { std::uintptr_t character = 0; };
using Result = character_zonability::Result;
enum class Status : int { complete, invalid_argument, source_failed };

namespace detail {
inline std::int32_t is_player(void* raw, character_zonability::State* state,
                              const character_zonability::Request* request,
                              character_zonability::Response* response) {
    if (!raw || !state || !request || !response) return 1;
    const auto& owner = *static_cast<const Owner*>(raw);
    if (!owner.character || state->character != owner.character ||
        request->character != owner.character || request->reserved ||
        request->operation != character_zonability::Operation::is_player)
        return 1;
    response->word = 1;
    return 0;
}
} // namespace detail

inline Status evaluate(const Owner* owner, Result* result) {
    if (!owner || !result || !owner->character) return Status::invalid_argument;
    character_zonability::State state{owner->character};
    const character_zonability::Services services{const_cast<Owner*>(owner),
                                                   detail::is_player};
    return character_zonability::evaluate(&state, &services, result) ==
                   character_zonability::Status::complete
        ? Status::complete : Status::source_failed;
}

} // namespace dh2::player_character_zonability_v1
