#include "runtime_boss_encounter_session_v1.hpp"

#include <cmath>

namespace dh2::bosses::runtime_v1 {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}
}

bool SwampKingSessionConsumerV1::bind(CombatSession& session, ActorId actor,
                                      std::string& error) {
    const auto lifetime = session.lifetime_lease().lock();
    const auto actorLease = session.actor_binding_lease().lock();
    const auto* currentActor = session.actor(actor);
    if (!lifetime || !lifetime->alive() || !actorLease || !currentActor ||
        currentActor->definition_id != "SwampKing")
        return fail(error, "Swamp King consumer requires the retained SwampKing actor in a live CombatSession");
    session_ = &session;
    actor_id_ = actor;
    session_lifetime_ = lifetime;
    actor_lease_ = actorLease;
    status_ = {};
    status_.phase = Phase::phase1;
    status_.last_session_frame = session.update_serial();
    remainder_ms_ = 0.0;
    error.clear();
    return true;
}

bool SwampKingSessionConsumerV1::validate_session(CombatSession& session,
                                                  std::string& error) const {
    const auto lifetime = session_lifetime_.lock();
    const auto actorLease = actor_lease_.lock();
    const auto currentLease = session.actor_binding_lease().lock();
    const auto* currentActor = session.actor(actor_id_);
    if (!session_ || &session != session_ || !lifetime || !lifetime->alive() ||
        !actorLease || actorLease != currentLease || !currentActor ||
        currentActor->definition_id != "SwampKing")
        return fail(error, "Swamp King consumer lost its same-Session actor binding");
    error.clear();
    return true;
}

bool SwampKingSessionConsumerV1::on_frame_begin(
    CombatSession& session, double dt, const SessionFrameFactsV1& facts,
    std::string& error) {
    if (!validate_session(session, error)) return false;
    if (facts.source_actor != actor_id_)
        return fail(error, "Swamp King source facts belong to a different Session actor");
    if (!std::isfinite(dt) || dt < 0.0 || !std::isfinite(facts.source_hp_percent) ||
        facts.source_hp_percent < 0.0f || facts.source_hp_percent > 100.0f)
        return fail(error, "Swamp King frame requires a finite Session dt and source HP percentage in [0,100]");
    const auto frame = session.update_serial();
    if (!frame || frame <= status_.last_session_frame ||
        (status_.last_session_frame && frame != status_.last_session_frame + 1))
        return fail(error, "Swamp King timer must consume each new CombatSession frame exactly once");
    status_.last_session_frame = frame;

    const auto nextPhase = phase_after_update(status_.phase, facts.source_hp_percent);
    if (nextPhase != status_.phase) {
        status_.phase = nextPhase;
        // swampking_OnUpdate clears attack_flag on the one-way phase switch.
        status_.attack_ready = false;
    }

    if (!status_.timers_started) {
        if (facts.encounter_admitted && facts.source_native_ai_idle)
            status_.timers_started = true;
        return true;
    }

    remainder_ms_ += dt * 1000.0;
    const auto interval = static_cast<double>(swamp_king_plan().attack_ready_timer_ms);
    while (remainder_ms_ >= interval) {
        remainder_ms_ -= interval;
        // isAttackReady calls LookAtTarget only as a side effect when one is
        // present; target presence is not part of its readiness condition.
        status_.attack_ready = facts.source_script_idle &&
            !facts.source_native_ai_skill && !facts.can_dive_now;
        ++status_.timer_callbacks;
    }
    status_.attack_timer_elapsed_ms = remainder_ms_;
    error.clear();
    return true;
}

} // namespace dh2::bosses::runtime_v1
