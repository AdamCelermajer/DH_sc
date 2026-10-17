#pragma once
#include "character_ai_turn.hpp"
#include <cstdint>

namespace dh2::character_ai_queue {
struct Owner {
    std::uintptr_t identity;
    std::uint8_t forced, locked, visible, zoned, in_zone;
};
struct Entry { std::uintptr_t ai; Owner* owner; };
// The queue is app-owned, not one queue per actor. The original deque's
// allocator/block representation is replaced by bounded native storage.
struct State {
    Entry** entries;
    std::uint32_t count, capacity;
    std::int32_t timer;
    std::uint8_t global_blocked;
};
enum class Operation : std::uint32_t { frame_delta, is_faerie, is_follower, is_zonable };
struct Request { Operation operation; std::uintptr_t subject; };
struct Services {
    void* context;
    // Zero success. Query callbacks may replace Entry::owner or mutate live
    // owner fields/global block, but may not mutate queue/control metadata.
    // Old owner projections remain borrowed through return. Delta is raw u32.
    std::int32_t (*invoke)(void*, State*, Entry*, const Request*, std::uint32_t*);
};
enum class Decision : std::uint32_t { incomplete, timer_reduced, reset_small_queue, exhausted, selected };
struct Result { Decision decision; std::uint32_t rotations, calls; std::uintptr_t front; };
enum class Status : std::int32_t { complete, invalid_argument, service_unavailable, service_failed, invalid_source_fact };
// CharAI::IncUpdateQueue (568B): captures positive timer before GetDt; otherwise
// resets to180, rotates first, and examines at most count-1 candidates using
// original readiness order/fresh owner loads. Missing services preserve effects.
// Does not implement CharAI registration/destruction or original deque allocation.
Status advance(State*, const Services*, Result*);
character_ai_turn::Globals turn_globals(const State&) noexcept;
}
