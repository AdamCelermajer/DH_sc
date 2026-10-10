#pragma once
#include "character_state.hpp"
#include "original_combat_properties.hpp"
namespace dh::foundation {
// Startup projection into an already prepared same-class/same-level source
// sheet, including its actual gear. Does not create actors or admit combat.
// Rebuilds derived base from the selected authored row and prepared level,
// preserving saved contributions, gear and combat facts. Not a live-buff API.
// Character-only saves carry integer XP; preserve a matching live Q8 remainder.
bool project_player_profile_properties(const OriginalPropertyDatabase&,
    const CharacterState&,const OriginalCombatProperties& prepared,
    OriginalCombatProperties& output,std::string& error);
}
