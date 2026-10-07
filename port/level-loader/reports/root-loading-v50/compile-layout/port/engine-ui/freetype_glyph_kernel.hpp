#pragma once
#include <cstdint>
namespace dh2::ui {
struct FreetypeMetrics32 { std::int32_t width,height,bearing_x,bearing_y,advance,bitmap_width,bitmap_height,pitch; };
struct FreetypeLayout40 { std::uint32_t width,height;float bounds[4],advance;std::int32_t pixel_height;std::uint32_t bitmap_width,bitmap_height; };
static_assert(sizeof(FreetypeMetrics32)==32 && sizeof(FreetypeLayout40)==40);
}
extern "C" int dh2_freetype_glyph_layout(dh2::ui::FreetypeLayout40*,const dh2::ui::FreetypeMetrics32*,std::int32_t font_size,float scale);
