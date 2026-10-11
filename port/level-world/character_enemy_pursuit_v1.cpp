#include "character_enemy_pursuit_v1.hpp"

namespace dh2::character_enemy_pursuit_v1 {
namespace {

struct Dispatch {
    const Binding& binding;
    Result& result;
    bool failed{};
};

std::int32_t invoke(void* raw, character::AnimationAIState96* state,
            const character::AnimationAIRequest32* request,
            character::AnimationAIResponse16* response) {
    auto& dispatch = *static_cast<Dispatch*>(raw);
    ++dispatch.result.ai_service_calls;
    if (!state || !request || !response) {
        dispatch.failed = true;
        return -1;
    }
    if (request->service != character::ai_controller_move_to) {
        try {
            const auto status=dispatch.binding.ai_services.invoke(
                dispatch.binding.ai_services.context, state, request, response);
            if(status!=0)dispatch.failed=true;
            return status;
        } catch (...) { dispatch.failed = true;return -1; }
    }

    ++dispatch.result.move_to_calls;
    dispatch.result.path_dispatch_attempted = true;
    if (request->subject != dispatch.binding.controller->controller ||
        !request->payload || request->payload != state->target ||
        state->owner != dispatch.binding.controller->owner ||
        state->controller != dispatch.binding.controller->controller) {
        dispatch.failed = true;
        return -1;
    }
    const auto status = dh2_character_controller_character(
        dispatch.binding.controller, character::controller_move_object,
        request->payload, &dispatch.binding.control_services);
    dispatch.result.controller_status = static_cast<std::uint32_t>(status);
    if (status != 1) dispatch.failed = true;
    return status==1?0:-1;
}

} // namespace

Status move_begin(const Binding& binding, Result* output,
                  std::string& error) noexcept {
    error.clear();
    if (output) *output = {};
    if (!output || !binding.ai || !binding.controller ||
        !binding.ai_services.invoke || !binding.control_services.invoke ||
        !binding.ai->owner || !binding.ai->controller ||
        binding.ai->owner != binding.controller->owner ||
        binding.ai->controller != binding.controller->controller) {
        error = "enemy pursuit lacks one canonical Character/controller owner";
        return Status::invalid_argument;
    }
    Dispatch dispatch{binding, *output};
    const character::AnimationAIServices16 services{&dispatch, &invoke};
    int status = -1;
    try {
        status = dh2_character_animation_ai(binding.ai,
            character::ai_move_begin, &services);
    } catch (...) {
        dispatch.failed = true;
    }
    if (status != 1 || dispatch.failed) {
        error = "source enemy Move-begin/controller/path provider failed";
        return Status::provider_failed;
    }
    return Status::complete;
}

namespace {

struct EventDispatch {
    const EventBinding& binding;
    EventResult& result;
    std::string& error;
    bool pursuit_failed{};
};

std::int32_t dispatch_event(void* raw, const character::AnimationEventRequest* request) {
    auto& dispatch = *static_cast<EventDispatch*>(raw);
    if (!request) return 0;
    if (request->service == character::animation_attack_begin &&
        dispatch.binding.attack_begin) {
        ++dispatch.result.attack_begin_calls;
        const auto status = character_enemy_attack_event_v1::attack_begin(
            *dispatch.binding.attack_begin, &dispatch.result.attack_begin,
            dispatch.error);
        if (status != character_enemy_attack_event_v1::Status::complete) {
            dispatch.pursuit_failed = true;
            return 0;
        }
        // The source Attack-begin consumer accepts and forwards the original
        // Character animation event after its internal event 0x1a handling.
        return 1;
    }
    if (request->service == character::animation_state_event)
        ++dispatch.result.state_event_calls;
    if (request->service != character::animation_move_begin) {
        ++dispatch.result.forwarded_calls;
        try {
            return dispatch.binding.character_events.invoke(
                dispatch.binding.character_events.context, request);
        } catch (...) {
            dispatch.pursuit_failed = true;
            dispatch.error = "Character animation-event provider threw";
            return 0;
        }
    }

    ++dispatch.result.move_begin_calls;
    const auto status = move_begin(dispatch.binding.pursuit,
                                   &dispatch.result.pursuit, dispatch.error);
    if (status != Status::complete) {
        dispatch.pursuit_failed = true;
        return 0;
    }
    // The source Move-begin consumer returns 1 after accepted dispatch, which
    // makes Character's animation router deliver the original state event.
    return 1;
}

} // namespace

Status animation_event(const EventBinding& binding, EventResult* output,
                       std::string& error) noexcept {
    error.clear();
    if (output) *output = {};
    if (!output || !binding.facts || !binding.character_events.invoke) {
        error = "enemy animation event lacks the canonical Character event owner";
        return Status::invalid_argument;
    }
    EventDispatch dispatch{binding, *output, error};
    const character::AnimationEventServices services{&dispatch, &dispatch_event};
    int status = -1;
    try {
        ++output->route_calls;
        status = dh2_character_animation_event_route(binding.facts, &services);
    } catch (...) {
        error = "source Character animation-event route failed";
        return Status::provider_failed;
    }
    if (status != 1) {
        error = "source Character animation-event route rejected input";
        return Status::invalid_argument;
    }
    if (dispatch.pursuit_failed) return Status::provider_failed;
    return Status::complete;
}

} // namespace dh2::character_enemy_pursuit_v1
