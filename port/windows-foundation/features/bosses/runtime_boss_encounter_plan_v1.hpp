#pragma once

#include <cstdint>
#include <cstddef>
#include <string_view>

#include "../../character_state.hpp"
#include "../quests/character_quest_progress_v1.hpp"
#include "../../../game-data/quest_persistence_v51.hpp"

namespace dh2::bosses::runtime_v1 {

// This plan carries authored identifiers and the source's exact attack-slot
// decisions. It deliberately does not evaluate campaign symbols or call the
// native AI/skill FSM; those authorities remain with their existing owners.
enum class Phase : std::uint8_t {
    unknown = 0,
    phase1 = 1,
    phase2 = 2,
};

enum class CombatRange : std::uint8_t {
    close,
    ranged,
};

enum class DecisionKind : std::uint8_t {
    invalid_roll,
    no_skill_dispatched,
    skill_slot,
    intimidate_then_skill_slot,
};

struct AttackDecision {
    DecisionKind kind = DecisionKind::no_skill_dispatched;
    int skill_slot = -1; // Exact argument passed to source DoSkill(...).
    bool consumes_first_melee = false;
};

struct AttackScriptRow {
    std::string_view file;
    std::string_view class_table_id;
    float range;
    bool multi_target;
};

struct TransitionRow {
    std::string_view from;
    std::string_view to;
    std::string_view source_event;
    std::string_view source_gate;
    std::uint32_t delay_ms;
};

// Exact one-stub Quest condition row transport. comparison_id is the actual
// v2Conditions factory index: 0 compares argument_c == Quest.state0, 1
// compares argument_c > Quest.state0, and 2 compares argument_c < state0.
// Production admission remains owned by NativeConditionRuntimeV69; this pure
// helper makes the recovered row reusable and testable without another state
// store or a second condition arena.
struct QuestStatePredicate {
    std::string_view condition_name;
    std::int32_t quest_row;
    std::int32_t comparison_id;
    std::int32_t argument_c;
};

struct EncounterPlan {
    std::string_view encounter_id;
    std::string_view scene_module;
    std::string_view scene_instance;
    std::string_view character_properties_id;
    std::string_view activation_condition_symbol;
    QuestStatePredicate activation_quest_state_predicate;
    std::string_view ai_script;
    int phase2_at_or_below_hp_percent;
    int weak_animation_at_or_below_hp_percent;
    std::uint32_t attack_ready_timer_ms;
    std::uint32_t dive_start_timer_ms;
    std::uint32_t dive_pattern_timer_ms;
    std::uint32_t dive_emerge_delay_ms;
    std::uint32_t dive_retarget_delay_ms;
    unsigned dives_before_reset;
    std::string_view activation_native_ai_state;
    std::string_view attack_ready_script_state;
    std::string_view attack_ready_excluded_native_ai_state;
    bool attack_ready_requires_target; // Source HasTarget only gates LookAtTarget side effect.
    bool attack_ready_requires_dive_unavailable;
    bool attack_dispatch_requires_allowed;
    int emerge_skill_slot;
    const TransitionRow* transitions;
    std::size_t transition_count;
    const AttackScriptRow* attack_scripts;
    std::size_t attack_script_count;
};

const EncounterPlan& swamp_king_plan() noexcept;

// Mirrors the source's one-way phase switch. The caller must pass the
// percentage emitted by the native Character property owner; no HP is read
// or normalized here.
Phase phase_after_update(Phase current, float hp_percent) noexcept;

// Mirrors Do_MeleeAttackPhase1/2 and the surrounding first-melee branch.
// roll must be the original GetRand(1, 100) result. Out-of-domain values are
// rejected rather than clamped. Source omissions (phase 2 rolls 40 and 90-100)
// return no_skill_dispatched; the plan does not fill those gaps.
AttackDecision close_attack(Phase phase, bool first_melee, int roll) noexcept;

// Ranged callbacks in both source phases ultimately dispatch DoSkill(2), but
// phase 1 traverses the intimidate animation/OnEndOfAnim callback first.
AttackDecision ranged_attack(Phase phase) noexcept;

// Returns false only for an unsupported native comparison ID. On success,
// matched is the exact original comparison against the supplied same-owner
// Quest state cell; state lookup, condition evaluation and state mutation are
// deliberately outside this helper.
bool evaluate_quest_state_predicate(const QuestStatePredicate& predicate,
                                    std::int32_t actual_quest_state,
                                    bool& matched) noexcept;

// Resolves the plan's exact quest row through the existing per-Character
// Quest progress owner. Callers load that owner from CharacterState's CQPG
// field using RuntimeQuestMenuV1::load_progress_from_character (or the same
// owner's decode API) before evaluating. Collection and difficulty are the
// current profile's source selection; unknown/mismatched owner or row fails.
bool evaluate_swamp_king_activation_from_quest_progress_v1(
    const dh::foundation::CharacterState&,
    const dh::foundation::CharacterQuestProgressV1&,
    const dh2::data::QuestTablesPersistenceV51&,
    std::uint32_t collection, std::int32_t difficulty,
    bool& matched, std::string& error);

} // namespace dh2::bosses::runtime_v1
