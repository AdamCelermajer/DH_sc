#include "../freetype_font.hpp"
#include "../swf_freetype_provider.hpp"
#include "../swf_font_geometry.hpp"
#include "../swf_movie.hpp"
#include "../freetype_glyph_kernel.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <stdexcept>
#include <string>
using namespace dh2::ui;
static void check(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
static std::vector<std::uint8_t> bytes(const std::string& path){std::ifstream f(path,std::ios::binary);check(bool(f),"missing real resource");return {std::istreambuf_iterator<char>(f),{}};}
struct Test {
    std::string swfs,font;std::vector<std::vector<std::uint8_t>> images;
    unsigned alpha_images{},alpha_nonempty{},quads{},reads{},font_misses{},diagnostics{};
    std::map<std::string,unsigned> unresolved;
    static bool read(void* p,const char* uri,std::vector<std::uint8_t>& out,std::string& e){auto& t=*static_cast<Test*>(p);std::string name=uri;auto slash=name.find_last_of('/');if(slash!=std::string::npos)name=name.substr(slash+1);std::ifstream f(t.swfs+"/"+name,std::ios::binary);if(!f){e="unavailable source SWF: "+name;return false;}out.assign(std::istreambuf_iterator<char>(f),{});return true;}
    static bool font_read(void* p,const char* name,bool,bool,std::vector<std::uint8_t>& out,std::string& e){auto& t=*static_cast<Test*>(p);++t.reads;if(std::string(name)!="Fontin SmallCaps"){++t.font_misses;if(++t.unresolved[name]==1)std::cerr<<"Unresolved genuine font: "<<name<<'\n';e="unresolved genuine font: "+std::string(name);return false;}out=bytes(t.font);return true;}
    static bool texture(void*,const char*,int w,int h,SwfTexture& out,std::string&){out={123, w?w:1024,h?h:1024};return true;}
    static bool image(void* p,int w,int h,unsigned channels,const std::uint8_t* data,int pitch,SwfTexture& out,std::string&){auto& t=*static_cast<Test*>(p);check(w>0&&h>0&&data&&pitch>=w*int(channels),"invalid core image");std::vector<std::uint8_t> owned;for(int y=0;y<h;++y)owned.insert(owned.end(),data+y*pitch,data+y*pitch+w*channels);if(channels==1){++t.alpha_images;check(pitch==w && w>=4 && (w&(w-1))==0 && (h&(h-1))==0,"source alpha padding");if(std::any_of(owned.begin(),owned.end(),[](auto v){return v!=0;}))++t.alpha_nonempty;}t.images.push_back(std::move(owned));out={1000+t.images.size(),w,h};return true;}
    static bool draw(void* p,const SwfDraw& d,std::string&){if(d.kind==SwfDraw::bitmap_quad)++static_cast<Test*>(p)->quads;return true;}
    static bool native(void*,const char* name,const std::vector<SwfValue>& a,SwfValue& out,std::string& e){if(std::string(name)!="NativeGetStringFromSymbol"){e="native game function unavailable";return false;}out.kind=SwfValue::text;out.string=a.empty()?"":a[0].string;return true;}
    static bool stencil(void*,const float*,std::uint8_t,bool& out,std::string&){out=false;return true;}
    static void diagnostic(void* p,bool,const char*){++static_cast<Test*>(p)->diagnostics;}
};
int main(int argc,char**argv){try{
    if(argc!=4)return 2;Test t;t.swfs=argv[1];t.font=argv[2];unsigned checks=0,rasters=0,guards=0;
    FreetypeFont font;auto resource=bytes(t.font);std::string error;check(font.load(resource.data(),resource.size(),error),error.c_str());++checks;
    auto native_bytes=resource;FT_Library library{};FT_Face face{};check(FT_Init_FreeType(&library)==0,"reference FT initialization");check(FT_New_Memory_Face(library,native_bytes.data(),native_bytes.size(),0,&face)==0,"reference FT face");
    std::ofstream corpus(std::string(argv[3])+".bin",std::ios::binary);const std::uint32_t header[]={0x31474646,570};corpus.write(reinterpret_cast<const char*>(header),sizeof header);
    resource.assign(resource.size(),0); // FT_Face must retain owned original bytes.
    FreetypeGlyph reference;check(font.raster(reference,'A',24,1,error),error.c_str());++checks;
    for(int size:{8,12,16,24,32,48})for(unsigned code=32;code<127;++code){FreetypeGlyph g;check(font.raster(g,code,size,1,error),error.c_str());check(g.alpha.size()==std::size_t(g.width)*g.height,"owned alpha shape");check(g.pixel_height==size,"source pixel size");if(code!=' ')check(std::any_of(g.alpha.begin(),g.alpha.end(),[](auto v){return v!=0;}),"nonempty authored glyph");++rasters;checks+=4;
        check(FT_Set_Pixel_Sizes(face,0,size)==0 && FT_Load_Char(face,code,4)==0,"reference glyph source");const auto& m=face->glyph->metrics;const auto& b=face->glyph->bitmap;
        const FreetypeMetrics32 metrics{int(m.width),int(m.height),int(m.horiBearingX),int(m.horiBearingY),int(m.horiAdvance),b.width,b.rows,b.pitch};const std::uint32_t request[]={code,unsigned(size),0x3f800000};
        FreetypeLayout40 layout{g.width,g.height,{g.bounds[0],g.bounds[1],g.bounds[2],g.bounds[3]},g.advance,g.pixel_height,g.bitmap_width,g.bitmap_height};const std::uint32_t n=b.rows*b.pitch;
        corpus.write(reinterpret_cast<const char*>(request),sizeof request);corpus.write(reinterpret_cast<const char*>(&metrics),sizeof metrics);corpus.write(reinterpret_cast<const char*>(&layout),sizeof layout);corpus.write(reinterpret_cast<const char*>(&n),4);if(n)corpus.write(reinterpret_cast<const char*>(b.buffer),n);
    }
    corpus.close();FT_Done_Face(face);FT_Done_FreeType(library);
    FreetypeGlyph scaled;check(font.raster(scaled,'A',24,.5f,error),error.c_str());check(scaled.pixel_height==12&&scaled.alpha!=reference.alpha,"real scale changes raster");checks+=2;
    FreetypeGlyph saved=reference;for(auto code:{65536u,0xffffffffu}){check(!font.raster(saved,code,24,1,error),"malformed code accepted");check(saved.alpha==reference.alpha,"atomic raster failure");++guards;}
    for(auto size:{0,-1}){check(!font.raster(saved,'A',size,1,error),"malformed size accepted");check(saved.alpha==reference.alpha,"atomic size failure");++guards;}
    const std::uint8_t bad[]={1,2,3};check(!font.load(bad,sizeof bad,error),"invalid font accepted");check(font.raster(saved,'A',24,1,error)&&saved.alpha==reference.alpha,"failed replacement changed font");++guards;
    std::ofstream pgm(argv[3],std::ios::binary);pgm<<"P5\n"<<reference.width<<' '<<reference.height<<"\n255\n";pgm.write(reinterpret_cast<const char*>(reference.alpha.data()),reference.alpha.size());pgm.close();
#ifndef DH2_FONT_RASTER_ONLY
    SwfFontServices fs{&t,Test::font_read,nullptr};SwfFreetypeProvider provider(fs);
    {SwfMovie movie;SwfServices s;s.context=&t;s.read=Test::read;s.texture=Test::texture;s.image=Test::image;s.draw=Test::draw;s.native_call=Test::native;s.stencil=Test::stencil;s.diagnostic=Test::diagnostic;s.glyphs=provider.borrowed_provider();
     check(movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",s,error),error.c_str());check(movie.advance(0,error),error.c_str());check(movie.display(0,0,480,320,error),error.c_str());checks+=3;
     auto* f=movie.borrowed_font(7);check(f!=nullptr,"actual HUD font resource missing");SwfGlyphOutline outline;check(swf_glyph_outline(outline,*f,0,error)==1,error.c_str());check(outline.font_name=="Fontin SmallCaps"&&outline.glyph_index==0,"actual embedded font identity");checks+=3;
     std::cout<<"{\"embedded_character\":"<<outline.character_code<<",\"embedded_paths\":"<<outline.paths.size()<<",";
     check(t.alpha_nonempty>0 && t.quads>0,"actual HUD did not request/draw glyph alpha");checks+=2;
    }
    // Unresolved authored/default font requests are recorded, never substituted.
#else
    std::cout<<"{";
#endif
    std::cout<<"\"validation\":\"PASS\",\"checks\":"<<checks<<",\"rasters\":"<<rasters<<",\"atomic_guards\":"<<guards<<",\"alpha_uploads\":"<<t.alpha_images<<",\"nonempty_alpha_uploads\":"<<t.alpha_nonempty<<",\"bitmap_quads\":"<<t.quads<<",\"font_reads\":"<<t.reads<<",\"font_misses\":"<<t.font_misses<<",\"core_diagnostics\":"<<t.diagnostics<<"}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
