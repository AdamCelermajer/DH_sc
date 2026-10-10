#include "hud_glyphs.hpp"
#include "../engine-ui/hud_freetype_font.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <limits>
#include <utility>

namespace dh::foundation {
namespace {
bool decode_utf8(const std::string& text, std::vector<std::uint32_t>& codes, std::string& error) {
    if (text.size() > 4096) { error = "HUD text exceeds4096 UTF8 bytes"; return false; }
    for (std::size_t at=0; at<text.size();) {
        const auto first=static_cast<unsigned char>(text[at++]);
        unsigned count=0;
        std::uint32_t code=first, minimum=0;
        if (first>=0xc2 && first<=0xdf) {count=1;code=first&31;minimum=0x80;}
        else if(first>=0xe0 && first<=0xef) {count=2;code=first&15;minimum=0x800;}
        else if(first>=0x80) {error="Unsupported or malformed source UTF8 codepoint";return false;}
        if(count>text.size()-at) {error="Truncated HUD UTF8";return false;}
        while(count--) {
            const auto next=static_cast<unsigned char>(text[at++]);
            if((next&0xc0)!=0x80) {error="Malformed HUD UTF8 continuation";return false;}
            code=(code<<6)|(next&63);
        }
        if(code<minimum || (code>=0xd800&&code<=0xdfff) || code<32 || code==127) {
            error="HUD text requires supported printable single-line BMP characters";return false;
        }
        codes.push_back(code);
        if(codes.size()>512) {error="HUD text exceeds512 source characters";return false;}
    }
    return true;
}
}
struct HudGlyphFont::Impl {dh2::ui::HudFreetypeFont font;};
HudGlyphFont::HudGlyphFont()=default;
HudGlyphFont::~HudGlyphFont()=default;
const char* HudGlyphFont::version(){return dh2::ui::HudFreetypeFont::version();}

bool HudGlyphFont::load(const std::filesystem::path& path,std::string& error) {
    try {
        std::ifstream stream(path,std::ios::binary|std::ios::ate);
        if(!stream) {error="Original HUD font file unavailable: "+path.string();return false;}
        const auto size=stream.tellg();
        if(size<=0 || size>64*1024*1024) {error="Original font file size outside source budget";return false;}
        std::vector<std::uint8_t> bytes(static_cast<std::size_t>(size));stream.seekg(0);
        if(!stream.read(reinterpret_cast<char*>(bytes.data()),static_cast<std::streamsize>(bytes.size()))) {
            error="Original font file could not be read completely";return false;
        }
        auto loaded=std::make_unique<Impl>();
        if(!loaded->font.load(bytes.data(),bytes.size(),error))return false;
        impl_=std::move(loaded);error.clear();return true;
    } catch(const std::exception& failure) {error=failure.what();return false;}
}

bool HudGlyphFont::raster(const std::string& text,int size,float scale,HudGlyphRun& output,std::string& error) {
    if(!impl_ || size<=0 || size>65535 || !std::isfinite(scale) || scale<=0 ||
       !std::isfinite(size*scale) || size*scale<1 || size*scale>8192) {
        error="Original HUD glyph font or raster scale unavailable";return false;
    }
    try {
        std::vector<std::uint32_t> codes;
        if(!decode_utf8(text,codes,error))return false;
        HudGlyphRun run;
        bool have_bounds=false;
        std::uint64_t total_pixels=0;
        for(auto code:codes) {
            dh2::ui::FreetypeGlyph source;
            if(!impl_->font.raster(source,code,size,scale,error))return false;
            total_pixels+=std::uint64_t(source.width)*source.height;
            if(total_pixels>16*1024*1024) {error="HUD glyph run exceeds decoded pixel budget";return false;}
            HudGlyphQuad quad;quad.codepoint=code;
            quad.width=source.bitmap_width/scale;quad.height=source.bitmap_height/scale;
            quad.u1=source.bounds[1];quad.v1=source.bounds[3];
            const float bearing_x=quad.u1 ? -source.bounds[0]/quad.u1*source.bitmap_width : 0;
            const float bearing_y=quad.v1 ? source.bounds[2]/quad.v1*source.bitmap_height : 0;
            quad.x=run.advance+bearing_x/scale;quad.y=-bearing_y/scale;
            quad.image.width=source.width;quad.image.height=source.height;
            quad.image.rgba.resize(std::size_t(source.width)*source.height*4);
            for(std::size_t i=0;i<source.alpha.size();++i) {
                auto* pixel=quad.image.rgba.data()+i*4;
                pixel[0]=pixel[1]=pixel[2]=255;pixel[3]=source.alpha[i];
            }
            if(quad.width>0 && quad.height>0) {
                if(!have_bounds) {run.bounds={quad.x,quad.x+quad.width,quad.y,quad.y+quad.height};have_bounds=true;}
                else {run.bounds[0]=std::min(run.bounds[0],quad.x);run.bounds[1]=std::max(run.bounds[1],quad.x+quad.width);
                      run.bounds[2]=std::min(run.bounds[2],quad.y);run.bounds[3]=std::max(run.bounds[3],quad.y+quad.height);}
            }
            quad.advance=source.advance*size/(1024.f*scale);
            run.advance+=quad.advance;
            if(!std::isfinite(run.advance) || run.advance>65536) {error="HUD glyph run advance outside source budget";return false;}
            run.glyphs.push_back(std::move(quad));
        }
        output=std::move(run);error.clear();return true;
    } catch(const std::exception& failure) {error=failure.what();return false;}
}
} // namespace dh::foundation
