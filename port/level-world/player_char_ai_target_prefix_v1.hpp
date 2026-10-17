#pragma once

#include "character_ai_initialization.hpp"
#include "character_ai_master_update.hpp"
#include "character_ai_update_target.hpp"
#include "character_coordinator.hpp"

namespace dh2::player_char_ai_target_prefix_v1 {

// Source-backed Player TargetUpdate adapter for the pre-interaction prefix.
// It borrows the existing CharAI target fields, Character FSM and target
// owner; it does not make a second target or state owner. Once a nonnull target
// reaches an interaction query, the absent Player provider fails closed.
struct Bindings {
    character::Coordinator* character = nullptr;
    character_ai_initialization::State* ai = nullptr;
    character::set_target::OwnerFacts* target_owner = nullptr;
};

using Result = character_ai_update_target::Result;
enum class Status : int { complete = 0, invalid_argument = 1, source_failed = 2 };
using MasterResult = character_ai_master_update::Result;

namespace detail {
struct Call {
    const Bindings* bindings;
    std::uint32_t calls = 0;
};

inline std::int32_t invoke(void* raw, character_ai_update_target::State* state,
                           const character_ai_update_target::Request* request,
                           character_ai_update_target::Response* response) {
    if (!raw || !state || !request || !response) return 1;
    auto& call = *static_cast<Call*>(raw);
    const auto& bindings = *call.bindings;
    if (!bindings.character || !bindings.ai || !bindings.target_owner ||
        state->identity != bindings.ai->identity ||
        state->owner != bindings.target_owner ||
        bindings.ai->owner_04 != bindings.character->owner() ||
        bindings.target_owner->identity != bindings.character->owner() ||
        request->kind != character_ai_update_target::Subject::owner ||
        request->subject != bindings.character->owner() || request->other != 0)
        return 1;

    ++call.calls;
    switch (request->operation) {
    case character_ai_update_target::Operation::is_awaiting_spawn:
        response->word = bindings.character->state.current == 17;
        return 0;
    case character_ai_update_target::Operation::is_in_limbus:
        response->word = bindings.character->state.current == 0;
        return 0;
    default:
        // Interaction, sight, range, AI ID and event providers do not have a
        // complete Player owner in this runtime. Do not guess their result.
        return 1;
    }
}

inline std::int32_t invoke_master(void* raw,
                                 character_ai_master_update::State* state,
                                 const character_ai_master_update::Request*,
                                 character_ai_master_update::Response*) {
    if (!raw || !state) return 1;
    ++*static_cast<std::uint32_t*>(raw);
    // Player master is null at CharAI construction. If another owner later
    // attaches a master, this adapter has no live getter/range/event services.
    return 1;
}
} // namespace detail

inline Status update(const Bindings* bindings, Result* result) {
    if (!bindings || !result || !bindings->character || !bindings->ai ||
        !bindings->target_owner || !bindings->character->bound() ||
        !bindings->ai->identity ||
        bindings->ai->owner_04 != bindings->character->owner() ||
        bindings->target_owner->identity != bindings->character->owner() ||
        bindings->target_owner->reserved)
        return Status::invalid_argument;

    auto& ai = *bindings->ai;
    character_ai_update_target::State target{
        ai.identity, bindings->target_owner, ai.requested_target_3c,
        ai.target_40, ai.last_target_44, ai.alive_48, ai.sight_49,
        ai.sticky_4c, 0};
    detail::Call call{bindings};
    const character_ai_update_target::Services services{&call, detail::invoke};
    const auto status = character_ai_update_target::update(&target, &services,
                                                            result);
    // TargetUpdate effects are synchronous and remain authoritative even when
    // a later required provider fails.
    ai.requested_target_3c = target.requested_target;
    ai.target_40 = target.target;
    ai.last_target_44 = target.last_target;
    ai.alive_48 = target.alive_snapshot;
    ai.sight_49 = target.sight_snapshot;
    ai.sticky_4c = target.sticky;
    return status == character_ai_update_target::Status::complete
        ? Status::complete : Status::source_failed;
}

// The source master-update body is complete for the currently constructed
// Player because CharAI construction sets master_50=0. The retained kernel
// returns before calling any master services on that path. A future nonnull
// master is rejected at its first required query until those owners exist.
inline Status update_master_null(const Bindings* bindings, MasterResult* result) {
    if (!bindings || !result || !bindings->character || !bindings->ai ||
        !bindings->character->bound() || !bindings->ai->identity ||
        bindings->ai->owner_04 != bindings->character->owner())
        return Status::invalid_argument;
    auto& ai = *bindings->ai;
    character_ai_master_update::State state{
        ai.identity, ai.owner_04, ai.master_50, ai.byte_54, ai.byte_55, 0};
    std::uint32_t unavailable_calls = 0;
    const character_ai_master_update::Services services{
        &unavailable_calls, detail::invoke_master};
    const auto status = character_ai_master_update::update(&state, &services,
                                                            result);
    ai.byte_54 = state.alive_54;
    ai.byte_55 = state.sight_55;
    return status == character_ai_master_update::Status::complete
        ? Status::complete : Status::source_failed;
}

} // namespace dh2::player_char_ai_target_prefix_v1
