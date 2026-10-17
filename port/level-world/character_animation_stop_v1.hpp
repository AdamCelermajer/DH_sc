#pragma once

#include "../game-data/animation_scheduler.hpp"

#include <cstdint>
#include <string>

namespace dh2::character_animation_stop_v1 {

using Identity = std::uintptr_t;

enum class Status : std::int32_t {
    complete,
    invalid_argument,
    identity_mismatch,
    service_unavailable,
    event_rejected,
};

// The caller owns the canonical Character/Coordinator and keeps this
// synchronous adapter on that same Record. source_sequence is diagnostic
// owner metadata; Character::RaiseEvent receives event_id and source_payload.
struct Services {
    void* context{};
    bool (*raise_event)(void*, Identity character, std::uint32_t event_id,
                        std::int32_t source_sequence,
                        std::uintptr_t source_payload,
                        std::string& error){};
};

// Source CharAnimator::ANIM_Stop clears nested sequence depth, stops the
// selected controller clip, then raises Character event 34 once when the
// VisualObject controller exists. The existing sampled pose/cursor is retained.
// `sequence_closed` is the stable per-Character +0x48-equivalent latch byte;
// it is set before synchronous delivery. The source event34 payload is null.
Status stop(data::AnimationScheduler&, Identity actual_character,
            Identity expected_character, std::uint8_t& sequence_closed,
            bool controller_present, const Services&, std::string& error);

} // namespace dh2::character_animation_stop_v1
