#pragma once
#include "character_npc_animation_set_v6.hpp"
namespace dh2::character {
bool character_generic_register_animation_set_v62(const data::AnimationTables&,
 std::int32_t table,std::int32_t set,std::int32_t actual_stance_count,
 std::uint32_t actual_stanced_mask,const std::vector<std::int32_t>& skill_animations,
 NpcAnimationRegistrationServicesV6,std::string&);
}
