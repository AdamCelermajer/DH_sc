#include "runtime_session_death_rewards_v1.hpp"

#include "../../combat_session.hpp"
#include "../../playable_actor_world.hpp"

#include <algorithm>

namespace dh::foundation::loot {
namespace {
bool fail(std::string& error, const char* text) {
    error = text;
    return false;
}
bool same_weak_owner(const std::weak_ptr<const void>& left,
                     const std::weak_ptr<const void>& right) noexcept {
    return !left.owner_before(right) && !right.owner_before(left);
}
}

bool RuntimeSessionDeathRewardsV1::bind(
    dh::foundation::CombatSession& session,
    const frontend::creation::RuntimeCreationSourceOwnerV1& creation,
    const AssetCatalog& assets,
    std::shared_ptr<RuntimeWorldItemAdapterV1> world_items,
    RuntimeSessionDeathRewardBindingsV1 bindings,
    std::string& error) {
    error.clear();
    if (session_ || dispatching_) return fail(error, "Reset the existing session death-reward binding before rebinding");
    auto* world = session.world();
    const auto session_lease = session.actor_binding_lease();
    if (!world || session_lease.expired())
        return fail(error, "Death-reward bind requires an initialized, attached CombatSession");
    if (!creation.valid() || !creation.properties || !creation.loot_owner || !world_items ||
        !bindings.gameplay_context_lease || !bindings.context || !bindings.read_reward_admission ||
        !bindings.read_difficulty ||
        !bindings.source_loot_entry || !bindings.query_debug_switch || !bindings.resolve_character)
        return fail(error, "Death-reward bind is missing same-gameplay source, context, or store owners");

    RuntimeLootSourceOwnerV1 next_owner;
    if (!next_owner.load(assets, creation, error)) return false;
    auto source = next_owner.borrow();
    const auto exact_loot = creation.loot_owner->borrow();
    if (!source.valid() || source.properties.get() != creation.properties.get() ||
        !world_items->uses_loot_snapshot(exact_loot))
        return fail(error, "Death-reward source, creation owner, and existing world-item store are not the same source snapshot");

    loot_source_owner_ = std::move(next_owner);
    loot_source_ = std::move(source);
    world_items_ = std::move(world_items);
    gameplay_context_lease_ = std::move(bindings.gameplay_context_lease);
    bindings_ = std::move(bindings);
    creation_owner_ = &creation;
    session_ = &session;
    world_ = world;
    session_binding_lease_ = session_lease;
    error.clear();
    return true;
}

bool RuntimeSessionDeathRewardsV1::loot_entry_thunk(
    void* raw, const dh2::data::LootEntryRequestV8& request,
    std::int32_t& value, std::string& error) {
    auto* self = static_cast<RuntimeSessionDeathRewardsV1*>(raw);
    if (!self || !self->world_ || !self->loot_source_.properties)
        return fail(error, "Same-session source Loot entry context expired");
    switch (request.operation) {
    case dh2::data::LootEntryOperationV8::warrior_count:
    case dh2::data::LootEntryOperationV8::mage_count:
    case dh2::data::LootEntryOperationV8::rogue_count:
        return source_player_class_count_v1(*self->world_, *self->loot_source_.properties,
                                            request.operation, value, error);
    case dh2::data::LootEntryOperationV8::debug_load:
    case dh2::data::LootEntryOperationV8::debug_query:
    case dh2::data::LootEntryOperationV8::assertion:
        if (!self->bindings_.source_loot_entry)
            return fail(error, "Actual source Debug/Assertion scope is unavailable");
        return self->bindings_.source_loot_entry(self->bindings_.context, request, value, error);
    }
    return fail(error, "Unknown source LootEntry operation");
}

bool RuntimeSessionDeathRewardsV1::one_kill_level_up_thunk(
    void* raw, bool& enabled, std::string& error) {
    auto* self = static_cast<RuntimeSessionDeathRewardsV1*>(raw);
    if (!self || !self->bindings_.query_debug_switch)
        return fail(error, "Actual source Debug OneKillLevelUp query is unavailable");
    return self->bindings_.query_debug_switch(self->bindings_.context,
                                               "OneKillLevelUp", enabled, error);
}

bool RuntimeSessionDeathRewardsV1::level_up_thunk(
    void* raw, ActorId id, std::int32_t level, std::string& error) {
    auto* self = static_cast<RuntimeSessionDeathRewardsV1*>(raw);
    if (!self) return fail(error, "Level-up presentation owner is unavailable");
    if (!self->bindings_.level_up_presentation) return true;
    return self->bindings_.level_up_presentation(id, level, error);
}

bool RuntimeSessionDeathRewardsV1::resolve_character_thunk(
    void* raw, ActorId id, RuntimeDeathActorV1& output, std::string& error) {
    auto* self = static_cast<RuntimeSessionDeathRewardsV1*>(raw);
    if (!self || !self->bindings_.resolve_character)
        return fail(error, "Canonical same-session CharacterState resolver is unavailable");
    return self->bindings_.resolve_character(self->bindings_.context, id, output, error);
}

bool RuntimeSessionDeathRewardsV1::spawn_world_item_thunk(
    void* raw, const RuntimeWorldItemRecordV1& record,
    const ActorState& victim, const ActorState* killer, std::string& error) {
    auto* self = static_cast<RuntimeSessionDeathRewardsV1*>(raw);
    if (!self || !self->world_items_)
        return fail(error, "Same-gameplay WorldItemStore lease expired");
    RuntimeWorldItemIdV1 identity{};
    // P14: ItemObject::DropAndAwardLoot scatter over the same loot RNG.
    std::array<float, 3> killer_position{};
    if (killer) killer_position = killer->transform.position;
    return self->world_items_->publish_death_drop_scattered(
        record, victim, killer ? &killer_position : nullptr, self->scatter_rng_,
        identity, error);
}

bool RuntimeSessionDeathRewardsV1::after_update(
    dh::foundation::CombatSession& session,
    std::vector<RuntimeDeathRewardOutcomeV1>& outcomes,
    std::string& error) {
    outcomes.clear();
    error.clear();
    if (dispatching_) return fail(error, "Death-reward update is already dispatching");
    if (!session_ || &session != session_ || !world_ || session.world() != world_ ||
        !creation_owner_ || !creation_owner_->valid() ||
        creation_owner_->properties.get() != loot_source_.properties.get() ||
        !gameplay_context_lease_ || !world_items_ || !loot_source_.valid())
        return fail(error, "Death-reward binding does not belong to this live session/source/store");
    const auto current_lease = session.actor_binding_lease();
    if (current_lease.expired() || session_binding_lease_.expired() ||
        !same_weak_owner(current_lease, session_binding_lease_))
        return fail(error, "CombatSession actor binding lease changed; reset and bind after restore");
    if (!world_items_->uses_loot_snapshot(creation_owner_->loot_owner->borrow()))
        return fail(error, "Existing WorldItemStore no longer owns the bound source Loot snapshot");

    const auto has_death = std::any_of(session.events().begin(), session.events().end(),
        [](const dh::foundation::DamageEvent& event) { return event.applied && event.target_died; });
    if (!has_death) return true;

    RuntimeDeathRewardAdmissionV1 admission;
    if (!bindings_.read_reward_admission(bindings_.context, admission, error)) return false;

    std::int32_t current_difficulty = -1, unlocked_difficulty = -1;
    std::int32_t max_level{};
    if (!admission.rewards_suppressed) {
        if (!bindings_.read_difficulty(bindings_.context, current_difficulty,
                                       unlocked_difficulty, error)) return false;
        if (current_difficulty < 0 || unlocked_difficulty < 0)
            return fail(error, "Current/unlocked source difficulty is absent or negative");
        if (!loot_source_.max_level_for_difficulty(unlocked_difficulty, max_level, error)) return false;
        if (loot_source_.design_settings.rows().empty())
            return fail(error, "Original source DesignSettings table has no row for XP distribution");
    }

    RuntimeDeathRewardServicesV1 services;
    services.admission = admission;
    services.loot_tables = loot_source_.loot;
    services.loot_powers = loot_source_.power_resources;
    services.loot_entry = {this, &RuntimeSessionDeathRewardsV1::loot_entry_thunk};
    services.xp_design = loot_source_.design_settings.rows().empty()
        ? nullptr : &loot_source_.design_settings.rows().front();
    services.properties = loot_source_.properties.get();
    services.current_difficulty = current_difficulty;
    services.unlocked_difficulty = unlocked_difficulty;
    services.max_level = max_level;
    services.context = this;
    services.query_one_kill_level_up = &RuntimeSessionDeathRewardsV1::one_kill_level_up_thunk;
    services.resolve_character = &RuntimeSessionDeathRewardsV1::resolve_character_thunk;
    services.on_level_up = &RuntimeSessionDeathRewardsV1::level_up_thunk;
    services.spawn_world_item = &RuntimeSessionDeathRewardsV1::spawn_world_item_thunk;

    dispatching_ = true;
    const bool ok = admission.rewards_suppressed
        ? rewards_.consume(session, services, outcomes, error)
        : world_->with_loot_random(
            [&](dh2::data::LootRandom8V2& random, std::string& inner_error) {
                services.gameplay_rng = &random;
                scatter_rng_ = &random;
                const bool consumed = rewards_.consume(session, services, outcomes, inner_error);
                scatter_rng_ = nullptr;
                return consumed;
            }, error);
    dispatching_ = false;
    return ok;
}


// P16 QUESTS: quest XP through the bound session's progression services (same owners as kill XP).
bool RuntimeSessionDeathRewardsV1::award_experience(dh::foundation::ActorId player, float xp,
                                                    std::string& error) {
    error.clear();
    if (dispatching_) return fail(error, "Quest XP cannot run while death rewards dispatch");
    if (!session_ || !world_ || session_->world() != world_ || !creation_owner_ || !creation_owner_->valid() ||
        creation_owner_->properties.get() != loot_source_.properties.get() ||
        !gameplay_context_lease_ || !loot_source_.valid())
        return fail(error, "Quest XP binding does not belong to this live session/source");
    RuntimeDeathRewardAdmissionV1 admission;
    if (!bindings_.read_reward_admission(bindings_.context, admission, error)) return false;
    std::int32_t current_difficulty = -1, unlocked_difficulty = -1;
    std::int32_t max_level{};
    if (!admission.rewards_suppressed) {
        if (!bindings_.read_difficulty(bindings_.context, current_difficulty, unlocked_difficulty, error)) return false;
        if (current_difficulty < 0 || unlocked_difficulty < 0)
            return fail(error, "Current/unlocked source difficulty is absent or negative");
        if (!loot_source_.max_level_for_difficulty(unlocked_difficulty, max_level, error)) return false;
    }
    RuntimeDeathRewardServicesV1 services;
    services.admission = admission;
    services.loot_tables = loot_source_.loot;
    services.loot_powers = loot_source_.power_resources;
    services.loot_entry = {this, &RuntimeSessionDeathRewardsV1::loot_entry_thunk};
    services.xp_design = loot_source_.design_settings.rows().empty()
        ? nullptr : &loot_source_.design_settings.rows().front();
    services.properties = loot_source_.properties.get();
    services.current_difficulty = current_difficulty;
    services.unlocked_difficulty = unlocked_difficulty;
    services.max_level = max_level;
    services.context = this;
    services.query_one_kill_level_up = &RuntimeSessionDeathRewardsV1::one_kill_level_up_thunk;
    services.resolve_character = &RuntimeSessionDeathRewardsV1::resolve_character_thunk;
    services.on_level_up = &RuntimeSessionDeathRewardsV1::level_up_thunk;
    services.spawn_world_item = &RuntimeSessionDeathRewardsV1::spawn_world_item_thunk;
    dispatching_ = true;
    const bool ok = admission.rewards_suppressed
        ? rewards_.award_experience(*world_, player, xp, services, error)
        : world_->with_loot_random(
            [&](dh2::data::LootRandom8V2& random, std::string& inner_error) {
                services.gameplay_rng = &random;
                return rewards_.award_experience(*world_, player, xp, services, inner_error);
            }, error);
    dispatching_ = false;
    return ok;
}
void RuntimeSessionDeathRewardsV1::reset() noexcept {
    rewards_.clear();
    session_binding_lease_.reset();
    session_ = nullptr;
    world_ = nullptr;
    creation_owner_ = nullptr;
    world_items_.reset();
    gameplay_context_lease_.reset();
    bindings_ = {};
    loot_source_ = {};
    loot_source_owner_ = {};
    dispatching_ = false;
}

} // namespace dh::foundation::loot
