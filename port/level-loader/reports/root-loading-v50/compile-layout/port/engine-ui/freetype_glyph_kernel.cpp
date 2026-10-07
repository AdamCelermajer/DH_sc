#include "freetype_glyph_kernel.hpp"
#include <cmath>
extern "C" int dh2_freetype_glyph_layout(dh2::ui::FreetypeLayout40* out,const dh2::ui::FreetypeMetrics32* in,std::int32_t size,float scale) {
    const float pixels=static_cast<float>(size)*scale;
    if(!out || !in || size<=0 || size>65535 || !std::isfinite(scale) || scale<0 ||
       !std::isfinite(pixels) || pixels<0 || pixels>8192 || in->bitmap_width<0 || in->bitmap_height<0 ||
       in->pitch<in->bitmap_width || in->pitch>16384 || in->bitmap_height>16384) return -1;
    dh2::ui::FreetypeLayout40 next{};next.width=4;next.height=1;
    while(next.width<static_cast<std::uint32_t>(in->pitch))next.width*=2;
    while(next.height<static_cast<std::uint32_t>(in->bitmap_height))next.height*=2;
    if(static_cast<std::uint64_t>(next.width)*next.height>64*1024*1024)return -1;
    next.bounds[1]=static_cast<float>(in->bitmap_width)/static_cast<float>(next.width);
    next.bounds[3]=static_cast<float>(in->bitmap_height)/static_cast<float>(next.height);
    const float x=in->width>0 ? static_cast<float>(in->bearing_x)/static_cast<float>(in->width):0.0f;
    const float y=in->height>0 ? static_cast<float>(in->bearing_y)/static_cast<float>(in->height):0.0f;
    next.bounds[0]=x*-next.bounds[1];next.bounds[2]=y*next.bounds[3];
    next.advance=static_cast<float>(in->advance)*(16.0f/static_cast<float>(size));
    next.pixel_height=static_cast<std::int32_t>(pixels);
    next.bitmap_width=static_cast<std::uint32_t>(in->bitmap_width);next.bitmap_height=static_cast<std::uint32_t>(in->bitmap_height);
    *out=next;return 1;
}
