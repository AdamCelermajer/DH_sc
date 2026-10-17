#pragma once

#include <cstdint>
extern "C" {
#include "../random/random.h"
}

namespace dh2::character_aggro_delay {

// Raw ARM32 words from CharAI+8/+0xc. Signed comparisons and modulo32
// arithmetic are performed explicitly; no frame-delta clamp is introduced.
struct State {
    std::uint32_t countdown_ms;
    std::uint32_t elapsed_not_turn_ms;
};

// Synchronous borrowed providers. is_my_turn supplies original IsMyTurn;
// disable_optimization supplies DebugSwitches/GetSwitch's exact query result.
// Application::GetDt must be queried freshly at each requested read. Providers
// may refresh State/RNG, but all borrowed objects must stay live until return.
struct Services {
    void* context;
    std::int32_t (*is_my_turn)(void*, State*, std::uint32_t* value);
    std::int32_t (*disable_optimization)(void*, State*, std::uint32_t* value);
    std::int32_t (*frame_delta)(void*, State*, std::uint32_t* raw_dt);
};

enum class Decision : std::uint32_t {
    incomplete, waiting_turn, waiting_delay, proceed_to_acquisition,
};
enum class Status : std::int32_t {
    complete = 0, invalid_argument = 1, service_unavailable = 2,
    service_failed = 3,
};
struct Result {
    Decision decision;
    std::uint32_t turn_queries;
    std::uint32_t debug_queries;
    std::uint32_t delta_reads;
    std::uint32_t ordinary_random_draws;
};

// Bounded _UpdateAggro branch 0x3cf75c..0x3cf8c0. Caller has already followed
// the source non-player/non-Faerie branch. The NPC exclusion is a later
// acquisition gate; it does not bypass this timing branch. This does not implement IsMyTurn's
// owner/global producers, the other acquisition branches or TargetList search.
// Invalid top-level aliases reject before effects. Runtime provider failures
// and exceptions retain completed effects; no rollback or cleanup is invented.
// One owning thread; callbacks must not destroy or invalidate these projections.
Status update(State*, dh2_random_state*, const Services*, Result*);

}  // namespace dh2::character_aggro_delay
