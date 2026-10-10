#pragma once

#include "menu_text_layout_v1_flags.hpp"
#include <array>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace dh::foundation::character_menu {

struct SourceTextFlagsV1 {
    std::uint16_t raw{};
    bool has_text{}, word_wrap{}, multiline{}, password{}, read_only{};
    bool has_text_color{}, has_max_length{}, has_font{}, has_font_class{};
    bool auto_size{}, has_layout{}, no_select{}, border{}, was_static{};
    bool html{}, use_outlines{};
};

bool source_text_flags_v1(SourceMenuMovieV1 movie, std::uint16_t page_character,
                          std::uint16_t field_character, SourceTextFlagsV1&,
                          std::string& error);

struct MenuTextLayoutFieldV1 {
    SourceMenuMovieV1 movie{SourceMenuMovieV1::character_menu};
    std::uint16_t page_character{}, field_character{};
    // Source local RECT in authored pixels: xmin,xmax,ymin,ymax.
    std::array<float, 4> local_rect{};
    // Source text-field matrix in authored pixels.
    std::array<float, 6> matrix{1, 0, 0, 1, 0, 0};
    std::array<float, 3> margins{}; // left,right,indent
    unsigned alignment{}; // 0 left, 1 right, 2 centre
    float source_height{};
    float paragraph_leading{};
    float font_descent{}, font_leading{}; // original font units, not device glyph bounds
    float root_scale_word{1};
    bool define_font3{};
    // Must come from this exact authored DefineEditText, not a renderer/theme default.
    std::uint16_t source_font_character{};
    std::array<float, 4> source_rgba{};
};

struct MenuTextRunV1 {
    std::string text;
    float advance{};
    std::uint16_t font_character{}; // the field's authored font; color markup never changes it
    std::array<float, 4> rgba{};    // source color, overridden only by supported FONT color tags
};

struct MenuTextLineV1 {
    std::string text;
    float advance{};
    std::vector<MenuTextRunV1> runs;
    std::array<float, 2> baseline{}; // transformed source baseline
    std::array<float, 4> clip_rect{}; // source-local RECT; apply clip_matrix
    std::array<float, 6> clip_matrix{};
};

struct MenuTextLayoutV1 {
    SourceTextFlagsV1 flags;
    std::vector<MenuTextLineV1> lines;
};

// The caller measures candidate strings with its retained HudGlyphFont (raster
// the string and return HudGlyphRun::advance). This helper owns no font, art,
// color or renderer state. It uses the source SWF flags and source field
// geometry to return positioned runs and local clipping rectangles.
using MenuTextAdvanceV1 = std::function<bool(std::string_view, float&, std::string&)>;
bool menu_text_layout_v1(const MenuTextLayoutFieldV1&, std::string_view text,
                         const MenuTextAdvanceV1&, MenuTextLayoutV1&,
                         std::string& error);

} // namespace dh::foundation::character_menu
