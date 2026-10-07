#define main previous_font_fixture_main
#include "hud_freetype_provider.cpp"
#undef main
#include "../authored_hurt_pulse_v7.hpp"
#include "../player_status_hud.hpp"
#include "../swf_text_font_platform_v1.hpp"
int main(int argc,char** argv){try {
 check(argc==5,"usage: hurt-pulse swfs fonts cache-data fonts-pycst");
 auto t=std::make_shared<Test>();t->swfs=argv[1];t->fonts=argv[2];t->assets=argv[3];t->initialize(argv[4]);
 SwfServices services;services.context=t.get();services.read=Test::read;services.texture=Test::texture;
 services.image=Test::image;services.draw=Test::draw;services.native_call=Test::native;services.stencil=Test::stencil;
 TextFontBackendsV2 backends;backends.bitmap_face=[](const auto&,TextBitmapFaceV2& out,std::string&){out={};return true;};
 SwfTextFontPlatformV1 platform({t.get(),Test::font_read,nullptr},services,t,backends,1);
 platform.policy().renderer_feature=[](const auto& c,std::string& e){if(c.kind==edit_text_display_v1::Command::grid_fit)return true;e="fixture has no render cache";return false;};
 std::string e;SwfMovie movie;check(movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",platform.services(),e)&&movie.advance(0,e),e.c_str());
 PlayerStatusHud status(movie);check(status.bind("a4ffacd1abdf7c9b2ba19c46ebb81c60c100458731a4cdba5880391b9c11b238",e),e.c_str());
 std::vector<int> sheet(256);sheet[36]=10;sheet[38]=100;sheet[41]=sheet[43]=100;sheet[34]=100;
 AuthoredHurtPulseV7 pulse;AuthoredHurtPulseDiagnosticV7 d;float minimum=1,maximum=0;unsigned changed=0;
 check(status.update(sheet.data(),sheet.size(),1,e)&&pulse.update(movie,0,false,d,e),e.c_str());
 const auto initial=d.pulse_frame;
 for(unsigned n=0;n<66;++n){check(status.update(sheet.data(),sheet.size(),1,e)&&pulse.update(movie,34,true,d,e),e.c_str());
  check(d.outer_frame==9,"Pulse changed health-index authority");minimum=std::min(minimum,d.pulse_alpha);maximum=std::max(maximum,d.pulse_alpha);changed+=d.advanced;}
 check(minimum==166.f/256.f&&maximum==1&&changed==66,"Actual 22-frame authored pulse did not cycle");
 const auto paused=d.pulse_frame;check(pulse.update(movie,500,false,d,e)&&d.pulse_frame==paused,"No-tick HUD pulse advanced");
 sheet[36]=100;check(status.update(sheet.data(),sheet.size(),1,e)&&pulse.update(movie,34,true,d,e)&&d.outer_frame==99&&d.health_alpha==0,"Recovery did not use genuine transparent outer health frame");
 sheet[36]=51;check(status.update(sheet.data(),sheet.size(),1,e)&&pulse.update(movie,0,false,d,e)&&d.outer_frame==50&&d.health_alpha==0,"Source health threshold differs");
 sheet[36]=50;check(status.update(sheet.data(),sheet.size(),1,e)&&pulse.update(movie,0,false,d,e)&&d.outer_frame==49&&d.health_alpha==13.f/256.f,"Source last visible health frame differs");
 std::cout<<"{\"validation\":\"PASS\",\"real_authored_pulse_frames\":22,\"advanced\":"<<changed<<",\"minimum_alpha\":"<<minimum<<",\"maximum_alpha\":"<<maximum<<",\"no_tick_and_health_recovery\":true,\"initial_frame\":"<<initial<<"}\n";
 return 0;
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
