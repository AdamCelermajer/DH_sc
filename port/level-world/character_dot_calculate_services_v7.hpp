#pragma once
#include "character_skill_combat_v6.hpp"
namespace dh2::character::skills {
// Borrow actual WorldSkillExecution debug services; no alternate DebugSwitches.
DotServices16 dot_calculate_services_v7(const SkillAttackNativeServicesV6&);
}
