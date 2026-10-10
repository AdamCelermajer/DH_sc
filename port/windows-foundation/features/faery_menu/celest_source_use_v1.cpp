#include "celest_source_use_v1.hpp"

#include <algorithm>
#include <limits>

namespace dh::foundation::faery_menu {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

bool same_owner(const std::weak_ptr<const void>& a,
                const std::weak_ptr<const void>& b) noexcept {
    return !a.owner_before(b) && !b.owner_before(a);
}

bool current_prepared(const CombatSession& session,
                      const CelestPreparedCastV1& prepared,
                      const CharacterState& character,
                      const HottyCooldownClockV1& clock,
                      std::string& error) {
    const auto lease = session.actor_binding_lease();
    if (lease.expired() || !clock.has_binding_lease || clock.binding_lease.expired() ||
        !same_owner(lease, clock.binding_lease))
        return fail(error, "Celest cast and cooldown clock do not share one live CombatSession owner");
    if (prepared.status != CelestPrepareStatusV1::prepared_pending_spell_combat &&
        prepared.status != CelestPrepareStatusV1::prepared_no_targets_pending_spell_combat)
        return fail(error, "Celest source occurrence requires a committed OnPre prefix");
    if (prepared.caster == invalid_actor_id || prepared.caster != session.player_id())
        return fail(error, "Celest source occurrence must use the same Session player");
    if (prepared.session_update_serial == 0 ||
        prepared.session_update_serial > session.update_serial() ||
        clock.session_update_serial != session.update_serial() ||
        prepared.cooldown_ready_at_ms <= clock.elapsed_ms)
        return fail(error, "Celest source cooldown clock is not current after preparation");
    const auto ready = clock.spell_ready_at_ms.find(prepared.caster);
    if (ready == clock.spell_ready_at_ms.end() ||
        ready->second != prepared.cooldown_ready_at_ms)
        return fail(error, "Celest source cooldown no longer belongs to this prepared cast");
    const auto* actor = session.actor(prepared.caster);
    if (!actor || !actor->persistent_character_id ||
        *actor->persistent_character_id != character.id)
        return fail(error, "Celest cast CharacterState is not the same Session player owner");
    error.clear();
    return true;
}
}

bool dispatch_celest_player_pre_best_effort_v1(
    CombatSession& session, const CelestPreparedCastV1& prepared,
    const CharacterState& character, const HottyCooldownClockV1& clock,
    CelestTargetFxDispatchV1* optional_fx, CelestEffectReceiptV1& output,
    std::string& error) {
    output = {};
    if (!current_prepared(session, prepared, character, clock, error)) {
        output.status = CelestEffectStatusV1::dispatch_failed;
        output.diagnostic = error;
        error.clear(); // Source PlayFX is non-veto after mana/cooldown commit.
        return true;
    }
    if (!optional_fx) {
        output.status = CelestEffectStatusV1::not_requested;
        output.diagnostic = "Celest_Level_1_Player FX dispatch is not bound; source prefix remains committed";
        error.clear();
        return true;
    }
    std::string dispatch_error;
    if (!optional_fx->dispatch_player_pre(session, prepared, character, clock,
                                          output.status, dispatch_error)) {
        output.status = CelestEffectStatusV1::dispatch_failed;
        output.diagnostic = dispatch_error.empty()
            ? "Celest_Level_1_Player FX dispatch failed after source mana/cooldown"
            : std::move(dispatch_error);
    }
    error.clear();
    return true;
}

bool apply_celest_source_use_v1(
    CombatSession& session, const CelestPreparedCastV1& prepared,
    const HottySourceTargetListV1& targets, const CharacterState& character,
    const HottyCooldownClockV1& clock,
    const dh2::data::FaeryTables::Borrow& faery_tables,
    const dh2::data::ClassTables& classes, CelestTargetFxDispatchV1* optional_fx,
    CelestSourceUseV1& output, std::string& error) {
    output = {};
    if (!current_prepared(session, prepared, character, clock, error)) return false;
    if (!targets.query_complete || !targets.authored_world_order_preserved ||
        targets.scope != HottyTargetScopeV1::current_character_population ||
        !prepared.original_target_order_known ||
        prepared.character_targets != targets.character_targets_in_source_order)
        return fail(error, "Celest Use requires the exact complete retained OnPre Character target order");
    if (!faery_tables || prepared.difficulty < 0 || prepared.difficulty >= 3 ||
        character.source_faery_list_id < 0 ||
        static_cast<std::size_t>(character.source_faery_list_id) >= faery_tables.lists().size() ||
        prepared.faery_slot < 0 || prepared.faery_slot >= 5 ||
        prepared.faery_record_id < 0 ||
        static_cast<std::size_t>(prepared.faery_record_id) >= faery_tables.faeries().size() ||
        prepared.skill_level < 0 || prepared.skill_level > 1)
        return fail(error, "Celest Use requires its same source Save selection, FaeryTables row and rank");
    const auto& save = character.faery_by_difficulty[static_cast<std::size_t>(prepared.difficulty)];
    const auto& list = faery_tables.lists()[static_cast<std::size_t>(character.source_faery_list_id)];
    if (list.size() != 5 || save.current_faery != prepared.faery_slot ||
        list[static_cast<std::size_t>(prepared.faery_slot)] != prepared.faery_record_id ||
        faery_tables.faery_names()[static_cast<std::size_t>(prepared.faery_record_id)] != "Celest" ||
        faery_tables.faeries()[static_cast<std::size_t>(prepared.faery_record_id)].script != "faerie_celest" ||
        faery_tables.faeries()[static_cast<std::size_t>(prepared.faery_record_id)].scalar.words[7] !=
            static_cast<std::uint32_t>(prepared.source_spell_type) ||
        std::min<std::int32_t>(save.faeries[static_cast<std::size_t>(prepared.faery_slot)].level, 1) !=
            prepared.skill_level)
        return fail(error, "Celest source row/slot/rank changed between OnPreSkill and OnSkill");
    auto* world = session.world();
    const auto* live = world ? world->combat_properties(prepared.caster) : nullptr;
    if (!live || classes.names.size() != classes.rows.size() ||
        prepared.spell_class_id < 0 ||
        static_cast<std::size_t>(prepared.spell_class_id) >= classes.names.size())
        return fail(error, "Celest Use source class/property tables are unavailable");
    const auto base_class = std::find(classes.names.begin(), classes.names.end(), "Spell_Lightning_1");
    const auto expected_class = base_class == classes.names.end() ? std::size_t(-1) :
        static_cast<std::size_t>(base_class - classes.names.begin()) +
            static_cast<std::size_t>(prepared.skill_level);
    const char* expected_name = prepared.skill_level == 0
        ? "Spell_Lightning_1" : "Spell_Lightning_2";
    if (expected_class >= classes.names.size() ||
        classes.names[expected_class] != expected_name ||
        prepared.spell_class_id != static_cast<std::int32_t>(expected_class))
        return fail(error, "Celest Use class is not actual Spell_Lightning_1 + saved rank");

    auto spell_properties = live->sheets.resolved;
    spell_properties[172] = std::int32_t(std::uint32_t(prepared.skill_level) << 8);
    if (!dh2::data::apply_class(classes, static_cast<std::int32_t>(expected_class),
                               spell_properties, error, &live->sheets.resolved)) {
        if (error.empty()) error = "Celest source ApplyPropClass failed before SpellCombatRoll";
        return false;
    }
    if (targets.character_targets_in_source_order.size() >
        (std::numeric_limits<std::uint32_t>::max() / 2u))
        return fail(error, "Celest doubled source occurrence indices exceed bounds");

    output.rolls.reserve(targets.character_targets_in_source_order.size() * 2);
    output.dead_target_calculations.reserve(targets.character_targets_in_source_order.size());
    output.target_effects.reserve(targets.character_targets_in_source_order.size());
    output.target_effects_dispatched = true;
    const auto lease = session.actor_binding_lease();
    for (std::size_t i = 0; i < targets.character_targets_in_source_order.size(); ++i) {
        const auto target = targets.character_targets_in_source_order[i];
        // The original Lua Character branch calls SpellCombatRoll twice, even
        // when the first result is lethal. The shared Session API currently
        // rejects that second calculation against its now-dead ActorState;
        // retain and report the reached prefix rather than double-applying.
        for (std::uint32_t roll = 0; roll < 2; ++roll) {
            CombatSessionSourceHit hit;
            hit.attacker = prepared.caster;
            hit.target = target;
            hit.binding_lease = lease;
            hit.generation = prepared.cooldown_ready_at_ms;
            hit.event_index = static_cast<std::uint32_t>(i * 2u + roll);
            hit.mask = 0x1005554Au;
            hit.source_id = "faerie_celest";
            hit.marker_name = "SpellCombatRoll";
            hit.category = -1;
            hit.element = prepared.source_spell_type;
            hit.direct_amount = 0;
            hit.attacker_formula_sheet = &spell_properties;

            const auto* target_actor = session.actor(target);
            if (!target_actor) {
                output.partial_prefix_diagnostic = "Celest target left the retained Session during its source roll pair";
                output.source_hits_applied = !output.rolls.empty();
                output.source_use_complete = false;
                return fail(error, "Celest source target disappeared before its next roll");
            }
            if (!target_actor->alive()) {
                // Lua invokes the second Character SpellCombatRoll even when
                // its first call killed the target. Preserve that source RNG
                // calculation on the same occurrence ledger without replaying
                // health, death or reaction effects. The Session still
                // publishes its non-veto formula-resolution observer receipt.
                CombatSessionSourceCalculation calculation;
                if (!session.resolve_source_result_only(hit, calculation, error)) {
                    output.partial_prefix_diagnostic = error.empty()
                        ? "Celest dead-target second formula occurrence was rejected" : error;
                    output.source_hits_applied = !output.rolls.empty();
                    output.source_use_complete = false;
                    return false;
                }
                if (!calculation.calculated) {
                    output.partial_prefix_diagnostic = "Celest dead-target formula occurrence was stale or duplicated";
                    output.source_hits_applied = !output.rolls.empty();
                    output.source_use_complete = false;
                    return fail(error, "Celest dead-target source formula was not calculated exactly once");
                }
                output.dead_target_calculations.push_back(
                    {target, hit.event_index, std::move(calculation.result)});
            } else {
                DamageEvent receipt;
                if (!session.apply_source_result(hit, receipt, error)) {
                    output.partial_prefix_diagnostic = error.empty()
                        ? "Celest SpellCombatRoll failed after an already reached source prefix" : error;
                    output.source_hits_applied = !output.rolls.empty();
                    output.source_use_complete = false;
                    return false;
                }
                if (!receipt.applied) {
                    output.partial_prefix_diagnostic = "Celest SpellCombatRoll occurrence was stale or duplicated";
                    output.source_hits_applied = !output.rolls.empty();
                    output.source_use_complete = false;
                    return fail(error, "Celest source occurrence was not applied exactly once");
                }
                const bool died = receipt.target_died;
                output.rolls.push_back(std::move(receipt));
                if (roll == 0 && died) {
                    // Continue to the next iteration, which uses the formula-
                    // only API after observing the dead target.
                }
            }
        }

        CelestEffectReceiptV1 effect;
        effect.target = target;
        if (!optional_fx) {
            effect.status = CelestEffectStatusV1::not_requested;
            effect.diagnostic = "Celest_Level_1_Main FX dispatch is not bound; source rolls remain applied";
            output.target_effects_dispatched = false;
        } else {
            std::string fx_error;
            if (!optional_fx->dispatch_target_main(session, prepared, character, clock,
                                                   target, effect.status, fx_error)) {
                effect.status = CelestEffectStatusV1::dispatch_failed;
                effect.diagnostic = fx_error.empty()
                    ? "Celest_Level_1_Main FX dispatch failed after both source rolls"
                    : std::move(fx_error);
                output.target_effects_dispatched = false;
            }
        }
        output.target_effects.push_back(std::move(effect));
    }
    output.source_hits_applied = true;
    output.source_use_complete = true;
    error.clear();
    return true;
}

} // namespace dh::foundation::faery_menu
