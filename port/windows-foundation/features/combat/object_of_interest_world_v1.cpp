#include "object_of_interest_world_v1.hpp"

#include "../../playable_actor_world.hpp"

namespace dh::foundation {
namespace {
const PlayableActorWorld* live_world(const CombatSession& session) noexcept {
    return session.world();
}
} // namespace

void update_object_of_interest_v1(const CombatSession& session, ActorId player, double dt_seconds,
                                  ObjectOfInterestOwnerV1& owner) {
    const auto* world = live_world(session);
    const auto* self = world ? world->find_actor(player) : nullptr;
    if (!world || !self) {
        owner.reset();
        return;
    }
    std::vector<ObjectOfInterestCandidateV1> candidates;
    candidates.reserve(world->actors().size());
    for (const auto& pair : world->actors()) {
        if (pair.first == player) continue;
        const auto& actor = pair.second;
        candidates.push_back({pair.first, actor.transform.position[0], actor.transform.position[1],
                              world->eligible_target(*self, actor)});
    }
    owner.update(player, dt_seconds, self->transform.position[0], self->transform.position[1], candidates);
}

ActorId rendered_target_marker_actor_v1(const CombatSession& session, ActorId player,
                                        const ObjectOfInterestOwnerV1& owner) {
    const auto* world = live_world(session);
    const auto* self = world ? world->find_actor(player) : nullptr;
    if (!world || !self) return invalid_actor_id;
    const auto presentation = session.target_presentation_state(player);
    const ActorId object = owner.owner() == player ? owner.object() : invalid_actor_id;
    return resolve_rendered_target_marker_v1(
        player, presentation.last_target_known, presentation.last_target, object,
        [world, self](ActorId id) {
            const auto* target = world->find_actor(id);
            return target != nullptr && world->eligible_target(*self, *target);
        });
}

} // namespace dh::foundation
