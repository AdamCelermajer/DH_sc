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
}
