#pragma once

#include "character_aggro_target_search.hpp"

namespace dh2::character_aggro_candidate_events {

struct State {
    std::uintptr_t ai;
    std::uintptr_t owner;
    // Source CharAI+0x40, passed unchanged as event 0x0c payload when no
    // enemy candidate was notified. Its original target producer is external.
    std::uintptr_t source_identity_40;
};
enum class Relation : std::uint32_t { enemy, friend_, neutral };
struct Services {
    void* context;
    std::int32_t (*classify)(void*, State*, Relation,
                             std::uintptr_t owner, std::uintptr_t candidate,
                             std::uint32_t* value);
    std::int32_t (*raise_event)(void*, State*, std::uintptr_t owner,
                                std::uint32_t event, std::uintptr_t payload);
};
struct Result {
    std::uint32_t candidates_consumed;
    std::uint32_t relationship_queries;
    std::uint32_t enemy_events;
    std::uint32_t friend_events;
    std::uint32_t neutral_events;
    std::uint32_t source_event_12;
};
enum class Status : std::int32_t {
    complete = 0, invalid_argument = 1, service_unavailable = 2,
    service_failed = 3, target_list_failed = 4,
};

// Bounded normal _UpdateAggro candidate-consumption branch. Reuses the new
// filter2/all-relationship/closest TargetList heap, already populated by the
// source search. Consumes every candidate; never stops at the first enemy.
// Classify enemy, then friend, then neutral with a fresh owner before each
// call. Dispatch 9/7/8 before popping; with no enemy, a nonzero current +0x40
// identity causes event 0x0c with that identity (not a null payload).
// Candidate identity is TargetInfo::object_identity (the source +0 field),
// even when the resolved Character projection has a different adapter identity.
//
// The caller retains this state, list, heap, service context and all identities
// through synchronous calls. Callbacks may replace owner/+0x40, but must not
// change, destroy or recursively consume this stack-local-equivalent list.
// One owning thread; a nested call requires its own retained list/output.
// Lists must satisfy the existing TargetList initialization contract, including
// positive capacity and retained owner/object projections even when empty.
// Classification
// bodies, search/timing/type-specific branches, assertions for invalid source
// TargetInfo flags, and Character::RaiseEvent are external services.
// Errors retain completed effects and leave the current candidate unpopped.
Status consume(State*, dh2::character::aggro_search::TargetList*,
               const Services*, Result*);

}  // namespace dh2::character_aggro_candidate_events
