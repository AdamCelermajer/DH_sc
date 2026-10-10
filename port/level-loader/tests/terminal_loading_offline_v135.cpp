// Executes the byte-exact shipped Loading sprite action block on GameSWF.
// Script/save, menu effects and player records are explicit host fixtures;
// the constructor, current-Level bridge, query kernels, terminal dispatcher,
// controller attachment and Loading ActionScript execute their production bodies.
#include "canonical_level_context_v1.hpp"
#include "native_gslevel_runtime_v27.hpp"
#include "lifecycle_v36_counter_borrow.hpp"
#include "terminal_loading_v97.hpp"
#include "menu_end_loading_source_v114.hpp"
#include "source_online_loading_menu_v135.hpp"
#include "application_services_owner_v5.hpp"
#include "player_controller_attachment_v70.hpp"
#include "swf_movie.hpp"
#include "swf_loading_menu_v1.hpp"
#include "gameswf/gameswf_player.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_function.h"
#include "gameswf/gameswf_action.h"
#include "gameswf/gameswf_stream.h"
#include "base/tu_file.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <stdexcept>
using namespace dh2;
namespace {
unsigned checks{};
void check(bool value,const std::string& error="Terminal loading assertion"){
 ++checks;if(!value)throw std::runtime_error(error+" ["+std::to_string(checks)+"]");
}
using Bytes=std::vector<std::uint8_t>;
Bytes read(const std::string& path){std::ifstream f(path,std::ios::binary);check(bool(f),path);return {std::istreambuf_iterator<char>(f),{}};}
struct Fixture:std::enable_shared_from_this<Fixture> {
 std::shared_ptr<application::ApplicationServicesOwnerV5> app=std::make_shared<application::ApplicationServicesOwnerV5>();
 std::shared_ptr<loader::NativeGSLevelGlobalsV27> globals=std::make_shared<loader::NativeGSLevelGlobalsV27>();
 std::shared_ptr<loader::NativeGSLevelRuntimeV27> gs;
 ui::LoadingMenuStateServicesV1 state;ui::LoadingMenuMultiplayerServicesV1 multiplayer;
 std::uint32_t online_state{},debug_count{},module_id{},stage36{},stage37{},progress_calls{},end_calls{},fade_calls{},back_calls{},refresh_calls{},sound_calls{};
 std::uintptr_t character{0x13501},controller_id{};
 player::PlayerControllerAttachmentV70 controller;
 std::shared_ptr<events::EventManagerOwnerV12> events=std::make_shared<events::EventManagerOwnerV12>(0x13502);
 std::shared_ptr<ui::MenuStackOwnerV1> menus=std::make_shared<ui::MenuStackOwnerV1>(16);
 ui::MenuStackGlobalsV1 menu_globals{};
 ui::MenuStackCharacterV1 menu_character{0x13503,1,1,1,1};
 ui::MenuStackServicesV1 menu_services{};
 ui::SwfMovie movie;Bytes movie_bytes,actions;std::unique_ptr<gameswf::action_buffer> action;
 std::string failure;
 static int menu_effect(void* raw,ui::MenuStackV1*,ui::MenuStackRequestV1* q){
  auto& self=*static_cast<Fixture*>(raw);
  if(q->operation==ui::MenuStackOperationV1::menu_valid)q->result=q->menu->valid_menu;
  if(q->operation==ui::MenuStackOperationV1::menu_set_visible)q->menu->visible=q->value;
  if(q->operation==ui::MenuStackOperationV1::menu_hide)q->menu->visible=0;
  (void)self;return 0;
 }
 std::shared_ptr<loader::CanonicalLevelContextV1> level(){
  std::string e;std::shared_ptr<loader::CanonicalLevelContextV1> out;
  loader::LevelSourceRequestV1 request;request.identity="SWAMP";request.definition="worlds/swamp01.dwld";
  check(loader::CanonicalLevelContextV1::create(request,app,out,e),e);
  loader::LevelConstructorServicesV3 s;s.application=app;s.debug_level_load_count=&debug_count;s.module_id_global=&module_id;
  data::LevelTables table;s.levels=&table;
  s.construct_script=[](auto,bool,auto& script,auto& e){script=std::make_shared<int>(1);e.clear();return true;};
  s.script_assign_path=[](const auto&,const char*,std::size_t,auto& e){e.clear();return true;};
  s.script_load=[](const auto&,const char*,auto& e){e.clear();return true;};
  s.online_byte5=[online=app->get_online_loading_v55()](auto& value,auto& e){value=online->byte5();e.clear();return true;};
  check(out->construct_source_v3({"worlds/swamp01.dwld",0,7,0,0,0,0,-1,0},std::move(s),e),e);
  out->constructor_borrow_v3().fields->field130=35;return out;
 }
 void bind(){
  std::string e;gs=std::make_shared<loader::NativeGSLevelRuntimeV27>(globals);
  check(gs->prepare_loading([this](auto& value,auto& e){value=online_state;e.clear();return true;},e),e);
  state=gs->loading_services();check(state.context&&state.context_owner,"Loading context has no independent lease");
  multiplayer=application::source_online_loading_menu_v135(app->get_online_loading_v55());
  check(multiplayer.context_owner&&multiplayer.enabled,"Actual COnline query not owned");
 }
 static bool native(void* raw,const char* name,const gameswf::fn_call& fn,std::string& e){
  auto& self=*static_cast<Fixture*>(raw);
  if(!std::strcmp(name,"NativeGetLoadingProgress")){++self.progress_calls;return ui::swf_loading_progress_v1(fn,self.state,e);}
  if(!std::strcmp(name,"NativeIsMultiplayerLoadCompleted"))return ui::swf_loading_multiplayer_completed_v1(fn,self.multiplayer,e);
  if(!std::strcmp(name,"NativeIsMultiplayerHost"))return ui::swf_loading_multiplayer_host_v1(fn,self.multiplayer,e);
  if(!std::strcmp(name,"NativeMustWaitForHost"))return ui::swf_loading_wait_for_host_v1(fn,self.multiplayer,e);
  if(!std::strcmp(name,"NativeEndLoading")){++self.end_calls;bool advanced{};return ui::swf_loading_end_v1(fn,self.state,advanced,e);}
  if(!std::strcmp(name,"NativeBackToHud")){++self.back_calls;e.clear();return true;}
  if(!std::strcmp(name,"NativeRefreshHudManager")){++self.refresh_calls;e.clear();return true;}
  if(!std::strcmp(name,"NativePlaySoundFX")){++self.sound_calls;e.clear();return true;}
  e=std::string("Unexpected authored native call: ")+name;return false;
 }
 static gameswf::sprite_instance* child(gameswf::sprite_instance* parent,const char* name,int frames){
  auto* root=parent->get_root();gameswf::gc_ptr<gameswf::sprite_definition> def=new gameswf::sprite_definition(parent->get_player(),root->m_def.get_ptr());
  def->set_frame_count(frames);def->m_playlist.resize(frames);for(int i=0;i<frames;++i)def->inc_loading_frame();
  gameswf::gc_ptr<gameswf::sprite_instance> clip=new gameswf::sprite_instance(parent->get_player(),def.get_ptr(),root,parent,139);
  clip->set_name(name);gameswf::matrix matrix;gameswf::cxform color;
  parent->m_display_list.add_display_object(clip.get_ptr(),parent->m_display_list.size()+1,false,color,matrix,0,0,0);return clip.get_ptr();
 }
 static bool install(void* raw,ui::SwfAsGraph& graph,std::string& e){
  auto& self=*static_cast<Fixture*>(raw);ui::SwfAsValue root_value;gameswf::as_object* object{};
  if(!graph.root_value(root_value,e)||!graph.borrow_object(root_value,object,e))return false;
  auto* root=static_cast<gameswf::sprite_instance*>(object);
  auto* animation=child(root,"loading_anim",101);child(animation,"btn_loading_continue",1);
  auto* text=child(root,"continue_text",72);auto* label=child(text,"MENU_TOUCH_TO_CONTINUE",1);child(label,"text",1);
  child(root,"LoadingText",1);child(root,"btn_BLOCKER",1);
  tu_file input(tu_file::memory_buffer);input.write_bytes(self.actions.data(),static_cast<int>(self.actions.size()));input.set_position(0);
  gameswf::stream stream(&input);
  self.action=std::make_unique<gameswf::action_buffer>();self.action->read(&stream);
  check(self.action->get_length()==static_cast<int>(self.actions.size()),"Shipped Loading block length differs");
  self.action->execute(root->get_environment());return true;
 }
 bool call(const char* target,const char* method,std::string& e){
  struct Call{const char* target;const char* method;} call{target,method};
  return movie.menu_action_script(&call,[](void* raw,ui::SwfAsGraph& graph,std::string& e){
   auto& c=*static_cast<Call*>(raw);ui::SwfAsValue root,clip,result;bool callable{};
   if(!graph.root_value(root,e))return false;clip=root;
   if(c.target&&*c.target&&!graph.find_target(root,c.target,clip,e))return false;
   if(!graph.invoke(clip,clip,c.method,{},result,callable,e))return false;
   if(!callable){e=std::string("Shipped Loading method absent: ")+c.method;return false;}return true;
  },e);
 }
 void initialize(const std::string& swf,const std::string& block){
  std::string e;movie_bytes=read(swf);actions=read(block);bind();
  ui::SwfServices s;s.context=this;s.native_owner=shared_from_this();s.native_action=native;
  s.native_actions={"NativeGetLoadingProgress","NativeIsMultiplayerLoadCompleted","NativeIsMultiplayerHost","NativeMustWaitForHost","NativeEndLoading","NativeBackToHud","NativeRefreshHudManager","NativePlaySoundFX"};
  s.read=[](void* raw,const char*,auto& out,auto& e){out=static_cast<Fixture*>(raw)->movie_bytes;e.clear();return true;};
  s.draw=[](void*,const auto&,auto& e){e.clear();return true;};
  s.native_call=[](void*,const char* name,const auto& args,auto& out,auto& e){
   check(std::string(name)=="NativeGetStringFromSymbol"&&args.size()==1,"Unexpected translation fixture call");
   out.kind=ui::SwfValue::text;out.string=args[0].string;e.clear();return true;
  };
  check(movie.load({},"LoadingHostFixture",s,e),e);check(movie.menu_action_script(this,install,e),e);
  check(menus->add_render(0x13504,0x40,&menu_character,nullptr,e),e);
  check(menus->add_menu(0x13505,0x13504,"menu_Loading",&menu_character,1,1,e),e);
  check(menus->add_menu(0x13506,0x13504,"menu_FadeFromBlackScreen",&menu_character,1,0,e),e);
  check(menus->add_menu(0x13507,0x13504,"menu_HUD_0",&menu_character,1,0,e),e);
  check(menus->seal(0x13504,0x13504,menu_globals,e),e);menu_services={this,menu_effect};
  check(!menus->push("menu_Loading",menu_services),"Actual loading push failed");
  player::PlayerControllerAttachmentServicesV70 attachment;attachment.events=events;
  attachment.online=[online=app->get_online_loading_v55()](auto& value,auto& e){value=online->byte5()!=0;e.clear();return true;};
  check(controller.construct(character,0x13508,-1,std::move(attachment),[this](auto& actual,auto,bool,auto& e){controller_id=actual.controller;e.clear();return true;},e),e);
  controller.command().global_blocked=1;
 }
 void complete(const std::shared_ptr<loader::CanonicalLevelContextV1>& level){
  std::string e;globals->s_level=level;loader::LifecycleBorrowV36 fields;
  check(loader::borrow_lifecycle_fields_v36(level,fields,e),e);
  loader::TerminalLoadingServicesV97 t;t.owner=shared_from_this();
  t.current=[this,level](auto phase,auto& e){if(globals->s_level!=level||level->constructor_fields_v3().field130!=phase){e="Different source Level/phase";return false;}e.clear();return true;};
  t.trace=[](auto& e){e.clear();return true;};
  t.online=[online=app->get_online_loading_v55()](auto& value,auto& e){value=online->byte5()!=0;e.clear();return true;};
  t.capture_menu=[this](const char* name,auto& out,auto& e){return ui::capture_menu_lease_v101(menus,name,shared_from_this(),shared_from_this(),menu_services,
   [this](auto id,auto& visible,auto& e){auto* menu=menus->registered_menu_v27(id);if(!menu){e="Retired captured menu";return false;}visible=menu->visible!=0;e.clear();return true;},out,e);};
  t.lookup_menu=[this](const char* name,auto& id,auto& e){auto* menu=menus->menu(name);id=menu?menu->identity:0;e.clear();return true;};
  t.debug_switch=[](const char*,auto& value,auto& e){value=false;e.clear();return true;};
  t.num_local_players=[](bool flag,auto& count,auto& e){check(!flag);count=1;e.clear();return true;};
  t.local_player=[this](auto index,bool flag,auto& out,auto& e){check(index==0&&!flag);out.receiver=shared_from_this();out.identity=0x13509;out.character660=&character;e.clear();return true;};
  t.unblock_controller=[this](auto id,auto& e){check(id==character);return controller.source_end_loading_unblock_v97(controller_id,e);};
  t.place_faery_followers_null=[](auto& e){e.clear();return true;}; //Explicit empty qualifying-list fixture boundary.
  auto terminal=std::make_shared<loader::TerminalLoadingV97>(std::move(t));loader::LifecycleServicesV36 services;
  services.stage_body[36]=[this,terminal](auto& e){++stage36;return terminal->step(36,e);};
  services.stage_body[37]=[this,terminal](auto& e){++stage37;return terminal->step(37,e);};
  services.publish_progress=[this](auto phase,auto progress,auto& e){
   check(globals->s_level&&globals->s_level->constructor_fields_v3().field130==std::uint32_t(phase));
   check(globals->s_level->constructor_fields_v3().phase30==std::uint32_t(progress));
   if(phase==38){++fade_calls;if(menus->push("menu_FadeFromBlackScreen",menu_services)){e="Fade push failed";return false;}}
   return call("","onProgress",e);
  };
  loader::LifecycleV36 lifecycle(fields.fields,fields.actual_level_owner,{},std::move(services));
  check(lifecycle.tick()==loader::LifecycleStatusV36::loading,lifecycle.diagnostics().error);
  check(*fields.fields.state130==36&&*fields.fields.progress30==100&&stage36==0&&end_calls==0,"35 tail did not run shipped progress100 queries");
  check(lifecycle.tick()==loader::LifecycleStatusV36::awaiting_end_loading,lifecycle.diagnostics().error);
  check(stage36==1&&*fields.fields.state130==36,"Offline36 auto-advanced");
  check(call("loading_anim.btn_loading_continue","onRelease",e),e);
  check(*fields.fields.state130==37&&*fields.fields.progress30==100&&end_calls==1,"Shipped continue did not commit SAME36 to37");
  check(back_calls==1&&refresh_calls==1&&sound_calls==1,"Shipped continue native order/endpoints absent");
  check(lifecycle.tick()==loader::LifecycleStatusV36::source_finished,lifecycle.diagnostics().error);
  check(*fields.fields.state130==38&&*fields.fields.progress30==100&&stage37==1&&fade_calls==1,"37 did not reach38/fade/progress");
  check(controller.command().global_blocked==0&&menus->menu("menu_Loading")->visible==0,"37 controller/pop leaves missing");
 }
};
}
int main(int argc,char** argv){try{
 check(argc==3);auto fixture=std::make_shared<Fixture>();fixture->initialize(argv[1],argv[2]);auto level=fixture->level();fixture->complete(level);
 std::weak_ptr<loader::NativeGSLevelRuntimeV27> retired_gs=fixture->gs;
 std::weak_ptr<loader::CanonicalLevelContextV1> retired_level=level;
 std::weak_ptr<void> context=fixture->state.context_owner;
 auto retained=fixture->state;fixture->globals->s_level.reset();fixture->gs.reset();level.reset();
 check(retired_gs.expired()&&retired_level.expired()&&!context.expired(),"Retained movie pinned GS/Level or lost its process bridge");
 std::string e;bool advanced=true;check(ui::loading_menu_finish_v1(retained,advanced,e)&&!advanced,e);
 ui::MenuEndLoadingResultV114 result;check(ui::menu_fs_end_loading_v114(nullptr,nullptr,nullptr,retained,result,e)&&!result.advanced&&result.native_return==0,e);
 std::int32_t progress{};fixture->online_state=3;check(ui::loading_menu_read_progress_v1(retained,progress,e)&&progress==0,e);
 fixture->online_state=0;check(ui::loading_menu_read_progress_v1(retained,progress,e)&&progress==100,e);
 check(fixture->call("","onProgress",e),e); //Retained shipped callback after GS retirement.
 auto missing=std::make_shared<loader::NativeGSLevelRuntimeV27>(fixture->globals);check(missing->prepare_loading({},e),e);
 auto missing_state=missing->loading_services();missing.reset();
 check(!ui::loading_menu_read_progress_v1(missing_state,progress,e)&&e.find("OnlineGameState")!=std::string::npos,"Missing positive absent-Level producer was disguised");
 check(ui::loading_menu_finish_v1(missing_state,advanced,e)&&!advanced,e);missing_state={};
 fixture->bind();auto next=fixture->level();fixture->globals->s_level=next;
 check(ui::loading_menu_read_progress_v1(retained,progress,e)&&progress==0,"Old process bridge did not observe new sole Level");
 next->constructor_borrow_v3().fields->field130=36;bool moved{};check(ui::loading_menu_finish_v1(fixture->state,moved,e)&&moved&&next->constructor_fields_v3().field130==37,e);
 const auto before=next->constructor_fields_v3().field130;check(ui::loading_menu_finish_v1(retained,moved,e)&&!moved&&next->constructor_fields_v3().field130==before,e);
 fixture->app->get_online_loading_v55()->set_is_online_game(1);bool value{};
 check(!ui::loading_menu_multiplayer_completed_v1(fixture->multiplayer,value,e)&&e.find("AllLoadingDone")!=std::string::npos,"Online completion was fabricated");
 check(!ui::loading_menu_multiplayer_host_v1(fixture->multiplayer,value,e)&&e.find("OnlineGameState")!=std::string::npos,"Online hosting was fabricated");
 check(!ui::loading_menu_wait_for_host_v1(fixture->multiplayer,value,e)&&e.find("IsLocalPlayerHosting")!=std::string::npos,"Online waiting was fabricated");
 retained={};check(context.expired(),"Retired callback context leaked into new GS");
 fixture->globals->s_level.reset();next.reset();fixture->state={};fixture->gs.reset();fixture->multiplayer={};fixture->action.reset();fixture->movie=ui::SwfMovie{};fixture.reset();
 std::cout<<"PASS authored Loading35->36->EndLoading->37->38; retained post-retirement callbacks/relaunch; conditional online leaves; checks="<<checks<<'\n';
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
