#include "text_render_owner_v2.hpp"
#include "hud_freetype_font_v2.hpp"
#include <algorithm>
#include <cmath>
#include <map>
#include <tuple>

namespace dh2::ui {
namespace {
bool need(bool ok,const char* what,std::string& error){if(ok)return true;error=std::string("text owner requires ")+what;return false;}
void color(std::uint8_t* bytes,std::uint32_t value){for(unsigned i=0;i<4;++i)bytes[i]=(value>>(i*8))&255;}
struct DeviceFontDescriptor { std::string name;bool bold{},italic{};float descent{},leading{}; };
}
struct TextRenderOwnerV2::Impl {
    struct Image {
        SwfTexture texture;std::shared_ptr<void> platform,texture_owner,face;
    };
    struct Face {
        HudFreetypeFontV2 font;
        struct Glyph { FreetypeGlyph raster;std::weak_ptr<Image> image; };
        std::map<std::uint32_t,Glyph> glyphs;
    };
    SwfFontServices fonts;SwfServices renderer;std::shared_ptr<void> platform;
    TextFontBackendsV2 backends;float scale;
    std::map<std::tuple<std::string,bool,bool>,std::shared_ptr<Face>> faces;
    std::map<std::weak_ptr<void>,std::weak_ptr<Image>,std::owner_less<std::weak_ptr<void>>> images;
    std::map<std::weak_ptr<void>,bool,std::owner_less<std::weak_ptr<void>>> clones;
    Impl(SwfFontServices f,SwfServices r,std::shared_ptr<void> p,TextFontBackendsV2 b,float z)
        :fonts(f),platform(std::move(p)),backends(std::move(b)),scale(z){
        // Only synchronous render services are borrowed; a movie's retained
        // graph/provider tree must not be copied into this texture cache.
        renderer.context=r.context;renderer.image=r.image;renderer.draw=r.draw;
    }
    bool cloned(const text_v1::Font& f) const {return clones.find(std::weak_ptr<void>(f.native))!=clones.end();}
    bool face(const text_v1::Font& f,std::shared_ptr<Face>& out,std::string& error){
        const auto key=std::make_tuple(f.name,f.bold,f.italic);auto it=faces.find(key);
        if(it!=faces.end()){out=it->second;return true;}
        if(!need(bool(platform)&&bool(fonts.read),"owned font resolver",error))return false;
        std::vector<std::uint8_t> bytes;
        if(!fonts.read(fonts.context,f.name.c_str(),f.bold,f.italic,bytes,error)){
            if(!error.empty())return false;
            faces.emplace(key,nullptr);out.reset();return true;
        }
        auto next=std::make_shared<Face>();if(!next->font.load(bytes.data(),bytes.size(),error))return false;
        out=next;faces.emplace(key,std::move(next));return true;
    }
    bool bitmap_face(const std::shared_ptr<text_v1::Font>& f,TextBitmapFaceV2& out,std::string& error){
        return need(bool(f),"source font",error)&&need(bool(backends.bitmap_face),"bitmap font resolver",error)&&backends.bitmap_face(*f,out,error);
    }
    std::shared_ptr<Image> lookup(const std::shared_ptr<void>& id){
        auto it=images.find(std::weak_ptr<void>(id));return it==images.end()?nullptr:it->second.lock();
    }
    std::shared_ptr<Image> image(SwfTexture texture,std::shared_ptr<void> owner,std::shared_ptr<void> face,std::string& error){
        if(!need(texture.identity && texture.width>0 && texture.height>0 && bool(owner)&&bool(platform),"valid owned texture",error))return {};
        for(auto it=images.begin();it!=images.end();)if(it->first.expired())it=images.erase(it);else++it;
        auto p=std::make_shared<Image>();p->texture=texture;p->platform=platform;p->texture_owner=std::move(owner);p->face=std::move(face);
        images.emplace(std::weak_ptr<void>(p),p);return p;
    }
    bool metrics(const std::shared_ptr<text_v1::Font>& f,float& out,bool height,std::string& error){
        error.clear();
        TextBitmapFaceV2 bitmap;
        if(!bitmap_face(f,bitmap,error))return false;
        if(bitmap.owner){out=height?bitmap.height*20.0f:1024.0f;return true;}
        std::shared_ptr<Face> ft;if(!face(*f,ft,error))return false;
        if(!ft){out=height?0.0f:1.0f;return true;}
        float u{},h{};if(!ft->font.metrics(u,h,error))return false;out=height?h:u;return true;
    }
    bool glyph(const std::shared_ptr<text_v1::Font>& f,std::uint16_t code,int size,text_v1::Glyph& out,bool& found,std::string& error){
        error.clear();
        TextBitmapFaceV2 bf;if(!bitmap_face(f,bf,error))return false;
        out.advance=512;out.index=-1;out.type=0;found=false;
        if(bf.owner){
            out.image.reset();
            if(!need(bool(backends.bitmap_glyph),"bitmap glyph producer",error)||!backends.bitmap_glyph(bf,code,size,out,error))return false;
            if(out.image){if(f->define_font3)out.advance*=20;found=true;return true;}
        }
        std::shared_ptr<Face> ft;if(!face(*f,ft,error))return false;
        out.image.reset();
        if(ft){
            if(!need(size>0 && size<=65535 && std::isfinite(scale)&&scale>=0,"source font size/scale",error))return false;
            const auto key=std::uint32_t(code)|(std::uint32_t(size)<<16);auto it=ft->glyphs.find(key);
            if(it==ft->glyphs.end()){
                FreetypeGlyph raster;if(!ft->font.raster(raster,code,size,scale,error))return false;
                it=ft->glyphs.emplace(key,Face::Glyph{std::move(raster),{}}).first;
            }
            const auto& raster=it->second.raster;auto pin=it->second.image.lock();
            if(!pin){
                SwfTexture texture;
                if(!need(bool(renderer.image),"alpha texture upload",error)||!renderer.image(renderer.context,raster.width,raster.height,1,raster.alpha.data(),raster.width,texture,error))return false;
                if(!need(texture.width==int(raster.width)&&texture.height==int(raster.height),"unchanged texture dimensions",error))return false;
                pin=image(texture,platform,ft,error);if(!pin)return false;it->second.image=pin;
            }
            out.image=pin;
            out.advance=raster.advance;out.x0=raster.bounds[0];out.x1=raster.bounds[1];out.y0=raster.bounds[2];out.y1=raster.bounds[3];
            if(f->define_font3)out.advance*=20;found=true;return true;
        }
        // A source-cloned font has no embedded code/advance tables.
        if(cloned(*f))return true;
        TextEmbeddedGlyphV2 embedded;
        if(!need(bool(backends.embedded_glyph),"embedded glyph lookup",error)||!backends.embedded_glyph(*f,code,embedded,error))return false;
        if(embedded.found){
            if(!need(embedded.index>=0,"valid embedded index",error))return false;
            out.index=embedded.index;if(embedded.has_advance)out.advance=embedded.advance;
            else if(f->define_font3)out.advance*=20;found=true;
        }
        return true;
    }
    bool clone(const std::shared_ptr<text_v1::Font>& f,std::shared_ptr<text_v1::Font>& out,std::string& error){
        error.clear();
        if(!need(bool(f),"font clone source",error))return false;
        auto next=std::make_shared<text_v1::Font>(*f);
        next->native=std::make_shared<DeviceFontDescriptor>(DeviceFontDescriptor{f->name,f->bold,f->italic,f->metric58,f->metric5c});
        next->define_font3=false;
        for(auto it=clones.begin();it!=clones.end();)if(it->first.expired())it=clones.erase(it);else++it;
        clones.emplace(std::weak_ptr<void>(next->native),true);out=std::move(next);return true;
    }
    bool emit(const SwfDraw& draw,std::string& error){return need(bool(platform)&&bool(renderer.draw),"owned draw sink",error)&&renderer.draw(renderer.context,draw,error);}
};
TextRenderOwnerV2::TextRenderOwnerV2(SwfFontServices f,SwfServices r,std::shared_ptr<void> p,TextFontBackendsV2 b,float z)
    :impl_(std::make_shared<Impl>(f,std::move(r),std::move(p),std::move(b),z)){}
std::shared_ptr<void> TextRenderOwnerV2::bitmap(SwfTexture t,std::shared_ptr<void> owner,std::shared_ptr<void> face,std::string& e){return impl_->image(t,std::move(owner),std::move(face),e);}
bool TextRenderOwnerV2::units(const std::shared_ptr<text_v1::Font>& f,float& out,std::string& e){return impl_->metrics(f,out,false,e);}
bool TextRenderOwnerV2::height(const std::shared_ptr<text_v1::Font>& f,float& out,std::string& e){return impl_->metrics(f,out,true,e);}
bool TextRenderOwnerV2::glyph(const std::shared_ptr<text_v1::Font>& f,std::uint16_t code,int size,text_v1::Glyph& out,bool& found,std::string& e){return impl_->glyph(f,code,size,out,found,e);}
bool TextRenderOwnerV2::clone(const std::shared_ptr<text_v1::Font>& f,std::shared_ptr<text_v1::Font>& out,std::string& e){return impl_->clone(f,out,e);}
bool TextRenderOwnerV2::clear_fonts_v119(std::string& error){
 if(impl_->backends.clear_fonts&&!impl_->backends.clear_fonts(error))return false;
 impl_->faces.clear();impl_->images.clear();impl_->clones.clear();error.clear();return true;
}
text_v1::Services TextRenderOwnerV2::layout_services(text_v1::Services s) const {
    auto p=impl_;s.units_per_em=[p](auto& f,float& out,std::string& e){return p->metrics(f,out,false,e);};
    s.font_height=[p](auto& f,float& out,std::string& e){return p->metrics(f,out,true,e);};
    s.glyph=[p](auto& f,auto code,auto size,auto& out,bool& found,std::string& e){return p->glyph(f,code,size,out,found,e);};
    s.clone_font=[p](auto& f,auto& out,std::string& e){return p->clone(f,out,e);};
    auto kerning=s.kerning;s.kerning=[p,kerning](auto& f,auto first,auto second,float& out,std::string& e){
        if(f && p->cloned(*f)){out=0;return true;}
        return need(bool(kerning),"embedded kerning producer",e)&&kerning(f,first,second,out,e);
    };return s;
}
text_display_v2::Services TextRenderOwnerV2::display_services(text_display_v2::Services s) const {
    using namespace text_display_v2;auto p=impl_;
    s.bind_glyph=[p](const auto& g,GlyphBinding& out,std::string& e){auto pin=p->lookup(g.image);if(!need(bool(pin),"registered owned glyph texture",e))return false;
        out.bitmap={g.image,pin->texture.width,pin->texture.height};out.face=pin->face;return true;};
    s.line=[p](const Matrix& m,unsigned rgba,const float* xy,std::size_t n,std::string& e){
        SwfDraw d;d.kind=SwfDraw::line_strip;std::copy(m.begin(),m.end(),d.matrix.value);d.line.kind=SwfFill::color;color(d.line.rgba,rgba);
        d.xy.assign(xy,xy+n*2);return p->emit(d,e);};
    s.bitmap=[p](const Matrix& m,const Bitmap& b,const Rect& r,const Rect& uv,unsigned rgba,std::string& e){
        auto pin=p->lookup(b.owner);if(!need(bool(pin),"registered bitmap draw owner",e))return false;
        SwfDraw d;d.kind=SwfDraw::bitmap_quad;std::copy(m.begin(),m.end(),d.matrix.value);d.fill.kind=SwfFill::bitmap;d.fill.texture=pin->texture;
        color(d.fill.rgba,rgba);std::copy(r.begin(),r.end(),d.rect);std::copy(uv.begin(),uv.end(),d.uv_rect);return p->emit(d,e);};return s;
}
}
