#pragma once
#include "character_timer_effects.hpp"
#include "character_world_skill_combat_v6.hpp"
#include "character_dot_calculate_services_v7.hpp"
namespace dh2::character {
struct NpcRecurringBorrowV1 {
 std::uintptr_t identity{};data::PropertyView* properties{};
 const std::int32_t* network_id{};const std::uint8_t* remote_update{};
 const std::uint8_t* dead{};const State* state{};
 // SAME source CharAI outgoing/incoming trees; +8c/+a4 are their size
 // words, not separate boolean fields or PlayerAggroFields counters.
 const data::AggroTable* outgoing{};const data::AggroTable* incoming{};
 skills::CharacterWorldSkillCombatV6* combat{};DotCombatContext32* shared_cf{};
 const skills::SkillAttackNativeServicesV6* debug{};
};
class NpcRecurringEffectsV1 {
 NpcRecurringBorrowV1 b_;TimerEffectState32 state_{};
 TimerEffectServices16 services_{};std::string error_;
 static int invoke(void*,TimerEffectState32*,const TimerEffectRequest40*,std::int32_t*,data::CombatResult*);
 int service(const TimerEffectRequest40&,std::int32_t&,data::CombatResult*);
public:
 explicit NpcRecurringEffectsV1(NpcRecurringBorrowV1);
 bool valid()const noexcept;
 // Actual source event0x33/0x34 (decimal51/52), not timer IDs33/34.
 int event(std::uint32_t,TimerEffectResult24&);
 const std::string& error()const noexcept{return error_;}
};
}
