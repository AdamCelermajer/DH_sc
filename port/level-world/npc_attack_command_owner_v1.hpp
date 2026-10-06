#pragma once
#include "character_world_player_attack_owner_v1.hpp"
#include "character_animation_ai.hpp"
#include "character_world_npc_state_owner_v1.hpp"
#include "../script-runtime/script_runtime.h"
namespace dh2::character {
struct NpcAttackCommandBorrowV1 {
 skills::CharacterWorldRuntimeV1* world{};
 TargetBindings48* target{};
 CharacterWorldNpcStateOwnerV1* machine{};
 ControllerCommandState32* controller{};
 AnimationAIState96* ai{}; // SAME +74/+78/+79/+7a source field storage.
 const std::uintptr_t* object_of_interest14a4{};
 // Source v2Controller byte+a. Required only after genuine online=true.
 const std::uint8_t* network_enabled_a{};
};
struct NpcAttackCommandServicesV1 {
 void* context{};
 bool(*online)(void*,bool&,std::string&){};
 // Range attack/message queue remain real reached requirements. State event
 // backend is source SM_SetAttackState(false): SAME machine event c354.
 int(*backend)(void*,const AttackRequest32*,AttackResponse16*,
  const dh2_script_callback_scope*,std::string&){};
 WorldAIAttackServicesV1 geometry{};
 bool(*melee_radius)(void*,std::uintptr_t,float&,std::string&){};
};
// The historical PlayerAttack owner is reused only as the verified actor-
// generic World search/SetTarget/Cmd_Attack algorithm. No player owner, Gear,
// target, fields or FSM is borrowed. Source IsPlayer remains a World query.
class NpcAttackCommandOwnerV1 {
 NpcAttackCommandBorrowV1 b_;NpcAttackCommandServicesV1 s_;
 AttackState64 projection_{};ControllerAttackState32 controller_{};
 std::unique_ptr<skills::CharacterWorldPlayerAttackOwnerV1> algorithm_;
 const dh2_script_callback_scope* scope_{};std::string error_;bool active_{};
 static int backend(void*,const AttackRequest32*,AttackResponse16*);
 static bool radius(void*,std::uintptr_t,float&,std::string&);
 void read();void write();
public:
 NpcAttackCommandOwnerV1(NpcAttackCommandBorrowV1,NpcAttackCommandServicesV1,std::uint32_t capacity=4096);
 int command(std::uintptr_t,const dh2_script_callback_scope*);
 const dh2_script_callback_scope* scope_borrow()const noexcept{return scope_;}
 const std::string& error()const noexcept{return error_;}
};
}
