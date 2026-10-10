#pragma once

// B004/B029 object-of-interest (OOI) owner and rendered target-marker policy.
//
// Source: Character::UpdateObjectOfInterest (0x3abb9c) keeps the OOI at
// Character+0x14a4 and a 500 ms refresh timer at Character+0x14aa. On expiry it
// clears the OOI and re-queries CharacterDesign.OOI_Distance (200). CharacterTargetMarkerV28
// draws last_target (Character+0x40c) first and falls back to the OOI.
//
// This owner never writes combat state. Combat target, last/preferred target,
// sticky target and the rendered marker stay separate values.

#include "../../actor_state.hpp"
#include "auto_target_marker_v1.hpp"

#include <cstdint>
#include <functional>
#include <vector>

namespace dh::foundation {

// Character+0x14aa reset value in UpdateObjectOfInterest.
inline constexpr float object_of_interest_refresh_ms_v1 = 500.0f;
// CharacterDesign.OOI_Distance (port/script-runtime/reference/design-bindings/cache-constant-inventory.json).
inline constexpr float object_of_interest_distance_v1 = 200.0f;

struct ObjectOfInterestCandidateV1 {
    ActorId id = invalid_actor_id;
    // Source horizontal plane (XY), as used by the collision scene.
    float x = 0.0f;
    float y = 0.0f;
    // Alive, enemy to the owner, targetable. Computed by the caller from the live world.
    bool eligible = false;
};

class ObjectOfInterestOwnerV1 {
public:
    // Advances the refresh timer by dt_seconds. When it expires the timer restarts at 500 ms and
    // the OOI is re-derived as the nearest eligible candidate within range (ties: lower id).
    // A change of owner resets the owner state so the first update refreshes immediately.
    void update(ActorId owner, double dt_seconds, float owner_x, float owner_y,
                const std::vector<ObjectOfInterestCandidateV1>& candidates) noexcept;
    void reset() noexcept;
    ActorId owner() const noexcept { return owner_; }
    ActorId object() const noexcept { return object_; }

private:
    ActorId owner_ = invalid_actor_id;
    ActorId object_ = invalid_actor_id;
    float timer_ms_ = 0.0f;
};

// Marker precedence: last target (only when its projection is known, non-null and not the player),
// else the OOI. The chosen actor must pass the render-time eligibility gate, otherwise the marker is
// hidden (invalid_actor_id). A rejected last target does not fall back to the OOI.
ActorId resolve_rendered_target_marker_v1(ActorId player, bool last_known, ActorId last_target,
                                          ActorId object_of_interest,
                                          const std::function<bool(ActorId)>& eligible_now) noexcept;

} // namespace dh::foundation
