#include "runtime_skill_progression_v1.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::generic_skills;

namespace {
using Bytes = std::vector<std::uint8_t>;
Bytes read(const std::string& path) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream) throw std::runtime_error("missing source table: " + path);
    return {std::istreambuf_iterator<char>(stream), {}};
}
void check(bool value, int line) {
    if (!value) throw std::runtime_error("runtime skill progression test failed at line " + std::to_string(line));
}
#define CHECK(value) check((value), __LINE__)

bool same_skills(const std::vector<SkillProgress>& a, const std::vector<SkillProgress>& b) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i)
        if (a[i].id != b[i].id || a[i].rank != b[i].rank) return false;
    return true;
}

std::int32_t signed_word(std::uint32_t raw) {
    std::int32_t value{};
    std::memcpy(&value, &raw, sizeof(value));
    return value;
}
} // namespace

int main(int argc, char** argv) {
    try {
        CHECK(argc == 2);
        const std::string root = argv[1];
        auto skill_data = read(root + "/skills_pyarray.bin");
        auto skill_names = read(root + "/skills_pyarraynames.bin");
        auto skill_schema = read(root + "/skills_pystructnames.bin");
        dh2::data::SkillTables skill_owner;
        std::string error;
        CHECK(skill_owner.load({skill_data.data(), skill_data.size()},
                               {skill_names.data(), skill_names.size()},
                               {skill_schema.data(), skill_schema.size()}, error));
        const auto tables = skill_owner.borrow();

        auto character_data = read(root + "/character_properties_pyarray.bin");
        auto character_names = read(root + "/character_properties_pyarraynames.bin");
        auto character_schema = read(root + "/character_properties_pystructnames.bin");
        dh2::data::CharacterTable characters;
        CHECK(dh2::data::load_characters({character_data.data(), character_data.size()},
                                         {character_names.data(), character_names.size()},
                                         {character_schema.data(), character_schema.size()},
                                         characters, error));

        const auto class_at = std::find(characters.names.begin(), characters.names.end(), "KnightPlayerBase");
        CHECK(class_at != characters.names.end());
        const auto class_row = static_cast<std::size_t>(class_at - characters.names.begin());
        const auto tree_at = std::find(characters.fields.begin(), characters.fields.end(), "SkillTree");
        CHECK(tree_at != characters.fields.end());
        const auto tree_column = static_cast<std::size_t>(tree_at - characters.fields.begin());
        const int list_id = characters.rows[class_row][tree_column];
        CHECK(list_id >= 0 && static_cast<std::size_t>(list_id) < tables.lists().size());
        const auto& list = tables.lists()[static_cast<std::size_t>(list_id)];

        std::size_t position = list.size();
        for (std::size_t i = 0; i < list.size(); ++i) {
            if (list[i] < 0 || static_cast<std::size_t>(list[i]) >= tables.skills().size()) continue;
            const auto required = signed_word(tables.skills()[static_cast<std::size_t>(list[i])].scalar.words[8]);
            if (required >= 1) { position = i; break; }
        }
        CHECK(position < list.size());
        const auto table_id = list[position];
        const auto& record = tables.skills()[static_cast<std::size_t>(table_id)];
        const auto required = signed_word(record.scalar.words[8]);

        CharacterState state;
        state.id = "same-generic-character";
        state.class_id = "KnightPlayerBase";
        state.stats.level = static_cast<std::uint32_t>(required);
        state.source_points_known = true;
        state.source_skill_points = 2;
        state.source_skill_slots_known = true;
        for (const auto skill : list) {
            CHECK(skill >= 0 && static_cast<std::size_t>(skill) < tables.skill_names().size());
            state.skills.push_back({tables.skill_names()[static_cast<std::size_t>(skill)], 0});
        }
        state.skill_slots.push_back({0, 1, static_cast<std::uint32_t>(position)});

        // Values are the original CharacterDesign constants loaded and
        // reported by runtime_creation_source_loader_v1: B=10, C=15, D=20.
        CharacterDesignSkillCapsV1 caps{true, 0, {10, 15, 20}};
        SkillProgressionV1 result;
        CharacterState unknown_rank;
        unknown_rank.class_id = "KnightPlayerBase";
        unknown_rank.stats.level = static_cast<std::uint32_t>(required);
        unknown_rank.source_points_known = true;
        unknown_rank.source_skill_points = 2;
        CHECK(evaluate_skill_progression_v1(unknown_rank, characters, tables, caps,
                                            static_cast<int>(position), result, error));
        CHECK(result.available && !result.rank_known && !result.equippable_known &&
              !result.can_increment_known && !result.training_known);

        CHECK(evaluate_skill_progression_v1(state, characters, tables, caps,
                                            static_cast<int>(position), result, error));
        CHECK(result.character_row == static_cast<int>(class_row));
        CHECK(result.skill_list_id == list_id && result.skill_table_id == table_id);
        CHECK(result.saved_skill_row == static_cast<int>(position));
        CHECK(result.rank_known && result.rank == 0);
        CHECK(result.available && result.can_increment_known && result.can_increment);
        CHECK(result.equippable_known && !result.equippable);
        CHECK(result.training_known && result.can_train && result.rank_cap == 10);

        auto not_yet_available = state;
        --not_yet_available.stats.level;
        CHECK(evaluate_skill_progression_v1(not_yet_available, characters, tables, caps,
                                            static_cast<int>(position), result, error));
        CHECK(!result.available && !result.can_increment && !result.can_train);
        const auto unavailable_snapshot = not_yet_available;
        CHECK(!train_skill_v1(not_yet_available, characters, tables, caps,
                              static_cast<int>(position), error));
        CHECK(not_yet_available.source_skill_points == unavailable_snapshot.source_skill_points &&
              same_skills(not_yet_available.skills, unavailable_snapshot.skills) &&
              not_yet_available.skill_slots.size() == unavailable_snapshot.skill_slots.size());

        const auto original_slots = state.skill_slots;
        const auto original_state = &state;
        CHECK(train_skill_v1(state, characters, tables, caps, static_cast<int>(position), error));
        CHECK(&state == original_state);
        CHECK(state.skills[position].rank == 1 && state.source_skill_points == 1);
        CHECK(state.skill_slots.size() == original_slots.size());
        CHECK(state.skill_slots[0].equipment_set == original_slots[0].equipment_set &&
              state.skill_slots[0].slot == original_slots[0].slot &&
              state.skill_slots[0].saved_skill_row == original_slots[0].saved_skill_row);

        CHECK(evaluate_skill_progression_v1(state, characters, tables, caps,
                                            static_cast<int>(position), result, error));
        const bool source_assignable = record.scalar.words[18] != 0xffffffffu;
        CHECK(result.equippable == source_assignable);
        CHECK(result.can_increment == (std::int64_t(1) <=
              std::int64_t(state.stats.level) - required));

        if (!result.can_increment) {
            auto level_locked = state;
            const auto level_snapshot = level_locked;
            CHECK(!train_skill_v1(level_locked, characters, tables, caps,
                                  static_cast<int>(position), error));
            CHECK(level_locked.source_skill_points == level_snapshot.source_skill_points &&
                  same_skills(level_locked.skills, level_snapshot.skills) &&
                  level_locked.skill_slots.size() == level_snapshot.skill_slots.size());
        }

        // Source point rejection is atomic.
        auto point_rejected = state;
        point_rejected.source_skill_points = 0;
        const auto point_snapshot = point_rejected;
        CHECK(!train_skill_v1(point_rejected, characters, tables, caps,
                              static_cast<int>(position), error));
        CHECK(point_rejected.source_skill_points == point_snapshot.source_skill_points &&
              same_skills(point_rejected.skills, point_snapshot.skills) &&
              point_rejected.skill_slots.size() == point_snapshot.skill_slots.size());

        // CharacterDesign's actual normal cap is exclusive: rank 10 cannot
        // become 11 even when points and character level are sufficient.
        auto cap_rejected = state;
        cap_rejected.stats.level = 100;
        cap_rejected.skills[position].rank = 10;
        const auto cap_snapshot = cap_rejected;
        CHECK(!train_skill_v1(cap_rejected, characters, tables, caps,
                              static_cast<int>(position), error));
        CHECK(cap_rejected.source_skill_points == cap_snapshot.source_skill_points &&
              cap_rejected.skills[position].rank == cap_snapshot.skills[position].rank &&
              cap_rejected.skill_slots.size() == cap_snapshot.skill_slots.size());

        // Missing design caps leave training unknown and fail closed without
        // changing rank, points, or saved hotbar mappings.
        auto unknown_caps_state = state;
        CharacterDesignSkillCapsV1 unknown_caps;
        CHECK(evaluate_skill_progression_v1(unknown_caps_state, characters, tables,
                                            unknown_caps, static_cast<int>(position), result, error));
        CHECK(!result.training_known && !result.can_train);
        const auto unknown_snapshot = unknown_caps_state;
        CHECK(!train_skill_v1(unknown_caps_state, characters, tables, unknown_caps,
                              static_cast<int>(position), error));
        CHECK(unknown_caps_state.source_skill_points == unknown_snapshot.source_skill_points &&
              unknown_caps_state.skills[position].rank == unknown_snapshot.skills[position].rank &&
              unknown_caps_state.skill_slots.size() == unknown_snapshot.skill_slots.size());

        std::cout << "runtime skill progression PASS: actual KnightPlayerBase SkillList/SkillTable row "
                  << table_id << ", available, CanIncrementSkill, IsSkillEquippable, design cap, "
                     "points, and transactional same-state rank update\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
