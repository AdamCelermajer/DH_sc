#pragma once
#include "npc_injury_runtime_v1.hpp"
#include "character_native_effects.hpp"
#include "character_knockback_reaction_v1.hpp"
#include "character_slow_reaction_v1.hpp"
namespace dh2::character {
struct NpcSkillReactionsBorrowV2 {
 CharacterWorldNpcStateOwnerV1* machine{};NpcInjuryRuntimeV1* injury{};
 data::PropertyView* properties{};const data::AiTables* ai{};
 const data::AnimationTables* animations{};const CharacterGameDesign::Borrow* design{};
 TimerStore32* timers{};const TimerServices32* timer_services{};
 SlowReactionBorrowV1 slow;
};
struct NpcSkillReactionsServicesV2 {
 KnockbackReactionServicesV1 knockback;
 // Real source heading/random/IsPlayer/StopLoop endpoints. Other operations
 // below are composed directly over this SAME FSM/table/TimerStore.
 NativeEffectServices16 remaining;
};
class NpcSkillReactionsV2 {
 NpcSkillReactionsBorrowV2 b_;NpcSkillReactionsServicesV2 s_;
 std::vector<std::int32_t> stunned_,scared_;
 NativeEffects32 effects_{};NativeEffectServices16 effect_services_{};
 static int effect(void*,NativeEffects32*,const NativeEffectRequest32*,std::uint32_t*);
 KnockbackReactionBorrowV1 knockback_borrow()const;
 std::string error_;
public:
 NpcSkillReactionsV2(NpcSkillReactionsBorrowV2,NpcSkillReactionsServicesV2);
 bool valid()const noexcept;
 //1 handled,0 not this owner,negative required service failure.
 int application(const skills::SkillApplyRequestV6&,skills::SkillApplyResponseV6*);
 int body(StateOwnerMachine40*,const StateOwnerRequest48&);
 int update(StateOwnerMachine40*,const StateOwnerUpdateRequest24&);
 // Source AI/state timer43/44 event route; no duplicate TimerStore or clock.
 int timer_event(std::uint32_t event,std::uintptr_t payload=0);
 // Called from the existing outer NativeFsmUpdate service, which alone owns
 // elapsed time and pending effect enforcement. No second Update/clock here.
 int frame_effect(const NativeFsmRequest32&);
 const std::string& error()const noexcept{return error_;}
};
}
