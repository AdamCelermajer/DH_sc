#pragma once

#include "../../actor_state.hpp"

#include <array>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace dh::foundation::companions {

struct SourceCompanionFollowRecordV1 {
    std::string_view source_object_name;
    std::string_view profile_id;
    std::int32_t ai_row = -1;
    std::string_view ai_script;
    std::int32_t animation_table = -1;
    std::array<std::int32_t, 4> source_vitals{}; // HP, Max_HP, MP, Max_MP raw source words.
    std::array<float, 3> authored_position{};
};

// Exact Swamp source CharacterProperties/AI/MGP rows. Positions are evidence
// and initial placement only; the plan never writes them to the live actor.
const std::array<SourceCompanionFollowRecordV1, 2>&
source_companion_follow_records_v1() noexcept;

struct RuntimeCompanionFollowActorBorrowV1 {
    std::string source_object_name;
    std::string profile_id;
    std::string ai_script;
    std::int32_t ai_row = -1;
    std::int32_t animation_table = -1;
    ActorState* actor = nullptr; // Borrow from the caller's existing CombatSession.
    const void* retained_pose_owner = nullptr;
    std::shared_ptr<const void> session_binding_lease;
    bool animation_only_session_owner = false;
};

struct RuntimeCompanionFollowMemberV1 {
    const SourceCompanionFollowRecordV1* source = nullptr;
    ActorId actor_id = invalid_actor_id;
    ActorState* actor = nullptr;
    const void* retained_pose_owner = nullptr;
};

struct RuntimeCompanionFollowPlanV1 {
    std::shared_ptr<const void> session_binding_lease;
    std::vector<RuntimeCompanionFollowMemberV1> members;

    const RuntimeCompanionFollowMemberV1* find(const std::string& source_object_name) const noexcept;
};

// Build a same-session reference plan for the two exact Swamp actors. The
// caller must obtain every actor pointer, pose owner and lease from its current
// animationOnly CombatSession and retain the session lease while using output.
// No actor is spawned, moved, damaged, or assigned a synthesized vitals value.
bool build_runtime_companion_follow_plan_v1(
    const std::vector<RuntimeCompanionFollowActorBorrowV1>&,
    RuntimeCompanionFollowPlanV1&, std::string& error);

enum class SourceFollowerEventV1 {
    friend_spotted,
    master_out_of_sight,
    master_out_of_range,
    master_in_ranged_range,
    master_in_close_range,
    master_in_melee_range,
    unregistered,
};

enum class SourceFollowerOperationV1 {
    set_master,
    move_to_master,
    clear_target,
    warp_behind_master,
    stop,
};

struct RuntimeCompanionFollowFactsV1 {
    bool has_master = false;
    ActorId master_id = invalid_actor_id;
    ActorId friend_id = invalid_actor_id;
    bool state_known = false;
    bool state_is_move = false;
};

struct RuntimeCompanionFollowDecisionV1 {
    bool handled = false;
    bool source_policy_supported = false;
    ActorId actor_id = invalid_actor_id;
    std::array<SourceFollowerOperationV1, 2> operations{};
    std::array<ActorId, 2> arguments{invalid_actor_id, invalid_actor_id};
    std::size_t operation_count = 0;
};

// Exact follower.luac callback projection. It emits commands for the existing
// source Character/controller owner to execute. It does not move an actor,
// inspect or replace the source path, generate events, or advance animation.
bool plan_runtime_companion_follow_event_v1(
    const RuntimeCompanionFollowPlanV1&, const std::string& source_object_name,
    SourceFollowerEventV1, const RuntimeCompanionFollowFactsV1&,
    RuntimeCompanionFollowDecisionV1&, std::string& error);

} // namespace dh::foundation::companions
