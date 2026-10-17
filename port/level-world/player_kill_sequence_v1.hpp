#pragma once

#include <cstdint>
#include <string>

namespace dh2::player_kill_sequence_v1 {

enum class Step : std::uint8_t { drop_loot, player_credit, distribute_xp, quest_tail };
using Callback = int (*)(void*, Step, std::string& error);

struct State {
    bool attempted[4]{};
    bool busy = false;
};

struct Result {
    std::uint32_t calls = 0;
    std::uint32_t attempted_mask = 0;
    std::uint32_t failed_mask = 0;
};

enum class Status { complete, invalid_argument, busy };

// Character::Kill order. Each stage is latched before invoking its owner so
// callbacks may not replay already-applied source effects. A failed stage is
// reported but does not suppress later stages; the source call site likewise
// continues after DropLoot/credit/XP provider diagnostics.
inline Status run(State& state, void* context, Callback callback,
                  Result* out, std::string& error) {
    if (!callback || !out) return Status::invalid_argument;
    if (state.busy) return Status::busy;
    state.busy = true;
    struct BusyReset { bool& value; ~BusyReset() { value = false; } } reset{state.busy};
    Result result{};
    constexpr Step steps[] = {Step::drop_loot, Step::player_credit,
                              Step::distribute_xp, Step::quest_tail};
    for (unsigned i = 0; i < 4; ++i) {
        if (state.attempted[i]) continue;
        state.attempted[i] = true;
        result.attempted_mask |= 1u << i;
        ++result.calls;
        std::string stage_error;
        if (callback(context, steps[i], stage_error) != 0) {
            result.failed_mask |= 1u << i;
            if (error.empty()) error = stage_error.empty() ? "Kill stage failed" : stage_error;
        }
    }
    *out = result;
    return Status::complete;
}

} // namespace dh2::player_kill_sequence_v1
