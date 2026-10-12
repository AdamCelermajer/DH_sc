#pragma once
#include <cstdint>
namespace dh2::character {
struct ScriptCollisionState72 {
 std::uintptr_t script_owner,owner,ai_owner,controller,current_target,preferred_target;
 std::uint32_t collision_ms,last_collision_frame,paused,movement_state,application_frame,dt_ms;
};
struct ScriptCollisionObject16 {std::uintptr_t identity;std::uint32_t type,reserved;};
enum ScriptCollisionService : std::uint32_t {
 script_collision_is_character=0,script_collision_owner_is_player,
 script_collision_is_enemy,script_collision_cancel_sneaking,script_collision_set_target
};
struct ScriptCollisionRequest32 {
 std::uint32_t service,arg0;
 std::uintptr_t subject,target;
 std::uint32_t reserved0,reserved1;
};
struct ScriptCollisionServices16 {
 void* context;
 std::uint32_t(*invoke)(void*,ScriptCollisionState72*,ScriptCollisionObject16*,const ScriptCollisionRequest32*);
};
static_assert(sizeof(void*)==8&&sizeof(ScriptCollisionState72)==72&&sizeof(ScriptCollisionObject16)==16&&sizeof(ScriptCollisionRequest32)==32&&sizeof(ScriptCollisionServices16)==16);
}
// Complete AISDefault::OnCollisionPersist control flow. State4/19 and exact
// current Application frame/dt are supplied from their real producers. Named
// virtual/AI services remain synchronous and may mutate live state. Source
// current/preferred-target snapshots are retained across those calls.
// Return1 completed/-1 malformed native boundary; no generic clock advance.
extern "C" int dh2_character_script_collision(dh2::character::ScriptCollisionState72*,
 dh2::character::ScriptCollisionObject16*,std::uint32_t persist,
 const dh2::character::ScriptCollisionServices16*);
