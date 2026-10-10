#pragma once

#include "menu_text_layout_v1.hpp"
#include "../../character_state.hpp"

#include <array>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace dh::foundation::character_menu {

using SkillPageSymbolTextV1 = std::function<bool(
    std::string_view symbol, std::string& localized_text, std::string& error)>;

struct SkillPageTextInputV1 {
    // These are the exact native-owner results consumed by the authored AS:
    // NativeSkillsGetSkillPointsLeft and NativeGetSkillDetails.SkillLevel.
    int skill_points_left{};
    struct Cell {
        int skill_level{};
        bool skill_unlocked{};
        bool skill_assignable{};
        std::string skill_icon_frame;
    };
    // Must be the complete NativeGetSkillDetails projection from the current
    // class list and same live CharacterState, not a fixed first-rank sample.
    std::array<Cell, 16> cells{};
    SkillPageSymbolTextV1 symbol_text;
};

struct SkillPageCellPresentationV1 {
    int skill_level{};
    std::string skill_icon_frame;
    bool button_disabled{};
    bool lock_visible{};
    bool grey_visible{};
};

struct SkillPageTextBindingV1 {
    std::string field_path;
    std::string text;
};

// Exact same-state facts used to ask the native SkillDetails provider for the
// current/next descriptions. The provider must return text already produced
// by the original SkillTable OIDs + AI_SkillInfo/parseEx path; this projection
// does not calculate skill properties or apply Faery offsets itself.
struct SkillPageCurrentNextRequestV1 {
    const CharacterState* state{}; // borrowed only for the synchronous query
    int skill_list_id = -1;
    int class_skill_position = -1;
    int skill_table_id = -1;
    std::uint32_t saved_rank{};
    std::uint32_t character_level{};
};

struct SkillPageCurrentNextTextV1 {
    const CharacterState* state{};
    int skill_list_id = -1;
    int class_skill_position = -1;
    int skill_table_id = -1;
    std::uint32_t saved_rank{};
    std::uint32_t character_level{};
    // Source NativeGetSkillDetails SkillCurrLevel/SkillNextLevel strings after
    // ParseEx, before the AS caller prefixes localized headings.
    std::string current_level;
    std::string next_level;
    bool available = true;
    std::string unavailable_reason;
};

struct SkillPageSourceFieldStyleV1 {
    std::uint16_t field_character{};
    std::uint16_t font_character{};
    std::array<float, 4> rgba{};
};

// Projects only text receivers assigned by original sprite496 onShow and
// presetAllSkills. All labels come from the supplied original symbol resolver;
// points and ranks come from the same live native skill-page query.
bool project_skill_page_text_v1(const SkillPageTextInputV1&,
                                std::vector<SkillPageTextBindingV1>&,
                                std::string& error);

// Adds the original localized current/next headings around already-source-
// formatted strings. Every request fact must match the returned source proof;
// stale rank, row, list, CharacterState, or level is rejected atomically.
bool project_skill_page_current_next_v1(
    const SkillPageCurrentNextRequestV1&,
    const SkillPageCurrentNextTextV1&,
    const SkillPageSymbolTextV1&,
    std::vector<SkillPageTextBindingV1>&,
    std::string& error);

// Replays the source sprite496 presetAllSkills visibility/disabled rules from
// each native SkillLevel/SkillUnlocked/SkillAssignable/SkillIcon result.
bool project_skill_page_cells_v1(const SkillPageTextInputV1&,
                                 std::array<SkillPageCellPresentationV1, 16>&,
                                 std::string& error);

// Exact DefineEditText source font/color for original sprite496 fields. The
// field geometry/margins/metrics remain caller-owned parsed source values.
bool skill_page_source_field_style_v1(std::string_view field_path,
                                     SkillPageSourceFieldStyleV1&,
                                     std::string& error);

// Connects a source field's exact font/color and character ID to the generic
// menu text layout, preserving inline source HTML colors such as Active/red.
bool layout_skill_page_text_v1(std::string_view field_path,
                               MenuTextLayoutFieldV1 source_geometry,
                               std::string_view source_html,
                               const MenuTextAdvanceV1& original_font_advance,
                               MenuTextLayoutV1&,
                               std::string& error);

} // namespace dh::foundation::character_menu
