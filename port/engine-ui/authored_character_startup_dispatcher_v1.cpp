#include "authored_character_startup_dispatcher_v1.hpp"
#include <algorithm>
namespace dh2::ui {
AuthoredCharacterStartupDispatcherV1::AuthoredCharacterStartupDispatcherV1(AuthoredCharacterStartupBindingsV1 bindings):bindings_(std::move(bindings)),bridge_(bindings_.owner,[this](const char* name,auto& call,auto& error){return route(name,call,error);}){}
bool AuthoredCharacterStartupDispatcherV1::dispatch(const char* name,const gameswf::fn_call& call,std::string& error)const{return bridge_.dispatch(name,call,error);}
bool AuthoredCharacterStartupDispatcherV1::route(const char* name,CharacterMenuCallV1& call,std::string& error){
 if(!name){error="Missing authored callback name";return false;}
 if(std::find(reached_.begin(),reached_.end(),name)==reached_.end())reached_.emplace_back(name);
 auto missing=[&](const char* owner){error=std::string("Reached original ")+name+" requires actual "+owner;return false;};
 switch(AuthoredCharacterMenuBridgeV1::route_for(name)){
 case AuthoredCharacterMenuRouteV1::query:return bindings_.queries?bindings_.queries(name,call,error):missing("same-player query owner");
 case AuthoredCharacterMenuRouteV1::reload:return bindings_.reload?bindings_.reload->dispatch(name,call,error):missing("whole native reload owner");
 case AuthoredCharacterMenuRouteV1::navigation:return bindings_.navigation?bindings_.navigation->dispatch(name,call,error):missing("same menu stack");
 case AuthoredCharacterMenuRouteV1::rollover:return bindings_.rollover?bindings_.rollover->dispatch(call,error):missing("same RenderFX input owner");
 case AuthoredCharacterMenuRouteV1::application:
  if(AuthoredCharacterApplicationV1::owns(name))return bindings_.application?bindings_.application->dispatch(name,call,error):missing("Application startup owner");
  return bindings_.required_application?bindings_.required_application(name,call,error):missing("application callback owner");
 }
 return missing("callback route");
}
}
