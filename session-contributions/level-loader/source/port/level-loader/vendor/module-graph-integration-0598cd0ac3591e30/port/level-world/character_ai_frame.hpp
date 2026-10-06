#pragma once
#include <cstdint>
namespace dh2::character {
// Borrowed stable owner projection. Capturing its pointer preserves the
// original old-owner zoning reads after a synchronous IsZonable callback.
struct AIFrameOwner48 {
 std::uintptr_t owner,controller;
 std::uint32_t flags520,forced,locked,zoned,in_zone,updated88,reserved0,reserved1;
};
struct AIFrameState32 {
 std::uintptr_t ai;
 AIFrameOwner48* owner;
 // global_blocked is actual v2Controller::s_blocked, not guessed debug policy.
 std::uint32_t paused,global_blocked,reserved0,reserved1;
};
enum AIFrameService : std::uint32_t {
 ai_frame_is_zonable=0,ai_frame_update_target,ai_frame_update_master,
 ai_frame_update_aggro,ai_frame_on_update,ai_frame_service_count
};
struct AIFrameRequest16 {std::uint32_t service,reserved;std::uintptr_t subject;};
struct AIFrameServices24 {
 void* context;
 std::int32_t(*invoke)(void*,AIFrameState32*,const AIFrameRequest16*,std::uint32_t* result);
 std::uint32_t available,reserved;
};
enum AIFrameSkip : std::uint32_t {ai_frame_not_skipped=0,ai_frame_paused,ai_frame_global_blocked,ai_frame_locked,ai_frame_policy_disabled,ai_frame_outside_zone};
struct AIFrameResult16 {std::uint32_t phase,skip,last_service,service_calls;};
static_assert(sizeof(void*)==8&&sizeof(AIFrameOwner48)==48&&sizeof(AIFrameState32)==32);
static_assert(sizeof(AIFrameRequest16)==16&&sizeof(AIFrameServices24)==24&&sizeof(AIFrameResult16)==16);
}
// Complete original CharAI::Update dispatcher. Proven empty release profiler
// bodies have no state effects. Target/master/aggro and virtual OnUpdate are
// explicit synchronous services; no clocks, Lua, ownership or Step invented.
// 0 complete(including source skip),1 malformed atomic,2 unavailable,3 failed.
// Phase=service+1 during call,6 completed; callback effects survive failures.
// Owner projections captured by the frame must outlive every callback. The
// callback may replace state.owner while updating either owner projection.
extern "C" int dh2_character_ai_frame(dh2::character::AIFrameResult16*,
 dh2::character::AIFrameState32*,const dh2::character::AIFrameServices24*);
