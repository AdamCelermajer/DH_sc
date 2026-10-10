#include "object_of_interest_owner_v1.hpp"

#include <cmath>
#include <limits>

namespace dh::foundation {
namespace {
float squared_distance(float ax, float ay, float bx, float by) noexcept {
    const float dx = ax - bx;
    const float dy = ay - by;
    return dx * dx + dy * dy;
}
} // namespace

void ObjectOfInterestOwnerV1::reset() noexcept {
    owner_ = invalid_actor_id;
    object_ = invalid_actor_id;
    timer_ms_ = 0.0f;
}

void ObjectOfInterestOwnerV1::update(ActorId owner, double dt_seconds, float owner_x, float owner_y,
                                     const std::vector<ObjectOfInterestCandidateV1>& candidates) noexcept {
    if (owner != owner_) {
        reset();
        owner_ = owner;
    }
    if (!(dt_seconds > 0.0)) dt_seconds = 0.0;
    timer_ms_ -= static_cast<float>(dt_seconds * 1000.0);
    if (timer_ms_ > 0.0f) return;

    // Source: the timer restarts at 500, the OOI is cleared and then re-queried. Remainders are not carried.
    timer_ms_ = object_of_interest_refresh_ms_v1;
    object_ = invalid_actor_id;
    if (owner_ == invalid_actor_id) return;

    const float range_squared = object_of_interest_distance_v1 * object_of_interest_distance_v1;
    float best = std::numeric_limits<float>::infinity();
    for (const auto& candidate : candidates) {
        if (candidate.id == invalid_actor_id || candidate.id == owner_ || !candidate.eligible) continue;
        const float distance = squared_distance(owner_x, owner_y, candidate.x, candidate.y);
        if (!(distance <= range_squared)) continue;
        if (object_ == invalid_actor_id || distance < best ||
            (distance == best && candidate.id < object_)) {
            object_ = candidate.id;
            best = distance;
        }
    }
}

ActorId resolve_rendered_target_marker_v1(ActorId player, bool last_known, ActorId last_target,
                                          ActorId object_of_interest,
                                          const std::function<bool(ActorId)>& eligible_now) noexcept {
    const auto candidate = resolve_auto_target_marker_v1(
        static_cast<std::uintptr_t>(player),
        last_known ? static_cast<std::uintptr_t>(last_target) : std::uintptr_t{0},
        static_cast<std::uintptr_t>(object_of_interest));
    if (candidate.identity == 0) return invalid_actor_id;
    const auto id = static_cast<ActorId>(candidate.identity);
    if (!eligible_now || !eligible_now(id)) return invalid_actor_id;
    return id;
}

} // namespace dh::foundation
