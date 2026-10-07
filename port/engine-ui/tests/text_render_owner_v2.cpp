#include "../text_render_owner_v2.hpp"
#include "../hud_freetype_font_v2.hpp"
#include "../hud_freetype_font.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <stdexcept>
using namespace dh2::ui;
static void check(bool b,const std::string& s){if(!b)throw std::runtime_error(s);}
static std::vector<std::uint8_t> bytes(const char* p){std::ifstream f(p,std::ios::binary);check(bool(f),"missing actual font");return {std::istreambuf_iterator<char>(f),{}};}
struct Platform {
    std::map<std::uintptr_t,std::vector<std::uint8_t>> alpha;
    std::vector<SwfDraw> draws;unsigned reads{},uploads{},bitmap_lookups{},embedded_lookups{};bool bitmap_found{};
    std::map<std::string,std::vector<std::uint8_t>> fonts;
    static bool read(void* context,const char* name,bool,bool,std::vector<std::uint8_t>& out,std::string& error){
        auto& p=*static_cast<Platform*>(context);++p.reads;auto it=p.fonts.find(name);error.clear();if(it==p.fonts.end())return false;out=it->second;return true;
    }
    static bool image(void* context,int w,int h,unsigned channels,const unsigned char* pixels,int pitch,SwfTexture& out,std::string& error){
        auto& p=*static_cast<Platform*>(context);check(channels==1&&pitch==w,"source alpha upload changed");check(w>0&&h>0,"malformed source upload");
        out={++p.uploads,w,h};p.alpha.emplace(out.identity,std::vector<std::uint8_t>(pixels,pixels+std::size_t(w)*h));error.clear();return true;
    }
    static bool draw(void* context,const SwfDraw& draw,std::string& error){
        auto& p=*static_cast<Platform*>(context);check(draw.kind==SwfDraw::line_strip||draw.kind==SwfDraw::bitmap_quad,"unexpected draw producer");
        if(draw.kind==SwfDraw::bitmap_quad)check(p.alpha.count(draw.fill.texture.identity),"unowned GPU texture");
        p.draws.push_back(draw);error.clear();return true;
    }
};
int main(int argc,char** argv){try {
    check(argc==3,"usage: text_render_owner_v2 Fontin.ttf wqy.ttf");
    auto p=std::make_shared<Platform>();std::weak_ptr<Platform> lifetime=p;
    p->fonts.emplace("Fontin",bytes(argv[1]));p->fonts.emplace("PLAYLIST",bytes(argv[2]));
    SwfFontServices font_services{p.get(),Platform::read,nullptr};SwfServices renderer;renderer.context=p.get();renderer.image=Platform::image;renderer.draw=Platform::draw;
    TextFontBackendsV2 b;
    b.bitmap_face=[weak=std::weak_ptr<Platform>(p)](const auto&,TextBitmapFaceV2& out,std::string&){auto p=weak.lock();check(bool(p),"bitmap producer lost platform");++p->bitmap_lookups;if(p->bitmap_found){out.owner=p;out.height=12.5f;}return true;};
    b.embedded_glyph=[weak=std::weak_ptr<Platform>(p)](const auto&,auto,TextEmbeddedGlyphV2& out,std::string&){auto p=weak.lock();check(bool(p),"embedded producer lost platform");++p->embedded_lookups;out={true,2,true,123.5f};return true;};
    unsigned metrics{},raster{},layouts{},draws{},guards{};
    {
        TextRenderOwnerV2 owner(font_services,renderer,p,b,1);
        for(const auto& entry:p->fonts){
            auto f=std::make_shared<text_v1::Font>();f->native=std::make_shared<std::string>(entry.first);f->name=entry.first;f->metric58=123;f->metric5c=17;
            FT_Library lib{};FT_Face actual{};check(!FT_Init_FreeType(&lib)&&!FT_New_Memory_Face(lib,entry.second.data(),entry.second.size(),0,&actual),"reference FreeType face rejected original bytes");
            float units{},height{};std::string error;
            check(owner.units(f,units,error)&&owner.height(f,height,error),error);
            check(units==actual->units_per_EM&&height==int(actual->ascender)-int(actual->descender),"layout metrics differ from the actual raster face");metrics+=2;
            HudFreetypeFont reference;check(reference.load(entry.second.data(),entry.second.size(),error),error);
            for(int size:{12,16,24})for(unsigned code:{32,65,80,86,103,937,0x65e5}){
                text_v1::Glyph a;bool found{};check(owner.glyph(f,code,size,a,found,error)&&found&&a.image,error);
                FreetypeGlyph expected;check(reference.raster(expected,code,size,1,error),error);
                auto services=owner.display_services({});text_display_v2::GlyphBinding binding;check(services.bind_glyph(a,binding,error),error);
                check(binding.bitmap.width==int(expected.width)&&binding.bitmap.height==int(expected.height)&&bool(binding.face),"glyph metrics/face/upload did not share an owner");
                check(a.advance==expected.advance&&a.x0==expected.bounds[0]&&a.x1==expected.bounds[1]&&a.y0==expected.bounds[2]&&a.y1==expected.bounds[3],"original raster layout changed");
                // A second lookup while this image is retained must reuse the
                // same actual texture and face instead of uploading another.
                auto uploads=p->uploads;text_v1::Glyph again;check(owner.glyph(f,code,size,again,found,error)&&again.image==a.image&&p->uploads==uploads,"retained glyph cache identity changed");
                ++raster;
            }
            FT_Done_Face(actual);FT_Done_FreeType(lib);
            for(bool html:{false,true}){
                text_v1::State state;state.font=f;state.text=html?"<p>A<font face='PLAYLIST' size='16'><u>PV</u></font></p>":"AV P\ntext";
                state.rect_max=6000;state.text_height=320;
                text_v1::Services seed;seed.root_scale_word=[](float& out,std::string&){out=1;return true;};
                seed.kerning=[](const auto&,int a,int b,float& out,std::string&){out=a==65&&b==86?-16:0;return true;};
                seed.preload_enabled=[](bool& out,std::string&){out=false;return true;};seed.diagnostic_state=std::make_shared<text_v1::DiagnosticState>();
                seed.missing_glyph=[](const auto&,int,std::string&){return true;};
                auto layout=owner.layout_services(std::move(seed));check(text_v1::format_text(state,html,layout,error),error);++layouts;
                text_display_v2::Services display_seed;display_seed.resolve_font=[](auto&,std::size_t,std::string&){return true;};
                display_seed.transform_color=[](auto in,auto& out,std::string&){out=in;return true;};
                auto display=owner.display_services(std::move(display_seed));auto before=p->draws.size();
                check(text_display_v2::display(state,{},display,error),error);check(p->draws.size()>before,"whole laid-out field produced no draw commands");draws+=p->draws.size()-before;
                for(auto i=before;i<p->draws.size();++i)check(p->draws[i].color_transform.value[0]==1&&p->draws[i].color_transform.value[1]==0,"already transformed glyph color would be applied twice");
            }
        }
        auto missing=std::make_shared<text_v1::Font>();missing->native=std::make_shared<int>(1);missing->name="genuine resolver miss";missing->define_font3=true;
        std::string error;float units{},height{};check(owner.units(missing,units,error)&&units==1&&owner.height(missing,height,error)&&height==0,"genuine FT font miss defaults changed");++guards;
        text_v1::Glyph embedded;bool found{};check(owner.glyph(missing,65,12,embedded,found,error)&&found&&embedded.index==2&&embedded.advance==123.5f,"genuine embedded advance was multiplied as a device glyph");++guards;
        std::shared_ptr<text_v1::Font> clone;check(owner.clone(missing,clone,error)&&clone->native!=missing->native&&!clone->define_font3,"clone retained original embedded/Font3 owner");
        auto embedded_count=p->embedded_lookups;check(owner.glyph(clone,65,12,embedded,found,error)&&!found&&p->embedded_lookups==embedded_count,"cloned font inherited embedded table");++guards;
        auto layout=owner.layout_services({});float kerning=123;check(layout.kerning(clone,65,86,kerning,error)&&kerning==0,"cloned font inherited embedded kerning");++guards;
        p->bitmap_found=true;auto reads=p->reads;check(owner.units(missing,units,error)&&units==1024&&owner.height(missing,height,error)&&height==250&&p->reads==reads,"bitmap-first metric producer order changed");++guards;p->bitmap_found=false;
        // Alien void owners must fail rather than be cast to native textures.
        auto display=owner.display_services({});text_display_v2::GlyphBinding binding;embedded.image=std::make_shared<int>(9);
        check(!display.bind_glyph(embedded,binding,error),"unregistered image pointer accepted");++guards;
        check(!owner.bitmap({9,0,1},p,{},error),"invalid real texture dimensions accepted");++guards;
        // An exported texture pin survives the facade and keeps platform and
        // glyph face alive; cache weak refs do not form face/image cycles.
        auto font=std::make_shared<text_v1::Font>();font->name="Fontin";font->native=std::make_shared<int>(2);
        text_v1::Glyph live;check(owner.glyph(font,65,16,live,found,error),error);
        auto held=live.image;live.image.reset();auto callbacks=owner.display_services({});
        check(bool(held),"missing final texture owner");++guards;
    }
    p.reset();check(lifetime.expired(),"font/texture/platform ownership cycle survived cache teardown");++guards;
    std::cout<<"{\"validation\":\"PASS\",\"actual_font_metrics\":"<<metrics<<",\"actual_raster_owner_cases\":"<<raster<<",\"whole_layout_display_compositions\":"<<layouts<<",\"native_draw_commands\":"<<draws<<",\"ownership_and_source_guards\":"<<guards<<"}\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
