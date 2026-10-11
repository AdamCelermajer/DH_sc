#include "../character_enemy_attack_event_v1.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>

namespace {
using namespace dh2::character;
using namespace dh2::character_enemy_attack_event_v1;
struct Fixture {
    std::uint32_t animation_queries{};
    std::uint32_t state_events{};
    std::uintptr_t looked_at{};
    std::uint32_t look_kind{};
};
std::int32_t animation_invoke(void* raw, AnimationAIState96*,
                      const AnimationAIRequest32* request,
                      AnimationAIResponse16* response) {
    auto& f = *static_cast<Fixture*>(raw);
    ++f.animation_queries;
    if (request->service == ai_animation_step_index) response->word = 3;
    else if (request->service == ai_animation_step_count) response->word = 4;
    return 0;
}
void state_invoke(void* raw, State*, const Request* request) {
    auto& f = *static_cast<Fixture*>(raw);
    if (request->service == look_at) {
        ++f.state_events;
        f.look_kind = static_cast<std::uint32_t>(request->argument[0]);
        f.looked_at = static_cast<std::uintptr_t>(request->identity);
    }
}
void require(bool ok, const char* text) {
    if (!ok) { std::fprintf(stderr, "FAIL: %s\n", text); std::exit(1); }
}
}

int main() {
    Fixture fixture;
    AnimationAIState96 ai{};
    ai.owner = 0x100;
    ai.controller = 0x200;
    ai.animation_depth = 0;
    State state{};
    state.current = 5;
    Facts facts{};
    facts.target = 0x300;
    const Binding binding{0x100, &ai, {&fixture, animation_invoke}, &state,
                          &facts, {&fixture, state_invoke}};
    Result result{};
    std::string error;
    require(attack_begin(binding, &result, error) == Status::complete &&
            ai.attack_index == 3 && result.character_event_calls == 1 &&
            result.character_event_status == 0 && result.forwarded_calls == 2 &&
            fixture.state_events == 1 && fixture.look_kind == 2 &&
            fixture.looked_at == facts.target,
            "CharAI attack-step 0x1a did not reach the same Character combat-state owner");

    const auto calls = fixture.animation_queries + fixture.state_events;
    require(attack_begin(Binding{0x101, &ai, {&fixture, animation_invoke}, &state,
                                 &facts, {&fixture, state_invoke}}, &result, error) ==
                Status::invalid_argument &&
            fixture.animation_queries + fixture.state_events == calls,
            "mismatched CharAI/Character identity dispatched the attack event");
    std::puts("PASS character_enemy_attack_event_v1 checks=2");
}
