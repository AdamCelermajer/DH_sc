#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace dh2::ui {
struct FreetypeGlyph {
    std::uint32_t width{}, height{}, bitmap_width{}, bitmap_height{};
    // Original alpha image: zero-padded power-of-two dimensions, positive pitch=width.
    std::vector<std::uint8_t> alpha;
    float bounds[4]{}; // xmin,xmax,ymin,ymax, original normalized glyph bounds
    float advance{};   // original 1024-em advance, before font3 caller multiplication
    std::int32_t pixel_height{};
};
class FreetypeFont {
public:
    FreetypeFont(); ~FreetypeFont();
    FreetypeFont(const FreetypeFont&)=delete; FreetypeFont& operator=(const FreetypeFont&)=delete;
    // Own exact caller bytes for the entire FT_Face lifetime. No filesystem/font fallback.
    bool load(const std::uint8_t*,std::size_t,std::string& error);
    // Source no-cache RenderFX path; scale is the genuine context parameter (default 1).
    // Output remains unchanged on malformed request or FreeType error.
    bool raster(FreetypeGlyph&,std::uint32_t code,std::int32_t font_size,float scale,std::string& error);
    static const char* version();
private:
    struct Impl; std::unique_ptr<Impl> impl_;
};
}
