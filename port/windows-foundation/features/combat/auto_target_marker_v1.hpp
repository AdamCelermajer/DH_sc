#pragma once

#include <cstdint>

namespace dh::foundation {

enum class AutoTargetMarkerSourceV1 : std::uint8_t {
    none,
    last_target,
    object_of_interest
};

struct AutoTargetMarkerCandidateV1 {
    std::uintptr_t identity{};
    AutoTargetMarkerSourceV1 source{AutoTargetMarkerSourceV1::none};
};

// CharacterTargetMarkerV28 consumes Character+0x40c (CharAI last target)
// first, then Character+0x14a4 (object of interest) when it is null or self.
// The returned identity still needs the caller's real actor/interaction lookup.
AutoTargetMarkerCandidateV1 resolve_auto_target_marker_v1(
    std::uintptr_t player,
    std::uintptr_t last_target,
    std::uintptr_t object_of_interest) noexcept;

} // namespace dh::foundation
