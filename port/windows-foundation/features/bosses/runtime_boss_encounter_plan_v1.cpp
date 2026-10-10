#include "runtime_boss_encounter_plan_v1.hpp"

#include <array>

namespace dh2::bosses::runtime_v1 {
namespace {

constexpr std::array<AttackScriptRow, 5> kSwampKingAttackScripts{{
    {"swamp_king_attack_1.luac", "SwampKingSkill5PCT", 300.0f, false},
    {"swamp_king_attack_2.luac", "SwampKingSkill30PCT", 200.0f, false},
    {"swamp_king_attack_3.luac", "SwampKingSkillPoisonAttack", 400.0f, true},
    {"swamp_king_attack_4.luac", "SwampKingSkillAOEPushback", 400.0f, true},
    {"swampking_atk_spit.luac", "SwampKingSkillPoisonAttack", -1.0f, true},
}};

constexpr std::array<TransitionRow, 8> kSwampKingTransitions{{
    {"phase1", "phase2", "swampking_OnUpdate", "HPPercentCurr <= phase2_initPct (75)", 0},
    {"idle", "intimidate", "swampking_OnTargetInRangedRange", "allowed && attack_flag && phase1", 0},
    {"intimidate", "idle", "swampking_OnEndOfAnim", "intimidateAnim_flag == 1; otherwise DoSkill(2)", 0},
    {"idle", "dive", "GoToDivePattern", "attack_flag cleared; native AI state == Idle", 0},
    {"dive", "emerge", "swampking_OnEndOfAnim", "diveAnim_flag", 1000},
    {"emerge", "decision", "swampking_OnEndOfAnim", "emergeAnim_flag", 500},
    {"decision", "dive", "timer_divepattern_flag callback", "DiveAttackDecision started it when diveCounter < 2", 8500},
    {"decision", "idle", "DiveAttackDecision", "diveCounter >= 2; reset counter and canDiveNow", 500},
}};

constexpr EncounterPlan kSwampKing{
    "swamp-king",
    "bossroom_ruins_ns__8",
    "_prim_Boss_SwampKing_01",
    "SwampKing",
    "IsBefore_Swamp_Escape",
    {"IsBefore_Swamp_Escape", 50, 1, 13},
    "swampking_core.luac",
    75,
    25,
    1500,
    15000,
    8500,
    1000,
    3000,
    2,
    "AIStates.Idle",
    "SK_STATE == ST_IDLE",
    "AIStates.Skill",
    false,
    true,
    true,
    3,
    kSwampKingTransitions.data(),
    kSwampKingTransitions.size(),
    kSwampKingAttackScripts.data(),
    kSwampKingAttackScripts.size(),
};

AttackDecision slot(int value, bool consumes_first = false) noexcept {
    return {DecisionKind::skill_slot, value, consumes_first};
}

} // namespace

const EncounterPlan& swamp_king_plan() noexcept { return kSwampKing; }

Phase phase_after_update(Phase current, float hp_percent) noexcept {
    if (current == Phase::phase1 && hp_percent <= kSwampKing.phase2_at_or_below_hp_percent)
        return Phase::phase2;
    return current;
}

AttackDecision close_attack(Phase phase, bool first_melee, int roll) noexcept {
    if (roll < 1 || roll > 100)
        return {DecisionKind::invalid_roll, -1, false};

    if (phase == Phase::phase1) {
        if (first_melee)
            return slot(1, true);
        return slot(roll < 80 ? 1 : 0);
    }

    if (phase == Phase::phase2) {
        if (first_melee)
            return slot(roll < 50 ? 0 : 1, true);
        if (roll < 40)
            return slot(0);
        if (roll < 90 && roll > 40)
            return slot(1);
        return {DecisionKind::no_skill_dispatched, -1, false};
    }

    return {DecisionKind::no_skill_dispatched, -1, false};
}

AttackDecision ranged_attack(Phase phase) noexcept {
    if (phase == Phase::phase1)
        return {DecisionKind::intimidate_then_skill_slot, 2, false};
    if (phase == Phase::phase2)
        return slot(2);
    return {DecisionKind::no_skill_dispatched, -1, false};
}

bool evaluate_quest_state_predicate(const QuestStatePredicate& predicate,
                                    std::int32_t actual_quest_state,
                                    bool& matched) noexcept {
    switch (predicate.comparison_id) {
    case 0:
        matched = predicate.argument_c == actual_quest_state;
        return true;
    case 1:
        matched = predicate.argument_c > actual_quest_state;
        return true;
    case 2:
        matched = predicate.argument_c < actual_quest_state;
        return true;
    default:
        matched = false;
        return false;
    }
}

bool evaluate_swamp_king_activation_from_quest_progress_v1(
    const dh::foundation::CharacterState& character,
    const dh::foundation::CharacterQuestProgressV1& progress,
    const dh2::data::QuestTablesPersistenceV51& tables,
    std::uint32_t collection, std::int32_t difficulty,
    bool& matched, std::string& error) {
    matched = false;
    const auto& predicate = kSwampKing.activation_quest_state_predicate;
    if (character.id.empty() || !tables.ready() ||
        predicate.quest_row < 0 ||
        static_cast<std::size_t>(predicate.quest_row) >= tables.rows().size() ||
        tables.rows()[static_cast<std::size_t>(predicate.quest_row)].name != "Swamp_Escape") {
        error = "Swamp King activation requires the original Quest table row Swamp_Escape";
        return false;
    }

    dh::foundation::CharacterQuestStateV1 source_state;
    if (!progress.query(character,
            {collection, difficulty, predicate.quest_row}, source_state, error))
        return false;
    if (!evaluate_quest_state_predicate(predicate, source_state.state, matched)) {
        error = "Swamp King activation uses an unsupported original Quest comparison";
        matched = false;
        return false;
    }
    error.clear();
    return true;
}

} // namespace dh2::bosses::runtime_v1
