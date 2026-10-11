#include "character_interact_v1.hpp"

namespace dh2::character_interact_v1 {

Status interact(const Request& request, const Services& services,
                Result& result, std::string& error) {
    result = {};
    error.clear();
    if (!request.target_character || !request.interactor ||
        request.target_character == request.interactor ||
        request.talk_to_npc_event_type < 0 || request.target_property_id < 0 ||
        request.interaction_animation < 0 || request.target_is_interacting > 1 ||
        request.target_flag_2fa > 1) {
        error = "Character::Interact request has invalid same-Record identities or source selectors";
        return Status::invalid_argument;
    }
    if (!services.raise_talk_to_npc || !services.set_interact_state ||
        !services.raise_character_event || !services.read_target_flag_2fa) {
        error = "Character::Interact requires its current-Level event and same-Character FSM providers";
        return Status::provider_unavailable;
    }

    if (!request.target_is_interacting) {
        level_world::current_level_quest_event_v1::Event event{};
        event.objective_type = request.talk_to_npc_event_type;
        event.character = request.interactor;
        event.network_id = request.target_property_id;
        event.subject_id = -1;
        if (!services.raise_talk_to_npc(services.context, event, error)) {
            if (error.empty()) error = "Character::Interact TalkToNPC RaiseAsync failed";
            return Status::provider_failed;
        }
        result.talk_event_raised = true;
    }

    if (!services.set_interact_state(services.context, request.target_character,
            request.interaction_animation, 1, request.interactor, 0, error)) {
        if (error.empty()) error = "Character::Interact SM_SetInteractState failed";
        return Status::provider_failed;
    }
    result.state_set = true;

    std::uint8_t target_flag_2fa = 0;
    if (!services.read_target_flag_2fa(services.context,
            request.target_character, target_flag_2fa, error) ||
        target_flag_2fa > 1) {
        if (error.empty()) error = "Character::Interact target +0x2fa read failed";
        return Status::provider_failed;
    }
    if (!target_flag_2fa) {
        if (!services.raise_character_event(services.context,
                request.target_character, 5, request.interactor, error)) {
            if (error.empty()) error = "Character::Interact RaiseEvent(5) failed";
            return Status::provider_failed;
        }
        result.character_event_raised = true;
    }
    error.clear();
    return Status::complete;
}

} // namespace dh2::character_interact_v1
