#include "../player_attack_result_edge_v1.hpp"
#include <iostream>
#include <stdexcept>

using namespace dh2::data;
using namespace dh2::data::player_attack_result_edge_v1;

namespace {
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
struct Observation {
    PropertyView* defender;
    CombatActorState* state;
    unsigned calls = 0;
    int status = 0;
    std::int32_t hp = -1;
    std::uint32_t dead = 0;
    std::uintptr_t attacker = 0, target = 0;
    std::vector<Step> steps;
    Step fail_step = Step::cancel_sneaking;
    bool fail_enabled = false;
    Step throw_step = Step::cancel_sneaking;
    bool throw_enabled = false;
};
int invoke(void* raw, Step step, std::uintptr_t attacker,
           std::uintptr_t defender, CombatResult& result) {
    auto& observation = *static_cast<Observation*>(raw);
    observation.steps.push_back(step);
    ++observation.calls;
    observation.hp = observation.defender->resolved[36];
    observation.dead = observation.state->dead;
    observation.attacker = attacker;
    observation.target = defender;
    if(step==Step::combat_sound&&(result.outcomes&0x40000000u))result.mask|=0x20000000u;
    if(observation.throw_enabled&&step==observation.throw_step)
        throw std::runtime_error("provider unavailable");
    if(observation.fail_enabled&&step==observation.fail_step)return observation.status;
    return 0;
}
}

int main() {
    try {
        PropertyRules rules{};
        rules.types.fill(8);
        PropertyState attacker_sheet{}, target_sheet{};
        attacker_sheet.resolved[204] = 256;
        target_sheet.resolved[36] = target_sheet.resolved[38] = 2560;
        auto attacker = property_view(rules, attacker_sheet);
        auto target = property_view(rules, target_sheet);
        CombatActorState attacker_state{}, target_state{};
        CombatResult attack{};
        attack.amount = 1280;
        MonsterApplicationRequest application{&attack, &attacker, &target,
                                               &attacker_state, &target_state};
        Observation observation{};
        observation.defender = &target;
        observation.state = &target_state;
        Request request{&application, 0x1001, 0x2002, &observation, &invoke};
        Result result{};

        check(execute(&request, &result) == Status::complete,
              "nonlethal player attack/result edge failed");
        const std::vector<Step> ordered{Step::cancel_sneaking,Step::combat_text,
            Step::combat_sound,Step::ai_combat_result};
        check(result.application_completed && result.steps_completed==4 &&
              observation.steps==ordered && observation.calls == 4 && observation.hp == 1280 &&
              observation.dead == 0 && observation.attacker == 0x1001 &&
              observation.target == 0x2002,
              "result callback did not see canonical post-hit HP/state and identities");

        target_sheet.resolved[36] = 1;
        attack.amount = 1280;
        observation.calls = 0;observation.steps.clear();
        const std::vector<Step> lethal_order{Step::lethal_hit,Step::cancel_sneaking,
            Step::combat_text,Step::combat_sound,Step::ai_combat_result};
        check(execute(&request, &result) == Status::complete &&
              observation.calls == 5 && observation.steps==lethal_order &&
              observation.hp == 0 &&
              observation.dead == 1 && result.application.health.kill_requested,
              "lethal continuation did not precede the source result tail");

        target_state = {};
        target_sheet.resolved[36] = 1;
        attack.amount = 1280;
        observation.calls = 0;observation.steps.clear();
        observation.status=7;observation.fail_enabled=true;
        observation.fail_step=Step::lethal_hit;
        check(execute(&request,&result)==Status::service_failed&&
              result.last_step==Step::lethal_hit&&result.steps_attempted==1&&
              result.steps_completed==0&&observation.steps==
                  std::vector<Step>{Step::lethal_hit},
              "failed lethal continuation incorrectly ran combat text/audio/AI tail");
        observation.fail_enabled=false;
        observation.status=0;

        target_state = {};
        target_sheet.resolved[36] = 2560;
        attack.amount = 1280;
        observation.calls = 0;observation.steps.clear();
        observation.status = 7;
        observation.fail_enabled=true;observation.fail_step=Step::combat_text;
        check(execute(&request, &result) == Status::service_failed &&
              result.application_completed && result.provider_status == 7 &&
              result.steps_completed==1&&observation.calls==2&&
              observation.steps[0]==Step::cancel_sneaking&&
              observation.steps[1]==Step::combat_text&&
              target_sheet.resolved[36] == 1280,
              "service failure did not preserve the reached source prefix");

        observation.status = 0;observation.fail_enabled=false;
        observation.calls=0;observation.steps.clear();
        target_state={};target_sheet.resolved[36]=2560;attack.amount=1280;
        attack.outcomes=0x40000000u;
        check(execute(&request,&result)==Status::complete&&
              observation.steps.size()==3&&result.ai_dispatch_suppressed&&
              result.steps_attempted==3&&result.steps_completed==3,
              "post-audio fresh suppression mask did not skip AIS result callbacks");
        attack.outcomes=0;

        // CombatSound is a source side-effect with ignored return status.
        // Provider absence must be visible but cannot abort AI/kill continuation.
        observation.status=9;observation.fail_enabled=true;
        observation.fail_step=Step::combat_sound;observation.calls=0;
        observation.steps.clear();target_state={};target_sheet.resolved[36]=2560;
        attack.amount=1280;attack.mask=0;
        check(execute(&request,&result)==Status::complete&&
              observation.steps==ordered&&result.sound_available==0&&
              result.sound_provider_status==9&&result.provider_status==0&&
              result.steps_completed==4,
              "unavailable optional sound incorrectly failed result continuation");

        // The source rereads its result mask after the sound call even when
        // the optional provider is unavailable.
        observation.calls=0;observation.steps.clear();target_state={};
        target_sheet.resolved[36]=2560;attack.amount=1280;attack.mask=0;
        attack.outcomes=0x40000000u;
        check(execute(&request,&result)==Status::complete&&
              observation.steps.size()==3&&result.sound_available==0&&
              result.sound_provider_status==9&&result.ai_dispatch_suppressed&&
              result.steps_completed==3,
              "optional sound failure skipped the fresh suppression-mask read");
        attack.outcomes=0;

        // A throwing sound provider has the same optional semantics.
        observation.fail_enabled=false;observation.throw_enabled=true;
        observation.throw_step=Step::combat_sound;observation.calls=0;
        observation.steps.clear();target_state={};target_sheet.resolved[36]=2560;
        attack.amount=1280;attack.mask=0;
        check(execute(&request,&result)==Status::complete&&
              observation.steps==ordered&&result.sound_available==0&&
              result.sound_provider_status==-1&&result.provider_status==0,
              "throwing optional sound provider incorrectly failed result continuation");
        observation.throw_enabled=false;

        request.defender_character = request.attacker_character;
        const auto before = target_sheet.resolved[36];
        check(execute(&request, &result) == Status::invalid_argument &&
              target_sheet.resolved[36] == before,
              "invalid Character identities mutated combat owners");

        std::cout << "PASS: player damage -> lethal kill continuation -> source result tail; cases=9\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
