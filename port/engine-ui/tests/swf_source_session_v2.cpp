// Controlled real-core graph/layout/native providers test the new ownership
// adapter. They are NOT original HUD resources or a production acceptance
// service. Original input/frame/value kernels retain their separate evidence.
#define main previous_input_session_fixture_main
#include "swf_input_session_v1.cpp"
#undef main
#include "swf_source_movie_v1.hpp"
#include "swf_input_session_v2.hpp"
#include "original_ui_input_session_v1.hpp"
#include "hud_player_values.hpp"
#include "gameswf/gameswf_environment.h"
namespace {
struct Connected:Test {
 unsigned layouts{};
 static bool native_layout(void*p,const char*n,const gameswf::fn_call&fn,std::string&){auto&t=*static_cast<Connected*>(p);require(std::string(n)=="FixtureStartup"&&fn.env);auto*root=fn.env->m_target.get_ptr();require(root&&root->is(gameswf::sprite_instance::m_class_id));layout(static_cast<gameswf::sprite_instance*>(root));++t.layouts;return true;}
 static bool read_layout(void*,const char*n,std::vector<std::uint8_t>&out,std::string&){out=movie(true);return true;}
 static gameswf::sprite_instance*child(gameswf::sprite_instance*p,const char*name,int id,int count){auto*r=p->get_root();gameswf::gc_ptr<gameswf::sprite_definition>d=new gameswf::sprite_definition(p->get_player(),r->m_def.get_ptr());d->set_frame_count(count);d->m_playlist.resize(count);for(int j=0;j<count;++j)d->inc_loading_frame();gameswf::gc_ptr<gameswf::sprite_instance>c=new gameswf::sprite_instance(p->get_player(),d.get_ptr(),r,p,id);c->set_name(name);gameswf::matrix m;gameswf::cxform cx;p->m_display_list.add_display_object(c.get_ptr(),p->m_display_list.size()+1,false,cx,m,0,0,0);return c.get_ptr();}
 static void layout(gameswf::sprite_instance*r){const int ids[5]{90,147,152,113,31};for(unsigned i=0;i<5;++i){std::string path=hud_value_clip_path(HudValueClip(i));auto*p=r;if(path.rfind("_root.",0)==0)path.erase(0,6);else path="menu_HUD_0."+path;
  std::size_t at=0;while(at<path.size()){auto end=path.find('.',at);const bool last=end==std::string::npos;std::string name=path.substr(at,last?end:end-at);gameswf::sprite_instance*next=nullptr;for(int j=0;j<p->m_display_list.size();++j){auto*c=p->m_display_list.get_character(j);if(name==c->get_name().c_str()){next=static_cast<gameswf::sprite_instance*>(c);break;}}if(!next)next=child(p,name.c_str(),last?ids[i]:0,last?(i<2?100:i==2?101:103):1);p=next;if(last)break;at=end+1;}
 }}
 SwfInputSessionConfigV1 configuration(const std::shared_ptr<Connected>&self){auto c=config(self);c.movie_services.read=read_layout;c.movie_services.native_action=native_layout;return c;}
};
}
int main(){try{
 std::string e;SwfInputSessionV1 dummy;auto t=std::make_shared<Connected>();t->session=&dummy;active=t.get();auto config=t->configuration(t);
 // No lazy constructor inference or stock mouse fallback. Caller migration
 // wraps services before loading the exact player/root graph.
 SwfServices wrapped;require(source_movie_services_v1(config.movie_services,wrapped,e),e);
 {SwfMovie legacy;require(legacy.load(config.shared,config.movie,wrapped,e),e);require(t->layouts==1,"Source shared initialization ran unexpected action count");require(legacy.advance(.034f,e),e);require(legacy.display(0,0,480,320,e),e);}
 auto bad=config.movie_services;bad.native_owner.reset();SwfServices unchanged=wrapped;require(!source_movie_services_v1(bad,unchanged,e)&&unchanged.context==wrapped.context,"Default observer legitimized an unowned typed callback");
 SwfInputSessionV2 source;require(source.load(config,e),e);SwfInputSessionStatusV2 status;constexpr const char*sha="a4ffacd1abdf7c9b2ba19c46ebb81c60c100458731a4cdba5880391b9c11b238";
 require(source.update(0,false,e),e);require(source.bind_status_hud(sha,status,e),e);require(status.current());std::int32_t sheet[224]{};sheet[33]=50;sheet[34]=100;sheet[36]=75;sheet[38]=100;sheet[41]=25;sheet[43]=100;
 require(status.update(sheet,224,1,e),e);std::array<std::int32_t,5>frames{};std::size_t dirty=0;require(status.frames(frames,dirty,e)&&frames[0]==74&&frames[1]==24&&frames[2]==50,"Source status values did not reach exact retained clips");
 auto failing=config;failing.viewport.reserved=1;require(!source.load(failing,e)&&status.current());require(source.load(config,e),e);require(!status.current()&&!status.update(sheet,224,1,e)&&e=="Status HUD belongs to a detached session generation");status.release();
 require(source.update(0,false,e),e);require(source.bind_status_hud(sha,status,e),e);source.release();require(!status.current()&&!status.update(sheet,224,1,e));status.release();
 dh2::android_ui::OriginalUiInputSessionV1 app;auto absent=config;absent.input_services.native_event=nullptr;require(!app.load(absent,480,320,0,0,false,sha,e)&&!app.active());
 require(app.load(config,480,320,0,0,false,sha,e),e);require(app.active());require(app.resize(960,640,0,e),e);for(unsigned i=0;i<4;++i)require(app.cursor({10,10,0,0},i,e),e);
 dh2::android_ui::OriginalUiFrameV1 frame{sheet,224,1,34,false,nullptr,nullptr};require(app.frame(frame,e),e);require(t->last_begin.viewport[2]==960&&t->last_begin.viewport[3]==640);require(app.resize(640,960,1,e),e);frame.source_advance_flag=true;require(app.frame(frame,e),e);require(t->last_begin.viewport[2]==640&&t->last_begin.viewport[3]==960);
 require(!app.load(failing,640,960,1,0,false,sha,e)&&app.active());app.release();require(!app.active()&&!app.frame(frame,e));source.release();wrapped={};unchanged={};bad={};config={};failing={};absent={};t.reset();active=nullptr;
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"weak_default_graph_owner\":true,\"generation_pinned_status_values\":true,\"stale_status_rejection\":true,\"Android_adapter_status_input_frame_draw\":true,\"controlled_layout_fixture\":true,\"original_HUD_resource_or_GPU_parity\":false}\n";
 }catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 1;}}
