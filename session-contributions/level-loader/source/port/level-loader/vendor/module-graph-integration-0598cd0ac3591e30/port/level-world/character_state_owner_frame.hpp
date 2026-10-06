#pragma once
#include "character_state_owner.hpp"
namespace dh2::character {
// Immutable source method metadata plus live owned current state. Addresses
// identify original methods; they are never callable native pointers.
struct StateOwnerUpdateRequest24 {
 std::uint32_t source_function;std::int32_t state;
 std::uintptr_t character;std::uint32_t elapsed_ms,reserved;
};
struct StateOwnerUpdateServices16 {
 void* context;
 // Required genuine backend for all other Update families. 0 delivered;
 // nonzero fails at this source prefix. May synchronously reenter the owner.
 int(*invoke)(void*,StateOwnerMachine40*,const StateOwnerUpdateRequest24*);
};
struct StateOwnerFrameContext56 {
 StateOwnerMachine40* machine;const Facts* facts;const Services* bodies;
 NativeFsmServices16 outer;StateOwnerUpdateServices16 other_updates;
};
static_assert(sizeof(StateOwnerUpdateRequest24)==24);
static_assert(sizeof(StateOwnerUpdateServices16)==16&&sizeof(StateOwnerFrameContext56)==56);
// Storage, fsm/state bindings and providers survive the complete call. Outer
// services provide actual profiling, engine dt and stun/scare command bodies.
// They may transition/event through StateOwner services, never directly assign
// the current ID. Refresh facts before returning after a producer changes.
// Every non-current outer request is delivered unchanged. Other Update bodies
// are mandatory providers even when the initial current state is bounded.
}
extern "C" {
// Complete source FSM frame composed with owned StateInfo and bounded native
// Idle3/Move4/Attack5/Dead12 Update bodies. Elapsed advances once (wraps uint32);
// nullable current skips virtual Update but preserves profile/end processing.
// 1 completed, -1 malformed before effects, -2 required failure at delivered
// prefix. Source elapsed/current changes before failures are not rolled back.
int dh2_character_state_owner_frame(dh2::character::StateOwnerFrameContext56*);
}
