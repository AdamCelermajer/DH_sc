// Focused tests for the Stats +/- route (stat_training_v1). A real source Knight
// Session is built from the same assets as the skill training test; every
// assertion checks the live property sheet, the actor vitals and CharacterState.
#include "stat_training_v1.hpp"

#include "../../asset_catalog.hpp"
#include "../../original_actor_properties.hpp"
#include "../../original_combat_visual_plan.hpp"
#include "../../original_melee_bindings.hpp"

#include <filesystem>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::character_menu;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
std::int32_t whole(std::int32_t raw) { return raw >> 8; }
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
                  customization, "stat-training-test", plan, error), error);

        CombatSessionConfig config;
        config.diagnosticRngSeed = 29;
        config.playerId = 1;
        config.playerProfileId = "KnightPlayerBase";
        config.tableRoot = "original-cache/data/pydata";
        config.playerVisualConfig = plan.config;
        config.playerVisualConfig.motion_node_id = "auto";
        config.playerVisualConfig.consume_root_motion = true;
        CombatSessionProfile profile;
        profile.initialIdle = {"Idle", 0, {0}};
        profile.animationOnly = true;
        profile.propertyOptions = {256, false}; // level 1
        profile.sourceAnimationClips = plan.config.clips;
        config.profiles.emplace(config.playerProfileId, profile);
        CombatSession session;
        CharacterVisual player_visual;
        ActorPopulation population;
        check(session.initialize(assets, properties, bindings, config, player_visual,
                  population, {0, 0, 0}, customization, error), error);
        check(session.actor(1) && session.world()->combat_properties(1), "Stat test Session lacks the player");
        const auto* traits = session.world()->traits(1);
        check(traits != nullptr, "Stat test Session lacks player traits");
        dh2::data::PropertyRules rules;
        check(dh2::data::load_property_rules(properties.characters, rules, error), error);

        // Give the live sheet `points` Stat_Points (property 148) and mirror them in CharacterState.
        auto seed = [&](std::uint32_t points, CharacterState& state) {
            auto combat = *session.world()->combat_properties(1);
            state = CharacterState{};
            state.id = "stat-training-profile";
            state.name = "Knight";
            state.class_id = "KnightPlayerBase";
            state.stats.level = static_cast<std::uint32_t>(whole(combat.sheets.resolved[19]));
            state.source_points_known = true;
            state.source_endurance_energy_known = true;
            state.source_stat_points = points;
            state.source_skill_points = static_cast<std::uint32_t>(whole(combat.sheets.resolved[157]));
            auto view = dh2::data::property_view(rules, combat.sheets);
            check(dh2_property_set_int(&view, 148, static_cast<std::int32_t>(points)) == 0,
                  "Stat test could not seed Stat_Points");
            check(session.world()->update_combat_properties(1, combat, *traits, error), error);
            check(whole(session.world()->combat_properties(1)->sheets.resolved[148]) == int(points),
                  "Stat test Session did not retain seeded Stat_Points");
            state.stats.strength = float(whole(combat.sheets.resolved[149]));
            state.stats.dexterity = float(whole(combat.sheets.resolved[150]));
            state.stats.endurance = float(whole(combat.sheets.resolved[151]));
            state.stats.energy = float(whole(combat.sheets.resolved[152]));
            const auto* live = session.world()->combat_properties(1);
            state.stats.max_health = float(whole(live->sheets.resolved[38]));
            state.stats.max_resource = float(whole(live->sheets.resolved[43]));
            state.stats.health = session.actor(1)->health;
            state.stats.resource = session.actor(1)->resource;
        };
        auto persisted = std::size_t{};
        StatTrainingPersistV1 accept = [&](const CharacterState&, std::string&) { ++persisted; return true; };
        auto refuse_persist = [](const CharacterState&, std::string& message) {
            message = "forced save failure";
            return false;
        };

        // 1. Endurance spend: one point leaves Stat_Points, raises Endurance, and recalculates max HP.
        {
            CharacterState state;
            seed(2, state);
            const auto before_health = session.actor(1)->max_health;
            StatTrainingCommitV1 commit;
            check(train_stat_in_session_v1(state, session, properties, 2, accept, commit, error), error);
            check(persisted == 1, "Stat spend did not persist exactly once");
            check(commit.previous_points == 2 && commit.remaining_points == 1, "Stat spend point count");
            check(commit.current_value == commit.previous_value + 1, "Endurance did not rise by one");
            const auto* live = session.world()->combat_properties(1);
            check(whole(live->sheets.resolved[148]) == 1, "Live Stat_Points not debited");
            check(whole(live->sheets.resolved[151]) == commit.current_value, "Live Endurance not raised");
            check(commit.current_max_health - commit.previous_max_health == 4 &&
                  session.actor(1)->max_health == before_health + 4.0f,
                  "Endurance spend did not recalculate live max HP by the derived amount");
            check(state.source_stat_points == 1 && state.stats.endurance == float(commit.current_value) &&
                  state.stats.max_health == float(live->sheets.resolved[38]) / 256.0f,
                  "CharacterState was not mirrored after the Endurance spend");
            std::cout << "stat_training: endurance points=" << commit.previous_points << "->" << commit.remaining_points
                      << " maxHP=" << commit.previous_max_health << "->" << commit.current_max_health << "\n";
        }
        // 2. Energy spend raises max MP.
        {
            CharacterState state;
            seed(1, state);
            const auto before_mp = session.actor(1)->max_resource;
            StatTrainingCommitV1 commit;
            check(train_stat_in_session_v1(state, session, properties, 3, accept, commit, error), error);
            check(session.actor(1)->max_resource == before_mp + 4.0f && state.stats.max_resource == before_mp + 4.0f,
                  "Energy spend did not recalculate live max MP");
            check(state.source_stat_points == 0, "Energy spend did not debit the last point");
        }
        // 3. Strength spend changes the base stat only; vitals stay unchanged.
        {
            CharacterState state;
            seed(1, state);
            const auto before_hp = session.actor(1)->max_health;
            StatTrainingCommitV1 commit;
            check(train_stat_in_session_v1(state, session, properties, 0, accept, commit, error), error);
            check(commit.current_value == commit.previous_value + 1, "Strength did not rise by one");
            check(session.actor(1)->max_health == before_hp, "Strength changed max HP");
        }
        // 4. Refuse at zero points: no mutation, no persistence.
        {
            CharacterState state;
            seed(0, state);
            const auto before = state;
            const auto persists = persisted;
            StatTrainingCommitV1 commit;
            check(!train_stat_in_session_v1(state, session, properties, 1, accept, commit, error),
                  "Zero Stat_Points spend was accepted");
            check(error.find("no Stat_Points") != std::string::npos, "Zero-point refusal message: " + error);
            check(persisted == persists, "Refused spend persisted");
            check(state.source_stat_points == before.source_stat_points && state.stats.endurance == before.stats.endurance,
                  "Refused spend mutated CharacterState");
            check(whole(session.world()->combat_properties(1)->sheets.resolved[148]) == 0,
                  "Refused spend mutated the live sheet");
        }
        // 5. A failed save leaves the live Session and CharacterState unchanged.
        {
            CharacterState state;
            seed(2, state);
            const auto before_state = state;
            const auto before_strength = session.world()->combat_properties(1)->sheets.resolved[149];
            const auto before_hp = session.actor(1)->max_health;
            StatTrainingCommitV1 commit;
            check(!train_stat_in_session_v1(state, session, properties, 0, refuse_persist, commit, error),
                  "Stat spend succeeded although persistence failed");
            check(error == "forced save failure", "Persistence failure message was not kept: " + error);
            check(state.source_stat_points == before_state.source_stat_points &&
                  state.stats.strength == before_state.stats.strength,
                  "Failed save mutated CharacterState");
            check(session.world()->combat_properties(1)->sheets.resolved[149] == before_strength &&
                  session.actor(1)->max_health == before_hp,
                  "Failed save mutated the live Session");
        }
        // 6. Out-of-range stat index is refused before any work.
        {
            CharacterState state;
            seed(2, state);
            StatTrainingCommitV1 commit;
            check(!train_stat_in_session_v1(state, session, properties, 4, accept, commit, error),
                  "Stat index 4 was accepted");
        }
        // 7. Authored button hit regions (480x320 menu space).
        check(stats_training_button_at_v1(30.0f, 154.0f) == 0, "Strength hit region");
        check(stats_training_button_at_v1(30.0f, 200.0f) == 1, "Dexterity hit region");
        check(stats_training_button_at_v1(30.0f, 244.0f) == 2, "Endurance hit region");
        check(stats_training_button_at_v1(30.0f, 290.0f) == 3, "Energy hit region");
        check(stats_training_button_at_v1(240.0f, 160.0f) == -1, "Centre of the menu is not a stat button");

        // 8. Staged visit with two granted points: both are spendable in one visit,
        // a third is refused, nothing is saved until Yes, and the saved state matches.
        CharacterState saved;
        StatTrainingPersistV1 capture = [&](const CharacterState& c, std::string&) {
            ++persisted;
            saved = c;
            return true;
        };
        {
            CharacterState state;
            seed(2, state);
            StatTrainingVisitV1 visit;
            visit.open_visit();
            const auto persists = persisted;
            StatTrainingCommitV1 commit;
            check(train_stat_in_session_v1(state, session, properties, 0, StatTrainingPersistV1{}, commit, error), error);
            ++visit.staged[0];
            check(train_stat_in_session_v1(state, session, properties, 2, StatTrainingPersistV1{}, commit, error), error);
            ++visit.staged[2];
            check(state.source_stat_points == 0 && visit.staged_total() == 2, "Two staged spends did not consume both points");
            check(whole(session.world()->combat_properties(1)->sheets.resolved[148]) == 0,
                  "Staged spends did not debit the live Stat_Points");
            check(persisted == persists, "A staged spend was saved before Yes");
            std::string refusal;
            const auto before_third = state;
            check(!train_stat_in_session_v1(state, session, properties, 1, StatTrainingPersistV1{}, commit, refusal),
                  "Third spend with no Stat_Points was accepted");
            check(refusal.find("no Stat_Points") != std::string::npos, "Third spend refusal message: " + refusal);
            check(state.stats.dexterity == before_third.stats.dexterity && state.source_stat_points == 0,
                  "Refused third spend mutated CharacterState");
            // Yes: one save of the staged state, then the batch is cleared.
            check(commit_stat_visit_v1(state, visit, capture, error), error);
            check(persisted == persists + 1, "Yes did not persist exactly once");
            check(saved.source_stat_points == 0 && saved.stats.strength == state.stats.strength &&
                  saved.stats.endurance == state.stats.endurance, "Yes persisted a different state than the live one");
            check(!visit.has_staged(), "Yes did not clear the staged batch");
            check(!commit_stat_visit_v1(state, visit, capture, error), "Yes with no staged spend was accepted");
            std::cout << "stat_training: two-points visit strength=" << state.stats.strength
                      << " endurance=" << state.stats.endurance << " saved points=" << saved.source_stat_points << "\n";
        }
        // 9. No (cancel): every staged spend is refunded; the live sheet and the saved state
        // return to the values before the visit exactly.
        {
            CharacterState state;
            seed(2, state);
            const auto before_state = state;
            const auto before_sheet = session.world()->combat_properties(1)->sheets.resolved;
            const auto before_hp = session.actor(1)->max_health;
            StatTrainingVisitV1 visit;
            visit.open_visit();
            StatTrainingCommitV1 commit;
            check(train_stat_in_session_v1(state, session, properties, 1, StatTrainingPersistV1{}, commit, error), error);
            ++visit.staged[1];
            check(train_stat_in_session_v1(state, session, properties, 1, StatTrainingPersistV1{}, commit, error), error);
            ++visit.staged[1];
            check(session.world()->combat_properties(1)->sheets.resolved[150] != before_sheet[150] &&
                  whole(session.world()->combat_properties(1)->sheets.resolved[150]) == whole(before_sheet[150]) + 2,
                  "Staged Dexterity spends did not raise the live Dexterity by two");
            const auto persists = persisted;
            check(cancel_stat_visit_v1(state, session, properties, visit, capture, error), error);
            check(persisted == persists + 1, "No did not persist the reverted state once");
            check(state.source_stat_points == before_state.source_stat_points &&
                  state.stats.dexterity == before_state.stats.dexterity, "No did not restore CharacterState points/Dexterity");
            check(saved.source_stat_points == before_state.source_stat_points && saved.stats.dexterity == before_state.stats.dexterity,
                  "No saved a state other than the pre-visit state");
            check(session.world()->combat_properties(1)->sheets.resolved == before_sheet,
                  "No did not restore the live property sheet exactly");
            check(session.actor(1)->max_health == before_hp, "No did not restore the live max HP");
            check(!visit.has_staged(), "No did not clear the staged batch");
        }
        // 10. No with a failed save: the staged batch and both live states stay as they were.
        {
            CharacterState state;
            seed(2, state);
            StatTrainingVisitV1 visit;
            visit.open_visit();
            StatTrainingCommitV1 commit;
            check(train_stat_in_session_v1(state, session, properties, 3, StatTrainingPersistV1{}, commit, error), error);
            ++visit.staged[3];
            const auto staged_state = state;
            const auto staged_sheet = session.world()->combat_properties(1)->sheets.resolved;
            check(!cancel_stat_visit_v1(state, session, properties, visit, refuse_persist, error),
                  "No succeeded although the save failed");
            check(error == "forced save failure", "No failure message was not kept: " + error);
            check(state.source_stat_points == staged_state.source_stat_points && state.stats.energy == staged_state.stats.energy,
                  "Failed No changed CharacterState");
            check(session.world()->combat_properties(1)->sheets.resolved == staged_sheet, "Failed No changed the live sheet");
            check(visit.staged_total() == 1, "Failed No cleared the staged batch");
            check(!commit_stat_visit_v1(state, visit, refuse_persist, error) && visit.has_staged(),
                  "Failed Yes cleared the staged batch");
            check(error == "forced save failure", "Failed Yes message was not kept: " + error);
        }
        std::cout << "stat_training_v1 tests passed\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "stat_training_v1 failed: " << exception.what() << "\n";
        return 1;
    }
}
