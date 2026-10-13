// v21.8 es-runtime: unit coverage for RuntimeSkillDeathWatchV1 (the Pestilence poll and the Rise slot check).
// Covered: tracking order, duplicate and invalid ids, the poll removing exactly the dead entries in tracking order,
// and the Session predicate for an absent actor (no actor in an empty CombatSession counts as dead).
#include "runtime_skill_death_watch_v1.hpp"
#include "../../combat_session.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh::foundation;
using namespace dh::foundation::generic_skills;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
} // namespace

int main() {
    try {
        RuntimeSkillDeathWatchV1 watch;
        check(watch.track(10) && watch.track(11) && watch.track(12), "Tracking failed");
        check(!watch.track(11), "A duplicate actor was tracked twice");
        check(!watch.track(invalid_actor_id), "The invalid actor id was tracked");
        check(watch.size() == 3 && watch.tracking(11) && !watch.tracking(99), "Tracking state is wrong");

        // Pestilence poll: dead entries are reported in tracking order and removed; live entries stay.
        std::vector<ActorId> dead_set{12, 10};
        const auto dead = watch.poll([&](ActorId actor) {
            for (const auto candidate : dead_set)
                if (candidate == actor) return true;
            return false;
        });
        check(dead.size() == 2 && dead[0] == 10 && dead[1] == 12, "Poll did not report the dead entries in tracking order");
        check(watch.size() == 1 && watch.tracking(11) && !watch.tracking(10), "Poll did not remove exactly the dead entries");
        check(watch.poll([](ActorId) { return false; }).empty() && watch.size() == 1, "A live poll changed the list");
        std::cout << "poll: dead {10,12} reported in order and removed; 11 kept\n";

        check(watch.untrack(11) && !watch.untrack(11) && watch.size() == 0, "untrack failed");

        // Session predicate: an actor that is not in the session counts as dead (INFERENCE, see the header).
        CombatSession empty;
        check(actor_is_dead_in_session(empty, 1234), "An absent actor was not reported dead");
        std::cout << "session predicate: absent actor is dead (inference)\n";
        std::cout << "runtime_skill_death_watch_v1 passed\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "runtime_skill_death_watch_v1 failed: " << ex.what() << '\n';
        return 1;
    }
}
