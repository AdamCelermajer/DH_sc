#include "celest_cast_v1.hpp"

#include "../../combat_session.hpp"

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
    return std::sqrt(source_add(source_add(source_mul(v[0], v[0]),
                                           source_mul(v[1], v[1])),
                                source_mul(v[2], v[2])));
}

bool same_owner(const std::weak_ptr<const void>& a,
                const std::weak_ptr<const void>& b) noexcept {
    return !a.owner_before(b) && !b.owner_before(a);
}

bool resolve_celest_row(const CharacterState& state, std::int32_t difficulty,
                        const dh2::data::FaeryTables::Borrow& tables,
                        std::int32_t& slot, std::int32_t& row_id,
                        std::int32_t& spell_type, std::int32_t& level,
                        std::string& error) {
    if (!state.source_faery_state_known)
        return fail(error, "Celest requires source-known Faery Save selection/progress");
    if (!tables || difficulty < 0 || difficulty >= 3 ||
        state.source_faery_list_id < 0 ||
        static_cast<std::size_t>(state.source_faery_list_id) >= tables.lists().size() ||
        static_cast<std::size_t>(state.source_faery_list_id) >= tables.list_names().size())
        return fail(error, "Celest requires the actual same-profile FaeryList and difficulty");
    const auto& save = state.faery_by_difficulty[static_cast<std::size_t>(difficulty)];
    slot = save.current_faery;
    const auto& list = tables.lists()[static_cast<std::size_t>(state.source_faery_list_id)];
    if (list.size() != 5 || slot < 0 || slot >= 5)
        return fail(error, "Celest current Save slot/list is outside the original five slots");
    row_id = list[static_cast<std::size_t>(slot)];
    if (row_id < 0 || static_cast<std::size_t>(row_id) >= tables.faeries().size() ||
        static_cast<std::size_t>(row_id) >= tables.faery_names().size())
        return fail(error, "Celest current FaeryList references an invalid source row");
    const auto& row = tables.faeries()[static_cast<std::size_t>(row_id)];
    if (tables.faery_names()[static_cast<std::size_t>(row_id)] != "Celest" ||
        row.script != "faerie_celest" || row.scalar.words[8] != static_cast<std::uint32_t>(slot))
        return fail(error, "Current source Faery is not the exact Celest script row; no fallback is permitted");
    spell_type = static_cast<std::int32_t>(row.scalar.words[7]);
    level = std::min<std::int32_t>(save.faeries[static_cast<std::size_t>(slot)].level, 1);
    error.clear();
    return true;
}

bool validate_targets(const PlayableActorWorld& world, ActorId caster_id,
                      const HottySourceTargetListV1& source, std::int32_t range,
                      std::vector<ActorId>& ordered, std::string& error) {
    if (!source.query_complete || !source.authored_world_order_preserved ||
        source.scope != HottyTargetScopeV1::current_character_population)
        return fail(error, "Celest requires a complete current Character NoSort query in ActorPopulation order");
    const auto* caster = world.find_actor(caster_id);
    const auto* caster_properties = world.combat_properties(caster_id);
    if (!caster || !caster_properties || range <= 0)
        return fail(error, "Celest target query requires the same live player and authored range");
    std::vector<ActorId> next;
    std::set<ActorId> seen;
    next.reserve(source.character_targets_in_source_order.size());
    for (ActorId id : source.character_targets_in_source_order) {
        if (id == invalid_actor_id || id == caster_id || !seen.insert(id).second)
            return fail(error, "Celest NoSort target list contains an invalid/duplicate Character");
        const auto* target = world.find_actor(id);
        if (!target || !target->alive() || !world.eligible_target(*caster, *target))
            return fail(error, "Celest source target list contains an ineligible Character");
        const auto* target_properties = world.combat_properties(id);
        if (!target_properties || caster_properties->sheets.resolved[199] <
                                  target_properties->sheets.resolved[198])
            return fail(error, "Celest source target list contains an invalid/stealth-gated Character");
        const auto* target_ai = dh2::data::ai_props(
            world.factions(), target_properties->sheets.resolved[1]);
        if (!target_ai)
            return fail(error, "Celest source target list lacks the original interaction-radius AI row");
        const float* center = target->source_target_position184
            ? target->source_target_position184->data() : target->transform.position.data();
        const float* origin = caster->source_target_position184
            ? caster->source_target_position184->data() : caster->transform.position.data();
        const float delta[3] = {source_sub(center[0], origin[0]),
                                source_sub(center[1], origin[1]),
                                source_sub(center[2], origin[2])};
        const float distance = source_sub(
            source_sub(source_length(delta), target_ai->interact_radius),
            world.target_radius(*caster));
        if (!std::isfinite(distance) || distance > float(range))
            return fail(error, "Celest source Character target lies outside the authored 600/800 range");
        next.push_back(id);
    }
    ordered = std::move(next);
    error.clear();
    return true;
}

bool project_spell_properties(const dh2::data::ClassTables& classes,
                              const dh2::data::PropertyRules& rules,
                              const dh2::data::PropertySheet& live,
                              std::int32_t skill_level, std::int32_t& class_id,
                              dh2::data::PropertySheet& output,
                              std::string& error) {
    if (classes.names.size() != classes.rows.size() ||
        rules.defaults[172] != 0 || rules.types[172] != 8)
        return fail(error, "Celest requires actual ClassTables and the source SnS_Level property schema");
    const auto base = std::find(classes.names.begin(), classes.names.end(), "Spell_Lightning_1");
    if (base == classes.names.end())
        return fail(error, "Original Spell_Lightning_1 class row is unavailable");
    class_id = static_cast<std::int32_t>(base - classes.names.begin());
    const auto expected = class_id + skill_level;
    const auto expected_name = skill_level == 0 ? "Spell_Lightning_1" : "Spell_Lightning_2";
    if (expected < 0 || static_cast<std::size_t>(expected) >= classes.names.size() ||
        classes.names[static_cast<std::size_t>(expected)] != expected_name)
        return fail(error, "Original Celest Spell_Lightning_1 + rank class rows are not contiguous");
    output = live;
    output[172] = std::int32_t(std::uint32_t(skill_level) << 8);
    if (!dh2::data::apply_class(classes, expected, output, error, &live)) {
        if (error.empty()) error = "Original Celest ApplyPropClass projection failed";
        return false;
    }
    error.clear();
    return true;
}
}

bool prepare_celest_spell_v1(
    CombatSession& session, CharacterState& state, std::int32_t difficulty,
    const dh2::data::FaeryTables::Borrow& tables,
    const dh2::data::ClassTables& classes, const dh2::data::PropertyRules& rules,
    const HottySourcePolicyV1& policy, HottyCooldownClockV1& clock,
    CelestPreparedCastV1& output, std::string& error,
    const HottySourceTargetListV1* exact_source_targets) {
    output = {};
    if (!policy.complete)
        return fail(error, "Celest source online/controller/GOD_MANA policy is incomplete");
    if (session.actor_binding_lease().expired() || !session.world() ||
        session.update_serial() == 0 || clock.session_update_serial != session.update_serial())
        return fail(error, "Celest requires a live CombatSession and current observed cooldown update");
    if (!clock.has_binding_lease || clock.binding_lease.expired() ||
        !same_owner(clock.binding_lease, session.actor_binding_lease()))
        return fail(error, "Celest cooldown clock belongs to a different or expired CombatSession");
    const ActorId caster_id = session.player_id();
    auto* caster = session.world()->find_actor(caster_id);
    if (caster_id == invalid_actor_id || !caster || !caster->persistent_character_id ||
        *caster->persistent_character_id != state.id)
        return fail(error, "Celest requires the same live Session player ActorState and CharacterState identity");

    std::int32_t slot{}, row_id{}, spell_type{}, level{};
    if (!resolve_celest_row(state, difficulty, tables, slot, row_id,
                            spell_type, level, error)) return false;
    auto* live = session.world()->combat_properties(caster_id);
    const auto* traits = session.world()->traits(caster_id);
    if (!live || !traits || !traits->is_player)
        return fail(error, "Celest requires the same player's live property sheet and player trait");
    constexpr std::int32_t source_level_property = 19;
    constexpr std::int32_t source_mana_property = 41;
    if (rules.defaults[172] != 0 || rules.types[172] != 8 ||
        live->sheets.resolved[source_level_property] < 0 ||
        live->sheets.resolved[source_mana_property] < 0)
        return fail(error, "Celest source Level/MP/SnS_Level properties are missing or invalid");
    if (clock.spell_ready_at_ms.count(caster_id))
        return fail(error, "Original Celest HasSpellCooldown check rejected this cast");

    std::int32_t class_id{};
    dh2::data::PropertySheet spell_properties{};
    if (!project_spell_properties(classes, rules, live->sheets.resolved,
                                 level, class_id, spell_properties, error)) return false;

    // Source Lua spells ToFixed(0.2) as a number, truncates it to integer 0,
    // then shifts to fixed point. The per-player-level slope is therefore 0.
    constexpr std::int32_t source_level_slope = 0;
    const auto slope = source_mul_fixed(source_level_slope,
                                        live->sheets.resolved[source_level_property]);
    const auto base = std::int32_t(std::uint32_t(level == 0 ? 10 : 20) << 8);
    const auto mana_cost = wrap_word(std::uint32_t(base) + std::uint32_t(slope));
    const auto range = level == 0 ? 600 : 800;

    std::vector<ActorId> targets;
    if (!exact_source_targets)
        return fail(error, "Celest preparation requires the retained complete source target query");
    if (!validate_targets(*session.world(), caster_id, *exact_source_targets,
                          range, targets, error)) return false;

    const bool controller_bypass = policy.source_online && policy.has_controller;
    if (!controller_bypass && mana_cost > live->sheets.resolved[source_mana_property])
        return fail(error, "Original Celest HasMana check rejected insufficient same-player MP");

    const bool mana_bypassed = controller_bypass || policy.saved_god_mana ||
                               policy.debug_god_mana || policy.character_god_mana;
    auto next_properties = *live;
    auto next_resource = state.stats.resource;
    if (!mana_bypassed) {
        const auto old_mp = live->sheets.resolved[source_mana_property];
        const float expected_resource = float(old_mp) * (1.0f / 256.0f);
        constexpr float sync_tolerance = 1.0f / 256.0f;
        if (std::fabs(caster->resource - expected_resource) > sync_tolerance ||
            std::fabs(state.stats.resource - expected_resource) > sync_tolerance)
            return fail(error, "Celest UseMana requires synchronized same-world ActorState, CharacterState and MP");
        // Original Character::UseMana calls CharProperties::PROPS_Add(41, -cost).
        // MP is a saved-based (type 32) property: a direct resolved[] write is
        // recomputed away by the next resolve, leaving the source sheet stale.
        auto mana_view = dh2::data::property_view(rules, next_properties.sheets);
        if (dh2_property_add(&mana_view, source_mana_property,
                static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(mana_cost))) != 0 ||
            next_properties.sheets.resolved[source_mana_property] != old_mp - mana_cost)
            return fail(error, "Celest UseMana PropertyAdd did not debit the source MP property");
        next_resource = expected_resource - float(mana_cost) * (1.0f / 256.0f);
    }
    if (clock.elapsed_ms > std::numeric_limits<std::uint64_t>::max() - 5000u)
        return fail(error, "Celest 5000 ms cooldown overflow");
    const auto ready_at = clock.elapsed_ms + 5000u;

    // Source OnPre order is TargetListSearch -> UseMana -> SetSpellCooldown.
    // Commit MP before the cooldown, after all target/policy validation.
    if (!mana_bypassed) {
        if (!session.world()->update_combat_properties(caster_id, next_properties,
                                                        *traits, error)) {
            if (error.empty()) error = "Same-world Celest UseMana property update failed";
            return false;
        }
        caster->resource = next_resource;
        state.stats.resource = next_resource;
    }
    clock.spell_ready_at_ms[caster_id] = ready_at;

    CelestPreparedCastV1 result{};
    result.status = targets.empty()
        ? CelestPrepareStatusV1::prepared_no_targets_pending_spell_combat
        : CelestPrepareStatusV1::prepared_pending_spell_combat;
    result.caster = caster_id;
    result.difficulty = difficulty;
    result.faery_slot = slot;
    result.faery_record_id = row_id;
    result.source_spell_type = spell_type;
    result.skill_level = level;
    result.fixed_mana_cost = mana_cost;
    result.range = range;
    result.character_targets = std::move(targets);
    result.session_update_serial = session.update_serial();
    result.cooldown_ready_at_ms = ready_at;
    result.spell_class_id = class_id + level;
    result.spell_properties = spell_properties;
    result.mana_debited = !mana_bypassed;
    result.original_target_order_known = true;
    output = std::move(result);
    error.clear();
    return true;
}

} // namespace dh::foundation::faery_menu
