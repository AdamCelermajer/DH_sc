#include "quest_update_completed_v1.hpp"

#include <exception>

namespace dh2::data::quest_update_completed_v1 {
namespace {

Status fail(Result& result, Stage stage, Status status, std::string& error,
            const char* fallback) {
    result.status = status;
    result.failed_stage = stage;
    if (error.empty()) error = fallback;
    return status;
}

} // namespace

Status update(QuestFields quest, const RewardListView& rewards,
              const Services& services, Result* out, std::string& error) {
    if (!out || reinterpret_cast<std::uintptr_t>(out) % alignof(Result) != 0) {
        error = "Quest::UpdateCompleted requires an aligned result";
        return Status::invalid_argument;
    }
    *out = {};
    error.clear();
    if (!quest.state || !quest.timestamp || quest.state == quest.timestamp ||
        reinterpret_cast<std::uintptr_t>(quest.state) % alignof(std::int32_t) != 0 ||
        reinterpret_cast<std::uintptr_t>(quest.timestamp) % alignof(std::int32_t) != 0) {
        out->status = Status::invalid_argument;
        out->failed_stage = Stage::state_guard;
        error = "Quest::UpdateCompleted requires distinct aligned canonical state fields";
        return out->status;
    }

    // The source dispatches UpdateCompleted only for Completed (12). Keep the
    // no-op path independent of callbacks so unrelated Quest states do nothing.
    if (*quest.state != 12) return Status::complete;

    if ((rewards.count && !rewards.children) || !services.give_reward ||
        !services.post_completed_constant || !services.application_time) {
        out->status = (rewards.count && !rewards.children)
                    ? Status::invalid_argument : Status::service_unavailable;
        out->failed_stage = (rewards.count && !rewards.children)
                          ? Stage::reward_list : Stage::give_reward;
        error = "Quest::UpdateCompleted requires its canonical RewardList and all reached providers";
        return out->status;
    }

    // RewardList::Give walks its canonical children in order and stops at the
    // first virtual Give returning zero. Earlier reward mutations are retained.
    for (std::uint32_t i = 0; i < rewards.count; ++i) {
        const auto identity = rewards.children[i];
        if (!identity) {
            out->failed_stage = Stage::reward_list;
            out->status = Status::invalid_argument;
            error = "Quest::UpdateCompleted reached a null canonical Reward child";
            return out->status;
        }
        std::int32_t source_result = 0;
        try {
            if (!services.give_reward(services.context, identity, source_result, error))
                return fail(*out, Stage::give_reward, Status::service_failed, error,
                            "Reward::Give provider failed");
        } catch (...) {
            return fail(*out, Stage::give_reward, Status::service_failed, error,
                        "Reward::Give provider threw");
        }
        ++out->rewards_attempted;
        if (!error.empty())
            return fail(*out, Stage::give_reward, Status::service_failed, error,
                        "Reward::Give provider failed");
        if (source_result == 0)
            return fail(*out, Stage::give_reward, Status::reward_rejected, error,
                        "Reward::Give returned zero; completion stopped");
    }

    std::int32_t post_completed = 0;
    try {
        if (!services.post_completed_constant(services.context, post_completed, error))
            return fail(*out, Stage::post_completed_constant, Status::service_failed,
                        error, "v2QuestState.PostCompleted provider failed");
    } catch (...) {
        return fail(*out, Stage::post_completed_constant, Status::service_failed,
                    error, "v2QuestState.PostCompleted provider threw");
    }
    if (!error.empty())
        return fail(*out, Stage::post_completed_constant, Status::service_failed,
                    error, "v2QuestState.PostCompleted provider failed");
    if (post_completed != 13)
        return fail(*out, Stage::post_completed_constant, Status::invalid_source_constant,
                    error, "v2QuestState.PostCompleted did not resolve to source value 13");

    std::int32_t application_time = 0;
    try {
        if (!services.application_time(services.context, application_time, error))
            return fail(*out, Stage::application_time, Status::service_failed, error,
                        "Application time provider failed");
    } catch (...) {
        return fail(*out, Stage::application_time, Status::service_failed, error,
                    "Application time provider threw");
    }
    if (!error.empty())
        return fail(*out, Stage::application_time, Status::service_failed, error,
                    "Application time provider failed");

    *quest.state = post_completed;
    out->failed_stage = Stage::store_state;
    *quest.timestamp = application_time;
    out->failed_stage = Stage::store_timestamp;
    out->outcome = Outcome::transitioned;
    out->status = Status::complete;
    out->failed_stage = Stage::none;
    error.clear();
    return Status::complete;
}

} // namespace dh2::data::quest_update_completed_v1
