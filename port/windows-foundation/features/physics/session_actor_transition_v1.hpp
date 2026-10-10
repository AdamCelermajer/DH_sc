#pragma once

#include "../../combat_session.hpp"
#include "../../playable_actor_bodies.hpp"
#include "../../../level-world/navigation_objects.hpp"

#include <functional>
#include <map>
#include <memory>
#include <string>

namespace dh::foundation::enemy_ai { class RuntimeEnemyNavigationV1; }

namespace dh::foundation::physics {

// Typed source leaves needed by the physical projection. These callbacks are
// deliberately limited to cells/effects absent from ActorState or the retained
// body/PF owners. Lifecycle tails (controller, timers, FX, buffs, events and
// animation policy) remain owned by their existing callers.
struct SessionActorTransitionProvidersV1 {
    std::function<bool(ActorId, bool&, std::string&)> read_idle_suppressed538;
    std::function<bool(ActorId, std::string&)> clear_idle_suppressed538;

    // Called after modern route-only release. Reproduce the source
    // GameObject::Stop remainder (destination/heading/path fields) and return
    // the exact IsUpdatingPositionFromPhysics predicate (source flags520 bit1,
    // mask2), which gates its physical Stop. This is not CharacterAction moving.
    std::function<bool(ActorState&, dh2::navigation::NavigationObject&,
                       bool& position_from_physics, std::string&)> stop_after_route_drop;

    // Source Attack focus/blur gate and timer owners are independent of body
    // pinning. Focus performs GetAttackDelay and gate528 OR1; Blur performs the
    // exact delay/timer2a prefix. They are reached before the body lookup.
    std::function<bool(const CombatSessionActorTransition&, std::string&)> attack_focus_delay_gate;
    std::function<bool(const CombatSessionActorTransition&, std::string&)> attack_blur_delay_timer;

    // Skill6 joins the current coordinator receipt's actor+generation to its
    // actual SkillTable row and returns scalar.words[2]&0xff; it must fail if
    // that same source row/owner is unavailable.
    std::function<bool(const CombatSessionActorTransition&, bool& moving_byte,
                       std::string&)> skill_focus_moving_byte;
    // Same live Character+0x528 owner; apply exactly
    // (gate & ~0x140) | (moving_byte ? 0x100 : 0).
    std::function<bool(const CombatSessionActorTransition&, bool moving_byte,
                       std::string&)> skill_focus_gate528;
    // After modern route release and Stop, read that same gate528 bit0x100.
    // If set, schedule the actual timer10/event0x30 and return true only after
    // storage succeeds. That timer owner pins on expiry iff state3/body present.
    // If clear, return false and the source branch pins immediately.
    std::function<bool(const CombatSessionActorTransition&, bool& timer_started,
                       std::string&)> skill_blur_timer10_event48;

    // Same embedded source gate528 and controller cells. KnockBack setter
    // mutates gate bits before Focus; these typed leaves borrow the host owner.
    std::function<bool(ActorId, std::uint32_t&, std::string&)> knockback_read_gate528;
    std::function<bool(ActorId, std::uint32_t, std::string&)> knockback_write_gate528;
    std::function<bool(ActorId, bool, std::string&)> knockback_controller_lock;
    // Source LookAt(attacker) followed by CancelSneaking, after controller lock.
    std::function<bool(const CombatSessionActorTransition&, std::string&)> knockback_look_at_cancel_sneaking;

    // Actual source IsPlayer fact controls the additional Dead bit.
    std::function<bool(ActorId, bool&, std::string&)> source_is_player;
    // The original physical filter arguments/predicate are not represented by
    // the generic body API. Invoke only when the same current body is present.
    std::function<bool(ActorId, std::string&)> dead_focus_physical_filter;
    std::function<bool(ActorId, std::string&)> dead_blur_reset_filter;
};

struct SessionActorTransitionConsumerConfigV1 {
    PlayableActorBodies* bodies = nullptr;
    enemy_ai::RuntimeEnemyNavigationV1* navigation = nullptr;
    SessionActorTransitionProvidersV1 source;
};

// Feature-local physical projection for the current CombatSession. It borrows
// Session actor/PF/body owners and owns no state graph, animation selection,
// actor registry, or physics world. Keep one shared instance alive through
// Session teardown; the Session callback itself retains the consumer.
class SessionActorTransitionConsumerV1 final :
    public std::enable_shared_from_this<SessionActorTransitionConsumerV1> {
public:
    explicit SessionActorTransitionConsumerV1(SessionActorTransitionConsumerConfigV1);
    bool bind(CombatSession&, std::string& error);

private:
    struct Pending {
        CombatSessionActorTransition receipt;
        CombatSessionTransitionStage next = CombatSessionTransitionStage::focus_prefix;
    };
    bool consume(CombatSession&, const CombatSessionActorTransition&, std::string&);
    bool blur(CombatSession&, const CombatSessionActorTransition&, ActorState&, std::string&);
    bool focus_prefix(CombatSession&, const CombatSessionActorTransition&, ActorState&, std::string&);
    bool focus_suffix(CombatSession&, const CombatSessionActorTransition&, ActorState&, std::string&);
    bool body_present(CombatSession&, ActorId, bool&, std::string&);
    bool set_body_pinned(CombatSession&, ActorId, bool, std::string&);
    bool validate_current(CombatSession&, const CombatSessionActorTransition&,
                          std::int32_t expected_state, ActorState*&, std::string&) const;

    SessionActorTransitionConsumerConfigV1 config_;
    CombatSession* session_ = nullptr;
    std::weak_ptr<const void> actor_lease_;
    std::weak_ptr<const CombatSessionLifetime> lifetime_lease_;
    std::map<ActorId, Pending> pending_;
    bool delivering_ = false;
};

std::shared_ptr<SessionActorTransitionConsumerV1> make_session_actor_transition_consumer_v1(
    SessionActorTransitionConsumerConfigV1);

} // namespace dh::foundation::physics
