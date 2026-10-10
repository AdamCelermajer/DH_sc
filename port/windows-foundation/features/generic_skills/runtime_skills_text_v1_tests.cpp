#include "runtime_skills_text_v1.hpp"
#include "../character_menu/skill_page_text_projection_v1.hpp"

#include "../../asset_catalog.hpp"
#include "../../../game-data/properties.hpp"
#include "../../../game-data/class_tables.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
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
    if (!value) throw std::runtime_error("runtime skills text test failed at line " + std::to_string(line));
}
#define CHECK(value) check((value), __LINE__)

std::int32_t signed_word(std::uint32_t raw) {
    std::int32_t value{};
    std::memcpy(&value, &raw, sizeof(value));
    return value;
}

std::uint32_t read_u32(std::istream& in) {
    std::uint32_t value{};
    if (!in.read(reinterpret_cast<char*>(&value), sizeof(value)))
        throw std::runtime_error("truncated original SkillInfo text fixture");
    return value;
}
std::string read_string(std::istream& in) {
    const auto size = read_u32(in);
    if (size > 1024 * 1024) throw std::runtime_error("original SkillInfo string exceeds source bound");
    std::string value(size, '\0');
    if (!in.read(value.data(), static_cast<std::streamsize>(size)))
        throw std::runtime_error("truncated original SkillInfo string");
    return value;
}
struct SourceSkillInfoText {
    std::string script;
    std::uint32_t skill_rank{};
    std::string current;
    std::string next;
};
std::vector<SourceSkillInfoText> read_source_skill_info_text(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("missing original SkillInfo text fixture: " + path);
    if (read_u32(in) != 0x31435348u) throw std::runtime_error("original SkillInfo fixture magic mismatch");
    const auto count = read_u32(in);
    if (count > 4096) throw std::runtime_error("original SkillInfo fixture case count exceeds bound");
    std::vector<SourceSkillInfoText> result;
    for (std::uint32_t i = 0; i < count; ++i) {
        const auto record_bytes = read_u32(in);
        const auto begin = in.tellg();
        SourceSkillInfoText row;
        row.script = read_string(in);
        (void)read_string(in); // source actor identity
        (void)read_string(in); // source class identity
        (void)read_u32(in);    // source actor row
        (void)read_u32(in);    // source class row
        row.skill_rank = read_u32(in);
        std::array<char, 896> source_sheet{};
        if (!in.read(source_sheet.data(), static_cast<std::streamsize>(source_sheet.size())))
            throw std::runtime_error("truncated original SkillInfo property sheet");
        row.current = read_string(in);
        row.next = read_string(in);
        if (in.tellg() - begin != static_cast<std::streamoff>(record_bytes))
            throw std::runtime_error("original SkillInfo fixture record size mismatch");
        result.push_back(std::move(row));
    }
    if (in.peek() != EOF) throw std::runtime_error("trailing bytes in original SkillInfo fixture");
    return result;
}
} // namespace

int main(int argc, char** argv) {
    try {
        CHECK(argc == 3);
        const std::string root = argv[1];
        auto skill_data = read(root + "/original-cache/data/pydata/skills_pyarray.bin");
        auto skill_names = read(root + "/original-cache/data/pydata/skills_pyarraynames.bin");
        auto skill_schema = read(root + "/original-cache/data/pydata/skills_pystructnames.bin");
        dh2::data::SkillTables skill_owner;
        std::string error;
        CHECK(skill_owner.load({skill_data.data(), skill_data.size()},
                               {skill_names.data(), skill_names.size()},
                               {skill_schema.data(), skill_schema.size()}, error));
        const auto tables = skill_owner.borrow();

        auto character_data = read(root + "/original-cache/data/pydata/character_properties_pyarray.bin");
        auto character_names = read(root + "/original-cache/data/pydata/character_properties_pyarraynames.bin");
        auto character_schema = read(root + "/original-cache/data/pydata/character_properties_pystructnames.bin");
        auto class_data = read(root + "/original-cache/data/pydata/character_classes_pyarray.bin");
        auto class_names = read(root + "/original-cache/data/pydata/character_classes_pyarraynames.bin");
        auto class_schema = read(root + "/original-cache/data/pydata/character_classes_pystructnames.bin");
        dh2::data::CharacterTable characters;
        dh2::data::ClassTables classes;
        dh2::data::PropertyRules rules;
        CHECK(dh2::data::load_characters({character_data.data(), character_data.size()},
                                         {character_names.data(), character_names.size()},
                                         {character_schema.data(), character_schema.size()},
                                         characters, error));
        CHECK(dh2::data::load_classes({class_data.data(), class_data.size()},
                                      {class_names.data(), class_names.size()},
                                      {class_schema.data(), class_schema.size()}, classes, error));
        CHECK(dh2::data::load_property_rules(characters, rules, error));

        // Audit every visible Rogue Skills cell using actual CharacterTable
        // specialization SkillTrees and source SkillTable fields. The source
        // SWF's SkillUnlocked is produced by NativeGetSkillDetails from the
        // same Character level vs SkillTable Level; its presetAllSkills then
        // applies SkillAssignable, rank, icon, and unlocked masks.
        const auto tree_field = std::find(characters.fields.begin(), characters.fields.end(), "SkillTree");
        CHECK(tree_field != characters.fields.end());
        const auto tree_column = static_cast<std::size_t>(tree_field - characters.fields.begin());
        struct RogueTree { const char* class_id; int expected_tree; const char* spec_name; };
        const RogueTree rogue_trees[] = {
            {"RoguePlayerBase", 27, "SpecializationSkill"},
            {"RoguePlayerBase_Archer", 28, "Volley"},
            {"RoguePlayerBase_Assassin", 29, "GrapplingHook"},
        };
        CharacterState rogue;
        rogue.id = "same-source-rogue-class-spec-transition";
        rogue.stats.level = 1;
        std::size_t rogue_source_cells = 0;
        for (const auto& tree : rogue_trees) {
            const auto class_at = std::find(characters.names.begin(), characters.names.end(), tree.class_id);
            CHECK(class_at != characters.names.end());
            const auto class_row = static_cast<std::size_t>(class_at - characters.names.begin());
            const auto list_id = characters.rows[class_row][tree_column];
            CHECK(list_id == tree.expected_tree);
            const auto& source_list = tables.lists()[static_cast<std::size_t>(list_id)];
            CHECK(source_list.size() == 16);
            rogue.class_id = tree.class_id;
            rogue.skills.clear();
            for (std::size_t position = 0; position < source_list.size(); ++position) {
                const auto id = source_list[position];
                CHECK(id >= 0 && static_cast<std::size_t>(id) < tables.skills().size());
                rogue.skills.push_back({tables.skill_names()[static_cast<std::size_t>(id)],
                                        position == 0 ? 1u : 0u});
            }
            PageV1 source_page(rogue, characters, tables);
            ViewV1 source_view;
            CHECK(source_page.view(source_view, error));
            CHECK(source_view.active_skill_list_id == list_id && source_view.rows.size() == 16);
            CHECK(source_view.rows[8].source_name == tree.spec_name);
            character_menu::SkillPageTextInputV1 actual_cells;
            for (std::size_t position = 0; position < 16; ++position) {
                const auto& row = source_view.rows[position];
                const auto& record = tables.skills()[static_cast<std::size_t>(row.table_id)];
                CHECK(row.saved_rank.has_value());
                actual_cells.cells[position] = {
                    static_cast<int>(*row.saved_rank),
                    !(static_cast<int>(rogue.stats.level) < row.required_level),
                    record.scalar.words[11] != 0,
                    row.source_icon.empty() ? "blank" : row.source_icon};
            }
            std::array<character_menu::SkillPageCellPresentationV1, 16> projected{};
            CHECK(character_menu::project_skill_page_cells_v1(actual_cells, projected, error));
            for (std::size_t position = 0; position < 16; ++position) {
                const auto& row = source_view.rows[position];
                const auto& record = tables.skills()[static_cast<std::size_t>(row.table_id)];
                const bool source_assignable = record.scalar.words[11] != 0;
                const bool source_unlocked = !(static_cast<int>(rogue.stats.level) < row.required_level);
                const bool source_nonblank = !record.icon.empty() && record.icon != "blank";
                const auto rank = *row.saved_rank;
                CHECK(projected[position].skill_level == static_cast<int>(rank));
                CHECK(projected[position].skill_icon_frame == (record.icon.empty() ? "blank" : record.icon));
                CHECK(projected[position].button_disabled == (!source_assignable || rank < 1));
                CHECK(projected[position].lock_visible == (!source_unlocked && !(rank < 1 && !source_nonblank)));
                CHECK(projected[position].grey_visible == (rank < 1 && source_nonblank));
                ++rogue_source_cells;
            }
            if (list_id == 27) {
                for (std::size_t position = 0; position < 4; ++position)
                    CHECK(tables.skills()[static_cast<std::size_t>(source_view.rows[position].table_id)]
                              .scalar.words[11] == 1);
                for (std::size_t position = 4; position < 16; ++position)
                    CHECK(tables.skills()[static_cast<std::size_t>(source_view.rows[position].table_id)]
                              .scalar.words[11] == 0);
                for (std::size_t position = 8; position < 16; ++position) {
                    CHECK(source_view.rows[position].source_name == "SpecializationSkill");
                    CHECK(source_view.rows[position].source_icon == "blank");
                    CHECK(projected[position].lock_visible == false &&
                          projected[position].grey_visible == false);
                }
            } else {
                for (std::size_t position = 0; position < 4; ++position)
                    CHECK(tables.skills()[static_cast<std::size_t>(source_view.rows[position].table_id)]
                              .scalar.words[11] == 1);
                for (std::size_t position = 4; position < 8; ++position)
                    CHECK(tables.skills()[static_cast<std::size_t>(source_view.rows[position].table_id)]
                              .scalar.words[11] == 0);
                for (std::size_t position = 8; position < 16; ++position) {
                    CHECK(source_view.rows[position].source_name != "SpecializationSkill");
                    CHECK(!source_view.rows[position].source_icon.empty() &&
                          source_view.rows[position].source_icon != "blank");
                    CHECK(projected[position].lock_visible && projected[position].grey_visible);
                }
                if (list_id == 28) {
                    const bool expected_assignable[] = {true, true, true, true, false, true, false, false};
                    for (std::size_t position = 8; position < 16; ++position)
                        CHECK((tables.skills()[static_cast<std::size_t>(source_view.rows[position].table_id)]
                                   .scalar.words[11] != 0) == expected_assignable[position - 8]);
                } else {
                    for (std::size_t position = 8; position < 12; ++position)
                        CHECK(tables.skills()[static_cast<std::size_t>(source_view.rows[position].table_id)]
                                  .scalar.words[11] == 1);
                    for (std::size_t position = 12; position < 16; ++position)
                        CHECK(tables.skills()[static_cast<std::size_t>(source_view.rows[position].table_id)]
                                  .scalar.words[11] == 0);
                }
            }
        }

        AssetCatalog assets(root);
        character_menu::MenuLocalization localization;
        CHECK(localization.load(assets, "original-cache/data", 0, error));
        CharacterState state;
        state.id = "same-skills-text-state";
        state.class_id = "KnightPlayerBase";
        CHECK(localization.bind_profile(&state, error));

        ServicesV1 services;
        RuntimeSkillsTextProviderV1 provider;
        CHECK(provider.bind(characters, tables, localization, state, services, error));
        CHECK(static_cast<bool>(services.localized_text));

        // Exercise the complete modern OnSkillInfo projection against actual
        // player rows, SkillList order, SkillTable OIDs/display_props and the
        // original ClassTables/property formulas. Only source admission facts
        // (difficulty cap and CanIncrementSkill) are fixture callbacks here;
        // the producer receives the same resolved sheet/state used by this test.
        const auto skill_tree_field = std::find(characters.fields.begin(), characters.fields.end(), "SkillTree");
        CHECK(skill_tree_field != characters.fields.end());
        const auto skill_tree_column = static_cast<std::size_t>(skill_tree_field - characters.fields.begin());
        struct SkillFixture { const char* character; const char* script; const char* class_token; };
        const SkillFixture detail_fixtures[] = {
            {"KnightPlayerBase", "prince_warrior_bashdown", "Skill_Warrior_BashDown"},
            {"RoguePlayerBase", "prince_rogue_jump_kick", "Skill_Rogue_JumpKick"},
        };
        std::size_t detailed_rows = 0;
        for (const auto& fixture : detail_fixtures) {
            const auto character = std::find(characters.names.begin(), characters.names.end(), fixture.character);
            CHECK(character != characters.names.end());
            const auto character_row = static_cast<std::size_t>(character - characters.names.begin());
            const auto list_id = characters.rows[character_row][skill_tree_column];
            CHECK(list_id >= 0 && static_cast<std::size_t>(list_id) < tables.lists().size());
            const auto& list = tables.lists()[static_cast<std::size_t>(list_id)];
            const auto selected = std::find_if(list.begin(), list.end(), [&](int id) {
                return id >= 0 && static_cast<std::size_t>(id) < tables.skills().size() &&
                       tables.skills()[static_cast<std::size_t>(id)].script == fixture.script;
            });
            CHECK(selected != list.end());
            const auto position = static_cast<int>(selected - list.begin());
            const auto table_id = *selected;
            dh2::data::PropertyState properties;
            dh2::data::reset_properties(rules, properties, &characters.rows[character_row]);
            CHECK(dh2::data::recalc_properties_with_class(classes, rules, properties, error));
            auto current_properties = properties;
            CharacterState actual;
            actual.id = std::string("same-state-") + fixture.script;
            actual.class_id = fixture.character;
            actual.stats.level = static_cast<std::uint32_t>(
                static_cast<std::uint32_t>(properties.resolved[19]) >> 8);
            character_menu::MenuLocalization details_localization;
            CHECK(details_localization.load(assets, "original-cache/data", 0, error));
            CHECK(details_localization.bind_profile(&actual, error));
            ServicesV1 details_services;
            RuntimeSkillsTextProviderV1 details_provider;
            CHECK(details_provider.bind(characters, tables, details_localization, actual,
                                        details_services, error));
            RuntimeSkillsDetailsBindingsV1 bindings;
            bindings.classes = &classes;
            bindings.property_rules = &rules;
            bindings.current_resolved = [&](const CharacterState& owner,
                    dh2::data::PropertySheet& result, std::string&) {
                if (&owner != &actual) return false;
                result = current_properties.resolved;
                return true;
            };
            std::int32_t fixture_max_level = 9;
            bool fixture_can_increment = true;
            bindings.max_skill_level = [&](const CharacterState&, std::int32_t& value, std::string&) {
                value = fixture_max_level; return true;
            };
            bindings.can_increment = [&](const CharacterState&, int, bool& value, std::string&) {
                value = fixture_can_increment; return true;
            };
            CHECK(details_provider.bind_details(bindings, details_services, error));
            CHECK(static_cast<bool>(details_services.skill_level_text));

            dh2::ui::HudTextV1* same_hud_text = nullptr;
            dh2::ui::HudTextEnvironmentV1 hud_environment;
            CHECK(details_localization.borrow_text(same_hud_text, hud_environment, error));
            CHECK(same_hud_text != nullptr);
            const auto source_details = tables.skills()[static_cast<std::size_t>(table_id)];
            const auto expected_level_text = [&](std::uint32_t level, int oid_word,
                                                 std::string& expected) {
                SkillTempPropertiesV1 sheet;
                CHECK(evaluate_skill_temp_properties_v1(classes, rules, current_properties.resolved,
                    fixture.class_token, level, sheet, error));
                std::vector<dh2::ui::HudTextVariantV1> args;
                for (const auto prop : source_details.display_props) {
                    CHECK(prop >= 0 && static_cast<std::size_t>(prop) < sheet.properties.size());
                    const auto fixed = sheet.properties[static_cast<std::size_t>(prop)];
                    const auto integer = fixed >= 0 ? fixed / 256 :
                        -1 - static_cast<std::int32_t>((~static_cast<std::uint32_t>(fixed)) >> 8);
                    args.push_back({static_cast<float>(fixed) * (1.0f / 256.0f), integer, nullptr});
                }
                const auto oid = signed_word(source_details.scalar.words[oid_word]);
                CHECK(oid >= 0);
                std::string templated;
                CHECK(details_localization.string_id(oid, templated, error));
                bool changed = false;
                CHECK(same_hud_text->parse_ex(templated.c_str(), args.empty() ? nullptr : args.data(),
                    args.size(), hud_environment, expected, changed, error));
                return true;
            };

            for (std::uint32_t rank : {1u, 2u}) {
                character_menu::SkillPageCurrentNextRequestV1 request{
                    &actual, static_cast<int>(list_id), position, table_id, rank, actual.stats.level};
                character_menu::SkillPageCurrentNextTextV1 details;
                CHECK(details_services.skill_level_text(actual, request, details, error));
                CHECK(details.available && details.current_level != std::string{} &&
                      details.next_level != std::string{});
                CHECK(details.state == &actual && details.saved_rank == rank &&
                      details.skill_table_id == table_id);
                std::string expected_current, expected_next;
                CHECK(expected_level_text(rank, 12, expected_current));
                CHECK(expected_level_text(rank + 1u, 17, expected_next));
                CHECK(details.current_level == expected_current && details.next_level == expected_next);
                ++detailed_rows;
            }

            // Exercise NativeGetSkillDetails' three exact static guard paths
            // against this same source SkillRecord and CharacterState.
            std::string expected_guard;
            fixture_max_level = 9;
            fixture_can_increment = true;
            const auto required = signed_word(source_details.scalar.words[8]);
            if (required > 0) {
                actual.stats.level = static_cast<std::uint32_t>(required - 1);
                current_properties.resolved[19] = static_cast<std::int32_t>((required - 1) * 256);
                character_menu::SkillPageCurrentNextRequestV1 below{
                    &actual, static_cast<int>(list_id), position, table_id, 1, actual.stats.level};
                character_menu::SkillPageCurrentNextTextV1 locked;
                CHECK(details_services.skill_level_text(actual, below, locked, error));
                bool changed = false;
                const std::int32_t level_argument = required;
                const dh2::ui::HudTextVariantV1 argument{
                    static_cast<float>(required), required, nullptr};
                std::string level_template;
                CHECK(details_localization.symbol("GAMEPLAYMENUS_SKILL_UNLOCK_AT_LEVEL", &actual,
                                                  level_template, error));
                CHECK(same_hud_text->parse_ex(level_template.c_str(), &argument, 1,
                    hud_environment, expected_guard, changed, error));
                CHECK(locked.available && locked.current_level == expected_guard &&
                      locked.next_level.empty());
                (void)level_argument;
                actual.stats.level = static_cast<std::uint32_t>(properties.resolved[19] >> 8);
                current_properties = properties;
            }

            character_menu::SkillPageCurrentNextRequestV1 zero_rank{
                &actual, static_cast<int>(list_id), position, table_id, 0, actual.stats.level};
            character_menu::SkillPageCurrentNextTextV1 zero_text;
            CHECK(details_services.skill_level_text(actual, zero_rank, zero_text, error));
            CHECK(details_localization.symbol("GAMEPLAYMENUS_NEEDS_SKILL_POINTS", &actual,
                                               expected_guard, error));
            CHECK(zero_text.current_level == expected_guard);

            fixture_max_level = 1;
            character_menu::SkillPageCurrentNextRequestV1 capped{
                &actual, static_cast<int>(list_id), position, table_id, 1, actual.stats.level};
            character_menu::SkillPageCurrentNextTextV1 capped_text;
            CHECK(details_services.skill_level_text(actual, capped, capped_text, error));
            CHECK(details_localization.symbol("GAMEPLAYMENUS_MAX_SKILL_LEVEL", &actual,
                                               expected_guard, error));
            CHECK(capped_text.next_level == expected_guard);

            fixture_max_level = 9;
            fixture_can_increment = false;
            character_menu::SkillPageCurrentNextTextV1 training_blocked;
            CHECK(details_services.skill_level_text(actual, capped, training_blocked, error));
            CHECK(details_localization.symbol("GAMEPLAYMENUS_SKILL_MAXIMUM_LEVEL_TRAINING", &actual,
                                               expected_guard, error));
            CHECK(training_blocked.next_level == expected_guard);

            const auto unaudited = std::find_if(list.begin(), list.end(), [&](int id) {
                return id >= 0 && static_cast<std::size_t>(id) < tables.skills().size() &&
                       tables.skills()[static_cast<std::size_t>(id)].script != fixture.script &&
                       tables.skills()[static_cast<std::size_t>(id)].script !=
                           "prince_warrior_bashdown" &&
                       tables.skills()[static_cast<std::size_t>(id)].script !=
                           "prince_rogue_jump_kick";
            });
            CHECK(unaudited != list.end());
            const auto unaudited_position = static_cast<int>(unaudited - list.begin());
            character_menu::SkillPageCurrentNextRequestV1 unknown_script{
                &actual, static_cast<int>(list_id), unaudited_position, *unaudited, 1,
                actual.stats.level};
            character_menu::SkillPageCurrentNextTextV1 unavailable;
            CHECK(details_services.skill_level_text(actual, unknown_script, unavailable, error));
            CHECK(!unavailable.available && unavailable.current_level.empty() &&
                  unavailable.next_level.empty() && !unavailable.unavailable_reason.empty());

            CharacterState detached = actual;
            character_menu::SkillPageCurrentNextRequestV1 stale{
                &detached, static_cast<int>(list_id), position, table_id, 1, actual.stats.level};
            character_menu::SkillPageCurrentNextTextV1 rejected;
            CHECK(!details_services.skill_level_text(detached, stale, rejected, error));
            CHECK(error.find("different CharacterState owner") != std::string::npos);
        }
        CHECK(detailed_rows == 4);

        std::size_t names = 0, descriptions = 0, faery_dependent = 0;
        for (std::size_t i = 0; i < tables.skills().size(); ++i) {
            const auto& row = tables.skills()[i];
            if (row.scalar.words[6] & 0xffu) ++faery_dependent;
            std::optional<std::string> name, description;
            CHECK(services.localized_text(state, static_cast<int>(i), name, description, error));

            const auto name_oid = signed_word(row.scalar.words[16]);
            if (name_oid >= 0) {
                std::string expected;
                CHECK(localization.string_id(name_oid, expected, error));
                CHECK(name && *name == expected && !name->empty());
                ++names;
            } else {
                CHECK(name && name->empty());
            }
            const auto description_oid = signed_word(row.scalar.words[13]);
            if (description_oid >= 0) {
                std::string expected;
                CHECK(localization.string_id(description_oid, expected, error));
                CHECK(description && *description == expected && !description->empty());
                ++descriptions;
            } else {
                CHECK(description && description->empty());
            }
        }
        CHECK(names > 0 && descriptions > 0);

        // The source-frozen SkillInfo fixture was generated by the recovered
        // original AI_SkillInfo/temporary-property/parseEx path. Tie every
        // applicable output to the actual SkillTable script and its authored
        // current/next OIDs before projecting it through the UI helper.
        const auto source_skill_texts = read_source_skill_info_text(argv[2]);
        CharacterState same_state;
        same_state.id = "actual-source-skill-info-projection";
        same_state.class_id = "KnightPlayerBase";
        same_state.stats.level = 24;
        std::size_t source_projected = 0;
        for (const auto& source : source_skill_texts) {
            const auto row = std::find_if(tables.skills().begin(), tables.skills().end(),
                [&](const auto& candidate) { return candidate.script == source.script; });
            if (row == tables.skills().end()) continue;
            CHECK(signed_word(row->scalar.words[12]) >= 0 && signed_word(row->scalar.words[17]) >= 0);
            const auto table_id = static_cast<int>(row - tables.skills().begin());
            character_menu::SkillPageCurrentNextRequestV1 request{
                &same_state, 17, 4, table_id, source.skill_rank, same_state.stats.level};
            character_menu::SkillPageCurrentNextTextV1 native_text{
                &same_state, request.skill_list_id, request.class_skill_position,
                request.skill_table_id, request.saved_rank, request.character_level,
                source.current, source.next, true, {}};
            std::vector<character_menu::SkillPageTextBindingV1> projected;
            const auto actual_symbol = [&](std::string_view symbol, std::string& value, std::string& e) {
                return localization.symbol(std::string(symbol), &same_state, value, e);
            };
            CHECK(character_menu::project_skill_page_current_next_v1(
                request, native_text, actual_symbol, projected, error));
            std::string expected_current, expected_next;
            CHECK(localization.symbol("GAMEPLAYMENUS_current_level", &same_state, expected_current, error));
            CHECK(localization.symbol("GAMEPLAYMENUS_next_level", &same_state, expected_next, error));
            CHECK(projected.size() == 2 &&
                  projected[0].text == expected_current + "\n\n" + source.current &&
                  projected[1].text == expected_next + "\n\n" + source.next);
            ++source_projected;
        }
        CHECK(source_projected > 0);

        // The native FaeryDependantText branch offsets only the current/next
        // level string OIDs by the same live faerie level before parsing them.
        // This generic callback exposes only the static base fields, so those
        // dynamic presentations are intentionally not guessed or populated.
        std::optional<std::string> name, description;
        CHECK(services.localized_text(state, 0, name, description, error));
        CharacterState detached = state;
        CHECK(!services.localized_text(detached, 0, name, description, error) &&
              error.find("different CharacterState owner") != std::string::npos);

        ServicesV1 custom;
        unsigned custom_calls = 0;
        custom.localized_text = [&](const CharacterState&, int,
                std::optional<std::string>& custom_name,
                std::optional<std::string>& custom_description, std::string&) {
            ++custom_calls;
            custom_name = "caller-owned";
            custom_description = "caller-description";
            return true;
        };
        RuntimeSkillsTextProviderV1 preserves_custom;
        CHECK(preserves_custom.bind(characters, tables, localization, state, custom, error));
        CHECK(custom.localized_text(state, 0, name, description, error));
        CHECK(custom_calls == 1 && name == "caller-owned" && description == "caller-description");

        std::cout << "runtime skills text PASS: " << names << " actual localized names, "
                  << descriptions << " actual base descriptions, " << faery_dependent
                  << " FaeryDependantText rows; " << source_projected
                  << " source SkillInfo current/next rows projected from actual SkillTable scripts/OIDs; "
                  << detailed_rows << " exact SetTempProps/current-next rows matched ordered display_props + parseEx; "
                  << rogue_source_cells << " actual Rogue specialization-cell states audited across SkillTrees 27/28/29\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
