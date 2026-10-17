#include "../character_target_out_of_sight_v1.hpp"

#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace dh2::character_target_out_of_sight_v1;

namespace {
struct Fixture {
    std::uintptr_t debug_owner{0x40};
    bool is_character{true};
    std::uintptr_t target_character{0x300};
    std::vector<Operation> calls;
    std::uintptr_t active_seen{};
    bool replace_debug_owner_on_ais_callback{};
};

std::int32_t invoke(void* raw, State* state, const Request* request, Response* response) {
    auto& fixture = *static_cast<Fixture*>(raw);
    fixture.calls.push_back(request->operation);
    switch (request->operation) {
    case Operation::capture_debug_switch_owner:
    case Operation::recapture_debug_switch_owner:
        response->identity = fixture.debug_owner;
        break;
    case Operation::query_debug_switch:
        if (request->subject != fixture.debug_owner) return 1;
        break;
    case Operation::is_target_a_character:
        if (request->subject != state->ai) return 1;
        response->word = fixture.is_character ? 1u : 0u;
        break;
    case Operation::get_target_as_character:
        if (request->subject != state->ai) return 1;
        response->identity = fixture.target_character;
        break;
    case Operation::clear_aggro:
        if (request->subject != state->ai || request->argument != fixture.target_character)
            return 1;
        break;
    case Operation::ais_on_target_out_of_sight:
        fixture.active_seen = request->subject;
        if (fixture.replace_debug_owner_on_ais_callback)
            fixture.debug_owner = 0x41;
        break;
    }
    return 0;
}

void require(bool condition, const char* message) {
    if (!condition) { std::fprintf(stderr, "FAIL: %s\n", message); std::exit(1); }
}
}

int main() {
    State state{0x100, 0x200, 1, 0};
    Fixture fixture;
    Services services{&fixture, invoke};
    Result result{};
    require(run(&state, &services, &result) == Status::complete &&
            fixture.calls == std::vector<Operation>{
                Operation::capture_debug_switch_owner,
                Operation::query_debug_switch,
                Operation::is_target_a_character,
                Operation::get_target_as_character,
                Operation::clear_aggro,
                Operation::ais_on_target_out_of_sight,
                Operation::recapture_debug_switch_owner} &&
            result.target_character == fixture.target_character &&
            result.out_of_sight_cleared == 1 && state.out_of_sight_4c == 0 &&
            result.ais_callback_target == state.active_ais &&
            fixture.active_seen == state.active_ais,
            "Character-target loss did not clear aggro, reset the source byte, then call active AIS");

    fixture.calls.clear();
    fixture.is_character = false;
    state.out_of_sight_4c = 1;
    require(run(&state, &services, &result) == Status::complete &&
            fixture.calls == std::vector<Operation>{
                Operation::capture_debug_switch_owner,
                Operation::query_debug_switch,
                Operation::is_target_a_character,
                Operation::ais_on_target_out_of_sight,
                Operation::recapture_debug_switch_owner} &&
            result.out_of_sight_cleared == 1 && state.out_of_sight_4c == 0,
            "Non-Character target incorrectly ran Character aggro cleanup");

    fixture.calls.clear();
    state.active_ais = 0;
    state.active_ais = 0x200;
    fixture.debug_owner = 0x40;
    fixture.replace_debug_owner_on_ais_callback = true;
    require(run(&state, &services, &result) == Status::source_invariant_failed &&
            result.debug_owner_changed == 1 && state.out_of_sight_4c == 0 &&
            fixture.calls.back() == Operation::recapture_debug_switch_owner,
            "Debug owner replacement did not preserve the source post-callback invariant check");
    std::puts("PASS character_target_out_of_sight_v1 checks=3");
}
