#pragma once
#include <cstdint>
namespace dh2::character {
// Logical timer projection; expiry passes this object, not user_ref.
struct Timer32 {
 std::uint32_t id=0;
 std::int32_t repeat=0;
 std::uint32_t duration_ms=0,elapsed_ms=0;
 std::uint8_t active=0,paused=0;
 std::uint16_t reserved=0;
 std::int32_t event=-1;
 std::uintptr_t user_ref=0;
};
struct TimerStore32 {
 Timer32* slots=nullptr;
 std::uint32_t count=0,capacity=0;
 std::uintptr_t owner=0;
 std::uint32_t update_depth=0,reserved=0;
};
using TimerExpired=void(*)(void*,std::uintptr_t,std::int32_t,Timer32*);
// Expiry may call timer operations or change timer fields synchronously. It
// must not relocate/shrink slots or change owner/update_depth/reserved directly.
// Optional caller allocator: preserve count/owner/active slots; grow capacity
// to at least requested, return 1 on success. Never invoked within Update.
using TimerGrow=int(*)(void*,TimerStore32*,std::uint32_t);
struct TimerServices32 {void* context=nullptr;TimerExpired expired=nullptr;TimerGrow grow=nullptr;std::uint64_t reserved=0;};
static_assert(sizeof(void*)==8&&sizeof(Timer32)==32&&sizeof(TimerStore32)==32&&sizeof(TimerServices32)==32);
}
extern "C" {
// Source success returns lowest free ID; native guards -1 malformed,
// -2 caller storage exhausted, -3 growth during synchronous Update unsupported.
std::int32_t dh2_character_timer_start(dh2::character::TimerStore32*,std::uint32_t duration_ms,
 std::int32_t repeat,std::int32_t event,std::uintptr_t user_ref,const dh2::character::TimerServices32*);
// 1 updated, -1 malformed. Clock and ScriptManager+0x30 are explicit inputs.
int dh2_character_timers_update(dh2::character::TimerStore32*,std::uint32_t dt_ms,
 std::uint32_t script_blocked,const dh2::character::TimerServices32*);
// 1 affected, 0 out of range, -1 malformed. Pause mode is a native bool.
int dh2_character_timer_pause(dh2::character::TimerStore32*,std::uint32_t id,std::uint32_t paused);
int dh2_character_timer_stop(dh2::character::TimerStore32*,std::uint32_t id);
int dh2_character_timers_stop_all(dh2::character::TimerStore32*);
// Source TimeLeft returns elapsed and duration, not their difference.
int dh2_character_timer_time_left(std::uint32_t* elapsed,std::uint32_t* duration,
 const dh2::character::TimerStore32*,std::uint32_t id);
}
