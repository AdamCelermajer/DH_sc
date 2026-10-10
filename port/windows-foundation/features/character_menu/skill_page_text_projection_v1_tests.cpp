#include "skill_page_text_projection_v1.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

using namespace dh::foundation::character_menu;

namespace {
void check(bool ok, const char* message) {
    if (!ok) throw std::runtime_error(message);
}
bool near(float a, float b) { return std::abs(a - b) < 0.0001f; }

MenuTextAdvanceV1 byte_advance() {
    return [](std::string_view text, float& advance, std::string&) {
        advance = static_cast<float>(text.size());
        return true;
    };
}
}

int main() {
    try {
        const std::unordered_map<std::string, std::string> source_symbols{
            {"GAMEPLAYMENUS_SKILLS_TITLE", "Skills"},
            {"MENU_CLASS_SKILL", "Class skill"},
            {"MENU_SPECIALISATION", "Specialization"},
            {"MENU_SKILL_BUTTONS", "Skill Mapping"},
            {"GAMEPLAYMENUS_SKILL_POINTS", "Skill points left"},
            {"GAMEPLAYMENUS_ADD_SKILL", "Upgrade Skill"},
        };
        std::vector<std::string> requested;
        SkillPageTextInputV1 input;
        input.skill_points_left = 7; // test fixture for the native points getter result
        for (int i = 0; i < 16; ++i) {
            auto& cell = input.cells[static_cast<std::size_t>(i)];
            cell.skill_level = i % 4;
            cell.skill_unlocked = i % 2 == 0;
            cell.skill_assignable = i % 3 != 0;
            cell.skill_icon_frame = i == 2 ? "blank" : "source-icon-frame";
        }
        input.cells[0] = {2, true, true, "source-icon-frame"};
        input.cells[1] = {0, false, true, "source-icon-frame"};
        input.cells[2] = {0, false, true, "blank"};
        input.cells[3] = {1, false, true, "source-icon-frame"};
        input.symbol_text = [&](std::string_view symbol, std::string& text, std::string& error) {
            const auto it = source_symbols.find(std::string(symbol));
            if (it == source_symbols.end()) { error = "fixture symbol absent"; return false; }
            requested.emplace_back(symbol);
            text = it->second;
            return true;
        };

        std::vector<SkillPageTextBindingV1> bindings;
        std::string error;
        check(project_skill_page_text_v1(input, bindings, error), error.c_str());
        check(bindings.size() == 39,
              "Original onShow/presetAllSkills receiver set changed");
        check(requested == std::vector<std::string>{
                  "GAMEPLAYMENUS_SKILLS_TITLE", "MENU_CLASS_SKILL", "MENU_SPECIALISATION",
                  "MENU_SKILL_BUTTONS", "GAMEPLAYMENUS_SKILL_POINTS", "GAMEPLAYMENUS_ADD_SKILL"},
              "Localized source symbols were not requested in source field order");
        const auto get = [&](std::string_view path) -> const std::string& {
            for (const auto& binding : bindings) if (binding.field_path == path) return binding.text;
            throw std::runtime_error("source text field missing from projection");
        };
        check(get("menu_SkillTreeSheetNew/MENU_SKILL_BUTTONS/text") == "Skill Mapping" &&
                  get("menu_SkillTreeSheetNew/btn_add/AddText/text") == "Upgrade Skill" &&
                  get("menu_SkillTreeSheetNew/GAMEPLAYMENUS_SKILL_POINTS/text") == "Skill points left",
              "Original localized button/points labels were not preserved");
        check(get("menu_SkillTreeSheetNew/cp_Skill_Points/anim/value") == "7" &&
                  get("menu_SkillTreeSheetNew/buttons/btn_skill9/cnt/value") == "1" &&
                  get("menu_SkillTreeSheetNew/buttons/skill9/cnt/value") == "1",
              "Native point/rank input was not projected to both source receiver copies");
        check(get("menu_SkillTreeSheetNew/buttons/btn_skill15/cnt/value") == "3",
              "Highest source skill rank receiver was omitted");

        dh::foundation::CharacterState same_state;
        SkillPageCurrentNextRequestV1 details_request{
            &same_state, 7, 2, 19, 3, 28};
        SkillPageCurrentNextTextV1 details{
            &same_state, 7, 2, 19, 3, 28,
            "Damage bonus: + 7", "Damage bonus: + 9"};
        const auto headings = [](std::string_view symbol, std::string& text, std::string& e) {
            if (symbol == "GAMEPLAYMENUS_current_level") text = "Current level";
            else if (symbol == "GAMEPLAYMENUS_next_level") text = "Next level";
            else { e = "unknown source symbol"; return false; }
            return true;
        };
        std::vector<SkillPageTextBindingV1> level_bindings;
        check(project_skill_page_current_next_v1(details_request, details, headings,
                                                 level_bindings, error), error.c_str());
        check(level_bindings.size() == 2 &&
                  level_bindings[0].field_path == "menu_SkillTreeSheetNew/current_skill_description/text" &&
                  level_bindings[0].text == "Current level\n\nDamage bonus: + 7" &&
                  level_bindings[1].field_path == "menu_SkillTreeSheetNew/next_skill_description/text" &&
                  level_bindings[1].text == "Next level\n\nDamage bonus: + 9",
              "Native current/next source strings or authored heading order changed");
        auto stale_details = details;
        stale_details.saved_rank++;
        const auto prior_level_bindings = level_bindings;
        check(!project_skill_page_current_next_v1(details_request, stale_details, headings,
                                                   level_bindings, error) &&
                  level_bindings.size() == prior_level_bindings.size() &&
                  level_bindings[0].text == prior_level_bindings[0].text,
              "Stale skill rank source result was accepted or mutated output");
        details.current_level = "<font color=\"#FF0000\">source value</font>";
        check(project_skill_page_current_next_v1(details_request, details, headings,
                                                 level_bindings, error) &&
                  level_bindings[0].text.find("<font color=\"#FF0000\">source value</font>") != std::string::npos,
              "Source HTML color markup was normalized or discarded");
        std::array<SkillPageCellPresentationV1, 16> cells{};
        check(project_skill_page_cells_v1(input, cells, error), error.c_str());
        check(cells[0].skill_level == 2 && !cells[0].button_disabled &&
                  !cells[0].lock_visible && !cells[0].grey_visible,
              "Trained/unlocked source cell state was not preserved");
        check(cells[1].button_disabled && cells[1].lock_visible && cells[1].grey_visible,
              "Rank-zero locked source icon did not show disabled+lock+grey state");
        check(cells[2].button_disabled && !cells[2].lock_visible && !cells[2].grey_visible,
              "Source blank icon did not suppress grey and lock overlays at rank zero");
        check(!cells[3].button_disabled && cells[3].lock_visible && !cells[3].grey_visible,
              "Source unlocked state was guessed from rank rather than native SkillUnlocked");

        SkillPageTextInputV1 missing_symbol = input;
        missing_symbol.symbol_text = [](std::string_view, std::string&, std::string& e) {
            e = "source corpus key unavailable";
            return false;
        };
        const auto before = bindings;
        check(!project_skill_page_text_v1(missing_symbol, bindings, error) &&
                  bindings.size() == before.size() && bindings[0].text == before[0].text,
              "Missing original localization mutated output or succeeded with a fallback");

        SkillPageSourceFieldStyleV1 style;
        check(skill_page_source_field_style_v1(
                  "menu_SkillTreeSheetNew/current_skill_description/text", style, error), error.c_str());
        check(style.field_character == 468 && style.font_character == 103 &&
                  style.rgba == std::array<float, 4>{1, 1, 204.f / 255.f, 1},
              "Current/next HTML text source font/default color differs from SWF field468");
        check(skill_page_source_field_style_v1(
                  "menu_SkillTreeSheetNew/TitleText/txt_title", style, error) &&
                  style.field_character == 322 && style.font_character == 287 &&
                  near(style.rgba[0], 1) && near(style.rgba[1], 238.f / 255.f) &&
                  near(style.rgba[2], 170.f / 255.f),
              "Skills title source font/color differs from SWF field322");
        check(!skill_page_source_field_style_v1("menu_SkillTreeSheetNew/Train/text", style, error),
              "An unsupported Train button label was fabricated as a source field");

        MenuTextLayoutFieldV1 geometry;
        geometry.local_rect = {0, 96, 0, 40};
        geometry.matrix = {1, 0, 0, 1, 20, 30};
        geometry.margins = {0, 0, 0};
        geometry.source_height = 10;
        MenuTextLayoutV1 layout;
        check(layout_skill_page_text_v1(
                  "menu_SkillTreeSheetNew/current_skill_description/text", geometry,
                  "<font color=\"#FF0000\">Slam current level</font>", byte_advance(), layout, error), error.c_str());
        check(layout.flags.raw == 0xed22 && layout.lines.size() == 1 &&
                  layout.lines[0].runs.size() == 1 && layout.lines[0].runs[0].text == "Slam current level" &&
                  layout.lines[0].runs[0].font_character == 103 &&
                  layout.lines[0].runs[0].rgba == std::array<float, 4>{1, 0, 0, 1},
              "Original current-level FONT HTML did not render as the authored red run");
        check(layout_skill_page_text_v1(
                  "menu_SkillTreeSheetNew/skill_description/text", geometry,
                  "[<font color=\"#9CFF9A\">Active</font>] A targeted strike",
                  byte_advance(), layout, error), error.c_str());
        check(layout.flags.html && layout.lines.size() == 1 && layout.lines[0].runs.size() == 3 &&
                  layout.lines[0].runs[1].text == "Active" &&
                  layout.lines[0].runs[1].rgba == std::array<float, 4>{156.f / 255.f, 1, 154.f / 255.f, 1},
              "Source SkillDescription Active markup did not retain the original green font color");

        std::cout << "PASS source Skills text projection: exact symbols, 32 ranks, native points, per-cell lock/grey/disabled states and HTML styles\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
