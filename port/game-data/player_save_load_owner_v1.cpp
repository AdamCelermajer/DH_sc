#include "player_save_load_owner_v1.hpp"
#include <stdexcept>
namespace dh2::data {
PlayerSaveLoadOwnerV1::PlayerSaveLoadOwnerV1(std::shared_ptr<PlayerSavegameV1> save,PlayerSaveLoadServicesV1 services):save_(std::move(save)),services_(std::move(services)){
 if(!save_)throw std::invalid_argument("Save load requires the same retained Save authority");
}
bool PlayerSaveLoadOwnerV1::publish_profile(PlayerSaveProfileV1 profile,std::string& error){
 if(destroy_attempted_v108_){error="Profile publication after PlayerSavegame D0 began";return false;}
 if(bool(profile.identity)!=bool(profile.owner)){error="Source profile identity and lifetime lease disagree";return false;}
 profile_=std::move(profile);return true;
}
bool PlayerSaveLoadOwnerV1::bind_services_v50(PlayerSaveLoadServicesV1 services,std::string& error){
 if(destroy_attempted_v108_){error="Save provider binding after PlayerSavegame D0 began";return false;}
 if(delivery_depth_v50_){error="Cannot replace SAME Save platform services during source delivery";return false;}
 if(!services.owner||!services.invoke){error="Required retained actual Save platform services";return false;}
 services_=std::move(services);error.clear();return true;
}
bool PlayerSaveLoadOwnerV1::send(PlayerSaveLoadRequestV1 request,PlayerSaveLoadResponseV1& response,std::string& error){
 if(destroy_attempted_v108_){error="Save load callback after PlayerSavegame D0 began";return false;}
 phase_=std::uint32_t(request.operation)+1;++calls_;request.save=save_.get();response={};
 const auto retained=save_;const auto services=services_;
 if(!services.owner||!services.invoke){error="Genuine Save load service required at phase "+std::to_string(phase_);return false;}
 ++delivery_depth_v50_;struct Delivery{std::uint32_t& depth;~Delivery(){--depth;}}delivery{delivery_depth_v50_};
 if(!services.invoke(request,response,error)){if(error.empty())error="Required Save load delivery failed at phase "+std::to_string(phase_);return false;}
 return true;
}
bool PlayerSaveLoadOwnerV1::initialize(PlayerSaveLoadOpV1 op,std::uint32_t index,std::string& error){
 PlayerSaveLoadRequestV1 request{op};request.argument=index;PlayerSaveLoadResponseV1 response;return send(request,response,error);
}
bool PlayerSaveLoadOwnerV1::section(const char* name,const PlayerSaveProfileV1& profile,bool reader,std::string& error){
 PlayerSaveLoadRequestV1 request{PlayerSaveLoadOpV1::load_section};request.profile=profile;request.section=name;request.reader_enabled=reader;
 PlayerSaveLoadResponseV1 response;return send(request,response,error);
}
bool PlayerSaveLoadOwnerV1::load_fields(std::uint32_t mask,std::string& error){
 if(!profile_.identity&&save_->slot()!=-1){
  PlayerSaveLoadRequestV1 request{PlayerSaveLoadOpV1::filename};request.argument=std::uint32_t(save_->slot());PlayerSaveLoadResponseV1 response;
  if(!send(request,response,error))return false;
  const auto filename=response.text;
  request={PlayerSaveLoadOpV1::create_profile};request.filename=filename.c_str();
  if(!send(request,response,error)||!response.profile.identity){if(error.empty())error="Source Savegame constructor did not publish a real profile";return false;}
  if(!publish_profile(std::move(response.profile),error))return false;
 }
 // Each source block has one null guard. Subsequent calls reread the actual
 // profile field, even if a synchronous earlier section replaced/cleared it.
 if((mask&1)&&profile_.identity){
  for(const char* name:{"PNAM","PLVL","PCLS","PDFL","LNAM","LEPT","LUSP"})if(!section(name,profile_,true,error))return false;
 }
 if(mask&2){
  if(!initialize(PlayerSaveLoadOpV1::init_levels,0,error)||!initialize(PlayerSaveLoadOpV1::init_skills,0,error)||
     !initialize(PlayerSaveLoadOpV1::init_faeries,0,error)||!initialize(PlayerSaveLoadOpV1::init_quests,0,error)||
     !initialize(PlayerSaveLoadOpV1::init_quests,1,error))return false;
 }
 if((mask&4)&&profile_.identity){
  for(const char* name:{"LVLS","SKIL","FAES"})if(!section(name,profile_,true,error))return false;
  const auto captured=profile_;
  PlayerSaveLoadResponseV1 response;
  if(!send({PlayerSaveLoadOpV1::online},response,error))return false;
  bool reader=false;
  if(response.flag){if(!send({PlayerSaveLoadOpV1::hosting_quest_flag},response,error))return false;reader=!response.flag;}
  if(!section("CFEE",captured,reader,error))return false;
  for(const char* name:{"QEST","PROP","GEAR","FTVL"})if(!section(name,profile_,true,error))return false;
 }
 if((mask&8)&&profile_.identity&&!section("SKIL",profile_,true,error))return false;
 if((mask&0x20)&&profile_.identity&&!section("PROP",profile_,true,error))return false;
 if((mask&0x10)&&profile_.identity){
  if(!initialize(PlayerSaveLoadOpV1::init_quests,0,error)||!initialize(PlayerSaveLoadOpV1::init_quests,1,error)||!section("QEST",profile_,true,error))return false;
 }
 return true;
}
bool PlayerSaveLoadOwnerV1::load_volatile(std::uint32_t mask,std::string& error){
 if(!(mask&0x14))return true;
 PlayerSaveLoadResponseV1 response;
 if(!send({PlayerSaveLoadOpV1::online},response,error))return false;
 if(!response.flag)return true;
 if(!send({PlayerSaveLoadOpV1::local_hosting},response,error))return false;
 if(response.flag){if(!send({PlayerSaveLoadOpV1::load_volatile_flag},response,error))return false;if(!response.flag)return true;}
 if(!send({PlayerSaveLoadOpV1::volatile_stream},response,error))return false;
 const auto stream=response.profile;
 if(!stream.identity||!stream.owner){error="Actual volatile quest stream and lease required";return false;}
 PlayerSaveLoadRequestV1 request{PlayerSaveLoadOpV1::stream_size};request.profile=stream;
 if(!send(request,response,error))return false;
 if(!response.amount)return true;
 request.operation=PlayerSaveLoadOpV1::stream_seek;request.argument=0;
 if(!send(request,response,error))return false;
 if(!send({PlayerSaveLoadOpV1::quest_definition},response,error))return false;
 request.operation=PlayerSaveLoadOpV1::unpack_quests;request.argument=0;request.definition=response.value;
 return send(request,response,error);
}
bool PlayerSaveLoadOwnerV1::load(std::int32_t mask,std::string& error){
 if(destroy_attempted_v108_){error="Save load after PlayerSavegame D0 began";return false;}
 error.clear();phase_=0;calls_=0;
 // Preserve the original bit mask, including negative masks. SG_Load always
 // finishes _Load first and only then reaches _LoadVolatileQuestsLog.
 return load_fields(std::uint32_t(mask),error)&&load_volatile(std::uint32_t(mask),error);
}
bool PlayerSaveLoadOwnerV1::destroy_source_v108(
 const std::function<bool(const PlayerSaveProfileV1&,std::string&)>& profile_d0,
 const std::function<bool(std::uint32_t,std::string&)>& quests_d1,std::string& error){
 if(destroy_complete_v108_){error.clear();return true;}
 if(delivery_depth_v50_){error="PlayerSavegame D0 during live source load/write delivery";return false;}
 if(destroy_attempted_v108_){error=destroy_error_v108_.empty()?"PlayerSavegame D0 cannot replay a failed prefix":destroy_error_v108_;return false;}
 destroy_attempted_v108_=true;
 const auto fail=[&]{destroy_error_v108_=error.empty()?"Required actual PlayerSavegame D0 body":error;error=destroy_error_v108_;return false;};
 try{
  if(profile_.identity){
   const auto actual=profile_;
   if(!actual.owner||!profile_d0||!profile_d0(actual,error))return fail();
   if(profile_.identity!=actual.identity||profile_.owner.get()!=actual.owner.get()||profile_.owner.owner_before(actual.owner)||actual.owner.owner_before(profile_.owner)){
    error="Save+8 changed inside its original profile D0";return fail();
   }
   profile_={}; //4637cc, only after positive profile D0 completed.
  }else if(profile_.owner){error="PlayerSavegame profile8 NULL has a foreign retained alias";return fail();}
  if(!save_||!save_->destroy_fields_v108(quests_d1,error))return fail();
  services_={};destroy_complete_v108_=true;error.clear();return true;
 }catch(const std::exception& ex){error=ex.what();return fail();}
 catch(...){error="PlayerSavegame D0 provider threw; source prefix retained";return fail();}
}
}
