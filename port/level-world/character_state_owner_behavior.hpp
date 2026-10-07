#pragma once
#include "character_state_owner.hpp"
namespace dh2::character {
// Character+544 and+441 live producers not present in bounded State. Do not
// infer these from animation, movement or target presence. Flags520/mask528/
// stopped442 are read live from the owner's State projection.
struct StateOwnerBehaviorPredicate8 {
 std::int32_t interaction;std::uint8_t can_interrupt,reserved[3];
};
struct StateOwnerBehaviorContext40 {
 const Facts* facts;const Services* bodies;
 const StateOwnerBehaviorPredicate8* predicates;
 // Required Character.RaiseEvent/pin/profiling and remaining state families/
 // Spawn services. No unsupported method is delivered as an accepted no-op.
 StateOwnerServices16 remaining;
};
static_assert(sizeof(StateOwnerBehaviorPredicate8)==8&&sizeof(StateOwnerBehaviorContext40)==40);
}
extern "C" {
// Borrowed context/facts/services must remain alive and at stable addresses for
// every bound call, including synchronous nested reentry. Body services must
// update their live Facts projection before returning from producer changes.
//1 bound,-1 malformed without replacing output. No owned context/allocation.
int dh2_character_state_owner_behavior_bind(dh2::character::StateOwnerServices16*,dh2::character::StateOwnerBehaviorContext40*);
// StateOwnerServices16 callback:0 delivered,nonzero failure. Uses only isolated
// Focus/Blur/OnEvent kernels for3/4/5/12; never calls legacy transition/event.
int dh2_character_state_owner_behavior_invoke(void*,dh2::character::StateOwnerMachine40*,const dh2::character::StateOwnerRequest48*,dh2::character::StateOwnerResponse8*);
}
