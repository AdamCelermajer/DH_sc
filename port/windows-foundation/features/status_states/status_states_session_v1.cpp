#include "status_states_session_v1.hpp"

#include "../../../game-data/ai.hpp"
#include "../../playable_actor_world.hpp"
#include "../../retained_pose_playback.hpp"

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
    stun_generation_.clear(); // a restore replaces the actors and their poses
    scare_generation_.clear();
    return true;
}

void StatusStateSessionV1::advance(double dt_ms) {
    owner_.advance(dt_ms);
}

namespace {
StatusStateV1 status_state_from_original_v1(std::int32_t state) noexcept {
    switch (state) {
    case 1: return StatusStateV1::spawn;
    case 3: return StatusStateV1::idle;
    case 4: return StatusStateV1::move;
    case 5: return StatusStateV1::attack;
    case 6: return StatusStateV1::skill;
    case 7: return StatusStateV1::cast;
    case 8: return StatusStateV1::scared;
    case 9: return StatusStateV1::stunned;
    case 10: return StatusStateV1::knocked_back;
    case 11: return StatusStateV1::injured;
    case 13: return StatusStateV1::interact;
    case 15: return StatusStateV1::reviving;
    case 18: return StatusStateV1::lifting_idle;
    case 19: return StatusStateV1::lifting_move;
    default: return StatusStateV1::other;
    }
}
} // namespace

void StatusStateSessionV1::set_pose_service(StatusPoseServiceV1 service) {
    pose_ = std::move(service);
    owner_.set_external_state_authority(static_cast<bool>(pose_.start));
}

bool StatusStateSessionV1::end_pose(CombatSession& session, ActorId id, bool stun, const char* reason) {
    auto& generations = stun ? stun_generation_ : scare_generation_;
    const auto found = generations.find(id);
    if (found == generations.end()) return false;
    const std::uint64_t generation = found->second;
    generations.erase(found);
    bool departed = false;
    std::string error;
    // CSStunned::OnUpdate -> SM_SetIdleState / CSScared event 44 -> Idle: the admitted departure to Idle3.
    const bool ok = session.cancel_actor_source_sequence(id, session.actor_binding_lease(), generation, 3, departed, error);
    std::cout << "[status-pose] end actor=" << id << " kind=" << (stun ? "stun" : "scare") << " reason=" << reason
              << " departed=" << (departed ? 1 : 0) << " ok=" << (ok ? 1 : 0) << " stateAfter=" << session.original_actor_state(id)
              << " frame=" << session.update_serial() << (ok ? "" : (" error=" + error)) << '\n';
    if (ok) ++report_.poses_ended;
    return ok;
}

bool StatusStateSessionV1::start_pose(CombatSession& session, ActorId id, bool stun, bool forced, std::int32_t live_state) {
    const std::uint64_t generation = ++next_generation_;
    bool started = false;
    std::string error;
    const bool ok = pose_.start(session, id, stun, forced, generation, started, error);
    owner_.clear_forced(id, stun);
    std::cout << "[status-pose] start actor=" << id << " kind=" << (stun ? "stun" : "scare") << " forced=" << (forced ? 1 : 0)
              << " stateBefore=" << live_state << " ok=" << (ok ? 1 : 0) << " started=" << (started ? 1 : 0)
              << " stateAfter=" << session.original_actor_state(id) << " leftMs="
              << (stun ? owner_.stun_left_ms(id) : owner_.scare_left_ms(id)) << " frame=" << session.update_serial()
              << (ok ? "" : (" error=" + error)) << '\n';
    if (!ok) {
        ++report_.pose_failures;
        owner_.refuse_pose(id, stun);
        return false;
    }
    if (!started) {
        owner_.refuse_pose(id, stun); // no clip in the row: nothing to enter or retry
        return true;
    }
    (stun ? stun_generation_ : scare_generation_)[id] = generation;
    ++report_.poses_started;
    return true;
}

void StatusStateSessionV1::advance(CombatSession& session, double dt_ms) {
    owner_.advance(dt_ms);
    if (!pose_.start) return;
    for (const ActorId id : owner_.tracked_actors()) {
        const ActorState* actor = session.actor(id);
        if (!actor || !actor->alive() || actor->action == CharacterAction::dead) {
            // Dead (or gone): the status bits and any pose generation die with the actor.
            owner_.remove_actor(id);
            stun_generation_.erase(id);
            scare_generation_.erase(id);
            continue;
        }
        const std::int32_t live = session.original_actor_state(id);
        if ((live == 8 || live == 9) && session.update_serial() % 20 == 0) { // pose trace: the retained slot that is actually playing
            const auto* pose = session.retained_actor_pose(id);
            if (pose) {
                const auto& slot = pose->slots().at(pose->current_slot());
                std::cout << "[status-pose] trace actor=" << id << " state=" << live << " clip=" << slot.clip_id << " ms=" << slot.timeline.current_ms
                          << " frame=" << session.update_serial() << '\n';
            }
        }
        owner_.sync_state(id, status_state_from_original_v1(live));
        const std::uint32_t flags520 = actor->source_flags520 ? *actor->source_flags520 : 0u;
        // Exits first: the bit cleared while the state is still the status state (CSStunned::OnUpdate / CSScared event 44).
        if (!owner_.stunned(id) && live == 9) { end_pose(session, id, true, "stun timer end"); continue; }
        if (!owner_.scared(id) && live == 8) { end_pose(session, id, false, "scare timer end"); continue; }
        // Starts: CharStateMachine::Update re-raises 50012 then 50013 while the bit is set and the state is not the status state.
        if (owner_.stunned(id) && live != 9 && !owner_.pose_refused(id, true)) {
            const bool forced = owner_.stun_forced(id);
            if (forced || combat::source_status_event_accepted_v1(live, 50012, flags520)) {
                if (live == 8 && !end_pose(session, id, false, "stun replaces scare")) continue;
                start_pose(session, id, true, forced, live);
                continue;
            }
        }
        if (owner_.scared(id) && live != 8 && live != 9 && !owner_.pose_refused(id, false)) {
            const bool forced = owner_.scare_forced(id);
            if (forced || combat::source_status_event_accepted_v1(live, 50013, flags520)) start_pose(session, id, false, forced, live);
        }
    }
}

bool StatusStateSessionV1::can_act(const ActorState& actor) const {
    return owner_.can_act(actor.id) && actor.action != CharacterAction::knocked_back;
}

bool StatusStateSessionV1::script_status(CombatSession& session, ActorId target, bool stun, double duration_ms,
                                         bool& applied, std::string& error) {
    applied = false;
    error.clear();
    const ActorState* actor = session.world() ? session.world()->find_actor(target) : nullptr;
    if (!actor || !actor->alive() || actor->action == CharacterAction::dead) return true;
    const BossFlagsV1 flags = boss_flags_for(session, target);
    // Lua `if not IsBoss(target) and not IsMiniBoss(target)`: the miniboss refusal is a Lua-side gate (the owner refuses bosses).
    if (flags.boss || flags.mini_boss) return true;
    const bool ok = stun ? owner_.set_stun(target, duration_ms, true, false, flags, true)
                         : owner_.set_scare(target, duration_ms, true, false, flags, true);
    if (!ok) {
        error = "script status owner refused the target";
        return false;
    }
    applied = true;
    if (stun) ++report_.stuns_requested;
    else ++report_.scares_requested;
    return true;
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
    if (!traits) return;
    // w3-pose: F_ApplyResult guards the stun/scare tail with Character::IsDead only (vtable +52), so the player takes it too;
    // the player's input lock and pose come from the Stunned9 / Scared8 transition consumer.
    if (traits->is_player) ++report_.player_targets;
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
                  << " scareMs=" << (plan.scare ? plan.scare_ms : 0.0) << " boss=" << (flags.boss ? 1 : 0) << " player=" << (traits->is_player ? 1 : 0)
                  << " stunnedAfter=" << (owner_.stunned(event.target) ? 1 : 0)
                  << " scaredAfter=" << (owner_.scared(event.target) ? 1 : 0) << '\n';
    }
}

} // namespace dh::foundation::status_states
