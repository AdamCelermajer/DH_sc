#include "runtime_skill_death_watch_v1.hpp"

#include "../../combat_session.hpp"

#include <algorithm>

namespace dh::foundation::generic_skills {

bool actor_is_dead_in_session(const CombatSession& session, ActorId actor) {
    const auto* state = session.actor(actor);
    return state == nullptr || !state->alive();
}

bool RuntimeSkillDeathWatchV1::track(ActorId actor) {
    if (actor == invalid_actor_id || tracking(actor)) return false;
    tracked_.push_back(actor);
    return true;
}

bool RuntimeSkillDeathWatchV1::untrack(ActorId actor) {
    const auto found = std::find(tracked_.begin(), tracked_.end(), actor);
    if (found == tracked_.end()) return false;
    tracked_.erase(found);
    return true;
}

bool RuntimeSkillDeathWatchV1::tracking(ActorId actor) const {
    return std::find(tracked_.begin(), tracked_.end(), actor) != tracked_.end();
}

std::vector<ActorId> RuntimeSkillDeathWatchV1::poll(const std::function<bool(ActorId)>& is_dead) {
    std::vector<ActorId> dead;
    std::vector<ActorId> alive;
    alive.reserve(tracked_.size());
    for (const auto actor : tracked_) {
        if (is_dead && is_dead(actor)) dead.push_back(actor);
        else alive.push_back(actor);
    }
    tracked_ = std::move(alive);
    return dead;
}

} // namespace dh::foundation::generic_skills
