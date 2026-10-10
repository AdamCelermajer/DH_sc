#include "runtime_boss_encounter_plan_v1.hpp"

#include <cassert>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh2::bosses::runtime_v1;

namespace {
std::vector<std::uint8_t> read_bytes(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("could not read original Quest cache file: " + path);
    return {std::istreambuf_iterator<char>(input), {}};
}
}

int main(int argc, char** argv) {
    assert(argc == 2);
    const auto& plan = swamp_king_plan();
    assert(plan.encounter_id == "swamp-king");
    assert(plan.scene_module == "bossroom_ruins_ns__8");
    assert(plan.scene_instance == "_prim_Boss_SwampKing_01");
    assert(plan.character_properties_id == "SwampKing");
    assert(plan.activation_condition_symbol == "IsBefore_Swamp_Escape");
    assert(plan.activation_quest_state_predicate.condition_name == "IsBefore_Swamp_Escape");
    assert(plan.activation_quest_state_predicate.quest_row == 50);
    assert(plan.activation_quest_state_predicate.comparison_id == 1);
    assert(plan.activation_quest_state_predicate.argument_c == 13);
    assert(plan.phase2_at_or_below_hp_percent == 75);
    assert(plan.weak_animation_at_or_below_hp_percent == 25);
    assert(plan.attack_ready_timer_ms == 1500);
    assert(plan.dive_start_timer_ms == 15000);
    assert(plan.dive_pattern_timer_ms == 8500);
    assert(plan.dive_emerge_delay_ms == 1000);
    assert(plan.dive_retarget_delay_ms == 3000);
    assert(plan.dives_before_reset == 2);
    assert(plan.activation_native_ai_state == "AIStates.Idle");
    assert(plan.attack_ready_script_state == "SK_STATE == ST_IDLE");
    assert(plan.attack_ready_excluded_native_ai_state == "AIStates.Skill");
    assert(!plan.attack_ready_requires_target);
    assert(plan.attack_ready_requires_dive_unavailable);
    assert(plan.attack_dispatch_requires_allowed);
    assert(plan.emerge_skill_slot == 3);
    assert(plan.transition_count == 8);
    assert(plan.transitions[0].from == "phase1" && plan.transitions[0].to == "phase2");
    assert(plan.transitions[0].source_gate == "HPPercentCurr <= phase2_initPct (75)");
    assert(plan.transitions[4].from == "dive" && plan.transitions[4].to == "emerge");
    assert(plan.transitions[4].delay_ms == 1000);
    assert(plan.transitions[5].from == "emerge" && plan.transitions[5].to == "decision");
    assert(plan.transitions[5].delay_ms == 500);
    assert(plan.transitions[6].from == "decision" && plan.transitions[6].to == "dive");
    assert(plan.transitions[6].delay_ms == 8500);
    assert(plan.transitions[7].from == "decision" && plan.transitions[7].to == "idle");
    assert(plan.transitions[7].delay_ms == 500);
    assert(plan.attack_script_count == 5);

    assert(phase_after_update(Phase::phase1, 75.01f) == Phase::phase1);
    assert(phase_after_update(Phase::phase1, 75.0f) == Phase::phase2);
    assert(phase_after_update(Phase::phase1, 25.0f) == Phase::phase2);
    assert(phase_after_update(Phase::phase2, 100.0f) == Phase::phase2);
    assert(phase_after_update(Phase::unknown, 1.0f) == Phase::unknown);

    auto d = close_attack(Phase::phase1, true, 100);
    assert(d.kind == DecisionKind::skill_slot && d.skill_slot == 1 && d.consumes_first_melee);
    d = close_attack(Phase::phase1, false, 79);
    assert(d.kind == DecisionKind::skill_slot && d.skill_slot == 1 && !d.consumes_first_melee);
    d = close_attack(Phase::phase1, false, 80);
    assert(d.kind == DecisionKind::skill_slot && d.skill_slot == 0);

    d = close_attack(Phase::phase2, true, 49);
    assert(d.kind == DecisionKind::skill_slot && d.skill_slot == 0 && d.consumes_first_melee);
    d = close_attack(Phase::phase2, true, 50);
    assert(d.kind == DecisionKind::skill_slot && d.skill_slot == 1 && d.consumes_first_melee);
    d = close_attack(Phase::phase2, false, 39);
    assert(d.kind == DecisionKind::skill_slot && d.skill_slot == 0);
    d = close_attack(Phase::phase2, false, 40);
    assert(d.kind == DecisionKind::no_skill_dispatched);
    d = close_attack(Phase::phase2, false, 41);
    assert(d.kind == DecisionKind::skill_slot && d.skill_slot == 1);
    d = close_attack(Phase::phase2, false, 89);
    assert(d.kind == DecisionKind::skill_slot && d.skill_slot == 1);
    d = close_attack(Phase::phase2, false, 90);
    assert(d.kind == DecisionKind::no_skill_dispatched);
    d = close_attack(Phase::phase2, false, 100);
    assert(d.kind == DecisionKind::no_skill_dispatched);
    d = close_attack(Phase::phase2, false, 0);
    assert(d.kind == DecisionKind::invalid_roll);
    d = close_attack(Phase::phase2, false, 101);
    assert(d.kind == DecisionKind::invalid_roll);

    d = ranged_attack(Phase::phase1);
    assert(d.kind == DecisionKind::intimidate_then_skill_slot && d.skill_slot == 2);
    d = ranged_attack(Phase::phase2);
    assert(d.kind == DecisionKind::skill_slot && d.skill_slot == 2);
    d = ranged_attack(Phase::unknown);
    assert(d.kind == DecisionKind::no_skill_dispatched);

    bool matched = false;
    const auto& predicate = plan.activation_quest_state_predicate;
    for (int source_state = 0; source_state <= 12; ++source_state) {
        assert(evaluate_quest_state_predicate(predicate, source_state, matched));
        assert(matched);
    }
    assert(evaluate_quest_state_predicate(predicate, 13, matched) && !matched);
    assert(evaluate_quest_state_predicate(predicate, 14, matched) && !matched);
    auto unknown_predicate = predicate;
    unknown_predicate.comparison_id = 7;
    assert(!evaluate_quest_state_predicate(unknown_predicate, 0, matched) && !matched);

    // Exercise the typed same-Character CQPG provider against the retained
    // original table rather than passing a raw state callback at the boundary.
    const std::string cache = argv[1];
    const auto quest_rows = read_bytes(cache + "/v2quests_pyarray.bin");
    const auto quest_names = read_bytes(cache + "/v2quests_pyarraynames.bin");
    dh2::data::QuestTablesPersistenceV51 source_tables;
    std::string error;
    assert(source_tables.decode({quest_rows.data(), quest_rows.size()},
                                {quest_names.data(), quest_names.size()}, error));
    assert(source_tables.rows().size() > 50);
    assert(source_tables.rows()[50].name == "Swamp_Escape");

    dh::foundation::CharacterState character;
    character.id = "profile-swamp-gate";
    dh::foundation::CharacterQuestProgressV1 mutable_progress;
    assert(mutable_progress.initialize_fresh(character, source_tables, error));
    const dh::foundation::CharacterQuestIdV1 swamp_escape{0, 1, 50};
    assert(mutable_progress.record_source_state(character, swamp_escape, 12, error));
    assert(mutable_progress.encode(source_tables, character.source_quest_progress_cqpg, error));
    dh::foundation::CharacterQuestProgressV1 loaded_progress;
    assert(loaded_progress.decode(character, source_tables,
                                  character.source_quest_progress_cqpg, error));
    assert(evaluate_swamp_king_activation_from_quest_progress_v1(
        character, loaded_progress, source_tables, 0, 1, matched, error));
    assert(matched);

    assert(mutable_progress.record_source_state(character, swamp_escape, 13, error));
    assert(mutable_progress.encode(source_tables, character.source_quest_progress_cqpg, error));
    assert(loaded_progress.decode(character, source_tables,
                                  character.source_quest_progress_cqpg, error));
    assert(evaluate_swamp_king_activation_from_quest_progress_v1(
        character, loaded_progress, source_tables, 0, 1, matched, error));
    assert(!matched);

    dh::foundation::CharacterState other_character;
    other_character.id = "different-profile";
    assert(!evaluate_swamp_king_activation_from_quest_progress_v1(
        other_character, loaded_progress, source_tables, 0, 1, matched, error));
    assert(!matched && !error.empty());
    dh::foundation::CharacterQuestProgressV1 unknown_progress;
    assert(!evaluate_swamp_king_activation_from_quest_progress_v1(
        character, unknown_progress, source_tables, 0, 1, matched, error));
    assert(!matched && !error.empty());
    assert(!evaluate_swamp_king_activation_from_quest_progress_v1(
        character, loaded_progress, source_tables, 0, 3, matched, error));
    assert(!matched && !error.empty());

    assert(plan.attack_scripts[0].class_table_id == "SwampKingSkill5PCT");
    assert(plan.attack_scripts[1].class_table_id == "SwampKingSkill30PCT");
    assert(plan.attack_scripts[2].class_table_id == "SwampKingSkillPoisonAttack");
    assert(plan.attack_scripts[3].class_table_id == "SwampKingSkillAOEPushback");
    assert(plan.attack_scripts[4].range == -1.0f); // Runtime character-table range.
    return 0;
}
