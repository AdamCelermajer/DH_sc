#include "source_input_manager_v60.hpp"
#include "application_player_manager_bootstrap_v59.hpp"

namespace dh2::input {
bool SourceInputManagerV60::get_gamepad(std::int32_t index,
 std::shared_ptr<const GamepadC1V60>& out,std::string& error){
 out.reset();
 // The valid source domain is0..3. Do not reproduce undefined negative
 // pointer arithmetic or original debug-assert behavior outside that domain.
 if(index<0||index>=gamepad_countc_){error="InputManager gamepad index outside constructed slots";return false;}
 auto owner=weak_from_this().lock();
 if(!owner){error="Required retained source InputManager before gamepad borrow";return false;}
 out=std::shared_ptr<const GamepadC1V60>(std::move(owner),&gamepads_[std::size_t(index)]);
 error.clear();return true;
}
bool SourceInputManagerV60::count_service(void* context,std::int32_t& out,std::string& error){
 if(!context){error="Required actual InputManager receiver";return false;}
 out=static_cast<SourceInputManagerV60*>(context)->gamepad_count();error.clear();return true;
}
bool SourceInputManagerV60::first_gamepad_service(void* context,
 player::FirstLocalControllerBorrowV59& out,std::string& error){
 out={};
 if(!context){error="Required actual InputManager.GetGamepad receiver";return false;}
 auto& self=*static_cast<SourceInputManagerV60*>(context);
 std::shared_ptr<const GamepadC1V60> gamepad;
 if(!self.get_gamepad(0,gamepad,error))return false;
 // This is a lease of the SAME embedded receiver, not a copy of byte758.
 out.identity=reinterpret_cast<std::uintptr_t>(gamepad.get());
 out.connected758=&gamepad->connected758;
 out.receiver=std::shared_ptr<void>(self.shared_from_this(),
   const_cast<GamepadC1V60*>(gamepad.get()));
 error.clear();return true;
}
player::FirstLocalControllerServicesV59 SourceInputManagerV60::first_local_services(){
 return {shared_from_this(),this,count_service,first_gamepad_service};
}
bool SourceInputManagerV60::get_first_connected_gamepad_v107(std::shared_ptr<const GamepadC1V60>& out,std::string& e){
 out.reset();for(std::int32_t index=0;index<gamepad_countc_;++index){
  std::shared_ptr<const GamepadC1V60> candidate;if(!get_gamepad(index,candidate,e)||!candidate){if(e.empty())e="Required actual Gamepad758 receiver";return false;}
  if(candidate->connected758)return get_gamepad(index,out,e); //source reload virtual8.
 }
 e.clear();return true; //source NULL at34d818, not a fake connected receiver.
}
}
