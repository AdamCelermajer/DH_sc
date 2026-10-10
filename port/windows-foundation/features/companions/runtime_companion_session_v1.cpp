#include "runtime_companion_session_v1.hpp"

#include <algorithm>

namespace dh::foundation::companions {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}
}

bool RuntimeCompanionSessionV1::bind(
    CombatSession& session, const std::vector<RuntimeCompanionSessionActorV1>& actors,
    bool rene_follow_active, std::string& error) {
    session_ = nullptr;
    session_lease_.reset();
    follow_plan_ = {};
    error.clear();
    const auto lease = session.actor_binding_lease().lock();
    if (!lease) return fail(error, "Companion Session binding requires an initialized current CombatSession");
    if (actors.empty()) return fail(error, "Companion Session binding requires an authored Faery actor");

    RuntimeCompanionFollowPlanV1 next;
    next.session_binding_lease = lease;
    for (const auto& source : actors) {
        const auto record = std::find_if(source_companion_follow_records_v1().begin(),
            source_companion_follow_records_v1().end(), [&](const auto& candidate) {
                return candidate.source_object_name == source.source_object_name;
            });
        if (record == source_companion_follow_records_v1().end())
            return fail(error, "Companion source object is outside the authored Priest/Faery MGP records");
        if (source.source_object_name == "_prim_NPC_PriestGood" && !rene_follow_active)
            return fail(error, "Swamp Priest source activation requires the evaluated RENE_FOLLOW condition");
        auto* actor = session.actor(source.actor_id);
        const auto* traits = session.world() ? session.world()->traits(source.actor_id) : nullptr;
        const auto* pose = session.retained_actor_pose(source.actor_id);
        if (!actor || actor->definition_id != source.source_object_name ||
            source.profile_id != record->profile_id || source.ai_script != record->ai_script ||
            source.ai_row != record->ai_row || source.animation_table != record->animation_table ||
            !traits || traits->targetable || !actor->attack_ids.empty() ||
            !session.owns_population_pose(source.actor_id) || !pose) {
            return fail(error, "Companion record does not resolve to its live attackless same-session pose owner");
        }
        if (std::any_of(next.members.begin(), next.members.end(), [&](const auto& member) {
                return member.actor_id == source.actor_id || member.actor == actor;
            })) {
            return fail(error, "Companion ActorId or ActorState is duplicated in this Session binding");
        }
        next.members.push_back({&*record, source.actor_id, actor, pose});
    }
    if (!next.find("_prim_Faery"))
        return fail(error, "Default Faery is authored without a source activation gate and must remain enrolled");
    if (rene_follow_active && !next.find("_prim_NPC_PriestGood"))
        return fail(error, "Evaluated RENE_FOLLOW requires the authored Priest Session actor");
    session_ = &session;
    session_lease_ = lease;
    follow_plan_ = std::move(next);
    error.clear();
    return true;
}

bool RuntimeCompanionSessionV1::bound_to(const CombatSession& session) const noexcept {
    const auto expected = session_lease_.lock();
    const auto current = session.actor_binding_lease().lock();
    return session_ == &session && expected && current && expected == current &&
        follow_plan_.session_binding_lease == expected;
}

bool RuntimeCompanionSessionV1::validate_current_session(std::string& error) const {
    const auto expected = session_lease_.lock();
    const auto current = session_ ? session_->actor_binding_lease().lock() : nullptr;
    if (!expected || !current || expected != current ||
        follow_plan_.session_binding_lease != expected) {
        return fail(error, "Companion owner belongs to a stale or replaced CombatSession");
    }
    for (const auto& member : follow_plan_.members) {
        const auto* actor = session_->actor(member.actor_id);
        const auto* traits = session_->world() ? session_->world()->traits(member.actor_id) : nullptr;
        if (!actor || actor != member.actor || !traits || traits->targetable ||
            !actor->attack_ids.empty() || !session_->owns_population_pose(member.actor_id) ||
            session_->retained_actor_pose(member.actor_id) != member.retained_pose_owner) {
            return fail(error, "Companion ActorId or retained pose owner is stale in the current CombatSession");
        }
    }
    error.clear();
    return true;
}

bool RuntimeCompanionSessionV1::resolve_actor(
    const std::string& source_object_name, ActorId& output, std::string& error) const {
    output = invalid_actor_id;
    if (!validate_current_session(error)) return false;
    const auto* member = follow_plan_.find(source_object_name);
    if (!member) return fail(error, "Authored companion source object is not enrolled in this Session binding");
    output = member->actor_id;
    error.clear();
    return true;
}

bool RuntimeCompanionSessionV1::plan_event(
    const std::string& source_object_name, SourceFollowerEventV1 event,
    const RuntimeCompanionFollowFactsV1& facts,
    RuntimeCompanionFollowDecisionV1& output, std::string& error) const {
    output = {};
    if (!validate_current_session(error)) return false;
    const auto* member = follow_plan_.find(source_object_name);
    if (!member) return fail(error, "Authored companion source object is not enrolled in this Session binding");
    const bool needs_master = event == SourceFollowerEventV1::master_out_of_sight ||
        event == SourceFollowerEventV1::master_out_of_range ||
        event == SourceFollowerEventV1::master_in_ranged_range ||
        event == SourceFollowerEventV1::master_in_close_range ||
        event == SourceFollowerEventV1::master_in_melee_range;
    if (needs_master && (!facts.has_master || facts.master_id == invalid_actor_id))
        return fail(error, "Reached source master callback has no live Master ActorId");
    if (facts.has_master &&
        (facts.master_id == invalid_actor_id || !session_->actor(facts.master_id)))
        return fail(error, "Source Master ActorId is missing from this same CombatSession");
    if (event == SourceFollowerEventV1::friend_spotted && facts.friend_id != invalid_actor_id &&
        !session_->actor(facts.friend_id))
        return fail(error, "Source friend ActorId is missing from this same CombatSession");
    return plan_runtime_companion_follow_event_v1(
        follow_plan_, source_object_name, event, facts, output, error);
}

} // namespace dh::foundation::companions
