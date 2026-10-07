#include "player_save_write_owner_v1.hpp"
#include <stdexcept>
namespace dh2::data {
PlayerSaveWriteOwnerV1::PlayerSaveWriteOwnerV1(std::shared_ptr<PlayerSaveLoadOwnerV1> authority,PlayerSaveWriteServicesV1 services):authority_(std::move(authority)),services_(std::move(services)){
 if(!authority_)throw std::invalid_argument("Required same Save/profile authority unavailable");
}
bool PlayerSaveWriteOwnerV1::send(PlayerSaveWriteOpV1 operation,PlayerSaveWriteResponseV1& response,std::string& error,bool first,bool second){
 phase_=std::uint32_t(operation)+1;++calls_;response={};
 const auto authority=authority_;const auto services=services_;
 if(!services.owner||!services.invoke){error="Genuine Save writer service required at phase "+std::to_string(phase_);return false;}
 PlayerSaveWriteRequestV1 request{operation,authority.get(),&authority->save(),authority->profile(),first,second};
 if(!services.invoke(request,response,error)){if(error.empty())error="Required Save writer delivery failed at phase "+std::to_string(phase_);return false;}
 return true;
}
bool PlayerSaveWriteOwnerV1::volatile_quests(std::string& error){
 PlayerSaveWriteResponseV1 response;
 if(!send(PlayerSaveWriteOpV1::online,response,error))return false;
 if(!response.flag)return true;
 if(!send(PlayerSaveWriteOpV1::local_hosting,response,error))return false;
 if(response.flag){if(!send(PlayerSaveWriteOpV1::hosting_quest_flag,response,error))return false;if(!response.flag)return true;}
 return send(PlayerSaveWriteOpV1::pack_volatile_quests,response,error);
}
bool PlayerSaveWriteOwnerV1::save(std::string& error){
 if(running_){error="Unsupported recursive Save writer callback";return false;}
 running_=true;struct Guard{bool& value;~Guard(){value=false;}}guard{running_};
 error.clear();phase_=calls_=0;
 if(!authority_->profile().identity||authority_->save_disabled_)return true;
 PlayerSaveWriteResponseV1 response;bool mode_two=false;
 if(!send(PlayerSaveWriteOpV1::online,response,error))return false;
 if(response.flag){
  if(!send(PlayerSaveWriteOpV1::local_hosting,response,error))return false;
  if(response.flag){if(!send(PlayerSaveWriteOpV1::hosting_quest_flag,response,error))return false;mode_two=!response.flag;}
 }
 authority_->save_mode_=mode_two?2:1;
 if(!send(PlayerSaveWriteOpV1::online,response,error))return false;
 if(response.flag){
  if(!send(PlayerSaveWriteOpV1::local_hosting,response,error))return false;
  bool synchronize=!response.flag;
  if(response.flag){if(!send(PlayerSaveWriteOpV1::hosting_quest_flag,response,error))return false;synchronize=response.flag;}
  if(synchronize){
   if(!send(PlayerSaveWriteOpV1::synchronize,response,error,true))return false;
   const bool changed=response.flag;
   if(!send(PlayerSaveWriteOpV1::setup_sections,response,error,true,changed))return false;
   if(!send(PlayerSaveWriteOpV1::profile_has_cache,response,error))return false;
   if(!response.flag&&!send(PlayerSaveWriteOpV1::cache_profile,response,error))return false;
   if(!send(PlayerSaveWriteOpV1::save_all,response,error))return false;
   if(!send(PlayerSaveWriteOpV1::setup_sections,response,error,false,changed))return false;
   if(changed){if(!send(PlayerSaveWriteOpV1::delete_level_checkpoint,response,error)||!send(PlayerSaveWriteOpV1::delete_player_checkpoint,response,error))return false;}
   return volatile_quests(error);
  }
 }
 if(!send(PlayerSaveWriteOpV1::save_all,response,error))return false;
 return volatile_quests(error);
}
}
