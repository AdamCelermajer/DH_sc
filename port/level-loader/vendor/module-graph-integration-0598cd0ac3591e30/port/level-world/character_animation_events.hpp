#pragma once
#include <cstdint>

namespace dh2::character {
// The Character/CharAI dispatcher accepts these six animation notifications.
// State-specific consumers execute synchronously through the owner services.
enum AnimationEventService : std::uint32_t {
 animation_end_virtual=0, animation_state_getter=1,
 animation_attack_end=2, animation_skill_end=3,
 animation_attack_begin=4, animation_skill_begin=5,
 animation_move_begin=6, animation_state_event=7
};
struct AnimationEventFacts {
 std::uint32_t event,global_blocked,controller_locked,controller_forced;
 std::uintptr_t payload;
};
struct AnimationEventRequest {
 std::uint32_t service,event;
 std::uintptr_t payload;
};
struct AnimationEventServices {
 void* context;
 // State getter returns the LIVE state. Begin/end consumers return nonzero to
 // forward to the machine. animation_end_virtual's return is ignored.
 std::int32_t(*invoke)(void*,const AnimationEventRequest*);
};
}
// 1 completed, -1 malformed/unsupported request, with no callbacks on reject.
// No default AI behavior is invented when a state-specific service is missing:
// callers must supply the actual consumer to preserve its mutations/reentry.
extern "C" int dh2_character_animation_event_route(
 const dh2::character::AnimationEventFacts*,
 const dh2::character::AnimationEventServices*);
