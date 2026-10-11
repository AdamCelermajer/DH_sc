#pragma once

#include "native_ghost_skills.hpp"
#include "../../../../../../port/level-world/character_ai_skill_script_update.hpp"
#include "../../../../../../port/level-world/monster_external_script_session.hpp"

#include <cstdint>
#include <string>

namespace dh2::native::ghost_skill_update_v1 {

struct Result {
    character_ai_skill_script_update::Result source{};
    std::uint32_t lua_errors = 0;
    int last_lua_status = 0;
};
struct CleanupResult {
    std::uint32_t slots_examined = 0;
    std::uint32_t null_slots = 0;
    std::uint32_t callbacks = 0;
    std::uint32_t lua_errors = 0;
};

// Executes the existing CharAISkillScript::OnSkillUpdate caller against the
// same Ghost AIS VM, its retained skill instance arguments, and source aliases.
// ReturnValues and projected Lua returns are owned here for one synchronous
// update; the VM, script and Character stay owned by their existing sessions.
int update(monster_external_script::Session&, ghost_skills::Runtime&,
           std::uintptr_t character, std::uintptr_t script,
           Result&, std::string& error);
// Source _SkillCleanUp/_SpellCleanUp call each nonnull instance's
// OnSkillCleanUp; they do not delete or clear those instances.
int cleanup(monster_external_script::Session&, ghost_skills::Runtime&,
            std::uintptr_t character, std::uintptr_t script,
            Result&, std::string& error);
int cleanup_list(monster_external_script::Session&, ghost_skills::Runtime&,
                 std::uintptr_t character,
                 character_ai_set_skills_and_spells::List,
                 CleanupResult&, std::string& error);

} // namespace dh2::native::ghost_skill_update_v1
