#include "freetype_bitmap_alpha.hpp"
extern "C" int dh2_freetype_bitmap_alpha(dh2::ui::FreetypeAlpha32* out,
    const dh2::ui::FreetypeBitmap40* in) noexcept {
    if(!out||!in||!out->bytes||out->reserved||in->width<0||in->rows<0||
        in->width>16384||in->rows>16384||in->pitch==(-2147483647-1)||
        in->pixel_mode<1||in->pixel_mode>4)return -1;
    const unsigned bits=in->pixel_mode==1?1:in->pixel_mode==2?8:in->pixel_mode==3?2:4;
    const auto pitch=in->pitch<0?-std::int64_t(in->pitch):std::int64_t(in->pitch);
    const auto row_bytes=(std::uint64_t(in->width)*bits+7)/8;
    if(pitch>16384||std::uint64_t(pitch)<row_bytes)return -1;
    if(in->rows&&in->width){
        if(!in->bytes)return -1;
        const auto last=std::int64_t(in->row0_offset)+std::int64_t(in->rows-1)*in->pitch;
        const auto lo=last<std::int64_t(in->row0_offset)?last:std::int64_t(in->row0_offset);
        const auto hi=last>std::int64_t(in->row0_offset)?last:std::int64_t(in->row0_offset);
        if(lo<0||std::uint64_t(hi)+row_bytes>in->size)return -1;
    }
    std::uint32_t width=4,height=1;
    const auto logical_pitch=bits==8?std::uint32_t(pitch):std::uint32_t(in->width);
    while(width<logical_pitch)width*=2;
    while(height<std::uint32_t(in->rows))height*=2;
    const auto size=std::uint64_t(width)*height;
    if(size>64*1024*1024||size>out->capacity)return -1;
    for(std::size_t i=0;i<std::size_t(size);++i)out->bytes[i]=0;
    const auto mask=(1u<<bits)-1u,multiplier=255u/mask;
    for(std::int32_t y=0;y<in->rows;++y){
        if(!in->width)continue;
        const auto* row=in->bytes+std::int64_t(in->row0_offset)+std::int64_t(y)*in->pitch;
        for(std::int32_t x=0;x<in->width;++x){
            const auto bit=std::uint32_t(x)*bits;
            out->bytes[std::size_t(y)*width+x]=std::uint8_t(((row[bit/8]>>(8-bits-bit%8))&mask)*multiplier);
        }
    }
    out->width=width;out->height=height;out->pitch=width;return 1;
}
