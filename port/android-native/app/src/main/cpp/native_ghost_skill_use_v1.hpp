#pragma once

#include "native_ghost_skills.hpp"
#include "../../../../../../port/level-world/character_ai_skill_script_check.hpp"
#include "../../../../../../port/level-world/monster_external_script_session.hpp"

#include <cstdint>
#include <string>

namespace dh2::native::ghost_skill_use_v1 {

enum class Check { usable, active };
enum class Callback { pre, use, post };

struct Result {
    // Source Value::getBool of the first return (Usable / Pre / Use) or of the
    // second OnSkillCheck return (Active). A normal Lua Error yields false;
    // a successful callback with no return value counts as true (Pre / Use).
    std::uint32_t value = 0;
    std::uint32_t call_count = 0;
    int last_lua_status = 0;
    bool lua_error = false;
};

// CharAISkillScript::OnSkillCheck_Usable (0x3da9dc) / OnSkillCheck_Active
// (0x3db16c) and OnPreSkill (0x3da8b8) / OnSkill (0x3da794) / OnPostSkill
// (0x3da6c0) over a Monster's retained skill instance and the SAME Monster AIS
// VM (see player_skill_use_session_v1 for the Player-VM counterpart). Each call
// runs SetSkill(name, index) first, erases its returns, then the named callback.
// The VM must be idle: callers run these from Services::after_callback, never
// from inside a Lua native. Returns 0 when the source caller completed (including
// a normal Lua Error), negative for a VM or required-service failure.
int check(monster_external_script::Session&, ghost_skills::Runtime&,
          std::uintptr_t character, std::uint32_t slot, Check, Result&,
          std::string& error);
int invoke(monster_external_script::Session&, ghost_skills::Runtime&,
           std::uintptr_t character, std::uint32_t slot, Callback, Result&,
           std::string& error);

} // namespace dh2::native::ghost_skill_use_v1
