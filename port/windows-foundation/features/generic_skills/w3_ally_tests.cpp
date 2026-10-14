// v21.8 w3-ally: CharAI regen tick (RegenHP/RegenMP caps, AI_Tick clock, idle/combat cell choice), the in-combat predicate,
// and the per-conversion UseMana debit (Terror), against a real source Session (Knight player + two monsters).
#include "../../playable_actor_world.hpp"
#include "../../asset_catalog.hpp"
#include "../../original_actor_properties.hpp"
#include "../../original_combat_visual_plan.hpp"
#include "../../original_melee_bindings.hpp"
#include "../status_effects/vital_regen_v1.hpp"
#include "runtime_skill_cast_prepare_v1.hpp"
#include "runtime_skill_use_profile_v1.hpp"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <map>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::generic_skills;
using namespace dh::foundation::status_effects;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
constexpr unsigned kHpCur = 36, kHpMax = 38, kMpCur = 41, kMpMax = 43;
constexpr unsigned kRegenHpIdle = 39, kRegenHpCombat = 40, kRegenMpIdle = 44, kRegenMpCombat = 45;
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply repository root");
        const std::filesystem::path repo(argv[1]);
        AssetCatalog assets(repo / ".local-inputs/windows-source-clock-v19-preview-9/assets");
        std::string error;
        OriginalPropertyDatabase properties;
        check(load_original_property_tables(assets, "original-cache/data/pydata", properties, error), error);
        OriginalMeleeBindings bindings;
        check(bindings.load(AssetCatalog(repo / ".local-inputs/windows-melee-bindings"),
                            "original-melee-bindings.xml", error), error);
        ActorCustomization customization;
        customization.skin_id_contains = "_default_warrior-mesh-skin";
        customization.expected_controller_count = 4;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan plan;
        check(build_original_combat_visual_plan(assets, bindings, "KnightPlayerBase",
                  customization, "w3-ally-test", plan, error), error);
        CombatSessionConfig config;
        config.diagnosticRngSeed = 31;
        config.playerId = 1;
        config.playerProfileId = "KnightPlayerBase";
        config.tableRoot = "original-cache/data/pydata";
        config.playerVisualConfig = plan.config;
        config.playerVisualConfig.motion_node_id = "auto";
        config.playerVisualConfig.consume_root_motion = true;
        CombatSessionProfile profile;
        profile.initialIdle = {"Idle", 0, {0}};
        profile.animationOnly = true;
        profile.propertyOptions = {256, false};
        profile.sourceAnimationClips = plan.config.clips;
        config.profiles.emplace(config.playerProfileId, profile);
        CombatSession session;
        CharacterVisual player_visual;
        ActorPopulation population;
        check(session.initialize(assets, properties, bindings, config, player_visual,
                  population, {0, 0, 0}, customization, error), error);
        check(session.actor(1) && session.world()->combat_properties(1), "w3-ally Session lacks the player");
        dh2::data::PropertyRules rules;
        check(dh2::data::load_property_rules(properties.characters, rules, error), error);
        PlayableActorWorld& world = *session.world();
        {
            auto seeded = *world.combat_properties(1);
            seeded.sheets.resolved[kHpCur] = seeded.sheets.resolved[kHpMax];
            seeded.sheets.resolved[kMpCur] = seeded.sheets.resolved[kMpMax];
            check(world.update_combat_properties(1, seeded, *world.traits(1), error), error);
            session.actor(1)->max_health = float(seeded.sheets.resolved[kHpMax]) / 256.0f;
            session.actor(1)->health = session.actor(1)->max_health;
            check(session.actor(1)->alive(), "w3-ally test player must be alive");
        }
        const auto cells = [&](ActorId id) -> const dh2::data::PropertySheet& { return world.combat_properties(id)->sheets.resolved; };
        // A non-player monster (faction 2) bound into the same world, for the monster-side target rule.
        {
            auto props = *world.combat_properties(1);
            props.sheets.resolved[0] = 2;
            props.sheets.saved[0] = 2;
            auto traits = *world.traits(1);
            traits.is_player = false;
            traits.targetable = true;
            ActorState state;
            state.id = 2;
            state.definition_id = "w3_ally_monster";
            state.class_id = "w3_ally_test";
            state.faction_id = 2;
            state.health = state.max_health = 100.0f;
            check(world.bind_actor(state, props, traits, error), "monster bind failed: " + error);
        }

        // 1. RegenHP: a positive raw amount adds to current HP and the saved cell moves with it.
        {
            auto seeded = *world.combat_properties(1);
            seeded.sheets.resolved[kHpCur] = seeded.sheets.resolved[kHpMax] - 100 * 256;
            seeded.sheets.saved[kHpCur] = seeded.sheets.resolved[kHpCur];
            check(world.update_combat_properties(1, seeded, *world.traits(1), error), error);
            const auto before = cells(1)[kHpCur];
            const auto saved_before = world.combat_properties(1)->sheets.saved[kHpCur];
            check(world.regen_vitals(1, 30 * 256, 0, error), error);
            check(cells(1)[kHpCur] == before + 30 * 256, "RegenHP must add the raw amount to current HP");
            check(world.combat_properties(1)->sheets.saved[kHpCur] == saved_before + 30 * 256, "Saved HP cell must move with the regen");
            check(std::fabs(session.actor(1)->health - float(cells(1)[kHpCur]) / 256.0f) < 1e-3f, "ActorState health must mirror the regen");
        }
        // 2. Cap: an amount past the maximum is cut to the room left; at the maximum nothing is added.
        {
            check(world.regen_vitals(1, 100000 * 256, 0, error), error);
            check(cells(1)[kHpCur] == cells(1)[kHpMax], "RegenHP must cap current HP at the maximum");
            const auto full = cells(1)[kHpCur];
            check(world.regen_vitals(1, 50 * 256, 0, error) && cells(1)[kHpCur] == full, "RegenHP at the maximum must add nothing");
        }
        // 3. Non-positive amounts do nothing (RegenHP runs PROPS_Add only for a2 > 0).
        {
            const auto full = cells(1)[kHpCur];
            check(world.regen_vitals(1, -40 * 256, 0, error) && cells(1)[kHpCur] == full, "A negative regen must not change HP");
            check(world.regen_vitals(1, 0, 0, error) && cells(1)[kHpCur] == full, "A zero regen must not change HP");
        }
        // 4. RegenMP: current MP 41 is raised by its own amount and capped at 43.
        {
            auto seeded = *world.combat_properties(1);
            seeded.sheets.resolved[kMpCur] = 10 * 256;
            check(world.update_combat_properties(1, seeded, *world.traits(1), error), error);
            check(world.regen_vitals(1, 0, 7 * 256, error) && cells(1)[kMpCur] == 17 * 256, "RegenMP must add to current MP");
            check(std::fabs(session.actor(1)->resource - 17.0f) < 1e-3f, "ActorState resource must mirror the MP regen");
            check(world.regen_vitals(1, 0, 1000 * 256, error) && cells(1)[kMpCur] <= cells(1)[kMpMax], "RegenMP must cap at MP max");
        }
        // 5. Dead actor: the CharAI IsDead gate skips RegenTick entirely.
        {
            const auto saved_health = session.actor(1)->health;
            session.actor(1)->health = 0.0f;
            const auto before = cells(1)[kHpCur];
            check(world.regen_vitals(1, 50 * 256, 0, error) && cells(1)[kHpCur] == before, "A dead actor must not regenerate");
            session.actor(1)->health = saved_health;
        }

        // 6. Idle vs combat cell choice: monster 2 targets nothing, so idle cells 39/44 apply; attacking switches to 40/45.
        {
            auto seeded = *world.combat_properties(1);
            seeded.sheets.resolved[kRegenHpIdle] = 3 * 256;
            seeded.sheets.resolved[kRegenHpCombat] = 9 * 256;
            seeded.sheets.resolved[kRegenMpIdle] = 1 * 256;
            seeded.sheets.resolved[kRegenMpCombat] = 2 * 256;
            seeded.sheets.resolved[kHpCur] = seeded.sheets.resolved[kHpMax] - 100 * 256;
            seeded.sheets.saved[kHpCur] = seeded.sheets.resolved[kHpCur];
            seeded.sheets.resolved[kMpCur] = 20 * 256;
            seeded.sheets.saved[kMpCur] = seeded.sheets.resolved[kMpCur];
            check(world.update_combat_properties(1, seeded, *world.traits(1), error), error);
            session.actor(1)->action = CharacterAction::idle;
            session.actor(1)->target_id = invalid_actor_id;
            check(!actor_in_combat_v1(world, 1), "An idle player with no monster on it must be out of combat");
            auto hp0 = cells(1)[kHpCur], mp0 = cells(1)[kMpCur];
            check(regen_tick_v1(world, 1, error), error);
            check(cells(1)[kHpCur] == hp0 + 3 * 256 && cells(1)[kMpCur] == mp0 + 1 * 256,
                  "Idle regen must use Regen_HP (39) and Regen_MP (44)");
            session.actor(1)->action = CharacterAction::attacking;
            check(actor_in_combat_v1(world, 1), "An attacking actor is in combat (SM_IsAttacking)");
            hp0 = cells(1)[kHpCur];
            mp0 = cells(1)[kMpCur];
            check(regen_tick_v1(world, 1, error), error);
            check(cells(1)[kHpCur] == hp0 + 9 * 256 && cells(1)[kMpCur] == mp0 + 2 * 256,
                  "Combat regen must use CombatRegen_HP (40) and CombatRegen_MP (45)");
            session.actor(1)->action = CharacterAction::idle;
            // A monster that targets the player puts the player in combat (AI_IsAggroed approximation).
            world.find_actor(2)->target_id = 1;
            check(actor_in_combat_v1(world, 1), "A monster targeting the player must put the player in combat");
            world.find_actor(2)->target_id = invalid_actor_id;
            check(!actor_in_combat_v1(world, 1), "Clearing the monster target must end the combat state");
        }

        // 7. AI_Tick clock: one tick per 3000 ms of frame time; the remainder carries over.
        {
            std::map<ActorId, double> clock;
            check(kAiTickMsV1 == 3000.0, "AI_Tick must be 3000 ms (design_pycst.bin)");
            auto due = advance_vital_regen_clock_v1(clock, world, 2999.0);
            check(std::count(due.begin(), due.end(), ActorId(1)) == 0, "No regen tick before 3000 ms");
            due = advance_vital_regen_clock_v1(clock, world, 1.0);
            check(std::count(due.begin(), due.end(), ActorId(1)) == 1, "Exactly one regen tick at 3000 ms");
            due = advance_vital_regen_clock_v1(clock, world, 6500.0);
            check(std::count(due.begin(), due.end(), ActorId(1)) == 2, "6500 ms of frames must yield two ticks (remainder 500 ms carried)");
            clock.erase(2);
            due = advance_vital_regen_clock_v1(clock, world, 0.0);
            check(std::count(due.begin(), due.end(), ActorId(1)) == 0 && clock.count(1) == 1, "Zero dt yields no tick");
        }

        // 8. Terror per-conversion UseMana: the raw cost comes off current MP; a zero cost changes nothing.
        {
            auto seeded = *world.combat_properties(1);
            seeded.sheets.resolved[kMpCur] = 60 * 256;
            seeded.sheets.saved[kMpCur] = seeded.sheets.resolved[kMpCur];
            check(world.update_combat_properties(1, seeded, *world.traits(1), error), error);
            check(debit_skill_mana_v1(session, 1, 15 * 256, rules, error), error);
            check(cells(1)[kMpCur] == 45 * 256 && std::fabs(session.actor(1)->resource - 45.0f) < 1e-3f,
                  "UseMana debit must lower current MP and the ActorState mirror");
            check(debit_skill_mana_v1(session, 1, 0, rules, error) && cells(1)[kMpCur] == 45 * 256,
                  "A zero UseMana debit must not change MP");
        }

        // 9. Ally-aura cap helper (the PurifyingAura Heal(target) of the Lua): min(current + rank x 15, max).
        check(self_heal_target_hp_v1(100.0f, 283.0f, 15.0f, 2) == 130.0f, "Heal(target) adds rank x 15");
        check(self_heal_target_hp_v1(280.0f, 283.0f, 15.0f, 2) == 283.0f, "Heal(target) is capped at the maximum");

        std::cout << "{\"validation\":\"PASS\",\"tests\":\"w3-ally regen/clock/combat/terror-debit/heal-cap\"}\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "w3_ally_tests FAILED: " << ex.what() << '\n';
        return 1;
    }
}
