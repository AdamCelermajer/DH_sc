#pragma once
#include "character_hit_player_v116.hpp"
#include "character_dot_player_reaction_v7.hpp"
namespace dh2::character::skills {
enum SkillApplyPlayerServiceV116:unsigned {
 skill_apply_trophy_manager_v116=21,skill_apply_local_player_v116,
 skill_apply_trophy_index_v116,skill_apply_unlock_v116
};
struct SkillApplyPlayerServicesV116 {
 const CharacterHitPlayerServicesV116* hit{};
 const DotPlayerReactionServicesV7* reaction{};
};
int character_skill_apply_result_v116(SkillApplyOutputV6*,data::CombatResult*,
 SkillApplyActorV6*,SkillApplyActorV6*,const SkillApplyServicesV6*,
 const SkillApplyPlayerServicesV116*);
}
