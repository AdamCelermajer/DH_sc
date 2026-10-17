#include "character_enemy_attack_event_v1.hpp"

namespace dh2::character_enemy_attack_event_v1 {
namespace {

struct Dispatch {
    const Binding& binding;
    Result& result;
    bool failed{};
};

std::int32_t invoke(void* raw, character::AnimationAIState96* ai,
            const character::AnimationAIRequest32* request,
            character::AnimationAIResponse16* response) {
    auto& dispatch = *static_cast<Dispatch*>(raw);
    ++dispatch.result.animation_service_calls;
    if (!ai || ai != dispatch.binding.ai || !request || !response ||
        ai->owner != dispatch.binding.character_identity) {
        dispatch.failed = true;
        return -1;
    }
    if (request->service == character::ai_character_event &&
        request->argument == 0x1a) {
        ++dispatch.result.character_event_calls;
        if (request->subject != dispatch.binding.character_identity) {
            dispatch.failed = true;
            return -1;
        }
        const int status = dh2_character_state_event(dispatch.binding.state,
            dispatch.binding.facts, 0x1a,
            static_cast<std::uint64_t>(request->payload),
            &dispatch.binding.state_services);
        dispatch.result.character_event_status = status;
        if (status < 0) {dispatch.failed = true;return -1;}
        response->word = static_cast<std::uint32_t>(status);
        return 0;
    }
    ++dispatch.result.forwarded_calls;
    try {
        const auto status=dispatch.binding.animation_services.invoke(
            dispatch.binding.animation_services.context, ai, request, response);
        if(status!=0)dispatch.failed=true;
        return status;
    } catch (...) { dispatch.failed = true;return -1; }
}

} // namespace

Status attack_begin(const Binding& binding, Result* output,
                    std::string& error) noexcept {
    error.clear();
    if (output) *output = {};
    if (!output || !binding.character_identity || !binding.ai ||
        binding.ai->owner != binding.character_identity || !binding.ai->controller ||
        !binding.animation_services.invoke || !binding.state || !binding.facts ||
        !binding.state_services.invoke) {
        error = "enemy attack event lacks one canonical Character/CharAI state graph";
        return Status::invalid_argument;
    }
    Dispatch dispatch{binding, *output};
    const character::AnimationAIServices16 services{&dispatch, &invoke};
    int status = -1;
    try {
        status = dh2_character_animation_ai(binding.ai,
            character::ai_attack_begin, &services);
    } catch (...) { dispatch.failed = true; }
    if (status != 1 || dispatch.failed) {
        error = "source attack-step Character event provider failed";
        return Status::provider_failed;
    }
    return Status::complete;
}

} // namespace dh2::character_enemy_attack_event_v1
