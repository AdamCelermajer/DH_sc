#pragma once

#include "player_profile_index_v1.hpp"
#include <array>

namespace dh2::data {

// An explicit four-byte section replacement. This is a raw profile utility;
// it does not dispatch SaveAll callbacks or infer online/offline save behavior.
struct PlayerProfileRawSectionV1 {
    std::array<char, 4> tag{};
    Bytes payload{};
};

// Rebuild a sorted campaign profile from the prior index, replacing or adding
// the supplied sections. Duplicate prior tags retain their last source body;
// duplicate replacements and embedded-NUL tags are rejected. Output changes
// only after the complete bounded profile has been assembled.
bool serialize_player_profile_raw_sections_v1(
    const PlayerProfileIndexV1::Borrow& prior,
    const std::vector<PlayerProfileRawSectionV1>& replacements,
    std::vector<std::uint8_t>& output,
    std::string& error);

}
