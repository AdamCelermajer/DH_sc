#pragma once
#include "player_injure_state_v7.hpp"
#include "character_state_owner.hpp"
#include "character_player_skills_v6.hpp"
#include "character_skill_combat_v6.hpp"
#include "character_target_bindings.hpp"
#include "character_target_providers.hpp"
#include "../game-data/animation_tables.hpp"
namespace dh2::character {
// Missing Character constructor fields, retained once per actual actor.
// Original3aa3fc/3aa50c/3aa514 writes -1.f;3aa248/3aa428 writes byte1.
struct PlayerInjuryFieldsV7 {float gate_14fc=-1.f;std::uint8_t low_health_1448=1;};
struct PlayerInjuryRuntimeBorrowV7 {
 CharacterStateOwner* state_owner{};
 skills::CharacterPlayerSkillsV6* player{};
 data::PropertyView* properties{};
 const data::AnimationTables* animations{};
 TargetState48* target{}; // Source Character+3c8 inline CharAI+40 = +408.
 PlayerInjuryFieldsV7* fields{};
 std::uint8_t* sneaking_415{};
};
struct PlayerInjuryRuntimeServicesV7 {
 const StateOwnerServices16* outer{};
 const skills::SkillAttackNativeServicesV6* debug{};
 target_providers::Services16 queries{};
 void* context{};
 int(*stance)(void*,std::uintptr_t,int*){}; // Same actual Gear GetAnimStance.
 int(*set_animation)(void*,int){}; // Same actual Character animator SM_SetAnim.
 int(*look_at)(void*,std::uintptr_t character,std::uintptr_t target){};
};
// No alternate FSM/Save/Gear/script/target/timers. All pointers are borrowed
// through this SAME retained player lifetime. Outer FSM service routes state11
// focus/blur/event back through body(); other states keep their real handlers.
class PlayerInjuryRuntimeV7 {
 PlayerInjuryRuntimeBorrowV7 borrow_;
 PlayerInjuryRuntimeServicesV7 services_;
 PlayerInjureServicesV7 kernel_{};
 static int is_player(void*,std::uintptr_t,bool*);
 static int animation_table(void*,std::uintptr_t,int*);
 static int injure_animation(void*,int,bool*,int*);
 static int constant(void*,const char*,const char*,int*);
 static int stance(void*,std::uintptr_t,int*);
 static int event(void*,int,std::uintptr_t);
 static int transition(void*,int,int,std::uintptr_t);
 static int debug(void*,const char*);
 static int animation(void*,int);
 static int cancel(void*,std::uintptr_t);
 static int look(void*,std::uintptr_t);
 PlayerInjureBorrowV7 kernel_borrow()const;
public:
 PlayerInjuryRuntimeV7(PlayerInjuryRuntimeBorrowV7,PlayerInjuryRuntimeServicesV7);
 bool valid()const noexcept;
 int injure(std::uintptr_t attacker,bool direct=false);
 int tick(std::uint32_t original_application_dt);
 //1 delivered,negative required failure,0 this request is not ours.
 int body(StateOwnerMachine40*,const StateOwnerRequest48&);
 // SAME World Apply backend:1 handled,-2 reached required failure,0 other
 // operation. Its caller translates1 to provider return0; never blanket-accept
 // scrolling text/audio/HitFX/Kill or another actor's application request.
 int application(const skills::SkillApplyRequestV6&,skills::SkillApplyResponseV6*);
 // Actual SM_IsIdle(false) kernel, includes source IDs13 and18.
 int is_idle(std::uintptr_t,bool*)const;
};
}
