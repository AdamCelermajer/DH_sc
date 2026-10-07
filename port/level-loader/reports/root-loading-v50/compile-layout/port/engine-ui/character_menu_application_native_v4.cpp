#include "character_menu_application_v4.hpp"
#include "character_menu_as_bridge_v1.hpp"
namespace dh2::ui {
bool character_menu_application_native_v4(const char* name,const gameswf::fn_call& call,const CharacterMenuApplicationServicesV4& services,bool& handled,std::string& error){
 if(!services.owner){handled=false;error="Required native menu application lease";return false;}
 CharacterMenuAsBridgeV1 bridge(services.owner,[&](const char* callback,CharacterMenuCallV1& projected,std::string& e){return character_menu_application_call_v4(callback,projected,services,handled,e);});
 return bridge.dispatch(name,call,error);
}
}
