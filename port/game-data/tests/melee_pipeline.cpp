#include "../melee_pipeline.hpp"
#include <array>
#include <cstring>
#include <iostream>
#include <stdexcept>

using namespace dh2::data;
using namespace dh2::data::melee_pipeline;

void check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

struct HitObservation {
    PropertyView* player_properties;
    CombatActorState* player_state;
    const CombatRandom* random;
    std::int32_t hp_before = 0;
    std::uint32_t dead_before = 0, rng_calls = 0, calls = 0;
    std::uint32_t kill_calls = 0, dead_at_kill = 1;
    std::int32_t hp_at_kill = -1;
    float threat = 0;
};
void observe_before_hit(void* context, float threat) {
    auto& observation = *static_cast<HitObservation*>(context);
    ++observation.calls;
    observation.hp_before = observation.player_properties->resolved[36];
    observation.dead_before = observation.player_state->dead;
    observation.rng_calls = observation.random->calls;
    observation.threat = threat;
}
int complete_source_kill(void* context, CombatResult& result) {
    auto& observation = *static_cast<HitObservation*>(context);
    ++observation.kill_calls;
    observation.hp_at_kill = observation.player_properties->resolved[36];
    observation.dead_at_kill = observation.player_state->dead;
    if (observation.hp_at_kill != 0 || observation.dead_at_kill != 0 || result.amount <= 0)
        return -1;
    observation.player_state->dead = 1;
    dh2_property_set(observation.player_properties, 36, 0);
    return 0;
}

struct Fixture {
    PropertyRules rules{};
    PropertyState ghost_properties{}, player_properties{};
    CombatActorState ghost_state{}, player_state{};
    PropertyView ghost_view, player_view;
    Owner ghost, player;
    CombatRandom random{1, 0};
    Exchange output{};
    HitObservation hit_observation{};
    Request request;

    Fixture(std::int32_t hp, std::int32_t damage) {
        rules.types.fill(8); // Runtime-only properties, writable in resolved sheet.
        ghost_properties.resolved[50] = 12800; // Ensure the fixture attack clears hit accuracy.
        ghost_properties.resolved[79] = damage;
        ghost_properties.resolved[80] = damage;
        player_properties.resolved[36] = hp;
        player_properties.resolved[38] = hp;
        ghost_view = property_view(rules, ghost_properties);
        player_view = property_view(rules, player_properties);
        ghost_properties.resolved[204] = 256;
        ghost = {&ghost_view, &ghost_state, -1, -1, 0, 0, 0, 0};
        player = {&player_view, &player_state, -1, -1, 0, 0, 0, 0};
        hit_observation = {&player_view, &player_state, &random};
        request = {&ghost, &player, &random, 0, 1, &observe_before_hit, &hit_observation};
    }
};

int main() {
    Fixture nonlethal(2560, 1280); // 10 HP against a fixed 5 HP melee hit.
    check(monster_to_player(&nonlethal.request, &nonlethal.output) == Status::complete, "nonlethal source pipeline failed");
    check(nonlethal.output.result.amount == 1280, "melee result amount differs");
    check(nonlethal.output.result.mask == 0x22aab5u, "F_MeleeAttack mask differs");
    check(nonlethal.output.result.outcomes & 16u, "idle player did not receive hurt reaction");
    check(nonlethal.output.application.health.before == 2560, "player HP before differs");
    check(nonlethal.output.application.health.after == 1280, "player HP after differs");
    check(nonlethal.output.application.health.low_health_cue == 1, "low-health hysteresis did not arm");
    check(nonlethal.output.application.status_requests & request_hurt, "hurt service request missing");
    check(nonlethal.ghost_state.combo_hits == 1, "attacker combo counter did not advance");
    check(nonlethal.player_state.dead == 0, "nonlethal hit marked the player dead");
    check(nonlethal.player_properties.resolved[36] == 1280, "nonlethal hit did not write player HP");
    check(nonlethal.random.calls > 0, "melee pipeline did not consume source RNG");
    check(nonlethal.hit_observation.calls == 1, "pre-hit aggro callback did not run exactly once");
    check(nonlethal.hit_observation.hp_before == 2560 && nonlethal.hit_observation.dead_before == 0,
          "pre-hit callback did not observe the untouched player owner");
    check(nonlethal.hit_observation.rng_calls == nonlethal.random.calls,
          "pre-hit callback did not run after melee calculation");
    check(nonlethal.hit_observation.threat == nonlethal.output.application.threat &&
          nonlethal.hit_observation.threat == 5.0f,
          "pre-hit callback threat differs from the calculated application threat");
    check(nonlethal.hit_observation.hp_before > nonlethal.player_properties.resolved[36],
          "pre-hit callback was not before HP mutation");

    Fixture lethal(1280, 2560); // The same source path exercises HitFor -> kill prefix.
    check(monster_to_player(&lethal.request, &lethal.output) == Status::complete, "lethal source pipeline failed");
    check(lethal.output.result.amount == 2560, "lethal melee result amount differs");
    check(lethal.output.application.health.kill_requested == 1, "lethal result did not request kill");
    check(lethal.output.application.health.lifecycle_write == 3, "kill lifecycle write differs");
    check(lethal.player_state.dead == 1, "lethal hit did not set actor dead state");
    check(lethal.player_properties.resolved[36] == 0, "lethal hit did not set player HP to zero");
    check(lethal.hit_observation.calls == 1 && lethal.hit_observation.hp_before == 1280 &&
          lethal.hit_observation.dead_before == 0 && lethal.hit_observation.threat == 10.0f,
          "lethal aggro callback did not precede HitFor/death writes");

    Fixture bridged_lethal(1280, 2560);
    bridged_lethal.request.before_dead = &complete_source_kill;
    bridged_lethal.request.before_dead_context = &bridged_lethal.hit_observation;
    check(monster_to_player(&bridged_lethal.request, &bridged_lethal.output) == Status::complete,
          "melee adapter failed to forward the source Kill callback");
    check(bridged_lethal.hit_observation.kill_calls == 1 &&
          bridged_lethal.hit_observation.hp_at_kill == 0 &&
          bridged_lethal.hit_observation.dead_at_kill == 0 &&
          bridged_lethal.player_state.dead == 1 &&
          bridged_lethal.player_properties.resolved[36] == 0,
          "melee adapter did not preserve the HP-zero, alive-at-entry Kill boundary");

    Fixture bridged_nonlethal(2560, 1280);
    bridged_nonlethal.request.before_dead = &complete_source_kill;
    bridged_nonlethal.request.before_dead_context = &bridged_nonlethal.hit_observation;
    check(monster_to_player(&bridged_nonlethal.request, &bridged_nonlethal.output) == Status::complete &&
          bridged_nonlethal.hit_observation.kill_calls == 0,
          "melee adapter invoked the source Kill callback for a nonlethal hit");

    Fixture invalid(2560, 1280);
    invalid.output.result.amount = 77;
    const auto output_before = invalid.output;
    const auto random_before = invalid.random;
    invalid.request.offhand = 2;
    check(monster_to_player(&invalid.request, &invalid.output) == Status::invalid_argument, "invalid hand accepted");
    check(std::memcmp(&invalid.output, &output_before, sizeof(output_before)) == 0, "invalid request changed output");
    check(std::memcmp(&invalid.random, &random_before, sizeof(random_before)) == 0, "invalid request consumed RNG");
    check(invalid.player_properties.resolved[36] == 2560, "invalid request changed player HP");

    std::cout << "{\"validation\":\"PASS\",\"source_order\":[\"F_MeleeAttack\",\"F_ApplyResult\"],"
                 "\"nonlethal_hp\":1280,\"pre_hit_aggro_order_verified\":true,"
                 "\"lethal_kill_prefix\":true,\"pre_dead_kill_callback_verified\":true,"
                 "\"invalid_input_atomic\":true}\n";
}
