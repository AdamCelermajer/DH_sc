#include "hud_freetype_font.hpp"
#include "freetype_glyph_kernel.hpp"
#include "freetype_bitmap_alpha.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <cmath>
#include <cstring>
#include <limits>
#include <utility>
namespace dh2::ui {
struct HudFreetypeFont::Impl {
    FT_Library library{}; FT_Face face{}; std::vector<std::uint8_t> bytes;
    ~Impl() { if(face) FT_Done_Face(face); if(library) FT_Done_FreeType(library); }
};
HudFreetypeFont::HudFreetypeFont()=default;
HudFreetypeFont::~HudFreetypeFont()=default;
const char* HudFreetypeFont::version() { return "2.3.7"; }
bool HudFreetypeFont::load(const std::uint8_t* bytes,std::size_t size,std::string& error) {
    error.clear();
    if(!bytes || !size || size>64*1024*1024) { error="invalid font byte span";return false; }
    auto next=std::make_unique<Impl>();next->bytes.assign(bytes,bytes+size);
    if(FT_Init_FreeType(&next->library)) { error="FreeType initialization failed";return false; }
    FT_Int major{},minor{},patch{};FT_Library_Version(next->library,&major,&minor,&patch);
    if(major!=2 || minor!=3 || patch!=7) { error="FreeType source version differs from original 2.3.7";return false; }
    if(FT_New_Memory_Face(next->library,next->bytes.data(),static_cast<FT_Long>(size),0,&next->face)) {
        error="FreeType rejected font bytes";return false;
    }
    impl_=std::move(next);return true;
}
bool HudFreetypeFont::raster(FreetypeGlyph& out,std::uint32_t code,std::int32_t font_size,float scale,std::string& error,HudBitmapInfo32* info) {
    error.clear();
    const float pixels=static_cast<float>(font_size)*scale;
    if(!impl_ || code>65535 || font_size<=0 || !std::isfinite(scale) || scale<0 ||
       !std::isfinite(pixels) || pixels<0 || pixels>8192) { error="invalid glyph raster request";return false; }
    const auto pixel_height=static_cast<std::int32_t>(pixels);
    if(FT_Set_Pixel_Sizes(impl_->face,0,static_cast<FT_UInt>(pixel_height))) { error="FreeType pixel size failed";return false; }
    if(FT_Load_Char(impl_->face,code,FT_LOAD_RENDER)) { error="FreeType glyph load failed";return false; }
    const auto& bitmap=impl_->face->glyph->bitmap;
    const auto& metrics=impl_->face->glyph->metrics;
    if(bitmap.pixel_mode<FT_PIXEL_MODE_MONO || bitmap.pixel_mode>FT_PIXEL_MODE_GRAY4 || bitmap.pitch<0 || bitmap.width<0 || bitmap.rows<0 ||
       bitmap.pitch>16384 || bitmap.rows>16384 ||
       (bitmap.width && bitmap.rows && !bitmap.buffer)) { error="unsupported source bitmap format";return false; }
    const FT_Pos words[]={metrics.width,metrics.height,metrics.horiBearingX,metrics.horiBearingY,metrics.horiAdvance};
    for(auto v:words) if(v<std::numeric_limits<std::int32_t>::min() || v>std::numeric_limits<std::int32_t>::max()) {
        error="glyph metrics exceed original signed32 domain";return false;
    }
    const FreetypeMetrics32 source_metrics{static_cast<std::int32_t>(metrics.width),static_cast<std::int32_t>(metrics.height),
        static_cast<std::int32_t>(metrics.horiBearingX),static_cast<std::int32_t>(metrics.horiBearingY),static_cast<std::int32_t>(metrics.horiAdvance),
        bitmap.width,bitmap.rows,bitmap.pixel_mode==FT_PIXEL_MODE_GRAY?bitmap.pitch:bitmap.width};
    FreetypeLayout40 layout;
    if(dh2_freetype_glyph_layout(&layout,&source_metrics,font_size,scale)!=1) { error="glyph image exceeds native budget";return false; }
    FreetypeGlyph next;next.width=layout.width;next.height=layout.height;
    next.alpha.assign(static_cast<std::size_t>(next.width)*next.height,0);
    const FreetypeBitmap40 input{bitmap.buffer,static_cast<std::size_t>(bitmap.rows)*bitmap.pitch,
        bitmap.width,bitmap.rows,bitmap.pitch,static_cast<std::uint32_t>(bitmap.pixel_mode),
        static_cast<std::uint32_t>(bitmap.num_grays),0};
    FreetypeAlpha32 alpha{next.alpha.data(),next.alpha.size(),0,0,0,0};
    if(dh2_freetype_bitmap_alpha(&alpha,&input)!=1 || alpha.width!=next.width || alpha.height!=next.height) {
        error="malformed source bitmap span";return false;
    }
    next.bitmap_width=static_cast<std::uint32_t>(bitmap.width);next.bitmap_height=static_cast<std::uint32_t>(bitmap.rows);
    std::memcpy(next.bounds,layout.bounds,sizeof next.bounds);next.advance=layout.advance;
    next.pixel_height=pixel_height;
    if(info)*info={code,font_size,static_cast<std::uint32_t>(bitmap.pixel_mode),bitmap.width,bitmap.rows,bitmap.pitch,
        static_cast<std::uint32_t>(bitmap.num_grays),bitmap.pixel_mode!=FT_PIXEL_MODE_GRAY};
    out=std::move(next);return true;
}
}
