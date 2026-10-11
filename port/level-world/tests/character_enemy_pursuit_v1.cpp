#include "../character_enemy_pursuit_v1.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

namespace {
using namespace dh2::character;
using namespace dh2::character_enemy_pursuit_v1;
struct Fixture {
    float target_position[3]{10.f, 20.f, 30.f};
    float radius_squared{4.f};
    std::uint32_t ai_calls{};
    std::uint32_t control_calls{};
    std::uint32_t path_calls{};
    std::uintptr_t path_owner{};
    float path_position[3]{};
    std::int32_t animation_state{4};
    std::uint32_t routed_state_events{};
    std::uint32_t last_animation_event{};
    std::uintptr_t last_animation_payload{};
    std::uint32_t source_attack_look_at{};
};
std::int32_t ai_invoke(void* raw, AnimationAIState96*, const AnimationAIRequest32* request,
               AnimationAIResponse16* response) {
    auto& f = *static_cast<Fixture*>(raw);
    ++f.ai_calls;
    if (request->service == ai_target_position)
        std::copy(f.target_position, f.target_position + 3, response->position);
    else if (request->service == ai_melee_radius_squared)
        std::memcpy(&response->word, &f.radius_squared, sizeof(response->word));
    return 0;
}
std::int32_t ai_attack_invoke(void* raw, AnimationAIState96*, const AnimationAIRequest32* request,
                      AnimationAIResponse16* response) {
    auto& f = *static_cast<Fixture*>(raw);
    ++f.ai_calls;
    if (request->service == ai_animation_step_index) response->word = 3;
    else if (request->service == ai_animation_step_count) response->word = 4;
    return 0;
}
int control_invoke(void* raw, const CharacterControlRequest32* request,
                   CharacterControlResponse16* response) {
    auto& f = *static_cast<Fixture*>(raw);
    ++f.control_calls;
    if (request->service == control_is_remotely_updated) {
        response->word = 0;
        return 1;
    }
    if (request->service == control_target_position) {
        std::copy(f.target_position, f.target_position + 3, response->position);
        return 1;
    }
    if (request->service == control_path_to) {
        f.path_owner = request->subject;
        std::copy(request->position, request->position + 3, f.path_position);
        ++f.path_calls;
        return 1;
    }
    return -1;
}
std::int32_t character_event_invoke(void* raw, const AnimationEventRequest* request) {
    auto& f = *static_cast<Fixture*>(raw);
    if (request->service == animation_state_getter) return f.animation_state;
    if (request->service == animation_state_event) {
        ++f.routed_state_events;
        f.last_animation_event = request->event;
        f.last_animation_payload = request->payload;
        return 1;
    }
    return -1;
}
void character_state_invoke(void* raw, State*, const Request* request) {
    auto& f = *static_cast<Fixture*>(raw);
    if (request->service == look_at) ++f.source_attack_look_at;
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
    ai.target = 0x300;
    ai.seeking = 1;
    ControllerCommandState32 controller{0x200, 0x100, 0, 0, 0, 0};
    Binding binding{&ai, {&fixture, ai_invoke}, &controller,
                    {&fixture, control_invoke}};
    Result result{};
    std::string error;
    require(move_begin(binding, &result, error) == Status::complete &&
            result.move_to_calls == 1 && result.path_dispatch_attempted &&
            result.controller_status == 1 && fixture.path_calls == 1 &&
            fixture.path_owner == ai.owner &&
            fixture.path_position[0] == fixture.target_position[0] &&
            fixture.path_position[1] == fixture.target_position[1] &&
            fixture.path_position[2] == fixture.target_position[2],
            "source Move-begin did not reach the canonical Character PathTo service");

    fixture.radius_squared = 2000.f;
    const auto paths_before = fixture.path_calls;
    require(move_begin(binding, &result, error) == Status::complete &&
            result.move_to_calls == 0 && fixture.path_calls == paths_before,
            "in-range enemy incorrectly requested a new path");

    controller.owner = 0x101;
    require(move_begin(binding, &result, error) == Status::invalid_argument &&
            fixture.path_calls == paths_before,
            "mismatched Character/controller identities were accepted");

    controller.owner = ai.owner;
    fixture.radius_squared = 4.f;
    AnimationEventFacts facts{0x26, 0, 0, 0, 0x100000007ull};
    EventBinding event_binding{&facts, binding,
        {&fixture, character_event_invoke}};
    EventResult event_result{};
    const auto event_paths = fixture.path_calls;
    const auto routed_status = animation_event(event_binding, &event_result, error);
    if (routed_status != Status::complete || event_result.move_begin_calls != 1 ||
        fixture.path_calls != event_paths + 1 || fixture.routed_state_events != 1)
        std::fprintf(stderr, "route status=%u error=%s calls=%u paths=%u/%u state-events=%u forwarded=%u\n",
            static_cast<unsigned>(routed_status), error.c_str(), event_result.move_begin_calls,
            fixture.path_calls, event_paths, fixture.routed_state_events, event_result.forwarded_calls);
    require(routed_status == Status::complete &&
            event_result.route_calls == 1 && event_result.move_begin_calls == 1 &&
            event_result.pursuit.path_dispatch_attempted &&
            fixture.path_calls == event_paths + 1 && fixture.routed_state_events == 1 &&
            fixture.last_animation_event == facts.event &&
            fixture.last_animation_payload == facts.payload,
            "source event 0x26 did not gate and route enemy Move-begin through PathTo");

    fixture.animation_state = 3;
    const auto wrong_state_paths = fixture.path_calls;
    require(animation_event(event_binding, &event_result, error) == Status::complete &&
            event_result.move_begin_calls == 0 &&
            fixture.path_calls == wrong_state_paths && fixture.routed_state_events == 2,
            "non-Move FSM state incorrectly dispatched enemy pursuit");

    fixture.animation_state = 4;
    facts.global_blocked = 1;
    facts.controller_locked = 1;
    const auto gated_paths = fixture.path_calls;
    require(animation_event(event_binding, &event_result, error) == Status::complete &&
            event_result.move_begin_calls == 0 && event_result.forwarded_calls == 1 &&
            fixture.path_calls == gated_paths && fixture.routed_state_events == 3,
            "blocked Character animation event bypassed source movement gates");

    facts.global_blocked = facts.controller_locked = 0;
    fixture.animation_state = 5;
    State enemy_state{};
    enemy_state.current = 5;
    Facts enemy_facts{};
    enemy_facts.target = ai.target;
    const Services enemy_state_services{&fixture, character_state_invoke};
    const dh2::character_enemy_attack_event_v1::Binding attack_binding{
        ai.owner, &ai, {&fixture, ai_attack_invoke}, &enemy_state,
        &enemy_facts, enemy_state_services};
    event_binding.attack_begin = &attack_binding;
    const auto paths_before_attack = fixture.path_calls;
    require(animation_event(event_binding, &event_result, error) == Status::complete &&
            event_result.attack_begin_calls == 1 &&
            event_result.attack_begin.character_event_calls == 1 &&
            event_result.attack_begin.character_event_status == 0 &&
            fixture.source_attack_look_at == 1 &&
            fixture.path_calls == paths_before_attack &&
            fixture.last_animation_event == facts.event,
            "source event 0x26/state 5 did not route attack-begin before the Character event");
    std::puts("PASS character_enemy_pursuit_v1 checks=7");
}
