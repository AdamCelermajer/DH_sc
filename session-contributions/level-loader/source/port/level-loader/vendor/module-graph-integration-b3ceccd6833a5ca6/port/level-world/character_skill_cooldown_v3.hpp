#pragma once
#include "character_skills_owner_v3.hpp"
#include "../script-runtime/script_runtime.h"
namespace dh2::character::skills {
struct SkillCooldownBindingsV3 {
 CharacterSkillOwnerV3* owner{};void* context{};
 int(*number)(void*,const dh2_script_value*,float*){};
};
int skill_set_cooldown_v3(void*,const dh2_script_value*,std::uint32_t,
 dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
int spell_set_cooldown_v3(void*,const dh2_script_value*,std::uint32_t,
 dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
}
