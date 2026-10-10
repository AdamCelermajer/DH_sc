// Executes current production input/navigation/Show bodies with declared movie
// and scene leaves. Real EventManager and MenuStack storage/delivery are linked.
// This does not execute gameswf bytecode, GPU submission or a packaged APK.
#include <event_manager_owner_v12.hpp>
#include <authored_menu_lifecycle_v1.hpp>
#include <menu_stack_actions_v1.hpp>
#include <menu_stack_owner_v1.hpp>
#include <source_menu_consumption_v121.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
int checks{};
void require(bool value,const std::string& message){++checks;if(!value)throw std::runtime_error(message);}
struct Audit {
 std::vector<std::string> calls;
 std::string fail;
 bool old_missing_show{};
 int deliveries{};
 bool leaf(const std::string& name,std::string& e){calls.push_back(name);if(fail==name){e="injected "+name;return false;}e.clear();return true;}
};
Audit* audit{};
}
namespace gameswf {struct fn_call {std::string target;};}
constexpr int ANDROID_LOG_INFO=4;
template<class... T>int __android_log_print(int,const char*,const char*,T...){return 0;}
namespace dh2::ui {
struct SwfAsValue {
 std::uintptr_t id{};std::string string;
 std::uintptr_t identity()const{return id;}
 static SwfAsValue text(const char* value){return {0,value};}
 static SwfAsValue number(int){return {};}
 static SwfAsValue boolean(bool value){return {0,value?"true":"false"};}
};
struct SwfAsGraph {
 bool root_value(SwfAsValue& out,std::string& e){out={1,"_root"};return audit->leaf("update_class",e);}
 bool find_target(const SwfAsValue&,const char* path,SwfAsValue& out,std::string& e){
  const std::string name=path;out={name=="menu_SelectClass.btn_left"?401u:name=="menu_SelectClass.btn_right"?402u:2u,name};
  return audit->leaf("find:"+name,e);
 }
 bool set_member(const SwfAsValue& object,const char* member,const SwfAsValue& value,bool& accepted,std::string& e){
  accepted=true;return audit->leaf(std::string("set:")+object.string+"."+member+"="+value.string,e);
 }
 bool invoke(const SwfAsValue&,const SwfAsValue&,const char* method,std::initializer_list<SwfAsValue> args,SwfAsValue&,bool& callable,std::string& e){
  callable=true;const std::string value=args.size()?args.begin()->string:"";return audit->leaf(std::string(method)+":"+value,e);
 }
 bool invoke_renderfx(const char*,const char*,std::initializer_list<SwfAsValue>,std::string& e){e.clear();return true;}
};
struct MenuMovieBorrowV58 {std::uintptr_t identity{};};
struct CursorInputFixture {float x{},y{},unused{};int pressed{};};
struct SwfMovie {
 std::string action;
 int continue_hits{};
 std::function<bool(const char*,const gameswf::fn_call&,std::string&)> native;
 template<class F>bool menu_action_script(void* context,F callback,std::string& e){SwfAsGraph graph;return callback(context,graph,e);}
 template<class F>bool menu_display_callback(const char* name,void*,F,std::string& e){return audit->leaf(std::string("display:")+name,e);}
 bool input_cursor_slot_v120(CursorInputFixture payload,std::uint32_t,std::string& e){
  if(payload.pressed||action.empty()){e.clear();return true;}
  if(action=="Continue"){++continue_hits;action.clear();e.clear();return true;}
  gameswf::fn_call call{action};action.clear();
  return native&&native("NativePushMenu",call,e);
 }
};
}
namespace dh2::application {
struct OnlineFixture {std::uint8_t byte5()const{return 0;}};
struct ApplicationServicesOwnerV5 {
 std::shared_ptr<dh2::events::EventManagerOwnerV12> events;
 OnlineFixture online;
 auto events14(){return events;}
 OnlineFixture* get_online_loading_v55(){return &online;}
};
}
namespace dh2::loader {
struct CanonicalCurrentLevelBorrowV1 {
 struct Fields {std::uint8_t byte198{};};
 struct Level {Fields constructor_fields_v3(){return {};}};
 explicit operator bool()const{return false;}
 Level* level(){static Level level;return &level;}
};
}
namespace dh2::world {bool source_animation_scaling_enabled_v99(){return true;}}

struct AvatarStateFixture {int slot{3};};
struct NativeMenuPreviewFixture {
 struct Services {AvatarStateFixture* avatar_state{};std::shared_ptr<void> avatar_state_owner; } services_;
 bool destroyed_{};
 bool busy_{};
 bool current(std::string& e){e.clear();return true;}
 bool destroy_scene(std::string& e){return audit->leaf("DestroyScene",e);}
 bool destroy_avatar_camera(std::string& e){return audit->leaf("DestroyAvatarCamera",e);}
#include "class_creation_preview_under_test.inc"
};
NativeMenuPreviewFixture* preview{};

enum class MenuSingletonKindV67 {main,enter_name,select_class,background};
using FieldsFixture=dh2::ui::AuthoredMenuFieldsV1;
struct NativeMenuSingletonV67 {FieldsFixture fields;MenuSingletonKindV67 kind{};bool constructed{true};int select160{-1};};
std::vector<std::shared_ptr<NativeMenuSingletonV67>> source_singleton_v67;
struct MenuFieldsFixture {
 std::uint32_t consumed{},multitouch{};std::uintptr_t current_event{};
 auto& source_event_consumed_ac_v120(){return consumed;}
 auto& source_current_event_b4_v120(){return current_event;}
 auto manager_multitouch110_v4(){return multitouch;}
 void source_consume_event_v120(){consumed=1;}
};
struct NativeCharacterMenuV4 {
 std::shared_ptr<dh2::ui::MenuStackOwnerV1> shared_stack_v27;
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> process_application_v104;
 std::function<bool(const char*,std::int32_t&,std::string&)> process_debug_v115;
 MenuFieldsFixture fields;
 struct Touch {std::array<std::uint8_t,8> bytes{};auto& active_source_bytes(){return bytes;}} touch;
};
std::shared_ptr<NativeCharacterMenuV4> authored_character_menu;
std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> application_services_v5;
struct NativeProcessFixture {
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> app;
 bool current(std::string& e){e.clear();return true;}
 bool base_show(std::uintptr_t id,std::string& e){
  if(!audit->leaf(id==103?"BaseShow:EnterName":"BaseShow:SelectClass",e))return false;
  for(auto& owner:source_singleton_v67)if(owner->fields.identity==id){
   dh2::ui::AuthoredMenuLifecycleServicesV1 services;services.owner=app.lock();
   services.invoke=[](auto&,const auto& request,std::int32_t& result,std::string& error){
    using O=dh2::ui::AuthoredMenuOperationV1;result=0;
    const char* name="unused";
    switch(request.operation){
    case O::debug_load:name="base:debug_load";break;
    case O::debug_query:name="base:debug_query";break;
    case O::store_rollover_event_enabled:name="base:rollover";break;
    case O::localize:name="base:localize";break;
    case O::set_visible:name="base:visible";break;
    case O::invoke_as:name="base:onPush";break;
    case O::store_application_ec:name="base:application_ec";break;
    case O::register_deadzones:name="base:deadzones";break;
    default:error="unexpected creation MenuBase.Show operation";return false;
    }
    return audit->leaf(name,error);
   };
   return dh2::ui::authored_menu_show_v1(owner->fields,services,e);
  }
  e="missing exact MenuBase owner";return false;
 }
};
std::shared_ptr<NativeProcessFixture> native_process_startup_v119;

namespace dh2::android_ui {
namespace ui=dh2::ui;
constexpr const char* tag="DH2Native";
struct FrontUiSessionV87 {
 struct Impl {
  std::string front_screen{"main"};
  std::shared_ptr<dh2::ui::SwfMovie> movie;
  int class_index{2},class_applied_index{2},class_left{1},class_right{1};
  bool process_class_select_active_v87{};
  std::shared_ptr<void> source_menu_owner_v93;
  std::function<bool(const char*,const gameswf::fn_call&,std::string&)> source_navigation_v93;
  static bool constant(void*,const char*,const char* symbol,std::uint32_t& out,std::string& e){out=1;return audit->leaf(std::string("constant:")+symbol,e);}
  bool text_id_v101(std::uint32_t,std::string& out,std::string& e){out="localized";return audit->leaf("text_id",e);}
#include "class_creation_update_class_under_test.inc"
  static void class_pane(){}
  static bool native_action(void*,const char*,const gameswf::fn_call&,std::string&);
 };
 std::shared_ptr<Impl> impl_;
 std::uintptr_t movie_identity{202};
 bool movie_slot_v93(std::uint32_t slot,dh2::ui::MenuMovieBorrowV58& out,std::string& e){out.identity=slot==2?movie_identity:0;e.clear();return true;}
 bool process_class_select_show_v87(std::uintptr_t,std::string&);
};
#include "class_creation_front_show_under_test.inc"
#include "class_creation_native_action_under_test.inc"
}
dh2::android_ui::FrontUiSessionV87 front_ui;

namespace model_renderer {
FieldsFixture* native_derived_menu_fields_v67(std::uintptr_t id){for(auto& owner:source_singleton_v67)if(owner->fields.identity==id)return &owner->fields;return nullptr;}
bool native_menu_preview_select_show_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,int& slot,std::string& e){
 if(app!=application_services_v5){e="different preview Application";return false;}return preview->select_show(slot,e);
}
bool borrow_current_native_level_v27(dh2::loader::CanonicalCurrentLevelBorrowV1&,std::string& e){e.clear();return true;}
bool native_process_derived_menu_show_v119(std::uintptr_t,std::uintptr_t,std::string&);
bool native_process_derived_menu_focus_v119(std::uintptr_t,std::uintptr_t,std::string&);
bool native_process_derived_menu_blur_v119(std::uintptr_t,std::uintptr_t,std::string&);
}
#include "class_creation_selected_show_under_test.inc"
#include "class_creation_focus_blur_under_test.inc"

struct MovieDirectoryFixture {
 std::shared_ptr<dh2::ui::SwfMovie> main;
 bool movie_slot(std::uint32_t slot,dh2::ui::MenuMovieBorrowV58& out,std::string& e){out.identity=slot==2?202:0;e.clear();return true;}
 bool movie(std::uint32_t slot,const dh2::ui::MenuMovieBorrowV58& receipt,dh2::ui::SwfMovie*& out,std::shared_ptr<void>& pin,std::string& e){
  if(slot!=2||receipt.identity!=202){e="different movie receipt";return false;}out=main.get();pin=main;e.clear();return true;
 }
};
struct PostMovieFixture {std::shared_ptr<MovieDirectoryFixture> directory;};
std::weak_ptr<PostMovieFixture> source_postmovie_v62;
#include "class_creation_application_event_under_test.inc"

struct Fixture {
 Audit records;
 AvatarStateFixture avatar;
 NativeMenuPreviewFixture preview_owner;
 dh2::ui::MenuStackGlobalsV1 globals{};
 std::array<dh2::ui::MenuStackCharacterV1,5> characters{};
 dh2::ui::MenuStackServicesV1 services{this,service};
 std::shared_ptr<PostMovieFixture> post;
 std::shared_ptr<dh2::events::EventManagerOwnerV12> manager;
 std::string service_error;
 bool seed{};
 Fixture(bool initial_splash=false){
  audit=&records;preview=&preview_owner;preview_owner.services_.avatar_state=&avatar;preview_owner.services_.avatar_state_owner=std::make_shared<int>(1);
  application_services_v5=std::make_shared<dh2::application::ApplicationServicesOwnerV5>();
  manager=std::make_shared<dh2::events::EventManagerOwnerV12>(999);application_services_v5->events=manager;
  authored_character_menu=std::make_shared<NativeCharacterMenuV4>();auto& native=*authored_character_menu;
  native.shared_stack_v27=std::make_shared<dh2::ui::MenuStackOwnerV1>(16);native.process_application_v104=application_services_v5;
  native.process_debug_v115=[](const char*,std::int32_t& out,std::string& e){out=0;e.clear();return true;};
  native_process_startup_v119=std::make_shared<NativeProcessFixture>();native_process_startup_v119->app=application_services_v5;
  front_ui.impl_=std::make_shared<dh2::android_ui::FrontUiSessionV87::Impl>();front_ui.movie_identity=202;
  auto movie=std::make_shared<dh2::ui::SwfMovie>();front_ui.impl_->movie=movie;front_ui.impl_->source_menu_owner_v93=native.shared_stack_v27;
  front_ui.impl_->source_navigation_v93=[this](const char* action,const gameswf::fn_call& call,std::string& e){
   dh2::ui::MenuStackActionsGraphV1 graph;graph.owner=authored_character_menu->shared_stack_v27;
   graph.instance=[](auto*& out,std::string& error){out=authored_character_menu->shared_stack_v27.get();error.clear();return true;};
   graph.lifecycle=services;dh2::ui::MenuStackActionsV1 navigation(std::move(graph));
   const dh2::ui::MenuStackActionValueV1 argument{1,4,0};const dh2::ui::MenuStackActionCallV1 arguments{&argument,1,0};
   const bool okay=navigation.dispatch(action,arguments,[&](const auto& value,std::string& out,std::string& error){
    if(value.identity!=1||value.source_type!=4){error="wrong authored argument token/tag";return false;}
    out=call.target;error.clear();return true;
   },e);
   if(!okay&&!service_error.empty())e=service_error;
   return okay;
  };
  movie->native=[](const char* name,const gameswf::fn_call& call,std::string& e){return dh2::android_ui::FrontUiSessionV87::Impl::native_action(front_ui.impl_.get(),name,call,e);};
  post=std::make_shared<PostMovieFixture>();post->directory=std::make_shared<MovieDirectoryFixture>();post->directory->main=movie;source_postmovie_v62=post;
  source_singleton_v67.clear();std::string e;
  for(int i=0;i<5;++i)characters[i]={std::uintptr_t(301+i),1,1,1,1};
  require(native.shared_stack_v27->add_render(202,1,&characters[0],nullptr,e),e);
  const char* names[]{"menu_bg","menu_MainMenu","menu_EnterName","menu_SelectClass","menu_splash"};
  MenuSingletonKindV67 kinds[]{MenuSingletonKindV67::background,MenuSingletonKindV67::main,MenuSingletonKindV67::enter_name,MenuSingletonKindV67::select_class,MenuSingletonKindV67::main};
  for(int i=0;i<5;++i){
   auto owner=std::make_shared<NativeMenuSingletonV67>();owner->fields.identity=101+i;owner->fields.render=202;owner->fields.name=names[i];owner->fields.valid7c=1;owner->kind=kinds[i];source_singleton_v67.push_back(owner);
   require(native.shared_stack_v27->add_menu(101+i,202,names[i],&characters[i],1,1,e),e);
  }
  require(native.shared_stack_v27->seal(202,202,globals,e),e);seed=true;
  require(native.shared_stack_v27->push("menu_bg",services)==0,"seed background");const int initial_push=native.shared_stack_v27->push(initial_splash?"menu_splash":"menu_MainMenu",services);require(initial_push==0,(initial_splash?"seed splash":"seed Main")+std::string(" returned ")+std::to_string(initial_push)+": "+e);seed=false;
  // This fixture omits the production Main.Show body; begin its touch replay
  // after that body's successful tail, which sets the independent slot-2 gate.
  source_main_menu_input_enabled_v120=true;
  dh2::events::EventReceiverV12 receiver;receiver.identity=1040;receiver.context=this;
  receiver.on_event=[](void* raw,const auto& event,auto&,std::int32_t& result,std::string& e){auto& self=*static_cast<Fixture*>(raw);++self.records.deliveries;return source_menu_application_event_v119(application_services_v5,*authored_character_menu,event,result,e);};
  bool inserted{};require(manager->attach(4,receiver,0,inserted,e)&&inserted,"attach App input");records.calls.clear();
 }
 static int service(void* raw,dh2::ui::MenuStackV1*,dh2::ui::MenuStackRequestV1* q){
  auto& self=*static_cast<Fixture*>(raw);auto& request=*q;auto& e=self.service_error;e.clear();
  using O=dh2::ui::MenuStackOperationV1;
  if(self.seed){if(request.operation==O::menu_valid)request.result=1;return 0;}
 if(request.operation==O::menu_valid){request.result=request.menu->valid_menu;return 0;}
  if(request.operation==O::menu_show){
   if(self.seed){request.result=1;return 0;}
   if(self.records.old_missing_show&&request.menu->identity==104){e="Required actual selected Show override for menu_SelectClass";return 1;}
   if(!self.records.leaf(std::string("selectedShow:")+request.menu->name,e))return 1;
   const bool okay=[&]() -> bool {
#include "class_creation_show_dispatch_under_test.inc"
    e="unhandled selected Show fixture";return false;
   }();return okay?0:1;
  }
  if(request.operation==O::menu_focus){if(!self.records.leaf(std::string("focus:")+request.menu->name,e))return 1;return model_renderer::native_process_derived_menu_focus_v119(request.menu->identity,request.render->identity,e)?0:1;}
  if(request.operation==O::menu_blur){auto* f=model_renderer::native_derived_menu_fields_v67(request.menu->identity);if(f&&f->name=="menu_MainMenu")return 0;return model_renderer::native_process_derived_menu_blur_v119(request.menu->identity,request.render->identity,e)?0:1;}
  if(request.operation==O::invoke_as)return self.records.leaf(std::string(request.text)+":"+request.menu->name,e)?0:1;
  if(request.operation==O::play_animation){const bool okay=self.records.leaf(std::string("animation:")+request.text+":"+request.menu->name,e);request.result=1;return okay?0:1;}
  if(request.operation==O::render_reset)return self.records.leaf("render_reset",e)?0:1;
  return 0;
 }
 bool deliver(const char* menu,std::string& e){
  if(!dispatch_source_app_touch_v121(0,7,120,240,e))return false;
  front_ui.impl_->movie->action=menu;
  return dispatch_source_app_touch_v121(2,7,120,240,e);
 }
 auto* stack(){return authored_character_menu->shared_stack_v27->view();}
 auto* selected(){return source_singleton_v67[3].get();}
 void set_initial_splash(){source_main_menu_input_enabled_v120=false;}
 void enter(std::string& e){require(deliver("menu_EnterName",e),e);require(stack()->count==3,"EnterName appends third occurrence");records.calls.clear();}
};
std::size_t step(const std::vector<std::string>& calls,const char* name){for(std::size_t i=0;i<calls.size();++i)if(calls[i]==name)return i;throw std::runtime_error(std::string("missing ")+name);}
void ordered(const std::vector<std::string>& calls,std::initializer_list<const char*> names){std::size_t prior{};bool first=true;for(auto* name:names){const auto index=step(calls,name);require(first||index>prior,std::string("out of order ")+name);prior=index;first=false;}}
int main(){try{
 std::string e;
 {
  Fixture f(true);f.set_initial_splash();
  require(f.stack()->count==2&&std::string(f.stack()->renders[0]->states[1]->name)=="menu_splash","real initial splash is active on primary-2 stack");
  require(!source_main_menu_input_enabled_v120,"original authored Main.Show gate remains false at startup");
  f.post->directory->main->action="Continue";
  require(dispatch_source_app_touch_v121(0,7,120,240,e),e);require(dispatch_source_app_touch_v121(2,7,120,240,e),e);
  require(f.post->directory->main->continue_hits==1,"initial splash Continue hit-test receives release while global gate is false");
  require(!source_main_menu_input_enabled_v120,"splash exception does not publish global Main.Show gate");
  auto stack=authored_character_menu->shared_stack_v27;f.seed=true;require(stack->push("menu_MainMenu",f.services)==0,"seed ordinary main menu");f.seed=false;
  f.post->directory->main->action="Continue";
  require(dispatch_source_app_touch_v121(0,8,120,240,e),e);require(dispatch_source_app_touch_v121(2,8,120,240,e),e);
  require(f.post->directory->main->continue_hits==1,"ordinary menu remains gated until Main.Show succeeds");
 }
 {
  Fixture f;f.enter(e);require(f.deliver("menu_SelectClass",e),e);
  require(f.stack()->count==4&&f.stack()->renders[3]->states[3]->identity==104,"SelectClass appended fourth SAME render occurrence");
  require(f.selected()->select160==3&&f.avatar.slot==-1&&f.preview_owner.destroyed_,"slot3 transfer and destroyed flag");
  require(f.globals.last_open_menu==17,"class menu global retained across inherited GotFocus");
  require(front_ui.impl_->class_index==0&&front_ui.impl_->class_applied_index==0&&front_ui.impl_->class_left==401&&front_ui.impl_->class_right==402,"Show resets then publishes class state and actual cached arrows");
  require(front_ui.impl_->process_class_select_active_v87,"display hook completed before active");
  ordered(f.records.calls,{"OnShow:menu_SelectClass","animation:show:menu_SelectClass","render_reset","selectedShow:menu_SelectClass","DestroyScene","DestroyAvatarCamera","BaseShow:SelectClass","base:debug_load","base:debug_query","base:rollover","base:localize","base:visible","base:onPush","base:application_ec","base:deadzones","update_class","set:menu_SelectClass.btn_left._visible=false","set:menu_SelectClass.btn_right._visible=true","CurrentClass:KnightPlayerBase","display:_root.menu_SelectClass.class_select","focus:menu_SelectClass"});
  for(const char* call:{"OnShow:menu_SelectClass","animation:show:menu_SelectClass","BaseShow:SelectClass","base:onPush","display:_root.menu_SelectClass.class_select"})
   require(std::count(f.records.calls.begin(),f.records.calls.end(),call)==1,std::string("delivered once: ")+call);
  require(f.selected()->fields.visible74==1&&f.selected()->fields.counter78==0,"qualified source MenuBase state stores");
  require(!f.manager->failed(),"successful current transition does not latch EventManager");
  const int deliveries=f.records.deliveries;require(dispatch_source_app_touch_v121(0,7,120,240,e),e);require(dispatch_source_app_touch_v121(2,7,120,240,e),e);
  require(f.records.deliveries==deliveries+2&&!f.manager->failed(),"later current input delivered without secondary no-replay");
  require(authored_character_menu->fields.current_event==0&&dh2::ui::SourceMenuConsumptionV121::current==nullptr,"event identity and consume scope released");
 }
 // Each required leaf preserves its already completed stack/Show prefix; a
 // second input must neither deliver again nor mutate the active occurrences.
 const std::vector<const char*> failures={"OnShow:menu_SelectClass","animation:show:menu_SelectClass","render_reset","selectedShow:menu_SelectClass","DestroyScene","DestroyAvatarCamera","BaseShow:SelectClass","base:debug_load","base:debug_query","base:rollover","base:localize","base:visible","base:onPush","base:application_ec","base:deadzones","update_class","find:menu_SelectClass","find:menu_SelectClass.class_title.text","constant:MENU_CLASS_00","text_id","set:menu_SelectClass.class_title.text.htmlText=localized","find:menu_SelectClass.class_description.text","constant:MENU_KNIGHT_DESC","set:menu_SelectClass.class_description.text.htmlText=localized","find:menu_SelectClass.btn_left","set:menu_SelectClass.btn_left._visible=false","find:menu_SelectClass.btn_right","set:menu_SelectClass.btn_right._visible=true","CurrentClass:KnightPlayerBase","display:_root.menu_SelectClass.class_select","focus:menu_SelectClass"};
 for(const char* failure:failures){
  Fixture f;f.enter(e);f.records.fail=failure;require(!f.deliver("menu_SelectClass",e),"expected injected rejection");
  const std::string failed_leaf=failure;
  const auto reason=failed_leaf=="find:menu_SelectClass.class_title.text"||failed_leaf=="find:menu_SelectClass.class_description.text"?
   std::string("Required original class text field absent at ")+failed_leaf.substr(5)+" (injected "+failed_leaf+")":
   failed_leaf=="find:menu_SelectClass.btn_left"||failed_leaf=="find:menu_SelectClass.btn_right"?
   std::string("Required class selector button absent"):"injected "+failed_leaf;
  require(e==reason,"actual source failure reason retained: "+failed_leaf+" got "+e);
  require(f.manager->failed()&&f.stack()->count==4,"failed source stack prefix retained");
  const bool transferred=std::find(f.records.calls.begin(),f.records.calls.end(),"BaseShow:SelectClass")!=f.records.calls.end();
  require(f.avatar.slot==(transferred?-1:3)&&f.selected()->select160==(transferred?3:-1)&&f.preview_owner.destroyed_==transferred,"failed slot state matches exact completed prefix");
  const auto calls=f.records.calls;const int deliveries=f.records.deliveries;
  f.records.fail.clear();require(!dispatch_source_app_touch_v121(0,7,120,240,e),"failed dispatch cannot replay");
  require(e=="EventManager cannot replay a failed delivery prefix; first failure: "+reason,"secondary banner identifies first failure");
  require(f.records.calls==calls&&f.records.deliveries==deliveries&&f.stack()->count==4,"no further callback or state append");
  require(authored_character_menu->fields.current_event==0,"failed Show event scope released");
  require(!front_ui.impl_->process_class_select_active_v87||std::string(failure)=="focus:menu_SelectClass","failed Show active flag retains completed prefix");
 }
 {
  Fixture f;f.enter(e);f.records.old_missing_show=true;require(!f.deliver("menu_SelectClass",e),"historical missing Show reproduced");
  require(e=="Required actual selected Show override for menu_SelectClass","historical first failure");
  require(!dispatch_source_app_touch_v121(0,7,120,240,e),"historical failure latch");
  require(e=="EventManager cannot replay a failed delivery prefix; first failure: Required actual selected Show override for menu_SelectClass","historical secondary failure explanation");
 }
 {
  Fixture f;f.enter(e);f.selected()->fields.valid7c=0;
  require(model_renderer::native_process_derived_menu_show_v119(104,202,e),e);
  require(f.selected()->select160==3&&f.avatar.slot==-1,"invalid menu still transfers source slot");
  require(f.records.calls==std::vector<std::string>{"DestroyScene","DestroyAvatarCamera"},"validity gate follows teardown before base/Front");
 }
 {
  Fixture f;require(!model_renderer::native_process_derived_menu_show_v119(104,203,e),"foreign render rejected");
  require(f.records.calls.empty()&&f.avatar.slot==3,"foreign identity rejection precedes teardown");
  require(!front_ui.process_class_select_show_v87(203,e),"Front rejects foreign primary2");
  require(!front_ui.impl_->process_class_select_active_v87,"foreign Front route cannot activate");
 }
 std::cout<<"PASS initial menu_splash Continue hit-test with global gate false; ordinary slot-2 menu remains gated; current App touch -> EventManager -> native navigation -> real MenuStack/qualified MenuBase -> EnterName/SelectClass Show/CurrentClass; "<<failures.size()<<" failure prefixes; historical missing Show; "<<checks<<" checks\n";
 return 0;
 }catch(const std::exception& x){std::cerr<<"FAIL "<<x.what()<<'\n';return 1;}}
