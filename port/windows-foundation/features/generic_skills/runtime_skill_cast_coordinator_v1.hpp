#pragma once

#include "runtime_skill_cast_prepare_v1.hpp"
#include "runtime_skill_target_query_v1.hpp"
#include "runtime_skill_animation_bank_v1.hpp"
#include "../../actor_population.hpp"
#include "../../original_combat_visual_plan.hpp"
#include "../../original_attack_sequence.hpp"
#include "../faery_menu/hotty_cast_v1.hpp"
#include "../faery_menu/hotty_character_cast_v1.hpp"
#include "../faery_menu/hotty_effects_v1.hpp"
#include "../faery_menu/celest_source_use_v1.hpp"

#include <map>

namespace dh::foundation::generic_skills {

enum class RuntimeSkillCastPhaseV1 : std::uint8_t {
    rejected,
    prepared_pending_use,
    use_applied,
    completed,
    interrupted,
    partial_failure
};

enum class RuntimeSkillCastDispatchV1 : std::uint8_t {
    native_hud_skill,
    native_hud_spell
};

enum class RuntimeSkillSourceAnimStateV1 : std::uint8_t {
    skill = 6,
    cast = 7
};

// Internal discriminator resolved from the actual active Faery row script.
// Callers cannot select a spell implementation or request a fallback.
enum class RuntimeSkillFaeryScriptV1 : std::uint8_t {
    hotty,
    celest
};

enum class RuntimeWarriorSkillBranchV1 : std::uint8_t {
    bashdown,
    ground_slam,
    charge,
    rogue_jump_kick
};

struct RuntimeSkillFaerySpellArmV1 {
    // NativeHUDSpell is current-Faery dispatch, not a CharacterState hotbar
    // slot. The source list/rank/SpellType are resolved by the Faery adapter.
    const dh2::data::FaeryTables::Borrow* tables = nullptr;
    std::int32_t difficulty = -1;
    const faery_menu::HottySourcePolicyV1* policy = nullptr;
    const faery_menu::HottyAuthoredActorOrderV1* actor_order = nullptr;
    const std::vector<faery_menu::HottySourceActorFactsV1>* source_facts = nullptr;
    const faery_menu::HottySourceTargetListV1* targets = nullptr;
    faery_menu::HottyCooldownClockV1* cooldown_clock = nullptr;
    // Optional presentation-only FX dispatch. It never gates source mana or
    // same-session result application; failures are retained as diagnostics.
    faery_menu::HottyTargetFxDispatchV1* effect_dispatch = nullptr;
    faery_menu::CelestTargetFxDispatchV1* celest_effect_dispatch = nullptr;
    // Legacy construction facts remain available to older callers. The
    // coordinator does not require these for a valid combat cast.
    effects::RuntimeEffectsFactoryV1* effects_factory = nullptr;
    dh2::data::EffectsTables::Borrow effects_tables;
    faery_menu::HottyEffectIdsV1 effect_ids;
};

struct RuntimeSkillCastReceiptV1 {
    RuntimeSkillCastPhaseV1 phase = RuntimeSkillCastPhaseV1::rejected;
    ActorId actor = invalid_actor_id;
    std::uint64_t generation = 0;
    std::uint64_t session_update_serial = 0;
    std::uint32_t equipment_set = 0, source_slot = 0, saved_skill_row = 0;
    int class_skill_position = -1, skill_table_id = -1;
    std::string skill_name, source_script;
    std::vector<ActorId> target_order;
    std::vector<DamageEvent> applied_results;
    // Formula-only Celest rolls against a target killed by the prior source
    // roll consume the exact RNG occurrence but do not apply a second hit.
    std::vector<faery_menu::CelestDeadTargetCalculationV1> celest_dead_target_calculations;
    SkillManaPrepareResultV1 mana;
    bool native_hud_spell = false;
    std::int32_t faery_slot = -1, faery_record_id = -1, spell_type = -1;
    std::string detail;
};

// Current source inputs are borrowed from the actual Character owner and active
// population. The coordinator deliberately supports only source scripts whose
// complete Use payload has been recovered; an unsupported script rejects
// before committing mana or changing the actor/session.
struct RuntimeSkillCastRequestV1 {
    RuntimeSkillCastDispatchV1 dispatch = RuntimeSkillCastDispatchV1::native_hud_skill;
    ActorId actor = invalid_actor_id;
    CharacterState* character = nullptr;
    std::uint32_t equipment_set = 0, source_slot = 0;
    const dh2::data::CharacterTable* characters = nullptr;
    dh2::data::SkillTables::Borrow skills;
    const dh2::data::ClassTables* classes = nullptr;
    const dh2::data::PropertyRules* property_rules = nullptr;
    const ActorPopulation* population = nullptr;
    SkillManaSourceFactsV1 mana_policy;
    std::vector<SkillTargetSourceFactsV1> target_facts;
    // Exact visibility/zone/interactivity source query evidence, when those
    // native fields are available. The Character-only coordinator never
    // treats an absent object registry as a successful no-target result.
    std::optional<bool> non_character_attackable_objects_absent;
    const OriginalCombatVisualPlan* visual_plan = nullptr;
    const OriginalSequencePolicies* sequence_policies = nullptr;
    // When supplied, NativeHUDSkill selection is resolved fresh from the
    // current saved slot and this pre-init bank; request.selection is not a
    // source of stale assignment state.
    const RuntimeSkillAnimationBankV1* animation_bank = nullptr;
    OriginalAttackSelection selection;
    CombatSessionStateAnimationServices downstream_animation_services;
    RuntimeSkillFaerySpellArmV1* active_faery_spell = nullptr;
};

class RuntimeSkillCastCoordinatorV1 {
public:
    // Begin is the source Check/Pre prefix. It validates the saved hotbar
    // assignment and source preconditions, applies the source Pre effects,
    // then starts the exact source SkillTable animation root. Use is executed
    // only by `apply_use_v1` at the caller's actual authored Use/animation
    // boundary, never at request time.
    bool begin_skill_cast_v1(const RuntimeSkillCastRequestV1&,
                             CombatSession&, RuntimeSkillCastReceiptV1&,
                             std::string& error);
    // Call once immediately after every successful CombatSession::update.
    // This is the transient source SetSkillCooldown timer owner for supported
    // active skills and is bound to one Session lease/update stream.
    bool advance_after_session_update(CombatSession&, double update_dt_seconds,
                                      std::string& error);
    // Authored animation owner handoff. The state value comes from the exact
    // source event dispatcher: Warrior uses state6/do_skill; NativeHUDSpell
    // uses state7/do_spell (event40). The coordinator rejects cross-routing.
    bool apply_retained_use_event_v1(
        CombatSession&, ActorId, std::uint64_t generation,
        RuntimeSkillSourceAnimStateV1, const RetainedAnimationEvent&,
        RuntimeSkillCastReceiptV1&, std::string& error);
    // Source Post/clip completion boundary. Only scripts with an audited
    // unconditional ClearTarget callback publish the generic same-actor clear;
    // ordinary attack and empty/conditional Post paths retain their owners.
    bool complete_v1(CombatSession&, ActorId, std::uint64_t generation,
                     RuntimeSkillCastReceiptV1&, std::string& error);
    // Session's accepted departure callback calls this before incoming focus.
    // A pre-Use interruption retires the generation and runs its audited Post
    // without refunding source mana/cooldown or fabricating Use.
    bool source_departed_v1(CombatSession&, ActorId, std::uint64_t generation,
                            std::int32_t from_state, std::int32_t to_state,
                            std::string& error);
    // Source timers have no CharacterState save fields. A gameplay checkpoint
    // is allowed only after sequence and transient cooldown owners are quiescent.
    bool checkpoint_v1(CombatSession&, std::string& error) const;
    RuntimeSkillCastReceiptV1* receipt(ActorId) noexcept;
    const RuntimeSkillCastReceiptV1* receipt(ActorId) const noexcept;
    // Remaining fraction (1 at cast, 0 ready) of the active source SetSkillCooldown
    // timer for one actor/skill row; 0 when no timer is active. HUD-only read.
    double skill_cooldown_remaining_fraction_v1(ActorId, int skill_table_id) const noexcept;

private:
    // Lower-level implementation. Production callers must use
    // apply_retained_use_event_v1 so state6/state7 and marker identity are
    // checked before reaching the source Use payload.
    bool apply_use_v1(CombatSession&, ActorId, std::uint64_t generation,
                      RuntimeSkillCastReceiptV1&, std::string& error);
    struct ActiveCastV1 {
        RuntimeSkillCastReceiptV1 receipt;
        std::weak_ptr<const void> binding_lease;
        CharacterState* character = nullptr;
        const dh2::data::CharacterTable* characters = nullptr;
        const dh2::data::ClassTables* classes = nullptr;
        const dh2::data::PropertyRules* property_rules = nullptr;
        int source_class_id = -1;
        std::uint32_t row_mask = 0;
        std::int32_t element = -1, category = -1;
        RuntimeWarriorSkillBranchV1 warrior_branch = RuntimeWarriorSkillBranchV1::bashdown;
        std::string class_token;
        std::vector<ActorId> population_order;
        std::vector<SkillTargetSourceFactsV1> target_facts;
        std::optional<bool> non_character_attackable_objects_absent;
        float target_range = 0.0f, target_cone_radians = 3.1415927410125732421875f;
        SkillTargetSortV1 target_sort = SkillTargetSortV1::frontal_first;
        double cooldown_ms = 0.0;
        bool is_hotty = false;
        RuntimeSkillFaeryScriptV1 faery_script = RuntimeSkillFaeryScriptV1::hotty;
        dh2::data::FaeryTables::Borrow faery_tables;
        std::int32_t faery_difficulty = -1;
        faery_menu::HottySourcePolicyV1 hotty_policy;
        faery_menu::HottyAuthoredActorOrderV1 hotty_actor_order;
        std::vector<faery_menu::HottySourceActorFactsV1> hotty_source_facts;
        faery_menu::HottySourceTargetListV1 hotty_targets;
        faery_menu::HottyCooldownClockV1* hotty_cooldown_clock = nullptr;
        faery_menu::HottyPreparedCastV1 hotty_prepared;
        faery_menu::HottyTargetFxDispatchV1* hotty_effect_dispatch = nullptr;
        effects::RuntimeEffectsFactoryV1* hotty_effects_factory = nullptr;
        dh2::data::EffectsTables::Borrow hotty_effects_tables;
        faery_menu::HottyEffectIdsV1 hotty_effect_ids;
        faery_menu::HottyEffectStatusV1 hotty_player_pre_status =
            faery_menu::HottyEffectStatusV1::source_branch_skipped;
        faery_menu::CelestPreparedCastV1 celest_prepared;
        faery_menu::CelestTargetFxDispatchV1* celest_effect_dispatch = nullptr;
        faery_menu::CelestEffectReceiptV1 celest_player_pre;
        SkillActorTargetQueryV1 target_query;
        dh2::data::PropertySheet formula_sheet{};
        CharacterAction previous_action = CharacterAction::idle;
        bool source_post_clears_target = false;
        std::int32_t source_state = -1;
        std::uint32_t source_focus_flags = 0;
        bool source_post_delivered = false;
        bool use_delivered = false;
    };
    std::uint64_t next_generation_ = 1;
    std::uint64_t timer_update_serial_ = 0;
    double elapsed_ms_ = 0.0;
    bool timer_clock_bound_ = false;
    std::weak_ptr<const void> timer_binding_lease_;
    std::map<std::pair<ActorId, int>, double> skill_ready_at_ms_;
    // Same keys as skill_ready_at_ms_: the authored SetSkillCooldown duration,
    // so the HUD can show the remaining fraction (HUDBTN).
    std::map<std::pair<ActorId, int>, double> skill_cooldown_total_ms_;
    std::map<ActorId, faery_menu::HottyCooldownClockV1*> faery_cooldown_clocks_;
    std::map<ActorId, ActiveCastV1> active_;
};

} // namespace dh::foundation::generic_skills
