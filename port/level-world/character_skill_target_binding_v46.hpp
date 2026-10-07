#pragma once
#include "character_skill_state_v4.hpp"
#include "character_target_bindings.hpp"
namespace dh2::character::skills {
// Borrows the sole CharAI target graph. The legacy SkillState fields are a
// synchronous projection, not a second target owner.
int character_skill_state_target_bound_v46(SkillStateV4&,TargetState48&,
 std::uint32_t operation,std::uint32_t index,std::uint32_t moving,
 std::uintptr_t payload,std::uint32_t force,const SkillStateServices16V4&);
}
