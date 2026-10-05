#pragma once
#include "character_skill_combat_v6.hpp"
#include "character_hit_player_tail_v7.hpp"
#include "character_dot_player_reaction_v7.hpp"
namespace dh2::character::skills {
struct DotPlayerServicesV7 {
 const HitPlayerTailBorrowV7* hit_actor{};
 const HitPlayerTailServicesV7* hit_services{};
 const DotPlayerReactionServicesV7* reaction_services{};
};
// Source V6 common kernels plus isolated player receiver continuations.
// Existing V6 exports/unsupported contracts remain unchanged. Self/self DoT
// only; actual World supplies unchanged actor/property/HitFor/Apply services.
int character_dot_hit_for_v7(HitResult40*,HitActor32*,std::uint32_t,
 const HitAttacker24*,const HitServices16*,const DotPlayerServicesV7*);
int character_dot_apply_v7(SkillApplyOutputV6*,data::CombatResult*,
 SkillApplyActorV6*,const SkillApplyServicesV6*,const DotPlayerServicesV7*);
}
