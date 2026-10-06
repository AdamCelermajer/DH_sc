#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wreturn-type"
#endif
#define main previous_font_fixture_main
#include "hud_freetype_provider.cpp"
#undef main
#include "../authored_hurt_corners_layout_v7.hpp"
#include "../authored_hurt_pulse_v7.hpp"
#include "../player_status_hud.hpp"
#include "../swf_text_font_platform_v1.hpp"
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_sprite.h"
#include <cstring>
#include <stdexcept>
namespace {
unsigned checks{};int width{},height{};
void verify(bool v,const std::string& error){++checks;if(!v)throw std::runtime_error(error);}
struct LayoutTest:Test {
 unsigned vertices{};
 static bool draw(void* p,const SwfDraw& d,std::string& e){auto& t=*static_cast<LayoutTest*>(p);
  if(d.kind==SwfDraw::triangles||d.kind==SwfDraw::triangle_strip)t.vertices+=d.xy.size();
  if(d.kind==SwfDraw::bitmap_quad)t.vertices+=8;
  return Test::draw(p,d,e);}
};
bool orientation(void*,std::int32_t& result,std::string&){result=0;return true;}
bool dimensions(void*,std::int32_t& w,std::int32_t& h,std::string&){w=width;h=height;return true;}
struct Query {
 gameswf::gc_ptr<gameswf::character> outer,inner,pulse_shape;gameswf::matrix outer_matrix,inner_matrix;gameswf::cxform outer_alpha,inner_alpha,pulse_alpha;int frame{},pulse_frame{};
 static bool inspect(void* p,SwfAsGraph& graph,std::string& e){auto& q=*static_cast<Query*>(p);SwfAsValue root,value;gameswf::as_object* object{};
  if(!graph.root_value(root,e)||!graph.find_target(root,"_root.HurtCorners",value,e)||!graph.borrow_object(value,object,e)||!object||!object->is(gameswf::sprite_instance::m_class_id))return false;
  auto* sprite=static_cast<gameswf::sprite_instance*>(object);q.outer=sprite;q.inner=sprite->m_display_list.get_character(0);
  if(!q.inner||!q.inner->is(gameswf::sprite_instance::m_class_id)){e="Required original pulse child";return false;}
  auto* pulse=static_cast<gameswf::sprite_instance*>(q.inner.get_ptr());q.pulse_shape=pulse->m_display_list.get_character(0);
  if(!q.pulse_shape){e="Required source pulse shape wrapper";return false;}
  q.pulse_alpha=q.pulse_shape->get_cxform();q.pulse_frame=pulse->get_current_frame();
  q.outer_matrix=sprite->get_matrix();q.inner_matrix=q.inner->get_matrix();q.outer_alpha=sprite->get_cxform();q.inner_alpha=q.inner->get_cxform();q.frame=sprite->get_current_frame();return true;
 }
 void restored()const{
  verify(std::memcmp(outer_matrix.m_,outer->get_matrix().m_,sizeof(outer_matrix.m_))==0,"HP outer source matrix changed");
  verify(std::memcmp(inner_matrix.m_,inner->get_matrix().m_,sizeof(inner_matrix.m_))==0,"Pulse source transform changed");
  verify(std::memcmp(outer_alpha.m_,outer->get_cxform().m_,sizeof(outer_alpha.m_))==0,"Health alpha changed by layout");
  verify(std::memcmp(inner_alpha.m_,inner->get_cxform().m_,sizeof(inner_alpha.m_))==0,"Health-selected child alpha changed by layout");
  verify(std::memcmp(pulse_alpha.m_,pulse_shape->get_cxform().m_,sizeof(pulse_alpha.m_))==0,"Authored pulse alpha changed by layout");
  verify(inner->get_current_frame()==pulse_frame,"Pulse phase changed by fullscreen layout");
  verify(outer->get_current_frame()==frame,"Health frame changed by fullscreen layout");
 }
};
}
int main(int argc,char** argv){try{
 verify(argc==5,"Expected swfs fonts cache-data fonts-pycst");
 for(auto size:{std::array<int,2>{480,320},{2400,1080},{1024,768},{1080,2400}}){
  width=size[0];height=size[1];auto t=std::make_shared<LayoutTest>();t->swfs=argv[1];t->fonts=argv[2];t->assets=argv[3];t->initialize(argv[4]);SwfMovie movie;std::string e;
  SwfServices services;services.context=t.get();services.read=Test::read;services.texture=Test::texture;services.image=Test::image;
  services.draw=LayoutTest::draw;services.native_call=Test::native;services.stencil=Test::stencil;
  TextFontBackendsV2 backends;backends.bitmap_face=[](const auto&,TextBitmapFaceV2& out,std::string&){out={};return true;};
  SwfTextFontPlatformV1 platform({t.get(),Test::font_read,nullptr},services,t,backends,1);
  platform.policy().renderer_feature=[](const auto& c,std::string& error){if(c.kind==edit_text_display_v1::Command::grid_fit)return true;error="fixture has no render cache";return false;};
  verify(movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",platform.services(),e)&&movie.advance(0,e),e);
  // Standalone fixture supplies the shown-HUD visibility lifecycle; source
  // root initialization hides HurtCorners until the actual HUD Show owner.
  // The tested production layout never modifies this flag or the source art.
  verify(movie.set_visible("_root.HurtCorners",true,e),e);
  ViewportState64 viewport{{0,9600,0,6400},{0,0,width,height},{0,0,width,height},1,0,0};
  verify(movie.connect_viewport(viewport,{nullptr,orientation,dimensions},e),e);FlashCamera40 camera{};
  verify(movie.update_viewport(camera,e),e);std::int32_t bounds[]{0,0,width,height};verify(movie.set_source_bounds(bounds,2,e),e);
  // Same actual HP-index frame used by the low-health case; layout must not
  // alter its stopped frame or the pulse's authored alpha/transform.
  PlayerStatusHud status(movie);verify(status.bind("a4ffacd1abdf7c9b2ba19c46ebb81c60c100458731a4cdba5880391b9c11b238",e),e);
  std::vector<int> sheet(256);sheet[36]=10;sheet[38]=100;sheet[41]=sheet[43]=100;sheet[34]=100;
  AuthoredHurtPulseV7 pulse;AuthoredHurtPulseDiagnosticV7 pulse_info;float minimum=1,maximum=0;
  for(int repeat=0;repeat<66;++repeat){
   verify(status.update(sheet.data(),sheet.size(),1,e)&&pulse.update(movie,34,true,pulse_info,e),e);
   verify(pulse_info.outer_frame==9,"Real source HP lookup changed");minimum=std::min(minimum,pulse_info.pulse_alpha);maximum=std::max(maximum,pulse_info.pulse_alpha);
   Query before;verify(movie.action_script(&before,Query::inspect,e),e);HurtCornersLayoutV7 layout;
   const auto previous=t->vertices;verify(display_authored_hurt_corners_v7(movie,&layout,e),e);before.restored();
   verify(t->vertices>previous,"Actual authored blood geometry not submitted");
   const float sw=layout.stage[1]-layout.stage[0],sh=layout.stage[3]-layout.stage[2];
   const float dx=(layout.display[1]-layout.display[0])/sw,dy=(layout.display[3]-layout.display[2])/sh;
   verify(std::fabs(layout.paint_bounds[0]-layout.display[0])<30*dx,"Blood misses left viewport edge");
   verify(std::fabs(layout.paint_bounds[1]-layout.display[1])<30*dx,"Blood misses right viewport edge");
   verify(std::fabs(layout.paint_bounds[2]-layout.display[2])<30*dy,"Blood misses top viewport edge");
   verify(std::fabs(layout.paint_bounds[3]-layout.display[3])<30*dy,"Blood misses bottom viewport edge");
  }
  verify(minimum==166.f/256.f&&maximum==1,"Full source pulse cycle lost through layout");
 }
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"viewports\":4,\"pulse_advances_per_viewport\":66,\"scope\":\"actual cached HurtCorners full-display geometry and untouched health/pulse frames alpha transforms; declared shown-HUD visibility and texture/render fixtures\"}\n";
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
