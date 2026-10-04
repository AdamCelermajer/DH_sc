// Real cache resources/retained core; Debug, locale, no-player and external
// texture delivery remain explicit controlled providers from the old audit.
#define main previous_hud_provider_audit
#include "hud_freetype_provider.cpp"
#undef main
#include "../swf_text_font_platform_v1.hpp"
#include "gameswf/gameswf_font.h"
#include "gameswf/gameswf_render.h"
struct PlatformTest:Test {
    gameswf::player* player{};
    SwfTextFontPlatformV1* platform{};
    unsigned unified{},metrics{},core_callbacks{};
    static bool start(void* c,const SwfAsLease& lease,std::string&){auto& t=*static_cast<PlatformTest*>(c);t.player=lease.player;return true;}
    static bool probe(void* c,SwfAsGraph&,std::string& error){
        auto& t=*static_cast<PlatformTest*>(c);
        gameswf::gc_ptr<gameswf::font> font=new gameswf::font(t.player);font->set_name("Fontin SmallCaps");
        auto projected=t.platform->font(font.get_ptr(),error);check(bool(projected),error.c_str());
        auto source=t.platform->layout_services({});float units{},height{};
        check(source.units_per_em(projected,units,error)&&source.font_height(projected,height,error),error.c_str());
        check(units>1&&height>0,"real provider metrics unavailable");t.metrics+=2;
        for(unsigned code:{65,86,46,32}) {
            text_v1::Glyph g;bool found{};
            check(source.glyph(projected,code,16,g,found,error)&&found&&g.image,error.c_str());
            const auto before=t.images.size();gameswf::glyph core;
            check(font->get_glyph(&core,code,16)&&core.m_bitmap_info!=nullptr,"real core glyph failed");++t.core_callbacks;
            check(t.images.size()==before,"core provider uploaded a second source glyph texture");
            check(core.m_glyph_advance==g.advance&&core.m_bounds.m_x_min==g.x0&&core.m_bounds.m_x_max==g.x1&&
                  core.m_bounds.m_y_min==g.y0&&core.m_bounds.m_y_max==g.y1,"layout/core used different face or raster");
            auto callback=t.platform->display_services({});text_display_v2::GlyphBinding bound;
            check(callback.bind_glyph(g,bound,error),error.c_str());
            check(bound.bitmap.width==core.m_bitmap_info->get_width()&&bound.bitmap.height==core.m_bitmap_info->get_height(),"layout/core bitmap dimensions changed");++t.unified;
            const auto quads=t.quads;gameswf::matrix m;gameswf::rect r,uv;r.m_x_max=r.m_y_max=100;uv.m_x_max=uv.m_y_max=1;gameswf::render::draw_bitmap(m,core.m_bitmap_info.get_ptr(),r,uv,gameswf::rgba(255,255,255,255));
            check(t.quads==quads+1,"real materialized core glyph did not reach facade sink");
        }
        return true;
    }
};
int main(int argc,char** argv){try {
    check(argc==5,"usage: swf_text_font_platform_v1 swfs fonts cache-data fonts-pycst");
    auto t=std::make_shared<PlatformTest>();t->swfs=argv[1];t->fonts=argv[2];t->assets=argv[3];t->initialize(argv[4]);
    SwfServices services;services.context=t.get();services.read=Test::read;services.texture=Test::texture;
    services.image=Test::image;services.draw=Test::draw;services.native_call=Test::native;services.stencil=Test::stencil;
    services.native_owner=t;services.graph_start=PlatformTest::start;
    SwfFontServices fonts{t.get(),Test::font_read,nullptr};TextFontBackendsV2 backends;
    // The exact authored HUD requests resolve to genuine .ttf resources. This
    // explicit bitmap-font miss fixture is not a production GFNT cache backend.
    backends.bitmap_face=[](const auto&,TextBitmapFaceV2& out,std::string&){out={};return true;};
    std::string error;unsigned checks{};
    {SwfTextFontPlatformV1 platform(fonts,services,t,backends,1);t->platform=&platform;
      platform.policy().renderer_feature=[](const auto& command,std::string& error){if(command.kind==edit_text_display_v1::Command::grid_fit)return true;error="fixture has no render cache";return false;};
      auto owned=platform.services();SwfMovie movie;
      check(movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",owned,error),error.c_str());
      check(movie.advance(0,error)&&movie.display(0,0,480,320,error),error.c_str());
      check(platform.error().empty(),platform.error().c_str());
      check(t->resolved.count("Fontin SmallCaps")&&t->resolved.count("Arial"),"actual authored HUD font requests absent");
      check(movie.action_script(t.get(),PlatformTest::probe,error),error.c_str());
      check(t->unified==4&&t->metrics==2&&t->core_callbacks==4,"unified face/core proof incomplete");
      // A queue lease can expire before the frame flush. Failure must retain
      // the diagnostic and restore the caller's scheduling state.
      std::shared_ptr<SwfEditTextFieldV1> lease(nullptr,[](SwfEditTextFieldV1*){});
      check(platform.enqueue(lease,error),error.c_str());lease.reset();
      check(!platform.flush_buffered_text(error)&&!error.empty()&&!platform.policy().flushing,"failed flush leaked flushing state");++checks;
      platform.policy().flushing=true;
      check(!platform.flush_buffered_text(error)&&platform.policy().flushing,"failed nested flush lost prior state");++checks;
      platform.policy().flushing=false;
      auto missing=owned;missing.native_owner.reset();SwfMovie rejected;
      check(!rejected.load({},"dqhud_droid.swf",missing,error),"unowned startup callback accepted");++checks;
    }
    t->platform=nullptr;t->player=nullptr;services={};auto weak=std::weak_ptr<PlatformTest>(t);t.reset();
    check(weak.expired(),"text platform retained a graph/provider cycle");++checks;
    std::cout<<"{\"validation\":\"PASS\",\"unified_real_layout_core_glyphs\":4,\"actual_face_metrics\":2,\"lifetime_guards\":"<<checks<<",\"authored_HUD_loaded\":true,\"same_face_raster_texture\":true,\"GFNT_cache_fixture_boundary\":true}\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
