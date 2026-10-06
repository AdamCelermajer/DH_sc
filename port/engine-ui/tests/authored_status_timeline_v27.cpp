#define main previous_font_fixture_main
#include "hud_freetype_provider.cpp"
#undef main
#include "../menu_status_movie_v26.hpp"
#include "../swf_text_font_platform_v1.hpp"
#include "gameswf/gameswf_function.h"
#include "gameswf/gameswf_sprite.h"
#include "../authored_status_timeline_v27.hpp"
struct StatusTest:Test {
 SwfMovie* movie{};std::uintptr_t root_identity{};
 std::shared_ptr<void> actual_provider=std::make_shared<int>(1);
 std::unique_ptr<MenuStatusMessagesV26> messages;
 unsigned status_calls{},message_queries{},stops{};
 static bool native_action(void* raw,const char* name,const gameswf::fn_call& fn,std::string& error){
  auto& t=*static_cast<StatusTest*>(raw);bool handled{};
  if(std::strcmp(name,"NativeGetNextStatusMessage")==0)++t.message_queries;
  if(std::strcmp(name,"NativeStopMessage")==0)++t.stops;
  if(!menu_status_native_v26(*t.messages,name,fn,handled,error))return false;
  if(!handled){error=std::string("Required reached status test native ")+name;return false;}return true;
 }
};
static bool root_id(void* raw,SwfAsGraph& graph,std::string& error){SwfAsValue root;if(!graph.root_value(root,error))return false;static_cast<StatusTest*>(raw)->root_identity=root.identity();return true;}
struct TimelineTest : StatusTest {
 unsigned checks{};
 void require(bool ok,const std::string& e){++checks;check(ok,e.c_str());}
 static bool native_action(void* raw,const char* name,const gameswf::fn_call& fn,std::string& error){
  const std::string n=name;
  if(n=="NativeGetOptionParameters"){
   if(fn.nargs!=2||std::string(fn.arg(0).to_string())!="HUDStyle"||!fn.arg(1).to_object()){error="Actual activation argument order";return false;}
   fn.arg(1).to_object()->set_member("CurrentOption",gameswf::as_value(2));return true;
  }
  if(n=="NativeSkillGetEquipedSkillsIDs"){
   auto* array=fn.arg(0).to_object();if(!array){error="Required actual skill receiver";return false;}
   for(const char* key:{"0","1","2"})array->set_member(key,gameswf::as_value(-1));return true;
  }
  if(n=="NativeHUDGetActiveFaery"){fn.result->set_int(-1);return true;}
  if(n=="NativeUseIpodPlayer"){fn.result->set_bool(false);return true;}
  // Declared external platform observers only. Status dispatch is real below.
  if(n=="NativeChangeRolloverInputBehavior"||n=="NativeUpdateOrientation"||n=="NativePauseAllSounds")return true;
  return StatusTest::native_action(raw,name,fn,error);
 }
};
int main(int argc,char** argv){try{
 check(argc==5,"Expected actual swfs/fonts/cache/fonts_pycst");auto t=std::make_shared<TimelineTest>();
 t->swfs=argv[1];t->fonts=argv[2];t->assets=argv[3];t->initialize(argv[4]);std::string error;SwfMovie movie;t->movie=&movie;
 MenuStatusServicesV26 status;status.owner=t->actual_provider;
 status.hud_root=[&](auto& id,auto&){id=t->root_identity;return true;};
 status.invoke=[&](auto id,const char* method,int context,auto& e){t->require(id==t->root_identity&&context==0,"Wrong same HUD context");++t->status_calls;return menu_status_invoke_movie_v26(movie,method,context,e);};
 t->messages=std::make_unique<MenuStatusMessagesV26>(status);
 SwfServices services;services.context=t.get();services.read=Test::read;services.texture=Test::texture;services.image=Test::image;
 services.draw=Test::draw;services.native_call=Test::native;services.stencil=Test::stencil;services.native_owner=t->actual_provider;
 services.native_actions={"NativeGetNextStatusMessage","NativeStopMessage","NativeGetOptionParameters","NativeSkillGetEquipedSkillsIDs","NativeHUDGetActiveFaery","NativeUseIpodPlayer","NativeChangeRolloverInputBehavior","NativeUpdateOrientation","NativePauseAllSounds"};services.native_action=TimelineTest::native_action;
 TextFontBackendsV2 backends;backends.bitmap_face=[](const auto&,TextBitmapFaceV2& out,std::string&){out={};return true;};
 SwfTextFontPlatformV1 platform({t.get(),Test::font_read,nullptr},services,t->actual_provider,backends,1);
 platform.policy().renderer_feature=[](const auto& c,std::string& e){if(c.kind==edit_text_display_v1::Command::grid_fit)return true;e="fixture has no render cache";return false;};
 t->require(movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",platform.services(),error)&&movie.advance(0,error),error);
 t->require(movie.action_script(t.get(),root_id,error),error);
 AuthoredGameplayHudV1 hud(movie);t->require(hud.bind(2,error)&&hud.activate(error)&&hud.refresh_skills(error),error);
 AuthoredStatusTimelineV27 timeline;AuthoredStatusDiagnosticV27 d;
 t->require(timeline.update(movie,hud,0,false,d,error),error);
 t->require(d.frames==48,"Source status frame count differs");
 SwfValue arg,out;arg.kind=SwfValue::text;arg.string="MENU_LEVEL_UP";
 t->require(Test::native(t.get(),"NativeGetStringFromSymbol",{arg},out,error)&&!out.string.empty(),error);
 t->require(t->messages->enqueue_level_up(out.string,error),error);
 t->require(t->messages->enqueue_status(0,out.string,19,error)&&t->messages->count(0)==2&&t->status_calls==1,"Second status must queue without restart");
 t->require(timeline.update(movie,hud,0,false,d,error)&&d.visible,error);
 const int started=d.frame;
 t->require(timeline.update(movie,hud,1000,false,d,error)&&d.frame==started,"Paused status advanced");
 unsigned advances{},first_stop_at{},second_stop_at{};
 for(unsigned n=1;n<=120;++n){
  t->require(timeline.update(movie,hud,34,true,d,error),error);advances+=d.advanced;
  if(t->stops==1&&!first_stop_at)first_stop_at=n;
  if(t->stops==2){second_stop_at=n;break;}
 }
 t->require(first_stop_at&&second_stop_at>first_stop_at,"Original onAnimationEnd did not dequeue both statuses");
 t->require(t->stops==2&&t->status_calls==2&&t->messages->count(0)==0&&!d.visible,"Final authored stop must hide and empty queue");
 const int stopped=d.frame;
 for(unsigned n=0;n<4;++n)t->require(timeline.update(movie,hud,250,true,d,error)&&!d.visible&&d.frame==stopped,"Hidden completed status advanced");
 std::cout<<"{\"status\":\"PASS\",\"checks\":"<<t->checks<<",\"authored_frames\":48,\"advances\":"<<advances<<",\"first_stop_at\":"<<first_stop_at<<",\"second_stop_at\":"<<second_stop_at<<",\"source_stop_callbacks\":"<<t->stops<<",\"source_start_callbacks\":"<<t->status_calls<<",\"selected_current_hud\":true,\"external_platform_render_fixtures\":true}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
