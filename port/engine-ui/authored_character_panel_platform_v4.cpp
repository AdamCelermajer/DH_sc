#include "authored_character_panel_platform_v4.hpp"
#include <utility>
namespace dh2::ui {
AuthoredCharacterPanelPlatformV4::AuthoredCharacterPanelPlatformV4(AuthoredCharacterPanelPlatformServicesV4 s):services_(std::move(s)){}
bool AuthoredCharacterPanelPlatformV4::bind(AuthoredCharacterPanelServicesV2& panel,std::string& error){
 if(bound_){error="Character panel platform already bound";return false;}
 if(!services_.owner||!services_.fields||!services_.touch||!services_.scoped_graph){error="Required same native MenuManager/Application/TouchScreen/Scope owners";return false;}
 MenuStackGlobalsV1* globals=nullptr;
 if(!services_.fields->globals(services_.native_drm,globals,error))return false;
 panel.globals=globals;
 panel.lifecycle.owner=services_.owner;
 panel.lifecycle.invoke=[this](AuthoredMenuFieldsV1& f,const AuthoredMenuRequestV1& r,std::int32_t& result,std::string& e){return lifecycle(f,r,result,e);};
 panel.remaining_stack={this,stack_callback};bound_=true;return true;
}
const std::vector<AuthoredMenuDeadZoneV3>* AuthoredCharacterPanelPlatformV4::deadzones(std::uintptr_t identity)const noexcept{
 auto p=deadzones_.find(identity);return p==deadzones_.end()?nullptr:&p->second.rectangles;
}
bool AuthoredCharacterPanelPlatformV4::lifecycle(AuthoredMenuFieldsV1& f,const AuthoredMenuRequestV1& r,std::int32_t& result,std::string& error){
 auto& fields=*services_.fields;
 switch(r.operation){
 case AuthoredMenuOperationV1::debug_load:
 case AuthoredMenuOperationV1::debug_query:
  if(services_.debug)return services_.debug(r.text,result,error);
  error="Required actual MenuBase Debug producer";return false;
 case AuthoredMenuOperationV1::store_rollover_event_enabled:fields.rollover()=static_cast<std::uint8_t>(r.value);return true;
 case AuthoredMenuOperationV1::store_application_ec:fields.store_application_ec_v58(static_cast<std::uint8_t>(r.value));return true;
 case AuthoredMenuOperationV1::store_igm_opened:fields.igm_opened()=static_cast<std::uint8_t>(r.value);return true;
 case AuthoredMenuOperationV1::clear_manager_60:fields.manager60()=0;return true;
 case AuthoredMenuOperationV1::unregister_listener:
  return fields.unregister_listener(f.identity,[this](std::string& e){std::int32_t value{};if(services_.debug)return services_.debug(nullptr,value,e);e="Required actual listener Debug producer";return false;},error);
 case AuthoredMenuOperationV1::register_deadzones:{
  if(!f.identity){error="Required actual MenuBase identity for dead zones";return false;}
  auto& zones=deadzones_[f.identity];
  struct Call {AuthoredCharacterPanelPlatformV4& p;AuthoredMenuFieldsV1& f;DeadZones& zones;
   static bool run(void* raw,SwfAsGraph& graph,std::string& e){auto& c=*static_cast<Call*>(raw);auto& s=c.p.services_;AuthoredMenuCharacterBorrowV3* root=nullptr;
    if(!s.deadzone_root||!s.deadzone_root(graph,c.f.name,root,e)){if(e.empty())e="Required actual scoped MenuBase display list";return false;}
    AuthoredMenuDeadZoneServicesV3 services;
    services.debug=[&](std::string& de){std::int32_t value{};if(s.debug)return s.debug(nullptr,value,de);de="Required actual RegisterDeadZones Debug producer";return false;};
    services.bounds=[&](AuthoredMenuCharacterBorrowV3& character,AuthoredMenuDeadZoneV3& out,std::string& be){if(s.absolute_bounds)return s.absolute_bounds(graph,character,out,be);be="Required source absolute bounding rectangle producer";return false;};
    return authored_menu_register_deadzones_v3(c.zones.registered,root,services,c.zones.rectangles,e);
   }} call{*this,f,zones};
  return services_.scoped_graph(&call,Call::run,error);
 }
 default:
  if(services_.required_lifecycle.invoke)return services_.required_lifecycle.invoke(f,r,result,error);
  error="Required native MenuBase lifecycle continuation";return false;
 }
}
int AuthoredCharacterPanelPlatformV4::stack_callback(void* raw,MenuStackV1* stack,MenuStackRequestV1* request){
 if(!raw||!stack||!request)return -1;
 auto& p=*static_cast<AuthoredCharacterPanelPlatformV4*>(raw);p.failure_.clear();bool delivered=false;
 switch(request->operation){
 case MenuStackOperationV1::debug_message:
 case MenuStackOperationV1::debug_load:
 case MenuStackOperationV1::debug_switch:{std::int32_t result{};
  if(p.services_.debug){delivered=p.services_.debug(request->text,result,p.failure_);request->result=static_cast<std::uintptr_t>(result);}
  else p.failure_="Required actual MenuManager Debug producer";
  break;
 }
 case MenuStackOperationV1::register_listener:
  delivered=p.services_.fields->register_listener(request->menu?request->menu->identity:request->result,p.failure_);break;
 case MenuStackOperationV1::unregister_listener:
  delivered=p.services_.fields->unregister_listener(request->menu?request->menu->identity:request->result,[&](std::string& e){std::int32_t value{};if(p.services_.debug)return p.services_.debug(nullptr,value,e);e="Required actual listener Debug producer";return false;},p.failure_);break;
 case MenuStackOperationV1::reset_touch:delivered=p.services_.touch->reset(p.services_.release_touch,p.failure_);break;
 case MenuStackOperationV1::process_touch:delivered=p.services_.touch->process(p.services_.process_touch,p.failure_);break;
 default:
  if(p.services_.required_stack)delivered=p.services_.required_stack(*stack,*request,p.failure_);
  else p.failure_="Required native MenuManager stack continuation";
 }
 return delivered?0:-2;
}
}
