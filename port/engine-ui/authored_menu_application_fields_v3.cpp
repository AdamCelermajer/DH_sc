#include "authored_menu_application_fields_v3.hpp"
namespace dh2::ui {
bool AuthoredMenuApplicationFieldsV3::bind_application_ec_v58(
 std::shared_ptr<void> owner,std::uint8_t* byte,std::string& error){
 if(!owner||!byte){error="Required retained actual Application+ec byte";return false;}
 if(application_ec_v58_&&(application_ec_v58_!=byte||
    application_owner_v58_.owner_before(owner)||owner.owner_before(application_owner_v58_))){
  error="MenuManager Application+ec already belongs to another source owner";return false;
 }
 application_owner_v58_=std::move(owner);application_ec_v58_=byte;
 application_ec_=*byte;return true;
}
void AuthoredMenuApplicationFieldsV3::store_application_ec_v58(std::uint8_t value)noexcept{
 if(application_ec_v58_)*application_ec_v58_=value;
 application_ec_=value;
}
bool AuthoredMenuApplicationFieldsV3::globals(
 const std::function<bool(bool&,std::string&)>& drm,MenuStackGlobalsV1*& out,std::string& error){
 out=nullptr;bool value{};
 if(!drm||!drm(value,error)){
  if(error.empty())error="Required source native DRM platform query";return false;
 }
 globals_.use_native_drm=value;out=&globals_;return true;
}
bool AuthoredMenuApplicationFieldsV3::register_listener(std::uintptr_t receiver,std::string& error){
 //431750 performs actual map insert/update even for a NULL MenuBase key.
 //Confirm.Hide legitimately passes NULL from an empty legacy GetMenuBelow.
 listeners_[receiver]=true;error.clear();return true;
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
