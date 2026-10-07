#include "swf_glyph_lookup.hpp"
namespace {
using namespace dh2::ui;
int invoke(const GlyphLookupServices16& s,GlyphLookupService k,
    GlyphLookup48& g,const GlyphLookupFont48& f,std::uintptr_t& identity,
    std::int32_t& index,std::uint32_t& found) noexcept {
    GlyphLookupRequest48 r{k,0,identity,&g,&f,index,found,0};
    if(s.invoke(s.context,&r))return -2;
    identity=r.identity;index=r.index;found=r.found;return 0;
}
int finish(GlyphLookup48& g,const GlyphLookupFont48& f) noexcept {
    if(f.zone_count)g.advance=g.advance*20.0f;return 1;
}
}
extern "C" int dh2_swf_glyph_lookup(dh2::ui::GlyphLookup48* out,
    const dh2::ui::GlyphLookupFont48* f,const dh2::ui::GlyphLookupServices16* services) noexcept {
    using namespace dh2::ui;
    if(!out||!f||!services||!services->invoke||out->reserved||!f->name||
        f->code>65535||f->bold>1||f->italic>1||f->bitmap_provider>1||
        f->freetype_provider>1||f->advance_count>32768||
        (f->advance_count&&!f->advances))return -1;
    const auto s=*services;out->advance=512.0f;out->index=-1;
    std::uintptr_t identity=0;std::int32_t index=-1;std::uint32_t found=0;
    if(f->bitmap_provider){
        out->bitmap_flag=0;
        if(invoke(s,GlyphLookupService::bitmap_face,*out,*f,identity,index,found))return -2;
        out->face=identity;
        if(identity){
            if(invoke(s,GlyphLookupService::bitmap_image,*out,*f,identity,index,found))return -2;
            out->bitmap=identity;
            if(out->bitmap)return finish(*out,*f);
        }
    }
    if(f->freetype_provider){
        out->bitmap_flag=0;identity=0;
        if(invoke(s,GlyphLookupService::freetype_image,*out,*f,identity,index,found))return -2;
        out->bitmap=identity;identity=0;
        if(invoke(s,GlyphLookupService::freetype_face,*out,*f,identity,index,found))return -2;
        out->face=identity;
        if(out->bitmap)return finish(*out,*f);
    }
    identity=0;index=-1;found=0;
    if(invoke(s,GlyphLookupService::embedded_lookup,*out,*f,identity,index,found))return -2;
    if(found>1||(found&&(index<0||index>32767)))return -2;
    if(!found)return 0;
    out->index=index;
    if(static_cast<std::uint32_t>(index)<f->advance_count){
        out->advance=f->advances[index];return 1;
    }
    return finish(*out,*f);
}
