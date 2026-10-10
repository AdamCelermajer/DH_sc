#include "runtime_companion_follow_v1.hpp"

#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::companions;

namespace {
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

struct VitalsAndTransform {
    float health, max_health, resource, max_resource;
    Transform transform;
    ActorId target;
};

VitalsAndTransform capture(const ActorState& actor) {
    return {actor.health, actor.max_health, actor.resource, actor.max_resource,
            actor.transform, actor.target_id};
}

bool unchanged(const ActorState& actor, const VitalsAndTransform& before) {
    return actor.health == before.health && actor.max_health == before.max_health &&
        actor.resource == before.resource && actor.max_resource == before.max_resource &&
        actor.transform.position == before.transform.position &&
        actor.transform.rotation == before.transform.rotation &&
        actor.transform.scale == before.transform.scale && actor.target_id == before.target;
}
} // namespace

int main() {
    try {
        const auto& source = source_companion_follow_records_v1();
        check(source[0].source_object_name == "_prim_NPC_PriestGood" &&
              source[0].profile_id == "WanderingPriest" && source[0].ai_row == 50 &&
              source[0].ai_script == "rene" && source[0].animation_table == 47 &&
              source[0].authored_position == std::array<float, 3>{793.9f, -617.658f, 272.064f} &&
              source[0].source_vitals == std::array<std::int32_t, 4>{-1, -1, -1, -1},
              "Priest source record changed or invented missing vitals");
        check(source[1].source_object_name == "_prim_Faery" &&
              source[1].profile_id == "DefaultFairy" && source[1].ai_row == 20 &&
              source[1].ai_script == "follower" && source[1].animation_table == 23 &&
              source[1].authored_position == std::array<float, 3>{544.191f, 414.689f, -108.175f} &&
              source[1].source_vitals == std::array<std::int32_t, 4>{153600, 153600, -1, -1},
              "Faery source record changed or invented vitals");

        ActorState priest, faery;
        priest.id = 0x100000001ull;
        priest.definition_id = "WanderingPriest";
        priest.health = 72.0f;
        priest.max_health = 72.0f;
        priest.resource = 0.0f;
        priest.transform.position = {793.9f, -617.658f, 272.064f};
        priest.target_id = 91;
        faery.id = 0x100000002ull;
        faery.definition_id = "DefaultFairy";
        faery.health = 600.0f;
        faery.max_health = 600.0f;
        faery.resource = 0.0f;
        faery.transform.position = {544.191f, 414.689f, -108.175f};
        faery.target_id = 92;
        const auto priest_before = capture(priest);
        const auto faery_before = capture(faery);
        auto lease = std::make_shared<int>(17);
        int priest_pose = 1, faery_pose = 2;
        std::vector<RuntimeCompanionFollowActorBorrowV1> borrows{
            {"_prim_NPC_PriestGood", "WanderingPriest", "rene", 50, 47,
             &priest, &priest_pose, lease, true},
            {"_prim_Faery", "DefaultFairy", "follower", 20, 23,
             &faery, &faery_pose, lease, true},
        };
        RuntimeCompanionFollowPlanV1 plan;
        std::string error;
        check(build_runtime_companion_follow_plan_v1(borrows, plan, error), error.c_str());
        check(plan.members.size() == 2 && plan.find("_prim_NPC_PriestGood")->actor_id == priest.id &&
              plan.find("_prim_Faery")->actor_id == faery.id && plan.session_binding_lease == lease,
              "Same-session actors did not preserve the actual ActorIds and leases");

        RuntimeCompanionFollowDecisionV1 decision;
        RuntimeCompanionFollowFactsV1 facts;
        facts.has_master = true;
        facts.master_id = 0x100000010ull;
        facts.state_known = true;
        facts.state_is_move = false;
        check(plan_runtime_companion_follow_event_v1(plan, "_prim_Faery",
              SourceFollowerEventV1::master_out_of_range, facts, decision, error), error.c_str());
        check(decision.handled && decision.source_policy_supported && decision.actor_id == faery.id &&
              decision.operation_count == 2 &&
              decision.operations[0] == SourceFollowerOperationV1::move_to_master &&
              decision.arguments[0] == facts.master_id &&
              decision.operations[1] == SourceFollowerOperationV1::clear_target,
              "follower.OutOfRange must MoveTo(master) only when not moving, then always ClearTarget");

        facts.state_is_move = true;
        check(plan_runtime_companion_follow_event_v1(plan, "_prim_Faery",
              SourceFollowerEventV1::master_out_of_range, facts, decision, error), error.c_str());
        check(decision.operation_count == 1 &&
              decision.operations[0] == SourceFollowerOperationV1::clear_target,
              "follower.OutOfRange while moving must preserve source path and only ClearTarget");

        check(plan_runtime_companion_follow_event_v1(plan, "_prim_Faery",
              SourceFollowerEventV1::master_out_of_sight, facts, decision, error), error.c_str());
        check(decision.operation_count == 1 &&
              decision.operations[0] == SourceFollowerOperationV1::warp_behind_master &&
              decision.arguments[0] == facts.master_id,
              "follower.OutOfSight must WarpBehind(actual master)");

        for (auto event : {SourceFollowerEventV1::master_in_ranged_range,
                           SourceFollowerEventV1::master_in_close_range,
                           SourceFollowerEventV1::master_in_melee_range}) {
            check(plan_runtime_companion_follow_event_v1(plan, "_prim_Faery", event,
                  facts, decision, error), error.c_str());
            check(decision.operation_count == 1 &&
                  decision.operations[0] == SourceFollowerOperationV1::stop,
                  "follower in-range callbacks must Stop");
        }

        facts.has_master = false;
        facts.friend_id = 0x100000011ull;
        check(plan_runtime_companion_follow_event_v1(plan, "_prim_Faery",
              SourceFollowerEventV1::friend_spotted, facts, decision, error), error.c_str());
        check(decision.operation_count == 1 &&
              decision.operations[0] == SourceFollowerOperationV1::set_master &&
              decision.arguments[0] == facts.friend_id,
              "follower friend-spotted binds the source friend only while HasMaster is false");
        facts.has_master = true;
        check(plan_runtime_companion_follow_event_v1(plan, "_prim_Faery",
              SourceFollowerEventV1::friend_spotted, facts, decision, error), error.c_str());
        check(decision.handled && decision.operation_count == 0,
              "follower friend-spotted must leave an existing master unchanged");

        check(plan_runtime_companion_follow_event_v1(plan, "_prim_Faery",
              SourceFollowerEventV1::unregistered, facts, decision, error), error.c_str());
        check(!decision.handled && decision.source_policy_supported && decision.operation_count == 0,
              "Unregistered source event must remain unknown/no-op");
        check(plan_runtime_companion_follow_event_v1(plan, "_prim_NPC_PriestGood",
              SourceFollowerEventV1::master_out_of_range, facts, decision, error), error.c_str());
        check(!decision.source_policy_supported && !decision.handled && decision.operation_count == 0,
              "Rene must stay on its distinct catch-up policy");

        facts.has_master = false;
        check(!plan_runtime_companion_follow_event_v1(plan, "_prim_Faery",
              SourceFollowerEventV1::master_out_of_sight, facts, decision, error) &&
              error.find("actual master") != std::string::npos,
              "Missing source master must fail closed");
        facts.has_master = true;
        facts.state_known = false;
        check(!plan_runtime_companion_follow_event_v1(plan, "_prim_Faery",
              SourceFollowerEventV1::master_out_of_range, facts, decision, error) &&
              error.find("actual current Character state") != std::string::npos,
              "Missing source state must fail closed instead of guessing movement");

        check(unchanged(priest, priest_before) && unchanged(faery, faery_before),
              "Planning must not modify source vitals, transform, target or actor state");

        auto wrong = borrows;
        wrong[1].session_binding_lease = std::make_shared<int>(18);
        RuntimeCompanionFollowPlanV1 rejected;
        check(!build_runtime_companion_follow_plan_v1(wrong, rejected, error) &&
              error.find("same CombatSession lease") != std::string::npos,
              "Actors from different sessions must not be combined");
        wrong = borrows;
        wrong[1].animation_only_session_owner = false;
        check(!build_runtime_companion_follow_plan_v1(wrong, rejected, error),
              "Plan must require the existing animationOnly owner");
        wrong = borrows;
        wrong[0].ai_script = "follower";
        check(!build_runtime_companion_follow_plan_v1(wrong, rejected, error),
              "Priest must retain its actual Rene owner");

        std::cout << "runtime_companion_follow_v1 PASS: source-bound same-session Priest/Faery plan; exact follower callbacks; no invented offset or vitals changes\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
