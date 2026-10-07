#include "authored_menu_lifecycle_v1.hpp"
namespace dh2::ui {
namespace {
bool deliver(AuthoredMenuFieldsV1& m,const AuthoredMenuLifecycleServicesV1& s,
 AuthoredMenuOperationV1 op,std::string& e,const char* text=nullptr,std::int32_t value=0,std::int32_t* result=nullptr){
 if(!s.owner||!s.invoke){e="Required original MenuBase lifecycle service unavailable";return false;}
 std::int32_t out=0;if(!s.invoke(m,{op,text,value},out,e))return false;
 if(result)*result=out;
 return true;
}
bool debug(AuthoredMenuFieldsV1& m,const AuthoredMenuLifecycleServicesV1& s,std::string& e){
 return deliver(m,s,AuthoredMenuOperationV1::debug_load,e)&&deliver(m,s,AuthoredMenuOperationV1::debug_query,e,"isTracingMenuBase");
}
bool visible(AuthoredMenuFieldsV1& m,const AuthoredMenuLifecycleServicesV1& s,bool value,std::string& e){
 if(!m.valid7c)return true; // SetVisible repeats the virtual IsValidMenu gate.
 // SetVisible performs actual weak-character resolution and native visible
 // mutation; only after delivered do we publish its source MenuBase byte74.
 if(!deliver(m,s,AuthoredMenuOperationV1::set_visible,e,nullptr,value))return false;
 m.visible74=std::uint8_t(value);return true;
}
}
bool authored_menu_show_v1(AuthoredMenuFieldsV1& m,const AuthoredMenuLifecycleServicesV1& s,std::string& e){
 if(!m.valid7c)return true;
 if(!debug(m,s,e)||!deliver(m,s,AuthoredMenuOperationV1::store_rollover_event_enabled,e,nullptr,1))return false;
 if(!m.localized75&&!deliver(m,s,AuthoredMenuOperationV1::localize,e))return false;
 if(!visible(m,s,true,e)||!deliver(m,s,AuthoredMenuOperationV1::invoke_as,e,"onPush"))return false;
 m.counter78=0;
 if(!deliver(m,s,AuthoredMenuOperationV1::store_application_ec,e,nullptr,m.name=="menu_VerificationLoading"))return false;
 if(m.name=="menu_language"||m.name=="menu_splash")if(!deliver(m,s,AuthoredMenuOperationV1::clear_loading_screen,e))return false;
 // Retain original comparisons (4255b4..425628), including their order.
 if(m.name=="menu_Ingame"||m.name=="menu_playlist"||m.name=="menu_Merchant")
  if(!deliver(m,s,AuthoredMenuOperationV1::store_igm_opened,e,nullptr,1))return false;
 if(m.name=="menu_Options"&&!deliver(m,s,AuthoredMenuOperationV1::option_custom_level_running,e,"option_Custom"))return false;
 return deliver(m,s,AuthoredMenuOperationV1::register_deadzones,e,"deadzone_");
}
bool authored_menu_hide_v1(AuthoredMenuFieldsV1& m,const AuthoredMenuLifecycleServicesV1& s,std::string& e){
 if(!m.valid7c)return true;
 if(!debug(m,s,e))return false;
 if(m.drag5c&&!deliver(m,s,AuthoredMenuOperationV1::reset_drag_positions,e))return false;
 if(m.name=="menu_CharacterMenu"||m.name=="menu_Merchant")
  if(!deliver(m,s,AuthoredMenuOperationV1::clear_manager_60,e))return false;
 if(m.name=="menu_language"){
  std::int32_t language=0;
  if(!deliver(m,s,AuthoredMenuOperationV1::get_saved_language,e,nullptr,0,&language))return false;
  if(static_cast<std::uint32_t>(language)>7u){
   if(!deliver(m,s,AuthoredMenuOperationV1::set_saved_language,e,nullptr,0)||!deliver(m,s,AuthoredMenuOperationV1::save_settings,e))return false;
  }
 }
 if(m.name=="menu_Ingame"||m.name=="menu_playlist"||m.name=="menu_Merchant")
  if(!deliver(m,s,AuthoredMenuOperationV1::store_igm_opened,e,nullptr,0))return false;
 if(m.name=="menu_VerificationLoading"&&!deliver(m,s,AuthoredMenuOperationV1::store_application_ec,e,nullptr,0))return false;
 return visible(m,s,false,e)&&deliver(m,s,AuthoredMenuOperationV1::unregister_listener,e)&&deliver(m,s,AuthoredMenuOperationV1::invoke_as,e,"onPop");
}
}
