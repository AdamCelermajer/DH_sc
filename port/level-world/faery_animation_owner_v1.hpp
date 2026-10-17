#pragma once

#include "../game-data/animation_scheduler.hpp"
#include "../engine-animation/events.hpp"
#include "visual_timeline.hpp"

#include <cstdint>
#include <string>

namespace dh2::character_faery_visual_facade_v1 {
struct CharacterVisual;
}

namespace dh2::faery_animation_owner_v1 {

using Identity = std::uintptr_t;

enum class Phase : std::uint32_t {
    scene = 1,
    completion = 2,
    replay = 3,
};

// This state is embedded in the canonical CharacterVisual on the factory
// Record. The source frame timestamp is supplied by the existing scene clock.
struct State {
    Identity character{};
    data::AnimationScheduler scheduler;
    timeline::State timeline{};
    timeline::Completion completion{};
    std::int32_t source_sequence{-1};
    std::int32_t animation_dictionary{-1};
    std::int32_t event_dictionary{-1};
    std::uint32_t root_timestamp{};
    // CharAnimator +0x48: event-34 latch. The source RaiseEvent payload is null.
    std::uint8_t sequence_closed{};
    std::uint32_t processing_completion{};
    std::int32_t pending_sequence{-1};
    float pending_speed{1.f};
    animation::EventCursor event_cursor{};
    std::uint64_t event_generation{};

    State() noexcept {
        timeline.scale = 1.f;
        timeline.library_present = 1;
        timeline.clip_index = -1;
    }
};

// The renderer/Character owners supply per-AnimDict interval and pose/event
// operations. `event` forwards the source CharAnimator notification through
// the already-retained Character Coordinator. No data or clock is owned by
// these callbacks.
struct Services {
    void* context{};
    bool (*interval)(void*, Identity, std::int32_t animation_dictionary,
                    std::int32_t* start_ms, std::int32_t* end_ms,
                    std::string& error){};
    bool (*sample)(void*, Identity, std::int32_t animation_dictionary,
                   std::int32_t previous_ms, std::int32_t current_ms,
                   std::int32_t start_ms, std::int32_t end_ms, Phase,
                   std::string& error){};
    void (*event)(void*, Identity, std::uint32_t source_event,
                  std::int32_t source_sequence,
                  std::int32_t animation_dictionary,
                  std::uintptr_t source_payload){};
    bool (*has_pending)(void*, Identity){};
    bool (*event_track)(void*, Identity, std::int32_t animation_dictionary,
                        const animation::EventView** track,
                        std::string& error){};
    void (*triggered)(void*, Identity, std::int32_t animation_dictionary,
                      std::int32_t lag_ms, const char* event_name, Phase){};
};

enum class Status : std::int32_t {
    complete,
    queued_during_completion,
    invalid_argument,
    identity_mismatch,
    service_unavailable,
    source_rejected,
};

// Select a source AnimationTables sequence on this Character's one animator.
// start() uses the existing AnimationScheduler, including redirects, random
// step choice, source event24/event26 delivery and clip preparation.
Status start(character_faery_visual_facade_v1::CharacterVisual&,
             Identity expected_character, const data::AnimationTables&,
             data::AnimationRandom&, std::int32_t source_sequence,
             std::uint32_t absolute_ms, float global_speed,
             const Services&, std::string& error);

// Called by the existing scene pass with its current absolute timestamp.
// Sampling remains on that pass; it does not allocate a frame clock.
Status scene_phase(character_faery_visual_facade_v1::CharacterVisual&,
                   Identity expected_character, std::uint32_t absolute_ms,
                   float global_speed,
                   const Services&, std::string& error);

// Called by the existing Character/Animator phase after the source scene and
// physics phases. A reached clip end advances the same retained scheduler and
// emits its authored source callbacks. `random` is the existing game RNG.
Status animator_phase(character_faery_visual_facade_v1::CharacterVisual&,
                      Identity expected_character,
                      const data::AnimationTables&, data::AnimationRandom&,
                      float global_speed, const Services&, std::string& error);

// Source CharAnimator::ANIM_Stop: reset nested scheduler depth, stop its one
// selected controller clip, preserve the last sampled pose, and raise source
// event 34 once with its null payload. The
// retained renderer pose is sampled manually, so stopping the scheduler and
// freezing that pose is its controller-stop operation.
Status stop(character_faery_visual_facade_v1::CharacterVisual&,
            Identity expected_character, const Services&, std::string& error);

bool retire(character_faery_visual_facade_v1::CharacterVisual&,
            Identity expected_character) noexcept;

} // namespace dh2::faery_animation_owner_v1
