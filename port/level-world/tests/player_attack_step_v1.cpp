#include "../player_attack_step_v1.hpp"

#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
struct Fixture {
    dh2::character::State* state = nullptr;
    const dh2::character::Facts* facts = nullptr;
    std::uintptr_t target = 0;
    std::vector<std::string> order;
    std::vector<dh2::character::AnimationAIRequest32> ai_requests;
    dh2::character::Services character_services{};
    std::uintptr_t look_target = 0;
    std::uint32_t look_mode = 0;
    std::uint32_t last_character_event = 0;
};
void character_service(void* raw, dh2::character::State*,
                       const dh2::character::Request* request) {
    auto& f = *static_cast<Fixture*>(raw);
    using namespace dh2::character;
    if (request->service == look_at) {
        f.order.emplace_back("look_at");
        f.look_mode = std::uint32_t(request->argument[0]);
        f.look_target = request->identity;
    }
}
std::int32_t raise_character_event(void* raw, std::uint32_t event,
                                   std::uintptr_t payload) {
    auto& f = *static_cast<Fixture*>(raw);
    f.order.emplace_back(event == 0x1a ? "character_event_1a" :
                         event == 0x26 ? "character_event_26" : "other_event");
    f.last_character_event = event;
    return dh2_character_state_event(f.state, f.facts, event, payload,
                                     &f.character_services);
}
std::int32_t animation_ai_service(void* raw, dh2::character::AnimationAIState96*,
                          const dh2::character::AnimationAIRequest32* request,
                          dh2::character::AnimationAIResponse16* response) {
    auto& f = *static_cast<Fixture*>(raw);
    using namespace dh2::character;
    f.ai_requests.push_back(*request);
    switch (request->service) {
    case ai_animation_step_index:
        f.order.emplace_back("step_index");
        response->word = 2;
        break;
    case ai_animation_step_count:
        f.order.emplace_back("step_count");
        response->word = 4;
        break;
    case ai_character_event:
        check(request->argument == 0x1a && request->payload == 2,
              "AttackBegin must raise Character event 0x1a with the captured step");
        check(raise_character_event(&f, request->argument, request->payload) >= 0,
              "nested Character RaiseEvent(0x1a) failed");
        break;
    default:
        throw std::runtime_error("Unexpected AttackBegin source service");
    }
    return 0;
}
}

int main() {
    try {
        using namespace dh2::character;
        State state;
        state.current = 5;
        state.flags = 0x2341;
        state.heading_active = 1;
        Facts facts;
        facts.is_player = 1;
        facts.target = 0x100000987ull;
        facts.heading[1] = 1;
        Fixture fixture;
        fixture.state = &state;
        fixture.facts = &facts;
        const Services character_services{&fixture, character_service};
        fixture.character_services = character_services;
        const AnimationAIServices16 ai_services{&fixture, animation_ai_service};
        AnimationAIState96 ai{};
        ai.owner = 0x100000123ull;
        ai.controller = 0x100000456ull;
        ai.target = facts.target;
        ai.look_target = facts.target;
        ai.animation_depth = 0;
        const player_attack_step_v1::Binding binding{
            &state, &ai, &ai_services, &fixture, raise_character_event, 0, 0};
        check(dh2_player_attack_step_begin_v1(&binding) == 1,
              "Player Attack step-begin event 0x26 was not accepted");
        check(ai.attack_index == 2 && state.current == 5 &&
              fixture.look_mode == 2 && fixture.look_target == facts.target,
              "Attack step did not retain canonical step or look at its target");
        check(fixture.last_character_event == 0x26,
              "final animation event did not use the owning Character::RaiseEvent route");
        check(fixture.order == std::vector<std::string>{
                  "step_index", "step_count", "character_event_1a", "look_at",
                  "character_event_26"},
              "Attack step nested source event order differs");
        check(fixture.ai_requests.size() == 3 &&
              fixture.ai_requests[0].subject == ai.owner &&
              fixture.ai_requests[2].payload == 2,
              "Attack step lost the retained Character/step identities");
        std::printf("PASS: event 0x26 -> AttackBegin -> Character 0x1a/target facing -> FSM 0x26\n");
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "Player Attack step audit: %s\n", error.what());
        return 1;
    }
}
