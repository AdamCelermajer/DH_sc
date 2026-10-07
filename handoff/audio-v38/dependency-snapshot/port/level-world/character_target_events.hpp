#pragma once
#include "character_target_bindings.hpp"
namespace dh2::character {
struct TargetEventState32 {
 TargetState48* target;
 std::uintptr_t active;
 std::uint8_t continued,reserved_bytes[7];
 std::uint64_t reserved;
};
enum TargetEventService : std::uint32_t {target_event_debug_load=0,
 target_event_debug_construct,target_event_debug_query,target_event_debug_destroy,
 target_event_is_target_character,target_event_get_target_character,
 target_event_clear_aggro,target_event_owner_ai_id,target_event_owner_position,
 target_event_play_sound,target_event_active_dispatch};
struct TargetEventRequest64 {
 std::uint32_t service,event;
 std::uintptr_t subject,other;
 const char* text;
 std::int32_t sound;
 std::uint32_t flag;
 float position[3],parameters[2];
 std::int32_t integer;
};
struct TargetEventResponse24 {std::uintptr_t identity;std::uint32_t word;float position[3];};
struct TargetEventServices40 {
 void* context;
 int(*invoke)(void*,TargetEventState32*,const TargetEventRequest64*,TargetEventResponse24*);
 // Genuine AI-row sound+0x24 projection, in row order. Capture table/count
 // before owner_ai_id callback, like the original global table pointer load.
 const std::int32_t* ai_sounds;
 std::uint32_t ai_count,reserved;
 std::uintptr_t sound_manager;
};
static_assert(sizeof(TargetEventState32)==32&&sizeof(TargetEventRequest64)==64);
static_assert(sizeof(TargetEventResponse24)==24&&sizeof(TargetEventServices40)==40);
}
// Complete CharAI OnTargetDied/Revived/OutSight/InSight/OutRange/Ranged/Close/
// Melee handlers. event is original Character0xa..0x11, not AIS enum indices.
// Services execute synchronously; active, owner and state may change/reenter.
// is/get_target_character and clear_aggro subjects identify the captured OWNER
// whose inline CharAI receives the call, not a substituted master lookup.
// active_dispatch is the genuine AIS virtual endpoint after prefixes; no
// callback-availability gate is introduced here. Sound ABI is true,0,-1,-1.
// 0 complete,1 malformed entry atomic,2 failed provider/lifetime after prefix.
extern "C" int dh2_character_target_event(dh2::character::TargetEventState32*,std::uint32_t event,const dh2::character::TargetEventServices40*);
