#include "runtime_skill_mana_v1.hpp"

#include "../../../game-data/data.hpp"
#include "../../../game-data/skill_tables.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
using Bytes = std::vector<std::uint8_t>;
Bytes read(const std::string& path) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream) throw std::runtime_error("missing source table: " + path);
    return {std::istreambuf_iterator<char>(stream), {}};
}
void check(bool value, int line) {
    if (!value) throw std::runtime_error("runtime skill mana test failed at line " +
                                         std::to_string(line));
}
#define CHECK(value) check((value), __LINE__)
} // namespace

int main(int argc, char** argv) {
    try {
        CHECK(argc == 2);
        const std::string root = argv[1] + std::string("/original-cache/data/pydata/");
        auto character_data = read(root + "character_properties_pyarray.bin");
        auto character_names = read(root + "character_properties_pyarraynames.bin");
        auto character_schema = read(root + "character_properties_pystructnames.bin");
        auto class_data = read(root + "character_classes_pyarray.bin");
        auto class_names = read(root + "character_classes_pyarraynames.bin");
        auto class_schema = read(root + "character_classes_pystructnames.bin");
        auto skill_data = read(root + "skills_pyarray.bin");
        auto skill_names = read(root + "skills_pyarraynames.bin");
        auto skill_schema = read(root + "skills_pystructnames.bin");

        dh2::data::CharacterTable characters;
        dh2::data::ClassTables classes;
        dh2::data::PropertyRules rules;
        dh2::data::SkillTables skill_owner;
        std::string error;
        CHECK(dh2::data::load_characters({character_data.data(), character_data.size()},
                                         {character_names.data(), character_names.size()},
                                         {character_schema.data(), character_schema.size()},
                                         characters, error));
        CHECK(dh2::data::load_classes({class_data.data(), class_data.size()},
                                      {class_names.data(), class_names.size()},
                                      {class_schema.data(), class_schema.size()},
                                      classes, error));
        CHECK(dh2::data::load_property_rules(characters, rules, error));
        CHECK(skill_owner.load({skill_data.data(), skill_data.size()},
                               {skill_names.data(), skill_names.size()},
                               {skill_schema.data(), skill_schema.size()}, error));
        const auto skills = skill_owner.borrow();
        CHECK(characters.fields.size() == 224);
        CHECK(characters.fields[41] == "MP" && characters.fields[43] == "Max_MP");
        CHECK(characters.fields[172] == "SnS_Level" && characters.fields[173] == "SnS_ManaCost");

        const auto player = std::find(characters.names.begin(), characters.names.end(),
                                      "KnightPlayerBase");
        CHECK(player != characters.names.end());
        const auto player_row = static_cast<std::size_t>(player - characters.names.begin());
        const auto tree_field = std::find(characters.fields.begin(), characters.fields.end(),
                                          "SkillTree");
        CHECK(tree_field != characters.fields.end());
        const auto tree_column = static_cast<std::size_t>(tree_field - characters.fields.begin());
        const auto skill_list_id = characters.rows[player_row][tree_column];
        CHECK(skill_list_id >= 0 && static_cast<std::size_t>(skill_list_id) < skills.lists().size());
        CHECK(!skills.lists()[static_cast<std::size_t>(skill_list_id)].empty());
        const auto skill_table_id = skills.lists()[static_cast<std::size_t>(skill_list_id)][0];
        CHECK(skill_table_id == 7);
        CHECK(skills.skill_names()[static_cast<std::size_t>(skill_table_id)] == "BashDown");
        CHECK(skills.skills()[static_cast<std::size_t>(skill_table_id)].script ==
              "prince_warrior_bashdown");
        dh2::data::PropertyState player_properties;
        dh2::data::reset_properties(rules, player_properties, &characters.rows[player_row]);
        CHECK(dh2::data::recalc_properties_with_class(classes, rules, player_properties, error));
        const auto current_before = player_properties.resolved;

        const auto skill_class = std::find(classes.names.begin(), classes.names.end(),
                                           "Skill_Warrior_BashDown");
        CHECK(skill_class != classes.names.end());
        const auto skill_class_id = static_cast<std::int32_t>(skill_class - classes.names.begin());

        for (const std::uint32_t rank : {0u, 1u, 2u, 7u, 65535u}) {
            dh::foundation::generic_skills::SkillManaCostV1 actual;
            CHECK(dh::foundation::generic_skills::evaluate_skill_mana_cost_v1(
                classes, rules, player_properties.resolved, "Skill_Warrior_BashDown",
                rank, actual, error));
            CHECK(actual.class_table_id == skill_class_id && actual.rank == rank);
            CHECK(actual.fixed_skill_level == static_cast<std::int32_t>(rank << 8));

            // Independent source-table projection of the exact authored
            // CalcManaCost operation order for this real class row.
            auto expected = rules.defaults;
            expected[172] = actual.fixed_skill_level;
            CHECK(dh2::data::apply_class(classes, skill_class_id, expected, error,
                                         &player_properties.resolved));
            CHECK(actual.fixed_mana_cost == expected[173]);
            CHECK(player_properties.resolved == current_before);
        }

        dh::foundation::generic_skills::SkillManaCostV1 rejected;
        CHECK(!dh::foundation::generic_skills::evaluate_skill_mana_cost_v1(
            classes, rules, player_properties.resolved, "invented_skill_class", 1,
            rejected, error));
        CHECK(!error.empty());
        CHECK(!dh::foundation::generic_skills::evaluate_skill_mana_cost_v1(
            classes, rules, player_properties.resolved, "Skill_Warrior_BashDown",
            65536, rejected, error));
        CHECK(!error.empty());
        std::cout << "PASS original ClassTables CalcManaCost: BashDown ranks 0/1/2/7/65535, "
                     "same resolved-property buff input, raw SnS_ManaCost, no owner mutation; "
                     "unknown class/u16 overflow rejected\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
        return 1;
    }
}
