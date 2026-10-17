#include "player_attack_step_v1.hpp"

namespace {
using namespace dh2::character;
struct Context {
    const player_attack_step_v1::Binding* binding;
    bool failed = false;
};
std::int32_t invoke(void* raw, const AnimationEventRequest* request) {
    auto& context = *static_cast<Context*>(raw);
    const auto& binding = *context.binding;
    switch (request->service) {
    case animation_state_getter:
        return binding.character_state->current;
    case animation_attack_begin:
        if (dh2_character_animation_ai(binding.animation_ai, ai_attack_begin,
                                       binding.animation_ai_services) != 1) {
            context.failed = true;
            return 0;
        }
        return 1;
    case animation_state_event: {
        const auto delivered = binding.raise_character_event(
            binding.event_context, request->event, request->payload);
        if (delivered < 0) context.failed = true;
        return delivered;
    }
    default:
        context.failed = true;
        return 0;
    }
}
}

extern "C" int dh2_player_attack_step_begin_v1(
    const dh2::character::player_attack_step_v1::Binding* binding) {
    using namespace dh2::character;
    if (!binding || !binding->character_state || !binding->raise_character_event ||
        !binding->animation_ai || !binding->animation_ai_services ||
        !binding->animation_ai_services->invoke ||
        binding->global_blocked > 1 || binding->controller_forced > 1 ||
        binding->character_state->current != 5)
        return -1;
    Context context{binding};
    const AnimationEventFacts facts{0x26, binding->global_blocked,
        binding->character_state->controller_locked, binding->controller_forced, 0};
    const AnimationEventServices services{&context, invoke};
    const auto routed = dh2_character_animation_event_route(&facts, &services);
    return routed == 1 && !context.failed ? 1 : -1;
}
