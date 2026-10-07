#include "authored_character_application_v1.hpp"
#include <cstring>
#include <stdexcept>
namespace dh2::ui {
struct AuthoredCharacterApplicationV1::Invocation {AuthoredCharacterApplicationV1& owner;CharacterMenuCallV1& call;};
AuthoredCharacterApplicationV1::AuthoredCharacterApplicationV1(std::shared_ptr<void> owner,HudStartupState48& state,HudStartupServices16 services):owner_(std::move(owner)),state_(state),services_(services){if(!owner_||!services_.invoke)throw std::invalid_argument("Authored startup requires retained actual Application services");}
bool AuthoredCharacterApplicationV1::owns(const char* name)noexcept{return name&&(!std::strcmp(name,"NativeLoadSettings")||!std::strcmp(name,"NativeIsMultiplayerEnabled"));}
int AuthoredCharacterApplicationV1::service(void* context,HudStartupState48* state,const HudStartupRequest40* request,HudStartupResponse16* response){
 auto& invocation=*static_cast<Invocation*>(context);auto& self=invocation.owner;
 if(state!=&self.state_||!request||!response)return -1;
 if(request->operation==HudStartupOperation::set_result_bool){
  if(request->subject!=reinterpret_cast<std::uintptr_t>(&invocation.call))return -1;
  invocation.call.result=CharacterMenuValueV1::flag(request->argument!=0);*response={};return 0;
 }
 return self.services_.invoke(self.services_.context,state,request,response);
}
bool AuthoredCharacterApplicationV1::dispatch(const char* name,CharacterMenuCallV1& call,std::string& error){
 if(!owns(name)){error="Unowned authored Application callback";return false;}
 const auto lease=owner_;Invocation invocation{*this,call};HudStartupServices16 services{&invocation,service};
 const auto previous=state_.result;state_.result=reinterpret_cast<std::uintptr_t>(&call);
 struct Restore{HudStartupState48& state;std::uintptr_t result;~Restore(){state.result=result;}}restore{state_,previous};
 const auto status=!std::strcmp(name,"NativeLoadSettings")?dh2_hud_load_settings(&state_,&services):dh2_hud_is_multiplayer_enabled(&state_,&services);
 if(status){error=std::string("Required source authored Application callback failed: ")+name+" status="+std::to_string(status);return false;}
 return true;
}
}
