#include "object_of_interest_world_v1.hpp"

#include "../../playable_actor_world.hpp"
#include "../interactions/interactable_registry_v1.hpp"

#include <cmath>

namespace dh::foundation {
namespace {
const PlayableActorWorld* live_world(const CombatSession& session) noexcept {
    return session.world();
}
} // namespace

void update_object_of_interest_v1(const CombatSession& session, ActorId player, double dt_seconds,
                                  ObjectOfInterestOwnerV1& owner, const InteractableRegistryV1* objects) {
    const auto* world = live_world(session);
    const auto* self = world ? world->find_actor(player) : nullptr;
    if (!world || !self) {
        owner.reset();
        return;
    }
    // Look vector from the facing yaw (ActorMovement::root_world_delta: local +Y rotated by facingRadians).
    const float heading = self->transform.rotation[2];
    ObjectOfInterestOwnerStateV1 state;
    state.owner = player;
    state.position = self->transform.position;
    state.look = {-std::sin(heading), std::cos(heading), 0.0f};
    state.melee_radius = world->melee_reach(player);

    std::vector<ObjectOfInterestCandidateV1> candidates;
    candidates.reserve(world->actors().size());
    for (const auto& pair : world->actors()) {
        if (pair.first == player) continue;
        const auto& actor = pair.second;
        ObjectOfInterestCandidateV1 c;
        c.id = pair.first;
        c.is_character = true;
        c.position = actor.transform.position;
        c.radius = world->target_radius(actor);
        c.eligible = world->eligible_target(*self, actor);
        // Source Character::GetInteractionType: an enemy to the viewer is type 8 (attack).
        c.interaction_type = c.eligible ? 8 : -1;
        candidates.push_back(c);
    }
    if (objects) {
        auto extra = objects->candidates(player);
        candidates.insert(candidates.end(), extra.begin(), extra.end());
    }
    owner.update(state, dt_seconds, candidates);
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
