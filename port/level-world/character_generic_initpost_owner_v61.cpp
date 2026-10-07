#include "character_generic_initpost_owner_v61.hpp"
#include "character_scene.hpp"
#include <cstring>
namespace dh2::character {
bool CharacterGenericInitPostOwnerV61::call(std::uint32_t entry,NpcInitPostResponseV1& out,std::uint32_t a,std::uint32_t b,std::uintptr_t payload,const char* text){
 last_entry_=entry;++calls_;out={};
 if(!services_.invoke||!services_.invoke(services_.context,{entry,a,b,fields_.common.identity,payload,text},out,error_)){
  if(error_.empty())error_="Required original generic Character InitPost service "+std::to_string(entry);
  failed_=true;return false;
 }return true;
}
bool CharacterGenericInitPostOwnerV61::debug(){NpcInitPostResponseV1 r;return call(0x337888,r)&&call(0x337a88,r,0,0,0,"isTracingChar_Init");}
bool CharacterGenericInitPostOwnerV61::is_player(bool& value){NpcInitPostResponseV1 r;if(!call(0x3a49f0,r))return false;value=r.value!=0;return true;}
bool CharacterGenericInitPostOwnerV61::player(){
 auto& f=fields_.common;NpcInitPostResponseV1 r;
 if(!fields_.save14e8||!fields_.store_gold3a4||!fields_.store_capacity3a8){error_="Required same player Save14e8 and embedded inventory source stores";failed_=true;return false;}
 if(*f.visual2d8&&!call(0x59719c,r,0,0,*f.visual2d8))return false;
 if(!call(0x3bc4d0,r,4)||!call(0x31f594,r))return false;
 if(r.identity&&!call(0x3bb950,r,0,0,r.identity))return false; // SG_SetGameDifficulty: actual current Level+118.
 if(!call(0x36effc,r))return false;
 if(r.value){ // PlayerManager::IsLocalPlayer36effc returns bool.
  if(!call(0x3b395c,r)||!call(0x3defac,r))return false;
  if(*fields_.save14e8&&!call(0x4679e8,r,0,0,*fields_.save14e8))return false;
 }
 const auto id=static_cast<std::uint32_t>(static_cast<std::int32_t>(*f.properties_id13c8));
 if(!call(0x3df2a4,r,id)||!call(0x3df480,r)||!call(0x3e0810,r,1)||!call(0x36effc,r))return false;
 if(r.value&&!call(0x3b3a90,r))return false;
 // Source stores this even after the positive PM callback, at3b51c4.
 if(!fields_.store_gold3a4(fields_.inventory_context,999999999,error_)){failed_=true;return false;}
 if(!call(0x3df6e0,r,194,0))return false;
 const auto nonnegative=r.value<0?0:static_cast<std::uint32_t>(r.value);
 if(!fields_.store_capacity3a8(fields_.inventory_context,static_cast<std::uint8_t>(nonnegative),error_)){failed_=true;return false;}
 //Whole original marker block tests the process VisualFXManager1c/20
 //AnimFXSetInfo vector. Its C1-empty branch3b5210→3b538c skips the
 //nine/local markers and highlight; equipment was initialized earlier.
 if(!call(0x3b5214,r))return false;
 if(!r.value)return true;
 return call(0x3a41a0,r); // AddMultiplayerHighlight, never an equipment grant.
}
bool CharacterGenericInitPostOwnerV61::initialize(){
 auto& f=fields_.common;if(failed_){if(error_.empty())error_="Failed generic InitPost prefix cannot retry";return false;}
 if(!f.identity||!f.called1394||!f.spawn_probability274||!f.starting_waypoint13e8||!f.room64||!f.properties_id13c8||!f.properties||!f.model290||!f.scale120||(!f.delayed3ec&&!f.live_delayed3ec)||!f.self_fx_offset1488||!f.self_fx1484||!f.visual2d8||!f.fade1440||!f.fade1444||!f.position160||!f.initial_position1450||!f.rotation16c||!f.initial_rotation145c){error_="Required same generic Character InitPost field authority";return false;}
 if(*f.called1394)return true;*f.called1394=1;NpcInitPostResponseV1 r;bool player_now{};
 if(!call(0x38bd64,r))return false;if(r.value>=*f.spawn_probability274)return true;
 if(!debug())return false;
 if(!f.starting_waypoint13e8->empty()){
  if(!call(0x34aca0,r,static_cast<std::uint32_t>(*f.room64),0,0,f.starting_waypoint13e8->c_str()))return false;
  const auto handle=r.identity;if(!call(0x33fdc0,r,0,0,handle))return false;
  if(r.identity){if(!call(0x33fee4,r,0,0,handle))return false;if(r.identity&&!call(0x405540,r,0,0,r.identity))return false;}
 }
 if(!call(0x3b3d38,r))return false;if(*f.properties_id13c8!=-1&&!debug())return false;
 const auto property_id=static_cast<std::uint32_t>(static_cast<std::int32_t>(*f.properties_id13c8));
 if(!call(0x3df2a4,r,property_id)||!call(0x3e0810,r,1)||!call(0x3a54d4,r))return false;if(r.text)*f.model290=r.text;
 if(!debug())return false;
 if(!f.properties->base||dh2_character_visual_scale(f.scale120,f.properties->base+12)!=0){error_="Required same source Character scale sheet";failed_=true;return false;}
 if(!call(0x38be5c,r)||!call(0x38ab60,r))return false;
 if(!r.value){if(!call(0xffffff40,r)||!call(0x33ddb4,r))return false;return true;}
 if(!call(0x3bc4d0,r,2)||!call(0x3a2fec,r))return false;
 const auto* ai=r.ai;if(!ai){error_="Required captured actual AiProps row";failed_=true;return false;}
 if(!is_player(player_now))return false;
 if(!player_now&&ai->delayed_load){auto* delayed=f.live_delayed3ec?f.live_delayed3ec(f.lifecycle_context):f.delayed3ec;if(!delayed){error_="Required same delayed3ec";failed_=true;return false;}*delayed=1;}
 else {
  if(!is_player(player_now))return false;
  if(player_now){if(!call(0x36effc,r))return false;if(!r.value){auto* delayed=f.live_delayed3ec?f.live_delayed3ec(f.lifecycle_context):f.delayed3ec;if(!delayed){error_="Required same delayed3ec";failed_=true;return false;}*delayed=1;}else if(!call(0x3cf1f0,r))return false;}
  else if(!call(0x3cf1f0,r))return false;
 }
 if(!call(0x495430,r,static_cast<std::uint32_t>(*f.self_fx_offset1488)+static_cast<std::uint32_t>(ai->self_fx)))return false;*f.self_fx1484=r.identity;
 if(!call(0x3b4738,r)||!debug()||!call(0x3c9f4c,r)||!debug()||!call(0x3b3b00,r)||!debug()||!is_player(player_now))return false;
 if(player_now&&!player())return false;
 if(!f.properties->resolved){error_="Required source FadeIn/FadeOut sheet";failed_=true;return false;}
 *f.fade1440=static_cast<float>(f.properties->resolved[210]);*f.fade1444=static_cast<float>(f.properties->resolved[211]);
 if(!call(0x3a58f4,r,0,0,reinterpret_cast<std::uintptr_t>(f.position160))||!call(0x393db4,r,1,0,reinterpret_cast<std::uintptr_t>(f.initial_position1450)))return false;
 std::memcpy(f.initial_rotation145c,f.rotation16c,3*sizeof(float));
 if(*f.visual2d8&&!call(0x470a54,r,0,0,*f.visual2d8))return false;
 if(!call(0x3a59ac,r,0,1)||!call(0x3b3a70,r))return false;
 auto* delayed=f.live_delayed3ec?f.live_delayed3ec(f.lifecycle_context):f.delayed3ec;if(!delayed){error_="Required live same delayed3ec";failed_=true;return false;}
 if(!*delayed&&!call(0x3ce7c0,r,0))return false;
 return call(0x3d37d0,r)&&debug();
}
}
