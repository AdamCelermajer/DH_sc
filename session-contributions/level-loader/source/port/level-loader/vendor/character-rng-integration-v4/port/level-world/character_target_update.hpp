#pragma once
#include "character_target_bindings.hpp"
namespace dh2::character {
enum TargetUpdateService : std::uint32_t {target_update_awaiting_spawn=0,target_update_in_limbus,target_update_interactive,target_update_owner_ai_id,target_update_dead,target_update_sight,target_update_can_range,target_update_close_range,target_update_ranged_range,target_update_melee_range,target_update_raise_event};
struct TargetUpdateRequest24 {std::uint32_t service,event;std::uintptr_t subject,other;};
struct TargetUpdateServices16 {void* context;int(*invoke)(void*,TargetState48*,const TargetUpdateRequest24*,std::uint32_t* result);};
static_assert(sizeof(TargetUpdateRequest24)==24&&sizeof(TargetUpdateServices16)==16);
}
// Complete CharAI::_UpdateTarget3cb908. Uses the same live target/owner
// projections as AI_SetTarget; synchronous services may mutate or reenter.
// RaiseEvent is Character's genuine event route, not direct Lua/AIS dispatch.
// Predicate results are raw words; source stores captured low bytes AFTER
// transition callbacks. Providers must keep every borrowed projection alive.
// 0 complete/source skip;1 malformed entry atomic;2 provider/lifetime failure
// after the source prefix. There is no atomic rollback or default predicate.
extern "C" int dh2_character_target_update(dh2::character::TargetState48*,const dh2::character::TargetUpdateServices16*);
