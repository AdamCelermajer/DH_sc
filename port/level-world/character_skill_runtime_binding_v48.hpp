#pragma once
#include "character_skill_target_binding_v46.hpp"
namespace dh2::character::skills {
// Read-only borrowed SAME Character+2dc slot, whose real constructor/SetPhysical
// producer owns its value. NULL slot pointer is unavailable; *slot==0 is valid.
int character_skill_runtime_bound_v48(SkillStateV4&,TargetState48&,
 const std::uintptr_t* same_physical2dc,std::uint32_t operation,
 std::uint32_t index,std::uint32_t moving,std::uintptr_t payload,
 std::uint32_t force,const SkillStateServices16V4&);
}
