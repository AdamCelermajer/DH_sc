#include "authored_character_panel_v2.hpp"
#include "authored_character_menu_bridge_v1.hpp"
#include "authored_menu_character_projection_v4.hpp"
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wself-assign"
#pragma clang diagnostic ignored "-Wdeprecated-copy-with-user-provided-copy"
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#pragma clang diagnostic ignored "-Wmismatched-tags"
#pragma clang diagnostic ignored "-Wnew-returns-null"
#pragma clang diagnostic ignored "-Wignored-qualifiers"
#endif
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_sprite.h"
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <limits>
namespace dh2::ui {
namespace {
#include "reference/character-movie-startup-v1/root_menu_catalog_v1.inc"
constexpr const char* tabs[]{"btnCharacterSheet","btnInventoryTab","btnSkillTreeTab","btnFaeriesTab"};
}
struct AuthoredCharacterPanelV2::Impl {
 struct Provider {Impl* runtime{};};
 AuthoredCharacterPanelServicesV2 services;
 std::shared_ptr<Provider> provider=std::make_shared<Provider>();
 std::shared_ptr<MenuStackOwnerV1> stack;
 std::unique_ptr<MenuStackActionsV1> navigation;
 std::unique_ptr<CharacterMenuAsBridgeV1> bridge;
 std::unique_ptr<CharacterMenuMovieV1> movie;
 std::array<AuthoredMenuFieldsV1,20> fields;
 std::array<AuthoredCharacterStateV1,20> states;
 std::array<MenuStackCharacterV1,20> characters{};
 std::array<bool,20> deleted_v59{};
 MenuStackCharacterV1 root{};
 std::uintptr_t render{};std::string failure;bool loaded{};
 int pointer_id{-1};
 Impl(){provider->runtime=this;}
 ~Impl(){movie.reset();bridge.reset();navigation.reset();provider->runtime=nullptr;}
 static bool native(void* p,const char* name,const gameswf::fn_call& call,std::string& error){auto& r=*static_cast<Provider*>(p)->runtime;
  struct Guard {Impl& r;SwfAsGraph* prior;~Guard(){r.active_graph=prior;}} guard{r,r.active_graph};if(!r.active_graph)r.active_graph=r.retained_graph;
  const auto route=AuthoredCharacterMenuBridgeV1::route_for(name);
  if(route==AuthoredCharacterMenuRouteV1::navigation||route==AuthoredCharacterMenuRouteV1::query||route==AuthoredCharacterMenuRouteV1::reload){const bool delivered=r.bridge->dispatch(name,call,error);if(!delivered&&!r.failure.empty())error=r.failure;return delivered;}
  if(!r.services.movie.native_action){error=std::string("Required authored character native application: ")+name;return false;}
  return r.services.movie.native_action(r.services.movie.context,name,call,error);
 }
 static int stack_service(void* p,MenuStackV1* stack,MenuStackRequestV1* request){auto& r=*static_cast<Impl*>(p);if(!request)return -1;
  auto found=std::find_if(r.fields.begin(),r.fields.end(),[&](const auto& f){return request->menu&&f.identity==request->menu->identity;});
  const auto op=request->operation;
  if(found!=r.fields.end()){
   auto& f=*found;
   // PostLoad42f284..42f2a4 constructs these discovered menu_* receivers as
   // MenuBase. Its actual GotFocus41b3e8/LostFocus41b3ec are literal bx lr.
   if(op==MenuStackOperationV1::menu_focus||op==MenuStackOperationV1::menu_blur)return 0;
   if(op==MenuStackOperationV1::menu_valid){request->result=f.valid7c;return 0;}
   if(op==MenuStackOperationV1::menu_show||op==MenuStackOperationV1::menu_hide){AuthoredMenuLifecycleServicesV1 lifecycle{r.provider,[&r](auto& m,const auto& q,auto& result,auto& e){return r.lifecycle(m,q,result,e);}};
    bool delivered=op==MenuStackOperationV1::menu_show?authored_menu_show_v1(f,lifecycle,r.failure):authored_menu_hide_v1(f,lifecycle,r.failure);return delivered?0:-1;}
   if(op==MenuStackOperationV1::invoke_as||op==MenuStackOperationV1::play_animation||op==MenuStackOperationV1::menu_set_visible){
     bool accepted=false;
     const bool delivered=r.movie_operation(f,op==MenuStackOperationV1::menu_set_visible?"_visible":request->text,request->value,op==MenuStackOperationV1::play_animation,r.failure,&accepted);
     if(op==MenuStackOperationV1::play_animation)request->result=accepted;
     return delivered?0:-1;
   }
  }
  if(!r.services.remaining_stack.invoke){r.failure="Required original character menu stack service "+std::to_string(unsigned(op));return -1;}
  return r.services.remaining_stack.invoke(r.services.remaining_stack.context,stack,request);
 }
  bool movie_operation(AuthoredMenuFieldsV1& f,const char* method,std::uint32_t value,bool animation,std::string& error,bool* accepted_animation=nullptr){
   struct Call {Impl& r;AuthoredMenuFieldsV1& f;const char* method;std::uint32_t value;bool animation;bool* accepted_animation;
   static bool apply(void* p,SwfAsGraph& graph,std::string& e){auto& c=*static_cast<Call*>(p);SwfAsValue root,receiver,result;bool callable=false;
    const auto index=static_cast<std::size_t>(&c.f-c.r.fields.data());bool live{};
    if(index>=c.r.states.size()||!c.r.states[index].weak_character_v59.borrow(graph,receiver,live,e)||!live){if(e.empty())e="Expired original character menu context: "+c.f.name;return false;}
    if(c.method&&!std::strcmp(c.method,"_visible")){bool accepted=false;if(!graph.set_member(receiver,"_visible",SwfAsValue::boolean(c.value!=0),accepted,e))return false;if(!accepted){e="Original menu visibility write rejected";return false;}c.f.visible74=std::uint8_t(c.value!=0);return true;}
    if(!c.method){e="Missing original character menu method";return false;}
     if(c.animation){bool accepted=false;if(!authored_menu_play_animation_v4(graph,receiver,c.method,accepted,e))return false;
      if(c.accepted_animation)*c.accepted_animation=accepted;return true;}
     return graph.invoke(receiver,receiver,c.method,{},result,callable,e);
    }} call{*this,f,method,value,animation,accepted_animation};
  return scoped(&call,Call::apply,error);
 }
 // Native callbacks already execute in the exact owning core Scope. An
 // explicit borrowed graph is installed only around our outer calls below;
 // callback lifecycle can reuse it without reentering the facade.
 SwfAsGraph* active_graph{};SwfAsGraph* retained_graph{};
 bool scoped(void* p,bool(*apply)(void*,SwfAsGraph&,std::string&),std::string& e){if(active_graph)return apply(p,*active_graph,e);
  struct Scope {Impl& r;void* p;bool(*apply)(void*,SwfAsGraph&,std::string&);
   static bool run(void* raw,SwfAsGraph& graph,std::string& error){auto& c=*static_cast<Scope*>(raw);struct Guard {Impl& r;~Guard(){r.active_graph=nullptr;}} guard{c.r};c.r.active_graph=&graph;c.r.retained_graph=&graph;return c.apply(c.p,graph,error);}} call{*this,p,apply};return movie->action_script(&call,Scope::run,e);
 }
 bool lifecycle(AuthoredMenuFieldsV1& f,const AuthoredMenuRequestV1& q,std::int32_t& result,std::string& error){
  if(q.operation==AuthoredMenuOperationV1::set_visible)return movie_operation(f,"_visible",std::uint32_t(q.value),false,error);
  if(q.operation==AuthoredMenuOperationV1::invoke_as)return movie_operation(f,q.text,0,false,error);
  if(q.operation==AuthoredMenuOperationV1::localize){struct Local {Impl& r;AuthoredMenuFieldsV1& f;
   static bool run(void* p,SwfAsGraph& graph,std::string& e){auto& c=*static_cast<Local*>(p);SwfAsValue receiver;bool live{};
    const auto index=static_cast<std::size_t>(&c.f-c.r.fields.data());
    if(index>=c.r.states.size()||!c.r.states[index].weak_character_v59.borrow(graph,receiver,live,e))return false;
    if(!live){e="Expired original registered MenuBase localization receiver";return false;}
    return authored_menu_process_localization_v1(graph,c.f,receiver,c.r.services.localization,e);}} call{*this,f};return scoped(&call,Local::run,error);}
  if(!services.lifecycle.owner||!services.lifecycle.invoke){error="Required original character menu lifecycle service "+std::to_string(unsigned(q.operation));return false;}
  return services.lifecycle.invoke(f,q,result,error);
 }
 bool register_movie(std::string& error){struct Bind {Impl& r;
  static bool run(void* p,SwfAsGraph& graph,std::string& e){auto& r=static_cast<Bind*>(p)->r;SwfAsValue root;gameswf::as_object* object=nullptr;if(!graph.root_value(root,e)||!graph.borrow_object(root,object,e)||!object||!object->is(gameswf::sprite_instance::m_class_id)){if(e.empty())e="Original character movie root is not a sprite";return false;}
   if(!r.services.character||!r.services.character(object,r.root,e)){if(e.empty())e="Required original root character projection";return false;}
   std::uint32_t flags;if(!r.services.panel_render_flags||!r.services.panel_render_flags(flags,e)){if(e.empty())e="Required source character RenderFX flags";return false;}
    auto* render=r.stack->render(r.render);
    if(!render){e="Required published source character RenderFX";return false;}
    render->root=&r.root;
   for(unsigned i=0;i<r.fields.size();++i){auto& f=r.fields[i];f.identity=reinterpret_cast<std::uintptr_t>(&f);f.name=source_screens[i].name;f.render=r.render;r.states[i].fields=&f;
    f.owned7d=1; // Actual PostLoad42f298 generated MenuBase ownership.
    AuthoredCharacterRegistrationServicesV1 registration;
    registration.owner=r.provider;registration.render=r.render;
    registration.append_state=[&r,i](auto& m,auto& error){return r.stack->register_menu_live_v27(m.identity,r.render,m.name,&r.characters[i],0,0,error);};
    registration.find=[&graph,&root](const char* name,auto& receiver,bool& found,auto& error){if(!graph.find_target(root,(std::string("_root.")+name).c_str(),receiver,error))return false;found=receiver.kind()==SwfAsValue::Kind::object;return true;};
    registration.bind_weak_context=[&r,&graph,i](auto& m,const auto& receiver,auto& error){gameswf::as_object* live=nullptr;if(!graph.borrow_object(receiver,live,error)||!live||!live->is(gameswf::sprite_instance::m_class_id)){if(error.empty())error="Original character screen unavailable: "+m.name;return false;}return r.services.character(live,r.characters[i],error);};
    registration.create=[&r](auto& m,auto& error){if(r.services.create)return r.services.create(m,error);return true;}; // source MenuBase::Create41b3e4 = bx lr
    if(!authored_character_register_state_v1(graph,r.states[i],nullptr,registration,e))return false;
   }
   if(!r.stack->view()){
    if(!r.stack->seal(r.services.hud.identity,r.services.base.identity,*r.services.globals,e))return false;
   }else if(r.stack->view()->base_render->identity!=r.services.base.identity||r.stack->view()->hud_root->identity!=r.services.hud.identity||r.stack->view()->globals!=r.services.globals){e="Character panel shared MenuManager ownership differs";return false;}
   for(const auto& f:r.fields){auto* m=r.stack->menu(f.name.c_str());if(!m){e="Registered authored menu missing from sealed catalog: "+f.name;return false;}m->valid_menu=f.valid7c;}
   return true;
  }} call{*this};return scoped(&call,Bind::run,error);
 }
};
AuthoredCharacterPanelV2::AuthoredCharacterPanelV2()=default;
AuthoredCharacterPanelV2::~AuthoredCharacterPanelV2()=default;
bool AuthoredCharacterPanelV2::rebind_process_services_v104(const AuthoredCharacterPanelServicesV2& source,std::string& error){
 if(!impl_||!impl_->loaded||impl_->active_graph||!source.movie.native_owner||!source.queries||
    source.shared_stack_v27!=impl_->stack||source.globals!=impl_->services.globals){
  error="Character rebind requires SAME retained process movie/stack and idle graph";return false;
 }
 auto* view=impl_->stack->view();
 if(!view||!view->base_render||!view->hud_root||view->base_render->identity!=source.base.identity||
    view->hud_root->identity!=source.hud.identity){error="Character rebind requires actual new HUD/base publication";return false;}
 //Native/AS/font forwarding callbacks already read these SAME service cells.
 //Keep player, root, receiver states, movie, weak caches and navigation alive.
 impl_->services=source;error.clear();return true;
}
bool AuthoredCharacterPanelV2::initialize(const AuthoredCharacterPanelServicesV2& s,std::string& error){return initialize_impl_v98(s,nullptr,error);}
bool AuthoredCharacterPanelV2::load_source_primary1_v98(const AuthoredCharacterPanelServicesV2& s,const char* uri,std::string& error){
 if(!uri||!*uri){error="Required original primary1 URI";return false;}return initialize_impl_v98(s,uri,error);
}
bool AuthoredCharacterPanelV2::initialize_impl_v98(const AuthoredCharacterPanelServicesV2& s,const char* source_uri,std::string& error){
 if(!source_uri){
  if(!s.movie.native_owner||!s.maximum_occurrences||!s.globals||!s.hud.identity||!s.base.identity||!s.hud.root||!s.base.root){error="Required actual character menu platform/HUD/base/stack globals";return false;}
 }else{
  auto* view=s.shared_stack_v27?s.shared_stack_v27->view():nullptr;
  if(impl_||!s.source_continue_v98||!s.movie.native_owner||!s.maximum_occurrences||!s.globals||!s.queries||!s.base.identity||!s.base.root||!view||
   view->globals!=s.globals||!view->base_render||view->base_render->identity!=s.base.identity||
   (s.hud.identity?(!s.hud.root||!view->hud_root||view->hud_root->identity!=s.hud.identity):(s.hud.root||view->hud_root))){
   error="Required SAME actual primary1 process stack/base/current nullable HUD and native queries";return false;
  }
 }
 auto p=std::make_unique<Impl>();p->services=s;p->stack=s.shared_stack_v27?s.shared_stack_v27:std::make_shared<MenuStackOwnerV1>(s.maximum_occurrences);
 const auto add_render=[&](const CharacterPanelRenderBorrowV2& r){auto* existing=p->stack->render(r.identity);if(existing){if(existing->flags!=r.flags||existing->root!=r.root||existing->controller_focus!=r.focus){error="Character panel reused RenderFX ownership differs";return false;}return true;}return p->stack->register_render_live_v27(r.identity,r.flags,r.root,r.focus,error);};
 if(((!source_uri||s.hud.identity)&&!add_render(s.hud))||!add_render(s.base))return false;
 for(const auto& render:s.additional_renders)if(!add_render(render))return false;
 MenuStackActionsGraphV1 navigation;p->services=s;navigation.owner=p->provider;navigation.instance=[raw=p.get()](auto*& stack,auto&){stack=raw->stack.get();return true;};navigation.lifecycle={p.get(),Impl::stack_service};p->navigation=std::make_unique<MenuStackActionsV1>(std::move(navigation));
 p->bridge=std::make_unique<CharacterMenuAsBridgeV1>(p->provider,[raw=p.get()](const char* name,auto& call,auto& e){if(AuthoredCharacterMenuBridgeV1::route_for(name)==AuthoredCharacterMenuRouteV1::navigation)return raw->navigation->dispatch(name,call,e);if(!raw->services.queries){e=std::string("Required same-profile character query/action: ")+name;return false;}return raw->services.queries(name,call,e);});
 auto movie_services=s.movie;movie_services.context=p->provider.get();movie_services.native_owner=p->provider;movie_services.native_action=Impl::native;
 // Only native_action context changes; other platform callbacks retain their
 // original context through the forwarding source wrapper below.
 struct Forward {static Impl& r(void* p){return *static_cast<Impl::Provider*>(p)->runtime;}
  static bool read(void* p,const char* n,std::vector<std::uint8_t>& b,std::string& e){auto& s=r(p).services.movie;return s.read&&s.read(s.context,n,b,e);}
  static bool texture(void* p,const char* n,int w,int h,SwfTexture& b,std::string& e){auto& s=r(p).services.movie;return s.texture&&s.texture(s.context,n,w,h,b,e);}
  static bool image(void* p,int w,int h,unsigned c,const std::uint8_t* b,int pitch,SwfTexture& out,std::string& e){auto& s=r(p).services.movie;return s.image&&s.image(s.context,w,h,c,b,pitch,out,e);}
  static bool draw(void* p,const SwfDraw& d,std::string& e){auto& s=r(p).services.movie;return s.draw&&s.draw(s.context,d,e);}
  static bool call(void* p,const char* n,const std::vector<SwfValue>& a,SwfValue& o,std::string& e){auto& s=r(p).services.movie;return s.native_call&&s.native_call(s.context,n,a,o,e);}
  static bool stencil(void* p,const float b[4],std::uint8_t v,bool& o,std::string& e){auto& s=r(p).services.movie;return s.stencil&&s.stencil(s.context,b,v,o,e);}
  static void diagnostic(void* p,bool failure,const char* n){auto& s=r(p).services.movie;if(s.diagnostic)s.diagnostic(s.context,failure,n);}
  static bool start(void* p,const SwfAsLease& lease,std::string& e){auto& s=r(p).services.movie;return !s.graph_start||s.graph_start(s.context,lease,e);}
 };
 movie_services.read=Forward::read;movie_services.texture=Forward::texture;movie_services.image=Forward::image;movie_services.draw=Forward::draw;movie_services.native_call=Forward::call;movie_services.stencil=Forward::stencil;movie_services.diagnostic=Forward::diagnostic;movie_services.graph_start=Forward::start;
  p->render=reinterpret_cast<std::uintptr_t>(p.get());
  std::uint32_t flags{};
  //Original RenderFX C1 input flags1c=0; LoadMenu sets84 only after native Load.
  if((!source_uri&&(!p->services.panel_render_flags||!p->services.panel_render_flags(flags,error)))||!p->stack->register_render_live_v27(p->render,flags,nullptr,nullptr,error))return false;
  // Publish the retained implementation before Load executes native callbacks.
  // Lifecycle/deadzone adapters reuse this same active graph during startup.
  impl_=std::move(p);auto* runtime=impl_.get();
  //C1 facade exists before published primary1 callbacks can borrow/unload it.
  runtime->movie=std::make_unique<CharacterMenuMovieV1>();
  if(runtime->services.publish_render&&!runtime->services.publish_render(runtime->render,error))return false;
  if(source_uri&&!runtime->services.source_continue_v98(error))return false;
 // Multi.LoadSWF publishes its cached MenuFX before virtual Load and does not
 // roll the constructor/publication prefix back on a required Load failure.
 // Retain that same owner here; an application lookup must never dangle.
  if(source_uri){
   if(!runtime->movie->load_source_resource_v98(source_uri,movie_services,error))return false;
   if(!runtime->services.source_continue_v98(error))return false;
   struct Root {Impl& r;static bool run(void* raw,SwfAsGraph& graph,std::string& e){auto& r=static_cast<Root*>(raw)->r;SwfAsValue root;gameswf::as_object* actual{};
    if(!graph.root_value(root,e)||!graph.borrow_object(root,actual,e)||!actual||!actual->is(gameswf::sprite_instance::m_class_id)||!r.services.character||!r.services.character(actual,r.root,e)){
     if(e.empty())e="Required SAME source primary1 root character projection";return false;
    }
    if(!r.services.source_continue_v98(e))return false;
    auto* render=r.stack->render(r.render);if(!render){e="Source primary1 lost actual published RenderFX";return false;}render->root=&r.root;e.clear();return true;
   }} root{*runtime};
   if(!runtime->scoped(&root,Root::run,error))return false;
   if(!runtime->services.source_continue_v98(error)||!runtime->movie->source_movie_v91()->source_set_text_buffering_v98(true,error))return false;
   if(!runtime->services.source_continue_v98(error))return false;
   runtime->loaded=true;return true; //V98 caller owns virtual10/common PostLoad/DragHide.
  }
  if(!runtime->movie->load(movie_services,error)||!runtime->register_movie(error))return false;
  runtime->loaded=true;return true;
}
bool AuthoredCharacterPanelV2::open(std::string& e){if(!bound()){e="Original character panel not bound";return false;}struct Open {Impl& p;static bool run(void* raw,SwfAsGraph&,std::string& e){auto& p=static_cast<Open*>(raw)->p;p.failure.clear();const int rc=p.stack->push("menu_CharacterMenu",{&p,Impl::stack_service});if(rc){e=p.failure.empty()?"Original CharacterMenu push failed "+std::to_string(rc):p.failure;return false;}return true;}} c{*impl_};return impl_->scoped(&c,Open::run,e);}
bool AuthoredCharacterPanelV2::release(const char* path,std::string& e){if(!bound()||!path){e="Original character button path requires a bound panel";return false;}struct Call {const char* path;static bool run(void* p,SwfAsGraph& graph,std::string& e){auto path=static_cast<Call*>(p)->path;SwfAsValue root,button,result;bool callable=false;if(!graph.root_value(root,e)||!graph.find_target(root,path,button,e)||button.kind()!=SwfAsValue::Kind::object){if(e.empty())e=std::string("Original character button unavailable: ")+path;return false;}if(!graph.invoke(button,button,"onRelease",{},result,callable,e))return false;if(!callable){e=std::string("Original character onRelease unavailable: ")+path;return false;}return true;}} c{path};return impl_->scoped(&c,Call::run,e);}
bool AuthoredCharacterPanelV2::tab(unsigned i,std::string& e){if(i>=4){e="Unknown original character panel tab";return false;}return release((std::string("_root.menu_CharacterMenu.CharacterMenuTabs.")+tabs[i]).c_str(),e);}
bool AuthoredCharacterPanelV2::geometry(const char* path,float sx,float sy,AuthoredHudGeometryV1& out,std::string& error){
 if(!bound()||!path){error="Original character geometry requires a bound movie and actual path";return false;}
 float point[2]{sx,sy};if(!impl_->movie->movie()->screen_to_logical(point,error))return false;
 struct Query {const char* path;float x,y;AuthoredHudGeometryV1& out;
 static bool run(void* raw,SwfAsGraph& graph,std::string& error){auto& q=*static_cast<Query*>(raw);SwfAsValue root,value;gameswf::as_object* object=nullptr;
  if(!graph.root_value(root,error)||!graph.find_target(root,q.path,value,error)||!graph.borrow_object(value,object,error))return false;
  if(!object||!object->is(gameswf::character::m_class_id)){error="Required original character menu shape receiver";return false;}
  auto* c=static_cast<gameswf::character*>(object);auto* parent=c->get_parent();gameswf::rect bounds;c->get_bound(&bounds);if(parent)parent->get_world_matrix().transform(&bounds);
  q.out={};q.out.bounds[0]=bounds.m_x_min;q.out.bounds[1]=bounds.m_x_max;q.out.bounds[2]=bounds.m_y_min;q.out.bounds[3]=bounds.m_y_max;q.out.character_id=c->get_id();
  gameswf::point local;c->get_world_matrix().transform_by_inverse(&local,gameswf::point(q.x,q.y));q.out.local[0]=local.m_x;q.out.local[1]=local.m_y;
  const auto& matrix=c->get_matrix();for(int r=0;r<2;++r)for(int col=0;col<3;++col)q.out.local_matrix[r*3+col]=matrix.m_[r][col];
  gameswf::point pp(q.x,q.y);if(parent)parent->get_world_matrix().transform_by_inverse(&pp,gameswf::point(q.x,q.y));gameswf::character* selected=nullptr;q.out.hit=c->get_topmost_mouse_entity(selected,pp.m_x,pp.m_y);
  for(auto* a=c;a;a=a->get_parent())if(!a->get_visible())q.out.hit=false;
  return true;
 }} query{path,point[0]*20.f,point[1]*20.f,out};return impl_->scoped(&query,Query::run,error);
}
bool AuthoredCharacterPanelV2::back(std::string& e){return release("_root.menu_CharacterMenu.CharacterMenuTabs.btnBack",e);}
bool AuthoredCharacterPanelV2::pointer(int action,int pointer_id,float sx,float sy,std::string& error){
 if(!bound()||action<0||action>3||pointer_id<0||!std::isfinite(sx)||!std::isfinite(sy)){error="Invalid authored panel pointer transport";return false;}
 if(action==0&&impl_->pointer_id!=-1)return true; // source mouse has one primary owner
 if(action!=0&&impl_->pointer_id!=pointer_id)return true;
 float point[]{sx,sy};if(!impl_->movie->movie()->screen_to_logical(point,error))return false;
 for(float x:point)if(!std::isfinite(x)||double(x)<double(std::numeric_limits<int>::min())||double(x)>double(std::numeric_limits<int>::max())){error="Authored movie pointer coordinate outside native int domain";return false;}
 struct Input {Impl& p;int action,id,x,y;
  static bool run(void* raw,SwfAsGraph& graph,std::string& e){auto& c=*static_cast<Input*>(raw);SwfAsValue value;gameswf::as_object* object{};
   if(!graph.root_value(value,e)||!graph.borrow_object(value,object,e)||!object||!object->is(gameswf::sprite_instance::m_class_id)){if(e.empty())e="Required same authored menu root";return false;}
   auto* root=static_cast<gameswf::sprite_instance*>(object)->get_root();if(!root){e="Required retained GameSWF mouse root";return false;}
   if(c.action==3){
    // Host cancellation has no new target. Deliver the source outside-release
    // to the actual captured character, then stop that root's actual drag.
    auto& state=root->m_mouse_button_state;auto* captured=state.m_active_entity.get_ptr();
    if(captured)captured->on_event(gameswf::event_id::RELEASE_OUTSIDE);
    root->stop_drag();state.m_mouse_button_state_last=false;state.m_mouse_button_state_current=false;
    state.m_active_entity=nullptr;state.m_topmost_entity=nullptr;state.m_mouse_inside_entity_last=false;
    root->notify_mouse_state(c.x,c.y,0);c.p.pointer_id=-1;return true;
   }
   const auto process=[&]{
    // The recovered native Root::Advance intentionally excludes desktop
    // mouse processing. Transport explicitly runs the retained core's input
    // generator, leaving native frame scheduling/timers completely separate.
    gameswf::character* selected{};
    static_cast<gameswf::sprite_instance*>(object)->get_topmost_mouse_entity(selected,c.x*20.f,c.y*20.f);
    auto& state=root->m_mouse_button_state;state.m_topmost_entity=selected;
    state.m_mouse_button_state_current=root->m_mouse_buttons&1;state.m_x=c.x;state.m_y=c.y;
    root->generate_mouse_button_events(&state);
   };
   if(c.action==0){
    // The original mouse generator's PRESS consumes its previously selected
    // active_entity. Touch has no hover, so publish the actual up-position
    // first to run that SAME source rollover selection before the press.
    root->notify_mouse_state(c.x,c.y,0);process();
   }
   root->notify_mouse_state(c.x,c.y,c.action==1?0:1);
   if(c.action==0)c.p.pointer_id=c.id;
   process();if(c.action==1)c.p.pointer_id=-1;return true;
  }} input{*impl_,action,pointer_id,static_cast<int>(point[0]),static_cast<int>(point[1])};
 return impl_->scoped(&input,Input::run,error);
}
bool AuthoredCharacterPanelV2::advance(float seconds,std::string& e){if(!bound()){e="Original character panel not bound";return false;}return impl_->movie->advance(seconds,e);}
bool AuthoredCharacterPanelV2::connect_viewport(const ViewportState64& seed,const SwfViewportDriver& driver,std::string& e){if(!bound()){e="Original character panel not bound";return false;}return impl_->movie->movie()->connect_viewport(seed,driver,e);}
bool AuthoredCharacterPanelV2::display(int x,int y,int w,int h,std::string& e){if(!bound()){e="Original character panel not bound";return false;}return impl_->movie->display(x,y,w,h,e);}
bool AuthoredCharacterPanelV2::bound()const noexcept{return impl_&&impl_->loaded;}
MenuStackOwnerV1* AuthoredCharacterPanelV2::stack()noexcept{return bound()?impl_->stack.get():nullptr;}
MenuStackOwnerV1* AuthoredCharacterPanelV2::source_stack_v4()noexcept{return impl_?impl_->stack.get():nullptr;}
CharacterMenuMovieV1* AuthoredCharacterPanelV2::movie()noexcept{return bound()?impl_->movie.get():nullptr;}
bool AuthoredCharacterPanelV2::scoped_graph(void* context,bool(*apply)(void*,SwfAsGraph&,std::string&),std::string& error){
 if(!impl_||!apply){error="Required authored panel scoped graph callback";return false;}
 return impl_->scoped(context,apply,error);
}
AuthoredMenuFieldsV1* AuthoredCharacterPanelV2::receiver_fields_v59(std::uintptr_t id)noexcept{
 if(!impl_)return nullptr;
 for(std::size_t i=0;i<impl_->fields.size();++i)
  if(!impl_->deleted_v59[i]&&impl_->fields[i].identity==id)return &impl_->fields[i];
 return nullptr;
}
std::uintptr_t AuthoredCharacterPanelV2::source_render_identity_v91()const noexcept{return impl_?impl_->render:0;}
SwfMovie* AuthoredCharacterPanelV2::source_movie_v91()const noexcept{return impl_&&impl_->movie?impl_->movie->source_movie_v91():nullptr;}
std::shared_ptr<void> AuthoredCharacterPanelV2::source_movie_lease_v94()const noexcept{
 return impl_&&impl_->movie?impl_->movie->source_movie_lease_v94():std::shared_ptr<void>{};
}
bool AuthoredCharacterPanelV2::source_movie_virtual10_v93(std::uintptr_t expected,std::int32_t dt,bool flag,std::string& e){
 if(!impl_||impl_->render!=expected||!impl_->movie){e="Required same character1 RenderFX/source movie";return false;}
 return impl_->movie->source_virtual10_v93(dt,flag,e);
}
bool AuthoredCharacterPanelV2::deleting_movie_v93(std::uintptr_t expected,std::string& e){
 if(!impl_||impl_->render!=expected||!impl_->movie){e="Character resource D0 receiver differs from same render";return false;}
 if(!impl_->movie->deleting_movie_v93(e))return false;
 impl_->loaded=false;e.clear();return true;
}
bool AuthoredCharacterPanelV2::clear_movie_slot_v93(std::string& e){
 if(!impl_||!impl_->movie){e="Required retained character movie field owner";return false;}
 if(!impl_->movie->clear_movie_slot_v93(e))return false;
 impl_->render=0;e.clear();return true;
}
bool AuthoredCharacterPanelV2::claim_source_update_v93(std::shared_ptr<void> owner,std::string& e){
 if(!impl_||!impl_->movie){e="Required actual character timing owner";return false;}return impl_->movie->claim_source_update_v93(std::move(owner),e);
}
bool AuthoredCharacterPanelV2::release_source_update_v93(const std::shared_ptr<void>& owner,std::string& e){
 if(!impl_||!impl_->movie){e="Required actual character timing owner";return false;}return impl_->movie->release_source_update_v93(owner,e);
}
bool AuthoredCharacterPanelV2::update_receiver_v59(std::uintptr_t id,std::string& error){
 auto* field=receiver_fields_v59(id);
 if(!field){error="Required actual character-panel MenuBase Update receiver";return false;}
 const auto index=static_cast<std::size_t>(field-impl_->fields.data());
 struct Call {Impl& panel;std::size_t index;
  static bool run(void* raw,SwfAsGraph& graph,std::string& e){auto& c=*static_cast<Call*>(raw);
   return c.panel.states[c.index].weak_character_v59.update(graph,c.panel.fields[c.index],e);}} call{*impl_,index};
 return impl_->scoped(&call,Call::run,error);
}
bool AuthoredCharacterPanelV2::delete_receiver_v59(std::uintptr_t id,std::string& error){
 auto* field=receiver_fields_v59(id);
 if(!field){error="Required actual character-panel MenuBase deleting receiver";return false;}
 if(field->render||!field->owned7d||impl_->active_graph){error="MenuBase deletion requires detached owned receiver outside active movie Scope";return false;}
 if(field->drag5c){error="Required actual nonnull DragAndDrop destructor/free provider";return false;}
 const auto index=static_cast<std::size_t>(field-impl_->fields.data());
 impl_->states[index].weak_character_v59.reset();
 impl_->states[index].resolve_context={};impl_->deleted_v59[index]=true;
 auto* projection=impl_->stack->registered_menu_v27(id);
 if(projection){projection->character=nullptr;projection->saved_focus=nullptr;}
 // Fixed adapter storage stays until panel teardown, but retired source
 // MenuBase ownership cannot be looked up/reused as a surviving receiver.
 return true;
}
}
