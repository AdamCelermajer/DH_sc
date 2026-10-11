#include "quest_conditions_v1.hpp"

#include <cstring>

namespace dh::foundation::quest_runtime {

namespace {
bool read_u32(const std::vector<std::uint8_t>& b, std::size_t at, std::uint32_t& v) {
    if (at + 4 > b.size()) return false;
    v = std::uint32_t(b[at]) | (std::uint32_t(b[at + 1]) << 8) | (std::uint32_t(b[at + 2]) << 16) | (std::uint32_t(b[at + 3]) << 24);
    return true;
}
} // namespace

bool decode_named_conditions_v1(const std::vector<std::uint8_t>& array, const std::vector<std::uint8_t>& names,
    std::map<std::string, NamedConditionV1>& out, std::string& error) {
    std::uint32_t count = 0, fields = 0, zero = 0, record = 0;
    if (!read_u32(array, 0, count) || !read_u32(array, 4, fields) || !read_u32(array, 8, zero) || !read_u32(array, 12, record)) {
        error = "v2conditions array header is truncated";
        return false;
    }
    if (record != 16 || fields != 2) {
        error = "v2conditions array has an unexpected record layout";
        return false;
    }
    std::vector<std::string> labels;
    std::size_t p = 0;
    std::uint32_t n = 0;
    if (!read_u32(names, p, n)) { error = "v2conditions names are truncated"; return false; }
    p += 4;
    for (std::uint32_t i = 0; i < n; ++i) {
        std::uint32_t len = 0;
        if (!read_u32(names, p, len) || p + 4 + len > names.size()) { error = "v2conditions name is truncated"; return false; }
        labels.emplace_back(reinterpret_cast<const char*>(names.data() + p + 4), len);
        p += 4 + len;
    }
    if (labels.size() != count || array.size() < 16 + std::size_t(count) * 16) {
        error = "v2conditions names and records disagree";
        return false;
    }
    std::map<std::string, NamedConditionV1> decoded;
    for (std::uint32_t i = 0; i < count; ++i) {
        const std::size_t at = 16 + std::size_t(i) * 16;
        std::uint32_t type = 0, p1 = 0, p2 = 0;
        read_u32(array, at + 4, type);
        read_u32(array, at + 8, p1);
        read_u32(array, at + 12, p2);
        decoded[labels[i]] = NamedConditionV1{std::int32_t(type), std::int32_t(p1), std::int32_t(p2)};
    }
    out = std::move(decoded);
    return true;
}

bool evaluate_named_condition_v1(const NamedConditionV1& c,
    const std::function<bool(std::int32_t row, std::int32_t& state)>& quest_state,
    std::int32_t level_row, bool& met, bool& unsupported) {
    met = false;
    unsupported = false;
    switch (c.type) {
    case 0: case 1: case 2: {
        std::int32_t state = 0;
        if (!quest_state || !quest_state(c.parameter1, state)) return true;   // no save record: false
        if (c.type == 0) met = state == c.parameter2;
        else if (c.type == 1) met = state < c.parameter2;
        else met = state > c.parameter2;
        return true;
    }
    case 3:
        met = level_row >= 0 && level_row == c.parameter1;
        return true;
    default:
        unsupported = true;
        return false;
    }
}

} // namespace dh::foundation::quest_runtime

#include "../../character_state.hpp"
#include "../quests/character_quest_progress_v1.hpp"

namespace dh::foundation::quest_runtime {

bool quest_state_from_character_v1(const CharacterState& character, const QuestTableV1* table, std::int32_t row, std::int32_t& state) {
    if (character.source_quest_progress_cqpg.empty()) {
        if (!table || !table->ready() || row < 0 || std::size_t(row) >= table->rows().size()) return false;
        state = table->rows()[std::size_t(row)].state;
        return true;
    }
    CharacterQuestProgressV1 progress;
    CharacterQuestStateV1 found;
    std::string error;
    if (!progress.query(character, CharacterQuestIdV1{0, character_current_difficulty(character), row}, found, error)) return false;
    state = found.state;
    return true;
}

} // namespace dh::foundation::quest_runtime
