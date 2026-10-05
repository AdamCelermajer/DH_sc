#pragma once
#include "player_injure_state_v7.hpp"
#include "character_world_npc_state_owner_v1.hpp"
#include "character_script_session.hpp"
#include "character_skill_combat_v6.hpp"
#include "character_cancel_sneaking.hpp"
#include "../game-data/animation_tables.hpp"
namespace dh2::character {
struct NpcInjuryBorrowV1 {
 CharacterWorldNpcStateOwnerV1* machine{};
 CharacterScriptSession* session{};
 data::PropertyView* properties{};
 const data::AnimationTables* animations{};
 const CharacterGameDesign::Borrow* design{};
 TargetState48* target{};
 skills::SkillTargetCharacterV6* character{}; // Canonical byte415, no copy owner.
 float* gate_14fc{}; // Original Character ctor-1, SAME retained actor field.
 data::CombatActorState* life{}; // SAME World/MonsterSession owner, no mirror.
 // Optional actual skill rows/script-vector projection, only required when
 // real resolved198>0 reaches native AI_CancelSkill. Never fake an empty list.
 sneaking::Character48* skill_character{};
 const sneaking::Services16* skill_services{};
};
struct NpcInjuryServicesV1 {
 const skills::SkillAttackNativeServicesV6* debug{};
 target_providers::Services16 queries{};
 void* context{};
 int(*stance)(void*,std::uintptr_t,int*){};
 // Existing real NPC SM_SetAnim service selects original sequence/CPU bank.
 int(*set_animation)(void*,State*,int){};
 int(*look_at)(void*,std::uintptr_t,std::uintptr_t){};
};
class NpcInjuryRuntimeV1 {
 NpcInjuryBorrowV1 borrow_;NpcInjuryServicesV1 services_;
 PlayerInjureServicesV7 kernel_{};
 PlayerInjureBorrowV7 kernel_borrow()const;
 static int is_player(void*,std::uintptr_t,bool*);
 static int table(void*,std::uintptr_t,int*);
 static int animation_id(void*,int,bool*,int*);
 static int constant(void*,const char*,const char*,int*);
 static int stance(void*,std::uintptr_t,int*);
 static int event(void*,int,std::uintptr_t);
 static int transition(void*,int,int,std::uintptr_t);
 static int debug(void*,const char*);
 static int animation(void*,int);
 static int cancel(void*,std::uintptr_t);
 static int look(void*,std::uintptr_t);
public:
 NpcInjuryRuntimeV1(NpcInjuryBorrowV1,NpcInjuryServicesV1);
 bool valid()const noexcept;
 int injure(std::uintptr_t attacker,bool direct=false);
 int cancel_sneaking();
 int tick(std::uint32_t actual_application_dt);
 //1 handled,0 not this source body/service,negative required failure.
 int body(StateOwnerMachine40*,const StateOwnerRequest48&);
 int application(const skills::SkillApplyRequestV6&,skills::SkillApplyResponseV6*);
};
}
