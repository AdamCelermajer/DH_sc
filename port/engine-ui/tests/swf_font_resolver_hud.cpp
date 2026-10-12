#include "../swf_font_resolver.hpp"
#include "../swf_movie.hpp"
#include "../swf_freetype_provider.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>
using namespace dh2::ui;
static void check(bool ok,const char* e){if(!ok)throw std::runtime_error(e);}
struct Test {
    std::string swfs,fonts;std::vector<std::vector<std::uint8_t>> images;
    unsigned alpha{},nonempty{},quads{},reads{},opens{},closes{};std::map<std::string,std::string> resolved;
    static int resolver(void* p,FontResolveRequest40* r){auto& t=*static_cast<Test*>(p);
        switch(r->kind){
        case FontResolveService::debug_load:case FontResolveService::debug_get_switch:return 0; // controlled Debug services
        case FontResolveService::language:r->value=0;return 0; // explicit non-Asian language fixture
        case FontResolveService::rewrite_path:{std::string name=r->text;const std::string prefix="cache/data/";if(name.rfind(prefix,0)!=0)return -1;name=t.fonts+"/"+name.substr(prefix.size());if(name.size()>=r->capacity)return -1;name.copy(r->buffer,name.size());r->buffer[name.size()]=0;return 0;}
        case FontResolveService::open_read:r->value=reinterpret_cast<std::uintptr_t>(std::fopen(r->text,"rb"));++t.opens;return 0;
        case FontResolveService::close_read:++t.closes;return std::fclose(reinterpret_cast<FILE*>(r->value));
        }return -1;
    }
    static bool font_read(void* p,const char* name,bool bold,bool italic,std::vector<std::uint8_t>& out,std::string& e){auto& t=*static_cast<Test*>(p);++t.reads;char path[4096];FontResolveOutput32 result{path,sizeof path,0,0,0};FontResolveInput24 input{name,"cache/",bold,italic};FontResolveServices16 services{p,resolver};
        if(dh2_swf_font_resolve(&result,&input,&services)!=0||!result.found){e="unavailable actual resolved font";return false;}t.resolved[name]=path;std::ifstream f(path,std::ios::binary);if(!f){e="resolved file vanished";return false;}out.assign(std::istreambuf_iterator<char>(f),{});return true;}
    static bool read(void* p,const char* uri,std::vector<std::uint8_t>& out,std::string& e){auto& t=*static_cast<Test*>(p);std::string s=uri;s=s.substr(s.find_last_of('/')+1);std::ifstream f(t.swfs+"/"+s,std::ios::binary);if(!f){e="missing actual SWF";return false;}out.assign(std::istreambuf_iterator<char>(f),{});return true;}
    static bool texture(void*,const char*,int w,int h,SwfTexture& out,std::string&){out={123,w?w:1024,h?h:1024};return true;}
    static bool image(void* p,int w,int h,unsigned channels,const std::uint8_t* pixels,int pitch,SwfTexture& out,std::string&){auto& t=*static_cast<Test*>(p);check(w>0&&h>0&&pixels&&pitch>=w*int(channels),"malformed actual upload");std::vector<std::uint8_t> copy;for(int y=0;y<h;++y)copy.insert(copy.end(),pixels+y*pitch,pixels+y*pitch+w*channels);if(channels==1){++t.alpha;for(auto x:copy)if(x){++t.nonempty;break;}}t.images.push_back(std::move(copy));out={1000+t.images.size(),w,h};return true;}
    static bool draw(void* p,const SwfDraw& d,std::string&){if(d.kind==SwfDraw::bitmap_quad)++static_cast<Test*>(p)->quads;return true;}
    static bool native(void*,const char* name,const std::vector<SwfValue>& a,SwfValue& out,std::string& e){if(std::string(name)!="NativeGetStringFromSymbol"){e="unavailable game function";return false;}out.kind=SwfValue::text;out.string=a.empty()?"":a[0].string;return true;}
    static bool stencil(void*,const float*,std::uint8_t,bool& out,std::string&){out=false;return true;}
};
int main(int argc,char**argv){try{if(argc!=3)return 2;Test t;t.swfs=argv[1];t.fonts=argv[2];std::string e;SwfFontServices fs{&t,Test::font_read,nullptr};SwfFreetypeProvider provider(fs);
    {SwfMovie movie;SwfServices s;s.context=&t;s.read=Test::read;s.texture=Test::texture;s.image=Test::image;s.draw=Test::draw;s.native_call=Test::native;s.stencil=Test::stencil;s.glyphs=provider.borrowed_provider();
     check(movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",s,e),e.c_str());check(movie.advance(0,e),e.c_str());check(movie.display(0,0,480,320,e),e.c_str());
     check(t.resolved.count("Fontin SmallCaps")==1&&t.resolved.count("Arial")==1,"source HUD font requests missing");check(t.resolved.at("Arial")==t.fonts+"/wqy-zenhei.ttf","Arial did not select actual original font");check(t.opens==t.closes&&t.opens==t.reads,"genuine file ownership mismatch");check(t.alpha>238&&t.nonempty>232&&t.quads>0,"Arial failed to produce real glyph images");
    }
    std::cout<<"{\"validation\":\"PASS\",\"font_reads\":"<<t.reads<<",\"genuine_file_opens\":"<<t.opens<<",\"genuine_file_closes\":"<<t.closes<<",\"resolved_fonts\":[\"Fontin SmallCaps\",\"Arial\"],\"alpha_uploads\":"<<t.alpha<<",\"nonempty_alpha_uploads\":"<<t.nonempty<<",\"bitmap_quads\":"<<t.quads<<"}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
