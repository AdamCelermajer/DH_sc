#include "runtime_companion_follow_v1.hpp"

#include <algorithm>
#include <stdexcept>

namespace dh::foundation::companions {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

void append(RuntimeCompanionFollowDecisionV1& decision, SourceFollowerOperationV1 operation,
            ActorId argument = invalid_actor_id) {
    if (decision.operation_count < decision.operations.size()) {
        const auto index = decision.operation_count++;
        decision.operations[index] = operation;
        decision.arguments[index] = argument;
    }
}
} // namespace

const std::array<SourceCompanionFollowRecordV1, 2>&
source_companion_follow_records_v1() noexcept {
    static const std::array<SourceCompanionFollowRecordV1, 2> records{{
        {"_prim_NPC_PriestGood", "WanderingPriest", 50, "rene", 47,
         {-1, -1, -1, -1}, {793.9f, -617.658f, 272.064f}},
        {"_prim_Faery", "DefaultFairy", 20, "follower", 23,
         {153600, 153600, -1, -1}, {544.191f, 414.689f, -108.175f}},
    }};
    return records;
}

const RuntimeCompanionFollowMemberV1* RuntimeCompanionFollowPlanV1::find(
    const std::string& source_object_name) const noexcept {
    const auto found = std::find_if(members.begin(), members.end(),
        [&](const auto& member) {
            return member.source && member.source->source_object_name == std::string_view(source_object_name);
        });
    return found == members.end() ? nullptr : &*found;
}

bool build_runtime_companion_follow_plan_v1(
    const std::vector<RuntimeCompanionFollowActorBorrowV1>& actors,
    RuntimeCompanionFollowPlanV1& output, std::string& error) {
    try {
        const auto& records = source_companion_follow_records_v1();
        if (actors.size() != records.size())
            throw std::runtime_error("Runtime companion plan requires the exact Priest and Faery source records");

        RuntimeCompanionFollowPlanV1 next;
        std::vector<bool> matched(records.size(), false);
        for (const auto& actor : actors) {
            if (actor.source_object_name.empty() || actor.profile_id.empty() || !actor.actor ||
                actor.actor->id == invalid_actor_id || actor.actor->id == actor.actor->target_id ||
                !actor.retained_pose_owner || !actor.session_binding_lease ||
                !actor.animation_only_session_owner || !actor.actor->attack_ids.empty()) {
                throw std::runtime_error("Companion borrow must be a live attackless animationOnly actor with retained pose and Session lease");
            }
            const auto source = std::find_if(records.begin(), records.end(), [&](const auto& record) {
                return record.source_object_name == actor.source_object_name;
            });
            if (source == records.end())
                throw std::runtime_error("Companion source object is outside the audited Priest/Faery records");
            const auto source_index = static_cast<std::size_t>(source - records.begin());
            if (matched[source_index] || actor.profile_id != source->profile_id ||
                actor.ai_script != source->ai_script || actor.ai_row != source->ai_row ||
                actor.animation_table != source->animation_table) {
                throw std::runtime_error("Companion source name/profile/AI/animation table identity differs");
            }
            if (next.session_binding_lease &&
                next.session_binding_lease != actor.session_binding_lease) {
                throw std::runtime_error("Companion actors must borrow from the same CombatSession lease");
            }
            if (std::any_of(next.members.begin(), next.members.end(), [&](const auto& member) {
                    return member.actor_id == actor.actor->id || member.actor == actor.actor;
                })) {
                throw std::runtime_error("Companion ActorId or ActorState is duplicated");
            }
            next.session_binding_lease = actor.session_binding_lease;
            next.members.push_back({&*source, actor.actor->id, actor.actor, actor.retained_pose_owner});
            matched[source_index] = true;
        }
        if (std::find(matched.begin(), matched.end(), false) != matched.end())
            throw std::runtime_error("Runtime companion plan is missing an authored Priest/Faery actor");
        output = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
}

bool plan_runtime_companion_follow_event_v1(
    const RuntimeCompanionFollowPlanV1& plan, const std::string& source_object_name,
    SourceFollowerEventV1 event, const RuntimeCompanionFollowFactsV1& facts,
    RuntimeCompanionFollowDecisionV1& output, std::string& error) {
    output = {};
    error.clear();
    if (!plan.session_binding_lease || !plan.session_binding_lease.get())
        return fail(error, "Companion follow plan has no live CombatSession lease");
    const auto* member = plan.find(source_object_name);
    if (!member || !member->source || !member->actor || member->actor->id != member->actor_id ||
        !member->retained_pose_owner)
        return fail(error, "Companion follow actor lost its same-session ActorId/pose binding");
    output.actor_id = member->actor_id;
    if (member->source->ai_script != "follower") {
        output.source_policy_supported = false;
        return true; // e.g. Priest uses the separate Rene callback owner.
    }
    output.source_policy_supported = true;
    auto& decision = output;
    switch (event) {
        case SourceFollowerEventV1::friend_spotted:
            decision.handled = true;
            if (facts.has_master) return true;
            if (facts.friend_id == invalid_actor_id || facts.friend_id == member->actor_id)
                return fail(error, "follower.OnFriendSpotted requires the actual distinct friend Character");
            append(decision, SourceFollowerOperationV1::set_master, facts.friend_id);
            return true;
        case SourceFollowerEventV1::master_out_of_sight:
            decision.handled = true;
            if (!facts.has_master || facts.master_id == invalid_actor_id || facts.master_id == member->actor_id)
                return fail(error, "follower.OnMasterOutOfSight requires its actual master Character");
            append(decision, SourceFollowerOperationV1::warp_behind_master, facts.master_id);
            return true;
        case SourceFollowerEventV1::master_out_of_range:
            decision.handled = true;
            if (!facts.has_master || facts.master_id == invalid_actor_id || facts.master_id == member->actor_id)
                return fail(error, "follower.OnMasterOutOfRange requires its actual master Character");
            if (!facts.state_known)
                return fail(error, "follower.OnMasterOutOfRange requires the actual current Character state");
            if (!facts.state_is_move)
                append(decision, SourceFollowerOperationV1::move_to_master, facts.master_id);
            append(decision, SourceFollowerOperationV1::clear_target);
            return true;
        case SourceFollowerEventV1::master_in_ranged_range:
        case SourceFollowerEventV1::master_in_close_range:
        case SourceFollowerEventV1::master_in_melee_range:
            decision.handled = true;
            append(decision, SourceFollowerOperationV1::stop);
            return true;
        case SourceFollowerEventV1::unregistered:
            return true; // follower.luac registers no other Master event callback.
    }
    return true;
}

} // namespace dh::foundation::companions
