#pragma once

#include "../../character_state.hpp"
#include "../../../game-data/data.hpp"
#include "../../../game-data/skill_tables.hpp"

#include <array>
#include <cstdint>
#include <string>

namespace dh::foundation::generic_skills {

// Values are the three original CharacterDesign MaxSkillLevel constants in
// normal, hard, very-hard order. The caller supplies the actual unlocked
// difficulty; this helper does not invent defaults or read a native profile.
struct CharacterDesignSkillCapsV1 {
    bool known = false;
    unsigned unlocked_difficulty = 0;
    std::array<std::uint32_t, 3> max_skill_level{};
};

struct SkillProgressionV1 {
    int character_row = -1;
    int skill_list_id = -1;
    int class_skill_position = -1;
    int skill_table_id = -1;
    int saved_skill_row = -1;
    int required_character_level = 0;
    std::uint32_t character_level = 0;
    std::uint32_t rank = 0;
    std::uint32_t rank_cap = 0;
    bool rank_known = false;
    bool available = false;
    bool can_increment_known = false;
    bool can_increment = false;
    bool equippable_known = false;
    bool equippable = false;
    bool training_known = false;
    bool can_train = false;
};

// Evaluate one class-list position against the same CharacterState and actual
// CharacterTable/SkillTables. Missing saved rank, points, or design caps stays
// explicitly unknown; no fallback rank or cap is synthesized.
bool evaluate_skill_progression_v1(
    const CharacterState&, const dh2::data::CharacterTable&,
    dh2::data::SkillTables::Borrow, const CharacterDesignSkillCapsV1&,
    int class_skill_position, SkillProgressionV1&, std::string& error);

// Apply the original generic progression effects only when the source points,
// difficulty cap, availability, and CanIncrementSkill level/rank gate pass.
// Failure leaves the entire same CharacterState unchanged.
bool train_skill_v1(
    CharacterState&, const dh2::data::CharacterTable&,
    dh2::data::SkillTables::Borrow, const CharacterDesignSkillCapsV1&,
    int class_skill_position, std::string& error);

} // namespace dh::foundation::generic_skills
