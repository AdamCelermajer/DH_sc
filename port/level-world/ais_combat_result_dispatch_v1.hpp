#pragma once

#include "../game-data/combat_result.hpp"
#include <cstdint>

namespace dh2::ais_combat_result_dispatch_v1 {

enum class Callback : std::uint32_t { target_hit, target_missed };

// Live references to the already-retained per-Character AIS. A null AIS means
// CharAI has no script owner and its source virtual call is a no-op. The flags
// pointer is read immediately before each callback, matching the source's
// per-AIS cached VCB membership test.
struct Actor {
    std::uintptr_t character;
    std::uintptr_t ais;
    const std::uint32_t* flags_b8;
};

struct Arguments {
    Actor* attacker;
    Actor* defender;
    const data::CombatResult* result;
};

struct Services {
    void* context;
    // Invoke the named AIS callback through that owner's retained VM. Passes
    // the original attacker and defender Character identities as arguments.
    // Zero means success; the VM and all borrowed identities stay retained for
    // the complete synchronous dispatch.
    std::int32_t (*dispatch)(void*, std::uintptr_t ais, Callback,
                             std::uintptr_t attacker, std::uintptr_t defender);
};

struct Report {
    // `attempts` counts source-selected owners (membership bit set), while
    // `callbacks` counts successful VM dispatches. Failed/unavailable attempts
    // do not stop the source attacker-then-defender sequence.
    std::uint32_t callbacks;
    std::uint32_t attempts;
    std::uint32_t failures;
    Callback selected;
    // Last nonzero callback status, or Status::service_unavailable when the
    // selected owner had no dispatch provider. Zero when no attempt failed.
    std::int32_t last_status;
};

enum class Status : std::int32_t {
    complete, invalid_argument, service_unavailable, service_failed
};

// Source CharAI::OnCombatResults order: attacker AIS, then defender AIS.
// AISDefault selects OnTargetMissed when AttackResult.outcomes & 3 is nonzero,
// otherwise OnTargetHit. Its own VCB membership bits are 0x1000 and 0x800.
// F_ApplyResult suppresses both calls when mask bit 0x20000000 is set.
// This adapter only dispatches callbacks; damage, result mutation, VM ownership
// and callback registration remain with their existing owners. Source calls
// both selected AIS owners in attacker/defender order even if an earlier Lua
// callback fails; report aggregates failures after both attempts.
Status dispatch(const Arguments*, const Services*, Report*);

} // namespace dh2::ais_combat_result_dispatch_v1
