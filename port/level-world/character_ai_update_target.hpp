#pragma once
#include "character_ai_set_target.hpp"
#include <cstdint>

namespace dh2::character_ai_update_target {
// Reuse the actor-owned target state; do not create independent current/last
// targets or duplicate alive/sight snapshots for this producer.
using State=character::set_target::State;
using Owner=character::set_target::OwnerFacts;
enum class Operation : std::uint32_t {
    is_awaiting_spawn, is_in_limbus, is_interactive, char_ai_id,
    is_dead, is_in_sight, can_range_attack,
    is_in_close_range, is_in_range, is_in_melee_range, raise_event,
};
enum class Subject : std::uint32_t { owner, object, original_ai };
struct Request {
    Operation operation; Subject kind;
    std::uintptr_t subject, other;
    std::uint32_t event;
};
struct Response { std::uint32_t word; };
struct Services {
    void* context;
    // Zero success; calls are synchronous. Caller supplies genuine FSM, virtual,
    // Character getter, source sight/range and RaiseEvent implementations.
    // char_ai_id is called even though its result is discarded. All predicate
    // return words remain raw until the original caller's tests/conversions.
    // is_in_sight should bind character_ai_sight::evaluate_object with live owner
    // propagation; range callbacks are original full-body boundaries, never an
    // approximate reach or a substitute for unresolved target ownership.
    std::int32_t (*invoke)(void*,State*,const Request*,Response*);
};
enum class Decision : std::uint32_t {
    incomplete, awaiting_spawn, in_limbus, no_target, cleared_untargetable,
    no_target_after_callback, out_of_sight, no_longer_interactive,
    range_event,
};
struct Result {
    Decision decision;
    std::uint32_t service_calls, event_calls, last_event,
        computed_alive, computed_sight, range_event;
};
enum class Status : std::int32_t {
    complete, invalid_argument, service_unavailable, service_failed,
    invalid_source_fact,
};
// Entire original 556-byte _UpdateTarget caller ordering; its owning/math/event
// callees remain explicit synchronous services. No handle conversion or stored
// visibility-byte query exists in this body. Every owner/target reload occurs
// at the original source point. Edge events precede snapshot writes; events may
// clear/replace targets, snapshots or owners. Cached computed words survive them.
// One owning thread retains State/services/context/current and retired owner,
// target and callback storage through return. Services/Result must not alias
// State/owner or each other. Callbacks must not overwrite output/services or
// destroy borrowed state. Independent nested State/output is allowed; reentry
// into this same State is forbidden. Errors preserve earlier effects and writes
// with no rollback, extra events, cleanup or fabricated false predicate.
Status update(State*,const Services*,Result*);
} // namespace dh2::character_ai_update_target
