#include "authored_menu_touchscreen_v3.hpp"
#include <vector>
namespace dh2::ui {
bool AuthoredMenuTouchScreenV3::reset(
 const std::function<bool(std::int16_t,std::int16_t,std::int32_t,std::string&)>& release,std::string& error){
 std::vector<std::int32_t> snapshot;
 for(std::int32_t i=0;i<8;++i)if(active28_[i])snapshot.push_back(i);
 for(auto id:snapshot){
  if(!release||!release(-1,-1,id,error)){
   if(error.empty())error="Required Application ResetTouch source release";return false;
  }
 }
 return true;
}
bool AuthoredMenuTouchScreenV3::process(
 const std::function<bool(AuthoredMenuTouchScreenV3&,std::string&)>& nonempty,std::string& error){
 if(empty())return true; // actual33c5d4→33c74c branch, no discarded events
 if(head1a0_>=16||tail1a4_>=16){error="Invalid source TouchScreen queue indices";return false;}
 if(!nonempty){error="Required whole nonempty TouchScreenBase ProcessEvents";return false;}
 return nonempty(*this,error);
}
}
