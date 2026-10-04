#pragma once
#include "text_layout_v1.hpp"
#include <optional>

namespace dh2::ui::text_display_v2 {
// Native owners, never ARM object layouts. Matrix order matches GameSWF's
// two rows: m00, m01, tx, m10, m11, ty. Rect order is xmin,xmax,ymin,ymax.
using Matrix = std::array<float,6>;
using Rect = std::array<float,4>;
struct Bitmap { std::shared_ptr<void> owner; std::int32_t width{},height{}; };
struct GlyphBinding { Bitmap bitmap; std::shared_ptr<void> face; };
struct Filter { std::uint8_t kind{},x{},y{}; };
struct Context {
    Matrix matrix{1,0,0,0,1,0};
    float provider_scale{1}, pixel_scale{1};
    bool renderer_present{true}, freetype_cache{}, bitmap_cache{};
    std::shared_ptr<void> freetype_atlas;
    std::optional<std::uint32_t> override_rgba;
    // Original arguments in call order: strength, horizontal, vertical.
    // Their semantic filter names remain a caller/backend boundary.
    std::uint8_t argument0{},argument1{},argument2{};
};
struct Services {
    std::function<bool(text_v1::State&,std::size_t,std::string&)> resolve_font;
    std::function<bool(std::uint32_t,std::uint32_t&,std::string&)> transform_color;
    std::function<bool(const text_v1::Glyph&,GlyphBinding&,std::string&)> bind_glyph;
    // Atlas regions are pixel coordinates, delivered from the actual cache.
    // Filter outputs occupy a separate rect, as in the original ABI.
    std::function<bool(bool,const text_v1::Glyph&,const GlyphBinding&,Filter,Rect&,std::string&)> region;
    std::function<bool(const Matrix&,std::uint32_t,const float*,std::size_t,std::string&)> line;
    std::function<bool(const Matrix&,const Bitmap&,const Rect&,const Rect&,std::uint32_t,std::string&)> bitmap;
    std::function<bool(const Matrix&,const std::shared_ptr<text_v1::Font>&,std::int16_t,
                       float,std::uint32_t,std::string&)> shape;
};
// Display the whole mutable record array. Callback mutations are reread before
// the next advance/record, while current owners remain pinned. A callback that
// removes its currently executing source storage is an explicit unsafe-domain
// error, rather than a stale native reference.
bool display(text_v1::State&,const Context&,const Services&,std::string&);
}
