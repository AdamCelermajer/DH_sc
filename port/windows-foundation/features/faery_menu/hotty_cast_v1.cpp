#include "hotty_cast_v1.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <set>

namespace dh::foundation::faery_menu {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

std::int32_t wrap_word(std::uint32_t value) noexcept {
    std::int32_t result{};
    std::memcpy(&result, &value, sizeof(result));
    return result;
}

std::int32_t source_mul_fixed(std::int32_t left, std::int32_t right) noexcept {
    const auto product = std::uint32_t(left) * std::uint32_t(right);
    const auto shifted = (product >> 8) |
        ((product & 0x80000000u) ? 0xff000000u : 0u);
    return wrap_word(shifted);
}

float source_add(float a, float b) { volatile float r = a + b; return r; }
float source_sub(float a, float b) { volatile float r = a - b; return r; }
float source_mul(float a, float b) { volatile float r = a * b; return r; }
float source_length(const float v[3]) {
    return std::sqrt(source_add(source_add(source_mul(v[0],v[0]),
                                            source_mul(v[1],v[1])),
                                source_mul(v[2],v[2])));
}

const std::array<float, 3>& source_target_position(const ActorState& actor) {
    // GameObject::GetTargetPosition (0x3935dc) checks node180 first. A null
    // node returns transform+0x160 even when constructor cache+0x184 exists.
    // Only a non-null node can make the produced cache the selected position.
    if (actor.source_target_node180 && *actor.source_target_node180 != 0 &&
        actor.source_target_position184) {
        return *actor.source_target_position184;
    }
    return actor.transform.position;
}

bool source_player(PlayableActorWorld& world, ActorId id,
                   const CharacterState& state, ActorState*& actor,
                   std::string& error) {
    actor = id != invalid_actor_id ? world.find_actor(id) : nullptr;
    if (!actor || !actor->persistent_character_id ||
        *actor->persistent_character_id != state.id) {
        return fail(error, "Hotty cast requires the same session player ActorState and CharacterState identity");
    }
    return true;
}

bool valid_source_character_list(const PlayableActorWorld& world, ActorId caster_id,
    const HottySourceTargetListV1& source, std::int32_t range,
    std::vector<ActorId>& ordered, std::string& error) {
    if (!source.query_complete || !source.authored_world_order_preserved ||
        source.scope != HottyTargetScopeV1::current_character_population)
        return fail(error, "Hotty cast requires a complete current Character-population NoSort query in authored ActorPopulation order");
    const auto* caster = world.find_actor(caster_id);
    if (!caster || range <= 0)
        return fail(error, "Hotty source target list requires a live caster and authored range");
    const auto* caster_properties = world.combat_properties(caster_id);
    if (!caster_properties)
        return fail(error, "Hotty source target list requires the same-player source property sheet");
    std::vector<ActorId> next;
    next.reserve(source.character_targets_in_source_order.size());
    for (ActorId id : source.character_targets_in_source_order) {
        if (id == invalid_actor_id || id == caster_id ||
            std::find(next.begin(), next.end(), id) != next.end())
            return fail(error, "Hotty source NoSort target list contains an invalid or duplicate Character");
        const auto* target = world.find_actor(id);
        if (!target || !target->alive() || !world.eligible_target(*caster, *target))
            return fail(error, "Hotty source target list contains an ineligible same-world Character");
        const auto* target_properties = world.combat_properties(id);
        if (!target_properties || caster_properties->sheets.resolved[199] <
                                  target_properties->sheets.resolved[198])
            return fail(error, "Hotty source target list contains an invalid/stealth-gated Character");
        const auto* target_ai = dh2::data::ai_props(
            world.factions(), target_properties->sheets.resolved[1]);
        if (!target_ai)
            return fail(error, "Hotty source target list lacks the original interaction-radius AI row");
        const float* center = source_target_position(*target).data();
        const float* origin = source_target_position(*caster).data();
        const float delta[3] = {source_sub(center[0], origin[0]),
                                source_sub(center[1], origin[1]),
                                source_sub(center[2], origin[2])};
        const float distance = source_sub(
            source_sub(source_length(delta), target_ai->interact_radius),
            world.target_radius(*caster));
        if (!std::isfinite(distance) || distance > float(range))
            return fail(error, "Hotty source target list contains a Character outside 600/800 range");
        next.push_back(id);
    }
    ordered = std::move(next);
    error.clear();
    return true;
}

bool resolve_hotty_row(const CharacterState& state, std::int32_t difficulty,
                       const dh2::data::FaeryTables::Borrow& tables,
                       std::int32_t& slot, std::int32_t& row_id,
                       std::int32_t& spell_type, std::int32_t& skill_level,
                       std::string& error) {
    if (!state.source_faery_state_known)
        return fail(error, "Hotty cast requires source-known Faery Save state");
    if (!tables || difficulty < 0 || difficulty >= 3 ||
        state.source_faery_list_id < 0 ||
        std::size_t(state.source_faery_list_id) >= tables.lists().size() ||
        std::size_t(state.source_faery_list_id) >= tables.list_names().size())
        return fail(error, "Hotty cast requires the same known difficulty and actual FaeryList tables");

    const auto& save = state.faery_by_difficulty[std::size_t(difficulty)];
    slot = save.current_faery;
    const auto& list = tables.lists()[std::size_t(state.source_faery_list_id)];
    if (list.size() != 5 || slot < 0 || slot >= 5)
        return fail(error, "Hotty cast source slot/list is outside the original five Faery slots");
    row_id = list[std::size_t(slot)];
    if (row_id < 0 || std::size_t(row_id) >= tables.faeries().size() ||
        std::size_t(row_id) >= tables.faery_names().size())
        return fail(error, "Hotty cast FaeryList references an invalid source row");
    const auto& row = tables.faeries()[std::size_t(row_id)];
    // DEFAULT's Fake_Hotty row has no SpellScript. The actual class lists
    // select the playable Hotty row and its authored script.
    if (tables.faery_names()[std::size_t(row_id)] != "Hotty" ||
        row.script != "faerie_hotty") {
        error = "Active Faery spell is not Hotty; this source adapter does not substitute another spell";
        return false;
    }
    if (row.scalar.words[8] != std::uint32_t(slot))
        return fail(error, "Hotty Faery source type does not match the selected Save slot");

    const auto saved_level = save.faeries[std::size_t(slot)].level;
    skill_level = std::min<std::int32_t>(saved_level, 1);
    spell_type = static_cast<std::int32_t>(row.scalar.words[7]);
    error.clear();
    return true;
}

bool project_spell_properties(const dh2::data::ClassTables& classes,
                              const dh2::data::PropertyRules& rules,
                              const dh2::data::PropertySheet& live_resolved,
                              std::int32_t skill_level,
                              std::int32_t& class_id,
                              dh2::data::PropertySheet& output,
                              std::string& error) {
    if (classes.names.size() != classes.rows.size() ||
        rules.defaults[172] != 0 || rules.types[172] != 8)
        return fail(error, "Hotty requires the actual ClassTables and SnS_Level/MP source property schema");
    const auto base = std::find(classes.names.begin(), classes.names.end(), "Spell_Fire_1");
    if (base == classes.names.end())
        return fail(error, "Source Spell_Fire_1 class row is unavailable");
    class_id = static_cast<std::int32_t>(base - classes.names.begin());
    const auto expected = class_id + skill_level;
    if (expected < 0 || std::size_t(expected) >= classes.names.size() ||
        classes.names[std::size_t(expected)] !=
            (skill_level == 0 ? "Spell_Fire_1" : "Spell_Fire_2"))
        return fail(error, "Source Hotty Spell_Fire_1 + skill_level class rows are not contiguous");

    output = live_resolved;
    // ToFixed(0/1) is truncation followed by wrapping <<8. Hotty caps ranks at 1.
    output[172] = std::int32_t(std::uint32_t(skill_level) << 8);
    if (!dh2::data::apply_class(classes, expected, output, error, &live_resolved)) {
        if (error.empty()) error = "Source Hotty ApplyPropClass projection failed";
        return false;
    }
    error.clear();
    return true;
}

}

bool hotty_mana_cost_v1(std::uint16_t saved_faery_level,
                        std::int32_t player_level_fixed,
                        std::int32_t& skill_level, std::int32_t& fixed_cost,
                        std::string& error) {
    const auto level = std::min<std::uint32_t>(saved_faery_level, 1u);
    // Source ToFixed(0.2): (__aeabi_f2iz(0.2) << 8) == 0.
    constexpr std::int32_t fixed_level_multiplier = 0;
    const auto slope = source_mul_fixed(fixed_level_multiplier, player_level_fixed);
    const auto base = std::int32_t((level == 0 ? 10u : 20u) << 8);
    skill_level = static_cast<std::int32_t>(level);
    fixed_cost = wrap_word(std::uint32_t(base) + std::uint32_t(slope));
    error.clear();
    return true;
}

bool source_hotty_character_targets_v1(const PlayableActorWorld& world,
                                       ActorId caster_id, std::int32_t range,
                                       std::vector<ActorId>& targets,
                                       std::string& error) {
    const auto* caster = world.find_actor(caster_id);
    if (!caster || range <= 0)
        return fail(error, "Hotty source target query requires a live caster and positive authored range");
    std::vector<ActorId> next;
    const auto limit = double(range) * double(range);
    for (const auto& entry : world.actors()) {
        const auto& candidate = entry.second;
        if (!world.eligible_target(*caster, candidate)) continue;
        const double dx = double(candidate.transform.position[0]) - caster->transform.position[0];
        const double dy = double(candidate.transform.position[1]) - caster->transform.position[1];
        const double dz = double(candidate.transform.position[2]) - caster->transform.position[2];
        if (dx * dx + dy * dy + dz * dz <= limit) next.push_back(candidate.id);
    }
    targets = std::move(next);
    error.clear();
    return true;
}

bool query_hotty_character_targets_v1(
    const PlayableActorWorld& world, ActorId caster_id,
    const HottyAuthoredActorOrderV1& actor_order,
    const std::vector<HottySourceActorFactsV1>& source_facts,
    std::int32_t range, HottySourceTargetListV1& output,
    std::string& error) {
    output = {};
    if (!actor_order.complete || actor_order.actor_ids.size() > 65536 ||
        source_facts.size() > 65536 || range <= 0)
        return fail(error, "Hotty requires a complete active ActorPopulation order and valid 600/800 range");
    const auto* caster = world.find_actor(caster_id);
    const auto* caster_properties = world.combat_properties(caster_id);
    if (!caster || !caster_properties || !world.traits(caster_id) ||
        !world.traits(caster_id)->is_player)
        return fail(error, "Hotty target query requires the same-world player and source property sheet");
    const float owner_radius = world.target_radius(*caster);
    if (!std::isfinite(owner_radius) || owner_radius < 0.0f)
        return fail(error, "Hotty source owner interaction radius is unavailable");

    std::map<ActorId, const HottySourceActorFactsV1*> facts_by_actor;
    for (const auto& facts : source_facts) {
        if (facts.actor == invalid_actor_id ||
            !facts_by_actor.emplace(facts.actor, &facts).second)
            return fail(error, "Hotty source visibility/zone facts contain invalid or duplicate ActorId");
    }
    std::set<ActorId> seen;
    std::vector<ActorId> targets;
    targets.reserve(actor_order.actor_ids.size());
    for (ActorId id : actor_order.actor_ids) {
        if (id == invalid_actor_id || !seen.insert(id).second)
            return fail(error, "Hotty active ActorPopulation order contains invalid/duplicate ActorId");
        if (id == caster_id) continue; // Source TargetList excludes its owner.
        const auto* candidate = world.find_actor(id);
        if (!candidate)
            return fail(error, "Hotty active ActorPopulation actor is absent from the same world");
        const auto fact = facts_by_actor.find(id);
        if (fact == facts_by_actor.end())
            return fail(error, "Reached Hotty source visibility/zone/interaction facts are missing");
        const auto& f = *fact->second;
        if (!f.visible)
            return fail(error, "Reached Hotty source GameObject visible fact is unknown");
        if (!*f.visible) continue;
        if (!f.zonable)
            return fail(error, "Reached Hotty source IsZonable result is unknown");
        if (*f.zonable) {
            if (!f.zoned)
                return fail(error, "Reached Hotty source zoned field is unknown");
            if (*f.zoned) {
                if (!f.in_zone)
                    return fail(error, "Reached Hotty source in-zone field is unknown");
                if (!*f.in_zone) continue;
            }
        }
        if (!f.interactive)
            return fail(error, "Reached Hotty source IsInteractive result is unknown");
        if (!*f.interactive) continue;

        const auto* candidate_properties = world.combat_properties(id);
        if (!candidate_properties)
            return fail(error, "Hotty candidate Character source property sheet is unavailable");
        // Character::F_IsCharacterValid compares owner word+1314 to candidate
        // word+1310 before the Enemy filter; shared target facts map to 199/198.
        if (caster_properties->sheets.resolved[199] <
            candidate_properties->sheets.resolved[198]) continue;
        if (!world.eligible_target(*caster, *candidate)) continue; // live, Enemy, targetable, non-player
        const auto* ai = dh2::data::ai_props(
            world.factions(), candidate_properties->sheets.resolved[1]);
        if (!ai)
            return fail(error, "Hotty candidate original AI interaction-radius row/fallback is unavailable");
        const float* center = source_target_position(*candidate).data();
        const float* origin = source_target_position(*caster).data();
        const float delta[3] = {source_sub(center[0], origin[0]),
                                source_sub(center[1], origin[1]),
                                source_sub(center[2], origin[2])};
        const float distance = source_sub(source_sub(source_length(delta), ai->interact_radius),
                                          owner_radius);
        if (!std::isfinite(distance))
            return fail(error, "Hotty source interaction-radius distance is non-finite");
        if (distance > float(range)) continue;
        targets.push_back(id); // NoSort: preserve authoritative host enrollment order.
    }
    if (seen.size() != world.actors().size())
        return fail(error, "Hotty authored ActorPopulation order is not complete for the current generic Character world");
    for (const auto& actor : world.actors())
        if (!seen.count(actor.first))
            return fail(error, "Hotty authored ActorPopulation order omitted a current generic Character actor");
    output.query_complete = true;
    output.authored_world_order_preserved = true;
    output.scope = HottyTargetScopeV1::current_character_population;
    output.character_targets_in_source_order = std::move(targets);
    error.clear();
    return true;
}

bool advance_hotty_cooldown_clock_v1(std::uint64_t serial,
                                     double update_dt_seconds,
                                     HottyCooldownClockV1& clock,
                                     std::string& error) {
    if (serial == 0 || !std::isfinite(update_dt_seconds) ||
        update_dt_seconds < 0.0 || update_dt_seconds > 3600.0) {
        return fail(error, "Hotty cooldown clock requires a live updated CombatSession and valid source dt");
    }
    if (clock.session_update_serial == 0) {
        if (serial != 1)
            return fail(error, "Hotty cooldown clock must observe the session from its first update");
    } else if (clock.session_update_serial == std::numeric_limits<std::uint64_t>::max() ||
               serial != clock.session_update_serial + 1) {
        return fail(error, "Hotty cooldown clock requires exactly one observation per session update serial");
    }
    const auto delta = static_cast<std::uint64_t>(update_dt_seconds * 1000.0);
    if (clock.elapsed_ms > std::numeric_limits<std::uint64_t>::max() - delta)
        return fail(error, "Hotty cooldown source clock overflow");
    clock.elapsed_ms += delta;
    clock.session_update_serial = serial;
    for (auto it = clock.spell_ready_at_ms.begin(); it != clock.spell_ready_at_ms.end();) {
        if (it->second <= clock.elapsed_ms) it = clock.spell_ready_at_ms.erase(it);
        else ++it;
    }
    error.clear();
    return true;
}

bool prepare_hotty_spell_v1(
    PlayableActorWorld& world, ActorId caster_id, std::uint64_t session_update_serial,
    CharacterState& state, std::int32_t difficulty,
    const dh2::data::FaeryTables::Borrow& tables,
    const dh2::data::ClassTables& classes, const dh2::data::PropertyRules& rules,
    const HottySourcePolicyV1& policy, HottyCooldownClockV1& clock,
    HottyPreparedCastV1& output, std::string& error,
    const HottySourceTargetListV1* exact_source_targets) {
    output = {};
    if (!policy.complete)
        return fail(error, "Hotty cast source online/controller/GOD_MANA policy facts are incomplete");
    if (session_update_serial == 0 ||
        session_update_serial != clock.session_update_serial)
        return fail(error, "Hotty cast must run against the current observed CombatSession update");

    ActorState* caster{};
    if (!source_player(world, caster_id, state, caster, error)) return false;
    std::int32_t slot{}, row_id{}, spell_type{}, skill_level{};
    if (!resolve_hotty_row(state, difficulty, tables, slot, row_id,
                           spell_type, skill_level, error)) return false;

    auto* live_properties = world.combat_properties(caster_id);
    const auto* traits = world.traits(caster_id);
    if (!live_properties || !traits || !traits->is_player)
        return fail(error, "Hotty cast requires the same live player's original property sheet and player trait");
    constexpr std::int32_t source_level_property = 19;
    constexpr std::int32_t source_mana_property = 41;
    if (rules.defaults[172] != 0 || rules.types[172] != 8 ||
        live_properties->sheets.resolved[source_level_property] < 0 ||
        live_properties->sheets.resolved[source_mana_property] < 0)
        return fail(error, "Hotty source Level/MP/SnS_Level properties are missing or invalid");
    if (clock.spell_ready_at_ms.count(caster_id))
        return fail(error, "Hotty source spell cooldown is still active");

    std::int32_t class_id{};
    dh2::data::PropertySheet spell_properties{};
    if (!project_spell_properties(classes, rules, live_properties->sheets.resolved,
                                 skill_level, class_id, spell_properties, error)) return false;

    // Literal recovered ToFixed/MulFixed behavior: ToFixed(0.2) truncates to
    // zero before <<8, so the source's nominal per-level term is exactly zero.
    std::int32_t computed_level{}, mana_cost{};
    if (!hotty_mana_cost_v1(state.faery_by_difficulty[std::size_t(difficulty)]
                                .faeries[std::size_t(slot)].level,
                            live_properties->sheets.resolved[source_level_property],
                            computed_level, mana_cost, error)) return false;
    if (computed_level != skill_level)
        return fail(error, "Hotty saved-rank projection changed during source resolution");
    const std::int32_t range = skill_level == 0 ? 600 : 800;

    std::vector<ActorId> targets;
    if (exact_source_targets) {
        if (!valid_source_character_list(world, caster_id, *exact_source_targets,
                                         range, targets, error)) return false;
    } else if (!source_hotty_character_targets_v1(world, caster_id, range, targets, error)) {
        return false;
    }
    const bool controller_bypass = policy.source_online && policy.has_controller;
    const bool has_mana = controller_bypass ||
        mana_cost <= live_properties->sheets.resolved[source_mana_property];
    if (!has_mana)
        return fail(error, "Original Hotty OnSkillCheck rejected insufficient same-player MP");

    auto next_properties = *live_properties;
    auto next_character_resource = state.stats.resource;
    const bool god_mana = policy.saved_god_mana || policy.debug_god_mana ||
                          policy.character_god_mana;
    const bool mana_bypassed = controller_bypass || god_mana;
    if (!mana_bypassed) {
        const auto old_mp = live_properties->sheets.resolved[source_mana_property];
        const float expected_resource = float(old_mp) * (1.0f / 256.0f);
        constexpr float sync_tolerance = 1.0f / 256.0f;
        if (std::fabs(caster->resource - expected_resource) > sync_tolerance ||
            std::fabs(state.stats.resource - expected_resource) > sync_tolerance)
            return fail(error, "Hotty UseMana requires synchronized same-world ActorState, CharacterState and MP property");
        // Original Character::UseMana calls CharProperties::PROPS_Add(41, -cost).
        // MP is a saved-based (type 32) property: a direct resolved[] write is
        // recomputed away by the next resolve, leaving the source sheet stale.
        auto mana_view = dh2::data::property_view(rules, next_properties.sheets);
        if (dh2_property_add(&mana_view, source_mana_property,
                static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(mana_cost))) != 0 ||
            next_properties.sheets.resolved[source_mana_property] != old_mp - mana_cost)
            return fail(error, "Hotty UseMana PropertyAdd did not debit the source MP property");
        next_character_resource = expected_resource - float(mana_cost) * (1.0f / 256.0f);
    }

    const auto ready_at = clock.elapsed_ms + 5000u;
    if (ready_at < clock.elapsed_ms)
        return fail(error, "Hotty source 5000 ms cooldown overflow");
    // Commit the same actor property owner first. Remaining writes are infallible
    // value assignments, so no source mutation is published on an update failure.
    if (!mana_bypassed) {
        if (!world.update_combat_properties(caster_id, next_properties, *traits, error)) {
            if (error.empty()) error = "Same-world Hotty UseMana property update failed";
            return false;
        }
        caster->resource = next_character_resource;
        state.stats.resource = next_character_resource;
    }
    clock.spell_ready_at_ms[caster_id] = ready_at;

    HottyPreparedCastV1 result{};
    result.status = targets.empty()
        ? HottyPrepareStatusV1::prepared_no_targets_pending_spell_combat
        : HottyPrepareStatusV1::prepared_pending_spell_combat;
    result.caster = caster_id;
    result.difficulty = difficulty;
    result.faery_slot = slot;
    result.faery_record_id = row_id;
    result.source_spell_type = spell_type;
    result.skill_level = skill_level;
    result.fixed_mana_cost = mana_cost;
    result.range = range;
    result.character_targets = std::move(targets);
    result.session_update_serial = session_update_serial;
    result.cooldown_ready_at_ms = ready_at;
    result.spell_class_id = class_id + skill_level;
    result.spell_properties = spell_properties;
    result.mana_debited = !mana_bypassed;
    result.original_target_order_known = exact_source_targets != nullptr;
    output = std::move(result);
    error.clear();
    return true;
}

} // namespace dh::foundation::faery_menu
