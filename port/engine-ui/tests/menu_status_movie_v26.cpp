#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wreturn-type"
#endif
#define main previous_font_fixture_main
#include "hud_freetype_provider.cpp"
#undef main
#include "../menu_status_movie_v26.hpp"
#include "../swf_text_font_platform_v1.hpp"
#include "gameswf/gameswf_function.h"
#include "gameswf/gameswf_sprite.h"
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
static void inspect_tree(gameswf::sprite_instance* sprite,const std::string& path,int depth){
 if(depth>4)return;
 for(int i=0;i<sprite->m_display_list.size();++i){auto* c=sprite->m_display_list.get_character(i);if(!c)continue;
  const std::string name=std::string(c->get_name().c_str());auto p=path+"."+name;
  if(c->is(gameswf::sprite_instance::m_class_id)){auto* child=static_cast<gameswf::sprite_instance*>(c);
   if(child->get_visible()&&child->get_frame_count()>1)std::cerr<<"active_timeline "<<p<<" id "<<child->get_id()<<" frame "<<child->get_current_frame()<<" frames "<<child->get_frame_count()<<'\n';
   inspect_tree(child,p,depth+1);
  }
 }
}
static bool inspect(void*,SwfAsGraph& graph,std::string& error){SwfAsValue root;gameswf::as_object* object{};if(!graph.root_value(root,error)||!graph.borrow_object(root,object,error)||!object->is(gameswf::sprite_instance::m_class_id))return false;inspect_tree(static_cast<gameswf::sprite_instance*>(object),"_root",0);return true;}
int main(int argc,char** argv){try{
 check(argc==5,"Expected actual swfs/fonts/cache/fonts_pycst");auto t=std::make_shared<StatusTest>();
 t->swfs=argv[1];t->fonts=argv[2];t->assets=argv[3];t->initialize(argv[4]);std::string error;SwfMovie movie;t->movie=&movie;
 MenuStatusServicesV26 status;status.owner=t->actual_provider;
 status.hud_root=[&](auto& id,auto&){id=t->root_identity;return true;};
 status.invoke=[&](auto id,const char* method,int context,auto& e){check(id==t->root_identity&&context==0,"Wrong same HUD/root context");++t->status_calls;return menu_status_invoke_movie_v26(movie,method,context,e);};
 t->messages=std::make_unique<MenuStatusMessagesV26>(status);
 SwfServices services;services.context=t.get();services.read=Test::read;services.texture=Test::texture;services.image=Test::image;
 services.draw=Test::draw;services.native_call=Test::native;services.stencil=Test::stencil;services.native_owner=t->actual_provider;
 services.native_actions={"NativeGetNextStatusMessage","NativeStopMessage"};services.native_action=StatusTest::native_action;
 TextFontBackendsV2 backends;backends.bitmap_face=[](const auto&,TextBitmapFaceV2& out,std::string&){out={};return true;};
 SwfTextFontPlatformV1 platform({t.get(),Test::font_read,nullptr},services,t->actual_provider,backends,1);
 platform.policy().renderer_feature=[](const auto& c,std::string& e){if(c.kind==edit_text_display_v1::Command::grid_fit)return true;e="fixture has no render cache";return false;};
 check(movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",platform.services(),error)&&movie.advance(0,error),error.c_str());
 check(movie.action_script(t.get(),root_id,error),error.c_str());
 SwfValue arg,out;arg.kind=SwfValue::text;arg.string="MENU_LEVEL_UP";
 check(Test::native(t.get(),"NativeGetStringFromSymbol",{arg},out,error),error.c_str());check(!out.string.empty(),"Actual level-up localization missing");
 check(t->messages->enqueue_level_up(out.string,error),error.c_str());
 check(movie.action_script(nullptr,inspect,error),error.c_str());
 check(t->status_calls==1&&t->message_queries>0,"Original onStatusMessage did not query source queue");
 std::string message;bool found{};check(t->messages->next(0,message,found,error)&&found&&message==out.string,"Same queue text mismatch");
 check(t->messages->enqueue_level_up(out.string,error)&&t->status_calls==1&&t->messages->count(0)==2,"Queued level-up duplicated start");
 check(t->messages->stop_status(error)&&t->messages->count(0)==1&&t->status_calls==2,error.c_str());
 check(t->messages->stop_status(error)&&t->messages->count(0)==0,error.c_str());
 std::cout<<"{\"status\":\"PASS\",\"actual_localization\":true,\"source_message_queries\":"<<t->message_queries<<",\"source_start_callbacks\":"<<t->status_calls<<",\"texture_draw_debug_fixtures\":true}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
