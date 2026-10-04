// Reuse only the frozen binary reader/snapshot helpers. This test's expected
// data comes from actual original setters and actual original whole formatter.
#define main frozen_text_layout_audit_main
#include "text_layout_v1.cpp"
#undef main
#include "../edit_text_field_v1.hpp"
namespace field=dh2::ui::edit_text_v1;

static void run_field(unsigned index,unsigned op,bool html,std::int32_t maximum,
        const unsigned* cfg,const std::string& initial,const std::string& supplied,
        const std::string& variable,unsigned flags,const std::string& first,
        const std::string& second,const Bytes& expected){
    field::State q;auto& s=q.layout;q.definition=std::make_shared<field::Definition>();
    q.definition->variable=variable;q.definition->maximum=maximum;
    q.definition->multiline=cfg[3];q.definition->rectangle[1]=real(cfg[2]);
    s.cached_rect.fill(0);s.word_glyph=0;s.font=std::make_shared<Font>();
    s.font->native=std::make_shared<int>(1);s.font->name="Original";
    s.font->metric58=800;s.font->metric5c=200;s.font->define_font3=cfg[4];
    s.rgba=0;s.text=initial;s.text_height=real(cfg[0]);s.alignment=cfg[1];
    s.rect_max=real(cfg[2]);s.multiline=cfg[3];s.left=real(cfg[5]);
    s.right=real(cfg[6]);s.indent=real(cfg[7]);s.leading=real(cfg[8]);
    s.letter_spacing=real(cfg[9]);s.cursor=3;
    if(!(cfg[11]&1))s.font.reset();Bytes events,bound;field::Services services;auto& v=services.layout;
    v.diagnostic_state=std::make_shared<DiagnosticState>();v.diagnostic_state->missing_glyphs=cfg[11]>>8;
    auto event=[&](unsigned op,const std::shared_ptr<Font>& f){word(events,op);font(events,f);};
    v.root_scale_word=[](float& o,std::string&){o=1;return true;};
    v.units_per_em=[&](auto& f,float& o,std::string&){event(1,f);o=1024;return true;};
    v.font_height=[&](auto& f,float& o,std::string&){event(2,f);o=1024;return true;};
    v.kerning=[&](auto& f,int previous,int code,float& o,std::string&){event(3,f);word(events,previous);word(events,code);o=previous==65&&code==86?-16:0;return true;};
    v.glyph=[&](auto& f,std::uint16_t code,int size,Glyph& g,bool& found,std::string&){event(4,f);word(events,code);word(events,size);g.advance=code==32?512:600;g.x0=g.y0=0;g.x1=g.y1=1;found=!(cfg[10]&2);return true;};
    v.missing_glyph=[&](auto& f,int code,std::string&){event(7,f);word(events,code);return true;};
    v.clone_font=[&](auto& f,std::shared_ptr<Font>& o,std::string&){event(5,f);o=std::make_shared<Font>(*f);o->native=std::make_shared<int>(2);return true;};
    v.image=[&](auto&,int,int,Image& o,std::string&){if(cfg[10]&1){o.native=std::make_shared<int>(3);o.width=7;o.height=9;}return true;};
    v.preload_enabled=[](bool& o,std::string&){o=true;return true;};
    v.preload=[&](auto&,std::string&){word(events,6);return true;};
    unsigned conversions=0;
    q.definition->font=s.font;q.definition->text_height=s.text_height;
    q.definition->alignment=s.alignment;q.definition->left=s.left;
    q.definition->right=s.right;q.definition->indent=s.indent;q.definition->leading=s.leading;
    q.definition->rgba=0;q.definition->default_text=supplied;q.definition->html=true;
    services.read_bound=[&](const std::string& name,field::BoundRead& out,std::string&){
        word(bound,10);text(bound,name);word(bound,11);text(bound,name);
        out.found=flags&1;out.self=flags&2;
        out.to_text=[&](std::string& out,std::string&){auto n=conversions++;word(bound,12);word(bound,n);if(flags&4)s.text=first;out=n?second:first;return true;};return true;
    };
    services.write_bound=[&](const std::string& name,const std::string& input,std::string&){
        word(bound,10);text(bound,name);word(bound,13);text(bound,name);text(bound,input);return true;
    };
    services.initialize_fill=[&](std::string&){for(unsigned n=14;n<=17;++n)word(bound,n);return true;};
    std::string e;bool ok=op==0?field::set_text(q,supplied,html,services,e):
        op==1?field::set_text_value(q,supplied,html,services,e):op==2?field::refresh_bound(q,services,e):field::initialize(q,services,e);
    check(ok,std::to_string(index)+" "+e);
    Bytes actual;text(actual,s.text);auto layout=snapshot(s,events,v.diagnostic_state->missing_glyphs);
    actual.insert(actual.end(),layout.begin(),layout.end());word(actual,bound.size());actual.insert(actual.end(),bound.begin(),bound.end());
    if(op==3){word(actual,q.needs_update);word(actual,q.focus);word(actual,q.layout.cursor);word(actual,q.background);}
    if(actual!=expected){std::size_t p=0;while(p<actual.size()&&p<expected.size()&&actual[p]==expected[p])++p;
        std::cerr<<"field case "<<index<<" op "<<op<<" mismatch at "<<p<<" actual "<<actual.size()<<" expected "<<expected.size()<<'\n';
        throw std::runtime_error("original complete field coordinator mismatch");}
}
int main(int argc,char** argv){try{
    check(argc==2,"usage: edit_text_field_v1 field-gold.bin");std::ifstream f(argv[1],std::ios::binary);
    check(bool(f)&&read(f)==0x31464554,"missing original field corpus");auto count=read(f);
    for(unsigned i=0;i<count;++i){auto op=read(f);auto html=read(f);auto maximum=static_cast<std::int32_t>(read(f));unsigned cfg[12];for(auto& w:cfg)w=read(f);
        auto initial=readtext(f),supplied=readtext(f),variable=readtext(f);auto flags=read(f);auto first=readtext(f),second=readtext(f);
        auto n=read(f);check(n<1000000,"unbounded field corpus");Bytes expected(n);check(bool(f.read(reinterpret_cast<char*>(expected.data()),n)),"truncated field snapshot");
        run_field(i,op,html,maximum,cfg,initial,supplied,variable,flags,first,second,expected);
    }
    check(f.peek()==std::char_traits<char>::eof(),"trailing original field corpus");
    unsigned guards=0;field::State q;field::Services v;std::string e;
    check(!field::set_text(q,"text",false,v,e),"missing definition accepted");++guards;
    q.definition=std::make_shared<field::Definition>();q.definition->maximum=1;
    check(field::set_text(q,"é",false,v,e)&&q.layout.text.size()==1&&q.layout.records.empty(),"source null-font byte truncation prefix lost");++guards;
    q.layout.font=std::make_shared<Font>();q.layout.text="old";
    check(!field::set_text(q,"new",false,v,e)&&q.layout.text=="n"&&q.layout.cached_rect[0]==0xffffffff,"missing layout service lost source prefix");++guards;
    q.layout.font.reset();q.definition->variable="bound";
    check(!field::set_text_value(q,"supplied",false,v,e)&&q.layout.text=="s","missing setter accepted or truncation prefix lost");++guards;
    check(!field::refresh_bound(q,v,e),"missing getter accepted");++guards;
    v.read_bound=[](auto&,auto& value,std::string&){value.found=true;return true;};
    check(!field::refresh_bound(q,v,e),"missing retained conversion accepted");++guards;
    std::cout<<"{\"validation\":\"PASS\",\"original_field_cases\":"<<count<<",\"required_and_prefix_guards\":"<<guards<<",\"mismatches\":0}\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
