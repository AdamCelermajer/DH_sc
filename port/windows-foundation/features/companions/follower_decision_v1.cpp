#include "follower_decision_v1.hpp"

#include <cmath>

namespace dh::foundation::companions {

FollowerScriptV1 follower_script_v1(std::string_view ai_script) noexcept {
    if (ai_script == "follower") return FollowerScriptV1::follower;
    if (ai_script == "attacking_follower") return FollowerScriptV1::attacking_follower;
    if (ai_script == "rene") return FollowerScriptV1::rene;
    return FollowerScriptV1::none;
}

bool follower_in_sight_v1(const FollowerInputV1& in) noexcept {
    // AI_IsInSight(float): ViewRadius^2 > squared distance (strict).
    return in.radii.view_radius * in.radii.view_radius > in.distance * in.distance;
}

bool follower_close_v1(const FollowerInputV1& in) noexcept {
    // AI_IsInInteractionRange: (distance - own melee radius - master radius) <= InteractRadius.
    return (in.distance - in.radii.own_reach) - in.radii.master_radius <= in.radii.interact_radius;
}

namespace {
constexpr double caught_up_delay_ms = 200.0; // rene.luac StartTimerCB(200, false, CaughtUp)

FollowerDecisionV1 step_follower(FollowerMemoryV1& m, const FollowerInputV1& in) noexcept {
    FollowerDecisionV1 d;
    const bool sight = follower_in_sight_v1(in);
    const bool was = m.prev_sight;
    m.prev_sight = sight;
    if (!in.master_alive) return d;
    if (was && !sight) { // event 20: OnMasterOutOfSight -> WarpBehind(g_master)
        m.moving = false;
        d.action = FollowerActionV1::warp_behind;
        return d;
    }
    if (!sight) return d; // no event is raised while the master stays out of sight
    if (follower_close_v1(in)) { // events 24/25: Stop()
        if (m.moving) { m.moving = false; d.action = FollowerActionV1::stop; }
        return d;
    }
    // event 22: OnMasterOutOfRange -> MoveTo(g_master) (continued each frame by the route owner)
    m.moving = true;
    d.action = FollowerActionV1::approach;
    return d;
}

FollowerDecisionV1 step_rene(FollowerMemoryV1& m, const FollowerInputV1& in) noexcept {
    FollowerDecisionV1 d;
    const bool sight = follower_in_sight_v1(in);
    const bool was = m.prev_sight;
    m.prev_sight = sight;
    if (!in.master_alive) return d;
    // Edge events: OnMasterOutOfSight / OnMasterInSight both flag "far" unless already catching up.
    if ((was && !sight) || (!was && sight)) { if (!m.catching_up) m.far = true; }
    bool armed_now = false;
    if (sight) {
        if (follower_close_v1(in)) { // OnMasterInCloseRange
            m.far = false;
            if (m.catching_up && m.caught_timer_ms < 0) { m.caught_timer_ms = caught_up_delay_ms; armed_now = true; }
        } else if (!m.catching_up) { // OnMasterInRangedRange (no target tracking here: see report)
            m.far = true;
        }
    }
    if (m.caught_timer_ms >= 0 && !armed_now) { // timer callback CaughtUp (fires 200 ms after it was started)
        m.caught_timer_ms -= in.dt_ms;
        if (m.caught_timer_ms <= 0) {
            m.caught_timer_ms = -1;
            if (m.catching_up) {
                m.catching_up = false;
                m.moving = false;
                d.release_haste = true;
                d.action = FollowerActionV1::stop;
                return d;
            }
        }
    }
    // rene_OnUpdate
    if (m.catching_up) { // CatchingUp(): MoveTo(g_master)
        m.moving = true;
        d.action = FollowerActionV1::approach;
    } else if (m.far) { // CatchUp(): haste buff, collisions off, MoveTo(g_master)
        m.catching_up = true;
        m.moving = true;
        d.apply_haste = true;
        d.action = FollowerActionV1::approach;
    }
    return d;
}
} // namespace

FollowerDecisionV1 follower_step_v1(FollowerScriptV1 script, FollowerMemoryV1& memory, const FollowerInputV1& input) noexcept {
    switch (script) {
    case FollowerScriptV1::follower:
    case FollowerScriptV1::attacking_follower: return step_follower(memory, input);
    case FollowerScriptV1::rene: return step_rene(memory, input);
    default: return {};
    }
}

void follower_warp_behind_position_v1(const float master_position[3], float master_heading, float out[3]) noexcept {
    out[0] = master_position[0] - std::cos(master_heading) * 100.0f;
    out[1] = master_position[1] - std::sin(master_heading) * 100.0f;
    out[2] = master_position[2];
}

} // namespace dh::foundation::companions
