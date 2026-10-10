#include "skill_page_text_projection_v1.hpp"

#include <algorithm>
#include <string>

namespace dh::foundation::character_menu {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

constexpr std::string_view root = "menu_SkillTreeSheetNew/";
constexpr std::array<std::pair<std::string_view, std::string_view>, 6> localized_fields{{
    {"TitleText/txt_title", "GAMEPLAYMENUS_SKILLS_TITLE"},
    {"MENU_CLASS_SKILL/text", "MENU_CLASS_SKILL"},
    {"MENU_SPECIALISATION/text", "MENU_SPECIALISATION"},
    {"MENU_SKILL_BUTTONS/text", "MENU_SKILL_BUTTONS"},
    {"GAMEPLAYMENUS_SKILL_POINTS/text", "GAMEPLAYMENUS_SKILL_POINTS"},
    {"btn_add/AddText/text", "GAMEPLAYMENUS_ADD_SKILL"},
}};

bool add_symbol(const SkillPageSymbolTextV1& resolver, std::string_view suffix,
                std::string_view symbol, std::vector<SkillPageTextBindingV1>& output,
                std::string& error) {
    if (!resolver) return fail(error, "Original Skills page symbol resolver unavailable");
    std::string text;
    if (!resolver(symbol, text, error)) return false;
    if (text.empty()) return fail(error, "Original Skills page symbol resolved to empty text");
    output.push_back({std::string(root) + std::string(suffix), std::move(text)});
    return true;
}

bool parse_rank_path(std::string_view path, std::string_view& prefix, int& rank) {
    constexpr std::string_view left = "menu_SkillTreeSheetNew/buttons/";
    constexpr std::string_view suffix = "/cnt/value";
    if (path.substr(0, left.size()) != left || path.size() <= left.size() + suffix.size() ||
        path.substr(path.size() - suffix.size()) != suffix) return false;
    const auto middle = path.substr(left.size(), path.size() - left.size() - suffix.size());
    const auto digit = middle.rfind("skill");
    if (digit == std::string_view::npos || (digit != 0 && digit != 4)) return false;
    prefix = middle.substr(0, digit);
    if (prefix != "" && prefix != "btn_") return false;
    const auto digits = middle.substr(digit + 5);
    if (digits.empty() || digits.size() > 2) return false;
    rank = 0;
    for (char c : digits) {
        if (c < '0' || c > '9') return false;
        rank = rank * 10 + c - '0';
    }
    return rank >= 0 && rank < 16;
}

bool parse_source_field(std::string_view path, std::uint16_t& character,
                        std::uint16_t& font, std::array<float, 4>& rgba) {
    if (path == "menu_SkillTreeSheetNew/SKILL_NAME/text") {
        character = 464; font = 287; rgba = {1, 1, 1, 1}; return true;
    }
    if (path == "menu_SkillTreeSheetNew/skill_description/text") {
        character = 466; font = 103; rgba = {1, 1, 204.f / 255.f, 1}; return true;
    }
    if (path == "menu_SkillTreeSheetNew/current_skill_description/text" ||
        path == "menu_SkillTreeSheetNew/next_skill_description/text") {
        character = 468; font = 103; rgba = {1, 1, 204.f / 255.f, 1}; return true;
    }
    if (path == "menu_SkillTreeSheetNew/TitleText/txt_title") {
        character = 322; font = 287; rgba = {1, 238.f / 255.f, 170.f / 255.f, 1}; return true;
    }
    if (path == "menu_SkillTreeSheetNew/GAMEPLAYMENUS_SKILL_POINTS/text") {
        character = 470; font = 103; rgba = {1, 1, 1, 1}; return true;
    }
    if (path == "menu_SkillTreeSheetNew/cp_Skill_Points/anim/value") {
        character = 295; font = 103; rgba = {1, 1, 1, 1}; return true;
    }
    if (path == "menu_SkillTreeSheetNew/MENU_CLASS_SKILL/text" ||
        path == "menu_SkillTreeSheetNew/MENU_SPECIALISATION/text" ||
        path == "menu_SkillTreeSheetNew/MENU_SKILL_BUTTONS/text" ||
        path == "menu_SkillTreeSheetNew/btn_add/AddText/text") {
        character = 473; font = 103; rgba = {1, 1, 1, 1}; return true;
    }
    std::string_view group;
    int rank{};
    if (parse_rank_path(path, group, rank)) {
        character = 482; font = 103; rgba = {1, 1, 1, 1}; return true;
    }
    return false;
}
}

bool project_skill_page_text_v1(const SkillPageTextInputV1& input,
                                std::vector<SkillPageTextBindingV1>& output,
                                std::string& error) {
    if (!input.symbol_text) return fail(error, "Original Skills page symbol resolver unavailable");
    std::vector<SkillPageTextBindingV1> next;
    for (const auto& [path, symbol] : localized_fields)
        if (!add_symbol(input.symbol_text, path, symbol, next, error)) return false;

    next.push_back({std::string(root) + "cp_Skill_Points/anim/value",
                    std::to_string(input.skill_points_left)});
    for (int position = 0; position < 16; ++position) {
        const auto rank = std::to_string(input.cells[static_cast<std::size_t>(position)].skill_level);
        next.push_back({std::string(root) + "buttons/btn_skill" + std::to_string(position) + "/cnt/value", rank});
        next.push_back({std::string(root) + "buttons/skill" + std::to_string(position) + "/cnt/value", rank});
    }
    output = std::move(next);
    error.clear();
    return true;
}

bool project_skill_page_current_next_v1(
    const SkillPageCurrentNextRequestV1& request,
    const SkillPageCurrentNextTextV1& source,
    const SkillPageSymbolTextV1& resolver,
    std::vector<SkillPageTextBindingV1>& output,
    std::string& error) {
    if (!request.state || request.skill_list_id < 0 || request.class_skill_position < 0 ||
        request.skill_table_id < 0)
        return fail(error, "Original Skills current/next projection requires complete same-state source identity");
    if (source.state != request.state || source.skill_list_id != request.skill_list_id ||
        source.class_skill_position != request.class_skill_position ||
        source.skill_table_id != request.skill_table_id || source.saved_rank != request.saved_rank ||
        source.character_level != request.character_level)
        return fail(error, "Original Skills current/next result is stale or belongs to another source row/state");
    if (!source.available) {
        output.clear();
        error.clear();
        return true;
    }
    if (!resolver) return fail(error, "Original Skills current/next heading resolver unavailable");

    std::vector<SkillPageTextBindingV1> next;
    std::string current_heading, next_heading;
    if (!resolver("GAMEPLAYMENUS_current_level", current_heading, error)) return false;
    if (!resolver("GAMEPLAYMENUS_next_level", next_heading, error)) return false;
    if (current_heading.empty() || next_heading.empty())
        return fail(error, "Original Skills current/next source heading resolved empty");
    current_heading += "\n\n";
    next_heading += "\n\n";
    current_heading += source.current_level;
    next_heading += source.next_level;
    next.push_back({std::string(root) + "current_skill_description/text", std::move(current_heading)});
    next.push_back({std::string(root) + "next_skill_description/text", std::move(next_heading)});
    output = std::move(next);
    error.clear();
    return true;
}

bool project_skill_page_cells_v1(const SkillPageTextInputV1& input,
                                 std::array<SkillPageCellPresentationV1, 16>& output,
                                 std::string& error) {
    std::array<SkillPageCellPresentationV1, 16> next{};
    for (std::size_t i = 0; i < input.cells.size(); ++i) {
        const auto& source = input.cells[i];
        auto& projected = next[i];
        projected.skill_level = source.skill_level;
        projected.skill_icon_frame = source.skill_icon_frame;
        projected.button_disabled = !source.skill_assignable || source.skill_level < 1;
        projected.lock_visible = !source.skill_unlocked;
        projected.grey_visible = source.skill_level < 1 && source.skill_icon_frame != "blank";
        if (source.skill_level < 1 && source.skill_icon_frame == "blank")
            projected.lock_visible = false;
    }
    output = std::move(next);
    error.clear();
    return true;
}

bool skill_page_source_field_style_v1(std::string_view path,
                                     SkillPageSourceFieldStyleV1& output,
                                     std::string& error) {
    SkillPageSourceFieldStyleV1 next;
    if (!parse_source_field(path, next.field_character, next.font_character, next.rgba))
        return fail(error, "No exact source Skills DefineEditText style for field path");
    output = next;
    error.clear();
    return true;
}

bool layout_skill_page_text_v1(std::string_view path,
                               MenuTextLayoutFieldV1 geometry,
                               std::string_view source_html,
                               const MenuTextAdvanceV1& advance,
                               MenuTextLayoutV1& output,
                               std::string& error) {
    SkillPageSourceFieldStyleV1 style;
    if (!skill_page_source_field_style_v1(path, style, error)) return false;
    geometry.movie = SourceMenuMovieV1::character_menu;
    geometry.page_character = 496;
    geometry.field_character = style.field_character;
    geometry.source_font_character = style.font_character;
    geometry.source_rgba = style.rgba;
    return menu_text_layout_v1(geometry, source_html, advance, output, error);
}

} // namespace dh::foundation::character_menu
