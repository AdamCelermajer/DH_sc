#pragma once

#include "texture_loader.hpp"
#include <array>
#include <memory>

namespace dh::foundation {

struct HudGlyphQuad {
    std::uint32_t codepoint{};
    TextureImage image; // matching original FreeType2.3.7 RGBA alpha image
    float x{}, y{}, width{}, height{}; // source pixels relative to text baseline
    float u1{}, v1{}; // cropped UVs omit original power-of-two image padding
    float advance{}; // this glyph's device-font advance in source pixels
};
struct HudGlyphRun {
    std::vector<HudGlyphQuad> glyphs;
    float advance{}; // source pixels; centre-align the complete run by this
    std::array<float,4> bounds{}; // xmin,xmax,ymin,ymax relative to baseline
    std::uint32_t source_layout_font{}; // set by original SWF font-table layout
    float source_layout_height{};
};

// Loads supplied original font bytes; no installed/system font substitution.
// Original dqhud font7 is Fontin SmallCaps, data/Fontin SmallCaps.ttf.
class HudGlyphFont {
public:
    HudGlyphFont();
    ~HudGlyphFont();
    HudGlyphFont(const HudGlyphFont&) = delete;
    HudGlyphFont& operator=(const HudGlyphFont&) = delete;
    bool load(const std::filesystem::path& original_ttf, std::string& error);
    // Strict UTF8, source BMP codepoints, bounded single line. font_size is
    // source pixels (target fields12); pixel_scale chooses glyph raster detail.
    // Source glyph advances/bearings retained; no replacement text invented.
    bool raster(const std::string& utf8, int font_size, float pixel_scale,
                HudGlyphRun& run, std::string& error);
    static const char* version();
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace dh::foundation
