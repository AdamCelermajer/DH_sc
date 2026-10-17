#pragma once

#include "../game-data/player_savegame_v1.hpp"

#include <cstdint>

namespace dh2::character_ai_update_skills {

enum class List : std::uint32_t { skill = 0, faery = 1 };
enum class Decision : std::uint32_t {
    updated = 0,
    skipped_while_using_skill = 1,
    skipped_while_casting = 2,
};

struct ScriptVector {
    const std::uintptr_t* begin{};
    const std::uintptr_t* end{};
};

struct State {
    std::uintptr_t ai{};
    std::uintptr_t owner{};
    // Borrow the live CharStateMachine current-state word. Each predicate
    // reads it again, matching the two distinct source virtual calls.
    const std::int32_t* current_state{};
    const data::PlayerSavegameV1* savegame{};
    // PlayerSavegame::m_difficultyLevel, read only after SG_TellSlots.
    const std::int32_t* current_difficulty{};
};

struct Services {
    void* context{};
    // Fresh view of the real CharAI vector at each source callback boundary.
    // Before CharAI::SetSkillsAndSpells, an actual empty vector is valid.
    std::int32_t (*script_vector)(void*, State*, List, ScriptVector*){};
    // Executes the existing CharAISkillScript::OnSkillUpdate path for a
    // non-null slot. index is the saved slot key, not its mapped skill row.
    std::int32_t (*on_skill_update)(void*, State*, List, std::uint32_t index,
                                    std::uintptr_t script){};
};

struct Result {
    Decision decision{};
    std::uint32_t service_calls{};
    std::uint32_t saved_slots{};
    std::uint32_t script_updates{};
    std::uint32_t using_state_word{};
    std::uint32_t casting_state_word{};
    std::uint32_t faery_index{UINT32_MAX};
    std::uint32_t faery_updated{};
};

enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    invalid_source_fact = 2,
    service_unavailable = 3,
    service_failed = 4,
};

// CharAI::UpdateSkills at ELF 0x3d8a04. It gates on live FSM states 6/7,
// visits the existing Save's skill-set-zero map in key order, then checks the
// Save's current faery for the live difficulty. This is distinct from
// UpdateAllSkills: only assigned skill-slot keys and the selected faery script
// are updated. No VM, Save, vector, timer, or idle-state owner is created.
// Completed source effects remain if a later provider fails.
Status update(State*, const Services*, Result*);

} // namespace dh2::character_ai_update_skills
