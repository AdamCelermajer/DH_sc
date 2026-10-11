#include "faery_animation_owner_v1.hpp"

#include "character_faery_visual_facade_v1.hpp"
#include "character_animation_stop_v1.hpp"

#include <cmath>
#include <cstring>

namespace dh2::faery_animation_owner_v1 {
namespace {
using Visual = character_faery_visual_facade_v1::CharacterVisual;

struct Context {
    Visual* visual{};
    const data::AnimationTables* tables{};
    data::AnimationRandom* random{};
    const Services* services{};
    float global_speed{1.f};
    bool ok{true};
    std::string* error{};
};

void timeline_ending(void* raw, timeline::State* timeline_state);

struct TrackDispatch {
    const Services* services{};
    Identity character{};
    std::int32_t dictionary{-1};
    Phase phase{Phase::scene};
};

void triggered_event(const animation::TriggeredEvent* input, void* raw) {
    auto& dispatch = *static_cast<TrackDispatch*>(raw);
    if (input && dispatch.services->triggered)
        dispatch.services->triggered(dispatch.services->context,
            dispatch.character, dispatch.dictionary, input->lag_ms,
            input->name, dispatch.phase);
}

bool dispatch_track(Visual& visual, std::int32_t dictionary,
                    std::int32_t previous, std::int32_t current,
                    Phase phase, const Services& services,
                    std::string& error) {
    if (!services.event_track) return true;
    const animation::EventView* track = nullptr;
    if (!services.event_track(services.context, visual.animator.character,
            dictionary, &track, error) || !track || !dh2_events_validate(track)) {
        if (error.empty()) error = "Faery AnimDict EventTrack lookup failed";
        return false;
    }
    auto& state = visual.animator;
    if (state.event_dictionary != dictionary) {
        state.event_dictionary = dictionary;
        state.event_cursor = {};
        ++state.event_generation;
    }
    auto cursor = state.event_cursor;
    const auto generation = state.event_generation;
    TrackDispatch dispatch{&services, state.character, dictionary, phase};
    if (!dh2_events_update(track, &cursor, previous, current,
            state.timeline.start_ms, state.timeline.end_ms,
            triggered_event, &dispatch)) {
        error = "Faery AnimDict EventTrack rejected the source time interval";
        return false;
    }
    if (generation == state.event_generation) state.event_cursor = cursor;
    return true;
}

bool finite_speed(float value) noexcept {
    return std::isfinite(value) && value >= 0.f;
}

struct ReplayContext {
    Visual* visual{};
    const Services* services{};
    std::string* error{};
    bool ok{true};
};

struct StopDispatch {
    const Services* services{};
    std::int32_t animation_dictionary{-1};
};

bool raise_stop_event(void* raw, Identity character, std::uint32_t event,
                      std::int32_t source_sequence,
                      std::uintptr_t payload, std::string& error) {
    auto& dispatch = *static_cast<StopDispatch*>(raw);
    if (!dispatch.services->event) {
        error = "Faery ANIM_Stop has no same-Record Character event adapter";
        return false;
    }
    dispatch.services->event(dispatch.services->context, character, event,
        source_sequence, dispatch.animation_dictionary, payload);
    return true;
}

void replay_event(void* raw, std::uint32_t event, timeline::State*,
                  const timeline::ReplayResult* result) {
    auto& context = *static_cast<ReplayContext*>(raw);
    if (event != timeline::new_animation || !result || !result->restart) return;
    auto& visual = *context.visual;
    auto& state = visual.animator;
    const auto previous = state.timeline.current_ms;
    std::int32_t timestamp = 0;
    std::memcpy(&timestamp, &result->timestamp, sizeof(timestamp));
    const timeline::Services ending{&visual, timeline_ending};
    if (dh2_timeline_update(&state.timeline, timestamp, &ending) != 0) {
        *context.error = "Faery Animator replay rejected the source timestamp";
        context.ok = false;
        return;
    }
    if (!dispatch_track(visual, state.animation_dictionary, previous,
            state.timeline.current_ms, Phase::replay, *context.services,
            *context.error)) {
        context.ok = false;
        return;
    }
    visual.clip_time_ms = static_cast<std::uint32_t>(state.timeline.current_ms);
    if (!context.services->sample(context.services->context, state.character,
            state.animation_dictionary, previous, state.timeline.current_ms,
            state.timeline.start_ms, state.timeline.end_ms, Phase::replay,
            *context.error)) {
        if (context.error->empty())
            *context.error = "Faery Animator replay pose/event provider failed";
        context.ok = false;
    }
}

bool registered(const Visual& visual, std::int32_t source_sequence,
                std::int32_t animation_dictionary) noexcept {
    if (!visual.anim_set.character ||
        visual.anim_set.character != visual.character ||
        visual.anim_set.phase != character_anim_set_owner_v1::Phase::ready)
        return false;
    for (const auto& registration : visual.anim_set.registrations)
        if (registration.kind == character_anim_set_owner_v1::RegistrationKind::animation &&
            registration.source_sequence_id == source_sequence &&
            registration.animation_dictionary_id == animation_dictionary)
            return true;
    return false;
}

std::int32_t root_sequence(const State& state) noexcept {
    const auto& frames = state.scheduler.frames();
    return frames.empty() ? state.source_sequence : frames.front().sequence;
}

bool select_clip(Context& context) {
    auto& visual = *context.visual;
    auto& state = visual.animator;
    const auto animation_dictionary = state.scheduler.clip().anim;
    if (animation_dictionary < 0 ||
        !registered(visual, root_sequence(state), animation_dictionary)) {
        *context.error = "Faery source Animator selected an AnimDict outside its retained AnimSet";
        return false;
    }
    if (!context.services->interval) {
        *context.error = "Faery source Animator clip interval provider is unavailable";
        return false;
    }
    std::int32_t start = 0, end = 0;
    if (!context.services->interval(context.services->context, state.character,
            animation_dictionary, &start, &end, *context.error)) {
        if (context.error->empty())
            *context.error = "Faery source Animator clip interval provider failed";
        return false;
    }
    const auto previous_dictionary = state.animation_dictionary;
    if (end <= start || (previous_dictionary != animation_dictionary &&
            dh2_timeline_clip(&state.timeline, animation_dictionary, start, end) != 0)) {
        *context.error = "Faery source Animator rejected an empty clip interval";
        return false;
    }
    state.animation_dictionary = animation_dictionary;
    state.event_dictionary = animation_dictionary;
    state.event_cursor = {};
    ++state.event_generation;
    // _SetAnimStep clears CharAnimator's +0x48 event-34 latch for each
    // successfully selected clip, including a redirect/repeat selection.
    state.sequence_closed = 0;
    ReplayContext replay{&visual, context.services, context.error, true};
    const timeline::ReplayFacts facts{
        previous_dictionary, animation_dictionary, state.completion.extra_ms,
        static_cast<std::uint32_t>(context.visual->animator.scheduler.clip().move_go),
        0, visual.pose ? 1u : 0u, state.root_timestamp, 0};
    const timeline::ReplayServices replay_services{&replay, replay_event};
    timeline::ReplayResult result{};
    if (dh2_timeline_replay(&result, &state.timeline, &facts,
                            &replay_services) != 1 || !replay.ok) {
        if (context.error->empty())
            *context.error = "Faery source Animator replay rejected clip selection";
        return false;
    }
    state.completion.extra_ms = 0;
    const float speed = context.global_speed * state.scheduler.clip().speed;
    if (!finite_speed(speed) || dh2_timeline_scale(&state.timeline, speed) != 0) {
        *context.error = "Faery source Animator rejected clip speed";
        return false;
    }
    return true;
}

void select_event(void* raw, data::AnimationScheduler&, std::uint32_t event) {
    auto& context = *static_cast<Context*>(raw);
    const auto& state = context.visual->animator;
    if (event == 0x22) {
        context.visual->animator.sequence_closed = 1;
    }
    const auto payload = event == 0x22
        ? reinterpret_cast<std::uintptr_t>(
              &context.visual->animator.sequence_closed)
        : std::uintptr_t{0};
    if (context.services->event)
        context.services->event(context.services->context, state.character,
            event, root_sequence(state), state.animation_dictionary, payload);
}

bool prepare(void* raw, data::AnimationScheduler&) {
    auto& context = *static_cast<Context*>(raw);
    context.ok = select_clip(context);
    return context.ok;
}

bool frame_step(void* raw, data::AnimationScheduler&, std::uint32_t, std::uint32_t) {
    return static_cast<Context*>(raw)->ok;
}

bool has_pending(void* raw) {
    auto& context = *static_cast<Context*>(raw);
    const auto& state = context.visual->animator;
    return state.pending_sequence >= 0 ||
        (context.services->has_pending && context.services->has_pending(
            context.services->context, state.character));
}

bool finish(void* raw, data::AnimationScheduler&) {
    return static_cast<Context*>(raw)->ok;
}

void timeline_ending(void* raw, timeline::State* timeline_state) {
    auto& visual = *static_cast<Visual*>(raw);
    dh2_timeline_notify(&visual.animator.completion, timeline_state);
}

Status validate(Visual& visual, Identity expected, const Services& services,
                std::string& error) {
    error.clear();
    if (!expected) {
        error = "Faery Animator requires a live Character identity";
        return Status::invalid_argument;
    }
    if (visual.character != expected) {
        error = "Faery Animator CharacterVisual identity does not match its factory Record";
        return Status::identity_mismatch;
    }
    if (!services.interval || !services.sample || !services.event_track ||
        !services.triggered) {
        error = "Faery Animator requires retained AnimDict interval, pose, and EventTrack providers";
        return Status::service_unavailable;
    }
    auto& state = visual.animator;
    if (state.character && state.character != expected) {
        error = "Faery Animator state belongs to a different Character";
        return Status::identity_mismatch;
    }
    state.character = expected;
    return Status::complete;
}

Status sample_current(Visual& visual, std::uint32_t timestamp,
                      const Services& services, Phase phase,
                      std::int32_t previous, std::string& error) {
    auto& state = visual.animator;
    if (!state.character || state.animation_dictionary < 0) {
        error = "Faery Animator has no selected source AnimDict";
        return Status::source_rejected;
    }
    if (!registered(visual, root_sequence(state), state.animation_dictionary)) {
        error = "Faery Animator selected AnimDict is no longer registered";
        return Status::source_rejected;
    }
    visual.clip_time_ms = static_cast<std::uint32_t>(state.timeline.current_ms);
    if (!services.sample(services.context, state.character,
            state.animation_dictionary, previous, state.timeline.current_ms,
            state.timeline.start_ms, state.timeline.end_ms, phase, error)) {
        if (error.empty()) error = "Faery Animator pose/event provider failed";
        return Status::source_rejected;
    }
    state.root_timestamp = timestamp;
    return Status::complete;
}

} // namespace

Status start(Visual& visual, Identity expected,
             const data::AnimationTables& tables, data::AnimationRandom& random,
             std::int32_t sequence, std::uint32_t absolute_ms,
             float global_speed, const Services& services,
             std::string& error) {
    const auto checked = validate(visual, expected, services, error);
    if (checked != Status::complete) return checked;
    if (sequence < 0 || static_cast<std::size_t>(sequence) >= tables.sequences.size() ||
        !finite_speed(global_speed)) {
        error = "Faery Animator source sequence or global speed is invalid";
        return Status::invalid_argument;
    }
    auto& state = visual.animator;
    if (state.processing_completion) {
        state.pending_sequence = sequence;
        state.pending_speed = global_speed;
        return Status::queued_during_completion;
    }
    state.source_sequence = sequence;
    state.animation_dictionary = -1;
    state.completion = {};
    state.sequence_closed = 0;
    state.pending_sequence = -1;
    state.root_timestamp = absolute_ms;
    Context context{&visual, &tables, &random, &services, global_speed, true, &error};
    const data::AnimationSelectionServices selection{
        &context, select_event, prepare, frame_step};
    if (!state.scheduler.start_with_services(tables, sequence, random, error,
                                              selection))
        return Status::source_rejected;
    visual.clip_sequence = sequence;
    return context.ok ? Status::complete : Status::source_rejected;
}

Status scene_phase(Visual& visual, Identity expected, std::uint32_t absolute_ms,
                   float global_speed, const Services& services,
                   std::string& error) {
    const auto checked = validate(visual, expected, services, error);
    if (checked != Status::complete) return checked;
    auto& state = visual.animator;
    if (!state.scheduler.active()) {
        if (state.animation_dictionary >= 0) return Status::complete;
        error = "Faery Animator has no active sequence";
        return Status::source_rejected;
    }
    if (!finite_speed(global_speed) || !finite_speed(state.scheduler.clip().speed) ||
        dh2_timeline_scale(&state.timeline,
            global_speed * state.scheduler.clip().speed) != 0) {
        error = "Faery Animator global speed is invalid";
        return Status::invalid_argument;
    }
    const auto previous = state.timeline.current_ms;
    const auto dictionary = state.animation_dictionary;
    std::int32_t signed_timestamp = 0;
    std::memcpy(&signed_timestamp, &absolute_ms, sizeof(signed_timestamp));
    const timeline::Services ending{&visual, timeline_ending};
    if (dh2_timeline_update(&state.timeline, signed_timestamp, &ending) != 0) {
        error = "Faery Animator timeline rejected the source frame timestamp";
        return Status::source_rejected;
    }
    if (!dispatch_track(visual, dictionary, previous, state.timeline.current_ms,
                        Phase::scene, services, error))
        return Status::source_rejected;
    return sample_current(visual, absolute_ms, services, Phase::scene,
                         previous, error);
}

Status animator_phase(Visual& visual, Identity expected,
                      const data::AnimationTables& tables,
                      data::AnimationRandom& random, float global_speed,
                      const Services& services, std::string& error) {
    const auto checked = validate(visual, expected, services, error);
    if (checked != Status::complete) return checked;
    auto& state = visual.animator;
    if (!finite_speed(global_speed)) {
        error = "Faery Animator global speed is invalid";
        return Status::invalid_argument;
    }
    if (!state.scheduler.active()) {
        state.completion.pending = 0;
        return Status::complete;
    }
    if (!state.completion.pending) {
        const float speed = global_speed * state.scheduler.clip().speed;
        if (!finite_speed(speed) || dh2_timeline_scale(&state.timeline, speed) != 0) {
            error = "Faery Animator clip speed is invalid";
            return Status::source_rejected;
        }
        return Status::complete;
    }

    state.completion.pending = 0;
    state.processing_completion = 1;
    Context context{&visual, &tables, &random, &services, global_speed, true, &error};
    const data::AnimationSelectionServices selection{
        &context, select_event, prepare, frame_step};
    const data::AnimationCompletionServices completion{
        &context, select_event, has_pending, prepare, finish, nullptr, &selection};
    bool completed = false;
    try {
        completed = state.scheduler.complete_with_services(
            tables, random, error, completion);
    } catch (...) {
        state.processing_completion = 0;
        throw;
    }
    state.processing_completion = 0;
    if (!completed || !context.ok) {
        if (error.empty()) error = "Faery Animator completion failed";
        return Status::source_rejected;
    }

    if (!state.scheduler.active()) {
        if (!state.sequence_closed) {
            state.sequence_closed = 1;
            if (services.event)
                services.event(services.context, expected, 0x22,
                               root_sequence(state), state.animation_dictionary,
                               reinterpret_cast<std::uintptr_t>(
                                   &state.sequence_closed));
        }
    } else {
        visual.clip_sequence = root_sequence(state);
    }
    // A reentrant SetAnimation queued during event27/event23 must run at the
    // common completion tail even when repeat handling left the old scheduler
    // active. The source Animator skips old Initialize when has_pending is set,
    // then applies this request after the old synchronous callback chain.
    if (state.pending_sequence >= 0) {
        const auto sequence = state.pending_sequence;
        const auto speed = state.pending_speed;
        state.pending_sequence = -1;
        return start(visual, expected, tables, random, sequence,
                     state.root_timestamp, speed, services, error);
    }
    return Status::complete;
}

Status stop(Visual& visual, Identity expected, const Services& services,
            std::string& error) {
    const auto checked = validate(visual, expected, services, error);
    if (checked != Status::complete) return checked;
    auto& state = visual.animator;

    state.completion.pending = 0;

    // The source calls AnimController::StopClip(true, 0) only when the
    // Character has its VisualObject/controller. This port's retained pose is
    // a manually sampled Resource::Player, so disabling this one scheduler
    // freezes the current sampled transform without allocating another clock.
    StopDispatch dispatch{&services, state.animation_dictionary};
    const character_animation_stop_v1::Services stop_services{
        &dispatch, raise_stop_event};
    const auto stopped = character_animation_stop_v1::stop(state.scheduler,
        state.character, expected, state.sequence_closed,
        visual.pose != nullptr, stop_services, error);
    switch (stopped) {
    case character_animation_stop_v1::Status::complete:
        return Status::complete;
    case character_animation_stop_v1::Status::invalid_argument:
        return Status::invalid_argument;
    case character_animation_stop_v1::Status::identity_mismatch:
        return Status::identity_mismatch;
    case character_animation_stop_v1::Status::service_unavailable:
    case character_animation_stop_v1::Status::event_rejected:
        return Status::service_unavailable;
    }
    error = "Faery ANIM_Stop returned an unknown source status";
    return Status::source_rejected;
}

bool retire(Visual& visual, Identity expected) noexcept {
    auto& state = visual.animator;
    if (!expected || visual.character != expected ||
        (state.character && state.character != expected))
        return false;
    state = {};
    return true;
}

} // namespace dh2::faery_animation_owner_v1
