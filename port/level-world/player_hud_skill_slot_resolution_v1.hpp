#pragma once

#include "../game-data/player_savegame_v1.hpp"
#include "../game-data/skill_tables.hpp"

#include <cstdint>
#include <vector>

namespace dh2::player_hud_skill_slot_resolution_v1 {

enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    invalid_source_fact = 2,
    empty_hud_slot = 3,
    missing_skill_row = 4,
    missing_prepared_script = 5,
};

struct Result {
    // HUD argument is the source Save-map key. skill_index is its mapped
    // zero-based index in the selected Character SkillList and Save rows.
    std::int32_t hud_slot = -1;
    std::uint32_t skill_index = 0;
    std::int32_t skill_id = -1;
    const data::SkillRow* skill_row = nullptr;
    std::uintptr_t script_identity = 0;
};

// NativeHUDSkill -> Character::SG_GetSkillInSlot -> PlayerSavegame map lookup.
// The caller passes the selected SkillList selector explicitly; this function
// never guesses how a Character property or field supplies that selector.
// `prepared_scripts` is borrowed from the existing Player preparation owner.
// This helper validates identity/index agreement and returns that same borrowed
// script identity; it creates no script, VM, Save, or vector.
inline Status resolve(const data::PlayerSavegameV1& save,
                      std::uintptr_t character,
                      std::int32_t hud_slot,
                      std::int32_t skill_list_selector,
                      const data::SkillTables& tables,
                      const std::vector<std::uintptr_t>* prepared_scripts,
                      Result* output) noexcept {
    if (!output || !character || hud_slot < 0 || skill_list_selector < 0)
        return Status::invalid_argument;
    if (save.character() != character || !save.skills_initialized() ||
        std::size_t(skill_list_selector) >= tables.skill_lists.size())
        return Status::invalid_source_fact;

    const auto mapped = save.skill_in_slot(hud_slot);
    if (mapped == -1) return Status::empty_hud_slot;
    if (mapped < 0) return Status::invalid_source_fact;
    const auto index = static_cast<std::uint32_t>(mapped);
    const auto& selected = tables.skill_lists[std::size_t(skill_list_selector)];
    const auto& saved_rows = save.skills();
    if (index >= selected.members.size() || index >= saved_rows.size())
        return Status::missing_skill_row;

    const auto skill_id = selected.members[index];
    if (skill_id < 0 || std::size_t(skill_id) >= tables.skills.size() ||
        saved_rows[index].id != skill_id)
        return Status::missing_skill_row;

    if (!prepared_scripts || index >= prepared_scripts->size() ||
        !(*prepared_scripts)[index])
        return Status::missing_prepared_script;

    Result value{};
    value.hud_slot = hud_slot;
    value.skill_index = index;
    value.skill_id = skill_id;
    value.skill_row = &tables.skills[std::size_t(skill_id)];
    value.script_identity = (*prepared_scripts)[index];
    *output = value;
    return Status::complete;
}

} // namespace dh2::player_hud_skill_slot_resolution_v1
