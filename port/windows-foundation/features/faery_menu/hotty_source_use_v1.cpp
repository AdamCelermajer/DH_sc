#include "hotty_source_use_v1.hpp"

#include <algorithm>
#include <utility>

namespace dh::foundation::faery_menu {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}
}

bool apply_hotty_source_use_v1(
    CombatSession& session, const HottyPreparedCastV1& prepared,
    const HottySourceTargetListV1& source_targets,
    const CharacterState& character, const HottyCooldownClockV1& clock,
    const dh2::data::FaeryTables::Borrow& faery_tables,
    const dh2::data::ClassTables& classes,
    HottyTargetFxDispatchV1* optional_fx, HottySourceUseV1& output,
    std::string& error) {
    output = {};
    if (!source_targets.query_complete || !source_targets.authored_world_order_preserved ||
        source_targets.scope != HottyTargetScopeV1::current_character_population ||
        !prepared.original_target_order_known ||
        prepared.character_targets != source_targets.character_targets_in_source_order)
        return fail(error, "Hotty source Use requires the complete retained OnPre target list in authored order");

    output.combat.generation = prepared.cooldown_ready_at_ms;
    if (source_targets.character_targets_in_source_order.empty()) {
        if (!apply_hotty_character_targets_v1(
                session, character, clock, prepared, source_targets,
                faery_tables, classes, output.combat, error)) return false;
        output.source_hits_applied = true;
        output.target_effects_dispatched = true;
        error.clear();
        return true;
    }

    output.combat.hits.reserve(source_targets.character_targets_in_source_order.size());
    output.target_effects.reserve(source_targets.character_targets_in_source_order.size());
    // Preserve Lua's per-target order: apply one SpellCombatRoll, then attempt
    // that result's positive-return FX before reaching the next target.
    for (std::size_t i = 0; i < source_targets.character_targets_in_source_order.size(); ++i) {
        const ActorId target = source_targets.character_targets_in_source_order[i];
        auto one_prepared = prepared;
        auto one_targets = source_targets;
        one_prepared.character_targets.assign(1, target);
        one_targets.character_targets_in_source_order = one_prepared.character_targets;
        HottyCharacterApplyV1 one_hit;
        std::string hit_error;
        if (!apply_hotty_character_targets_v1(
                session, character, clock, one_prepared, one_targets,
                faery_tables, classes, one_hit, hit_error,
                static_cast<std::uint32_t>(i))) {
            output.combat.status = HottyCharacterApplyStatusV1::source_ordered_character_hits_partially_applied;
            error = std::move(hit_error);
            if (error.empty()) error = "Hotty source hit failed after a reached ordered target prefix";
            return false;
        }
        if (one_hit.status != HottyCharacterApplyStatusV1::source_ordered_character_hits_applied ||
            one_hit.hits.size() != 1) {
            output.combat.status = HottyCharacterApplyStatusV1::source_ordered_character_hits_partially_applied;
            return fail(error, "Hotty source target loop did not produce exactly one applied SpellCombatRoll receipt");
        }

        const auto hit = one_hit.hits.front();
        output.combat.hits.push_back(hit);
        HottyTargetEffectReceiptV1 effect;
        effect.target = hit.target;
        if (!hit.spell_return_positive) {
            effect.status = HottyEffectStatusV1::source_branch_skipped;
        } else if (!optional_fx) {
            effect.status = HottyEffectStatusV1::not_requested;
            effect.diagnostic = "Hotty target effect dispatch was not bound; source hit remains applied";
            output.target_effects_dispatched = false;
        } else {
            std::string fx_error;
            if (!optional_fx->dispatch_positive_target(
                    session, prepared, character, clock, hit.target,
                    effect.status, fx_error)) {
                effect.status = HottyEffectStatusV1::dispatch_failed;
                effect.diagnostic = fx_error.empty() ?
                    "Hotty target effect dispatch failed after the source hit" : std::move(fx_error);
                output.target_effects_dispatched = false;
            }
        }
        output.target_effects.push_back(std::move(effect));
    }

    output.combat.status = HottyCharacterApplyStatusV1::source_ordered_character_hits_applied;
    output.source_hits_applied = true;
    // Preserve false set by any missing/failed positive FX branch. Skipped
    // branches and successful FX are fully dispatched source decisions.
    if (output.target_effects_dispatched ||
        std::all_of(output.target_effects.begin(), output.target_effects.end(), [](const auto& item) {
            return item.status == HottyEffectStatusV1::source_branch_skipped ||
                   item.status == HottyEffectStatusV1::source_instance_created;
        })) output.target_effects_dispatched = true;
    error.clear();
    return true;
}

} // namespace dh::foundation::faery_menu
