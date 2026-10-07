#include "authored_shared_menu_roster_v27.hpp"
#include <menu_fs_command_v114.hpp>
#include <cstring>
#include <algorithm>
#include "gameswf/gameswf_character.h"
namespace dh2::ui {
struct AuthoredSharedMenuRosterV27::Receiver {
 SwfMovie* movie{};std::shared_ptr<void> lease;std::uint32_t flags{};
 AuthoredMenuFieldsV1 fields;AuthoredCharacterStateV1 state;
 MenuStackCharacterV1 character{};std::string path;
 AuthoredMenuCharacterProjectionV4 projection;
 std::unique_ptr<AuthoredMenuDragV68> drag5c;
 std::uint8_t zones_registered{};std::vector<AuthoredMenuDeadZoneV3> zones;
 SwfAsGraph* active{};
 bool retired_v104{};
 bool scoped(void* raw,bool(*fn)(void*,SwfAsGraph&,std::string&),std::string& e){
  if(active)return fn(raw,*active,e);
  struct Call {Receiver& receiver;void* raw;bool(*fn)(void*,SwfAsGraph&,std::string&);
   static bool run(void* p,SwfAsGraph& graph,std::string& error){auto& c=*static_cast<Call*>(p);
    struct Restore {Receiver& r;SwfAsGraph* prior;~Restore(){r.active=prior;}} restore{c.receiver,c.receiver.active};
    c.receiver.active=&graph;return c.fn(c.raw,graph,error);}} call{*this,raw,fn};
  return movie->menu_action_script(&call,Call::run,e);
 }
};
AuthoredSharedMenuRosterV27::AuthoredSharedMenuRosterV27(std::shared_ptr<MenuStackOwnerV1> stack,AuthoredSharedMenuServicesV27 s):stack_(std::move(stack)),services_(std::move(s)){}
struct AuthoredSharedMenuRosterV27::FSLifetimeV114 {AuthoredSharedMenuRosterV27* roster{};};
AuthoredSharedMenuRosterV27::~AuthoredSharedMenuRosterV27(){
 if(fs_lifetime_v114_)fs_lifetime_v114_->roster=nullptr;
 fs_services_v114_.reset();fs_lifetime_v114_.reset();
}
bool AuthoredSharedMenuRosterV27::bind_fs_command_services_v114(MenuFSCommandServicesV114 services,std::string& e){
 if(!stack_||!services.owner||!services.current){e="Required actual roster/FS primitive authority";return false;}
 if(!services.current(e))return false;
 if(!fs_lifetime_v114_){fs_lifetime_v114_=std::make_shared<FSLifetimeV114>();fs_lifetime_v114_->roster=this;}
 const auto weak=std::weak_ptr<FSLifetimeV114>(fs_lifetime_v114_);
 if(!services.capture_menu)services.capture_menu=[weak](std::uintptr_t id,std::shared_ptr<void>& pin,std::string& e){
  auto life=weak.lock();if(!life||!life->roster){e="Retired actual shared MenuBase roster";return false;}
  std::function<bool(std::uintptr_t,bool&,std::string&)> visibility;
  return life->roster->capture_receiver_v104(id,pin,visibility,e);
 };
 if(!services.virtual30)services.virtual30=[weak](std::uintptr_t id,const char* command,const char* args,std::string& e){
  auto life=weak.lock();if(!life||!life->roster){e="Retired actual MenuBase virtual30";return false;}
  return life->roster->on_fs_command_v114(id,command,args,e);
 };
 if(!services.virtual38)services.virtual38=[weak](std::uintptr_t id,const char* command,const char* args,bool& handled,std::string& e){
  auto life=weak.lock();if(!life||!life->roster){e="Retired actual MenuBase virtual38";return false;}
  return life->roster->my_fs_command_v114(id,command,args,handled,e);
 };
 fs_services_v114_=std::make_shared<MenuFSCommandServicesV114>(std::move(services));e.clear();return true;
}
bool AuthoredSharedMenuRosterV27::my_fs_command_v114(std::uintptr_t id,const char* command,const char* args,bool& handled,std::string& e){
 auto services=fs_services_v114_;if(!services){e="Required bound actual MenuBase FS services";return false;}
 std::shared_ptr<void> receiver_pin;std::function<bool(std::uintptr_t,bool&,std::string&)> visibility;
 if(!capture_receiver_v104(id,receiver_pin,visibility,e))return false;
 auto* actual=stack_->registered_menu_v27(id);if(!actual){e="Required SAME registered MenuBase FS projection";return false;}
 std::shared_ptr<void> menu_pin,render_pin;if(!stack_->capture_storage_v101(actual,menu_pin,render_pin,e))return false;
 // Copy the admitted projection before callbacks can mutate its directory.
 // Its string/render storage stays pinned through the source terminal call.
 auto loan=*actual;return menu_base_my_fs_command_v114(loan,command,args,*services,handled,e);
}
bool AuthoredSharedMenuRosterV27::on_fs_command_v114(std::uintptr_t id,const char* command,const char* args,std::string& e){
 auto services=fs_services_v114_;if(!services){e="Required bound actual MenuBase OnFS services";return false;}
 return menu_base_on_fs_command_v114(id,command,args,*services,e);
}
bool AuthoredSharedMenuRosterV27::render_fs_command_v114(std::uintptr_t id,const char* command,const char* args,std::string& e){
 auto services=fs_services_v114_;auto* render=stack_?stack_->render(id):nullptr;
 if(!services||!render){e="Required SAME actual MenuFX FS services/render";return false;}
 return menu_fx_fs_command_v114(*render,command,args,*services,e);
}
bool AuthoredSharedMenuRosterV27::manager_fs_command_v114(const char* command,const char* args,std::string& e){
 auto services=fs_services_v114_;auto* stack=stack_?stack_->view():nullptr;
 if(!services||!stack){e="Required SAME actual MenuManager FS services/directory";return false;}
 return menu_manager_fs_command_v114(*stack,command,args,*services,e);
}
auto AuthoredSharedMenuRosterV27::receiver(std::uintptr_t id)noexcept->Receiver*{
 for(auto& r:receivers_)if(r->fields.identity==id)return r.get();return nullptr;
}
AuthoredMenuFieldsV1* AuthoredSharedMenuRosterV27::receiver_fields_v59(std::uintptr_t id)noexcept{
 auto* r=receiver(id);return r?&r->fields:nullptr;
}
const std::string* AuthoredSharedMenuRosterV27::receiver_path_v93(std::uintptr_t id)noexcept{
 auto* r=receiver(id);return r?&r->path:nullptr;
}
bool AuthoredSharedMenuRosterV27::capture_receiver_v104(std::uintptr_t id,std::shared_ptr<void>& pin,
 std::function<bool(std::uintptr_t,bool&,std::string&)>& visible,std::string& e){
 pin.reset();visible={};
 for(const auto& record:receivers_)if(record->fields.identity==id){
  if(record->retired_v104){e="Captured MenuBase already completed native deletion";return false;}
  pin=record;std::weak_ptr<Receiver> weak=record;
  visible=[weak](std::uintptr_t expected,bool& out,std::string& e){
   auto record=weak.lock();
   if(!record||record->retired_v104||record->fields.identity!=expected){e="Required same live captured MenuBase IsVisible receiver";return false;}
   out=record->fields.visible74!=0;e.clear();return true;
  };e.clear();return true;
 }
 e="Required actual shared MenuBase receiver lifetime";return false;
}
bool AuthoredSharedMenuRosterV27::update_receiver_v59(std::uintptr_t id,std::string& error){
 auto* r=receiver(id);
 if(!r){error="Required actual shared MenuBase Update receiver";return false;}
 struct Call {Receiver& receiver;
  static bool run(void* raw,SwfAsGraph& graph,std::string& e){auto& r=static_cast<Call*>(raw)->receiver;
   return r.state.weak_character_v59.update(graph,r.fields,e);}} call{*r};
 return r->scoped(&call,Call::run,error);
}
bool AuthoredSharedMenuRosterV27::register_drag_and_drops_v68(std::uintptr_t id,std::string& error){
 auto* r=receiver(id);if(!r){error="Required registered MenuBase.RegisterDragAndDrops receiver";return false;}
 std::int32_t ignored{};if(!services_.debug||!services_.debug(nullptr,ignored,error)||!services_.debug("isTracingMenuBase",ignored,error))return false;
 //423e24 destroys the previous owner before testing its weak menu context.
 r->drag5c.reset();r->fields.drag5c=0;
 struct Register {AuthoredSharedMenuRosterV27& self;Receiver& r;
  static bool run(void* raw,SwfAsGraph& graph,std::string& e){auto& q=*static_cast<Register*>(raw);SwfAsValue value;bool live{};
   if(!q.r.state.weak_character_v59.borrow(graph,value,live,e))return false;
   if(!live)return true; //source NULL/expired weak branch creates no DragAndDrop
   q.r.drag5c=std::make_unique<AuthoredMenuDragV68>();q.r.fields.drag5c=reinterpret_cast<std::uintptr_t>(q.r.drag5c.get());
   AuthoredMenuCharacterBorrowV3* root{};if(!q.r.projection.root(graph,q.r.fields.name,root,e))return false;
   std::vector<AuthoredMenuCharacterBorrowV3*> dragables;
   if(!authored_menu_collect_characters_v3(root,"dragable",0,dragables,e))return false;
   for(auto* actual:dragables){
    //The source searches this actual draggable's subtree for the first
    //draglimits character; NULL original filter accepts every draggable.
    std::vector<AuthoredMenuCharacterBorrowV3*> children;
    if(!authored_menu_collect_characters_v3(actual,nullptr,0,children,e))return false;
    AuthoredMenuCharacterBorrowV3* limits{};
    for(auto* child:children)if(std::strstr(child->name,"draglimits")){limits=child;break;}
    if(!q.r.drag5c->add_drag(graph,q.r.fields.render,*actual,limits,q.r.projection,q.self.services_.screen_dimensions,e))return false;
   }
   std::vector<AuthoredMenuCharacterBorrowV3*> dropables;
   if(!authored_menu_collect_characters_v3(root,"drop",0,dropables,e))return false;
   for(auto* actual:dropables)if(!q.r.drag5c->add_drop(graph,q.r.fields.render,*actual,e))return false;
   if(q.r.drag5c->empty()){
    std::int32_t ignored{};if(!q.self.services_.debug(nullptr,ignored,e)||!q.self.services_.debug("isTracingMenuBase",ignored,e))return false;
    q.r.drag5c.reset();q.r.fields.drag5c=0;
   }return true;
  }} call{*this,*r};return r->scoped(&call,Register::run,error);
}
bool AuthoredSharedMenuRosterV27::hide_receiver_v68(std::uintptr_t id,std::string& error){
 auto* r=receiver(id);if(!r){error="Required actual MenuBase.Hide receiver";return false;}
 AuthoredMenuLifecycleServicesV1 s{services_.owner,[this,r](auto&,const auto& q,auto& out,auto& e){return lifecycle(*r,q,out,e);}};
 return authored_menu_hide_v1(r->fields,s,error);
}
bool AuthoredSharedMenuRosterV27::native_event_v68(std::uintptr_t id,SwfEvent48& event,std::string& error){
 auto* r=receiver(id);if(!r){error="Required actual generated MenuBase.OnEvent receiver";return false;}
 struct Call {AuthoredSharedMenuRosterV27& self;Receiver& r;SwfEvent48& event;
  static bool run(void* raw,SwfAsGraph& graph,std::string& e){auto& q=*static_cast<Call*>(raw);SwfAsValue value;gameswf::as_object* object{};
   if(!graph.retain_object(reinterpret_cast<gameswf::as_object*>(q.event.character),value,e)||!graph.borrow_object(value,object,e)||!object||!object->is(gameswf::character::m_class_id)){if(e.empty())e="Required same native event movie character";return false;}
   MenuNativeEventV1 native;native.rollover_enabled=q.self.services_.fields->rollover()!=0;native.drag_bound=q.r.fields.drag5c!=0;native.render_bound=q.r.fields.render!=0;
   for(const auto& zone:q.r.zones)native.dead_zones.push_back({zone.xmin,zone.xmax,zone.ymin,zone.ymax});
   MenuNativeEventServicesV1 services;services.context=&q;
   services.drag_event=[](void* raw,auto& event,auto& e){auto& q=*static_cast<Call*>(raw);if(!q.r.drag5c){e="Missing SAME drag5c receiver";return false;}return q.r.drag5c->event(event,e);};
   services.can_mouse=[](void*,auto id,bool& out,auto&){out=reinterpret_cast<gameswf::character*>(id)->can_handle_mouse_event();return true;};
   services.focus=[](void* raw,auto id,auto& e){auto& q=*static_cast<Call*>(raw);return q.r.movie->source_event_focus_v68(id,static_cast<std::uint32_t>(q.event.cursor),e);};
   services.raw_position=[](void* raw,int& x,int& y,auto& e){auto& q=*static_cast<Call*>(raw);return q.r.movie->source_raw_event_position_v68(q.event.cursor,x,y,e);};
   services.consume=[](void* raw,auto&){static_cast<Call*>(raw)->event.consumed=1;return true;};
   services.browser=[](void*,const char*,auto& e){e="Required original nativeOpenBrowser provider";return false;};
   return native.base(q.event,services,e);
  }} call{*this,*r,event};return r->scoped(&call,Call::run,error);
}
bool AuthoredSharedMenuRosterV27::delete_receiver_v59(std::uintptr_t id,std::string& error){
 auto it=std::find_if(receivers_.begin(),receivers_.end(),[&](const auto& r){return r->fields.identity==id;});
 if(it==receivers_.end()){error="Required actual shared MenuBase deleting receiver";return false;}
 auto& r=**it;
 if(r.fields.render||!r.fields.owned7d||r.active){error="MenuBase deletion requires detached owned receiver outside active movie Scope";return false;}
 if(r.fields.drag5c&&r.fields.drag5c!=reinterpret_cast<std::uintptr_t>(r.drag5c.get())){error="Foreign actual DragAndDrop owner during MenuBase deletion";return false;}
 r.drag5c.reset();r.fields.drag5c=0;
 // Generated MenuBase C1 zeros5c/60/64/68. This roster has no native slide
 // allocation producer; concrete menu/drag overrides cannot use this path.
 r.zones.clear();r.projection=AuthoredMenuCharacterProjectionV4{};
 r.state.weak_character_v59.reset();r.state.resolve_context={};
 auto* projection=stack_->registered_menu_v27(id);
 if(projection){projection->character=nullptr;projection->saved_focus=nullptr;}
 r.retired_v104=true;receivers_.erase(it);return true;
}
bool AuthoredSharedMenuRosterV27::post_load(SwfMovie& movie,std::shared_ptr<void> lease,std::uint32_t flags,std::string& error){
 if(!stack_||!services_.owner||!services_.fields||!services_.debug||!lease){error="Required real shared movie/MenuManager/PostLoad providers";return false;}
 const auto render_id=reinterpret_cast<std::uintptr_t>(&movie);
 if(!stack_->render(render_id)){error="Required same movie RenderFX registration before PostLoad";return false;}
 struct Call {AuthoredSharedMenuRosterV27& self;SwfMovie& movie;std::shared_ptr<void> lease;std::uint32_t flags;
  static bool run(void* p,SwfAsGraph& graph,std::string& e){auto& c=*static_cast<Call*>(p);auto& self=c.self;
   SwfAsValue root;if(!graph.root_value(root,e))return false;AuthoredMenuSearchIndexV1 index;if(!index.initialize(graph,root,e))return false;
   for(const auto& entry:index.entries()){
    if(entry.name.find("menu_")==std::string::npos)continue;
    std::int32_t ignored{};if(!self.services_.debug("isTracingMenuManager",ignored,e))return false;
    bool exists=self.stack_->menu(entry.name.c_str())!=nullptr;
    for(const auto& prior:self.receivers_)if(prior->fields.name==entry.name)exists=true;
    if(exists){if(!self.services_.debug("isTracingMenuManager",ignored,e))return false;continue;}
    auto owned=std::make_shared<Receiver>();auto* r=owned.get();r->movie=&c.movie;r->lease=c.lease;r->flags=c.flags;r->path=entry.path;
    r->fields.identity=reinterpret_cast<std::uintptr_t>(r);r->fields.name=entry.name;r->state.fields=&r->fields;
    r->fields.owned7d=1; // PostLoad42f298; MenuBase C1 leaves it zero.
    // Retain the reached constructor/catalog prefix before registration.
    self.receivers_.push_back(std::move(owned));
    AuthoredCharacterRegistrationServicesV1 registration;registration.owner=r->lease;registration.render=reinterpret_cast<std::uintptr_t>(&c.movie);
    registration.append_state=[&self,r](auto& f,auto& e){return self.stack_->register_menu_live_v27(f.identity,f.render,f.name,&r->character,0,0,e);};
    registration.find=[&index](const char* name,auto& out,bool& found,auto& e){return index.find(name,out,found,e);};
    registration.bind_weak_context=[r,&graph](auto&,const auto& value,auto& e){gameswf::as_object* object{};return graph.borrow_object(value,object,e)&&authored_menu_stack_character_v4(object,r->flags,{},r->character,e);};
    registration.create=[](auto&,auto&){return true;}; // original MenuBase.Create literal return
    r->state.resolve_context=[r](auto& value,auto& e){if(!r->active){e="Required same active movie scope for weak MenuBase resolution";return false;}bool live{};
     if(!r->state.weak_character_v59.borrow(*r->active,value,live,e))return false;
     if(!live){e="Expired original registered MenuBase weak character";return false;}return true;};
    r->active=&graph;const bool registered=authored_character_register_state_v1(graph,r->state,nullptr,registration,e);r->active=nullptr;
    if(!registered)return false;
    auto* projected=self.stack_->registered_menu_v27(r->fields.identity);if(projected)projected->valid_menu=r->fields.valid7c;
   }return true;
  }} call{*this,movie,std::move(lease),flags};return movie.menu_action_script(&call,Call::run,error);
}
bool AuthoredSharedMenuRosterV27::lifecycle(Receiver& r,const AuthoredMenuRequestV1& q,std::int32_t& result,std::string& error){
 auto& fields=*services_.fields;
 if(q.operation==AuthoredMenuOperationV1::clear_loading_screen||q.operation==AuthoredMenuOperationV1::get_saved_language||
    q.operation==AuthoredMenuOperationV1::set_saved_language||q.operation==AuthoredMenuOperationV1::save_settings){
  if(!services_.process_lifecycle){error="Required actual process MenuBase startup/settings lifecycle";return false;}
  return services_.process_lifecycle(r.fields,q,result,error);
 }
 if(q.operation==AuthoredMenuOperationV1::reset_drag_positions){
  if(!r.drag5c||r.fields.drag5c!=reinterpret_cast<std::uintptr_t>(r.drag5c.get())){error="Required SAME MenuBase drag5c owner";return false;}
  struct Reset {Receiver& r;static bool run(void* raw,SwfAsGraph& graph,std::string& e){auto& r=static_cast<Reset*>(raw)->r;return r.drag5c->reset_positions(graph,e);}} reset{r};
  return r.scoped(&reset,Reset::run,error);
 }
 if(q.operation==AuthoredMenuOperationV1::debug_load||q.operation==AuthoredMenuOperationV1::debug_query)return services_.debug(q.text,result,error);
 if(q.operation==AuthoredMenuOperationV1::store_rollover_event_enabled){fields.rollover()=std::uint8_t(q.value);return true;}
 if(q.operation==AuthoredMenuOperationV1::store_application_ec){fields.store_application_ec_v58(std::uint8_t(q.value));return true;}
 if(q.operation==AuthoredMenuOperationV1::store_igm_opened){fields.igm_opened()=std::uint8_t(q.value);return true;}
 if(q.operation==AuthoredMenuOperationV1::clear_manager_60){fields.manager60()=0;return true;}
 if(q.operation==AuthoredMenuOperationV1::unregister_listener)return fields.unregister_listener(r.fields.identity,[this](auto& e){std::int32_t value{};return services_.debug(nullptr,value,e);},error);
 struct Call {AuthoredSharedMenuRosterV27& self;Receiver& r;const AuthoredMenuRequestV1& q;
  static bool run(void* p,SwfAsGraph& graph,std::string& e){auto& c=*static_cast<Call*>(p);
   if(c.q.operation==AuthoredMenuOperationV1::register_deadzones){AuthoredMenuCharacterBorrowV3* root{};
    if(!c.r.projection.root(graph,c.r.fields.name,root,e))return false;AuthoredMenuDeadZoneServicesV3 s;
    s.debug=[&](auto& de){std::int32_t value{};return c.self.services_.debug(nullptr,value,de);};
    s.bounds=[&](auto& character,auto& out,auto& be){return c.r.projection.absolute_bounds(character,out,be);};
    return authored_menu_register_deadzones_v3(c.r.zones_registered,root,s,c.r.zones,e);
   }
   auto localized=c.self.services_.localization;
   localized.set_context=[&](const auto& value,auto& le){gameswf::as_object* object{};if(!graph.borrow_object(value,object,le))return false;
    auto* render=c.self.stack_->render(c.r.fields.render);if(!render||c.r.character.identity!=reinterpret_cast<std::uintptr_t>(object)){le="Required same shared MenuFX localization context";return false;}render->context=&c.r.character;return true;};
   return authored_character_movie_operation_v1(graph,c.r.state,c.q,localized,e);
  }} call{*this,r,q};return r.scoped(&call,Call::run,error);
}
bool AuthoredSharedMenuRosterV27::route(MenuStackV1&,MenuStackRequestV1& q,bool& handled,std::string& error){
 auto* r=q.menu?receiver(q.menu->identity):nullptr;handled=r!=nullptr;if(!handled)return true;
 switch(q.operation){
 case MenuStackOperationV1::menu_valid:q.result=r->fields.valid7c;return true;
 case MenuStackOperationV1::menu_focus:case MenuStackOperationV1::menu_blur:return true; // literal MenuBase methods
 case MenuStackOperationV1::menu_show:case MenuStackOperationV1::menu_hide:{
  AuthoredMenuLifecycleServicesV1 s{services_.owner,[this,r](auto&,const auto& request,auto& out,auto& e){return lifecycle(*r,request,out,e);}};
  return q.operation==MenuStackOperationV1::menu_show?authored_menu_show_v1(r->fields,s,error):authored_menu_hide_v1(r->fields,s,error);
 }
 case MenuStackOperationV1::invoke_as:return invoke(r->fields.identity,q.text,error);
 case MenuStackOperationV1::menu_set_visible:{std::int32_t ignored{};return lifecycle(*r,{AuthoredMenuOperationV1::set_visible,nullptr,std::int32_t(q.value)},ignored,error);}
 case MenuStackOperationV1::play_animation:{
  struct Call {Receiver& r;const char* name;bool accepted{};
   static bool run(void* raw,SwfAsGraph& graph,std::string& e){auto& c=*static_cast<Call*>(raw);SwfAsValue value;return c.r.state.resolve_context(value,e)&&authored_menu_play_animation_v4(graph,value,c.name,c.accepted,e);}} call{*r,q.text};
  if(!r->scoped(&call,Call::run,error))return false;q.result=call.accepted;return true;
 }
 default:handled=false;return true;
 }
}
int AuthoredSharedMenuRosterV27::dispatch(void* raw,MenuStackV1* stack,MenuStackRequestV1* q){
 auto& self=*static_cast<AuthoredSharedMenuRosterV27*>(raw);if(!stack||!q)return -1;bool handled{};
 if(!self.route(*stack,*q,handled,self.failure_))return -2;if(handled)return 0;
 if(self.services_.remaining&&self.services_.remaining(*stack,*q,self.failure_))return 0;
 if(self.failure_.empty())self.failure_="Required actual shared MenuManager service "+std::to_string(unsigned(q->operation));return -2;
}
bool AuthoredSharedMenuRosterV27::invoke(std::uintptr_t id,const char* method,std::string& error){auto* r=receiver(id);std::int32_t ignored{};if(!r){error="Required actual registered shared MenuBase";return false;}return lifecycle(*r,{AuthoredMenuOperationV1::invoke_as,method,0},ignored,error);}
bool AuthoredSharedMenuRosterV27::check(std::uintptr_t id,std::string& error){auto* r=receiver(id);if(!r){error="Required actual shared MenuBase weak receiver";return false;}
 struct Call {Receiver& r;static bool run(void* raw,SwfAsGraph&,std::string& e){auto& c=*static_cast<Call*>(raw);SwfAsValue value;return c.r.state.resolve_context(value,e);}} call{*r};return r->scoped(&call,Call::run,error);
}
}
