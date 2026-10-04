#include "../text_layout_v1.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::ui::text_v1;
using Bytes=std::vector<unsigned char>;
static void check(bool b,const std::string& e){if(!b)throw std::runtime_error(e);}
static std::uint32_t bits(float f){std::uint32_t w;std::memcpy(&w,&f,4);return w;}
static float real(std::uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
static void word(Bytes& b,std::uint32_t w){for(int i=0;i<4;++i)b.push_back((w>>(8*i))&255);}
static void text(Bytes& b,const std::string& s){word(b,s.size());b.insert(b.end(),s.begin(),s.end());}
static void font(Bytes& b,const std::shared_ptr<Font>& f){text(b,f->name);word(b,f->bold);word(b,f->italic);}
static std::uint32_t read(std::ifstream& f){unsigned char b[4];check(bool(f.read(reinterpret_cast<char*>(b),4)),"truncated original text corpus");return b[0]|(std::uint32_t(b[1])<<8)|(std::uint32_t(b[2])<<16)|(std::uint32_t(b[3])<<24);}
static std::string readtext(std::ifstream& f){auto n=read(f);check(n<1000000,"unbounded text corpus");std::string s(n,'\0');check(bool(f.read(s.data(),n)),"truncated source text");return s;}
static Bytes snapshot(const State& s,const Bytes& events,unsigned diagnostics){
    Bytes b;for(auto w:s.cached_rect)word(b,w);for(auto w:s.bounds)word(b,bits(w));
    for(auto w:{s.xcursor,s.ycursor,s.x,s.y})word(b,bits(w));for(auto w:{s.first_line,s.word_record,s.word_glyph})word(b,w);word(b,s.records.size());
    for(auto& r:s.records){word(b,r.font_id);word(b,bool(r.font));if(r.font)font(b,r.font);word(b,r.rgba);word(b,r.underline);for(auto w:{r.x,r.y,r.height})word(b,bits(w));for(auto w:{r.has_x,r.has_y,r.has_font})word(b,w);word(b,r.glyphs.size());
        for(auto& g:r.glyphs){word(b,bits(g.advance));word(b,bool(g.image));for(auto w:{g.x0,g.x1,g.y0,g.y1})word(b,bits(w));word(b,std::uint16_t(g.height)|(std::uint32_t(std::uint16_t(g.index))<<16));word(b,g.code&255);word(b,g.code>>8);word(b,g.type);}
    }word(b,diagnostics);word(b,events.size());b.insert(b.end(),events.begin(),events.end());return b;
}
static void run(std::ifstream& input,std::uint32_t index,bool html,const std::uint32_t* cfg,const std::string& source,const Bytes& expected){
    State s;s.font=std::make_shared<Font>();s.font->native=std::make_shared<int>(1);s.font->name="Original";s.font->metric58=800;s.font->metric5c=200;s.font->define_font3=cfg[4];
    s.rgba=0;s.text=source;s.text_height=real(cfg[0]);s.alignment=cfg[1];s.rect_max=real(cfg[2]);s.multiline=cfg[3];s.left=real(cfg[5]);s.right=real(cfg[6]);s.indent=real(cfg[7]);s.leading=real(cfg[8]);s.letter_spacing=real(cfg[9]);s.cursor=3;
    if(!(cfg[11]&1))s.font.reset();Bytes events;Services v;v.diagnostic_state=std::make_shared<DiagnosticState>();v.diagnostic_state->missing_glyphs=cfg[11]>>8;
    auto event=[&](unsigned op,const std::shared_ptr<Font>& f){word(events,op);font(events,f);};
    v.root_scale_word=[](float& o,std::string&){o=1;return true;};
    v.units_per_em=[&](auto& f,float& o,std::string&){event(1,f);o=1024;return true;};
    v.font_height=[&](auto& f,float& o,std::string&){event(2,f);o=1024;return true;};
    v.kerning=[&](auto& f,int previous,int code,float& o,std::string&){event(3,f);word(events,previous);word(events,code);o=previous==65&&code==86?-16:0;return true;};
    v.glyph=[&](auto& f,std::uint16_t code,int size,Glyph& g,bool& found,std::string&){event(4,f);word(events,code);word(events,size);g.advance=code==32?512:600;g.x0=g.y0=0;g.x1=g.y1=1;found=!(cfg[10]&2);return true;};
    v.missing_glyph=[&](auto& f,int code,std::string&){event(7,f);word(events,code);return true;};
    v.clone_font=[&](auto& f,std::shared_ptr<Font>& o,std::string&){event(5,f);o=std::make_shared<Font>(*f);o->native=std::make_shared<int>(2);return true;};
    v.image=[&](auto&,int,int,Image& o,std::string&){if(cfg[10]&1){o.native=std::make_shared<int>(3);o.width=7;o.height=9;}return true;};
    v.preload_enabled=[](bool& o,std::string&){o=true;return true;};v.preload=[&](State&,std::string&){word(events,6);return true;};
    std::string e;check(format_text(s,html,v,e),std::to_string(index)+" "+e);auto actual=snapshot(s,events,v.diagnostic_state->missing_glyphs);
    if(actual!=expected){std::size_t p=0;while(p<actual.size()&&p<expected.size()&&actual[p]==expected[p])++p;
        std::cerr<<"case "<<index<<" html="<<html<<" text="<<source<<" byte="<<p<<" actual size="<<actual.size()<<" expected size="<<expected.size()<<"\n";
        for(std::size_t i=(p/4)*4;i<std::min(p+28,std::min(actual.size(),expected.size()));i+=4){unsigned a{},b{};std::memcpy(&a,actual.data()+i,4);std::memcpy(&b,expected.data()+i,4);std::cerr<<i<<" "<<std::hex<<a<<" "<<b<<std::dec<<"\n";}throw std::runtime_error("whole original layout mismatch");}
}
int main(int argc,char** argv){try{
    check(argc==2,"usage: text_layout_v1 corpus.bin");std::ifstream f(argv[1],std::ios::binary);check(bool(f)&&read(f)==0x31545854,"missing original text corpus");auto count=read(f);for(unsigned i=0;i<count;++i){auto html=read(f);std::uint32_t cfg[12];for(auto& w:cfg)w=read(f);auto s=readtext(f);auto size=read(f);check(size<1000000,"unbounded original snapshot");Bytes expected(size);check(bool(f.read(reinterpret_cast<char*>(expected.data()),size)),"truncated original snapshot");run(f,i,html,cfg,s,expected);}check(f.peek()==std::char_traits<char>::eof(),"trailing corpus");
    unsigned guards=0;State s;s.font=std::make_shared<Font>();s.text="A";Services empty;std::string e;check(!format_text(s,false,empty,e)&&s.records.empty(),"missing root accepted");++guards;
    Tag t;bool parsed=true;check(parse_tag(t,"font color=no",parsed,e)&&!parsed&&t.entries.size()==1,"malformed source tag prefix lost");++guards;
    check(!parse_tag(t,"fontcolor='x'",parsed,e),"unsafe backward scan accepted");++guards;
    check(parse_tag(t,"/font",parsed,e)&&!parsed,"closing tag performed writes");++guards;
    std::cout<<"{\"validation\":\"PASS\",\"whole_original_cases\":"<<count<<",\"required_or_unsafe_domain_guards\":"<<guards<<"}\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
