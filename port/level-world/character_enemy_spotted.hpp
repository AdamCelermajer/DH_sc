#pragma once

#include <cstdint>

namespace dh2::character_enemy_spotted {

// The adapter updates this coherent pair when the source AI+0x1c selection
// changes. callee is the selected AIS vtable byte slot +0x34 identity.
struct ActiveAIS {
    std::uintptr_t identity;
    std::uintptr_t callee;
};

// Borrowed source projections, not an overlay of the original ARM32 object.
// The AI itself and enemy argument remain fixed during this synchronous call;
// owner, group and selected AIS can change through services. in_progress is a
// port ownership guard, not an original Character/CharAI field.
struct State {
    std::uintptr_t ai_identity;
    std::uintptr_t owner_identity;
    std::uintptr_t group_identity;
    ActiveAIS active;
    std::uint32_t in_progress;
};

enum class DebugPoint : std::uint32_t { entry, positive_aggro_added };

struct Services {
    void* context;
    // All int32 callbacks return zero for a completed adapter call and nonzero
    // for a port failure. Source predicates/float results use output values.
    // debug_switch's status is not the ignored source GetSwitch return value.
    // Optional original GetDebug/string/GetSwitch/destructor sequence. If
    // absent, that diagnostic subsystem is explicitly outside this gate's
    // scope. It never supplies a gameplay acceptance predicate.
    std::int32_t (*debug_switch)(void*, State*, DebugPoint);
    std::int32_t (*group_enemy_spotted)(void*, State*, std::uintptr_t group,
                                       std::uintptr_t owner, std::uintptr_t enemy);
    // SM_IsAwaitingToSpawn compares the current state ID to 17. This source
    // handler does not use Character::IsDead or SM_IsDead for these two gates.
    std::int32_t (*is_awaiting_to_spawn)(void*, State*, std::uintptr_t character,
                                        std::uint32_t* source_boolean);
    std::int32_t (*is_in_limbus)(void*, State*, std::uintptr_t character,
                                std::uint32_t* source_boolean);
    std::int32_t (*is_in_combat)(void*, State*, std::uintptr_t ai,
                                std::uint32_t* source_boolean);
    std::int32_t (*is_player)(void*, State*, std::uintptr_t enemy,
                            std::uint32_t* source_boolean);
    // AI_GetAggro is the read-only outgoing-map lookup. Values are raw IEEE
    // binary32 words, preserving signed zero and NaN without integer coercion.
    std::int32_t (*get_aggro)(void*, State*, std::uintptr_t ai,
                            std::uintptr_t enemy, std::uint32_t* float_bits);
    // Original global design object +0x30. Read only on the zero-aggro path,
    // after the owner passed to AddAggro has already been captured.
    std::int32_t (*initial_aggro)(void*, State*, std::uint32_t* float_bits);
    // Calls owner.CharAI::AI_AddAggro(enemy, amount), including real relation
    // mutation/notifications. Its returned delta controls diagnostics only.
    std::int32_t (*add_aggro)(void*, State*, std::uintptr_t captured_owner,
                            std::uintptr_t enemy, std::uint32_t amount_bits,
                            std::uint32_t* returned_delta_bits);

    // Port lifetime boundary: retain the exact late-captured AIS/callee pair,
    // even if this callback replaces State.active. Success produces a non-null
    // hold. Failure produces null and acquires no hold. The adapter must retain
    // any callable/session owner, not just an identity token.
    std::int32_t (*retain_active)(void*, State*, const ActiveAIS*, void** hold);
    std::int32_t (*dispatch_active)(void*, State*, const ActiveAIS*, void* hold,
                                   std::uintptr_t enemy);
    void (*release_active)(void*, const ActiveAIS*, void* hold) noexcept;
};

enum class Decision : std::uint32_t {
    incomplete, enemy_awaiting_spawn, owner_awaiting_spawn, enemy_in_limbus, owner_in_limbus,
    no_active_ais, dispatched,
};
enum class Status : std::int32_t {
    complete = 0, invalid_argument = 1, service_unavailable = 2,
    service_failed = 3, reentrant_call = 4, invalid_live_projection = 5,
    active_lifetime_failed = 6,
};
struct Result {
    Decision decision;
    std::uint32_t debug_queries;
    std::uint32_t group_calls;
    std::uint32_t awaiting_spawn_queries;
    std::uint32_t limbus_queries;
    std::uint32_t combat_queries;
    std::uint32_t player_queries;
    std::uint32_t aggro_queries;
    std::uint32_t initial_aggro_reads;
    std::uint32_t aggro_adds;
    std::uint32_t active_retains;
    std::uint32_t active_dispatches;
    std::uint32_t aggro_bits;
    std::uint32_t added_delta_bits;
    ActiveAIS dispatched_active;
};

// Bounded original CharAI::OnEnemySpotted gate/dispatch. Caller keeps State,
// context, the fixed enemy, and every exposed owner/group projection alive.
// Services are copied before effects. Only taken branches require services.
// Providers may change owner/group/active, but must not overwrite in_progress,
// Result, or invalidate any borrowed projection while it is being consumed.
// No other thread may modify State. Same-state nested entry is rejected before
// effects; independent states may nest. Runtime failures/exceptions preserve
// completed effects and release only the port AIS hold. No rollback, source
// cleanup, candidate search, or AISMonster fallback is invented.
Status on_enemy_spotted(State*, std::uintptr_t enemy, const Services*, Result*);

}  // namespace dh2::character_enemy_spotted
