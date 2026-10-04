#include "swf_hud_freetype_provider.hpp"
#include "hud_freetype_font.hpp"
#include "gameswf/gameswf.h"
#include "gameswf/gameswf_render.h"
#include <cmath>
#include <map>
#include <tuple>
#include <utility>
#ifdef isfinite
#undef isfinite
#endif
namespace dh2::ui {
namespace {
struct NativeProvider final:gameswf::glyph_provider {
    struct CachedGlyph { gameswf::gc_ptr<gameswf::bitmap_info> bitmap;FreetypeGlyph raster; };
    struct Face { HudFreetypeFont font;std::map<std::uint32_t,CachedGlyph> glyphs; };
    SwfFontServices services;float scale;std::string error;HudBitmapProbe probe;
    std::map<std::tuple<std::string,bool,bool>,std::unique_ptr<Face>> faces;
    NativeProvider(const SwfFontServices& s,float z,HudBitmapProbe p):services(s),scale(z),probe(p){}
    gameswf::bitmap_info* failed(const char* fallback) {
        if(error.empty())error=fallback;
        if(services.diagnostic)services.diagnostic(services.context,error.c_str());
        return nullptr;
    }
    gameswf::bitmap_info* get_char_image(gameswf::character_def*,Uint16 code,
        const tu_string& name,bool bold,bool italic,int size,gameswf::rect* bounds,float* advance) override {
        error.clear();
        if(!bounds || !advance || !services.read || size<=0 || size>65535 || !std::isfinite(scale) || scale<0)
            return failed("missing or malformed genuine font provider service");
        const auto key=std::make_tuple(std::string(name.c_str(),name.size()),bold,italic);
        auto found=faces.find(key);
        if(found==faces.end()) {
            std::vector<std::uint8_t> bytes;
            if(!services.read(services.context,name.c_str(),bold,italic,bytes,error)) {
                faces.emplace(key,nullptr);return failed("font resolver miss");
            }
            auto face=std::make_unique<Face>();
            if(!face->font.load(bytes.data(),bytes.size(),error))return failed("invalid font resource");
            found=faces.emplace(key,std::move(face)).first;
        }
        if(!found->second) { error="cached font resolver miss";return nullptr; }
        const std::uint32_t glyph_key=static_cast<std::uint32_t>(code)|(static_cast<std::uint32_t>(size)<<16);
        auto& face=*found->second;auto cached=face.glyphs.find(glyph_key);
        if(cached==face.glyphs.end()) {
            CachedGlyph value;
            HudBitmapInfo32 info{};
            if(!face.font.raster(value.raster,code,size,scale,error,&info))return failed("glyph raster failed");
            if(probe)probe(services.context,info);
            value.bitmap=gameswf::render::create_bitmap_info_alpha(static_cast<int>(value.raster.width),
                static_cast<int>(value.raster.height),value.raster.alpha.data());
            if(value.bitmap==nullptr)return failed("genuine alpha bitmap upload unavailable");
            if(value.bitmap->get_width()!=static_cast<int>(value.raster.width) ||
               value.bitmap->get_height()!=static_cast<int>(value.raster.height))return failed("alpha upload changed source image dimensions");
            cached=face.glyphs.emplace(glyph_key,std::move(value)).first;
        }
        const auto& raster=cached->second.raster;
        bounds->m_x_min=raster.bounds[0];bounds->m_x_max=raster.bounds[1];
        bounds->m_y_min=raster.bounds[2];bounds->m_y_max=raster.bounds[3];*advance=raster.advance;
        return cached->second.bitmap.get_ptr();
    }
};
}
struct SwfHudFreetypeProvider::Impl { gameswf::gc_ptr<NativeProvider> provider; };
SwfHudFreetypeProvider::SwfHudFreetypeProvider(const SwfFontServices& services,float scale,HudBitmapProbe probe):impl_(std::make_unique<Impl>()) {
    impl_->provider=new NativeProvider(services,scale,probe);
}
SwfHudFreetypeProvider::~SwfHudFreetypeProvider()=default;
gameswf::glyph_provider* SwfHudFreetypeProvider::borrowed_provider() const { return impl_->provider.get_ptr(); }
std::string SwfHudFreetypeProvider::error() const { return impl_->provider->error; }
}
