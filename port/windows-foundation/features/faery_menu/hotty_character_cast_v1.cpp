#include "hotty_character_cast_v1.hpp"

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
}

bool apply_hotty_character_targets_v1(
    CombatSession& session, const CharacterState& character,
    const HottyCooldownClockV1& clock, const HottyPreparedCastV1& prepared,
    const HottySourceTargetListV1& source_targets,
    const dh2::data::FaeryTables::Borrow& tables,
    const dh2::data::ClassTables& classes,
    HottyCharacterApplyV1& output, std::string& error,
    std::uint32_t event_index_base) {
    output = {};
    if (session.actor_binding_lease().expired())
        return fail(error, "Hotty SpellCombatRoll requires a live CombatSession lease");
    if (!clock.has_binding_lease || clock.binding_lease.expired() ||
        !same_owner(clock.binding_lease, session.actor_binding_lease()))
        return fail(error, "Hotty cooldown clock belongs to a different or expired CombatSession");
    if (!session.world() || prepared.caster != session.player_id() ||
        prepared.caster == invalid_actor_id)
        return fail(error, "Hotty SpellCombatRoll requires the same live session player");
    if (clock.session_update_serial != session.update_serial() ||
        prepared.session_update_serial == 0 ||
        prepared.session_update_serial > session.update_serial())
        return fail(error, "Hotty SpellCombatRoll requires current Session/cooldown time after preparation");
    if (prepared.status != HottyPrepareStatusV1::prepared_pending_spell_combat &&
        prepared.status != HottyPrepareStatusV1::prepared_no_targets_pending_spell_combat)
        return fail(error, "Hotty SpellCombatRoll requires a committed source OnPreSkill prefix");
    if (prepared.cooldown_ready_at_ms <= clock.elapsed_ms)
        return fail(error, "Hotty source cooldown expired before SpellCombatRoll");
    const auto cooldown = clock.spell_ready_at_ms.find(prepared.caster);
    if (cooldown == clock.spell_ready_at_ms.end() ||
        cooldown->second != prepared.cooldown_ready_at_ms)
        return fail(error, "Hotty source cooldown no longer belongs to this prepared cast");

    const auto* player = session.actor(prepared.caster);
    if (!player || !player->persistent_character_id ||
        *player->persistent_character_id != character.id)
        return fail(error, "Hotty SpellCombatRoll CharacterState differs from the session player owner");
    auto* world = session.world();
    const auto* live_properties = world ? world->combat_properties(prepared.caster) : nullptr;
    if (!character.source_faery_state_known || !tables ||
        character.source_faery_list_id < 0 ||
        static_cast<std::size_t>(character.source_faery_list_id) >= tables.lists().size() ||
        static_cast<std::size_t>(character.source_faery_list_id) >= tables.list_names().size() ||
        prepared.difficulty < 0 || prepared.difficulty >= 3 ||
        prepared.faery_slot != 4 || prepared.faery_record_id < 0 ||
        static_cast<std::size_t>(prepared.faery_record_id) >= tables.faeries().size() ||
        prepared.skill_level < 0 || prepared.skill_level > 1 ||
        prepared.source_spell_type < 0 || prepared.spell_class_id < 0)
        return fail(error, "Hotty SpellCombatRoll requires the same known Save difficulty and FaeryList row");
    const auto& list = tables.lists()[static_cast<std::size_t>(character.source_faery_list_id)];
    const auto& save = character.faery_by_difficulty[static_cast<std::size_t>(prepared.difficulty)];
    if (list.size() != 5 || save.current_faery != prepared.faery_slot ||
        list[static_cast<std::size_t>(prepared.faery_slot)] != prepared.faery_record_id ||
        tables.faery_names()[static_cast<std::size_t>(prepared.faery_record_id)] != "Hotty" ||
        tables.faeries()[static_cast<std::size_t>(prepared.faery_record_id)].script != "faerie_hotty" ||
        tables.faeries()[static_cast<std::size_t>(prepared.faery_record_id)].scalar.words[7] !=
            static_cast<std::uint32_t>(prepared.source_spell_type) ||
        std::min<std::int32_t>(save.faeries[static_cast<std::size_t>(prepared.faery_slot)].level, 1) !=
            prepared.skill_level)
        return fail(error, "Hotty SpellCombatRoll Faery selection/rank changed after preparation");
    const auto base_class = std::find(classes.names.begin(), classes.names.end(), "Spell_Fire_1");
    const auto expected_name = prepared.skill_level == 0 ? "Spell_Fire_1" : "Spell_Fire_2";
    const auto expected_class = base_class == classes.names.end() ? std::size_t(-1) :
        static_cast<std::size_t>(base_class - classes.names.begin()) +
            static_cast<std::size_t>(prepared.skill_level);
    if (!live_properties || classes.names.size() != classes.rows.size() ||
        expected_class >= classes.names.size() || classes.names[expected_class] != expected_name ||
        prepared.spell_class_id != static_cast<std::int32_t>(expected_class))
        return fail(error, "Hotty SpellCombatRoll class is not the actual contiguous Spell_Fire_1+rank source row");
    if (!source_targets.query_complete || !source_targets.authored_world_order_preserved ||
        source_targets.scope != HottyTargetScopeV1::current_character_population)
        return fail(error, "Hotty requires a complete current Character-population NoSort list in authored world order");
    if (!prepared.original_target_order_known ||
        prepared.character_targets != source_targets.character_targets_in_source_order)
        return fail(error, "Hotty source NoSort order differs from the list committed before mana/cooldown");
    if ((prepared.status == HottyPrepareStatusV1::prepared_no_targets_pending_spell_combat) !=
        source_targets.character_targets_in_source_order.empty())
        return fail(error, "Hotty source target list changed between OnPreSkill and SpellCombatRoll");
    if (source_targets.character_targets_in_source_order.empty()) {
        output.status = HottyCharacterApplyStatusV1::no_character_targets;
        output.generation = prepared.cooldown_ready_at_ms;
        error.clear();
        return true;
    }
    if (source_targets.character_targets_in_source_order.size() >
        std::numeric_limits<std::uint32_t>::max())
        return fail(error, "Hotty source target sequence exceeds event-index bounds");

    // The recovered Hotty OnSkill_ calls SetProp(SnS_Level) and
    // ApplyPropClass(CLASS_ID+skill_level) once before its ordered target loop.
    // This is after OnPre's UseMana, so project from the current live sheet.
    auto spell_properties = live_properties->sheets.resolved;
    spell_properties[172] = std::int32_t(std::uint32_t(prepared.skill_level) << 8);
    if (!dh2::data::apply_class(classes, static_cast<std::int32_t>(expected_class),
                               spell_properties, error,
                               &live_properties->sheets.resolved)) {
        if (error.empty()) error = "Hotty source ApplyPropClass projection failed before SpellCombatRoll";
        return false;
    }

    output.generation = prepared.cooldown_ready_at_ms;
    const auto lease = session.actor_binding_lease();
    output.hits.reserve(source_targets.character_targets_in_source_order.size());
    for (std::size_t i = 0; i < source_targets.character_targets_in_source_order.size(); ++i) {
        CombatSessionSourceHit hit;
        hit.attacker = prepared.caster;
        hit.target = source_targets.character_targets_in_source_order[i];
        hit.binding_lease = lease;
        hit.generation = output.generation;
        if (i > std::numeric_limits<std::uint32_t>::max() - event_index_base)
            return fail(error, "Hotty source target sequence exceeds event-index bounds");
        hit.event_index = event_index_base + static_cast<std::uint32_t>(i);
        hit.mask = 0x1005554Au;
        hit.source_id = "faerie_hotty";
        hit.marker_name = "SpellCombatRoll";
        hit.category = -1;
        hit.element = prepared.source_spell_type;
        hit.direct_amount = 0;
        hit.attacker_formula_sheet = &spell_properties;

        HottyCharacterHitV1 applied;
        applied.target = hit.target;
        if (!session.apply_source_result(hit, applied.receipt, error)) {
            output.status = HottyCharacterApplyStatusV1::source_ordered_character_hits_partially_applied;
            if (error.empty()) error = "Hotty source result/application failed after a reached target prefix";
            return false;
        }
        if (!applied.receipt.applied) {
            output.status = HottyCharacterApplyStatusV1::source_ordered_character_hits_partially_applied;
            return fail(error, "Hotty SpellCombatRoll occurrence was stale or already delivered");
        }
        applied.spell_return_positive = applied.receipt.requested_damage > 0.0f;
        output.hits.push_back(std::move(applied));
    }
    output.status = HottyCharacterApplyStatusV1::source_ordered_character_hits_applied;
    error.clear();
    return true;
}

} // namespace dh::foundation::faery_menu
