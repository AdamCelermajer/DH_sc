#include "runtime_skill_cast_prepare_v1.hpp"

#include "../../playable_actor_world.hpp"
#include "../../original_combat_properties.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <utility>

namespace dh::foundation::generic_skills {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

bool source_bypass(const SkillManaSourceFactsV1& facts, bool& bypass,
                   std::string& error) {
    if (!facts.application_byte5) return fail(error, "Source Application.byte5 policy is unavailable");
    bypass = false;
    if (*facts.application_byte5) {
        if (!facts.is_player) return fail(error, "Source player classification is unavailable after Application.byte5");
        bypass = *facts.is_player;
    }
    return true;
}

bool character_vitals_match(const CharacterState& character, const ActorState& actor,
                            const dh2::data::PropertySheet& resolved,
                            std::string& error) {
    const float mp = original_signed256(resolved[41]);
    const float max_mp = original_signed256(resolved[43]);
    if (!std::isfinite(mp) || !std::isfinite(max_mp) || mp < 0 || max_mp < mp)
        return fail(error, "Source MP property sheet is outside the supported nonnegative vital range");
    if (actor.resource != mp || actor.max_resource != max_mp ||
        character.stats.resource != mp || character.stats.max_resource != max_mp)
        return fail(error, "CharacterState/Session vitals do not match the same live source MP property sheet");
    return true;
}
} // namespace

bool resolve_assigned_skill_position_v1(
    const CharacterState& character, dh2::data::SkillTables::Borrow tables,
    std::uint32_t equipment_set, std::uint32_t source_slot,
    int& class_skill_position, std::uint32_t& saved_skill_row,
    std::string& error) {
    error.clear();
    if (!tables || !character.source_skill_slots_known)
        return fail(error, "Skill hotbar dispatch requires source-known assignments and pinned SkillTables");
    if (source_slot > 2)
        return fail(error, "NativeHUDSkill accepts only source hotbar slots 0, 1, and 2");
    const SkillSlotBinding* binding = nullptr;
    for (const auto& candidate : character.skill_slots) {
        if (candidate.equipment_set != equipment_set || candidate.slot != source_slot) continue;
        if (binding) return fail(error, "CharacterState contains duplicate bindings for this source hotbar slot");
        binding = &candidate;
    }
    if (!binding || binding->saved_skill_row >= character.skills.size())
        return fail(error, "Selected source hotbar slot has no valid saved skill row");
    const auto row = binding->saved_skill_row;
    const auto& skill_id = character.skills[row].id;
    const auto source_id = std::find(tables.skill_names().begin(), tables.skill_names().end(), skill_id);
    if (source_id == tables.skill_names().end() || character.skills[row].rank == 0)
        return fail(error, "Selected source hotbar row is not a learned original SkillTable entry");

    bool matched_source_list = false;
    for (const auto& list : tables.lists()) {
        if (list.size() != character.skills.size()) continue;
        bool matches = true;
        for (std::size_t i = 0; i < list.size(); ++i) {
            const auto table_id = list[i];
            if (table_id < 0 || static_cast<std::size_t>(table_id) >= tables.skill_names().size() ||
                character.skills[i].id != tables.skill_names()[static_cast<std::size_t>(table_id)]) {
                matches = false;
                break;
            }
        }
        if (matches) {
            matched_source_list = true;
            break;
        }
    }
    if (!matched_source_list)
        return fail(error, "Saved source skill rows do not preserve an original SkillList position mapping");
    if (row > static_cast<std::uint32_t>(std::numeric_limits<int>::max()))
        return fail(error, "Saved skill row exceeds source class-list position range");
    class_skill_position = static_cast<int>(row);
    saved_skill_row = row;
    error.clear();
    return true;
}

bool pc_skill_number_to_source_slot_v1(unsigned key_number,
                                       std::uint32_t& source_slot,
                                       std::string& error) {
    error.clear();
    if (key_number < 1 || key_number > 3) {
        error = "PC skill number must identify one of the three authored mapping positions 1..3";
        return false;
    }
    // Exact native Skills-page art/control order. The separate gameplay HUD
    // itself shows source slots 0/1/2 in a different visual order.
    constexpr std::uint32_t source_slots_by_visible_number[] = {2, 0, 1};
    source_slot = source_slots_by_visible_number[key_number - 1];
    return true;
}

bool prepare_skill_cast_mana_v1(
    CharacterState& character, CombatSession& session, ActorId actor_id,
    const dh2::data::CharacterTable& characters, dh2::data::SkillTables::Borrow tables,
    const dh2::data::ClassTables& classes, const dh2::data::PropertyRules& rules,
    int position, const std::string& class_token, const SkillManaSourceFactsV1& facts,
    SkillManaPrepareResultV1& output, std::string& error) {
    error.clear();
    if (actor_id == invalid_actor_id || !session.world())
        return fail(error, "Skill mana preparation requires the existing CombatSession world and actor");
    auto* actor = session.actor(actor_id);
    auto* world_actor = session.world()->find_actor(actor_id);
    const auto* traits = session.world()->traits(actor_id);
    const auto* current = session.world()->combat_properties(actor_id);
    if (!actor || actor != world_actor || !traits || !current)
        return fail(error, "Same-session actor combat-property owner is unavailable");
    if (!actor->persistent_character_id || *actor->persistent_character_id != character.id ||
        actor->definition_id != character.class_id) {
        error = "Skill request CharacterState does not identify this retained session actor: persistent=" +
            (actor->persistent_character_id ? *actor->persistent_character_id : "<missing>") +
            ", state=" + character.id + ", actor-definition=" + actor->definition_id +
            ", state-class=" + character.class_id;
        return false;
    }
    const auto resolved_class = std::find(classes.names.begin(), classes.names.end(), actor->class_id);
    if (resolved_class == classes.names.end() ||
        current->sheets.resolved[26] != static_cast<std::int32_t>(resolved_class - classes.names.begin()))
        return fail(error, "Session actor class and source property26 disagree with source ClassTables");
    if (rules.types[41] < 0 || !(rules.types[41] & (8 | 32)) || rules.types[43] < 0) {
        error = "Source property slot 41 is not a writable MP property or Max_MP slot 43 is absent: " +
            std::to_string(rules.types[41]) + "/" + std::to_string(rules.types[43]);
        return false;
    }

    SkillManaPrepareResultV1 next;
    CharacterDesignSkillCapsV1 caps;
    SkillProgressionV1 progression;
    if (!evaluate_skill_progression_v1(character, characters, tables, caps, position,
                                       progression, error)) return false;
    if (!progression.rank_known || progression.rank == 0 || progression.saved_skill_row < 0)
        return fail(error, "Source cast preparation requires a known positive rank in the selected class-list position");
    if (!progression.available)
        return fail(error, "Source SkillTable level requirement rejects this class-list position");
    if (!resolve_skill_visual_request_v1(character, characters, tables, position,
                                         next.skill, error)) return false;
    if (next.skill.skill_table_id != progression.skill_table_id ||
        next.skill.saved_rank != progression.rank || next.skill.class_id != character.class_id)
        return fail(error, "Source skill row changed while resolving cast preparation");

    const auto current_mp = current->sheets.resolved[41];
    if (!character_vitals_match(character, *actor, current->sheets.resolved, error)) return false;
    if (!evaluate_skill_mana_cost_v1(classes, rules, current->sheets.resolved, class_token,
                                     progression.rank, next.cost, error)) return false;
    if (next.cost.fixed_mana_cost < 0)
        return fail(error, "Original UseMana negative-cost assertion domain is unsupported");

    bool has_mana_bypass = false;
    if (!source_bypass(facts, has_mana_bypass, error)) return false;
    if (has_mana_bypass) {
        next.source_has_mana_check = true;
        next.status = SkillManaPrepareStatusV1::mana_bypass;
        next.mana_before = current_mp;
        next.mana_after = current_mp;
        output = std::move(next);
        error.clear();
        return true;
    }
    if (next.cost.fixed_mana_cost > current_mp) {
        next.source_has_mana_check = false;
        next.status = SkillManaPrepareStatusV1::insufficient_mana;
        next.mana_before = current_mp;
        next.mana_after = current_mp;
        output = std::move(next);
        error.clear();
        return true;
    }

    // The source-equivalent BashDown OnSkillCheck_ HasMana comparison is true.
    // Keep this computed callback result distinct from outer CharAI/FSM admission.
    next.source_has_mana_check = true;

    // Native UseMana calls the same Application/player bypass again before
    // querying GOD_MANA and the Character byte14f0 gate.
    bool use_bypass = false;
    if (!source_bypass(facts, use_bypass, error)) return false;
    if (use_bypass) {
        next.status = SkillManaPrepareStatusV1::mana_bypass;
        next.mana_before = current_mp;
        next.mana_after = current_mp;
        output = std::move(next);
        error.clear();
        return true;
    }
    if (!facts.god_mana_registered)
        return fail(error, "Reached source GOD_MANA DebugSwitches registration query is unavailable");
    if (*facts.god_mana_registered) {
        next.status = SkillManaPrepareStatusV1::mana_bypass;
        next.mana_before = current_mp;
        next.mana_after = current_mp;
        output = std::move(next);
        error.clear();
        return true;
    }
    if (!facts.god_mana_enabled)
        return fail(error, "Reached source GOD_MANA DebugSwitches value query is unavailable");
    if (*facts.god_mana_enabled) {
        next.status = SkillManaPrepareStatusV1::mana_bypass;
        next.mana_before = current_mp;
        next.mana_after = current_mp;
        output = std::move(next);
        error.clear();
        return true;
    }
    if (!facts.character_byte14f0)
        return fail(error, "Source Character byte14f0 mana bypass field is unavailable");
    if (*facts.character_byte14f0) {
        next.status = SkillManaPrepareStatusV1::mana_bypass;
        next.mana_before = current_mp;
        next.mana_after = current_mp;
        output = std::move(next);
        error.clear();
        return true;
    }

    // Nested HasMana is reached by UseMana after all bypass checks. The same
    // session property snapshot remains authoritative for this synchronous call.
    bool nested_bypass = false;
    if (!source_bypass(facts, nested_bypass, error)) return false;
    if (nested_bypass || next.cost.fixed_mana_cost > current_mp) {
        next.status = nested_bypass ? SkillManaPrepareStatusV1::mana_bypass
                                    : SkillManaPrepareStatusV1::insufficient_mana;
        next.mana_before = current_mp;
        next.mana_after = current_mp;
        output = std::move(next);
        error.clear();
        return true;
    }

    OriginalCombatProperties candidate = *current;
    auto view = dh2::data::property_view(rules, candidate.sheets);
    const auto delta = static_cast<std::int32_t>(0u -
        static_cast<std::uint32_t>(next.cost.fixed_mana_cost));
    if (dh2_property_add(&view, 41, delta))
        return fail(error, "Source UseMana property41 debit failed before publication");
    next.mana_before = current_mp;
    next.mana_after = candidate.sheets.resolved[41];
    next.mana_spent = next.mana_after != next.mana_before;
    next.status = SkillManaPrepareStatusV1::mana_committed;

    const auto* live_traits = session.world()->traits(actor_id);
    if (!live_traits) return fail(error, "Same-session actor traits disappeared before MP publication");
    if (!session.world()->update_combat_properties(actor_id, std::move(candidate),
                                                    *live_traits, error)) return false;
    actor->resource = original_signed256(next.mana_after);
    character.stats.resource = actor->resource;

    // Native UseMana queries this only after the property debit; a missing
    // reached provider is reported without undoing the source prefix.
    if (!facts.tracing_character_stats) {
        next.status = SkillManaPrepareStatusV1::provider_failure_after_mana_commit;
        output = std::move(next);
        return fail(error, "Reached source isTracingChar_Stats query failed after MP debit");
    }
    output = std::move(next);
    error.clear();
    return true;
}

} // namespace dh::foundation::generic_skills
