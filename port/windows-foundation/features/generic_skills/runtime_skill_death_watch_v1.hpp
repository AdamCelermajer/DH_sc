#pragma once
// v21.8 es-runtime: death watch on tracked actors (E2: `IsDead()` polls).
//
// Original:
//   - prince_necromancer_pestilence.luac: every attacked victim is kept in `attackedTargets`; a repeating 500 ms skill
//     timer (started in OnSkillUpdate_) calls `object:IsDead()` on each entry, and an entry that reports dead is
//     explosion-ed (DamageNearbyTargets) and removed.
//   - prince_necromancer_rise.luac: checkslot() polls `RISEN[n]:IsDead()` before reusing a slot.
//   - IDA GameObject::_IsDead (0x38ea48) pushes a boolean produced by a virtual call; the callee is not decoded here.
//     INFERENCE: a tracked actor is dead when it is no longer in the Session or it is not alive (the port's
//     CombatSession::actor/alive). The atlas row for IsDead is the same name without a native address.
//
// RuntimeSkillDeathWatchV1 keeps the tracked list in tracking order and removes what it reports dead, which is the
// Pestilence poll. The predicate is injected so the same list can be checked against a Session or a test double.
#include "../../actor_state.hpp"

#include <functional>
#include <vector>

namespace dh::foundation {
class CombatSession;
}

namespace dh::foundation::generic_skills {

// GameObject::IsDead under the port's inference (see the header comment).
bool actor_is_dead_in_session(const CombatSession& session, ActorId actor);

class RuntimeSkillDeathWatchV1 {
public:
    // Adds an actor (no duplicates; invalid ids are ignored). Returns false when it was already tracked.
    bool track(ActorId actor);
    // Stops tracking an actor (for example when the caster's skill is cleaned up). Returns false when not tracked.
    bool untrack(ActorId actor);
    bool tracking(ActorId actor) const;
    std::size_t size() const noexcept { return tracked_.size(); }
    // One Pestilence poll: returns the tracked actors that `is_dead` reports dead, in tracking order, and removes them.
    std::vector<ActorId> poll(const std::function<bool(ActorId)>& is_dead);
    void clear() { tracked_.clear(); }

private:
    std::vector<ActorId> tracked_;
};

} // namespace dh::foundation::generic_skills
