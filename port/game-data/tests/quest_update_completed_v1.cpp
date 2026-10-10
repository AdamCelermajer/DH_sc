#include "../quest_update_completed_v1.hpp"

#include <array>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace qu = dh2::data::quest_update_completed_v1;

namespace {
struct Context {
    std::vector<std::uint32_t> events;
    std::uint32_t fail_identity = 0;
    std::int32_t post_completed = 13;
    std::int32_t time = 9001;
    bool missing_time = false;
};

bool give(void* raw, std::uintptr_t identity, std::int32_t& result,
          std::string&) {
    auto& context = *static_cast<Context*>(raw);
    context.events.push_back(static_cast<std::uint32_t>(identity));
    result = identity == context.fail_identity ? 0 : 1;
    return true;
}

bool post_state(void* raw, std::int32_t& value, std::string&) {
    auto& context = *static_cast<Context*>(raw);
    context.events.push_back(100);
    value = context.post_completed;
    return true;
}

bool application_time(void* raw, std::int32_t& value, std::string&) {
    auto& context = *static_cast<Context*>(raw);
    context.events.push_back(101);
    if (context.missing_time) return false;
    value = context.time;
    return true;
}

void check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

qu::Services services(Context& context) {
    return {&context, give, post_state, application_time};
}
}

int main() {
    try {
        std::uint32_t checks = 0;
        const std::array<std::uintptr_t, 3> children{{11, 22, 33}};
        const qu::RewardListView list{children.data(),
                                      static_cast<std::uint32_t>(children.size())};
        std::string error;
        qu::Result result{};

        // State guard: non-12 states do not call reward, constant, or time services.
        {
            std::int32_t state = 6, timestamp = 4;
            Context context;
            const auto status = qu::update({&state, &timestamp}, list,
                                           services(context), &result, error);
            check(status == qu::Status::complete &&
                  result.outcome == qu::Outcome::skipped_not_completed &&
                  context.events.empty() && state == 6 && timestamp == 4,
                  "state guard invoked completion providers or mutated quest");
            ++checks;
        }

        // Success applies rewards in canonical order, then resolves constant/time,
        // and only then commits PostCompleted plus application timestamp.
        {
            std::int32_t state = 12, timestamp = 4;
            Context context;
            const auto status = qu::update({&state, &timestamp}, list,
                                           services(context), &result, error);
            check(status == qu::Status::complete &&
                  result.outcome == qu::Outcome::transitioned &&
                  result.rewards_attempted == 3 && state == 13 &&
                  timestamp == context.time &&
                  context.events == std::vector<std::uint32_t>({11, 22, 33, 100, 101}),
                  "successful completion ordering or stores changed");
            ++checks;
        }

        // A zero Give result short-circuits later rewards and leaves state/time intact.
        {
            std::int32_t state = 12, timestamp = 4;
            Context context;
            context.fail_identity = 22;
            const auto status = qu::update({&state, &timestamp}, list,
                                           services(context), &result, error);
            check(status == qu::Status::reward_rejected &&
                  result.rewards_attempted == 2 &&
                  result.failed_stage == qu::Stage::give_reward &&
                  result.outcome == qu::Outcome::skipped_not_completed &&
                  context.events == std::vector<std::uint32_t>({11, 22}) &&
                  state == 12 && timestamp == 4,
                  "failed reward did not short-circuit before transition");
            ++checks;
        }

        // Required timestamp service failure also prevents the transition.
        {
            std::int32_t state = 12, timestamp = 4;
            Context context;
            context.missing_time = true;
            const auto status = qu::update({&state, &timestamp}, list,
                                           services(context), &result, error);
            check(status == qu::Status::service_failed && state == 12 &&
                  timestamp == 4 && result.rewards_attempted == 3 &&
                  result.failed_stage == qu::Stage::application_time,
                  "timestamp provider failure committed completion");
            ++checks;
        }

        std::cout << "PASS quest_update_completed_v1 " << checks << " checks\n";
        return EXIT_SUCCESS;
    } catch (const std::exception& error) {
        std::cerr << "FAIL quest_update_completed_v1: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
