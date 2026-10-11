#include "faery_animation_owner_v1.hpp"
#include "character_faery_visual_facade_v1.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

namespace {
using namespace dh2;
using namespace dh2::faery_animation_owner_v1;

struct Capture {
    std::vector<std::int32_t> selected;
    std::vector<std::uint32_t> events;
    std::vector<std::pair<std::int32_t, std::int32_t>> samples;
    character_faery_visual_facade_v1::CharacterVisual* reentrant_visual{};
    const data::AnimationTables* reentrant_tables{};
    data::AnimationRandom* reentrant_random{};
    const Services* reentrant_services{};
    std::int32_t reentrant_sequence{-1};
    bool reenter_from_track{};
    Status reentrant_status{Status::complete};
    std::string reentrant_error;
    character_faery_visual_facade_v1::CharacterVisual* stop_visual{};
    const Services* stop_services{};
    bool stop_on_next_step{};
    Status stop_status{Status::complete};
    std::uintptr_t close_payload{};
    std::uint8_t event_times[4]{50, 0, 0, 0};
    const char* event_names[1]{"clip_marker"};
    animation::EventGroup event_groups[1]{{1, event_names}};
    animation::EventView track{4, 1, event_times, event_groups};
    std::vector<std::tuple<std::int32_t, std::int32_t, std::uint32_t,
                           std::string>> triggered_events;
};

bool event_track(void* opaque, std::uintptr_t, std::int32_t dictionary,
                 const animation::EventView** out, std::string&) {
    if (!opaque || !out || dictionary < 0) return false;
    *out = &static_cast<Capture*>(opaque)->track;
    return true;
}

void triggered(void* opaque, std::uintptr_t, std::int32_t dictionary,
               std::int32_t lag, const char* name, Phase phase) {
    auto& capture = *static_cast<Capture*>(opaque);
    capture.triggered_events.emplace_back(
        dictionary, lag, static_cast<std::uint32_t>(phase), name ? name : "");
    if (capture.reenter_from_track && name &&
        std::string(name) == "clip_marker" &&
        capture.reentrant_sequence >= 0) {
        const auto sequence = capture.reentrant_sequence;
        capture.reentrant_sequence = -1;
        capture.reentrant_status = start(*capture.reentrant_visual,
            capture.reentrant_visual->character, *capture.reentrant_tables,
            *capture.reentrant_random, sequence,
            capture.reentrant_visual->animator.root_timestamp, 1.f,
            *capture.reentrant_services, capture.reentrant_error);
    }
}

bool interval(void* opaque, std::uintptr_t, std::int32_t dictionary,
              std::int32_t* start, std::int32_t* end, std::string&) {
    static_cast<Capture*>(opaque)->selected.push_back(dictionary);
    *start = dictionary == 10 ? 0 : 20;
    *end = dictionary == 10 ? 100 : 220;
    return true;
}

bool sample(void* opaque, std::uintptr_t, std::int32_t dictionary,
            std::int32_t, std::int32_t now, std::int32_t, std::int32_t,
            faery_animation_owner_v1::Phase, std::string&) {
    static_cast<Capture*>(opaque)->samples.emplace_back(dictionary, now);
    return true;
}

void event(void* opaque, std::uintptr_t, std::uint32_t event_id,
           std::int32_t, std::int32_t, std::uintptr_t payload) {
    auto& capture = *static_cast<Capture*>(opaque);
    capture.events.push_back(event_id);
    if (event_id == 0x22) capture.close_payload = payload;
    if (event_id == 0x23 && capture.stop_on_next_step) {
        capture.stop_on_next_step = false;
        std::string stop_error;
        capture.stop_status = stop(*capture.stop_visual,
            capture.stop_visual->character, *capture.stop_services, stop_error);
        assert(stop_error.empty());
    }
    if (event_id == 0x27 && capture.reentrant_sequence >= 0) {
        const auto sequence = capture.reentrant_sequence;
        capture.reentrant_sequence = -1;
        capture.reentrant_status = start(*capture.reentrant_visual,
            capture.reentrant_visual->character, *capture.reentrant_tables,
            *capture.reentrant_random, sequence,
            capture.reentrant_visual->animator.root_timestamp, 1.f,
            *capture.reentrant_services, capture.reentrant_error);
    }
}

} // namespace

int main() {
    using namespace faery_animation_owner_v1;
    character_faery_visual_facade_v1::CharacterVisual visual;
    visual.character = 0x1234;
    visual.anim_set.character = visual.character;
    visual.anim_set.phase = character_anim_set_owner_v1::Phase::ready;
    for (const auto dictionary : {10, 11}) {
        character_anim_set_owner_v1::Registration row;
        row.kind = character_anim_set_owner_v1::RegistrationKind::animation;
        row.source_sequence_id = 0;
        row.animation_dictionary_id = dictionary;
        visual.anim_set.registrations.push_back(row);
    }

    data::AnimationTables tables;
    data::AnimationSequence sequence;
    sequence.type = 1;
    data::AnimationStep first, second;
    first.anim = 10;
    second.anim = 11;
    sequence.steps = {first, second};
    tables.sequences.push_back(sequence);
    data::AnimationRandom random{};
    Capture capture;
    const Services services{&capture, interval, sample, event, nullptr,
                            event_track, triggered};
    std::string error;
    assert(start(visual, visual.character, tables, random, 0, 100, 1.f,
                services, error) == Status::complete);
    assert(error.empty() && visual.animator.animation_dictionary == 10);

    assert(scene_phase(visual, visual.character, 100, 1.f, services, error) ==
           Status::complete);
    assert(scene_phase(visual, visual.character, 201, 1.f, services, error) ==
           Status::complete);
    assert(capture.triggered_events.size() == 1);
    assert(std::get<0>(capture.triggered_events.front()) == 10);
    assert(std::get<3>(capture.triggered_events.front()) == "clip_marker");
    assert(visual.animator.event_cursor.last_entry == 0);
    assert(visual.animator.completion.pending == 1);
    assert(animator_phase(visual, visual.character, tables, random, 1.f,
                          services, error) == Status::complete);
    assert(visual.animator.animation_dictionary == 11);
    assert(visual.clip_sequence == 0);

    assert(scene_phase(visual, visual.character, 202, 1.f, services, error) ==
           Status::complete);
    assert(scene_phase(visual, visual.character, 403, 1.f, services, error) ==
           Status::complete);
    assert(animator_phase(visual, visual.character, tables, random, 1.f,
                          services, error) == Status::complete);
    assert(visual.animator.sequence_closed == 1);
    assert(!visual.animator.scheduler.active());
    assert((capture.selected == std::vector<std::int32_t>{10, 11}));
    assert(std::count(capture.events.begin(), capture.events.end(), 0x22u) == 1);
    assert(visual.clip_time_ms == 220);
    assert(capture.close_payload == reinterpret_cast<std::uintptr_t>(
        &visual.animator.sequence_closed));

    // The same Record rejects cross-Character update calls without mutating
    // its selected clip or cursor.
    const auto prior_clip = visual.animator.animation_dictionary;
    assert(scene_phase(visual, 0x4321, 400, 1.f, services, error) ==
           Status::identity_mismatch);
    assert(visual.animator.animation_dictionary == prior_clip);
    assert(retire(visual, visual.character));
    assert(visual.animator.character == 0);

    // A queued SetAnimation during repeat completion suppresses old repeat
    // initialization and is applied at the common completion tail, even
    // though the repeat leaves the prior scheduler active.
    character_faery_visual_facade_v1::CharacterVisual looping;
    looping.character = 0x2345;
    looping.anim_set.character = looping.character;
    looping.anim_set.phase = character_anim_set_owner_v1::Phase::ready;
    for (const auto [sequence_id, dictionary] :
         {std::pair{0, 10}, std::pair{1, 11}}) {
        character_anim_set_owner_v1::Registration row;
        row.kind = character_anim_set_owner_v1::RegistrationKind::animation;
        row.source_sequence_id = sequence_id;
        row.animation_dictionary_id = dictionary;
        looping.anim_set.registrations.push_back(row);
    }
    data::AnimationTables looping_tables;
    data::AnimationSequence repeated;
    repeated.type = 0;
    repeated.loop = -1;
    data::AnimationStep repeated_step;
    repeated_step.anim = 10;
    repeated.steps.push_back(repeated_step);
    data::AnimationSequence replacement;
    replacement.type = 1;
    data::AnimationStep replacement_step;
    replacement_step.anim = 11;
    replacement.steps.push_back(replacement_step);
    looping_tables.sequences = {repeated, replacement};
    data::AnimationRandom looping_random{};
    Capture looping_capture;
    const Services looping_services{&looping_capture, interval, sample, event, nullptr,
                                    event_track, triggered};
    looping_capture.reentrant_visual = &looping;
    looping_capture.reentrant_tables = &looping_tables;
    looping_capture.reentrant_random = &looping_random;
    looping_capture.reentrant_services = &looping_services;
    looping_capture.reentrant_sequence = 1;
    assert(start(looping, looping.character, looping_tables, looping_random, 0,
                 1000, 1.f, looping_services, error) == Status::complete);
    assert(scene_phase(looping, looping.character, 1000, 1.f,
                       looping_services, error) == Status::complete);
    assert(scene_phase(looping, looping.character, 1201, 1.f,
                       looping_services, error) == Status::complete);
    assert(looping.animator.completion.pending == 1);
    assert(animator_phase(looping, looping.character, looping_tables,
                          looping_random, 1.f, looping_services, error) ==
           Status::complete);
    assert(looping_capture.reentrant_status == Status::queued_during_completion);
    assert(looping.animator.source_sequence == 1);
    assert(looping.animator.scheduler.active());
    assert(looping.animator.animation_dictionary == 11);

    character_faery_visual_facade_v1::CharacterVisual overshoot;
    overshoot.character = 0x3456;
    overshoot.anim_set.character = overshoot.character;
    overshoot.anim_set.phase = character_anim_set_owner_v1::Phase::ready;
    character_anim_set_owner_v1::Registration repeat_registration;
    repeat_registration.kind = character_anim_set_owner_v1::RegistrationKind::animation;
    repeat_registration.source_sequence_id = 0;
    repeat_registration.animation_dictionary_id = 10;
    overshoot.anim_set.registrations.push_back(repeat_registration);
    data::AnimationTables repeat_tables;
    data::AnimationSequence repeat_sequence;
    repeat_sequence.type = 0;
    repeat_sequence.loop = -1;
    data::AnimationStep repeat_step;
    repeat_step.anim = 10;
    repeat_sequence.steps.push_back(repeat_step);
    repeat_tables.sequences.push_back(repeat_sequence);
    data::AnimationRandom repeat_random{};
    Capture repeat_capture;
    const Services repeat_services{&repeat_capture, interval, sample, event, nullptr,
                                   event_track, triggered};
    assert(start(overshoot, overshoot.character, repeat_tables, repeat_random,
                 0, 1000, 1.f, repeat_services, error) == Status::complete);
    assert(scene_phase(overshoot, overshoot.character, 1000, 1.f,
                       repeat_services, error) == Status::complete);
    assert(scene_phase(overshoot, overshoot.character, 1117, 1.f,
                       repeat_services, error) == Status::complete);
    assert(overshoot.animator.completion.pending == 1);
    assert(overshoot.animator.completion.extra_ms == 16);
    assert(animator_phase(overshoot, overshoot.character, repeat_tables,
                          repeat_random, 1.f, repeat_services, error) ==
           Status::complete);
    assert(overshoot.animator.scheduler.active());
    assert(overshoot.animator.timeline.current_ms == 16);
    assert(overshoot.animator.completion.extra_ms == 0);

    // An EventTrack callback can synchronously replace the selected clip. The
    // old cursor must not overwrite the new Animator's reset cursor on return.
    character_faery_visual_facade_v1::CharacterVisual marker_reentry;
    marker_reentry.character = 0x4567;
    marker_reentry.anim_set.character = marker_reentry.character;
    marker_reentry.anim_set.phase = character_anim_set_owner_v1::Phase::ready;
    marker_reentry.anim_set.registrations = looping.anim_set.registrations;
    data::AnimationRandom marker_random{};
    Capture marker_capture;
    const Services marker_services{&marker_capture, interval, sample, event,
        nullptr, event_track, triggered};
    marker_capture.reentrant_visual = &marker_reentry;
    marker_capture.reentrant_tables = &looping_tables;
    marker_capture.reentrant_random = &marker_random;
    marker_capture.reentrant_services = &marker_services;
    marker_capture.reentrant_sequence = 1;
    marker_capture.reenter_from_track = true;
    assert(start(marker_reentry, marker_reentry.character, looping_tables,
                 marker_random, 0, 2000, 1.f, marker_services, error) ==
           Status::complete);
    assert(scene_phase(marker_reentry, marker_reentry.character, 2000, 1.f,
                       marker_services, error) == Status::complete);
    assert(scene_phase(marker_reentry, marker_reentry.character, 2051, 1.f,
                       marker_services, error) == Status::complete);
    assert(marker_capture.reentrant_status == Status::complete);
    assert(marker_reentry.animator.source_sequence == 1);
    assert(marker_reentry.animator.animation_dictionary == 11);
    assert(marker_reentry.animator.event_cursor.last_entry == -1);

    // ANIM_Stop during source event35 (0x23) resets child depth, stops the
    // same scheduler, emits source event34 (0x22) once with the stable close latch,
    // and prevents the stale completion pass selecting step 2.
    character_faery_visual_facade_v1::CharacterVisual stopped;
    stopped.character = 0x5678;
    stopped.anim_set.character = stopped.character;
    stopped.anim_set.phase = character_anim_set_owner_v1::Phase::ready;
    for (const auto dictionary : {10, 11}) {
        character_anim_set_owner_v1::Registration row;
        row.kind = character_anim_set_owner_v1::RegistrationKind::animation;
        row.source_sequence_id = 0;
        row.animation_dictionary_id = dictionary;
        stopped.anim_set.registrations.push_back(row);
    }
    data::AnimationTables stop_tables;
    data::AnimationSequence stop_root, stop_sequence;
    stop_root.type = 0;
    data::AnimationStep stop_redirect;
    stop_redirect.redir = 1;
    stop_redirect.anim = 1;
    stop_root.steps.push_back(stop_redirect);
    stop_sequence.type = 1;
    data::AnimationStep stop_first, stop_second;
    stop_first.anim = 10;
    stop_second.anim = 11;
    stop_sequence.steps = {stop_first, stop_second};
    stop_tables.sequences = {stop_root, stop_sequence};
    data::AnimationRandom stop_random{};
    Capture stop_capture;
    const Services stop_services{&stop_capture, interval, sample, event, nullptr,
                                 event_track, triggered};
    stop_capture.stop_visual = &stopped;
    stop_capture.stop_services = &stop_services;
    stop_capture.stop_on_next_step = true;
    assert(start(stopped, stopped.character, stop_tables, stop_random, 0,
                 3000, 1.f, stop_services, error) == Status::complete);
    assert(scene_phase(stopped, stopped.character, 3000, 1.f,
                       stop_services, error) == Status::complete);
    assert(scene_phase(stopped, stopped.character, 3201, 1.f,
                       stop_services, error) == Status::complete);
    assert(stopped.animator.scheduler.frames().size() == 2);
    const auto samples_before_stop = stop_capture.samples.size();
    assert(stopped.animator.completion.pending == 1);
    assert(animator_phase(stopped, stopped.character, stop_tables, stop_random,
                          1.f, stop_services, error) == Status::complete);
    assert(stop_capture.stop_status == Status::complete);
    assert(!stopped.animator.scheduler.active());
    assert(stopped.animator.scheduler.frames().size() == 1);
    assert(stopped.animator.animation_dictionary == 10);
    assert(stopped.animator.sequence_closed == 1);
    assert(stop_capture.close_payload == 0);
    assert(std::count(stop_capture.events.begin(), stop_capture.events.end(),
                      0x22u) == 1);
    assert(std::find(stop_capture.selected.begin(), stop_capture.selected.end(),
                     11) == stop_capture.selected.end());
    assert(scene_phase(stopped, stopped.character, 3300, 1.f,
                       stop_services, error) == Status::complete);
    assert(stop_capture.samples.size() == samples_before_stop);
    assert(stopped.clip_time_ms == 100);

    // A later ANIM_Set is the source _SetAnimStep latch reset and starts the
    // same scheduler again on the retained CharacterVisual.
    assert(start(stopped, stopped.character, stop_tables, stop_random, 0,
                 3300, 1.f, stop_services, error) == Status::complete);
    assert(stopped.animator.scheduler.active());
    assert(stopped.animator.sequence_closed == 0);
}
