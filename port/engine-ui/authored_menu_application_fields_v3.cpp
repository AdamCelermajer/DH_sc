#include "authored_menu_application_fields_v3.hpp"
namespace dh2::ui {
bool AuthoredMenuApplicationFieldsV3::globals(
 const std::function<bool(bool&,std::string&)>& drm,MenuStackGlobalsV1*& out,std::string& error){
 out=nullptr;bool value{};
 if(!drm||!drm(value,error)){
  if(error.empty())error="Required source native DRM platform query";return false;
 }
 globals_.use_native_drm=value;out=&globals_;return true;
}
bool AuthoredMenuApplicationFieldsV3::register_listener(std::uintptr_t receiver,std::string& error){
 if(!receiver){error="Required actual MenuBase listener identity";return false;}
 listeners_[receiver]=true;return true;
}
bool AuthoredMenuApplicationFieldsV3::unregister_listener(std::uintptr_t receiver,
 const std::function<bool(std::string&)>& debug,std::string& error){
 if(!debug||!debug(error)){
  if(error.empty())error="Required MenuManager UnRegisterListener Debug service";return false;
 }
 auto found=listeners_.find(receiver);
 if(found!=listeners_.end())found->second=false;
 return true;
}
}
