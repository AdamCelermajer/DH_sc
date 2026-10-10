#pragma once
#include "character_ai_death.hpp"
#include "character_aggro_clear_all_v2.hpp"
#include "character_skills.hpp"
#include "npc_dead_state_v1.hpp"
namespace dh2::character {
struct NpcDeathBorrowV2 {
 // SAME retained source projections; constructor fields are never reset here.
 AIDeathState64* death{};
 AIEventState64* events{};
 TargetBindings48* targets{};
 AggroClearActorBorrowV2 aggro;
 NpcDeadStateV1* dead_state{};
 skills::CharacterSkillOwner* skills{};
};
struct NpcDeathServicesV2 {
 AggroClearAllServicesV2 aggro;
 void* context{};
 bool(*group)(void*,std::uintptr_t group,std::uintptr_t character,
              std::uintptr_t attacker,std::string&){};
 // Real selected AIS source dispatcher. Scope is a borrowed same-VM capability.
 bool(*ais)(void*,std::uintptr_t receiver,std::uintptr_t source_method,
            std::uintptr_t attacker,const dh2_script_callback_scope*,std::string&){};
 // Publish CharAI's in-place timer-ID reset before reciprocal aggro callbacks
 // can synchronously reenter this Character's source Script/AI owner.
 void(*publish_timer_ids)(void*){};
};
// Whole original CharAI::OnDied -> AI_SetDead, composing the native source
// target, FSM, TimerStop, reciprocal Aggro and actual SkillOwner cleanups.
// Borrowed AIS/group services retain their source required failure boundaries.
// No HP write, kill rewards or direct state12 shortcut: caller enters through
// original Ctrl_Kill -> RaiseEvent2. This owner delivers only the AI virtual.
class NpcDeathOwnerV2 final {
 NpcDeathBorrowV2 b_;NpcDeathServicesV2 s_;std::string error_;
 const dh2_script_callback_scope* scope_{};
 static int invoke(void*,AIDeathState64*,const AIDeathRequest48*);
 bool coherent()const noexcept;
 void refresh_dispatch()noexcept;
public:
 NpcDeathOwnerV2(NpcDeathBorrowV2 b,NpcDeathServicesV2 s):b_(b),s_(s){}
 int on_died(AIDeathResult24&,std::uintptr_t attacker,
             const dh2_script_callback_scope* scope=nullptr);
 const std::string& error()const noexcept{return error_;}
};
}
