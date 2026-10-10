// P14 SCHEMA: LevelList row 41 identity, stamping rules and the slot panel
// projection (MenuLocalization + engine-ui menu_save_slot_projection_v1).
// argv[1] = shared assets root (original-cache/data text + pydata),
// argv[2] = asset root whose data/ folder holds levels_pyarray.bin/levels_pyarraynames.bin/levels_pystructnames.bin.
#include "menu_metadata_v1.hpp"
#include "../../asset_catalog.hpp"
#include "../../../engine-ui/save_slot_date_v1.hpp"

#include <algorithm>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

namespace {
using namespace dh::foundation;
namespace mm = dh::foundation::menu_metadata;
void require(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}
std::vector<std::uint8_t> file(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    require(bool(in), "cannot read " + path);
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}
// The shipped locations corpus stores "The Boglands " with one trailing space;
// the panel shows the source text unchanged, so compare modulo trailing blanks.
std::string trim_end(std::string value) {
    while (!value.empty() && value.back() == ' ') value.pop_back();
    return value;
}
dh2::data::Bytes span(const std::vector<std::uint8_t>& v) { return {v.data(), v.size()}; }
std::string expected_date(std::uint32_t stamp) {
    const std::time_t t = static_cast<std::time_t>(static_cast<std::int32_t>(stamp));
    std::tm calendar{};
#ifdef _WIN32
    localtime_s(&calendar, &t);
#else
    localtime_r(&t, &calendar);
#endif
    char buffer[80]{};
    std::strftime(buffer, sizeof buffer, "%m/%d     %H:%M", &calendar);
    return buffer;
}
} // namespace

int main(int argc, char** argv) {
    try {
        require(argc == 3, "usage: menu_metadata_v1_tests <shared-assets> <levels-data-dir>");
        std::string error;
        // Level tables from the genuine cache: row 41 is the first map.
        dh2::data::LevelTables levels;
        require(mm::load_level_tables(AssetCatalog(argv[2]), levels, error) && levels.levels.size() == 51, "levels load: " + error);
        dh2::data::LevelTables missing;
        require(!mm::load_level_tables(AssetCatalog(argv[1]), missing, error) && !error.empty() && missing.levels.empty(),
                "missing LevelList assets must fail without a partial table");
        std::cout << "levels[41].file=" << levels.levels[41].file << " words[9]=" << levels.levels[41].scalar.words[9] << '\n';
        require(levels.levels[41].file.find("001_swamp.mlx") != std::string::npos, "LevelList row 41 is not 001_swamp.mlx");
        require(mm::find_level_row(levels, "data/scene/001_swamp.mlx") == 41, "find_level_row(001_swamp) != 41");
        require(mm::find_level_row(levels, "DATA\\SCENE\\001_SWAMP.MLX") == 41, "find_level_row is not path/case tolerant");
        require(mm::find_level_row(levels, "data/scene/does_not_exist.mlx") == -1 && mm::find_level_row(levels, "") == -1, "unknown level must give -1");
        for (std::size_t i = 0; i < levels.levels.size(); ++i) {
            const auto row = mm::find_level_row(levels, levels.levels[i].file);
            require(row >= 0 && levels.levels[row].file == levels.levels[i].file, "row lookup is not a left inverse");
        }

        // Localization and CharacterTable.
        AssetCatalog assets(argv[1]);
        character_menu::MenuLocalization text;
        require(text.load(assets, "original-cache/data", 0, error), "localization: " + error);
        std::string location;
        require(text.string_id(std::int32_t(levels.levels[41].scalar.words[9]), location, error), "row 41 location string: " + error);
        std::cout << "row 41 location=" << location << '\n';
        require(trim_end(location) == "The Boglands" && location.size() <= 13, "row 41 words[9] is not LOCATIONS_LOC_BOGLANDS (The Boglands): " + location);
        std::int32_t boglands = -1;
        require(text.constant("StrID", "LOCATIONS_LOC_BOGLANDS", boglands, error) && boglands == std::int32_t(levels.levels[41].scalar.words[9]),
                "row 41 StrID is not LOCATIONS_LOC_BOGLANDS");
        dh2::data::CharacterTable characters;
        const auto root = std::string(argv[1]) + "/original-cache/data/pydata/";
        const auto crecords = file(root + "character_properties_pyarray.bin"), cnames = file(root + "character_properties_pyarraynames.bin"),
                   cschema = file(root + "character_properties_pystructnames.bin");
        require(dh2::data::load_characters(span(crecords), span(cnames), span(cschema), characters, error), "characters: " + error);
        const auto knight_row = std::int32_t(std::find(characters.names.begin(), characters.names.end(), "KnightPlayerBase") - characters.names.begin());
        require(std::size_t(knight_row) < characters.names.size(), "KnightPlayerBase row");

        // Fresh profile (FS_StartGame): row 41, act 1, Normal, date now.
        auto fresh = make_default_character("profile-slot-0", "CRISR82", "KnightPlayerBase");
        require(!fresh.menu_metadata.known && fresh.current_difficulty == 0, "default character must be unknown/Normal");
        const std::uint32_t stamp = 1760113980u;
        mm::initialize_fresh_menu_metadata(fresh, stamp);
        require(validate_character_state(fresh).ok() && fresh.menu_metadata.known && fresh.menu_metadata.save_time == stamp &&
                fresh.menu_metadata.level_row == std::array<std::int32_t, 3>{41, 41, 41} &&
                fresh.menu_metadata.current_act == std::array<std::int32_t, 3>{1, 1, 1}, "fresh metadata differs");
        frontend::creation::SavedProfilePresentation panel{};
        require(mm::project_slot_presentation(fresh, 0, knight_row, characters, levels, text, 0, panel, error), "fresh projection: " + error);
        std::cout << "fresh panel: level='" << panel.level_text << "' act='" << panel.act_text << "' loc='" << panel.localized_location
                  << "' date='" << panel.formatted_save_date << "' lastsave='" << panel.last_save_label << "' diff='" << panel.difficulty_title
                  << "' '" << panel.difficulty_text << "'\n";
        require(panel.level_text == "LEVEL 1" && panel.act_text == "Act 1" && trim_end(panel.localized_location) == "The Boglands" &&
                panel.last_save_label == "Last Save" && panel.difficulty_title == "Difficulty:" && panel.difficulty_text == "Normal" &&
                panel.current_act_known && panel.difficulty_known && panel.difficulty == 0 && panel.current_act == 1 &&
                panel.formatted_save_date == expected_date(stamp), "fresh panel strings differ from the original panel");

        // Legacy (known=false): only the level text, everything else blank.
        auto legacy = make_default_character("legacy", "OLD", "KnightPlayerBase");
        frontend::creation::SavedProfilePresentation blank{};
        require(mm::project_slot_presentation(legacy, 0, knight_row, characters, levels, text, 0, blank, error), "legacy projection: " + error);
        require(blank.level_text == "LEVEL 1" && !blank.current_act_known && !blank.difficulty_known && blank.localized_location.empty() &&
                blank.formatted_save_date.empty() && blank.act_text.empty() && blank.difficulty_text.empty(), "legacy slot must stay blank");

        // Stamping at a save point: date, level row of the CURRENT difficulty, act only when known.
        auto stamped = legacy;
        mm::stamp_menu_metadata_for_level(stamped, 1760200000u, &levels, "data/scene/001_swamp.mlx");
        require(stamped.menu_metadata.known && stamped.menu_metadata.save_time == 1760200000u && stamped.menu_metadata.level_row[0] == 41 &&
                stamped.menu_metadata.level_row[1] == -1 && stamped.menu_metadata.current_act[0] == 1 && validate_character_state(stamped).ok(),
                "legacy slot stamp differs");
        const auto before_unknown_level = stamped.menu_metadata.level_row;
        mm::stamp_menu_metadata_for_level(stamped, 1760300000u, &levels, "data/scene/not-in-levellist.mlx");
        require(stamped.menu_metadata.level_row == before_unknown_level && stamped.menu_metadata.save_time == 1760300000u,
                "a level missing from LevelList must keep the stored row and still stamp the date");
        mm::stamp_menu_metadata_for_level(stamped, 1760400000u, nullptr, "data/scene/001_swamp.mlx");
        require(stamped.menu_metadata.save_time == 1760400000u && stamped.menu_metadata.level_row == before_unknown_level, "null tables must not invent a row");
        // Hard: the row and act land in the current difficulty slot only.
        auto hard = fresh; hard.current_difficulty = 1; hard.unlocked_difficulty = 1;
        mm::stamp_menu_metadata(hard, 1760500000u, 43, 2);
        require(hard.menu_metadata.level_row == std::array<std::int32_t, 3>{41, 43, 41} &&
                hard.menu_metadata.current_act == std::array<std::int32_t, 3>{1, 2, 1}, "hard stamp touched the wrong difficulty");
        frontend::creation::SavedProfilePresentation hard_panel{};
        require(mm::project_slot_presentation(hard, 0, knight_row, characters, levels, text, 0, hard_panel, error), "hard projection: " + error);
        std::string row43;
        require(text.string_id(std::int32_t(levels.levels[43].scalar.words[9]), row43, error), "row 43 string");
        std::cout << "hard panel: act='" << hard_panel.act_text << "' loc='" << hard_panel.localized_location << "' diff='" << hard_panel.difficulty_text << "'\n";
        require(hard_panel.difficulty_text == "Hard" && hard_panel.act_text == "Act 2" && hard_panel.localized_location == row43 && hard_panel.difficulty == 1,
                "hard panel differs");
        hard.current_difficulty = 2; hard.unlocked_difficulty = 2;
        frontend::creation::SavedProfilePresentation heroic{};
        require(mm::project_slot_presentation(hard, 0, knight_row, characters, levels, text, 0, heroic, error) && heroic.difficulty_text == "Heroic",
                "heroic panel differs");
        // Unset level row of the current difficulty: no location (the source would show row 0).
        auto unset = fresh; unset.menu_metadata.level_row[0] = -1;
        frontend::creation::SavedProfilePresentation unset_panel{};
        require(mm::project_slot_presentation(unset, 0, knight_row, characters, levels, text, 0, unset_panel, error) && unset_panel.localized_location.empty(),
                "unset row must not show a location");
        // Invalid inputs fail loudly instead of publishing partial text.
        auto bad_row = fresh; bad_row.menu_metadata.level_row[0] = 4000;
        frontend::creation::SavedProfilePresentation failed{};
        require(!mm::project_slot_presentation(bad_row, 0, knight_row, characters, levels, text, 0, failed, error) && !error.empty(), "row outside LevelList accepted");
        require(!mm::project_slot_presentation(fresh, 0, 99999, characters, levels, text, 0, failed, error), "invalid character row accepted");

        // Visited helper keeps the section sorted/unique and valid.
        auto visited = fresh;
        mm::set_visited_module(visited, "data/scene/b.mlx", 5, true);
        mm::set_visited_module(visited, "data/scene/a.mlx", 9, true);
        mm::set_visited_module(visited, "data/scene/a.mlx", 2, false);
        mm::set_visited_module(visited, "data/scene/a.mlx", 9, false);
        require(visited.visited_modules.size() == 3 && visited.visited_modules[0].module_id == 2 && visited.visited_modules[1].module_id == 9 &&
                visited.visited_modules[1].visited == 0 && visited.visited_modules[2].level_uri == "data/scene/b.mlx" && validate_character_state(visited).ok(),
                "visited helper order/update differs");

        std::cout << "menu_metadata_v1_tests: all checks passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "menu_metadata_v1_tests: " << e.what() << '\n';
        return 1;
    }
}
