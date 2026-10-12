#pragma once
#include "localization.hpp"
namespace dh2::ui {
// Genuine fonts PyData FontPalette projection. Immutable source rows retain
// glowcolor and textcolor; item-name queries read source textcolor only.
class CharacterMenuFontPaletteV1 {
 std::vector<std::array<std::uint32_t,2>> rows_;
public:
 bool load(LocalizationBytes records,LocalizationBytes schema,std::string&);
 bool text_color(std::int32_t row,std::uint32_t&,std::string&)const;
};
// Display-only native item-name wrapping. The caller resolves font_palette_row
// from the same live ItemInstance and authentic ItemPowerColor source owner.
// Keeping constant lookup outside MenuLocalization avoids borrowing a source
// group that is absent from that menu's common_text/fonts constant tables.
bool character_menu_item_name_color_v1(const std::string& raw_name,
    std::int32_t font_palette_row,
    const CharacterMenuFontPaletteV1&, std::string&, std::string&);
}
