// B077: outcome branches of the follower decisions (follower.luac / attacking_follower.luac / rene.luac driven by
// CharAI::_UpdateMaster events). Pure logic: no assets needed.
#include "follower_decision_v1.hpp"

#include <cmath>
#include <iostream>

using namespace dh::foundation::companions;

namespace {
int failures = 0;
void check(bool ok, const char* what) {
    if (!ok) { std::cerr << "FAIL: " << what << '\n'; ++failures; }
}
FollowerInputV1 at(float distance, double dt_ms = 16) {
    FollowerInputV1 in;
    in.distance = distance;
    in.dt_ms = dt_ms;
    in.radii.view_radius = 1200;
    in.radii.interact_radius = 200;
    in.radii.own_reach = 20;
    in.radii.master_radius = 30;
    return in;
}
} // namespace

int main() {
    check(follower_script_v1("follower") == FollowerScriptV1::follower, "script follower");
    check(follower_script_v1("attacking_follower") == FollowerScriptV1::attacking_follower, "script attacking_follower");
    check(follower_script_v1("rene") == FollowerScriptV1::rene, "script rene");
    check(follower_script_v1("monster") == FollowerScriptV1::none, "monster is not a follower");
    check(follower_script_v1("") == FollowerScriptV1::none, "empty script is not a follower");

    // Range predicates: close = (distance - reach - master radius) <= interact radius; sight = view^2 > d^2 (strict).
    check(follower_close_v1(at(250)), "close at the interaction radius edge (250-20-30=200)");
    check(!follower_close_v1(at(251)), "not close just beyond it");
    check(follower_in_sight_v1(at(1199)) && !follower_in_sight_v1(at(1200)), "sight is strict");

    { // follower: far but in sight -> approach (MoveTo), then Stop once close; Stop only while it was moving.
        FollowerMemoryV1 m;
        auto d = follower_step_v1(FollowerScriptV1::follower, m, at(600));
        check(d.action == FollowerActionV1::approach && m.moving, "follower approaches a master in sight and out of range");
        d = follower_step_v1(FollowerScriptV1::follower, m, at(500));
        check(d.action == FollowerActionV1::approach, "follower keeps approaching each turn");
        d = follower_step_v1(FollowerScriptV1::follower, m, at(240));
        check(d.action == FollowerActionV1::stop && !m.moving, "follower stops in close range");
        d = follower_step_v1(FollowerScriptV1::follower, m, at(240));
        check(d.action == FollowerActionV1::none, "follower already idle does not stop again");
    }
    { // follower: leaving the view radius raises OutOfSight once -> WarpBehind; no repeat while out of sight.
        FollowerMemoryV1 m;
        follower_step_v1(FollowerScriptV1::follower, m, at(600));
        auto d = follower_step_v1(FollowerScriptV1::follower, m, at(1500));
        check(d.action == FollowerActionV1::warp_behind && !m.moving, "follower warps behind when the master leaves sight");
        d = follower_step_v1(FollowerScriptV1::follower, m, at(1500));
        check(d.action == FollowerActionV1::none, "no second warp while the master stays out of sight");
        d = follower_step_v1(FollowerScriptV1::follower, m, at(100));
        check(d.action == FollowerActionV1::none, "close after the warp: nothing to stop");
    }
    { // A follower that never saw its master does not warp (CharAI +85 starts false, only the edge raises event 20).
        FollowerMemoryV1 m;
        auto d = follower_step_v1(FollowerScriptV1::follower, m, at(5000));
        check(d.action == FollowerActionV1::none, "never-seen master: no warp");
        d = follower_step_v1(FollowerScriptV1::attacking_follower, m, at(5000));
        check(d.action == FollowerActionV1::none, "attacking_follower: same");
    }
    { // dead master: nothing moves
        FollowerMemoryV1 m;
        auto in = at(600);
        in.master_alive = false;
        check(follower_step_v1(FollowerScriptV1::follower, m, in).action == FollowerActionV1::none, "dead master: no action");
    }
    { // rene: catch-up with haste, close range arms the 200 ms CaughtUp timer, then stop and release haste.
        FollowerMemoryV1 m;
        auto d = follower_step_v1(FollowerScriptV1::rene, m, at(600));
        check(m.catching_up && d.apply_haste && d.action == FollowerActionV1::approach, "rene starts catching up with haste");
        d = follower_step_v1(FollowerScriptV1::rene, m, at(500));
        check(!d.apply_haste && d.action == FollowerActionV1::approach, "haste is applied once while catching up continues");
        d = follower_step_v1(FollowerScriptV1::rene, m, at(240, 100));
        check(!m.far && m.caught_timer_ms > 0 && d.action == FollowerActionV1::approach, "close range arms the timer; still walking");
        d = follower_step_v1(FollowerScriptV1::rene, m, at(240, 100));
        check(m.catching_up && d.action == FollowerActionV1::approach, "timer not elapsed yet (100 of 200 ms)");
        d = follower_step_v1(FollowerScriptV1::rene, m, at(240, 150));
        check(!m.catching_up && d.release_haste && d.action == FollowerActionV1::stop && !m.moving, "CaughtUp: release haste and stop");
        d = follower_step_v1(FollowerScriptV1::rene, m, at(240));
        check(d.action == FollowerActionV1::none, "caught up and close: idle");
        d = follower_step_v1(FollowerScriptV1::rene, m, at(700));
        check(d.apply_haste && d.action == FollowerActionV1::approach, "master walks off again: catches up again");
    }
    { // rene: master out of sight also makes him catch up (and keep walking while out of sight).
        FollowerMemoryV1 m;
        follower_step_v1(FollowerScriptV1::rene, m, at(100)); // in sight and close
        check(!m.far && !m.catching_up, "close and idle");
        auto d = follower_step_v1(FollowerScriptV1::rene, m, at(2000));
        check(d.apply_haste && d.action == FollowerActionV1::approach, "out of sight: catch up");
        d = follower_step_v1(FollowerScriptV1::rene, m, at(2000));
        check(!d.apply_haste && d.action == FollowerActionV1::approach, "still out of sight: keeps walking (CatchingUp)");
    }
    { // rene: the master moving away before the timer fires does not cancel CaughtUp (the timer is not cancelled by the source).
        FollowerMemoryV1 m;
        follower_step_v1(FollowerScriptV1::rene, m, at(600));
        follower_step_v1(FollowerScriptV1::rene, m, at(240, 100));
        auto d = follower_step_v1(FollowerScriptV1::rene, m, at(900, 250));
        check(!m.catching_up && d.release_haste, "timer fires even though the master moved away");
        d = follower_step_v1(FollowerScriptV1::rene, m, at(900));
        check(d.apply_haste, "and the next turn starts a new catch-up");
    }
    { // warp-behind position: 100 units behind the master's facing, same height.
        const float master[3] = {1000, 500, 258};
        float out[3];
        follower_warp_behind_position_v1(master, 0.0f, out);
        check(std::fabs(out[0] - 900) < 1e-3f && std::fabs(out[1] - 500) < 1e-3f && out[2] == 258, "behind a master facing +X");
        follower_warp_behind_position_v1(master, 1.5707964f, out);
        check(std::fabs(out[0] - 1000) < 1e-2f && std::fabs(out[1] - 400) < 1e-2f, "behind a master facing +Y");
    }
    if (failures) { std::cerr << failures << " failure(s)\n"; return 1; }
    std::cout << "follower_decision_v1 tests passed\n";
    return 0;
}
