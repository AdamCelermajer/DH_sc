#include "object_of_interest_owner_v1.hpp"

#include <algorithm>
#include <cmath>

namespace dh::foundation {
namespace {
float length(const std::array<float, 3>& v) noexcept {
    return std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

// Source TargetList::Search: |angle(c - p, look)| in radians, full arc (no cut-off).
float angle_to_look(const std::array<float, 3>& delta, const std::array<float, 3>& look) noexcept {
    const float d = length(delta), l = length(look);
    if (!(d > 0.0f) || !(l > 0.0f)) return 0.0f;
    const float dot = delta[0] * look[0] + delta[1] * look[1] + delta[2] * look[2];
    const float c = std::clamp(dot / (d * l), -1.0f, 1.0f);
    return std::fabs(std::acos(c));
}

// Centre distance minus candidate radius minus owner melee radius (TargetList::Search v19).
float search_distance(const ObjectOfInterestOwnerStateV1& owner, const ObjectOfInterestCandidateV1& c) noexcept {
    const std::array<float, 3> delta{c.position[0] - owner.position[0], c.position[1] - owner.position[1],
                                     c.position[2] - owner.position[2]};
    return length(delta) - c.radius - owner.melee_radius;
}
} // namespace

void ObjectOfInterestOwnerV1::reset() noexcept {
    owner_ = invalid_actor_id;
    object_ = invalid_actor_id;
    type_ = interaction_type_none_v1;
    changed_ = false;
    timer_ms_ = 0.0f;
}

void ObjectOfInterestOwnerV1::update(const ObjectOfInterestOwnerStateV1& owner, double dt_seconds,
                                     const std::vector<ObjectOfInterestCandidateV1>& candidates) noexcept {
    if (owner.owner != owner_) {
        reset();
        owner_ = owner.owner;
    }
    changed_ = false;
    if (owner_ == invalid_actor_id) return;
    if (!(dt_seconds > 0.0)) dt_seconds = 0.0;

    // Per-frame validity (source vt+140 on the stored OOI): an invalid OOI is cleared. The cached type is
    // not cleared here, matching the source, which only zeroes the OOI pointer.
    if (object_ != invalid_actor_id) {
        const bool still_valid = std::any_of(candidates.begin(), candidates.end(), [this](const auto& c) {
            return c.id == object_ && c.eligible;
        });
        if (!still_valid) object_ = invalid_actor_id;
    }

    timer_ms_ -= static_cast<float>(dt_seconds * 1000.0);
    if (timer_ms_ > 0.0f) return;

    // Source: the timer restarts at 500, the cached type is -1, the OOI is cleared and then re-queried.
    timer_ms_ = object_of_interest_refresh_ms_v1;
    const ActorId previous = object_;
    object_ = invalid_actor_id;
    type_ = interaction_type_none_v1;
    const auto selection = select_object_of_interest_v1(
        owner_, order_object_of_interest_candidates_v1(owner, candidates));
    object_ = selection.object;
    type_ = selection.interaction_type;
    changed_ = object_ != invalid_actor_id && object_ != previous;
}

ObjectOfInterestSelectionV1 select_object_of_interest_v1(
    ActorId owner, const std::vector<ObjectOfInterestCandidateV1>& ordered) noexcept {
    ObjectOfInterestSelectionV1 out;
    for (const auto& c : ordered) {
        if (c.id == owner || c.id == invalid_actor_id) continue;
        // Source: once an OOI is assigned, a non-Character candidate is considered only if its type is 1.
        if (out.object != invalid_actor_id && !c.is_character && c.interaction_type != interaction_type_trigger_v1)
            continue;
        // The candidate is assigned before its type is tested, so a rejected candidate remains the OOI.
        out.object = c.id;
        out.interaction_type = c.interaction_type;
        if (c.is_character) return out;                                              // flag bit: accept
        if (c.interaction_type == interaction_type_openable_v1 ||
            c.interaction_type == 2) return out;                                     // types 0 and 2: accept
        if (c.interaction_type == interaction_type_trigger_v1 && c.type1_targets_owner) return out;
    }
    return out;
}

std::vector<ObjectOfInterestCandidateV1> order_object_of_interest_candidates_v1(
    const ObjectOfInterestOwnerStateV1& owner, const std::vector<ObjectOfInterestCandidateV1>& candidates) {
    struct Queued {
        ObjectOfInterestCandidateV1 candidate;
        float angle;
    };
    std::vector<Queued> queue;
    queue.reserve(candidates.size());
    for (const auto& c : candidates) {
        if (c.id == owner.owner || c.id == invalid_actor_id || !c.eligible) continue;
        // GameObjects enter only with an action type (_IsGameObjectValid mode 1: type != -1).
        if (!c.is_character && c.interaction_type == interaction_type_none_v1) continue;
        if (!(search_distance(owner, c) <= object_of_interest_distance_v1)) continue;
        const std::array<float, 3> delta{c.position[0] - owner.position[0], c.position[1] - owner.position[1],
                                         c.position[2] - owner.position[2]};
        queue.push_back({c, angle_to_look(delta, owner.look)});
    }
    // _sortFrontal: flag bit (Character) first, then smaller angle first; equal keys by id for determinism.
    std::stable_sort(queue.begin(), queue.end(), [](const Queued& a, const Queued& b) {
        if (a.candidate.is_character != b.candidate.is_character) return a.candidate.is_character;
        if (a.angle != b.angle) return a.angle < b.angle;
        return a.candidate.id < b.candidate.id;
    });
    std::vector<ObjectOfInterestCandidateV1> ordered;
    ordered.reserve(queue.size());
    for (const auto& q : queue) ordered.push_back(q.candidate);
    return ordered;
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
