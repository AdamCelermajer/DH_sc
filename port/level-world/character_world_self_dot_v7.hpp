#pragma once
#include "character_dot_player_v7.hpp"
#include "character_world_skill_execution_v6.hpp"
namespace dh2::character::skills {
struct WorldSelfDotOutputV7 {
 DotResult24 calculate{};
 SkillApplyOutputV6 apply{};
 data::CombatResult attack{};
 int status{};
};
// Timer34 passes its actual player identity, freshly reread positive raw
// property126..131 and element-1..4. shared_cf is the retained common CF owner.
// No dt multiplier, timer authority, actor registry or alternate HP storage.
int character_world_self_dot_v7(WorldSelfDotOutputV7*,
 CharacterWorldSkillExecutionV6&,DotCombatContext32& shared_cf,
 std::uintptr_t player,std::int32_t amount,std::int32_t element,
 const DotServices16*,const DotPlayerServicesV7*);
}
