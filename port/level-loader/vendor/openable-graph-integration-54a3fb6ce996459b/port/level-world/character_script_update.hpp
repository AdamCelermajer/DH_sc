#pragma once
#include <cstdint>
namespace dh2::character {
struct ScriptUpdateState48 {
 std::uintptr_t active,script_owner,ai_owner,controller;
 std::uint32_t collision_ms,paused,reserved0,reserved1;
};
enum ScriptUpdateService : std::uint32_t {script_update_timer_start=0,script_update_controller_stop};
struct ScriptUpdateRequest32 {
 std::uint32_t service,duration_ms,repeat,event;
 std::uintptr_t subject,user_ref;
};
struct ScriptUpdateServices16 {
 void* context;
 void(*invoke)(void*,ScriptUpdateState48*,const ScriptUpdateRequest32*);
};
static_assert(sizeof(void*)==8&&sizeof(ScriptUpdateState48)==48&&sizeof(ScriptUpdateRequest32)==32&&sizeof(ScriptUpdateServices16)==16);
}
// dispatch=0: complete AISDefault::OnUpdate body (also selected iPhone virtual).
// dispatch=1: CharAI::OnUpdate's selected-active virtual prefix; its subsequent
// owner animation/visibility body remains separate. 1 completed, -1 malformed.
// Timer Start and Cmd_Stop are borrowed synchronous services, not invented AIS.
extern "C" int dh2_character_script_update(dh2::character::ScriptUpdateState48*,
 std::uint32_t dispatch,const dh2::character::ScriptUpdateServices16*);
// Original RaiseAIEvent(0x31): clears paused and returns; no AIS/FSM service.
extern "C" int dh2_character_script_pause_expired(dh2::character::ScriptUpdateState48*);
