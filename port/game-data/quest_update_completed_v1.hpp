#pragma once

#include <cstdint>
#include <string>

namespace dh2::data::quest_update_completed_v1 {

struct QuestFields {
    std::int32_t* state = nullptr;
    std::int32_t* timestamp = nullptr;
};

struct RewardListView {
    const std::uintptr_t* children = nullptr;
    std::uint32_t count = 0;
};

enum class Stage : std::uint32_t {
    none,
    state_guard,
    reward_list,
    give_reward,
    post_completed_constant,
    application_time,
    store_state,
    store_timestamp,
};

enum class Status : std::uint32_t {
    complete,
    invalid_argument,
    service_unavailable,
    service_failed,
    reward_rejected,
    invalid_source_constant,
};

enum class Outcome : std::uint32_t {
    skipped_not_completed,
    transitioned,
};

struct Services {
    void* context = nullptr;
    // Returns the source virtual Reward::Give result. Zero stops the list.
    bool (*give_reward)(void*, std::uintptr_t reward_identity,
                        std::int32_t& source_result, std::string&) = nullptr;
    bool (*post_completed_constant)(void*, std::int32_t& value,
                                    std::string&) = nullptr;
    bool (*application_time)(void*, std::int32_t& value,
                             std::string&) = nullptr;
};

struct Result {
    Status status = Status::complete;
    Stage failed_stage = Stage::none;
    Outcome outcome = Outcome::skipped_not_completed;
    std::uint32_t rewards_attempted = 0;
};

// Quest::UpdateCompleted source-order slice over caller-owned Quest and
// RewardList storage. Only state 12 acts; rewards are visited in list order,
// and PostCompleted (13) plus timestamp are committed only after every Give
// succeeds. This owns no Quest, Reward, Character, VM, or save state.
Status update(QuestFields, const RewardListView&, const Services&, Result*,
              std::string& error);

} // namespace dh2::data::quest_update_completed_v1
