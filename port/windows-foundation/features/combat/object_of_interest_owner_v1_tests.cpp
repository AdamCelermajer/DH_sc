#include "object_of_interest_owner_v1.hpp"

#include <cstdio>
#include <cstdlib>
#include <set>

using namespace dh::foundation;

namespace {
constexpr ActorId player = 1;
constexpr ActorId bogwomp = 7;
constexpr ActorId goblin = 9;
constexpr ActorId far_enemy = 12;

int failures = 0;
void check(bool ok, const char* name) {
    std::printf("%s %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) ++failures;
}

ObjectOfInterestCandidateV1 cand(ActorId id, float x, float y, bool eligible = true) {
    return {id, x, y, eligible};
}

// Eligible set used by the render gate.
std::function<bool(ActorId)> eligible_set(std::set<ActorId> live) {
    return [live](ActorId id) { return live.count(id) != 0; };
}
} // namespace

int main() {
    // 1. Skill Post clears the combat/last target (ClearTarget -> SyncLastTarget -> last known null).
    //    The OOI candidate stays eligible, so the marker must stay on it.
    {
        ObjectOfInterestOwnerV1 ooi;
        const std::vector<ObjectOfInterestCandidateV1> world{cand(bogwomp, 50, 0)};
        ooi.update(player, 0.016, 0, 0, world);
        const auto shown = resolve_rendered_target_marker_v1(player, true, invalid_actor_id, ooi.object(),
                                                             eligible_set({bogwomp}));
        check(ooi.object() == bogwomp && shown == bogwomp, "skill Post cleared last target keeps marker on eligible OOI");
    }

    // 2. Space release with no explicit target: same as case 1 (combat target is not a resolver input).
    {
        ObjectOfInterestOwnerV1 ooi;
        const std::vector<ObjectOfInterestCandidateV1> world{cand(bogwomp, 40, 0)};
        ooi.update(player, 0.016, 0, 0, world);
        const auto shown = resolve_rendered_target_marker_v1(player, false, bogwomp, ooi.object(),
                                                             eligible_set({bogwomp}));
        check(shown == bogwomp, "unknown last target falls back to eligible OOI after release");
    }

    // 3. OOI dies inside a refresh period: the render gate hides the marker at once; the stored OOI is
    //    re-derived on the next 500 ms refresh (no other eligible actor -> none).
    {
        ObjectOfInterestOwnerV1 ooi;
        std::vector<ObjectOfInterestCandidateV1> world{cand(bogwomp, 50, 0)};
        ooi.update(player, 0.0, 0, 0, world);
        world[0].eligible = false;
        ooi.update(player, 0.3, 0, 0, world);  // 300 ms: still inside the period
        const auto hidden = resolve_rendered_target_marker_v1(player, true, invalid_actor_id, ooi.object(),
                                                              eligible_set({}));
        check(hidden == invalid_actor_id, "dead OOI hides marker immediately (render gate)");
        ooi.update(player, 0.25, 0, 0, world);  // 550 ms total: refresh has run
        check(ooi.object() == invalid_actor_id, "dead OOI is dropped by the 500 ms refresh");
    }

    // 4. Candidate leaves range (200): stored OOI survives until the refresh, then is re-derived.
    {
        ObjectOfInterestOwnerV1 ooi;
        std::vector<ObjectOfInterestCandidateV1> world{cand(bogwomp, 100, 0), cand(goblin, 0, 150)};
        ooi.update(player, 0.0, 0, 0, world);
        check(ooi.object() == bogwomp, "nearest eligible candidate is chosen");
        world[0].x = 250;  // out of range
        ooi.update(player, 0.4, 0, 0, world);
        check(ooi.object() == bogwomp, "out-of-range OOI kept until refresh (400 ms)");
        ooi.update(player, 0.1, 0, 0, world);  // 500 ms
        check(ooi.object() == goblin, "out-of-range OOI replaced by in-range eligible one at 500 ms");
    }

    // 5. Tab sticky: explicit last target survives ticks and a combo/skill clear of OOI-only state.
    {
        ObjectOfInterestOwnerV1 ooi;
        const std::vector<ObjectOfInterestCandidateV1> world{cand(bogwomp, 30, 0), cand(goblin, 10, 0)};
        for (int i = 0; i < 3; ++i) ooi.update(player, 0.5, 0, 0, world);
        const auto shown = resolve_rendered_target_marker_v1(player, true, goblin, ooi.object(),
                                                             eligible_set({bogwomp, goblin}));
        check(shown == goblin, "sticky/last target has precedence over nearer OOI");
    }

    // 6. Ring never appears for ineligible actors.
    {
        ObjectOfInterestOwnerV1 ooi;
        const std::vector<ObjectOfInterestCandidateV1> world{cand(bogwomp, 30, 0, false), cand(goblin, 80, 0, true)};
        ooi.update(player, 0.0, 0, 0, world);
        check(ooi.object() == goblin, "ineligible candidate never chosen as OOI");
        const auto rejected = resolve_rendered_target_marker_v1(player, true, bogwomp, ooi.object(),
                                                                eligible_set({goblin}));
        check(rejected == invalid_actor_id, "ineligible last target hides marker (no OOI fallback)");
        const auto self_last = resolve_rendered_target_marker_v1(player, true, player, ooi.object(),
                                                                 eligible_set({goblin}));
        check(self_last == goblin, "self last target falls back to OOI");
        const auto unknown_last = resolve_rendered_target_marker_v1(player, false, bogwomp, invalid_actor_id,
                                                                    eligible_set({bogwomp}));
        check(unknown_last == invalid_actor_id, "unknown last target value is ignored (no OOI -> hidden)");
    }

    // 7. Timer: first update refreshes; 500 ms expiry exact; 16.7 ms steps refresh about every 30 frames.
    {
        ObjectOfInterestOwnerV1 ooi;
        std::vector<ObjectOfInterestCandidateV1> world{cand(bogwomp, 20, 0)};
        ooi.update(player, 0.0, 0, 0, world);
        check(ooi.object() == bogwomp, "first update refreshes immediately");
        world.push_back(cand(goblin, 5, 0));
        ooi.update(player, 0.499, 0, 0, world);
        check(ooi.object() == bogwomp, "499 ms: no refresh yet");
        ooi.update(player, 0.001, 0, 0, world);
        check(ooi.object() == goblin, "500 ms: refresh picks the nearer candidate");
        // Goblin (nearest) is the OOI at t=0; it then leaves range. Count frames until the refresh moves OOI.
        int frames_to_refresh = 0;
        ObjectOfInterestOwnerV1 steps;
        steps.update(player, 0.0, 0, 0, world);
        check(steps.object() == goblin, "t=0 OOI is the nearest in-range candidate");
        world[1].x = 500;  // goblin leaves range
        for (int i = 1; i <= 60; ++i) {
            steps.update(player, 1.0 / 60.0, 0, 0, world);
            if (steps.object() == bogwomp && frames_to_refresh == 0) frames_to_refresh = i;
        }
        check(frames_to_refresh >= 29 && frames_to_refresh <= 31, "60 fps steps refresh after about 30 frames");
    }

    // 8. Owner change resets; nearest tie prefers the lower id; self and invalid never chosen.
    {
        ObjectOfInterestOwnerV1 ooi;
        const std::vector<ObjectOfInterestCandidateV1> world{cand(goblin, 10, 0), cand(bogwomp, -10, 0),
                                                              cand(player, 0, 0, false), cand(invalid_actor_id, 1, 0)};
        ooi.update(player, 0.0, 0, 0, world);
        check(ooi.object() == bogwomp, "equal distance ties choose the lower actor id");
        ooi.update(goblin, 0.0, 0, 0, world);
        check(ooi.owner() == goblin && ooi.object() == bogwomp, "owner change refreshes for new owner");
        ObjectOfInterestOwnerV1 none;
        none.update(player, 0.0, 0, 0, std::vector<ObjectOfInterestCandidateV1>{cand(far_enemy, 201, 0)});
        check(none.object() == invalid_actor_id, "candidate beyond 200 is not chosen");
    }

    // 9. Bad dt is treated as zero and does not corrupt the timer.
    {
        ObjectOfInterestOwnerV1 ooi;
        const std::vector<ObjectOfInterestCandidateV1> world{cand(bogwomp, 20, 0)};
        ooi.update(player, -5.0, 0, 0, world);
        check(ooi.object() == bogwomp, "negative dt does not block the first refresh");
    }

    if (failures) {
        std::printf("FAILED %d check(s)\n", failures);
        return EXIT_FAILURE;
    }
    std::printf("OOI owner and marker policy tests passed\n");
    return EXIT_SUCCESS;
}
