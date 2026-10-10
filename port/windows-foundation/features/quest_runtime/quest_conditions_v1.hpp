#pragma once
// OPENING2: named activation conditions (v2conditions) of the original level declarations
// (`activate_cond` / `deactivate_cond`, for example GameStartOnly, IsAfter_Swamp_Moths).
//
// IDA: the names are the v2conditions name table; each record is 4 words (w0, type, parameter1, parameter2).
// The type indexes GetConditionImplInstance (map `s_conditionImplDataMap`, relocated table at 0x969258):
//   0 Condition_IsQuestInState  (quest parameter1 state == parameter2)
//   1 Condition_IsQuestStateLower
//   2 Condition_IsQuestStateHigher
//   3 Condition_IsPlayerInLevel (level row == parameter1)
//   4 Condition_IsLevelInState, 5 Condition_IsPlayerAtLevel, 6 Condition_IsEventInState (not evaluated here)
// Condition_IsQuestInStateGeneric::Eval reads the quest state from the save (Character::SG_GetQuestByID).
// A quest row with no save record evaluates false (original).
#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include "quest_table_v1.hpp"

namespace dh::foundation::quest_runtime {

struct NamedConditionV1 {
    std::int32_t type{-1};
    std::int32_t parameter1{-1};
    std::int32_t parameter2{-1};
};

// Decodes v2conditions_pyarray.bin and v2conditions_pyarraynames.bin (count, 2 fields, 0, record size 16; records
// at byte 16 with the names in the same order). Malformed input fails atomically.
bool decode_named_conditions_v1(const std::vector<std::uint8_t>& array, const std::vector<std::uint8_t>& names,
    std::map<std::string, NamedConditionV1>& out, std::string& error);

// quest_state(row, state) returns false when the row has no save record.
// Returns false (and sets unsupported) for types the port does not evaluate.
bool evaluate_named_condition_v1(const NamedConditionV1& c,
    const std::function<bool(std::int32_t row, std::int32_t& state)>& quest_state,
    std::int32_t level_row, bool& met, bool& unsupported);

} // namespace dh::foundation::quest_runtime

namespace dh::foundation {
struct CharacterState;
}
namespace dh::foundation::quest_runtime {
// Reads one quest row state from a character's saved quest progress (the same read the runtime uses).
// Returns false when the row has no save record.
// A character with no quest progress yet (a new game before the quest runtime initializes it) reads the authored
// initial state of the row (the same values initialize_fresh writes).
bool quest_state_from_character_v1(const CharacterState& character, const QuestTableV1* table, std::int32_t row, std::int32_t& state);
} // namespace dh::foundation::quest_runtime
