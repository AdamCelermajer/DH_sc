#pragma once
#include <cstdint>
#include "../game-data/combat_result.hpp"

namespace dh2::character_dot_tick {
// Native projections, not overlays of the original ARM objects. Owner is the
// live CharProperties+4 word; the property/sheet identities are captured once.
struct Owner { std::uintptr_t character; };
struct State { std::uintptr_t properties, resolved_sheet; Owner* owner; };
struct ReadRequest { std::uintptr_t properties, sheet; std::uint32_t property; };
struct AttackRequest {
    dh2::data::CombatResult* result;
    std::uintptr_t attacker, defender;
    std::uint32_t amount;
    std::int32_t element;
};
struct ApplyRequest {
    dh2::data::CombatResult* result;
    std::uintptr_t attacker, defender;
    std::uint32_t mode;
};
struct Services {
    void* context;
    // Zero succeeds. Read is _GetProperty(captured resolved sheet,126..131),
    // not an uncached property recomputation. IsDead returns its raw word.
    std::int32_t (*read_property)(void*, const ReadRequest*, std::uint32_t*);
    std::int32_t (*is_dead)(void*, std::uintptr_t character, std::uint32_t*);
    // These are genuine F_DotAttack and F_ApplyResult providers. CombatResult
    // is the maintained native 40-byte result projection, not an ARM overlay.
    // Attack must produce every field Apply consumes; its output is reused for
    // all six entries. Apply may change that output as the source body does.
    std::int32_t (*dot_attack)(void*, const AttackRequest*);
    std::int32_t (*apply_result)(void*, const ApplyRequest*);
};
enum class Operation : std::uint32_t { read_property, is_dead, dot_attack, apply_result };
struct Result {
    std::uint32_t calls, last_operation, property_reads, positive_properties,
                  dead_skips, attacks, applications, completed_properties,
                  last_property, captured_amount;
};
enum class Status : std::int32_t { complete, invalid_argument, service_unavailable, service_failed };

// Complete original CharProperties::HandleDots(),144B0x3df3f0. Each positive
// signed word has element property-127 (-1..4). IsDead, Attack and Apply each
// receive a fresh owner read; each self pair uses one captured owner. Apply's
// mode is the captured zero IsDead return. There is no elapsed-time/duration
// mutation in this caller. Event0x34 dispatch/timer ownership remain external.
Status tick(const State*, const Services*, Result*);

// One owning thread retains State/Owner/Services/output and all borrowed live
// sheets, Characters, virtual-query and combat-provider backing through return,
// including owners retired by callbacks. State identities and Services are
// captured; callbacks may mutate Owner.character, source properties and their
// own combat result. They may not overwrite output/control storage, invalidate
// borrowed backing, or reenter the same Owner/output. Independent owners and
// outputs may nest. Requests/local result may not escape synchronous callbacks.
// Controls must be aligned and disjoint. Zero owner is permitted until a
// positive property reaches it, then fails explicitly instead of dereferencing
// null. A missing reached provider, error or exception stops with prior effects
// retained: no rollback, extra application, cleanup or property loop is added.
} // namespace dh2::character_dot_tick
