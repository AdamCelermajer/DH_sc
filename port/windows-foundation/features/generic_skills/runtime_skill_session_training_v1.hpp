#pragma once

#include "runtime_skill_progression_v1.hpp"
#include "../../combat_session.hpp"
#include "../../original_combat_properties.hpp"

#include <cstdint>
#include <string>

namespace dh::foundation::generic_skills {

// Result of the same-Session source IncSkill operation. Potion capacity is a
// derived side effect of IncSkill and is returned for the host's live
// inventory owner to publish; this helper does not invent a second inventory.
struct SessionSkillTrainingCommitV1 {
    int class_skill_position = -1;
    int saved_skill_row = -1;
    std::uint32_t previous_rank = 0;
    std::uint32_t current_rank = 0;
    std::uint32_t remaining_points = 0;
    std::uint8_t potion_capacity = 0;
};

// Runs original IncSkill against staged copies of the same canonical
// CharacterState and this CombatSession's player property sheet. Both owners
// are published only after the source checks, saved-row update and class-sheet
// recalc succeed. The caller remains responsible for publishing the returned
// potion capacity to its live inventory owner.
bool train_skill_in_session_v1(
    CharacterState& same_state, CombatSession& same_session,
    const OriginalPropertyDatabase& source_properties,
    dh2::data::SkillTables::Borrow source_skills,
    const CharacterDesignSkillCapsV1& source_caps,
    int class_skill_position, SessionSkillTrainingCommitV1& output,
    std::string& error);

// Nonmutating original IncSkill test probe over the same canonical state and
// live player property sheet. Rejection is a successful query with accepted
// false; source/owner failures return false.
bool probe_skill_training_in_session_v1(
    const CharacterState& same_state, CombatSession& same_session,
    const OriginalPropertyDatabase& source_properties,
    dh2::data::SkillTables::Borrow source_skills,
    const CharacterDesignSkillCapsV1& source_caps,
    int class_skill_position, bool& accepted, std::string& error);

} // namespace dh::foundation::generic_skills
