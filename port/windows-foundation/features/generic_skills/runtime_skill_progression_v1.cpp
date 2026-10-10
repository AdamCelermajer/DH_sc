#include "runtime_skill_progression_v1.hpp"

#include "generic_skills_page_v1.hpp"

#include <algorithm>
#include <cstring>
#include <limits>
#include <optional>
#include <utility>

namespace dh::foundation::generic_skills {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

} // namespace

bool evaluate_skill_progression_v1(
    const CharacterState& state, const dh2::data::CharacterTable& characters,
    dh2::data::SkillTables::Borrow tables,
    const CharacterDesignSkillCapsV1& caps, int position,
    SkillProgressionV1& output, std::string& error) {
    error.clear();
    if (!tables) return fail(error, "Skill progression requires the original SkillTables borrow");
    if (position < 0) return fail(error, "Skill progression class position is negative");
    if (caps.known && caps.unlocked_difficulty >= caps.max_skill_level.size())
        return fail(error, "Original unlocked difficulty is outside CharacterDesign skill caps");

    const auto character_name = std::find(characters.names.begin(), characters.names.end(), state.class_id);
    if (character_name == characters.names.end())
        return fail(error, "CharacterState.class_id is not an original CharacterTable row token");
    const auto character_row = static_cast<std::size_t>(character_name - characters.names.begin());
    const auto skill_tree = std::find(characters.fields.begin(), characters.fields.end(), "SkillTree");
    if (skill_tree == characters.fields.end())
        return fail(error, "Original CharacterTable has no SkillTree field");
    const auto skill_tree_column = static_cast<std::size_t>(skill_tree - characters.fields.begin());
    if (character_row >= characters.rows.size() || skill_tree_column >= characters.rows[character_row].size())
        return fail(error, "Original CharacterTable class row has no SkillTree value");
    const int authored_list = characters.rows[character_row][skill_tree_column];
    if (authored_list < 0 || static_cast<std::size_t>(authored_list) >= tables.lists().size())
        return fail(error, "Original CharacterTable.SkillTree references an absent SkillList");

    const auto list_matches_saved_rows = [&](const std::vector<std::int32_t>& list) {
        if (list.size() != state.skills.size()) return false;
        for (std::size_t i = 0; i < list.size(); ++i) {
            const auto id = list[i];
            if (id < 0 || static_cast<std::size_t>(id) >= tables.skill_names().size() ||
                state.skills[i].id != tables.skill_names()[static_cast<std::size_t>(id)]) return false;
        }
        return true;
    };
    int active_list = authored_list;
    bool saved_rows_match = false;
    if (!state.skills.empty()) {
        for (std::size_t i = 0; i < tables.lists().size(); ++i) {
            if (!list_matches_saved_rows(tables.lists()[i])) continue;
            active_list = static_cast<int>(i);
            saved_rows_match = true;
            if (active_list == authored_list) break;
        }
        if (state.source_skill_slots_known && !saved_rows_match)
            return fail(error, "Source-known saved skill rows do not match any original SkillList order");
    }
    const auto& list = tables.lists()[static_cast<std::size_t>(active_list)];
    if (static_cast<std::size_t>(position) >= list.size())
        return fail(error, "Skill progression position is outside the active source SkillList");
    const int table_id = list[static_cast<std::size_t>(position)];
    if (table_id < 0 || static_cast<std::size_t>(table_id) >= tables.skills().size() ||
        static_cast<std::size_t>(table_id) >= tables.skill_names().size())
        return fail(error, "Active source SkillList references an absent SkillTable row");

    std::optional<std::uint32_t> saved_row;
    if (saved_rows_match && static_cast<std::size_t>(position) < state.skills.size()) {
        saved_row = static_cast<std::uint32_t>(position);
    } else {
        for (std::size_t i = 0; i < state.skills.size(); ++i) {
            if (state.skills[i].id != tables.skill_names()[static_cast<std::size_t>(table_id)]) continue;
            if (saved_row) return fail(error, "CharacterState has ambiguous duplicate saved skill IDs");
            saved_row = static_cast<std::uint32_t>(i);
        }
    }

    SkillProgressionV1 next;
    next.character_row = static_cast<int>(character_row);
    next.skill_list_id = active_list;
    next.class_skill_position = position;
    next.skill_table_id = table_id;
    const auto raw_required_level = tables.skills()[static_cast<std::size_t>(table_id)].scalar.words[8];
    std::int32_t required_level{};
    static_assert(sizeof(required_level) == sizeof(raw_required_level), "source scalar word width");
    std::memcpy(&required_level, &raw_required_level, sizeof(required_level));
    next.required_character_level = required_level;
    next.character_level = state.stats.level;
    if (saved_row && *saved_row < state.skills.size()) {
        next.rank_known = true;
        next.rank = state.skills[*saved_row].rank;
        next.saved_skill_row = static_cast<int>(*saved_row);
    }

    next.available = static_cast<std::int64_t>(next.character_level) >=
                     static_cast<std::int64_t>(next.required_character_level);
    if (next.rank_known) {
        next.can_increment_known = true;
        // Character::CanIncrementSkill compares the current u16 saved rank
        // against Character::GetLevel - GetCharSkill(position)->Level.
        next.can_increment = static_cast<std::int64_t>(next.rank) <=
            static_cast<std::int64_t>(next.character_level) - next.required_character_level;
        const auto assignable = tables.skills()[static_cast<std::size_t>(table_id)].scalar.words[18] !=
                                std::numeric_limits<std::uint32_t>::max();
        next.equippable_known = true;
        next.equippable = next.available && assignable && next.rank > 0;
    }

    if (caps.known && next.rank_known && state.source_points_known) {
        next.rank_cap = caps.max_skill_level[caps.unlocked_difficulty];
        next.training_known = true;
        next.can_train = state.source_skill_points > 0 && next.available &&
                         next.rank < next.rank_cap && next.can_increment;
    }
    output = next;
    error.clear();
    return true;
}

bool train_skill_v1(
    CharacterState& state, const dh2::data::CharacterTable& characters,
    dh2::data::SkillTables::Borrow tables,
    const CharacterDesignSkillCapsV1& caps, int position,
    std::string& error) {
    SkillProgressionV1 current;
    if (!evaluate_skill_progression_v1(state, characters, std::move(tables), caps,
                                       position, current, error)) return false;
    if (!current.rank_known || current.saved_skill_row < 0)
        return fail(error, "Skill training requires the matching saved CharacterState rank");
    if (!state.source_points_known)
        return fail(error, "Skill training requires source-known skill points");
    if (!caps.known)
        return fail(error, "Skill training requires original CharacterDesign difficulty caps");
    if (!current.can_train) {
        if (!state.source_skill_points)
            return fail(error, "Original skill training requires at least one source skill point");
        if (!current.available)
            return fail(error, "Original skill training requires the SkillTable character-level requirement");
        if (current.rank >= current.rank_cap)
            return fail(error, "Original skill training is at the CharacterDesign difficulty cap");
        return fail(error, "Character::CanIncrementSkill rejected the current rank at this character level");
    }
    if (current.rank == std::numeric_limits<std::uint32_t>::max())
        return fail(error, "Saved skill rank cannot be incremented without overflow");

    // All guards have passed. The original source effects are one point spent
    // and one u16 saved rank incremented; slots and unrelated state stay put.
    --state.source_skill_points;
    ++state.skills[static_cast<std::size_t>(current.saved_skill_row)].rank;
    error.clear();
    return true;
}

} // namespace dh::foundation::generic_skills
