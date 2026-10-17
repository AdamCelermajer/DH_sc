#include "player_attack_result_edge_v1.hpp"
#include <cstdint>

namespace dh2::data::player_attack_result_edge_v1 {
namespace {
bool disjoint(const void* a, std::size_t as, const void* b, std::size_t bs) {
    const auto x = reinterpret_cast<std::uintptr_t>(a);
    const auto y = reinterpret_cast<std::uintptr_t>(b);
    if (!a || !b || x > UINTPTR_MAX - as || y > UINTPTR_MAX - bs) return false;
    return x + as <= y || y + bs <= x;
}
}

Status execute(const Request* request, Result* output) {
    if (!request || !output ||
        reinterpret_cast<std::uintptr_t>(request) % alignof(Request) ||
        reinterpret_cast<std::uintptr_t>(output) % alignof(Result) ||
        !disjoint(request, sizeof(*request), output, sizeof(*output)) ||
        !request->application || !request->application->result ||
        reinterpret_cast<std::uintptr_t>(request->application) %
            alignof(MonsterApplicationRequest) ||
        reinterpret_cast<std::uintptr_t>(request->application->result) %
            alignof(CombatResult) ||
        !request->attacker_character || !request->defender_character ||
        request->attacker_character == request->defender_character ||
        !request->invoke ||
        !disjoint(output, sizeof(*output), request->application,
                  sizeof(*request->application)) ||
        !disjoint(request, sizeof(*request), request->application,
                  sizeof(*request->application)) ||
        !disjoint(output, sizeof(*output), request->application->result,
                  sizeof(*request->application->result)) ||
        !disjoint(request, sizeof(*request), request->application->result,
                  sizeof(*request->application->result)))
        return Status::invalid_argument;

    Result candidate{};
    if (const auto applied = dh2_combat_apply_player_to_monster(
            &candidate.application, request->application)) {
        candidate.provider_status = static_cast<std::int32_t>(applied);
        *output = candidate;
        return Status::application_failed;
    }
    candidate.application_completed = 1;
    if(candidate.application.health.kill_requested){
        candidate.last_step=Step::lethal_hit;++candidate.steps_attempted;
        try{candidate.provider_status=request->invoke(request->service_context,
              Step::lethal_hit,request->attacker_character,
              request->defender_character,*request->application->result);}
        catch(...){candidate.provider_status=-1;}
        if(candidate.provider_status){*output=candidate;return Status::service_failed;}
        ++candidate.steps_completed;
    }
    constexpr Step prefix[]={Step::cancel_sneaking,Step::combat_text};
    for(const auto step:prefix){
        candidate.last_step=step;++candidate.steps_attempted;
        try{candidate.provider_status=request->invoke(request->service_context,step,
              request->attacker_character,request->defender_character,
              *request->application->result);}
        catch(...){candidate.provider_status=-1;}
        if(candidate.provider_status){*output=candidate;return Status::service_failed;}
        ++candidate.steps_completed;
    }
    // The native source calls F_ApplyCombatSound and ignores its return value.
    // Missing/failed optional audio must not suppress the remaining result
    // tail (including the fresh bit-29 read and AI result callback).
    candidate.last_step=Step::combat_sound;++candidate.steps_attempted;
    try{candidate.sound_provider_status=request->invoke(request->service_context,
          Step::combat_sound,request->attacker_character,
          request->defender_character,*request->application->result);}
    catch(...){candidate.sound_provider_status=-1;}
    candidate.sound_available=candidate.sound_provider_status==0;
    ++candidate.steps_completed;
    if(request->application->result->mask&0x20000000u){
        candidate.ai_dispatch_suppressed=1;*output=candidate;return Status::complete;
    }
    candidate.last_step=Step::ai_combat_result;++candidate.steps_attempted;
    try{candidate.provider_status=request->invoke(request->service_context,
          Step::ai_combat_result,request->attacker_character,
          request->defender_character,*request->application->result);}
    catch(...){candidate.provider_status=-1;}
    if(candidate.provider_status){*output=candidate;return Status::service_failed;}
    ++candidate.steps_completed;*output=candidate;return Status::complete;
}

} // namespace dh2::data::player_attack_result_edge_v1
