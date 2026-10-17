#include "character_animation_stop_v1.hpp"

namespace dh2::character_animation_stop_v1 {

Status stop(data::AnimationScheduler& scheduler, Identity actual_character,
            Identity expected_character, std::uint8_t& sequence_closed,
            bool controller_present, const Services& services,
            std::string& error) {
    error.clear();
    if (!expected_character || sequence_closed > 1) {
        error = "Character ANIM_Stop requires a live identity and one-byte close latch";
        return Status::invalid_argument;
    }
    if (actual_character != expected_character) {
        error = "Character ANIM_Stop identity does not match its canonical owner";
        return Status::identity_mismatch;
    }
    const auto& frames_before_stop = scheduler.frames();
    const auto source_sequence = frames_before_stop.empty()
        ? -1 : frames_before_stop.front().sequence;
    scheduler.stop(); // root-depth reset; leave current clip/pose/cursor intact

    if (controller_present && !sequence_closed) {
        sequence_closed = 1;
        constexpr std::uintptr_t payload = 0; // source passes its zeroed R2
        if (!services.raise_event) {
            error = "Character ANIM_Stop has no source event-34 adapter";
            return Status::service_unavailable;
        }
        if (!services.raise_event(services.context, expected_character, 0x22,
                                  source_sequence, payload, error)) {
            if (error.empty()) error = "Character event 34 rejected ANIM_Stop";
            return Status::event_rejected;
        }
    }
    return Status::complete;
}

} // namespace dh2::character_animation_stop_v1
