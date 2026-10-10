#pragma once

// Live Session/world adapter for object_of_interest_owner_v1. Reads CombatSession only; never writes
// combat target, last-target or sticky state.

#include "object_of_interest_owner_v1.hpp"
#include "../../combat_session.hpp"

namespace dh::foundation {

// Advances the owner for the player's Session. Call once per non-paused combat update.
void update_object_of_interest_v1(const CombatSession& session, ActorId player, double dt_seconds,
                                  ObjectOfInterestOwnerV1& owner);

// Actor whose ring and name/health frame should be drawn, or invalid_actor_id when hidden.
ActorId rendered_target_marker_actor_v1(const CombatSession& session, ActorId player,
                                        const ObjectOfInterestOwnerV1& owner);

} // namespace dh::foundation
