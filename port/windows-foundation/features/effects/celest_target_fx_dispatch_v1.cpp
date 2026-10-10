#include "celest_target_fx_dispatch_v1.hpp"

#include <algorithm>
#include <limits>

namespace dh::foundation::effects {
namespace {
constexpr const char* kPlayerName = "Celest_Level_1_Player";
constexpr const char* kMainName = "Celest_Level_1_Main";

bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

bool same_owner(const std::weak_ptr<const void>& left,
                const std::weak_ptr<const void>& right) noexcept {
    return !left.owner_before(right) && !right.owner_before(left);
}

bool find_set(const dh2::data::EffectsTables::Borrow& tables,
              const char* name, std::int32_t& id, std::string& error) {
    if (!tables) return fail(error, "Celest FX requires the original EffectsTables borrow");
    const auto& names = tables.set_names();
    const auto& sets = tables.sets();
    if (names.size() != sets.size())
        return fail(error, "Celest FX source set names and rows differ");
    const auto found = std::find(names.begin(), names.end(), name);
    if (found == names.end()) {
        error = std::string("Required original Celest AnimatedEffectTable row is missing: ") + name;
        return false;
    }
    const auto index = static_cast<std::size_t>(found - names.begin());
    if (index > static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max()) ||
        sets[index].steps.empty()) {
        error = std::string("Original Celest FX row has no playable source steps: ") + name;
        return false;
    }
    id = static_cast<std::int32_t>(index);
    error.clear();
    return true;
}

bool validate_prepared(const CombatSession& session,
                       const faery_menu::CelestPreparedCastV1& prepared,
                       const CharacterState& character,
                       const faery_menu::HottyCooldownClockV1& clock,
                       std::string& error) {
    const auto lease = session.actor_binding_lease();
    if (lease.expired() || !clock.has_binding_lease || clock.binding_lease.expired() ||
        !same_owner(lease, clock.binding_lease))
        return fail(error, "Celest FX cast and cooldown clock do not share one live CombatSession owner");
    if (prepared.status != faery_menu::CelestPrepareStatusV1::prepared_pending_spell_combat &&
        prepared.status != faery_menu::CelestPrepareStatusV1::prepared_no_targets_pending_spell_combat)
        return fail(error, "Celest FX requires a committed source OnPre prefix");
    if (prepared.caster == invalid_actor_id || prepared.caster != session.player_id())
        return fail(error, "Celest FX must use the same CombatSession player that prepared the cast");
    if (!prepared.mana_debited || prepared.session_update_serial == 0 ||
        prepared.session_update_serial > session.update_serial() ||
        clock.session_update_serial != session.update_serial() ||
        prepared.cooldown_ready_at_ms <= clock.elapsed_ms)
        return fail(error, "Celest FX cooldown clock is not current after source preparation");
    const auto ready = clock.spell_ready_at_ms.find(prepared.caster);
    if (ready == clock.spell_ready_at_ms.end() ||
        ready->second != prepared.cooldown_ready_at_ms)
        return fail(error, "Celest FX preparation no longer owns the same-player source cooldown");
    const auto* actor = session.actor(prepared.caster);
    if (!actor || !actor->persistent_character_id ||
        *actor->persistent_character_id != character.id)
        return fail(error, "Celest FX CharacterState is not the same CombatSession player owner");
    error.clear();
    return true;
}
}

bool resolve_celest_target_fx_ids_v1(
    const dh2::data::EffectsTables::Borrow& tables,
    CelestTargetFxIdsV1& output, std::string& error) {
    output = {};
    CelestTargetFxIdsV1 next;
    if (!find_set(tables, kPlayerName, next.player_pre, error) ||
        !find_set(tables, kMainName, next.target_main, error))
        return false;
    output = next;
    error.clear();
    return true;
}

bool CelestTargetFxDispatchV1::dispatch_source_set(
    CombatSession& session, const faery_menu::CelestPreparedCastV1& prepared,
    const CharacterState& character, const faery_menu::HottyCooldownClockV1& clock,
    ActorId anchor, const char* expected_name, std::int32_t set_id,
    faery_menu::CelestEffectStatusV1& status, std::string& error) {
    status = faery_menu::CelestEffectStatusV1::dispatch_failed;
    if (!validate_prepared(session, prepared, character, clock, error)) return false;
    if (!tables_ || ids_.player_pre < 0 || ids_.target_main < 0 || set_id < 0 ||
        static_cast<std::size_t>(ids_.player_pre) >= tables_.set_names().size() ||
        static_cast<std::size_t>(ids_.target_main) >= tables_.set_names().size() ||
        static_cast<std::size_t>(set_id) >= tables_.set_names().size() ||
        tables_.set_names()[static_cast<std::size_t>(ids_.player_pre)] != kPlayerName ||
        tables_.set_names()[static_cast<std::size_t>(ids_.target_main)] != kMainName ||
        tables_.set_names()[static_cast<std::size_t>(set_id)] != expected_name)
        return fail(error, "Celest FX IDs do not resolve to the exact original AnimatedEffectTable names");
    if (!factory_.is_bound_to(session))
        return fail(error, "Celest FX factory is foreign, detached, rebound, or expired for this CombatSession");
    if (anchor == invalid_actor_id || !session.actor(anchor))
        return fail(error, "Celest FX anchor is absent from the same CombatSession");
    if (!factory_.runtime().synchronize_session_binding(error)) return false;

    const auto* actor = session.actor(anchor);
    const float position[3] = {actor->transform.position[0],
                               actor->transform.position[1],
                               actor->transform.position[2]};
    std::uintptr_t created{};
    if (!factory_.manager().play_set(set_id, position, nullptr, 0, &created, error))
        return false;
    if (!created) return fail(error, "Original Celest PlayFX did not create a source instance");
    status = faery_menu::CelestEffectStatusV1::source_instance_created;
    error.clear();
    return true;
}

bool CelestTargetFxDispatchV1::dispatch_player_pre(
    CombatSession& session, const faery_menu::CelestPreparedCastV1& prepared,
    const CharacterState& character, const faery_menu::HottyCooldownClockV1& clock,
    faery_menu::CelestEffectStatusV1& status, std::string& error) {
    return dispatch_source_set(session, prepared, character, clock, prepared.caster,
                               kPlayerName, ids_.player_pre, status, error);
}

bool CelestTargetFxDispatchV1::dispatch_target_main(
    CombatSession& session, const faery_menu::CelestPreparedCastV1& prepared,
    const CharacterState& character, const faery_menu::HottyCooldownClockV1& clock,
    ActorId target, faery_menu::CelestEffectStatusV1& status,
    std::string& error) {
    if (std::find(prepared.character_targets.begin(), prepared.character_targets.end(), target) ==
        prepared.character_targets.end()) {
        status = faery_menu::CelestEffectStatusV1::dispatch_failed;
        return fail(error, "Celest target FX ActorId is not in the prepared source Character target order");
    }
    return dispatch_source_set(session, prepared, character, clock, target,
                               kMainName, ids_.target_main, status, error);
}

} // namespace dh::foundation::effects
