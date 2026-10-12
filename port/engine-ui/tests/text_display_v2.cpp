#include "../text_display_v2.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::ui;
namespace d=text_display_v2;
using Bytes=std::vector<unsigned char>;
static void check(bool b,const std::string& e){if(!b)throw std::runtime_error(e);}
static unsigned bits(float f){unsigned w;std::memcpy(&w,&f,4);return w;}
static float real(unsigned w){float f;std::memcpy(&f,&w,4);return f;}
static void word(Bytes& b,unsigned w){for(int i=0;i<4;++i)b.push_back((w>>(8*i))&255);}
static void floats(Bytes& b,const float* f,std::size_t n){for(std::size_t i=0;i<n;++i)word(b,bits(f[i]));}
static unsigned read(std::ifstream& f){unsigned char b[4];check(bool(f.read(reinterpret_cast<char*>(b),4)),"truncated display corpus");return b[0]|(unsigned(b[1])<<8)|(unsigned(b[2])<<16)|(unsigned(b[3])<<24);}
int main(int argc,char** argv){try {
    check(argc==2,"usage: text_display_v2 corpus.bin");std::ifstream f(argv[1],std::ios::binary);
    check(bool(f)&&read(f)==0x32445354,"missing original display corpus");auto count=read(f);
    for(unsigned case_index=0;case_index<count;++case_index){
        unsigned cfg[13];for(auto& w:cfg)w=read(f);
        text_v1::State state;d::Context context;for(int i=0;i<6;++i)context.matrix[i]=real(cfg[i]);
        context.provider_scale=real(cfg[6]);context.pixel_scale=real(cfg[7]);context.renderer_present=cfg[9]&1;
        context.freetype_cache=cfg[9]&16;context.bitmap_cache=cfg[9]&32;
        if(cfg[9]&2)context.override_rgba=cfg[8];
        context.argument0=cfg[10];context.argument1=cfg[11];context.argument2=cfg[12];
        auto bitmap=std::make_shared<int>(1),other=std::make_shared<int>(2);context.freetype_atlas=bitmap;
        auto font=std::make_shared<text_v1::Font>();font->native=std::make_shared<int>(3);font->define_font3=(cfg[9]>>8)&1;
        auto n=read(f);check(n<100,"unbounded source records");
        for(unsigned i=0;i<n;++i){
            unsigned style[7];for(auto& w:style)w=read(f);auto flags=style[6];text_v1::Record r;
            r.rgba=style[0];r.underline=style[1];r.x=real(style[2]);r.y=real(style[3]);r.height=real(style[4]);
            r.has_x=flags&1;r.has_y=flags&2;r.has_font=flags&4;if(flags&8)r.font=font;
            auto gn=read(f);check(gn<100,"unbounded glyphs");for(unsigned j=0;j<gn;++j){
                unsigned g[8];for(auto& w:g)w=read(f);text_v1::Glyph glyph;glyph.advance=real(g[0]);
                if(g[1])glyph.image=g[1]==1?bitmap:other;
                glyph.x0=real(g[2]);glyph.x1=real(g[3]);glyph.y0=real(g[4]);glyph.y1=real(g[5]);
                glyph.height=g[6]&65535;glyph.index=g[6]>>16;glyph.code=g[7]&65535;glyph.type=g[7]>>16;
                r.glyphs.push_back(glyph);
            }state.records.push_back(r);
        }
        auto size=read(f);check(size<1000000,"unbounded source trace");Bytes expected(size);check(bool(f.read(reinterpret_cast<char*>(expected.data()),size)),"truncated render trace");
        Bytes events;d::Services s;std::size_t record{};
        s.resolve_font=[&](auto&,std::size_t i,std::string&){record=i;word(events,1);word(events,i);return true;};
        s.transform_color=[](unsigned in,unsigned& out,std::string&){out=in^0x00112233;return true;};
        s.bind_glyph=[&](const auto& g,d::GlyphBinding& out,std::string&){out.bitmap={g.image,g.image==bitmap?128:32,g.image==bitmap?64:16};return true;};
        s.line=[&](const d::Matrix& matrix,unsigned color,const float* points,std::size_t count,std::string&){word(events,2);word(events,color);word(events,count);floats(events,matrix.data(),6);floats(events,points,count*2);return true;};
        s.bitmap=[&](const d::Matrix& matrix,const d::Bitmap& b,const d::Rect& rect,const d::Rect& uv,unsigned color,std::string&){word(events,3);word(events,b.owner==bitmap?1:2);word(events,color);floats(events,matrix.data(),6);floats(events,rect.data(),4);floats(events,uv.data(),4);return true;};
        s.shape=[&](const d::Matrix& matrix,const auto&,short index,float pixel,unsigned color,std::string&){if(!(cfg[9]&8)){word(events,4);word(events,index);word(events,bits(pixel));word(events,color);floats(events,matrix.data(),6);}return true;};
        s.region=[&](bool ft,const auto& g,const auto&,d::Filter filter,d::Rect& out,std::string&){word(events,5);word(events,ft);word(events,g.code);word(events,std::uint16_t(g.height));for(unsigned b:{filter.kind,filter.x,filter.y})word(events,b);out=(filter.kind||filter.x||filter.y)?d::Rect{7,99,9,87}:d::Rect{3,43,5,25};return true;};
        std::string error;check(d::display(state,context,s,error),"case "+std::to_string(case_index)+" "+error);
        if(events!=expected){std::size_t p=0;while(p<events.size()&&p<expected.size()&&events[p]==expected[p])++p;
            std::cerr<<"case="<<case_index<<" byte="<<p<<" actual="<<events.size()<<" expected="<<expected.size()<<'\n';
            for(auto i=(p/4)*4;i+4<=std::min(p+32,std::min(events.size(),expected.size()));i+=4){unsigned a,b;std::memcpy(&a,events.data()+i,4);std::memcpy(&b,expected.data()+i,4);std::cerr<<i<<" "<<std::hex<<a<<" "<<b<<std::dec<<'\n';}
            throw std::runtime_error("whole original display mismatch");
        }
    }
    check(f.peek()==std::char_traits<char>::eof(),"trailing corpus");
    unsigned guards{};text_v1::State st;st.records.emplace_back();d::Context cx;d::Services s;std::string error;
    check(!d::display(st,cx,s,error)&&error.find("font resolution")!=std::string::npos,"missing font service accepted");++guards;
    cx.renderer_present=false;check(d::display(st,cx,s,error),"absent renderer invoked services");++guards;
    // Reentrant draw writes the next advance and appends another glyph. The
    // coordinator must use current storage and pin its bitmap/font owners.
    cx.renderer_present=true;st.records[0].font=std::make_shared<text_v1::Font>();st.records[0].height=1024;st.records[0].glyphs.emplace_back();
    s.resolve_font=[](auto&,auto,std::string&){return true;};s.transform_color=[](auto in,auto& out,std::string&){out=in;return true;};
    unsigned calls{};float second_x{};s.line=[&](const auto& matrix,auto,const float*,auto,std::string&){
        if(++calls==1){st.records[0].glyphs[0].advance=37.5f;st.records[0].glyphs.emplace_back();}
        else second_x=matrix[2];return true;};
    check(d::display(st,cx,s,error)&&calls==2&&second_x==37.5f,"callback mutation used stale advance/storage");++guards;
    s.line=[&](const auto&,auto,const float*,auto,std::string&){st.records.clear();return true;};
    check(!d::display(st,cx,s,error)&&error.find("stable current glyph")!=std::string::npos,"removed current source storage accepted");++guards;
    st.records.emplace_back();st.records[0].font=std::make_shared<text_v1::Font>();
    text_v1::Glyph glyph;glyph.image=std::make_shared<int>(1);glyph.index=0;glyph.height=12;glyph.x1=glyph.y1=1;st.records[0].glyphs.push_back(glyph);
    s.bind_glyph=[](const auto& g,d::GlyphBinding& out,std::string&){out.bitmap={g.image,32,16};return true;};
    cx.bitmap_cache=true;cx.freetype_cache=false;
    check(!d::display(st,cx,s,error)&&error.find("live FreeType cache")!=std::string::npos,"source-invalid bitmap-only cache accepted");++guards;
    cx.freetype_cache=true;cx.bitmap_cache=false;
    check(!d::display(st,cx,s,error)&&error.find("live bitmap cache")!=std::string::npos,"source-invalid other atlas accepted");++guards;
    std::cout<<"{\"validation\":\"PASS\",\"whole_original_cases\":"<<count<<",\"ownership_and_required_guards\":"<<guards<<"}\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
