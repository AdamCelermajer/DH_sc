#pragma once
#include "character_ai_frame.hpp"
#include <cstdint>
namespace dh2::character_ai_pause_update {
using State = character::AIFrameState32;
struct Request {
    std::uintptr_t ai, owner, user_ref;
    std::uint32_t duration_ms;
    std::int32_t repeat, event;
    std::uint32_t source_timer_offset;
};
struct Services {
    void* context;
    // Map the captured Character's embedded CharTimers(+3b4) to its actual
    // native timer store. Return0 completed; timer_id is the original Start
    // return word, which the caller does not use to clear or gate the pause.
    std::int32_t (*start)(void*, State*, const Request*, std::int32_t* timer_id);
};
struct Result { std::uint32_t called; std::int32_t timer_id; };
enum class Status : std::int32_t {
    complete = 0, invalid_argument, service_unavailable, service_failed,
};
// Original52B AI_PauseUpdate: capture owner BEFORE paused(+18) :=1, then
// Start(duration,repeat0,event0x31,usernull) on that owner's embedded timer.
// Even zero/max durations pass unchanged. Timer failures do not unpause.
// The shared frame projection makes pause visible to CharAI::Update; genuine
// timer expiration must separately relay event0x31 through RaiseAIEvent.
// Valid nonoverlapping borrowed objects must survive the synchronous call.
Status pause(State*, std::uint32_t duration_ms, const Services*, Result*);
static_assert(sizeof(Request) == 40 && sizeof(Services) == 16 && sizeof(Result) == 8);
}
