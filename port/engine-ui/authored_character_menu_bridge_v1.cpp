#include "authored_character_menu_bridge_v1.hpp"
#include <cstring>
#include <utility>
namespace dh2::ui {
AuthoredCharacterMenuBridgeV1::AuthoredCharacterMenuBridgeV1(AuthoredCharacterMenuBindingsV1 bindings)
 :bindings_(std::move(bindings)),bridge_(bindings_.owner,[this](const char* name,CharacterMenuCallV1& call,std::string& error){return route(name,call,error);}){}
bool AuthoredCharacterMenuBridgeV1::route(const char* name,CharacterMenuCallV1& call,std::string& error){
 if(!bindings_.owner){error="Authored character menu requires its retained game owner";return false;}
 switch(route_for(name)){
 case AuthoredCharacterMenuRouteV1::query:
  if(bindings_.queries)return bindings_.queries->dispatch(name,call,error);
  break;
 case AuthoredCharacterMenuRouteV1::reload:
  if(bindings_.reload)return bindings_.reload->dispatch(name,call,error);
  break;
 case AuthoredCharacterMenuRouteV1::navigation:
  if(bindings_.navigation)return bindings_.navigation->dispatch(name,call,error);
  break;
 case AuthoredCharacterMenuRouteV1::rollover:
  if(bindings_.rollover)return bindings_.rollover->dispatch(call,error);
  break;
 case AuthoredCharacterMenuRouteV1::application:
  if(bindings_.application)return bindings_.application(name,call,error);
  break;
 }
 error=std::string("Required authored character menu owner unavailable: ")+(name?name:"<null>");return false;
}
bool AuthoredCharacterMenuBridgeV1::dispatch(const char* name,const gameswf::fn_call& call,std::string& error)const{return bridge_.dispatch(name,call,error);}
}
