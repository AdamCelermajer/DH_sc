#include "status_states_v1.hpp"

#include <cmath>
#include <limits>

namespace dh::foundation::status_states {
namespace {

constexpr double kNeverExpires = std::numeric_limits<double>::infinity();

const char* state_name(StatusStateV1 state) noexcept {
    switch (state) {
    case StatusStateV1::spawn: return "Spawn";
    case StatusStateV1::idle: return "Idle";
    case StatusStateV1::move: return "Move";
    case StatusStateV1::attack: return "Attack";
    case StatusStateV1::skill: return "Skill";
    case StatusStateV1::cast: return "Cast";
    case StatusStateV1::scared: return "Scared";
    case StatusStateV1::stunned: return "Stunned";
    case StatusStateV1::knocked_back: return "KnockedBack";
    case StatusStateV1::injured: return "Injured";
    case StatusStateV1::interact: return "Interact";
    case StatusStateV1::reviving: return "Reviving";
    case StatusStateV1::lifting_idle: return "LiftingIdle";
    case StatusStateV1::lifting_move: return "LiftingMove";
    case StatusStateV1::other: return "Other";
    }
    return "Other";
}

// TMR interval: a non-positive duration never fires in CharTimers::Update (the loop breaks on !interval).
double timer_from_duration(double duration_ms) noexcept {
    return duration_ms > 0.0 ? duration_ms : kNeverExpires;
}

} // namespace

bool accepts_event_v1(StatusStateV1 state, std::int32_t event) noexcept {
    switch (event) {
    case kEventKnockBackV1: // CSSpawn, CSIdle, CSMove, CSAttack, CSSkill, CSScared, CSStunned, CSInjured, CSInteract,
                            // CSReviving, CSLiftingIdle, CSLiftingMove. Cast, Dead, KnockedBack do not register it.
        switch (state) {
        case StatusStateV1::spawn: case StatusStateV1::idle: case StatusStateV1::move: case StatusStateV1::attack:
        case StatusStateV1::skill: case StatusStateV1::scared: case StatusStateV1::stunned: case StatusStateV1::injured:
        case StatusStateV1::interact: case StatusStateV1::reviving: case StatusStateV1::lifting_idle:
        case StatusStateV1::lifting_move:
            return true;
        default:
            return false;
        }
    case kEventStunV1: // CSSpawn, CSIdle, CSMove, CSAttack, CSSkill, CSScared, CSInteract, CSReviving, CSLifting*. CSStunned does not.
        switch (state) {
        case StatusStateV1::spawn: case StatusStateV1::idle: case StatusStateV1::move: case StatusStateV1::attack:
        case StatusStateV1::skill: case StatusStateV1::scared: case StatusStateV1::interact: case StatusStateV1::reviving:
        case StatusStateV1::lifting_idle: case StatusStateV1::lifting_move:
            return true;
        default:
            return false;
        }
    case kEventScareV1: // CSIdle, CSMove, CSAttack, CSSkill, CSInteract, CSReviving, CSLifting*. CSScared does not.
        switch (state) {
        case StatusStateV1::idle: case StatusStateV1::move: case StatusStateV1::attack: case StatusStateV1::skill:
        case StatusStateV1::interact: case StatusStateV1::reviving: case StatusStateV1::lifting_idle:
        case StatusStateV1::lifting_move:
            return true;
        default:
            return false;
        }
    default:
        return false;
    }
}

BossFlagsV1 boss_flags_from_ai_word_v1(std::uint32_t word) noexcept {
    BossFlagsV1 flags;
    flags.boss = ((word >> 2) & 1u) != 0u;
    flags.mini_boss = ((word >> 1) & 1u) != 0u;
    return flags;
}

StatusOutcomePlanV1 status_outcome_plan_v1(std::uint32_t outcomes, std::uint32_t mask,
                                           const std::function<std::int32_t(int)>& attacker_property) {
    StatusOutcomePlanV1 plan;
    const bool immediate = (mask & 0x18000000u) != 0u;
    if ((outcomes & 0x80u) != 0u) {
        plan.knockback = true;
        plan.knockback_great = (mask & 0x100000u) != 0u;
        plan.knockback_immediate = immediate;
    }
    if ((outcomes & 0x40u) != 0u) {
        // Property 185 (mask 0x1000) or 140, raw x256 -> milliseconds. Not zero-checked in F_ApplyResult.
        const int id = (mask & 0x1000u) != 0u ? 185 : 140;
        plan.stun = true;
        plan.stun_ms = static_cast<double>(attacker_property(id) >> 8);
    }
    if ((outcomes & 0x20u) != 0u) {
        // Property 187 (mask 0x4000) or 143; the scare call is skipped for a zero duration.
        const int id = (mask & 0x4000u) != 0u ? 187 : 143;
        const std::int32_t ms = attacker_property(id) >> 8;
        if (ms != 0) {
            plan.scare = true;
            plan.scare_ms = static_cast<double>(ms);
            plan.scare_immediate = immediate;
        }
    }
    return plan;
}

StatusActorV1& StatusStateOwnerV1::actor(ActorId id) {
    return actors_[id];
}

void StatusStateOwnerV1::enter(StatusActorV1& a, StatusStateV1 next, const char* reason, ActorId id) {
    if (a.state == next) return;
    log_.push_back("actor " + std::to_string(id) + ": " + state_name(a.state) + " -> " + state_name(next) + " (" + reason + ")");
    a.state = next;
}

bool StatusStateOwnerV1::set_stun(ActorId id, double duration_ms, bool persist, bool immediate,
                                  BossFlagsV1 flags, bool anim_ok) {
    // SM_SetStunState: IsBoss returns first (mini-boss is a Lua-side gate only); then the animation row.
    if (flags.boss || !anim_ok) return false;
    StatusActorV1& a = actor(id);
    if (!a.stun_bit) {
        a.stun_bit = true;
        a.stun_left_ms = timer_from_duration(duration_ms);
    }
    if (persist) a.stun_persist = true;
    if (external_state_) {
        if (immediate) a.stun_forced = true;
        return true;
    }
    if (immediate) enter(a, StatusStateV1::stunned, "SetStunState immediate", id);
    else if (accepts_event_v1(a.state, kEventStunV1)) enter(a, StatusStateV1::stunned, "SetStunState event 50012", id);
    return true;
}

bool StatusStateOwnerV1::set_scare(ActorId id, double duration_ms, bool persist, bool immediate,
                                   BossFlagsV1 flags, bool anim_ok) {
    if (flags.boss || !anim_ok) return false;
    StatusActorV1& a = actor(id);
    if (!a.scare_bit) {
        a.scare_bit = true;
        a.scare_left_ms = timer_from_duration(duration_ms);
    }
    if (persist) a.scare_persist = true;
    if (external_state_) {
        if (immediate) a.scare_forced = true;
        return true;
    }
    if (immediate) enter(a, StatusStateV1::scared, "SetScareState immediate", id);
    else if (accepts_event_v1(a.state, kEventScareV1)) enter(a, StatusStateV1::scared, "SetScareState event 50013", id);
    return true;
}

bool StatusStateOwnerV1::set_knockback(ActorId id, bool great, bool immediate, BossFlagsV1 flags, bool anim_ok) {
    if (flags.boss || !anim_ok) return false;
    StatusActorV1& a = actor(id);
    a.knockback_great = great;
    if (immediate) enter(a, StatusStateV1::knocked_back, "SetKnockBackState immediate", id);
    else if (accepts_event_v1(a.state, kEventKnockBackV1)) enter(a, StatusStateV1::knocked_back, "SetKnockBackState event 50011", id);
    return true;
}

bool StatusStateOwnerV1::apply_outcome(ActorId id, const StatusOutcomePlanV1& plan, BossFlagsV1 flags, bool anim_ok) {
    // Native order in F_ApplyResult: knockback (0x80), stun (0x40), scare (0x20). Boss immunity is inside each call.
    if (plan.knockback) set_knockback(id, plan.knockback_great, plan.knockback_immediate, flags, anim_ok);
    if (plan.stun) set_stun(id, plan.stun_ms, true, false, flags, anim_ok);
    if (plan.scare) set_scare(id, plan.scare_ms, true, plan.scare_immediate, flags, anim_ok);
    return true;
}

void StatusStateOwnerV1::advance(double dt_ms) {
    for (auto& [id, a] : actors_) {
        // CharTimers expiry raises event 43 / 44: the bit clears; CSScared registers 44 -> Idle (IDA). Stunned has no
        // 43 registration in the OnInit tables, so the port returns Stunned to Idle here (INFERENCE, see header).
        if (a.stun_bit && std::isfinite(a.stun_left_ms)) {
            a.stun_left_ms -= dt_ms;
            if (a.stun_left_ms <= 0.0) {
                a.stun_bit = false;
                a.stun_forced = false;
                a.stun_pose_refused = false;
                a.stun_left_ms = 0.0;
                if (!external_state_ && a.state == StatusStateV1::stunned) enter(a, StatusStateV1::idle, "stun timer end (INFERENCE)", id);
            }
        }
        if (a.scare_bit && std::isfinite(a.scare_left_ms)) {
            a.scare_left_ms -= dt_ms;
            if (a.scare_left_ms <= 0.0) {
                a.scare_bit = false;
                a.scare_forced = false;
                a.scare_pose_refused = false;
                a.scare_left_ms = 0.0;
                if (!external_state_ && a.state == StatusStateV1::scared) enter(a, StatusStateV1::idle, "scare timer end (event 44)", id);
            }
        }
        // CharStateMachine::Update re-raise while the bit is still set and the state left the status state.
        if (!external_state_ && a.stun_bit && a.state != StatusStateV1::stunned && accepts_event_v1(a.state, kEventStunV1))
            enter(a, StatusStateV1::stunned, "Update re-raise of active stun", id);
        if (!external_state_ && a.scare_bit && a.state != StatusStateV1::scared && accepts_event_v1(a.state, kEventScareV1))
            enter(a, StatusStateV1::scared, "Update re-raise of active scare", id);
    }
}

bool StatusStateOwnerV1::finish_knockback(ActorId id) {
    const auto found = actors_.find(id);
    if (found == actors_.end() || found->second.state != StatusStateV1::knocked_back) return false;
    enter(found->second, StatusStateV1::idle, "knockback clip end", id);
    return true;
}

void StatusStateOwnerV1::remove_actor(ActorId id) {
    actors_.erase(id);
}

void StatusStateOwnerV1::sync_state(ActorId id, StatusStateV1 next) {
    const auto found = actors_.find(id);
    if (found != actors_.end()) found->second.state = next;
}

std::vector<ActorId> StatusStateOwnerV1::tracked_actors() const {
    std::vector<ActorId> ids;
    ids.reserve(actors_.size());
    for (const auto& entry : actors_) ids.push_back(entry.first);
    return ids;
}

bool StatusStateOwnerV1::stun_forced(ActorId id) const {
    const auto found = actors_.find(id);
    return found != actors_.end() && found->second.stun_forced;
}

bool StatusStateOwnerV1::scare_forced(ActorId id) const {
    const auto found = actors_.find(id);
    return found != actors_.end() && found->second.scare_forced;
}

void StatusStateOwnerV1::clear_forced(ActorId id, bool stun) {
    const auto found = actors_.find(id);
    if (found == actors_.end()) return;
    (stun ? found->second.stun_forced : found->second.scare_forced) = false;
}

void StatusStateOwnerV1::refuse_pose(ActorId id, bool stun) {
    const auto found = actors_.find(id);
    if (found == actors_.end()) return;
    (stun ? found->second.stun_pose_refused : found->second.scare_pose_refused) = true;
}

bool StatusStateOwnerV1::pose_refused(ActorId id, bool stun) const {
    const auto found = actors_.find(id);
    return found != actors_.end() && (stun ? found->second.stun_pose_refused : found->second.scare_pose_refused);
}

bool StatusStateOwnerV1::can_act(ActorId id) const {
    const auto found = actors_.find(id);
    if (found == actors_.end()) return true;
    const StatusActorV1& a = found->second;
    return !a.stun_bit && !a.scare_bit && a.state != StatusStateV1::knocked_back && a.state != StatusStateV1::stunned &&
           a.state != StatusStateV1::scared;
}

StatusStateV1 StatusStateOwnerV1::state(ActorId id) const {
    const auto found = actors_.find(id);
    return found == actors_.end() ? StatusStateV1::idle : found->second.state;
}

bool StatusStateOwnerV1::stunned(ActorId id) const {
    const auto found = actors_.find(id);
    return found != actors_.end() && found->second.stun_bit;
}

bool StatusStateOwnerV1::scared(ActorId id) const {
    const auto found = actors_.find(id);
    return found != actors_.end() && found->second.scare_bit;
}

double StatusStateOwnerV1::stun_left_ms(ActorId id) const {
    const auto found = actors_.find(id);
    return found != actors_.end() && found->second.stun_bit ? found->second.stun_left_ms : 0.0;
}

double StatusStateOwnerV1::scare_left_ms(ActorId id) const {
    const auto found = actors_.find(id);
    return found != actors_.end() && found->second.scare_bit ? found->second.scare_left_ms : 0.0;
}

std::vector<std::string> StatusStateOwnerV1::take_log() {
    std::vector<std::string> out;
    out.swap(log_);
    return out;
}

StatusOutcomePlanV1 apply_status_outcome_without_knockback_v1(StatusStateOwnerV1& owner, ActorId target,
    std::uint32_t outcomes, std::uint32_t mask,
    const std::function<std::int32_t(int)>& attacker_property, BossFlagsV1 flags) {
    StatusOutcomePlanV1 plan = status_outcome_plan_v1(outcomes, mask, attacker_property);
    plan.knockback = false;
    owner.apply_outcome(target, plan, flags, true);
    return plan;
}

} // namespace dh::foundation::status_states
