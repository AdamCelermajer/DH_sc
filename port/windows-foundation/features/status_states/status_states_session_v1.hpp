#pragma once
// v21.8 ES-STATUS wiring: connects the status-state owner to a live CombatSession.
//
// - Outcome consumer: CombatSession::set_status_outcome_observer receives every APPLIED source hit with its outcome bits, mask
//   and the formula sheet the roll used. Stun (0x40) and scare (0x20) go to StatusStateOwnerV1 with the F_ApplyResult tail
//   rules. Knockback (0x80) is NOT applied here: the Push consumer (features/combat/b035_session_push_effect_v1) owns it.
//   Only non-player targets take status states; the player's own stun/scare (which would also block input in the original)
//   is not wired in this round.
// - Boss gate: the target's AI row flags word (AiProps.flags; see boss_flags_from_ai_word_v1) feeds the IsBoss check. Bosses
//   are immune to stun and scare exactly as in SM_SetStunState / SM_SetScareState.
// - Clock: advance(dt_ms) once per gameplay frame expires timers (Stunned/Scared -> Idle) and re-raises active states.
// - Controller gate: can_act(actor) is false while stunned, scared, or knocked back, so RuntimeEnemyControllerV1 takes no
//   new decision for that monster (no approach, no attack request).
// The observer captures the session by reference; bind() must run again after a restore (the restore clears the handler).
#include "status_states_v1.hpp"

#include "../../combat_session.hpp"

#include <cstddef>
#include <memory>
#include <string>

namespace dh::foundation::status_states {

struct StatusStateSessionReportV1 {
    std::size_t outcomes_seen = 0;       // applied source hits delivered to the consumer
    std::size_t status_outcomes = 0;     // hits carrying stun or scare
    std::size_t stuns_requested = 0;
    std::size_t scares_requested = 0;
    std::size_t boss_refusals = 0;       // stun/scare on an IsBoss target (no state change)
    std::size_t player_skipped = 0;      // player targets: not wired this round
};

class StatusStateSessionV1 final {
public:
    // Installs the observer on the session. Idempotent for the same session; call once per frame (a restore clears it).
    bool bind(CombatSession& session, std::string& error);
    // Gameplay clock in milliseconds (call after the session update, like the timed-buff owner).
    void advance(double dt_ms);
    // Controller gate. Knockback is read from the actor action set by the Push consumer.
    bool can_act(const ActorState& actor) const;
    // Consumer body (public for tests). Does not mutate the session.
    void on_outcome(const CombatSession& session, const CombatSession::StatusOutcomeEventV1& event);

    static BossFlagsV1 boss_flags_for(const CombatSession& session, ActorId target);

    StatusStateOwnerV1& owner() noexcept { return owner_; }
    const StatusStateOwnerV1& owner() const noexcept { return owner_; }
    const StatusStateSessionReportV1& report() const noexcept { return report_; }

private:
    StatusStateOwnerV1 owner_;
    StatusStateSessionReportV1 report_;
    std::weak_ptr<const void> lease_;
    bool bound_ = false;
};

} // namespace dh::foundation::status_states
