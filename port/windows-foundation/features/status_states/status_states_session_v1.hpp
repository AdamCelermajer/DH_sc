#pragma once
// v21.8 ES-STATUS wiring: connects the status-state owner to a live CombatSession.
//
// - Outcome consumer: CombatSession::set_status_outcome_observer receives every APPLIED source hit with its outcome bits, mask
//   and the formula sheet the roll used. Stun (0x40) and scare (0x20) go to StatusStateOwnerV1 with the F_ApplyResult tail
//   rules. Knockback (0x80) is NOT applied here: the Push consumer (features/combat/b035_session_push_effect_v1) owns it.
//   w3-pose: the player takes status states as well (F_ApplyResult guards the tail with IsDead, not IsPlayer). A stunned player is
//   input-locked by the Stunned9 transition consumer (CSStunned::OnFocus sets the controller lock byte, OnBlur clears it).
// - Pose (w3-pose): with a pose service installed the owner runs with external state authority. advance(session, dt) mirrors the
//   REAL Session original state into the owner, starts the Stunned9 / Scared8 pose when the bit is set and the current state
//   accepts event 50012 / 50013 (the original CharStateMachine::Update re-raise, which also covers a stun that arrived while the
//   target was Injured), and returns the actor to Idle when the bit clears (CSStunned::OnUpdate 0x3c549c, CSScared event 44).
// - Boss gate: the target's AI row flags word (AiProps.flags; see boss_flags_from_ai_word_v1) feeds the IsBoss check. Bosses
//   are immune to stun and scare exactly as in SM_SetStunState / SM_SetScareState.
// - Clock: advance(dt_ms) once per gameplay frame expires timers (Stunned/Scared -> Idle) and re-raises active states.
// - Controller gate: can_act(actor) is false while stunned, scared, or knocked back, so RuntimeEnemyControllerV1 takes no
//   new decision for that monster (no approach, no attack request).
// The observer captures the session by reference; bind() must run again after a restore (the restore clears the handler).
#include "status_states_v1.hpp"

#include "../../combat_session.hpp"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>

namespace dh::foundation::status_states {

struct StatusStateSessionReportV1 {
    std::size_t outcomes_seen = 0;       // applied source hits delivered to the consumer
    std::size_t status_outcomes = 0;     // hits carrying stun or scare
    std::size_t stuns_requested = 0;
    std::size_t scares_requested = 0;
    std::size_t boss_refusals = 0;       // stun/scare on an IsBoss target (no state change)
    std::size_t player_targets = 0;      // status outcomes that landed on the player
    std::size_t poses_started = 0;       // Stunned9 / Scared8 poses started through the Session
    std::size_t poses_ended = 0;         // poses ended because the status bit cleared
    std::size_t pose_failures = 0;       // pose start errors (the owner stops re-raising for that actor)
};

// Host-owned pose provider (banks per monster profile / player stance live in the host). `started` = false means the actor's
// CharAnimTable row has no clip for the kind (a source no-op: the owner keeps the bit and can_act but plays nothing).
struct StatusPoseServiceV1 {
    std::function<bool(CombatSession&, ActorId, bool stun, bool forced, std::uint64_t generation, bool& started,
                       std::string&)> start;
};

class StatusStateSessionV1 final {
public:
    // Installs the observer on the session. Idempotent for the same session; call once per frame (a restore clears it).
    bool bind(CombatSession& session, std::string& error);
    // Gameplay clock in milliseconds (call after the session update, like the timed-buff owner).
    void advance(double dt_ms);
    // w3-pose: timers, then the pose reconciliation against the real Session state (see the header comment). Call after the
    // session update, once per gameplay frame.
    void advance(CombatSession& session, double dt_ms);
    // Installs the pose provider and switches the owner to external state authority.
    void set_pose_service(StatusPoseServiceV1 service);
    // Controller gate. Knockback is read from the actor action set by the Push consumer.
    bool can_act(const ActorState& actor) const;
    // Consumer body (public for tests). Does not mutate the session.
    void on_outcome(const CombatSession& session, const CombatSession::StatusOutcomeEventV1& event);
    // v21.8 berserker: Lua target:SetStunState(duration, true) / SetScareState(duration, true) from a skill Use (Warcry).
    // Bosses and minibosses are refused (applied=false, returns true). Persist = the Lua `true`, event path (50012/50013).
    bool script_status(CombatSession& session, ActorId target, bool stun, double duration_ms, bool& applied, std::string& error);

    static BossFlagsV1 boss_flags_for(const CombatSession& session, ActorId target);

    StatusStateOwnerV1& owner() noexcept { return owner_; }
    const StatusStateOwnerV1& owner() const noexcept { return owner_; }
    const StatusStateSessionReportV1& report() const noexcept { return report_; }

private:
    StatusStateOwnerV1 owner_;
    StatusStateSessionReportV1 report_;
    std::weak_ptr<const void> lease_;
    bool bound_ = false;
    StatusPoseServiceV1 pose_;
    std::map<ActorId, std::uint64_t> stun_generation_;
    std::map<ActorId, std::uint64_t> scare_generation_;
    std::uint64_t next_generation_ = 0x5747000000000000ull;
    bool end_pose(CombatSession& session, ActorId id, bool stun, const char* reason);
    bool start_pose(CombatSession& session, ActorId id, bool stun, bool forced, std::int32_t live_state);
};

} // namespace dh::foundation::status_states
