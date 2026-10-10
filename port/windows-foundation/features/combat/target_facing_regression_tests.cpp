#include "../../actor_profiles.hpp"
#include "../../combat_session.hpp"
#include "../../content_paths.hpp"
#include "../../original_actor_properties.hpp"
#include "runtime_player_combo_chain_v1.hpp"
#include "../../../level-world/actor_rotation.hpp"
#include "../../../level-world/navigation_heading.hpp"

#include <cmath>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace dh::foundation;
using namespace dh::foundation::combat;

namespace {
constexpr float pi = 3.14159265358979323846f;

void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

void run_profile(const AssetCatalog& assets, const OriginalPropertyDatabase& properties,
                 const OriginalMeleeBindings& bindings, const std::string& profile_id,
                 const std::string& skin_filter, const std::string& label) {
    std::string error;
    ActorCustomization customization;
    customization.skin_id_contains = skin_filter;
    customization.expected_controller_count = 4;
    customization.allow_missing_animation_targets = true;
    OriginalCombatVisualPlan visual_plan;
    check(build_original_combat_visual_plan(assets, bindings, profile_id, customization,
          "target-facing-" + label, visual_plan, error), error);
    visual_plan.config.motion_node_id = "auto";
    visual_plan.config.consume_root_motion = true;

    CombatSessionConfig config;
    config.diagnosticRngSeed = 20261010;
    config.playerId = 1;
    config.playerProfileId = profile_id;
    config.tableRoot = "original-cache/data/pydata";
    config.playerVisualConfig = visual_plan.config;

    CombatSessionProfile player;
    player.initialIdle = {"Idle", 0, {0}};
    player.sequenceAction = OriginalAttackSelection{"AttackStatic", 0, {}};
    player.damageMarkerNames = {"attack_mainhand"};
    player.propertyOptions = {256, true};
    player.customization = customization;
    player.retainedPhaseClock = true;
    player.sourceCombo = true;
    player.sourceAnimationClips = visual_plan.config.clips;
    config.profiles.emplace(profile_id, player);

    CombatSessionProfile lizard;
    lizard.action = {"Attack", 0, {0, 1}};
    lizard.initialIdle = {"Idle", 0, {0}};
    lizard.damageMarkerNames = {"attack_mainhand"};
    lizard.propertyOptions = {std::nullopt, true};
    lizard.customization.allow_missing_animation_targets = true;
    config.profiles.emplace("Swamp_LizadMan_Type1", lizard);

    ActorPopulation population;
    PopulationActor placed;
    placed.profileId = "Swamp_LizadMan_Type1";
    placed.definition.stableId = 2;
    placed.definition.sourceId = "target-facing-real-lizard-" + label;
    // +X,-Y is a non-collinear +pi/4 source bearing from player heading zero.
    placed.definition.placement = {1,0,0,0, 0,1,0,0, 0,0,1,0, 50,-50,0,1};
    placed.transform = placed.definition.placement;
    population.actors().push_back(std::move(placed));

    CharacterVisual player_visual;
    CombatSession session;
    check(session.initialize(assets, properties, bindings, config, player_visual,
          population, {0,0,0}, customization, error), error);
    auto* actor = session.actor(1);
    auto* target = session.actor(2);
    check(actor && target,
          label + " did not bind the player and Lizard ActorStates");
    check(target->definition_id == "target-facing-real-lizard-" + label && target->alive(),
          label + " real Lizard actor identity/liveness mismatch: " + target->definition_id);
    check(session.retained_actor_visual_borrow(1) != nullptr,
          label + " player visual is not retained by CombatSession");
    actor->transform.rotation[2] = 0.0f;

    const auto random_before_select = session.world()->random_state();
    InputActions input;
    input.targetSelect = true;
    check(session.update(0, input, {0,0,0}, 0.0f, error), error);
    check(actor->target_id == 2 && session.selectedactor() &&
          session.selectedactor()->id == 2,
          label + " target selection did not retain the same live Lizard target");
    check(actor->transform.rotation[2] == 0.0f,
          label + " target selection forced a turn before attack admission");
    check(session.world()->random_state().seed == random_before_select.seed &&
          session.world()->random_state().calls == random_before_select.calls,
          label + " target selection consumed combat RNG");

    input.targetSelect = false;
    input.attack = true;
    std::uint32_t* turn_positive = nullptr;
    check(session.borrow_rotation_turn(1, turn_positive, error) && turn_positive, error);
    float direction[]{50.0f, -50.0f, 0.0f};
    float desired_heading = 0.0f;
    check(dh2_nav_look_towards(&desired_heading, direction) == 0 &&
          std::abs(desired_heading - pi / 4.0f) < 1e-6f,
          label + " authored navigation heading for +X,-Y was not +pi/4");
    const auto* source_properties = session.world()->combat_properties(1);
    check(source_properties != nullptr, label + " source player properties are absent");
    dh2::actor::RotationState expected_turn{{0.0f,0.0f,0.0f},desired_heading,*turn_positive,0};
    const dh2::actor::RotationPolicy rotation_policy{
        original_speed_modifier(source_properties->sheets.resolved[47]),16,1,1};
    std::uint32_t sync_visual = 0;
    check(dh2_actor_update_rotation(&expected_turn,&rotation_policy,&sync_visual) == 0,
          label + " original one-frame bounded turn reference rejected");
    check(session.update(.016, input, {0,0,0}, actor->transform.rotation[2], error), error);
    check(session.owns_pose(1) && actor->action == CharacterAction::attacking,
          label + " attack admission did not own the source attack pose");
    check(actor->transform.rotation[2] == expected_turn.rotation[2] &&
          actor->transform.rotation[2] > 0.0f && actor->transform.rotation[2] < pi / 4.0f,
          label + " fresh attack admission did not apply exactly one bounded source turn");
    check(!session.combo_boundaries().empty(),
          label + " attack admission did not expose the source combo generation");
    const auto generation = session.combo_boundaries().front().generation;
    const auto random_after_admission = session.world()->random_state();

    // Keep the command held so the test covers the active per-frame turn arm.
    dh2::actor::RotationState expected_active = expected_turn;
    check(dh2_nav_look_towards(&expected_active.heading_angle,direction) == 0 &&
          dh2_actor_update_rotation(&expected_active,&rotation_policy,&sync_visual) == 0,
          label + " original second active turn reference rejected");
    check(session.update(.016, input, {0,0,0}, actor->transform.rotation[2], error), error);
    const float first_heading = actor->transform.rotation[2];
    check(std::isfinite(first_heading) && first_heading == expected_active.rotation[2] &&
          first_heading < pi / 4.0f,
          label + " held active attack did not apply the exact next bounded turn toward +pi/4");
    check(session.owns_pose(1) && actor->action == CharacterAction::attacking,
          label + " first facing update interrupted the active attack pose");
    for (const auto& boundary : session.combo_boundaries())
        check(boundary.generation == generation,
              label + " held-facing update created a duplicate attack generation");
    check(session.world()->random_state().seed == random_after_admission.seed &&
          session.world()->random_state().calls == random_after_admission.calls,
          label + " held-facing update consumed combat RNG");

    // Retarget the same still-live actor across the player's forward axis while
    // the attack command remains held. Source Update must track the live point.
    target->transform.position = {-50.0f, -50.0f, 0.0f};
    check(session.world()->eligible_target(*actor, *target),
          label + " retarget bearing invalidated the same live enemy");
    const auto random_before_retarget_turn = session.world()->random_state();
    dh2::actor::RotationState expected_retarget = expected_active;
    const float retarget_direction[]{-50.0f,-50.0f,0.0f};
    check(dh2_nav_look_towards(&expected_retarget.heading_angle,retarget_direction) == 0 &&
          std::abs(expected_retarget.heading_angle + pi / 4.0f) < 1e-6f &&
          dh2_actor_update_rotation(&expected_retarget,&rotation_policy,&sync_visual) == 0,
          label + " original retarget turn reference rejected");
    check(session.update(.016, input, {0,0,0}, actor->transform.rotation[2], error), error);
    const float second_heading = actor->transform.rotation[2];
    check(std::isfinite(second_heading) && second_heading == expected_retarget.rotation[2] &&
          second_heading < first_heading && second_heading > -pi / 4.0f,
          label + " held active attack did not apply the exact bounded turn toward retarget heading -pi/4");
    check(session.actor(1)->target_id == 2 && session.selectedactor() &&
          session.selectedactor()->id == 2 && session.owns_pose(1),
          label + " target tracking replaced target ownership or restarted the attack");
    for (const auto& boundary : session.combo_boundaries())
        check(boundary.generation == generation,
              label + " retarget-facing update created a duplicate attack generation");
    check(session.world()->random_state().seed == random_before_retarget_turn.seed &&
          session.world()->random_state().calls == random_before_retarget_turn.calls,
          label + " retarget-facing update consumed combat RNG");

    std::cout << "PASS " << profile_id << " -> actual Swamp_LizadMan_Type1; selection stays yaw0; "
              << "held attack turns boundedly toward +pi/4 then live retarget -pi/4; "
              << "generation/RNG preserved\n";
}
} // namespace

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply unified original asset root");
        AssetCatalog assets(argv[1]);
        std::string error;
        OriginalPropertyDatabase properties;
        OriginalMeleeBindings bindings;
        check(load_original_property_tables(assets, "original-cache/data/pydata", properties, error), error);
        check(bindings.load(assets, "original-melee-bindings.xml", error), error);
        run_profile(assets, properties, bindings, "KnightPlayerBase",
                    "_default_warrior-mesh-skin", "knight");
        run_profile(assets, properties, bindings, "RoguePlayerBase",
                    "_default_rogue-mesh-skin", "rogue");
        return 0;
    } catch (const std::exception& failure) {
        std::cerr << failure.what() << '\n';
        return 1;
    }
}
