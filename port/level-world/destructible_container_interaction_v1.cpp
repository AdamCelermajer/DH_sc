#include "destructible_container_interaction_v1.hpp"

namespace dh2::level_world::destructible_container_interaction_v1 {
namespace {

bool required(bool (*callback)(void*, std::string&), void* context,
              std::string& error, const char* message) noexcept {
    if (!callback) {
        error = message;
        return false;
    }
    if (!callback(context, error)) {
        if (error.empty()) error = message;
        return false;
    }
    return true;
}

} // namespace

Status interact(State& state, const Services& services, Result& output,
                std::string& error) noexcept {
    output = {};
    error.clear();
    if (!state.identity || !state.attacker || state.data_id < 0 ||
        state.initial_hits < 0 || state.remaining_hits < 0 ||
        state.remaining_hits > state.initial_hits || state.container_state < 0 ||
        state.container_state > 4) {
        error = "DestructibleContainer Interact source facts are invalid";
        output.status = Status::invalid_argument;
        return output.status;
    }

    if (state.remaining_hits > 0) {
        --state.remaining_hits;
        output.hit_counter_decremented = true;
        if (state.visual_present) {
            if (!services.set_hit_animation_frame) {
                error = "DestructibleContainer live VisualObject timeline is unavailable";
                output.status = Status::source_callback_failed;
                return output.status;
            }
            // Source field +0x444 stores the initial counter, while +0x445
            // is decremented on each hit. The timeline frame is their delta.
            if (!services.set_hit_animation_frame(services.context,
                    state.initial_hits - state.remaining_hits, error)) {
                if (error.empty()) error = "DestructibleContainer hit timeline failed";
                output.status = Status::source_callback_failed;
                return output.status;
            }
        }
        if (state.interact_sound >= 0) {
            if (!services.play_hit_sound ||
                !services.play_hit_sound(services.context,
                                          state.interact_sound, error)) {
                if (error.empty()) error = "DestructibleContainer hit sound failed";
                output.status = Status::source_callback_failed;
                return output.status;
            }
        }
        output.status = Status::complete;
        return output.status;
    }

    bool level_present = false;
    if (!services.current_level ||
        !services.current_level(services.context, level_present, error)) {
        if (error.empty()) error = "Application::GetCurrentLevel failed for DestructibleContainer";
        output.status = Status::source_callback_failed;
        return output.status;
    }
    if (!level_present) {
        error = "DestructibleContainer::Interact reached without a current Level";
        output.status = Status::current_level_missing;
        return output.status;
    }

    std::int32_t event_type = -1;
    if (!services.destroy_game_object_constant ||
        !services.destroy_game_object_constant(services.context, event_type, error)) {
        if (error.empty()) error = "v2QuestObjectiveType.DestroyGameObject constant is unavailable";
        output.status = Status::source_callback_failed;
        return output.status;
    }
    events::Event event{};
    event.objective_type = event_type;
    event.character = state.attacker;
    event.network_id = state.network_id;
    event.flag0 = 0;
    event.flag1 = 0;
    event.subject_id = -1;
    event.source_data_id = state.data_id;
    if (!services.raise_current_level_event ||
        !services.raise_current_level_event(services.context, event, error)) {
        if (error.empty()) error = "Current Level DestroyGameObject RaiseAsync failed";
        output.status = Status::event_raise_failed;
        return output.status;
    }
    output.destroy_event_raised = true;

    // Container::Interact checks state 3/4 only after the derived event.
    if (state.container_state == 3 || state.container_state == 4) {
        output.already_open = true;
        output.status = Status::complete;
        return output.status;
    }
    if (!services.set_opener ||
        !services.set_opener(services.context, state.attacker, error)) {
        if (error.empty()) error = "Container opener publication failed";
        output.status = Status::source_callback_failed;
        return output.status;
    }
    if (!services.set_container_state ||
        !services.set_container_state(services.context, 4, error)) {
        if (error.empty()) error = "Container::SetState(4) failed";
        output.status = Status::source_callback_failed;
        return output.status;
    }
    state.container_state = 4;

    if (!state.keep_physics &&
        !required(services.remove_physical_object, services.context, error,
                  "Container physical object removal failed")) {
        output.status = Status::source_callback_failed;
        return output.status;
    }
    if (state.visual_present) {
        if (!services.set_container_state(services.context, 3, error)) {
            if (error.empty()) error = "Container::SetState(3) failed";
            output.status = Status::source_callback_failed;
            return output.status;
        }
        state.container_state = 3;
        if (!services.play_activate_animation ||
            !services.play_activate_animation(services.context, error)) {
            if (error.empty()) error = "Container activate animation failed";
            output.status = Status::source_callback_failed;
            return output.status;
        }
    } else if (!required(services.do_open, services.context, error,
                        "Container::DoOpen failed")) {
        output.status = Status::source_callback_failed;
        return output.status;
    }
    if (state.interact_sound >= 0 &&
        (!services.play_interact_sound ||
         !services.play_interact_sound(services.context,
                                       state.interact_sound, error))) {
        if (error.empty()) error = "Container interaction sound failed";
        output.status = Status::source_callback_failed;
        return output.status;
    }
    output.opened = true;
    output.status = Status::complete;
    return output.status;
}

Status raise_opened_event(std::int32_t network_id, std::int32_t data_id,
                          const OpenedEventServices& services,
                          std::string& error) noexcept {
    error.clear();
    if (network_id < 0 || data_id < 0) {
        error = "DestructibleContainer opened-event source identity is invalid";
        return Status::invalid_argument;
    }
    bool level_present = false;
    if (!services.current_level ||
        !services.current_level(services.context, level_present, error)) {
        if (error.empty()) error = "Application::GetCurrentLevel failed for Container opened event";
        return Status::source_callback_failed;
    }
    if (!level_present) {
        error = "Container opened event reached without a current Level";
        return Status::current_level_missing;
    }
    std::int32_t event_type = -1;
    if (!services.destroy_game_object_constant ||
        !services.destroy_game_object_constant(services.context, event_type, error)) {
        if (error.empty()) error = "v2QuestObjectiveType.DestroyGameObject constant is unavailable";
        return Status::source_callback_failed;
    }
    events::Event event{};
    event.objective_type = event_type;
    // This callback is emitted by the container animation itself, without an
    // attacker. Keep the null Character and source module/DataID payloads.
    event.character = 0;
    event.network_id = network_id;
    event.flag0 = 0;
    event.flag1 = 0;
    event.subject_id = -1;
    event.source_data_id = data_id;
    if (!services.raise_current_level_event ||
        !services.raise_current_level_event(services.context, event, error)) {
        if (error.empty()) error = "Current Level opened-event RaiseAsync failed";
        return Status::event_raise_failed;
    }
    error.clear();
    return Status::complete;
}

const char* status_name(Status status) noexcept {
    switch (status) {
    case Status::complete: return "complete";
    case Status::invalid_argument: return "invalid_argument";
    case Status::source_callback_failed: return "source_callback_failed";
    case Status::current_level_missing: return "current_level_missing";
    case Status::event_raise_failed: return "event_raise_failed";
    }
    return "unknown";
}

} // namespace dh2::level_world::destructible_container_interaction_v1
