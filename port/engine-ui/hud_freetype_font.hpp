#pragma once
#include "freetype_font.hpp"
namespace dh2::ui {
struct HudBitmapInfo32 {
    std::uint32_t code;
    std::int32_t font_size;
    std::uint32_t pixel_mode;
    std::int32_t width,rows,pitch;
    std::uint32_t num_grays,packed_conversion;
};
static_assert(sizeof(HudBitmapInfo32)==32);
class HudFreetypeFont {
public:
    HudFreetypeFont();~HudFreetypeFont();
    HudFreetypeFont(const HudFreetypeFont&)=delete;
    HudFreetypeFont& operator=(const HudFreetypeFont&)=delete;
    bool load(const std::uint8_t*,std::size_t,std::string&);
    bool raster(FreetypeGlyph&,std::uint32_t,std::int32_t,float,std::string&,HudBitmapInfo32* info=nullptr);
    static const char* version();
private:
    struct Impl;std::unique_ptr<Impl> impl_;
};
}
