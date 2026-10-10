#include "generic_skills_page_v1.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::generic_skills;

namespace {
using Raw = std::vector<std::uint8_t>;
Raw read(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("missing source table: " + path);
    return {std::istreambuf_iterator<char>(file), {}};
}
void check(bool value, int line) {
    if (!value) throw std::runtime_error("generic skills page test failed line " + std::to_string(line));
}
#define CHECK(x) check((x), __LINE__)

int field(const dh2::data::CharacterTable& characters, const char* name) {
    const auto at = std::find(characters.fields.begin(), characters.fields.end(), name);
    return at == characters.fields.end() ? -1 : static_cast<int>(at - characters.fields.begin());
}

std::vector<std::string> list_names(dh2::data::SkillTables::Borrow tables, int id) {
    std::vector<std::string> names;
    for (const int skill_id : tables.lists().at(static_cast<std::size_t>(id)))
        names.push_back(tables.skill_names().at(static_cast<std::size_t>(skill_id)));
    return names;
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
        const int skill_tree = field(characters, "SkillTree");
        CHECK(skill_tree == 28);

        // Exercise each actual class row whose original SkillTree points into
        // the actual SkillTables. No page rows are synthesized from class IDs.
        std::size_t checked_classes = 0;
        std::size_t checked_rows = 0;
        for (std::size_t class_row = 0; class_row < characters.names.size(); ++class_row) {
            const int list_id = characters.rows.at(class_row).at(static_cast<std::size_t>(skill_tree));
            if (list_id < 0 || static_cast<std::size_t>(list_id) >= tables.lists().size()) continue;
            CharacterState state;
            state.class_id = characters.names[class_row];
            PageV1 page(state, characters, tables);
            ViewV1 view;
            CHECK(page.view(view, error));
            CHECK(view.character_row == static_cast<int>(class_row));
            CHECK(view.authored_skill_list_id == list_id);
            CHECK(view.active_skill_list_id == list_id);
            CHECK(view.rows.size() == tables.lists()[static_cast<std::size_t>(list_id)].size());
            CHECK(!view.skill_points_known && !view.skill_points);
            CHECK(!view.slots_source_known && view.slots.empty());
            for (std::size_t position = 0; position < view.rows.size(); ++position) {
                const auto& row = view.rows[position];
                const auto source_id = tables.lists()[static_cast<std::size_t>(list_id)][position];
                CHECK(row.position == static_cast<int>(position));
                CHECK(row.table_id == source_id);
                CHECK(row.source_name == tables.skill_names().at(static_cast<std::size_t>(source_id)));
                CHECK(row.source_icon == tables.skills().at(static_cast<std::size_t>(source_id)).icon);
                CHECK(!row.saved_rank && !row.saved_skill_row);
                CHECK(!row.localized_name && !row.description);
                ++checked_rows;
            }
            ++checked_classes;
        }
        CHECK(checked_classes > 0 && checked_rows > 0);

        // Resolve a real source class and an actual different saved SkillList.
        const auto knight = std::find(characters.names.begin(), characters.names.end(), "KnightPlayerBase");
        CHECK(knight != characters.names.end());
        const int knight_row = static_cast<int>(knight - characters.names.begin());
        const int knight_list = characters.rows[static_cast<std::size_t>(knight_row)][static_cast<std::size_t>(skill_tree)];
        int override_list = -1;
        for (std::size_t i = 0; i < tables.lists().size(); ++i) {
            if (static_cast<int>(i) != knight_list && tables.lists()[i].size() > 1 &&
                tables.lists()[i] != tables.lists()[static_cast<std::size_t>(knight_list)]) {
                override_list = static_cast<int>(i);
                break;
            }
        }
        CHECK(override_list >= 0);
        CharacterState state;
        state.id = "same-character-state";
        state.class_id = "KnightPlayerBase";
        state.source_points_known = true;
        state.source_skill_points = 6;
        state.source_skill_slots_known = true;
        const auto saved_names = list_names(tables, override_list);
        for (std::size_t i = 0; i < saved_names.size(); ++i)
            state.skills.push_back({saved_names[i], i == 0 ? 0u : static_cast<std::uint32_t>(i + 1)});
        state.skill_slots.push_back({0, 0, 1});

        int localized_calls = 0;
        int assign_calls = 0;
        int train_calls = 0;
        const auto* original_state = &state;
        ServicesV1 services;
        services.localized_text = [&](const CharacterState& shared, int table_id,
                                      std::optional<std::string>& name,
                                      std::optional<std::string>& description,
                                      std::string&) {
            CHECK(&shared == original_state);
            CHECK(table_id >= 0 && static_cast<std::size_t>(table_id) < tables.skills().size());
            ++localized_calls;
            name = "localized:" + tables.skill_names()[static_cast<std::size_t>(table_id)];
            description = "source-text-provider:" + std::to_string(table_id);
            return true;
        };
        services.can_assign = [&](const CharacterState& shared, int table_id, int position,
                                  bool& available, std::string&) {
            CHECK(&shared == original_state);
            CHECK(state.skills.at(static_cast<std::size_t>(position)).id ==
                  tables.skill_names().at(static_cast<std::size_t>(table_id)));
            ++assign_calls;
            available = true;
            return true;
        };
        services.train = [&](CharacterState& shared, int list_id, int position, int table_id,
                             std::string&) {
            CHECK(&shared == original_state);
            CHECK(list_id == override_list);
            CHECK(table_id == tables.lists()[static_cast<std::size_t>(override_list)][static_cast<std::size_t>(position)]);
            ++train_calls;
            ++shared.skills.at(static_cast<std::size_t>(position)).rank;
            --shared.source_skill_points;
            return true;
        };
        PageV1 page(state, characters, tables, services);
        ViewV1 view;
        CHECK(page.view(view, error));
        CHECK(view.authored_skill_list_id == knight_list);
        CHECK(view.active_skill_list_id == override_list && view.active_list_from_saved_rows);
        CHECK(view.skill_points_known && view.skill_points == 6u);
        CHECK(view.slots_source_known && view.slots.size() == 1);
        CHECK(view.slots[0].saved_skill_row == 1u);
        CHECK(view.slots[0].class_skill_position == 1);
        CHECK(view.rows.size() == saved_names.size());
        CHECK(localized_calls == static_cast<int>(view.rows.size()));
        CHECK(view.rows[0].saved_rank == 0u && view.rows[0].saved_skill_row == 0u);
        CHECK(view.rows[0].localized_name == "localized:" + view.rows[0].source_name);
        CHECK(view.rows[0].description.has_value());

        // The assignment target comes from the original authored row, while
        // the resulting saved_skill_row indexes this same CharacterState.
        std::size_t assignable_position = view.rows.size();
        for (std::size_t i = 0; i < view.rows.size(); ++i) {
            const auto& row = view.rows[i];
            if (state.skills[i].rank > 0 &&
                tables.skills()[static_cast<std::size_t>(row.table_id)].scalar.words[18] != 0xffffffffu) {
                assignable_position = i;
                break;
            }
        }
        CHECK(assignable_position < view.rows.size());
        CHECK(page.select(static_cast<int>(assignable_position), error));
        const auto previous_size = state.skill_slots.size();
        CHECK(page.assign(1, 2, error));
        CHECK(assign_calls == 1 && state.skill_slots.size() == previous_size + 1);
        CHECK(state.skill_slots.back().equipment_set == 1 && state.skill_slots.back().slot == 2);
        CHECK(state.skill_slots.back().saved_skill_row == assignable_position);
        CHECK(&state == original_state);

        const auto slots_before_rejection = state.skill_slots;
        std::size_t nonassignable_position = view.rows.size();
        for (std::size_t i = 0; i < view.rows.size(); ++i) {
            if (state.skills[i].rank > 0 &&
                tables.skills()[static_cast<std::size_t>(view.rows[i].table_id)].scalar.words[18] == 0xffffffffu) {
                nonassignable_position = i;
                break;
            }
        }
        if (nonassignable_position < view.rows.size()) {
            CHECK(page.select(static_cast<int>(nonassignable_position), error));
            const auto assign_calls_before = assign_calls;
            CHECK(!page.assign(0, 0, error));
            CHECK(assign_calls == assign_calls_before && state.skill_slots.size() == slots_before_rejection.size());
        }
        CHECK(page.select(static_cast<int>(assignable_position), error));
        services.can_assign = [](const CharacterState&, int, int, bool& available, std::string&) {
            available = false;
            return true;
        };
        PageV1 unavailable(state, characters, tables, services);
        CHECK(unavailable.select(static_cast<int>(assignable_position), error));
        CHECK(!unavailable.assign(0, 0, error));
        CHECK(state.skill_slots.size() == slots_before_rejection.size());

        const auto old_rank = state.skills[assignable_position].rank;
        CHECK(page.train(error));
        CHECK(train_calls == 1 && state.skills[assignable_position].rank == old_rank + 1);
        CHECK(state.source_skill_points == 5 && &state == original_state);

        const auto before_failed = state;
        services.train = [&](CharacterState& shared, int, int position, int, std::string&) {
            CHECK(&shared == original_state);
            ++shared.skills.at(static_cast<std::size_t>(position)).rank;
            shared.source_skill_points = 999;
            return false;
        };
        PageV1 rejected(state, characters, tables, services);
        CHECK(rejected.select(static_cast<int>(assignable_position), error));
        CHECK(!rejected.train(error));
        CHECK(state.skills[assignable_position].rank == before_failed.skills[assignable_position].rank);
        CHECK(state.source_skill_points == before_failed.source_skill_points);

        // Unknown or malformed state stays explicit and cannot invent a rank,
        // localized description, slot, or a missing class mapping.
        CharacterState unknown;
        unknown.class_id = "not-an-original-class-token";
        PageV1 unsupported(unknown, characters, tables);
        CHECK(!unsupported.view(view, error));
        CHECK(error.find("CharacterTable") != std::string::npos);
        CharacterState malformed_source;
        malformed_source.class_id = "KnightPlayerBase";
        malformed_source.source_skill_slots_known = true;
        malformed_source.skills.push_back({"not-a-source-skill", 88});
        PageV1 malformed(malformed_source, characters, tables);
        CHECK(!malformed.view(view, error));
        CHECK(error.find("do not match") != std::string::npos);
        malformed_source.skills.clear();
        PageV1 missing_saved_rows(malformed_source, characters, tables);
        CHECK(!missing_saved_rows.view(view, error));
        CHECK(error.find("no saved") != std::string::npos);
        CHECK(!page.select(-1, error));
        CHECK(!page.assign(0, 3, error));

        std::cout << "{\"validation\":\"PASS\",\"actual_class_rows\":" << checked_classes
                  << ",\"actual_skill_positions\":" << checked_rows
                  << ",\"same_state_assignments\":" << assign_calls
                  << ",\"same_state_training_provider_calls\":" << train_calls
                  << ",\"native_progression_policy_claimed\":false}\n";
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
        return 1;
    }
}
