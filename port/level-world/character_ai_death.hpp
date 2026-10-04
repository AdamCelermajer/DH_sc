#pragma once
#include "character_target_bindings.hpp"
#include "character_timers.hpp"
namespace dh2::character {
struct AIDeathOwner24 {std::uintptr_t character,state_machine;TimerStore32* timers;};
struct AIDeathState64 {
 std::uintptr_t ai;AIDeathOwner24* owner;TargetState48* target;
 std::uintptr_t group,active;const std::uintptr_t* ais_virtuals;
 std::uint32_t timer0,timer1;std::uint64_t reserved;
};
enum AIDeathService:std::uint32_t {
 ai_death_group=0,ai_death_ais,ai_death_state,ai_death_clear_all_aggro,
 ai_death_clear_aggro_toward_me,ai_death_skill_cleanup,ai_death_spell_cleanup
};
struct AIDeathRequest48 {
 std::uint32_t service,argument;std::uintptr_t subject,owner,payload,callee;
 std::uint32_t operation,reserved;
};
struct AIDeathServices16 {
 void* context;
 //0 only for genuine synchronous delivery. Missing deeper bodies fail.
 // Group receives current Character and captured attacker; AIS receives
 // captured attacker and captured virtual+24 callee. State receives current
 // state_machine, (false,NULL,true): argument1 is the final true flag.
 // ClearAggroTowardMe argument0 is false. Other cleanup calls have no args.
 int(*invoke)(void*,AIDeathState64*,const AIDeathRequest48*);
};
struct AIDeathResult24 {std::uint32_t phase,callbacks,stopped_timers,reset_ids,completed;std::int32_t status;};
static_assert(sizeof(AIDeathOwner24)==24&&sizeof(AIDeathState64)==64);
static_assert(sizeof(AIDeathRequest48)==48&&sizeof(AIDeathServices16)==16&&sizeof(AIDeathResult24)==24);
}
extern "C" {
//Complete recoverable OnDied and AI_SetDead ordering. Source group/active/owner
// fields are live and reloaded at the original positions. Native target setter,
// SyncLastTarget and TimerStop execute; deeper owned group/state/aggro/skill/
// spell bodies remain required services. No fabricated state12/physics effects.
// Borrowed pointers/backings stay alive; replacement projections remain coherent
// (target.owner.identity == current death.owner.character). Services can reenter
// synchronously; no suppression or once-only guard. AIS table has51 slots.
//1 complete,-1 malformed atomic,-2 required/provider failure after source prefix.
int dh2_character_ai_on_died(dh2::character::AIDeathResult24*,dh2::character::AIDeathState64*,std::uintptr_t attacker,const dh2::character::TargetServices16*,const dh2::character::AIDeathServices16*);
int dh2_character_ai_set_dead(dh2::character::AIDeathResult24*,dh2::character::AIDeathState64*,const dh2::character::TargetServices16*,const dh2::character::AIDeathServices16*);
}
