// Test-only native field fixtures; this does not change vendor headers or ABI.
#include "gameswf/gameswf.h"
#include "gameswf/gameswf_types.h"
#include "gameswf/gameswf_impl.h"
#include "gameswf/gameswf_freetype.h"
#define private public
#include "gameswf/gameswf_font.h"
#undef private
#define main frozen_localized_hud_fixture_main
#include "hud_freetype_provider.cpp"
#undef main
#include <cmath>
#include <iomanip>
using namespace gameswf;
template<class T>static T gold(std::ifstream& f){T v{};check(bool(f.read(reinterpret_cast<char*>(&v),sizeof v)),"truncated original corpus");return v;}
static unsigned word(float v){unsigned w;std::memcpy(&w,&v,4);return w;}
static float fromword(unsigned w){float v;std::memcpy(&v,&w,4);return v;}
struct FixtureProvider: glyph_provider {
    unsigned cfg[5]{},calls{},nested{};gc_ptr<bitmap_info> image=new bitmap_info;
    font* receiver{};unsigned mutate{};bool inside{};
    bitmap_info* get_char_image(character_def* shape,Uint16,const tu_string&,bool,bool,int,rect* bounds,float* advance) override {
        check(shape==nullptr,"overlay supplied embedded outline before provider");++calls;
        if(mutate==1)receiver->m_code_table.clear();
        if(mutate==2&&!inside){inside=true;glyph inner;check(receiver->get_glyph(&inner,66,16),"nested provider lookup failed");++nested;inside=false;}
        const unsigned b[]={0x80000000,0x3f000000,0xbf800000,0x7fc01234};std::memcpy(bounds,b,16);*advance=fromword(cfg[4]);return cfg[0]&16?image.get_ptr():nullptr;
    }
};
struct Proxy: glyph_provider {
    glyph_provider* borrowed{};unsigned calls{},outline_inputs{};float last_advance{};
    bitmap_info* get_char_image(character_def* shape,Uint16 code,const tu_string& name,bool bold,bool italic,int size,rect* bounds,float* advance) override {
        ++calls;outline_inputs+=shape!=nullptr;auto* out=borrowed->get_char_image(shape,code,name,bold,italic,size,bounds,advance);last_advance=*advance;return out;
    }
};
struct FontTest:Test {
    SwfMovie* movie{};Proxy* proxy{};bool sampled{};int embedded_code{};float embedded_actual{},embedded_provider{},embedded_authored{};
    static bool draw(void* p,const SwfDraw& d,std::string& e){auto& t=*static_cast<FontTest*>(p);if(!t.sampled&&d.kind==SwfDraw::begin){t.sampled=true;auto* f=t.movie->borrowed_font(7);check(f&&f->get_glyph_count()==1&&f->m_advance_table.size()==1,"actual Fontin embedded glyph/layout missing");t.embedded_code=f->get_code_by_index(0);glyph g;check(f->get_glyph(&g,t.embedded_code,12),"actual embedded Fontin provider glyph missing");t.embedded_actual=g.m_glyph_advance;t.embedded_provider=t.proxy->last_advance*(f->is_define_font3()?20.0f:1.0f);t.embedded_authored=f->m_advance_table[0];
#ifndef DH2_STOCK_FONT_BASELINE
        check(g.m_glyph_index==-1&&word(t.embedded_actual)==word(t.embedded_provider),"actual embedded Fontin advance replaced native provider");
#endif
    }return Test::draw(p,d,e);}
};
int main(int argc,char**argv){try{if(argc!=6)return 2;FontTest t;t.swfs=argv[1];t.fonts=argv[2];t.assets=argv[3];t.initialize(argv[4]);SwfFontServices fs{&t,Test::font_read,nullptr};SwfHudFreetypeProvider provider(fs,1,Test::bitmap_probe);gc_ptr<Proxy> proxy=new Proxy;proxy->borrowed=provider.borrowed_provider();gc_ptr<FixtureProvider> fixture=new FixtureProvider;t.proxy=proxy.get_ptr();
    unsigned compared=0,arithmetic_nan=0,fallbacks=0,provider_hits=0,mutation=0,nested=0,cache_checks=0,actual_fallbacks=0;
    float actual_text_width=0,provider_text_width=0;unsigned authored_overrides=0;gc_ptr<bitmap_info> retained;
    set_glyph_provider(fixture.get_ptr());
    {SwfMovie movie;t.movie=&movie;struct ClearExternalGlobal {~ClearExternalGlobal(){set_glyph_provider(nullptr);}} clear_external;SwfServices s;s.context=&t;s.read=Test::read;s.texture=Test::texture;s.image=Test::image;s.draw=FontTest::draw;s.native_call=Test::native;s.stencil=Test::stencil;s.glyphs=proxy.get_ptr();std::string e;
     check(movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",s,e)&&movie.advance(0,e)&&movie.display(0,0,480,320,e),e.c_str());check(get_glyph_provider()==fixture.get_ptr(),"facade did not restore borrowed provider");
     auto* real=movie.borrowed_font(436);check(real&&std::string(real->get_name().c_str())=="Arial","actual HUD Arial font436 missing");
     set_glyph_provider(proxy.get_ptr());auto before_images=t.alpha,before_reads=t.reads;
     for(auto* c="PLAYLIST";*c;++c){glyph a,b;check(real->get_glyph(&a,*c,12)&&real->get_glyph(&b,*c,12),"actual cached HUD glyph missing");float expected=proxy->last_advance*(real->is_define_font3()?20.0f:1.0f);actual_text_width+=a.m_glyph_advance;provider_text_width+=expected;authored_overrides+=word(a.m_glyph_advance)!=word(expected);check(a.m_bitmap_info.get_ptr()==b.m_bitmap_info.get_ptr(),"cached provider identity changed");retained=a.m_bitmap_info;++cache_checks;
#ifndef DH2_STOCK_FONT_BASELINE
        check(a.m_glyph_index==-1&&word(a.m_glyph_advance)==word(expected),"real provider advance overwritten by embedded table");
#endif
     }
     check(t.alpha==before_images&&t.reads==before_reads,"cached glyph caused upload or file reload");set_glyph_provider(fixture.get_ptr());
#ifndef DH2_STOCK_FONT_BASELINE
     auto* embedded=movie.borrowed_font(7);fixture->cfg[0]=8;fixture->cfg[4]=0x3f800000;glyph fallback;check(embedded->get_glyph(&fallback,t.embedded_code,12)&&fallback.m_glyph_index==0&&fallback.m_shape_glyph.get_ptr()==embedded->get_glyph_by_index(0)&&word(fallback.m_glyph_advance)==word(t.embedded_authored),"actual authored embedded fallback changed");++actual_fallbacks;glyph missing;check(!embedded->get_glyph(&missing,65534,12)&&missing.m_glyph_index==-1&&missing.m_glyph_advance==1,"actual embedded miss lost provider writes");++actual_fallbacks;
     // Only bitmap-provider-disabled original cases belong to this native FT
     // adapter contract. Face lookup/lifetime remains provider-owned.
     font projected(real->get_player());projected.m_glyphs.resize(3);fixture->receiver=&projected;std::ifstream f(argv[5],std::ios::binary);check(bool(f)&&gold<unsigned>(f)==0x31594c47,"missing original glyph corpus");auto count=gold<unsigned>(f);check(count==6912,"wrong original count");
     for(unsigned i=0;i<count;++i){unsigned cfg[5];for(auto& x:cfg)x=gold<unsigned>(f);auto rc=gold<unsigned>(f);unsigned expected[9];for(auto& x:expected)x=gold<unsigned>(f);auto nc=gold<unsigned>(f);for(unsigned k=0;k<nc;++k)(void)gold<unsigned>(f);if(cfg[0]&1)continue;
        projected.m_code_table.clear();if(cfg[1])projected.m_code_table.add(65,2);projected.m_zone_table.resize(cfg[2]);projected.m_advance_table.resize(cfg[3]);if(cfg[3]>2)projected.m_advance_table[2]=fromword(cfg[4]);std::memcpy(fixture->cfg,cfg,sizeof cfg);fixture->calls=0;set_glyph_provider(cfg[0]&8?fixture.get_ptr():nullptr);glyph g;std::memset(&g.m_bounds,0xa5,16);check(projected.get_glyph(&g,65,16)==bool(rc),"original source return mismatch");auto w=word(g.m_glyph_advance);
        if((expected[0]&0x7fffffff)>0x7f800000&&(w&0x7fffffff)>0x7f800000&&projected.is_define_font3()&&rc&&!(expected[5]!=0xffffffff&&cfg[3]))++arithmetic_nan;else check(w==expected[0],"original source advance mismatch");check(unsigned(g.m_glyph_index)==expected[5]&&std::memcmp(&g.m_bounds,expected+1,16)==0,"original source index/bounds mismatch");check(bool(g.m_bitmap_info)==bool(expected[6]!=0&&expected[6]!=0x5555),"original source provider bitmap mismatch");check(fixture->calls==unsigned(bool(cfg[0]&8)),"native provider dispatch count mismatch");provider_hits+=bool(g.m_bitmap_info);fallbacks+=g.m_glyph_index>=0;++compared;
     }
     check(f.peek()==std::char_traits<char>::eof(),"trailing original corpus");set_glyph_provider(fixture.get_ptr());projected.m_zone_table.clear();projected.m_advance_table.clear();projected.m_code_table.clear();projected.m_code_table.add(65,2);fixture->cfg[0]=8;fixture->cfg[4]=0x3f800000;fixture->mutate=1;glyph changed;check(!projected.get_glyph(&changed,65,16)&&changed.m_glyph_index==-1&&changed.m_glyph_advance==1,"embedded lookup occurred before synchronous provider mutation");++mutation;
     fixture->mutate=2;fixture->cfg[0]=8|16;fixture->cfg[4]=0x40000000;glyph outer;check(projected.get_glyph(&outer,65,16)&&outer.m_glyph_index==-1&&outer.m_glyph_advance==2&&fixture->nested==1,"nested provider lost outer glyph");nested=fixture->nested;fixture->mutate=0;
#endif
     set_glyph_provider(fixture.get_ptr());
    }
    check(retained&&retained->get_width()>0&&retained->get_height()>0,"retained glyph bitmap invalid after movie destruction");check(get_glyph_provider()==nullptr,"external provider not cleared before last-player teardown");set_glyph_provider(nullptr);retained=nullptr;fixture->receiver=nullptr;
    std::cout<<std::setprecision(9)<<"{\"validation\":\"PASS\",\"original_supported_cases\":"<<compared<<",\"arithmetic_nan_class_cases\":"<<arithmetic_nan<<",\"provider_hits\":"<<provider_hits<<",\"embedded_fallbacks\":"<<fallbacks<<",\"actual_embedded_fallback_checks\":"<<actual_fallbacks<<",\"provider_mutation_cases\":"<<mutation<<",\"nested_provider_cases\":"<<nested<<",\"cache_identity_checks\":"<<cache_checks<<",\"actual_PLAYLIST_advance\":"<<actual_text_width<<",\"provider_PLAYLIST_advance\":"<<provider_text_width<<",\"authored_advance_overrides\":"<<authored_overrides<<",\"actual_embedded_code\":"<<t.embedded_code<<",\"actual_embedded_advance\":"<<t.embedded_actual<<",\"provider_embedded_advance\":"<<t.embedded_provider<<",\"authored_embedded_advance\":"<<t.embedded_authored<<",\"provider_outline_inputs\":"<<proxy->outline_inputs<<",\"alpha_uploads\":"<<t.alpha<<",\"bitmap_quads\":"<<t.quads<<"}\n";return 0;
}catch(const std::exception& e){set_glyph_provider(nullptr);std::cerr<<e.what()<<'\n';return 1;}}
