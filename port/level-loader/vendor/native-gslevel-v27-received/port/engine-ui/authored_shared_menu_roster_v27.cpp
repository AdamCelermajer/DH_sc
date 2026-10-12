#include "authored_shared_menu_roster_v27.hpp"
#include <cstring>
namespace dh2::ui {
struct AuthoredSharedMenuRosterV27::Receiver {
 SwfMovie* movie{};std::shared_ptr<void> lease;std::uint32_t flags{};
 AuthoredMenuFieldsV1 fields;AuthoredCharacterStateV1 state;
 MenuStackCharacterV1 character{};std::string path;
 AuthoredMenuCharacterProjectionV4 projection;
 std::uint8_t zones_registered{};std::vector<AuthoredMenuDeadZoneV3> zones;
 SwfAsGraph* active{};
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
AuthoredSharedMenuRosterV27::~AuthoredSharedMenuRosterV27()=default;
auto AuthoredSharedMenuRosterV27::receiver(std::uintptr_t id)noexcept->Receiver*{
 for(auto& r:receivers_)if(r->fields.identity==id)return r.get();return nullptr;
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
    auto owned=std::make_unique<Receiver>();auto* r=owned.get();r->movie=&c.movie;r->lease=c.lease;r->flags=c.flags;r->path=entry.path;
    r->fields.identity=reinterpret_cast<std::uintptr_t>(r);r->fields.name=entry.name;r->state.fields=&r->fields;
    // Retain the reached constructor/catalog prefix before registration.
    self.receivers_.push_back(std::move(owned));
    AuthoredCharacterRegistrationServicesV1 registration;registration.owner=r->lease;registration.render=reinterpret_cast<std::uintptr_t>(&c.movie);
    registration.append_state=[&self,r](auto& f,auto& e){return self.stack_->register_menu_live_v27(f.identity,f.render,f.name,&r->character,0,0,e);};
    registration.find=[&index](const char* name,auto& out,bool& found,auto& e){return index.find(name,out,found,e);};
    registration.bind_weak_context=[r,&graph](auto&,const auto& value,auto& e){gameswf::as_object* object{};return graph.borrow_object(value,object,e)&&authored_menu_stack_character_v4(object,r->flags,{},r->character,e);};
    registration.create=[](auto&,auto&){return true;}; // original MenuBase.Create literal return
    r->state.resolve_context=[r](auto& value,auto& e){if(!r->active){e="Required same active movie scope for weak MenuBase resolution";return false;}SwfAsValue root;return r->active->root_value(root,e)&&r->active->find_target(root,r->path.c_str(),value,e)&&value.kind()==SwfAsValue::Kind::object;};
    r->active=&graph;const bool registered=authored_character_register_state_v1(graph,r->state,nullptr,registration,e);r->active=nullptr;
    if(!registered)return false;
    auto* projected=self.stack_->registered_menu_v27(r->fields.identity);if(projected)projected->valid_menu=r->fields.valid7c;
   }return true;
  }} call{*this,movie,std::move(lease),flags};return movie.menu_action_script(&call,Call::run,error);
}
bool AuthoredSharedMenuRosterV27::lifecycle(Receiver& r,const AuthoredMenuRequestV1& q,std::int32_t& result,std::string& error){
 auto& fields=*services_.fields;
 if(q.operation==AuthoredMenuOperationV1::debug_load||q.operation==AuthoredMenuOperationV1::debug_query)return services_.debug(q.text,result,error);
 if(q.operation==AuthoredMenuOperationV1::store_rollover_event_enabled){fields.rollover()=std::uint8_t(q.value);return true;}
 if(q.operation==AuthoredMenuOperationV1::store_application_ec){fields.application_ec()=std::uint8_t(q.value);return true;}
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
