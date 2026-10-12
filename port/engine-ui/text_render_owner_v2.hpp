#pragma once
#include "text_display_v2.hpp"
#include "swf_movie.hpp"
#include "swf_freetype_provider.hpp"

namespace dh2::ui {
struct TextBitmapFaceV2 { std::shared_ptr<void> owner; float height{}; };
struct TextEmbeddedGlyphV2 { bool found{};std::int16_t index{-1};bool has_advance{};float advance{}; };
struct TextFontBackendsV2 {
    // Both are required producers: empty owner is a delivered bitmap-font
    // miss, not a missing resolver. Caller is the actual cache/table owner.
    std::function<bool(const text_v1::Font&,TextBitmapFaceV2&,std::string&)> bitmap_face;
    std::function<bool(const TextBitmapFaceV2&,std::uint16_t,std::int32_t,
                       text_v1::Glyph&,std::string&)> bitmap_glyph;
    std::function<bool(const text_v1::Font&,std::uint16_t,TextEmbeddedGlyphV2&,std::string&)> embedded_glyph;
};
// Owns one actual FT2.3.7 face per resolved font and one uploaded native image
// per glyph/size. The same face supplies layout metrics, raster and identity.
// Renderer/resource owners outlive the cache and every returned image pin.
class TextRenderOwnerV2 {
    struct Impl;std::shared_ptr<Impl> impl_;
public:
    TextRenderOwnerV2(SwfFontServices,SwfServices,std::shared_ptr<void> platform_owner,
                      TextFontBackendsV2,float provider_scale);
    // Register a real uploaded texture, including inline images or atlas
    // textures. Arbitrary void pointers cannot enter the renderer.
    std::shared_ptr<void> bitmap(SwfTexture,std::shared_ptr<void> texture_owner,
                                 std::shared_ptr<void> face,std::string&);
    bool units(const std::shared_ptr<text_v1::Font>&,float&,std::string&);
    bool height(const std::shared_ptr<text_v1::Font>&,float&,std::string&);
    bool glyph(const std::shared_ptr<text_v1::Font>&,std::uint16_t,std::int32_t,
               text_v1::Glyph&,bool& found,std::string&);
    // Clone copies the layout projection of the source descriptor/metrics;
    // private encoding/ascent/table-hint metadata is outside this projection.
    // Embedded glyphs, kerning,
    // owner movie and Font3 zones are intentionally not inherited.
    bool clone(const std::shared_ptr<text_v1::Font>&,std::shared_ptr<text_v1::Font>&,std::string&);
    // Add the owned renderer and font producers to a caller's complete source
    // graph services. Caller supplies root scale, real kerning/image/preload,
    // font resolution, color transform, cache regions and embedded shape draw.
    text_v1::Services layout_services(text_v1::Services) const;
    text_display_v2::Services display_services(text_display_v2::Services) const;
};
}
