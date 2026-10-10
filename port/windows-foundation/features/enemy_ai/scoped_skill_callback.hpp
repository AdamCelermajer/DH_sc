#pragma once
#include "../../../level-world/character_skill_callback_session_v3.hpp"
namespace dh::foundation::enemy_ai {
// Original SkillCallback choreography on SAME V1 VM. Scope is ephemeral and
// must be actual current native DoSkill provider; never retained after return.
int scoped_skill_callback(dh2::character::CharacterScriptSession&,
 const dh2::character::skills::Instance32*,std::uint32_t operation,std::uint32_t*,
 const dh2_script_callback_scope*,std::string&);
}
