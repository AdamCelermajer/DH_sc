#pragma once

#include <cstdint>

namespace dh2::character_ai_turn {

struct State {
    std::uintptr_t ai;
    std::uintptr_t owner;
};

// Logical view of source s_updateQueue/s_updateTimer. The caller owns the
// queue, computes its length, and retains its first AI identity. This is not
// an overlay of the original 32-bit STL deque or its allocator.
struct Globals {
    std::uintptr_t queue_front;
    std::uint32_t queue_length;
    std::int32_t update_timer;
};

enum class Query : std::uint32_t { is_follower, is_faerie, virtual_is_player };
struct Services {
    void* context;
    // Fresh owner is captured before each call. Callback replacement of
    // State::owner is observed by subsequent calls. All identities stay live.
    std::int32_t (*invoke)(void*, State*, Query, std::uintptr_t owner,
                           std::uint32_t* value);
};
struct Result {
    std::uint32_t value;
    std::uint32_t follower_queries;
    std::uint32_t faerie_queries;
    std::uint32_t player_queries;
    std::uint32_t queue_front_read;
};
enum class Status : std::int32_t {
    complete = 0, invalid_argument = 1, service_unavailable = 2,
    service_failed = 3, empty_queue_unsupported = 4,
};

// Original CharAI::IsMyTurn nonempty-queue decision. Source followers/faeries
// bypass turn selection; the final call is Character's virtual IsPlayer.
// Empty queue is reported explicitly: original assert-level diagnostics and
// its intentional null write at assert level 2 are outside this adapter.
// The queue/timer producers and original concrete character queries remain
// caller-owned. Errors keep prior callback effects; no rollback is added.
Status evaluate(State*, const Globals*, const Services*, Result*);

}  // namespace dh2::character_ai_turn
