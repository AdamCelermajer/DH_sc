#include "hotty_effects_v1.hpp"

#include <algorithm>

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

bool valid_prepared(const CombatSession& session,
                    const HottyPreparedCastV1& prepared,
                    const CharacterState& character,
                    const HottyCooldownClockV1& clock,
                    std::string& error) {
    const auto lease = session.actor_binding_lease();
    if (lease.expired() || !clock.has_binding_lease || clock.binding_lease.expired() ||
        !same_owner(lease, clock.binding_lease))
        return fail(error, "Hotty FX session and cooldown lease are not the same live owner");
    if (prepared.status != HottyPrepareStatusV1::prepared_pending_spell_combat &&
        prepared.status != HottyPrepareStatusV1::prepared_no_targets_pending_spell_combat)
        return fail(error, "Hotty FX requires a committed source preparation");
    if (prepared.caster == invalid_actor_id || prepared.caster != session.player_id())
        return fail(error, "Hotty FX must use the same CombatSession player that prepared the spell");
    if (prepared.session_update_serial == 0 ||
        prepared.session_update_serial > session.update_serial() ||
        clock.session_update_serial != session.update_serial())
        return fail(error, "Hotty FX cooldown clock is not the current session update after preparation");
    if (prepared.cooldown_ready_at_ms <= clock.elapsed_ms)
        return fail(error, "Hotty FX preparation has no active source cooldown");
    const auto cooldown = clock.spell_ready_at_ms.find(prepared.caster);
    if (cooldown == clock.spell_ready_at_ms.end() ||
        cooldown->second != prepared.cooldown_ready_at_ms)
        return fail(error, "Hotty FX preparation does not own the current same-player source cooldown");
    const auto* actor = session.actor(prepared.caster);
    if (!actor || !actor->persistent_character_id ||
        *actor->persistent_character_id != character.id)
        return fail(error, "Hotty FX requires the same live player ActorState and CharacterState identity");
    if (prepared.faery_slot != 4 || prepared.faery_record_id != 7 ||
        prepared.source_spell_type < 0 || prepared.skill_level < 0 ||
        prepared.skill_level > 1 || prepared.range != (prepared.skill_level ? 800 : 600))
        return fail(error, "Hotty FX preparation no longer matches the source Hotty slot/rank contract");
    error.clear();
    return true;
}
}

bool dispatch_hotty_effect_v1(
    CombatSession& session, effects::RuntimeEffectsFactoryV1& factory,
    const dh2::data::EffectsTables::Borrow& tables,
    const HottyEffectIdsV1& ids, const HottyPreparedCastV1& prepared,
    const CharacterState& character, const HottyCooldownClockV1& clock,
    ActorId scene_actor, ActorId effect_actor, HottyEffectKindV1 kind,
    bool target_count_known, std::uint32_t target_count,
    bool apply_result_known, bool source_return_positive,
    HottyEffectStatusV1& status, std::string& error) {
    status = HottyEffectStatusV1::source_branch_skipped;
    if (session.actor_binding_lease().expired())
        return fail(error, "Hotty source FX rejected an expired CombatSession lease");
    if (!valid_prepared(session, prepared, character, clock, error)) return false;
    if (!tables || ids.player_pre < 0 || ids.character_target < 0 ||
        static_cast<std::size_t>(ids.player_pre) >= tables.set_names().size() ||
        static_cast<std::size_t>(ids.character_target) >= tables.set_names().size() ||
        tables.set_names()[static_cast<std::size_t>(ids.player_pre)] != "Hotty_Level_1_Player_Pre" ||
        tables.set_names()[static_cast<std::size_t>(ids.character_target)] != "Hotty_Level_1_Target")
        return fail(error, "Hotty FX IDs do not resolve to the exact same source EffectsTables names");

    std::int32_t set_id = -1;
    ActorId anchor_actor = 0;
    bool should_dispatch = false;
    switch (kind) {
    case HottyEffectKindV1::player_pre:
        if (!target_count_known)
            return fail(error, "Hotty player-pre FX requires a complete original target-list count");
        if (!target_count) { error.clear(); return true; }
        if (target_count != prepared.character_targets.size())
            return fail(error, "Hotty player-pre FX cannot use an incomplete/non-character target list");
        set_id = ids.player_pre;
        anchor_actor = prepared.caster;
        should_dispatch = true;
        break;
    case HottyEffectKindV1::positive_character_target:
        if (!apply_result_known)
            return fail(error, "Hotty target FX requires the completed source F_ApplyResult return value");
        if (!source_return_positive) { error.clear(); return true; }
        if (std::find(prepared.character_targets.begin(), prepared.character_targets.end(),
                      effect_actor) == prepared.character_targets.end())
            return fail(error, "Hotty target FX ActorId is not in the prepared same-world character targets");
        set_id = ids.character_target;
        anchor_actor = effect_actor;
        should_dispatch = true;
        break;
    default:
        return fail(error, "Unknown Hotty source FX branch");
    }

    // A skipped source branch never mutates FX state and does not depend on a
    // renderer owner. Actual PlayFX calls require exact live host+Scene identity.
    if (!should_dispatch) {
        error.clear();
        return true;
    }
    if (scene_actor != prepared.caster)
        return fail(error, "Hotty FX factory Scene must belong to the same prepared player");
    // Check exact host and retained Scene identity before synchronizing or
    // mutating the FX manager. Matching ActorIds alone is insufficient.
    if (!factory.is_bound_to(session))
        return fail(error, "Hotty source FX factory is foreign, detached, rebound, or expired for this CombatSession");
    if (!factory.runtime().synchronize_session_binding(error)) return false;

    const auto* actor = session.actor(anchor_actor);
    if (!actor)
        return fail(error, "Hotty source FX anchor is missing from the same CombatSession");
    const float position[3] = {actor->transform.position[0],
                                actor->transform.position[1],
                                actor->transform.position[2]};
    std::uintptr_t created{};
    if (!factory.manager().play_set(set_id, position, nullptr, 0, &created, error))
        return false;
    // play_set creates or pools the actual authored source set. Rendering and
    // packet submission still require the normal RuntimeEffects update/frame path.
    status = HottyEffectStatusV1::source_instance_created;
    error.clear();
    return true;
}

bool dispatch_hotty_player_pre_v1(
    CombatSession& session, effects::RuntimeEffectsFactoryV1& factory,
    const dh2::data::EffectsTables::Borrow& tables,
    const HottyEffectIdsV1& ids, const HottyPreparedCastV1& prepared,
    const CharacterState& character, const HottyCooldownClockV1& clock,
    HottyEffectStatusV1& status, std::string& error) {
    return dispatch_hotty_effect_v1(
        session, factory, tables, ids, prepared, character, clock,
        prepared.caster, prepared.caster, HottyEffectKindV1::player_pre,
        true, static_cast<std::uint32_t>(prepared.character_targets.size()),
        false, false, status, error);
}

bool HottyRuntimeEffectsDispatchV1::dispatch_player_pre(
    CombatSession& session, const HottyPreparedCastV1& prepared,
    const CharacterState& character, const HottyCooldownClockV1& clock,
    HottyEffectStatusV1& status, std::string& error) {
    return dispatch_hotty_player_pre_v1(
        session, factory_, tables_, ids_, prepared, character, clock, status, error);
}

bool HottyRuntimeEffectsDispatchV1::dispatch_positive_target(
    CombatSession& session, const HottyPreparedCastV1& prepared,
    const CharacterState& character, const HottyCooldownClockV1& clock,
    ActorId target, HottyEffectStatusV1& status, std::string& error) {
    return dispatch_hotty_effect_v1(
        session, factory_, tables_, ids_, prepared, character, clock,
        prepared.caster, target, HottyEffectKindV1::positive_character_target,
        false, 0, true, true, status, error);
}

} // namespace dh::foundation::faery_menu
