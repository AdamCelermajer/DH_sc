#pragma once
#include "hud_freetype_font.hpp"
namespace dh2::ui {
class HudFreetypeFontV2 {
public:
    HudFreetypeFontV2();~HudFreetypeFontV2();
    HudFreetypeFontV2(const HudFreetypeFontV2&)=delete;
    HudFreetypeFontV2& operator=(const HudFreetypeFontV2&)=delete;
    bool load(const std::uint8_t*,std::size_t,std::string&);
    bool raster(FreetypeGlyph&,std::uint32_t,std::int32_t,float,std::string&,HudBitmapInfo32* info=nullptr);
    bool metrics(float& units,float& height,std::string&) const;
    static const char* version();
private:
    struct Impl;std::unique_ptr<Impl> impl_;
};
}
