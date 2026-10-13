#include "status_states_session_v1.hpp"

#include "../../../game-data/ai.hpp"
#include "../../playable_actor_world.hpp"

#include <iostream>

namespace dh::foundation::status_states {

BossFlagsV1 StatusStateSessionV1::boss_flags_for(const CombatSession& session, ActorId target) {
    const auto* world = session.world();
    if (!world) return {};
    const auto* props = world->combat_properties(target);
    const auto* tables = session.original_ai_tables();
    if (!props || !tables) return {};
    const auto* ai = dh2::data::ai_props(*tables, props->sheets.resolved[1]);
    return ai ? boss_flags_from_ai_word_v1(ai->flags) : BossFlagsV1{};
}

bool StatusStateSessionV1::bind(CombatSession& session, std::string& error) {
    error.clear();
    const auto lease = session.actor_binding_lease();
    if (lease.expired()) {
        error = "Status states require a live CombatSession actor lease";
        return false;
    }
    if (bound_ && !lease.owner_before(lease_) && !lease_.owner_before(lease)) return true;
    CombatSession* target = &session;
    session.set_status_outcome_observer([this, target](const CombatSession::StatusOutcomeEventV1& event) {
        on_outcome(*target, event);
    });
    lease_ = lease;
    bound_ = true;
    return true;
}

void StatusStateSessionV1::advance(double dt_ms) {
    owner_.advance(dt_ms);
}

bool StatusStateSessionV1::can_act(const ActorState& actor) const {
    return owner_.can_act(actor.id) && actor.action != CharacterAction::knocked_back;
}

void StatusStateSessionV1::on_outcome(const CombatSession& session, const CombatSession::StatusOutcomeEventV1& event) {
    ++report_.outcomes_seen;
    if ((event.outcomes & 0x60u) == 0) return;          // neither stun (0x40) nor scare (0x20)
    ++report_.status_outcomes;
    const auto* world = session.world();
    if (!world) return;
    const ActorState* target = world->find_actor(event.target);
    if (!target || !target->alive() || target->action == CharacterAction::dead) return;
    const auto* traits = world->traits(event.target);
    if (!traits || traits->is_player) {
        ++report_.player_skipped;                         // the player's own status states are not wired this round
        return;
    }
    // The roll's formula sheet (class-projected) supplies the stun/scare durations; fall back to the resolved sheet.
    const dh2::data::PropertySheet* sheet = event.attacker_sheet;
    if (!sheet) {
        const auto* attacker = world->combat_properties(event.attacker);
        if (!attacker) return;
        sheet = &attacker->sheets.resolved;
    }
    const BossFlagsV1 flags = boss_flags_for(session, event.target);
    const StatusOutcomePlanV1 plan = apply_status_outcome_without_knockback_v1(owner_, event.target,
        event.outcomes, event.mask, [sheet](int id) { return (*sheet)[static_cast<std::size_t>(id)]; }, flags);
    if (plan.stun) ++report_.stuns_requested;
    if (plan.scare) ++report_.scares_requested;
    if (flags.boss && (plan.stun || plan.scare)) ++report_.boss_refusals;
    if (plan.stun || plan.scare) {
        std::cout << "[status] outcome attacker=" << event.attacker << " target=" << event.target << " outcomes=0x" << std::hex
                  << event.outcomes << " mask=0x" << event.mask << std::dec << " stunMs=" << (plan.stun ? plan.stun_ms : 0.0)
                  << " scareMs=" << (plan.scare ? plan.scare_ms : 0.0) << " boss=" << (flags.boss ? 1 : 0)
                  << " stunnedAfter=" << (owner_.stunned(event.target) ? 1 : 0)
                  << " scaredAfter=" << (owner_.scared(event.target) ? 1 : 0) << '\n';
    }
}

} // namespace dh::foundation::status_states
