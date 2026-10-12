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
        CHECK(argc == 4);
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
                // B097: SetTempProps(CLASS_ID, slvl) scales with the rank, so the
                // current-level and next-level bodies (MP cost / damage) must differ.
                std::cout << fixture.script << " rank " << rank << " current=[" << details.current_level
                          << "] next=[" << details.next_level << "]\n";
                CHECK(details.current_level != details.next_level);
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

            CharacterState detached = actual;
            character_menu::SkillPageCurrentNextRequestV1 stale{
                &detached, static_cast<int>(list_id), position, table_id, 1, actual.stats.level};
            character_menu::SkillPageCurrentNextTextV1 rejected;
            CHECK(!details_services.skill_level_text(detached, stale, rejected, error));
            CHECK(error.find("different CharacterState owner") != std::string::npos);
        }
        CHECK(detailed_rows == 4);

        // ---- B147: every player SkillList row, every NativeGetSkillDetails state ----
        // Independent oracle: the SetTempProps class token is parsed straight from
        // the original skill script text (argv[3]), not from the provider's table.
        struct ScriptInfo { bool simple{}; std::string token; };
        const auto parse_script = [&](const std::string& path) {
            std::ifstream in(path, std::ios::binary);
            if (!in) throw std::runtime_error("missing original skill script: " + path);
            std::string text((std::istreambuf_iterator<char>(in)), {});
            std::string clean;
            {   // strip `--` line comments and CR
                std::istringstream lines(text);
                std::string line;
                while (std::getline(lines, line)) {
                    const auto comment = line.find("--");
                    if (comment != std::string::npos) line.erase(comment);
                    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
                    clean += line + "\n";
                }
            }
            ScriptInfo info;
            const auto head = clean.find("local function OnSkillInfo_(slvl)");
            CHECK(head != std::string::npos);
            const auto begin = head + std::strlen("local function OnSkillInfo_(slvl)");
            const auto end = clean.find("\nend", begin);
            CHECK(end != std::string::npos);
            std::istringstream body(clean.substr(begin, end - begin));
            std::vector<std::string> statements;
            std::string statement;
            while (std::getline(body, statement)) {
                const auto first = statement.find_first_not_of(" \t");
                if (first == std::string::npos) continue;
                const auto last = statement.find_last_not_of(" \t");
                statements.push_back(statement.substr(first, last - first + 1));
            }
            CHECK(!statements.empty());
            const std::string prefix = "SetTempProps(", suffix = ", slvl);";
            const auto& first = statements[0];
            if (first.rfind(prefix, 0) != 0 || first.size() < prefix.size() + suffix.size() ||
                first.compare(first.size() - suffix.size(), suffix.size(), suffix) != 0) return info;
            for (std::size_t i = 1; i < statements.size(); ++i)
                if (statements[i].rfind("return ", 0) != 0) return info;
            const auto variable = first.substr(prefix.size(), first.size() - prefix.size() - suffix.size());
            std::size_t declaration = std::string::npos;
            for (std::size_t at = clean.find("local " + variable); at != std::string::npos;
                 at = clean.find("local " + variable, at + 1)) {
                const auto after = at + 6 + variable.size();
                if (after < clean.size() && (clean[after] == ' ' || clean[after] == '	' || clean[after] == '=')) {
                    declaration = at;
                    break;
                }
            }
            if (declaration == std::string::npos) return info;
            const auto quote_a = clean.find("GetPyOID(\"ClassTable\", \"", declaration);
            if (quote_a == std::string::npos || quote_a > clean.find('\n', declaration)) return info;
            const auto name_begin = quote_a + std::strlen("GetPyOID(\"ClassTable\", \"");
            info.token = clean.substr(name_begin, clean.find('"', name_begin) - name_begin);
            info.simple = !info.token.empty();
            return info;
        };
        std::size_t audited_scripts = 0, magi_checked = 0, unaudited_scripts = 0;
        for (std::size_t i = 0; i < tables.skills().size(); ++i) {
            const auto& script = tables.skills()[i].script;
            if (script.rfind("prince_", 0) != 0) continue;
            const auto info = parse_script(std::string(argv[3]) + "/" + script + ".luac");
            const char* provider_token = audited_skill_info_class_v1(script, 0);
            if (script == "prince_mage_magi_shielding") {
                CHECK(!info.simple && provider_token && std::string(provider_token) == "Skill_Mage_MagisShielding");
                CHECK(audited_skill_info_class_v1(script, 1000) != nullptr &&
                      audited_skill_info_class_v1(script, 1001) == nullptr);
                ++magi_checked;
            } else if (info.simple) {
                CHECK(provider_token && info.token == provider_token);
                ++audited_scripts;
            } else {
                CHECK(provider_token == nullptr);
                CHECK(script == "prince_warrior_charge" || script == "prince_paladin_energy_balance");
                ++unaudited_scripts;
            }
        }
        CHECK(magi_checked == 1 && unaudited_scripts == 2 && audited_scripts >= 60);

        struct PlayerList { const char* character; };
        const PlayerList player_lists[] = {
            {"KnightPlayerBase"}, {"KnightPlayerBase_Berserker"}, {"KnightPlayerBase_Paladin"},
            {"MagePlayerBase"}, {"MagePlayerBase_Illusionist"}, {"MagePlayerBase_Necromancer"},
            {"RoguePlayerBase"}, {"RoguePlayerBase_Archer"}, {"RoguePlayerBase_Assassin"}};
        std::size_t matrix_rows = 0, matrix_locked = 0, matrix_rank0 = 0, matrix_ranked = 0,
                    matrix_capped = 0, matrix_blocked = 0, matrix_unaudited = 0;
        for (const auto& player : player_lists) {
            const auto character = std::find(characters.names.begin(), characters.names.end(), player.character);
            CHECK(character != characters.names.end());
            const auto character_row = static_cast<std::size_t>(character - characters.names.begin());
            const auto list_id = characters.rows[character_row][skill_tree_column];
            const auto& list = tables.lists()[static_cast<std::size_t>(list_id)];
            dh2::data::PropertyState base_properties;
            dh2::data::reset_properties(rules, base_properties, &characters.rows[character_row]);
            CHECK(dh2::data::recalc_properties_with_class(classes, rules, base_properties, error));

            CharacterState actual;
            actual.id = std::string("matrix-") + player.character;
            actual.class_id = player.character;
            character_menu::MenuLocalization matrix_localization;
            CHECK(matrix_localization.load(assets, "original-cache/data", 0, error));
            CHECK(matrix_localization.bind_profile(&actual, error));
            ServicesV1 matrix_services;
            RuntimeSkillsTextProviderV1 matrix_provider;
            CHECK(matrix_provider.bind(characters, tables, matrix_localization, actual, matrix_services, error));
            dh2::data::PropertySheet resolved = base_properties.resolved;
            const auto set_level = [&](std::uint32_t level) {
                actual.stats.level = level;
                resolved[19] = static_cast<std::int32_t>(level * 256u);
            };
            RuntimeSkillsDetailsBindingsV1 matrix_bindings;
            matrix_bindings.classes = &classes;
            matrix_bindings.property_rules = &rules;
            matrix_bindings.current_resolved = [&](const CharacterState& owner,
                    dh2::data::PropertySheet& result, std::string&) {
                if (&owner != &actual) return false;
                result = resolved;
                return true;
            };
            std::int32_t cap = 9;
            bool can_increment = true;
            matrix_bindings.max_skill_level = [&](const CharacterState&, std::int32_t& value, std::string&) {
                value = cap; return true;
            };
            matrix_bindings.can_increment = [&](const CharacterState&, int, bool& value, std::string&) {
                value = can_increment; return true;
            };
            CHECK(matrix_provider.bind_details(matrix_bindings, matrix_services, error));
            dh2::ui::HudTextV1* hud = nullptr;
            dh2::ui::HudTextEnvironmentV1 environment;
            CHECK(matrix_localization.borrow_text(hud, environment, error));
            const auto parse_symbol_text = [&](const char* symbol, int argument, bool with_argument) {
                std::string templated, output;
                std::int32_t id{};
                CHECK(matrix_localization.constant("StrID", symbol, id, error));
                CHECK(matrix_localization.string_id(id, templated, error));
                const dh2::ui::HudTextVariantV1 value{static_cast<float>(argument), argument, nullptr};
                bool changed = false;
                CHECK(hud->parse_ex(templated.c_str(), with_argument ? &value : nullptr,
                                    with_argument ? 1 : 0, environment, output, changed, error));
                return output;
            };
            const std::string needs_points = parse_symbol_text("GAMEPLAYMENUS_NEEDS_SKILL_POINTS", 0, false);
            const std::string max_level = parse_symbol_text("GAMEPLAYMENUS_MAX_SKILL_LEVEL", 0, false);
            const std::string training_blocked_text =
                parse_symbol_text("GAMEPLAYMENUS_SKILL_MAXIMUM_LEVEL_TRAINING", 0, false);
            CHECK(!needs_points.empty() && !max_level.empty() && !training_blocked_text.empty());

            for (std::size_t position = 0; position < list.size(); ++position) {
                const auto table_id = list[position];
                const auto& record = tables.skills()[static_cast<std::size_t>(table_id)];
                if (record.script.empty()) continue; // blank SpecializationSkill placeholder
                const auto required = signed_word(record.scalar.words[8]);
                ScriptInfo oracle;
                if (record.script.rfind("prince_", 0) == 0)
                    oracle = parse_script(std::string(argv[3]) + "/" + record.script + ".luac");
                if (record.script == "prince_mage_magi_shielding") {
                    oracle.simple = true;
                    oracle.token = "Skill_Mage_MagisShielding";
                }
                const auto expected_dynamic = [&](std::uint32_t rank, int oid_word) {
                    SkillTempPropertiesV1 sheet;
                    CHECK(evaluate_skill_temp_properties_v1(classes, rules, resolved, oracle.token, rank, sheet, error));
                    std::vector<dh2::ui::HudTextVariantV1> args;
                    for (const auto property : record.display_props) {
                        const auto fixed = sheet.properties[static_cast<std::size_t>(property)];
                        const auto integer = fixed >= 0 ? fixed / 256 :
                            -1 - static_cast<std::int32_t>((~static_cast<std::uint32_t>(fixed)) >> 8);
                        args.push_back({static_cast<float>(fixed) * (1.0f / 256.0f), integer, nullptr});
                    }
                    std::string templated, output;
                    const auto oid = signed_word(record.scalar.words[oid_word]);
                    CHECK(oid >= 0);
                    CHECK(matrix_localization.string_id(oid, templated, error));
                    bool changed = false;
                    CHECK(hud->parse_ex(templated.c_str(), args.empty() ? nullptr : args.data(),
                                        args.size(), environment, output, changed, error));
                    return output;
                };
                const auto query = [&](std::uint32_t rank, character_menu::SkillPageCurrentNextTextV1& result) {
                    character_menu::SkillPageCurrentNextRequestV1 request{
                        &actual, static_cast<int>(list_id), static_cast<int>(position), table_id, rank,
                        actual.stats.level};
                    if (!matrix_services.skill_level_text(actual, request, result, error))
                        throw std::runtime_error(std::string("matrix details query failed for ") + record.script + " rank " + std::to_string(rank) + " level " + std::to_string(actual.stats.level) + ": " + error);
                    CHECK(result.state == &actual && result.saved_rank == rank &&
                          result.skill_table_id == table_id && result.class_skill_position == static_cast<int>(position));
                };
                ++matrix_rows;

                // Locked by level: static string, no script involved (even for scripts
                // the projection has not audited).
                if (required > 0) {
                    set_level(static_cast<std::uint32_t>(required - 1));
                    cap = 9; can_increment = true;
                    for (std::uint32_t rank : {0u, 1u}) {
                        character_menu::SkillPageCurrentNextTextV1 locked;
                        query(rank, locked);
                        CHECK(locked.available && locked.next_level.empty() &&
                              locked.current_level == parse_symbol_text("GAMEPLAYMENUS_SKILL_UNLOCK_AT_LEVEL", required, true));
                        CHECK(locked.current_level.find(std::to_string(required)) != std::string::npos);
                    }
                    ++matrix_locked;
                }

                set_level(static_cast<std::uint32_t>(std::max(required, 0) + 5));
                cap = 9; can_increment = true;
                character_menu::SkillPageCurrentNextTextV1 rank0;
                query(0, rank0);
                CHECK(rank0.available && rank0.current_level == needs_points);
                if (oracle.simple) {
                    const auto expected = expected_dynamic(1, 17);
                    CHECK(!expected.empty() && rank0.next_level == expected);
                } else {
                    CHECK(rank0.next_level.empty() && !rank0.unavailable_reason.empty());
                }
                ++matrix_rank0;

                for (std::uint32_t rank : {1u, 3u}) {
                    character_menu::SkillPageCurrentNextTextV1 ranked;
                    query(rank, ranked);
                    if (oracle.simple) {
                        CHECK(ranked.available);
                        CHECK(ranked.current_level == expected_dynamic(rank, 12));
                        CHECK(ranked.next_level == expected_dynamic(rank + 1, 17));
                        CHECK(!ranked.current_level.empty() && !ranked.next_level.empty());
                        ++matrix_ranked;
                    } else {
                        CHECK(!ranked.available && ranked.current_level.empty() && ranked.next_level.empty());
                        ++matrix_unaudited;
                    }
                }

                cap = 1;
                character_menu::SkillPageCurrentNextTextV1 at_cap;
                query(1, at_cap);
                CHECK(at_cap.next_level == max_level);
                CHECK(oracle.simple ? at_cap.current_level == expected_dynamic(1, 12) : at_cap.current_level.empty());
                ++matrix_capped;

                cap = 9; can_increment = false;
                character_menu::SkillPageCurrentNextTextV1 blocked;
                query(2, blocked);
                CHECK(blocked.next_level == training_blocked_text);
                CHECK(oracle.simple ? blocked.current_level == expected_dynamic(2, 12) : blocked.current_level.empty());
                ++matrix_blocked;
                can_increment = true;
                cap = 9;

                if (player.character == std::string("KnightPlayerBase") && position < 8) {
                    set_level(static_cast<std::uint32_t>(required));
                    character_menu::SkillPageCurrentNextTextV1 s0, s1, s2;
                    query(0, s0);
                    query(1, s1);
                    query(2, s2);
                    std::cout << "KNIGHT " << tables.skill_names()[static_cast<std::size_t>(table_id)]
                              << " [" << record.script << "] req=" << required << "\n"
                              << "  rank0 current=[" << s0.current_level << "] next=[" << s0.next_level << "]\n"
                              << "  rank1 current=[" << s1.current_level << "] next=[" << s1.next_level << "]\n"
                              << "  rank2 current=[" << s2.current_level << "] next=[" << s2.next_level << "]\n";
                    if (required > 0) {
                        set_level(static_cast<std::uint32_t>(required - 1));
                        character_menu::SkillPageCurrentNextTextV1 lk;
                        query(0, lk);
                        std::cout << "  locked(level " << required - 1 << ") current=[" << lk.current_level
                                  << "] next=[" << lk.next_level << "]\n";
                    }
                }
            }
        }
        CHECK(matrix_rows > 100 && matrix_locked > 0 && matrix_rank0 == matrix_rows);
        std::cout << "B147 matrix: " << matrix_rows << " player SkillList rows (" << audited_scripts
                  << " simple + " << magi_checked << " guarded scripts audited, " << unaudited_scripts
                  << " unaudited), locked=" << matrix_locked << " rank0=" << matrix_rank0
                  << " ranked=" << matrix_ranked << " capped=" << matrix_capped
                  << " blocked=" << matrix_blocked << " unaudited-ranked=" << matrix_unaudited << "\n";

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
